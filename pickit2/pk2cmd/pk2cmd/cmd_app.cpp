//                            Software License Agreement
//
// Copyright (c) 2005-2009, Microchip Technology Inc. All rights reserved.
// Portions of this code by Jeff Post, j_post <AT> pacbell <DOT> net
//
// You may use, copy, modify and distribute the Software for use with Microchip
// products only.  If you distribute the Software or its derivatives, the
// Software must have this entire copyright and disclaimer notice prominently
// posted in a location where end users will see it (e.g., installation program,
// program headers, About Box, etc.).  To the maximum extent permitted by law,
// this Software is distributed “AS IS” and WITHOUT ANY WARRANTY INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR
// PARTICULAR PURPOSE, or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS
// LICENSORS BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES OF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION OR
// DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.
//
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "stdlib.h"
#include "cmd_app.h"
#include "string.h"
#include "time.h"
#ifdef WIN32
#include "Windows.h"
#include "conio.h"
#else
#include    <termios.h>
#include    <sys/ioctl.h>
extern bool verbose;
#endif

extern "C"{
	#include "strnatcmp.h"
}

Ccmd_app::Ccmd_app(void)
{
	preserveEEPROM = false;
	hexLoaded = false;
	usingLowVoltageErase = false;
	resetOnExit = false;
	Pk2Operation = true;
	pk2UnitIndex = 0;		
	ReturnCode = OPSUCCESS;
}

Ccmd_app::~Ccmd_app(void)
{
}

void Ccmd_app::PK2_CMD_Entry(int argc, _TCHAR* argv[])
{
	_TCHAR tempString[MAX_PATH] = "";

	processArgvForSpaces(argc, argv);
	argc = nargc;
	argv = nargv;

	// Check for help display requests
	if (checkHelp1(argc, argv))
	{
		return;
	}

	// Load device file
	bool loadDeviceFileFailed = false;
	if (checkDevFilePathOptionB(argc, argv, tempString))
	{ // check for explicit path with -B
#ifdef WIN32
		if (tempString[_tcslen(tempString)-1] != '\\')
			_tcscat_s(tempString, "\\PK2DeviceFile.dat");
		else
			_tcscat_s(tempString, "PK2DeviceFile.dat");
#else
		if (tempString[_tcslen(tempString)-1] != '/')
			_tcscat_s(tempString, "/PK2DeviceFile.dat");
		else
			_tcscat_s(tempString, "PK2DeviceFile.dat");
#endif
	}
	else if (ReturnCode == INVALID_CMDLINE_ARG)
	{ // -B, but bad path
		return;
	}
	else
	{ // no -B, check PATH
		_tsearchenv_s("PK2DeviceFile.dat", "PATH", tempString);
		if (_tcslen(tempString) < 17)
		{
			_tcsncpy_s(tempString, "PK2DeviceFile.dat", 17);
		}
	}
	if (!PicFuncs.ReadDeviceFile(tempString))
	{
		printf("PK2DeviceFile.dat device file not found.\n");
		fflush(stdout);
		ReturnCode = DEVICEFILE_ERROR;
		loadDeviceFileFailed = true;
	}
	else
	{
		char compatMinLevel = DevFileCompatLevelMin;
		if (PicFuncs.DevFile.Info.Compatibility < compatMinLevel)
		{
			printf("PK2DeviceFile.dat device file is too old.\n");
			fflush(stdout);
			ReturnCode = DEVICEFILE_ERROR;
			loadDeviceFileFailed = true;
		}
		if (PicFuncs.DevFile.Info.Compatibility > DevFileCompatLevel)
		{
			printf("PK2DeviceFile.dat device file requires an update of pk2cmd.\n");
			fflush(stdout);
			ReturnCode = DEVICEFILE_ERROR;
			loadDeviceFileFailed = true;
		}
	}

	// Check for help display requests that need the device file.
	if (checkHelp2(argc, argv, loadDeviceFileFailed) || loadDeviceFileFailed)
	{
		return;
	}

	// Check for Pk2Operation
	Pk2Operation = Pk2OperationCheck(argc, argv);

	// Look for PICkit 2
	if (Pk2Operation)
	{
		if (!selectUnitArg(argc, argv))
			return; // just listing units

		if (!findPICkit2(pk2UnitIndex))
		{
			if (ReturnCode == WRONG_OS)
				bootloadArg(argc, argv); // see if -d found
			return;
		}
	}

	// execute commands
	processArgs(argc, argv);

	if (Pk2Operation)
	{
		int status = PicFuncs.ReadPkStatus();
		if ((STATUS_VDD_ERROR & status) > 0)
		{
			printf("VDD Error detected.  Check target for proper connectivity.\n");
			fflush(stdout);
			ReturnCode = VOLTAGE_ERROR;
		}
		else if ((STATUS_VPP_ERROR & status) > 0)
		{
			printf("VPP Error detected.  Check target for proper connectivity.\n");
			fflush(stdout);
			ReturnCode = VOLTAGE_ERROR;
		}
	}
}

void Ccmd_app::ResetAtExit(void)
{
	if (resetOnExit)
	{
		printf("Resetting PICkit 2...\n");
		fflush(stdout);
		PicFuncs.ResetPICkit2(); // must re-enumerate with new UnitID in serial string
	}
}

bool Ccmd_app::Pk2OperationCheck(int argc, _TCHAR* argv[])
{
	int i;
	bool ret = false;

	for (i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{ // these options require PICkit 2 communications.
				case 'A':
				case 'a':
				case 'C':
				case 'c':
				case 'D':
				case 'd':
				case 'E':
				case 'e':
				case 'G':
				case 'g':
				case 'I':
				case 'i':
				case 'M':
				case 'm':
				case 'N':
				case 'n':
				case 'R':
				case 'r':
				case 'S':
				case 's':
				case 'T':
				case 't':
				case 'U':
				case 'u':
				case 'V':
				case 'v':
				case 'W':
				case 'w':
				case 'X':
				case 'x':
				case 'Y':
				case 'y':
				case 'Z':
				case 'z':
					ret = true;
					break;

				case 'P':
				case 'p':
					//if ((argv[i][2]) == 0) 
					//	ret = true; // auto detect
					//if ((((argv[i][2]) == 'F') || ((argv[i][2]) == 'f')) && ((argv[i][3]) != 0))
					//	ret = true; // family detect
					ret = true; // always true so Vdd gets shut off, MCLR is released
					break;

				default:
					break;
			}

			if (ret)
				return true;
		}
	}
	return false;
}

void Ccmd_app::processArgvForSpaces(int argc, _TCHAR* argv[])
{
	int	i, j;

	nargc = 0;

	for (i=0; i < K_MAX_ARGS; i++)
		nargv[i] = NULL;

	for (i=0, j=0; i < argc; i++, j++)
	{
		nargv[j] = (char *) malloc(MAX_PATH);
		strcpy(nargv[j], argv[i]);
		nargc++;

		if (checkSwitch(argv[i]))
		{
			if ((i < (argc - 1)) && (strlen(argv[i]) == 2)) // only append next string if first is just option
			{
				if (!checkSwitch(argv[i + 1]))
				{
					strcat(nargv[j], argv[++i]);
				}
			}
		}
	}

}

void Ccmd_app::processArgs(int argc, _TCHAR* argv[])
{
	int i;
	_TCHAR tempString[MAX_PATH] = "";

	// make sure VDD is off
	if (Pk2Operation)
	{
		PicFuncs.VddOff();
	}

	if (bootloadArg(argc, argv)) // ignore all other commands if -d found
		return;

	if (unitIDArg(argc, argv))	// ignore other commands if -n found
		return;

	// look for part name first
	for (i = 0; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			if ((argv[i][1] == 'P') || (argv[i][1] == 'p'))
				break;
		}
	}
	if (i == argc)
	{ // no part specified
		printf("-P is a required option\n\n");
		fflush(stdout);
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}
	_tcsncpy_s(tempString, &argv[i][2], 28);
	argv[i] = (char *) ""; // blank argument, we've already processed it.
	string2Upper(tempString, MAX_PATH);

	// auto detect?
	if (tempString[0] == 0) 
	{ // no argument, full autodetect
		if (detectAllFamilies(argc, argv))
		{ // found a device
			_tcsncpy_s(tempString, PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName, 28);
			printf("Auto-Detect: Found part %s.\n\n", tempString);
		}
		else if (ReturnCode == INVALID_CMDLINE_ARG)
		{ // arg error
			return;
		}
		else
		{
			printf("Auto-Detect: No known part found.\n\n");
			fflush(stdout);
			ReturnCode = AUTODETECT_FAILED;
			return;
		}
	}
	else if (((tempString[0] == 'f') || (tempString[0] == 'F')) && (tempString[1] == 0))
	{ // print family ID numbers
		printFamilies();
		return;
	}
	else  if (((tempString[0] == 'f') || (tempString[0] == 'F')) && (tempString[1] != 0))
	{ // auto detect family
		if (detectSpecificFamily(&tempString[1], argc, argv))
		{ // found a device
			_tcsncpy_s(tempString, PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName, 28);
			printf("Auto-Detect found part %s.\n\n", tempString);
		}
		else if (ReturnCode == INVALID_CMDLINE_ARG)
		{ // arg error
			return;
		}
		else
		{ // detect failed
			ReturnCode = AUTODETECT_FAILED;
			return;
		}
	}

	// look for the device in the device file - still need to do this on autodetect to properly set up buffers.
	if(!PicFuncs.FindDevice(tempString))
	{
		printf("Could not find device %s.\n\n", tempString);
		fflush(stdout);
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}

	if (!priority1Args(argc, argv, false))
		return;
	if (!checkArgsForBlankCheck(argc, argv))
	{
		printf("-C Blank Check must be done independent of other programming commands.\n");
		fflush(stdout);
		ReturnCode = INVALID_CMDLINE_ARG;
		return;
	}
	if (!priority2Args(argc, argv))
		return;

	if (!priority3Args(argc, argv))
	return;

	if (!priority4Args(argc, argv))
	return;

	if (!delayArg(argc, argv))
	return;

	// unrecognized commands ignored.
}

bool Ccmd_app::detectAllFamilies(int argc, _TCHAR* argv[])
{
	// on auto detect, must run these args first
	if (!priority1Args(argc, argv, true))
		return false;
    for (int searchIndex = 0; searchIndex < PicFuncs.DevFile.Info.NumberFamilies ; searchIndex++)
    {
		for (int searchPriority = 0; searchPriority < PicFuncs.DevFile.Info.NumberFamilies; searchPriority++)
		{
			if ((PicFuncs.DevFile.Families[searchPriority].PartDetect) && (PicFuncs.DevFile.Families[searchPriority].SearchPriority == searchIndex))
			{
				if (PicFuncs.SearchDevice(searchPriority))   
				{
					return true;
				}
			}
		}
    }
    return false; // no supported part found in any family
}

void Ccmd_app::printFamilies(void)
{
	printf("\nAuto-Detectable Part Families:\n\n");
	printf("     ID#  Family\n");
	int familyID = 0;
    for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
    {
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if ((PicFuncs.DevFile.Families[order].FamilyType == index) && PicFuncs.DevFile.Families[order].PartDetect)
			{
				printf("     %2i   %s\n", familyID++, PicFuncs.DevFile.Families[order].FamilyName);
			}
		}
    }
	fflush(stdout);
}

bool Ccmd_app::detectSpecificFamily(_TCHAR* idString, int argc, _TCHAR* argv[])
{
	int familyID = 0;

	if (!getValue((unsigned int*)&familyID, idString))
	{
		printf("-PF Illegal family ID value.\n");
		fflush(stdout);
		ReturnCode = INVALID_CMDLINE_ARG;
		return false;
	}
	// on auto detect, must run these args first
	if (!priority1Args(argc, argv, true))
		return false;

	int idNumber = 0;
    for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
    {
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if ((PicFuncs.DevFile.Families[order].FamilyType == index) && PicFuncs.DevFile.Families[order].PartDetect)
			{
				if (idNumber++ == familyID)
				{
					if (PicFuncs.SearchDevice(order))   
					{
						return true;
					}
					printf("Auto-Detect: No known %s part found.\n\n", PicFuncs.DevFile.Families[order].FamilyName);
					fflush(stdout);
					return false;
				}
			}
		}
    }
	printf("-PF Illegal family ID value.\n");
	fflush(stdout);
	ReturnCode = INVALID_CMDLINE_ARG;
	return false;
}

bool Ccmd_app::bootloadArg(int argc, _TCHAR* argv[])
{
	int i, j;
	_TCHAR tempString[MAX_PATH] = "";
	bool ret;

	for (i = 1; i < argc; i++)
	{
		// -D download OS
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'D') || (argv[i][1] == 'd')))
		{
			PicFuncs.ClosePICkit2Device();
			if ((pk2UnitIndex > 0) || (PicFuncs.DetectPICkit2Device(1, false)))
			{
				printf("\nTo update the PICkit 2 OS, it must be the only unit connected.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true;
			}

			PicFuncs.ClosePICkit2Device();
			PicFuncs.DetectPICkit2Device(0, true);

			_tcsncpy_s(tempString, &argv[i][2], _tcslen(argv[i])-2);
			argv[i] = (char *) "";
			j = 1;
			while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
			{ // check for path with space(s) in it
				_tcscat_s(tempString, " ");
				_tcscat_s(tempString, argv[i+j]);
				argv[i + j++] = (char *) "";
			}
			ret = Pk2BootLoadFuncs.ReadHexAndDownload(tempString, &PicFuncs, pk2UnitIndex);
			if (!ret)
			{
				printf("Error opening hex file.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			ret = Pk2BootLoadFuncs.ReadHexAndVerify(tempString, &PicFuncs);
			if (!ret)
			{
				printf("Error validating OS download.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			ret = PicFuncs.BL_WriteFWLoadedKey();
			if (!ret)
			{
				printf("Error with OS download.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			printf("Resetting PICkit 2...\n");
			fflush(stdout);
			PicFuncs.BL_Reset();
			Sleep(5000);
			if (!PicFuncs.DetectPICkit2Device(pk2UnitIndex, true))
			{
				printf("PICkit 2 failed to reset.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true; // download command found
			}
			printf("OS Update Successful.\n");
			fflush(stdout);
			return true;
		}
	}
	return false; // no bootload command
}

bool Ccmd_app::unitIDArg(int argc, _TCHAR* argv[])
{
	int i, j;
	_TCHAR writeString[MAX_PATH] = "";
	_TCHAR readString[MAX_PATH] = "";
	bool ret;

	for (i = 1; i < argc; i++)
	{
		// -N set Unit ID
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'N') || (argv[i][1] == 'n')))
		{
			_tcsncpy_s(writeString, &argv[i][2], _tcslen(argv[i])-2);
			argv[i] = (char *) "";
			j = 1;
			while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
			{ // check for name with space(s) in it
				_tcscat_s(writeString, " ");
				_tcscat_s(writeString, argv[i+j]);
				argv[i + j++] = (char *) "";
			}
			ret = PicFuncs.UnitIDWrite(writeString);
			if (!ret)
			{
				printf("Error writing Unit ID.\n");
				fflush(stdout);
				ReturnCode = OPFAILURE;
				return true; // unit id command found
			}
			ret = PicFuncs.UnitIDRead(readString);
			if ((_tcslen(writeString) == 0) && !ret)
			{
				printf("Unit ID successfully cleared...\n");
				fflush(stdout);
				resetOnExit = true;
				return true;
			}
			for (j = 0; j < 14; j++)
			{
				if ((writeString[j] != readString[j]) || !ret)
				{
					printf("Error verifying Unit ID.\n");
					fflush(stdout);
					ReturnCode = OPFAILURE;
					return true; // unit id command found
				}
				if (writeString[j] == 0)
					break;
			}
			printf("Unit ID successfully assigned...\n");
			fflush(stdout);
			resetOnExit = true;
			return true;
		}
	}
	return false; // no unit id command
}

bool Ccmd_app::selectUnitArg(int argc, _TCHAR* argv[])
{
	int i, j, k, len;
	bool listFWVer = false;
	_TCHAR unitIDString[MAX_PATH] = "";
	_TCHAR readString[MAX_PATH] = "";
	_TCHAR *pUnitID = 0;

	for (i = 1; i < argc; i++)
	{
		// -S use Unit ID
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'S') || (argv[i][1] == 's')))
		{
			if ((_tcslen(argv[i]) == 3) && (argv[i][2] == '#'))
				listFWVer = true;

			if ((_tcslen(argv[i]) > 2) && !listFWVer)
			{ // find specific unit
				_tcsncpy_s(unitIDString, &argv[i][2], _tcslen(argv[i])-2);
				argv[i] = (char *) "";
				for (j = 0; j < 8; j++)
				{
					if (PicFuncs.DetectPICkit2Device(j, false))
					{
						//ret = PicFuncs.UnitIDRead(readString);

						pUnitID = PicFuncs.GetUnitID();
						_tcsncpy_s(readString, pUnitID, _tcslen(pUnitID));
						if (_tcsncmp(readString, "-", 1) != 0) // UnitID not blank
						{
							k = _tcsncmp(unitIDString, readString, _tcslen(unitIDString));
							if (k == 0)
							{
								pk2UnitIndex = j;
								PicFuncs.ClosePICkit2Device();	// we'll re-open it when we check OS version, etc.
								return true;
							}
						}
						PicFuncs.ClosePICkit2Device();
					}
					else
					{
						if (j == 0)
							printf("\nNo PICkit 2 Units Found...\n");
						else
							printf("\nPICkit 2 with Unit ID '%s' not found.\n", unitIDString);
						fflush(stdout);
						break;
					}
				}				
				return false;
			}
			else
			{ // list all units, max 8
				for (j = 0; j < 8; j++)
				{
					if (PicFuncs.DetectPICkit2Device(j, listFWVer))
					{
						if (j == 0)
						{
							if (listFWVer)
								printf("\nUnit #     Unit ID          OS Firmware\n");
							else
								printf("\nUnit #     Unit ID\n");
						}

						//if ((PicFuncs.FirmwareVersion.major < 2) || (PicFuncs.FirmwareVersion.major == 'v'))
						//	ret = 0;
						//else
						//{
						//	ret = PicFuncs.UnitIDRead(readString);
						//}
							
						pUnitID = PicFuncs.GetUnitID();
						_tcsncpy_s(readString, pUnitID, _tcslen(pUnitID));

						//if (ret)
						//{
						//	len = printf("%d          %s", j, readString);
						//}
						//else
						//{
						//	len = printf("%d          -", j);
						//}
						if (_tcsncmp(readString, "PIC18F2550", 10) == 0)
						{
							if (listFWVer)
								len = printf("%d          -", j); 
							else
								len = printf("%d          <bootloader>", j);
						}
						else
						{
							len = printf("%d          %s", j, readString);
						}

						while (len < 28)
						{
							printf(" ");
							len++;
						}
						
						if (listFWVer)
						{
							if (PicFuncs.FirmwareVersion.major == 'v')
								printf("<bootloader>");
							else
								printf("%d.%02d.%02d",
									PicFuncs.FirmwareVersion.major,
									PicFuncs.FirmwareVersion.minor,
									PicFuncs.FirmwareVersion.dot); 
						}

						printf("\n");
						fflush(stdout);
						PicFuncs.ClosePICkit2Device();
					}
					else
					{
						if (j == 0)
							printf("\nNo PICkit 2 Units Found...\n");
						fflush(stdout);
						break;
					}
				}
				return false;
			}
		}
	}
	return true;
}

int Ccmd_app::getPk2UnitIndex(void)
{
	return pk2UnitIndex;
}

void Ccmd_app::string2Upper(_TCHAR* lcstring, int maxLength)
{
	for (int i = 0; i < maxLength; i++)
	{
		if (*(lcstring + i) == 0)
			break;
		else
			*(lcstring + i) = _totupper(*(lcstring + i));
	}
}

bool Ccmd_app::priority1Args(int argc, _TCHAR* argv[], bool preserveArgs)
{	// returns false if any command has an error.

	// priority 1 args are -A, -F, -J, -Q, -V, -W, -X, -Z
	// These can be processed before any programming communications
	int i, j;
	unsigned int tempi;
	float tempf;
	bool ret = true;

	_TCHAR tempString[MAX_PATH] = "";

	// Get default Vdd & Vpp
	if (preserveArgs)
	{
		PicFuncs.SetVddSetPoint(3.0); // 3 Volts always used for part detect.
	}
	else
	{
		PicFuncs.GetDefaultVdd();
		PicFuncs.GetDefaultVpp();
	}

	for (i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{
				case 'A':
				case 'a':
					// Set VDD voltage
					if (!preserveArgs) // skip during auto-detect
					{
						tempf = (float)_tstof(&argv[i][2]);
						if (tempf > PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].VddMax)
						{
							printf("-A Vdd setpoint exceeds maximum for this device of %.1fV\n", 
									PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].VddMax);
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
							fflush(stdout);
						}
						if (tempf < 2.5)
						{
							printf("-A Vdd setpoint below PICkit 2 minimum of 2.5V\n");
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
							fflush(stdout);
						}
						PicFuncs.SetVddSetPoint(tempf);
						argv[i] = (char *)"";
					}
					break;

				case 'F':
				case 'f':
					if (!preserveArgs) // skip if still looking for a part
					{
						// Hex File Selection
						_tcsncpy_s(tempString, &argv[i][2], _tcslen(argv[i])-2);
						argv[i] = (char *) "";
						j = 1;
						while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
						{ // check for path with space(s) in it
							_tcscat_s(tempString, " ");
							_tcscat_s(tempString, argv[i+j]);
							if (!preserveArgs)
								argv[i + j] = (char *) "";
							j++;
						}
						// Check for BIN file:
						ret = false; // assume not bin file
						j = (int)_tcslen(tempString);
						if (j > 3)
						{ // this is kind of brute force, but avoids a lot of string library calls and another tempstring
							if (tempString[j-1] == ' ') // may have space on the end
							{
								tempString[j-1] = 0; // kill the space
								j--;
							}
							if (((tempString[j-3] == 'b') || (tempString[j-3] == 'B'))
								&& ((tempString[j-2] == 'i') || (tempString[j-2] == 'I'))
								&& ((tempString[j-1] == 'n') || (tempString[j-1] == 'N')))
								ret = true; // BIN file
						}
						if (ret && PicFuncs.FamilyIsEEPROM())
						{ // bin file
							printf("Importing -f file as .BIN\n");
							ret = ImportExportFuncs.ImportBINFile(tempString, &PicFuncs);
						}
						else
						{ // hex file
							ret = ImportExportFuncs.ImportHexFile(tempString, &PicFuncs);
						}
						if (ret)
							hexLoaded = true;
						else
							ReturnCode = INVALID_HEXFILE;
					}
					break;

				case 'J':
				case 'j':
					// Display percentage operation completion
					if ((argv[i][2] == 'n') || (argv[i][2] == 'N'))
						PicFuncs.SetTimerFunctions(true, true); // use newlines
					else
						PicFuncs.SetTimerFunctions(true, false);
					if (!preserveArgs)
						argv[i] = (char *) "";
					break;

				case 'L':
				case 'l':
					// Set ICSP speed
					if (argv[i][2] == 0)
					{ // no specified value - illegal
						printf("-L Invalid value.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else if (getValue(&tempi, &argv[i][2]))
					{
						if (tempi > 16)
							tempi = 16;
						PicFuncs.SetProgrammingSpeedDefault((unsigned char)tempi);
					}
					else
					{ // no specified value - illegal
						printf("-L Invalid value.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					if (!preserveArgs)
						argv[i] = (char *) "";
					break;

				case 'Q':
				case 'q':
					// Disable PE
					PicFuncs.DisablePE33();
					if (!preserveArgs)
						argv[i] = (char *) "";
					break;

				case 'V':
				case 'v':
					// VPP override
					if (preserveArgs)
					{ // cannot be used with part detect
						printf("-V Cannot be used with part auto-detect.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						tempf = (float)_tstof(&argv[i][2]);
						PicFuncs.SetVppSetPoint(tempf);
						argv[i] = (char *) "";
					}
					break;

				case 'W':
				case 'w':
					// External power target
					float vdd, vpp;
					PicFuncs.SetSelfPowered(true);
					PicFuncs.ReadPICkitVoltages(&vdd, &vpp);
					PicFuncs.SetVddSetPoint(vdd);
					if (!preserveArgs)
						argv[i] = (char *) "";
					break;

				case 'X':
				case 'x':
					// VPP first
					if (preserveArgs)
					{ // cannot be used with part detect
						printf("-X Cannot be used with part auto-detect.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgEntryVPPScript == 0)
						{
							printf("-X This part does not support VPP first program mode\n");
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
							fflush(stdout);
						}
						PicFuncs.SetVppFirstEnable(true);	
							argv[i] = (char *) "";
					}
					break;

				case 'Z':
				case 'z':
					for (j = 1; j < argc; j++)
					{
						if ((checkSwitch(argv[j])) && ((argv[j][1] == 'M') || (argv[j][1] == 'm')))
						{
							preserveEEPROM = true;
						}
					}
					if (!preserveEEPROM)
					{
						printf("-Z Preserve EEData must be used in conjunction with the -M program command.\n");
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
						fflush(stdout);
					}
					if (!preserveArgs)
						argv[i] = (char *) "";
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	if (PicFuncs.GetSelfPowered() && PicFuncs.GetVppFirstEnable())
	{
		printf("-W -X VPP first not supported with external power\n");
		fflush(stdout);
		ret = false;
		ReturnCode = INVALID_CMDLINE_ARG;
	}

	return ret;
}

bool Ccmd_app::checkArgsForBlankCheck(int argc, _TCHAR* argv[])
{	// returns false if there is an error.

	// Blank Check (-C) cannot be used with -E, -G, -M, -U, -Y
	bool blankCheck = false;
	bool contradication = false;

	for (int i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{
				case 'C':
				case 'c':
					blankCheck = true;
					break;

				case 'E':
				case 'e':
					contradication = true;
					break;

				case 'G':
				case 'g':
					contradication = true;
					break;

				case 'M':
				case 'm':
					contradication = true;
					break;

				case 'U':
				case 'u':
					contradication = true;
					break;

				case 'Y':
				case 'y':
					contradication = true;
					break;

				default:
					break;
			}
		}
	}	

	return !(blankCheck && contradication);
}

bool Ccmd_app::priority2Args(int argc, _TCHAR* argv[])
{	// returns false if any command has an error.

	// priority 2 args are -C, -U, -E, -M, -Y, -G
	// In the order they will be processed.
	int i, j;
	bool program, eedata, config, userid;
	bool ret = true;

	_TCHAR tempString[MAX_PATH] = "";

	// get current date & time
	_TCHAR stime[128] = "";
	time_t now;
	struct tm today;
	_tzset();
	time(&now);
    _localtime64_s(&today, &now);
	_tcsftime( stime, 128, "%d-%b-%Y, %H:%M:%S", &today );


	// Prep PICkit 2 (set Vdd, vpp, download scripts)
	PicFuncs.PrepPICkit2();

	for (i = 1; i < argc; i++)
	{
		// -C Blank Check
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'C') || (argv[i][1] == 'c')) && ret)
		{
			if (PicFuncs.FamilyIsKeeloq())
			{
				printf("BlankCheck not supported for KEELOQ devices.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsMCP())
			{
				printf("BlankCheck not supported for MCP devices.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.ReadDevice(BLANK_CHECK, true, true, true, true))
			{
				printf("Device is blank\n");
				fflush(stdout);
			}
			else
			{
				printf("%s memory is NOT blank.\n\n", PicFuncs.ReadError.memoryType);
				printMemError();
				ret = false;
			}
		}
	}

	for (i = 1; i < argc; i++)
	{
		// -U Overwrite Cal
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'U') || (argv[i][1] == 'u')) && ret)
		{
			if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].OSSCALSave)
			{
				for (j = 1; j < argc; j++)
				{
					if ((checkSwitch(argv[j])) && ((argv[j][1] == 'M') || (argv[j][1] == 'm')))
					{
						ret = getValue(&PicFuncs.DeviceBuffers->OSCCAL, &argv[i][2]);
						if (ret)
						{
							PicFuncs.OverwriteOSCCAL = true;
						}
						else
						{
							printf("-U Error parsing value.\n");
							fflush(stdout);
							ReturnCode = INVALID_CMDLINE_ARG;
						}
					}
				}
				if (!PicFuncs.OverwriteOSCCAL)
				{
					printf("-U Overwrite OSCCAL must be used in conjunction with the -M program command.\n");
					fflush(stdout);
					ret = false;
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
					printf("-U Overwrite OSCCAL cannot be used with this device.\n");
					fflush(stdout);
					ret = false;
					ReturnCode = INVALID_CMDLINE_ARG;
			}
		}
	}

	for (i = 1; i < argc; i++)
	{
		// -E Erase
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'E') || (argv[i][1] == 'e')) && ret)
		{
			if (PicFuncs.FamilyIsKeeloq())
			{
				printf("Erase not supported for KEELOQ devices.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsMCP())
			{
				printf("Erase not supported for MCP devices.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
			else if (PicFuncs.FamilyIsEEPROM() 
				&& (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[PROTOCOL_CFG] != MICROWIRE_BUS)
				&& (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[PROTOCOL_CFG] != UNIO_BUS))
			{ // Microwire / UNIO have a true "chip erase".  Other devices must write every byte blank.
				printf("Erasing Device...\n");
				fflush(stdout);
				if (!PicFuncs.SerialEEPROMErase())
				{
					ret = false;
					ReturnCode = OPFAILURE;
				}
			}
			else
			{
				printf("Erasing Device...\n");
				fflush(stdout);
				PicFuncs.EraseDevice(true, !preserveEEPROM, &usingLowVoltageErase);
			}
		}
	}

	for (i = 1; i < argc; i++)
	{
		// -M Program 
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'M') || (argv[i][1] == 'm')) && ret)
		{
			if (hexLoaded)
			{
				bool noProgEntryForVerify = false;
				bool verify = true;
				bool argError = true;
								
				if (argv[i][2] == 0)
				{ // no specified region - erase then program all
					if (PicFuncs.FamilyIsEEPROM())
					{
						ret = PicFuncs.EepromWrite(WRITE_EE);
						verify = ret;
						argError = ret;
						if (!ret)
						{
							ReturnCode = PGMVFY_ERROR;
						}
						program = true;
						eedata = false;
						userid = false;
						config = false;
					}
					else
					{
						bool rewriteEE = PicFuncs.EraseDevice(true, !preserveEEPROM, &usingLowVoltageErase);
						program = true;
						eedata = (rewriteEE || !preserveEEPROM);
						userid = true;
						config = true;
						if (PicFuncs.FamilyIsPIC32())
						{
							// Program everything.
							noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
						}
						else
						{
							// program all but configs and verify, as configs may contain code protect
							noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, false, usingLowVoltageErase);
						}
						if (!noProgEntryForVerify)
						{ // if it is true, then configs are in program memory
							ret = PicFuncs.ReadDevice(VERIFY_MEM_SHORT, program, eedata, userid, false);
							verify = ret;
							argError = ret;
							if (ret)
							{ // now program configs
								program = false;
								eedata = false;
								userid = false;
								noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
							}
							else
							{
								ReturnCode = PGMVFY_ERROR;
							}
						}
					}
				}
				else
				{
					program = false;
					eedata = false;
					userid = false;
					config = false;
					for (j = 2; j < (int)_tcslen(argv[i]); j++)
					{
						switch (argv[i][j])
						{
							case 'p':
							case 'P':
								program = true;
								if (PicFuncs.FamilyIsEEPROM())
								{
									ret = PicFuncs.EepromWrite(WRITE_EE);
									argError = ret;
									verify = ret;
								}
								else
								{
									noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
								}
								break;

							case 'e':
							case 'E':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
								{
									if (preserveEEPROM)
									{
										printf("Cannot both program and preserve EEData memory.\n");
										fflush(stdout);
										ReturnCode = INVALID_CMDLINE_ARG;
										ret = false;
									}
									else
									{
										eedata = true;
										noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
									}
								}
								else
									ret = false;
								break;

							case 'i':
							case 'I':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
								{
									userid = true;
									noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
								}
								else
									ret = false;
								break;

							case 'c':
							case 'C':
								// check for devices with config in program memory - can't program seperately.
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
								{
									    int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
																PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
										int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
										if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
										{
											printf("This device has configuration words in Program Memory.\nThey cannot be programmed separately.\n");
											fflush(stdout);
											ReturnCode = INVALID_CMDLINE_ARG;
											ret = false;
										}
										else
										{
											config = true;
											noProgEntryForVerify = PicFuncs.WriteDevice(program, eedata, userid, config, usingLowVoltageErase);
										}
								}
								else
									ret = false;
								break;

							case 'v':
							case 'V':
								{
								_tcsncpy_s(tempString, &argv[i][3], _tcslen(argv[i])-3);
								argv[i] = (char *) "";
								int k = 1;
								if (((i+k) < argc) && (!checkSwitch(argv[i+k])))
								{ // check for space after v
									_tcscat_s(tempString, argv[i+k]);
									argv[i + k++] = (char *) "";
								}
								int vtop = 0;
								int vbot = 0;
								ret = getRange(&vtop, &vbot, tempString);
								if (ret)
								{
									PicFuncs.WriteVector(vtop, vbot);
								}
								}
								break;

							default:
								ret = false;
						}
					}
				}
				if (ret || !argError)
				{
					if (verify)
					{
						if (noProgEntryForVerify)
							ret = PicFuncs.ReadDevice(VERIFY_NOPRG_ENTRY, program, eedata, userid, config);
						else
							ret = PicFuncs.ReadDevice(VERIFY_MEM_SHORT, program, eedata, userid, config);
					}
					printf("PICkit 2 Program Report\n");
					printf("%s\n", stime);
					printf("Device Type: %s\n\n", PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName);
					if (ret)
					{
						printf("Program Succeeded.\n");
						fflush(stdout);
					}
					else
					{
						printf("%s Memory Errors\n\n", PicFuncs.ReadError.memoryType);
						printMemError();
						ReturnCode = PGMVFY_ERROR;
					}
				}
				else
				{
					printf("Invalid Memory region entered for program\n");
					fflush(stdout);
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
				printf("No Image loaded.\nPlease load a hex file before programming or verifying.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
		}
	}

	for (i = 1; i < argc; i++)
	{
		// -Y Verify
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'Y') || (argv[i][1] == 'y')) && ret)
		{
			if (hexLoaded)
			{

				if (PicFuncs.FamilyIsKeeloq())
				{
					printf("Verify not supported for KEELOQ devices.\n");
					fflush(stdout);
					ret = false;
				}
				else if (argv[i][2] == 0)
				{ // no specified region - verify all
					program = true;
					eedata = true;
					userid = true;
					config = true;
				}
				else
				{
					program = false;
					eedata = false;
					userid = false;
					config = false;
					for (j = 2; j < (int)_tcslen(argv[i]); j++)
					{
						switch (argv[i][j])
						{
							case 'p':
							case 'P':
								program = true;
								break;

							case 'e':
							case 'E':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
									eedata = true;
								else
									ret = false;
								break;

							case 'i':
							case 'I':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
									userid = true;
								else
									ret = false;
								break;

							case 'c':
							case 'C':
								if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
								{
									int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
															PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
									int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
									if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
									{
										printf("This device has configuration words in Program Memory.\n");
										fflush(stdout);
										ReturnCode = INVALID_CMDLINE_ARG;
										ret = false;
									}
									else
										config = true;
								}	
								else
									ret = false;
								break;

							default:
								ret = false;
						}
					}
				}
				if (ret)
				{
					ret = PicFuncs.ReadDevice(VERIFY_MEM, program, eedata, userid, config);
					printf("PICkit 2 Verify Report\n");
					printf("%s\n", stime);
					printf("Device Type: %s\n\n", PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].PartName);
					if (ret)
					{
						printf("Verify Succeeded.\n");
						fflush(stdout);
					}
					else
					{
						printf("%s Memory Errors\n\n", PicFuncs.ReadError.memoryType);
						printMemError();
						ReturnCode = PGMVFY_ERROR;
					}
				}
				else
				{
					printf("Invalid Memory region entered for verify\n");
					fflush(stdout);
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
			else
			{
				printf("No Image loaded.\nPlease load a hex file before programming or verifying.\n");
				fflush(stdout);
				ReturnCode = INVALID_CMDLINE_ARG;
				ret = false;
			}
		}
	}

	for (i = 1; i < argc; i++)
	{
		// -G Read 
		if ((checkSwitch(argv[i])) && ((argv[i][1] == 'G') || (argv[i][1] == 'g')) && ret)
		{
			int startAddr = 0;
			int stopAddr = 0;

			if (argv[i][2] == 0)
			{ // no specified type - illegal
				ret = false;
			}
			else if (PicFuncs.FamilyIsKeeloq())
			{
				printf("Read not supported for KEELOQ devices.\n");
				fflush(stdout);
				ret = false;
			}
			else
			{
				switch (argv[i][2])
				{
					case 'f':
					case 'F':
							if (PicFuncs.ReadDevice(READ_MEM, true, true, true, true))
							{
								_tcsncpy_s(tempString, &argv[i][3], _tcslen(argv[i])-3);
								argv[i] = (char *) "";
								j = 1;
								while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
								{ // check for path with space(s) in it
									if (_tcslen(tempString) != 0) // don't add space if it's between "F" and start of filename
										_tcscat_s(tempString, " ");
									_tcscat_s(tempString, argv[i+j]);
									argv[i + j++] = (char *) "";
								}
								// Check for BIN file:
								ret = false; // assume not bin file
								j = (int)_tcslen(tempString);
								if (j > 3)
								{ // this is kind of brute force, but avoids a lot of string library calls and another tempstring
									if (tempString[j-1] == ' ') // may have space on the end
									{
										tempString[j-1] = 0; // kill the space
										j--;
									}
									if (((tempString[j-3] == 'b') || (tempString[j-3] == 'B'))
										&& ((tempString[j-2] == 'i') || (tempString[j-2] == 'I'))
										&& ((tempString[j-1] == 'n') || (tempString[j-1] == 'N')))
										ret = true; // BIN file
								}
								if (ret && PicFuncs.FamilyIsEEPROM())
								{ // BIN file
									printf("Exporting -gf file as .BIN\n");
									ret = ImportExportFuncs.ExportBINFile(tempString, &PicFuncs);
								}
								else
								{ // hex file
									ret = ImportExportFuncs.ExportHexFile(tempString, &PicFuncs);
								}
								if (ret)
								{
									printf("Read successfully.\n");
									fflush(stdout);
									hexLoaded = true;
								}
								else
									ReturnCode = FILE_OPEN_ERROR;
							}
							else
							{
								printf("\nRead Error\n");
								fflush(stdout);
								ReturnCode = OPFAILURE;
							}
						break;

					case 'p':
					case 'P':
						// Read Program mem range to screen
						_tcsncpy_s(tempString, &argv[i][3], _tcslen(argv[i])-3);
						argv[i] = (char *) "";
						j = 1;
						if (((i+j) < argc) && (!checkSwitch(argv[i+j])))
						{ // check for space after p
							_tcscat_s(tempString, argv[i+j]);
							argv[i + j++] = (char *) "";
						}
						ret = getRange(&startAddr, &stopAddr, tempString);
						if (ret)
						{
							if (PicFuncs.ReadDevice(READ_MEM, true, false, false, false))
							{
								printf("Read successfully.\n");
								fflush(stdout);
								printProgramRange(startAddr, stopAddr);
							}
							else
							{
								printf("\nRead Error\n");
								fflush(stdout);
								ReturnCode = OPFAILURE;
							}
						}
						break;

					case 'e':
					case 'E':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem > 0)
						{
							// Read EE mem range to screen
							_tcsncpy_s(tempString, &argv[i][3], _tcslen(argv[i])-3);
							argv[i] = (char *) "";
							j = 1;
							if (((i+j) < argc) && (!checkSwitch(argv[i+j])))
							{ // check for space after p
								_tcscat_s(tempString, argv[i+j]);
								argv[i + j++] = (char *) "";
							}
							ret = getRange(&startAddr, &stopAddr, tempString);
							if (ret)
							{
								if (PicFuncs.ReadDevice(READ_MEM, false, true, false, false))
								{
									printf("Read successfully.\n");
									fflush(stdout);
									printEEDataRange(startAddr, stopAddr);
								}
								else
								{
									printf("\nRead Error\n");
									fflush(stdout);
									ReturnCode = OPFAILURE;
								}
							}
						}
						else
							ret = false;
						break;

					case 'i':
					case 'I':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords > 0)
						{
							// Read User IDs to screen
							if (PicFuncs.ReadDevice(READ_MEM, false, false, true, false))
								{
									printf("Read successfully.\n");
									fflush(stdout);
									printUserIDs();
								}
								else
								{
									printf("\nRead Error\n");
									fflush(stdout);
									ReturnCode = OPFAILURE;
								}
						}
						else
							ret = false;
						break;

					case 'c':
					case 'C':
						if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords > 0)
						{

						    int configLocation = (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigAddr /
													PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].ProgMemHexBytes;
							int configWords = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords;
							if ((configLocation < (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem) && (configWords > 0))
							{
								printf("This device has configuration words in Program Memory.\n");
								fflush(stdout);
								ReturnCode = INVALID_CMDLINE_ARG;
								ret = false;
							}
							else
							{
								// Read Configuration to screen
								if (PicFuncs.ReadDevice(READ_MEM, false, false, false, true))
									{
										printf("Read successfully.\n");
										fflush(stdout);
										printConfiguration();
									}
									else
									{
										printf("\nRead Error\n");
										fflush(stdout);
										ReturnCode = OPFAILURE;
									}
							}
						}
						else
							ret = false;
						break;

					case 'v':
					case 'V':
						{
						unsigned int vector = PicFuncs.ReadVector();
						printf("%8X\n", vector);
						fflush(stdout);
					    break;
						}

					default:
						ret = false;
				}
			}
			if (!ret)
			{
				if (ReturnCode != FILE_OPEN_ERROR)
				{
					printf("Illegal read parameter entered.\n");
					fflush(stdout);
					ReturnCode = INVALID_CMDLINE_ARG;
				}
			}
		}
	}	

	return ret;
}

bool Ccmd_app::priority3Args(int argc, _TCHAR* argv[])
{	// returns false if any command has an error.

	// priority 3 args are -I, -K
	// These can be processed after any programming communications
	int i, j;
	bool ret = true;

	for (i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{
				case 'I':
				case 'i':
					// Display Device ID
					if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].PartDetect)
					{
						int deviceID = PicFuncs.ReadDeviceID();
						printf("Device ID = %04X\n", deviceID);
						printf("Revision  = %04X\n", PicFuncs.GetDeviceRevision());
						// Display the device name matching the ID, not necessary the active device.
						if (deviceID == 0)
						{
							printf("Device Name = <no device>\n");
						}
						else
						{
							for (j = 1; j < PicFuncs.DevFile.Info.NumberParts; j++)
							{
								if ((PicFuncs.DevFile.PartsList[j].Family == PicFuncs.ActiveFamily) && (deviceID == (int)PicFuncs.DevFile.PartsList[j].DeviceID) && PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].PartDetect)
								{
									if (j == PicFuncs.ActivePart)
									{
										printf("Device Name = %s\n", PicFuncs.DevFile.PartsList[j].PartName);
									}
									else
									{
										printf("Device Name = %s   !WARNING! -P device mismatch\n", PicFuncs.DevFile.PartsList[j].PartName);
									}
									break;
								}
							}
							if (j == PicFuncs.DevFile.Info.NumberParts)
							{ // no matching device found.
								printf("Device Name = <unknown device>\n");
							}
						}
					}
					else
					{
						printf("This device does not have a Device ID.\n");
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					fflush(stdout);
					break;

				case 'K':
				case 'k':
					// Calculate Checksum
					if (hexLoaded)
					{
						printf("Checksum = %04X\n", PicFuncs.ComputeChecksum());
					}
					else
					{
						printf("The checksum can only be calculated when a hex file is loaded or written.\n");
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					fflush(stdout);
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

bool Ccmd_app::priority4Args(int argc, _TCHAR* argv[])
{	// returns false if any command has an error.

	// priority 4 args are -R, -T
	// These can be processed after any programming communications
	int i;
	bool ret = true;

	if (Pk2Operation)
	{
		PicFuncs.SetMCLR(true); // ensure /MCLR asserted.
		if (!PicFuncs.GetSelfPowered())
			PicFuncs.VddOff();      // ensure VDD off if no -T
	}

	for (i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{
				case 'R':
				case 'r':
					// Release /MCLR
					PicFuncs.SetMCLR(false);
					break;

				case 'T':
				case 't':
					// Power Target
					if (PicFuncs.GetSelfPowered())
					{
						printf("-W -T Cannot power an externally powered target.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
						PicFuncs.VddOn();
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

bool Ccmd_app::delayArg(int argc, _TCHAR* argv[])
{	// returns false if command has an error.

	// delay arg is -H
	// This must be processed last
	int i;
	unsigned int seconds;
	bool ret = true;
#ifndef WIN32
	struct termios	tios;
#endif

	for (i = 1; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			switch(argv[i][1])
			{
				case 'H':
				case 'h':
					// Delay before exit
					if (argv[i][2] == 0)
					{ // no specified value - illegal
						printf("-H Invalid value.\n");
						fflush(stdout);
						ret = false;
						ReturnCode = INVALID_CMDLINE_ARG;
					}
					else
					{
						if ((argv[i][2] == 'K') || (argv[i][2] == 'k'))
						{
							printf("\nPress any key to exit.\n");
							fflush(stdout);
#ifdef WIN32
							while(_kbhit() == 0) {};
#else
							tcgetattr(0, &tios);
							tios.c_lflag &= (~(ICANON | ECHO));
							tcsetattr(0, TCSANOW, &tios);
							getc(stdin);
							tios.c_lflag |= (ICANON | ECHO);
							tcsetattr(0, TCSANOW, &tios);
#endif
						}
						else if (getValue(&seconds, &argv[i][2]))
						{
							if (seconds == 0)
							{ // bad value
								printf("-H Invalid value.\n");
								fflush(stdout);
								ret = false;
								ReturnCode = INVALID_CMDLINE_ARG;
							}
							else
							{
								printf("\nDelaying %d seconds before exit.\n", seconds);
								fflush(stdout);
								PicFuncs.DelaySeconds(seconds);
							}
						}
						else 
						{ // bad value
							printf("-H Invalid value.\n");
							fflush(stdout);
							ret = false;
							ReturnCode = INVALID_CMDLINE_ARG;
						}
					}
					break;

				default:
					break;
			}
			if (!ret)
				break;	// stop on first error.
		}
	}	
	return ret;
}

void Ccmd_app::printProgramRange(int startAddr, int stopAddr)
{
	int addrInc = PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].AddressIncrement;
	int startWord = startAddr / addrInc;
	int stopWord = stopAddr / addrInc;
	int col;

	if (stopWord < startWord)
		stopWord = startWord;

	if (stopWord >= (int)PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem)
		stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ProgramMem - 1;

	printf("\nProgram Memory");

	do
	{ // address loop
		printf("\n%08X ", startWord * addrInc);

		col = 0;
		if (PicFuncs.FamilyIsEEPROM())
		{
			if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].BlankValue > 0xFF)
			{
				do
				{ // data columns loop
					printf("%04X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
					col++;
				} while ((startWord <= stopWord) && (col < 8));
			}
			else
			{
				do
				{ // data columns loop
					printf("%02X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
					col++;
				} while ((startWord <= stopWord) && (col < 16));
			}
		}
		else
		{
			do
			{ // data columns loop
				printf("%06X  ", PicFuncs.DeviceBuffers->ProgramMemory[startWord++]);
				col++;
			} while ((startWord <= stopWord) && (col < 8));
		}

	} while (startWord <= stopWord);
	printf("\n");
	fflush(stdout);
}

void Ccmd_app::printEEDataRange(int startAddr, int stopAddr)
{
	int addrInc = PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].EEMemAddressIncrement;
	int startWord = startAddr / addrInc;
	int stopWord = stopAddr / addrInc;
	int col;

	if (stopWord < startWord)
		stopWord = startWord;

	if (stopWord >= PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem)
		stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].EEMem -1;

	printf("\nEEData Memory");

	do
	{ // address loop
		printf("\n%04X ", startWord * addrInc);

		col = 0;
		do
		{ // data columns loop
			if (addrInc > 1)
				printf("%04X  ", PicFuncs.DeviceBuffers->EEPromMemory[startWord++]);
			else
				printf("%02X  ", PicFuncs.DeviceBuffers->EEPromMemory[startWord++]);
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	printf("\n");
	fflush(stdout);
}

void Ccmd_app::printUserIDs(void)
{
	int startWord = 0;
	int stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].UserIDWords -1;
	int col;

	printf("\nID Memory\n");

	do
	{ // address loop
		printf("\n");
		col = 0;
		do
		{ // data columns loop
			if (PicFuncs.DevFile.Families[PicFuncs.ActiveFamily].BlankValue > 0xFFFF)
				printf("%06X  ", PicFuncs.DeviceBuffers->UserIDs[startWord++]);
			else
				printf("%04X  ", PicFuncs.DeviceBuffers->UserIDs[startWord++]);
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	printf("\n");
	fflush(stdout);
}

void Ccmd_app::printConfiguration(void)
{
	int startWord = 0;
	int stopWord = PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigWords -1;
	int col;

	printf("\nConfiguration Memory\n");

	do
	{ // address loop
		printf("\n");
		col = 0;
		do
		{ // data columns loop
			if (PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].BandGapMask == 0)
				printf("%04X  ", (PicFuncs.DeviceBuffers->ConfigWords[startWord] 
								& PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[startWord]));
			else
				printf("%04X  ", (PicFuncs.DeviceBuffers->ConfigWords[startWord] 
								& PicFuncs.DevFile.PartsList[PicFuncs.ActivePart].ConfigMasks[startWord])
								| PicFuncs.DeviceBuffers->BandGap);
			startWord++;
			col++;
		} while ((startWord <= stopWord) && (col < 8));

	} while (startWord <= stopWord);
	printf("\n");
	fflush(stdout);
}

bool Ccmd_app::getRange(int* start, int* stop, _TCHAR* str_range)
{
	int i, j;
	_TCHAR temps[8] = "";

	if (*str_range == 0)
	{ // no range, return (use defaults)
		return true;
	}

	// get start address
	for (i = 0; i < 9; i++)
	{
		if (*(str_range + i) == '-')
		{
			temps[i] = 0;
			break;
		}
		else if (*(str_range + i) == 0)
		 // unexpected end of string
			return false;
		else
			temps[i] = *(str_range + i);			
	}

	if ((i >= 9) || ( i == 0))
	// more than 8 character address or no address
		return false;

	*start = ImportExportFuncs.ParseHex(temps, i++);

	// get stop address
	for (j = 0; j < 9; j++)
	{
		if (*(str_range + i + j) == 0)
		 // end of string
		{
			temps[j] = 0;
			break;
		}
		else
			temps[j] = *(str_range + i + j);			
	}

	if ((j >= 9) || (j ==0))
	// more than 8 character address or no address
		return false;	

	*stop = ImportExportFuncs.ParseHex(temps, j++);
	if (*start <= *stop)
		return true;
	else
		return false;
}

bool Ccmd_app::getValue(unsigned int* value, _TCHAR* str_value)
{
	int i;
	_TCHAR temps[8] = "";

	if (*str_value == 0)
	{ // no value, return error
		return false;
	}

	// get value
	for (i = 0; i < 9; i++)
	{
		if (*(str_value + i) == 0)
		{
			temps[i] = 0;
			break;
		}
		else
			temps[i] = *(str_value + i);			
	}

	if ((i >= 9) || ( i == 0))
	// more than 8 character value or no value
		return false;

	*value = ImportExportFuncs.ParseHex(temps, i++);

	return true;
}

bool Ccmd_app::checkSwitch(_TCHAR* argv)
{
	return ((argv[0] == '-') || (argv[0] == '/'));
}

bool Ccmd_app::findPICkit2(int unitIndex)
{
	unsigned char dot_min = PicFuncs.FW_DOT_MIN;

	if (PicFuncs.DetectPICkit2Device(unitIndex, true))
	{
		if ((PicFuncs.FirmwareVersion.major >= PicFuncs.FW_MAJ_MIN)
			&& (PicFuncs.FirmwareVersion.minor >= PicFuncs.FW_MNR_MIN)
			&& (PicFuncs.FirmwareVersion.dot >= dot_min))
		{
			return true;
		}
		printf("PICkit 2 found with Operating System v%d.%02d.%02d\n", PicFuncs.FirmwareVersion.major, 
									PicFuncs.FirmwareVersion.minor, PicFuncs.FirmwareVersion.dot);
		printf("Use -D to download minimum required OS v%d.%02d.%02d or later\n", PicFuncs.FW_MAJ_MIN, 
									PicFuncs.FW_MNR_MIN, PicFuncs.FW_DOT_MIN);
		fflush(stdout);
		ReturnCode = WRONG_OS;
	}
	else
	{
		printf("No PICkit 2 found.\n");
		fflush(stdout);
		ReturnCode = NO_PROGRAMMER;
	}
	return false;
}

void Ccmd_app::printMemError(void)
{
	if (!PicFuncs.FamilyIsPIC32() && !PicFuncs.useProgExec33())
	{
		printf("Address   Good     Bad\n");
		printf("%06X    %06X   %06X\n", PicFuncs.ReadError.address, PicFuncs.ReadError.expected, PicFuncs.ReadError.read);
	}
	fflush(stdout);
}

bool Ccmd_app::checkDevFilePathOptionB(int argc, _TCHAR* argv[], _TCHAR* path_string)
{
	_TCHAR path_temp[MAX_PATH];

	int i;
	// look for 'B' option. 
	for (i = 0; i < argc; i++)
	{
		if (checkSwitch(argv[i]))
		{
			if ((argv[i][1] == 'b') || (argv[i][1] == 'B'))
				break;
		}
	}
	if (i == argc)
		return false; // -b not found
	if (argv[i][2] == 0)
	{
		printf("-B No path given\n");
		ReturnCode = INVALID_CMDLINE_ARG;
		fflush(stdout);
		return false;
	}

	// Get path to device file:
	_tcsncpy_s(path_temp, &argv[i][2], _tcslen(argv[i])-2);
	argv[i] = (char *) "";
	int j = 1;
	while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
	{ // check for path with space(s) in it
		_tcscat_s(path_temp, " ");
		_tcscat_s(path_temp, argv[i+j]);
		argv[i + j++] = (char *) "";
	}
	i = 0;
	do
	{
		path_string[i] = path_temp[i];
	}while (path_string[i++] != 0);
	return true;
}

bool Ccmd_app::checkHelp1(int argc, _TCHAR* argv[])
{ // Helps that don't need the device file.
	int i;

	// if no arguments, display main help screen
	if (argc == 1)
	{
		displayHelp();
		return true;
	}

	// look for '?' in all arguments.  Display help for first found
	for (i = 0; i < argc; i++)
	{
		if (_tcschr(argv[i], '?'))
			break;
	}
	
	if (i == argc) // none found
		return false;

	if (checkSwitch(argv[i]))
	{
		switch (argv[i][1])
		{
			case '?':
				if (_tcslen(argv[i]) > 2)
				{
					if ((argv[i][2] == 'e') || (argv[i][2] == 'E'))
					{
						printf ("PK2CMD return codes:\n");
						printf ("Value   Code                    Notes\n");
						printf ("-----   ----                    -----\n");
						printf ("0       OPSUCCESS              -Returned if all selected operations complete\n");
						printf ("                                successfully.\n");
						printf ("5       VOLTAGE_ERROR          -A Vdd and/or Vpp voltage error was detected.\n");
						printf ("                                This could be due to PICkit 2 being \n");
						printf ("                                improperly connected to a part, incorrect\n");
						printf ("                                part selection, or interference from other\n");
						printf ("                                circuitry on the target board.\n");
						printf ("7       OPFAILURE              -Returned if an operation fails and a more\n");
						printf ("                                specific error does not exist.\n");
						printf ("10      NO_PROGRAMMER          -The PK2CMD executable is unable to find a\n");
						printf ("                                connected PICkit 2 programmer.\n");
						printf ("11      WRONG_OS                -Returned if the OS firmware must be updated\n");
						printf ("                                before being used with this version of\n");
						printf ("                                PK2CMD.\n");
						printf ("15      FILE_OPEN_ERROR        -Returned if a file specified for reading to\n");
						printf ("                                (-gf...) cannot be opened for writing.\n");
						printf ("24      DEVICEFILE_ERROR       -The PK2CMD executable cannot find the device\n");
						printf ("                                file PK2DeviceFile.dat or the device file\n");
						printf ("                                may be corrupted.\n");
						printf ("28      UPDGRADE_ERROR         -Returned when an OS firmware upgade (-d...)\n");
						printf ("                                fails.\n");
						printf ("34      PGMVFY_ERROR           -Returned if a program or verify operation\n");
						printf ("                                fails.\n");
						printf ("36      INVALID_CMDLINE_ARG    -A syntax error in a command line argument\n");
						printf ("                                was detected, an invalid combination of \n");
						printf ("                                operations was entered, an invalid value was\n");
						printf ("                                entered, or a memory region was selected\n");
						printf ("                                that is not supported by the current device.\n");
						printf ("37      INVALID_HEXFILE        -Error opening or loading a specified hex\n");
						printf ("                                file (-f...).\n");
						printf ("39      AUTODETECT_FAILED       A part autodetect operation failed to find\n");
						printf ("                                a known part.\n\n");
					}
					else if ((argv[i][2] == 'l') || (argv[i][2] == 'L'))
					{
						displayLicense();
					}
					else
					{
						//displayHelp();
						return false; // check later for devfile dependent helps
					}
				}
				else
				{
					displayHelp();
				}
				break;

			case 'a':
			case 'A':
				printf("Specifies the VDD voltage that the device is programmed at.  The value\n");
				printf("entered must be less than the allowed maximum of the device and 5.0 Volts\n");
				printf("(whichever is less), and greater than the allowed minimum of the device and\n");
				printf("2.5 Volts (whichever is greater).  A default voltage for the device will be\n");
				printf("used if this command is not specified.\n\n");
				printf("The parameter for this command is the floating point value of the desired\n");
				printf("VDD voltage.\n\n");
				printf("Syntax Example -a4.5\n");
				break;

			case 'b':
			case 'B':
				printf("Specifies the path to the device file PK2DeviceFile.dat.  By default, the\n");
				printf("directory from which the executable is searched first, then the PATH\n");
				printf("environment variable.  This option can be used to explicity specify the\n");
				printf("path to the device file.\n\n");
				printf("The parameter for this command is the complete file path to\n");
				printf("PK2DeviceFile.dat, not including the filename.\n\n");
				printf("Syntax Example -fc:\\pickit_2\\pk2cmd_dir\n");
				break;

			case 'c':
			case 'C':
				printf("Checks to see if the device is blank or not. Each memory region (Program,\n");
				printf("EEPROM, Configuration, and User ID memory) will be checked, and a message\n");
				printf("indicating whether or not the device is blank, will be displayed. If the\n");
				printf("device is not blank, the memory region and location of the first error\n");
				printf("will be displayed.\n\n");
				printf("This command takes no parameters.\n\n");
				printf("Syntax Example -c\n");
				break;

			case 'd':
			case 'D':
				printf("Upgrades the firmware on the programmer. This command must be done\n");
				printf("independently of any other commands.\n\n");
				printf("The parameter for this command is the complete file path to the .hex\n");
				printf("file to be downloaded.\n\n");
				printf("Syntax Example -dc:\\filepath\\PK2V021000.hex\n");
				break;

			case 'e':
			case 'E':
				printf("Erases the device.  A warning will be issued if the device can\n");
				printf("only be bulk erased and VDD is below the bulk erase voltage.\n\n");
				printf("This command takes no parameters.\n\n");
				printf("Syntax Example -e\n");
				break;

			case 'f':
			case 'F':
				printf("Loads a hex file to the programmer. The device will not actually be\n");
				printf("programmed with the contents of the transferred hex file unless the\n");
				printf("program command (-m) is also issued.\n\n");
				printf("Binary format files are also supported for serial EEPROM devices only.\n");
				printf("To load a binary file, the filename must end in BIN, ex: myfile.bin\n\n");
				printf("The parameter for this command is the complete file path to the hex\n");
				printf("file to be loaded\n\n");
				printf("Syntax Example -fc:\\filepath\\myfile.hex\n");
				break;

			case 'g':
			case 'G':
				printf("Reads the device and outputs it to either the screen or a hexfile\n");
				printf("based on the type of read performed. The command must be immediately\n");
				printf("followed by the type of read, which can be one of the following:\n");
				printf("     f = Read into hex file. This command must be immediately followed\n");
				printf("         by the complete file path and name of the file to be created.\n");
				printf("         Serial EEPROMs only may read into a binary file.  A binary file\n");
				printf("         will be created if the filename ends in BIN, ex: myfile.bin\n");
				printf("     p = Read program memory and output the result to the screen. This\n");
				printf("         command must be immediately followed by the hex address range\n");
				printf("         to be read, which must be in the form of x-y, where x = start\n");
				printf("         address and y = end address.\n");
				printf("     e = Read EEData memory and output the result to the screen. This\n");
				printf("         command must be immediately followed by the hex address range\n");
				printf("         to be read, which must be in the form of x-y, where x = start\n");
				printf("         address and y = end address.\n");
				printf("     i = Read User ID memory and output the result to the screen. No\n");
				printf("         further parameters are required for this command.\n");
				printf("     c = Read Configuration memory and output the result to the screen.\n");
				printf("         No further parameters are required for this command.\n");
				printf("Multiple types of read commands can be included in the same command line.\n");
				printf("NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'\n");
				printf("\n");
				printf("Syntax Examples -gfc:\\filepath\\myfile\n");
				printf("                -gp100-200\n");
				printf("                -gi -ge0-40 -gc\n");
				break;

			case 'h':
			case 'H':
				printf("If this switch is included, PK2CMD will delay before exiting.  If the value \n");
				printf("is set to 'K', then PK2CMD will wait for a keypress before exiting. If the \n");
				printf("value is set to a number from 1 to 9, then it will delay the given number\n");
				printf("of seconds before exiting.\n");
				printf("\n");
				printf("The parameter for this command is the number of seconds (max = 9) to delay\n");
				printf("before exiting.  Parameter K will cause it to wait for a keypress.\n");
				printf("\n");
				printf("Syntax Examples -h3\n");
				printf("                -hk\n");
				break;

			case 'i':
			case 'I':
				printf("Reads and displays the value in the Device ID location of the device,\n");
				printf("as well as the silicon revision code.\n");
				printf("\n");
				printf("This will also display the device name that matches the returned Device ID,\n");
				printf("and warn if the Device ID does not match the device specified using the -p\n");
				printf("command.\n");
				printf("\n");
				printf("This command takes no parameters.\n");
				printf("\n");
				printf("Syntax Example -i\n");
				break;

			case 'j':
			case 'J':
				printf("This switch will display a percentage completion for programming operations\n");
				printf("instead of the rotating slash.  If the switch is followed by the optional\n");
				printf("parameter 'n', then each percent update is printed on a newline.  This option\n");
				printf("is intended for GUI interfaces needing a newline to update the display.\n");
				printf("\n");
				printf("The optional parameter for this command, N, will print percentage updates\n");
				printf("on a new line.\n");
				printf("\n");
				printf("Syntax Examples -j\n");
				printf("                -jn\n");
				break;

			case 'l':
			case 'L':
				printf("Sets the ICSP clock (PGC) period to the given value, which must be a value\n");
				printf("between 1 and 16.  The value specifies the clock period in microseconds.\n");
				printf("The default value is 1, which gives a period of 1us and an ICSP clock rate\n");
				printf("of 1 MHz.  A value of 2 gives a period of 2us and a clock rate of 500 kHz.\n");
				printf("Slowing down the programming clock can help resolve programming issues with\n");
				printf("heavily loaded PGx lines and long programming cables.  A value of 4 usually\n");
				printf("resolves most such issues, but programming takes longer.\n");
				printf("\n");
				printf("The parameter for this command is a decimal value between 1 and 16 inclusive.\n");
				printf("\n");
				printf("Syntax Example -l4\n");
				break;

			case 'k':
			case 'K':
				printf("Displays the checksum of a loaded hexfile. This command must be\n");
				printf("accompanied by the Hex File Selection command (-f).\n");
				printf("\n");
				printf("This command takes no parameters.\n");
				printf("\n");
				printf("Syntax Example -k\n");
				break;

			case 'm':
			case 'M':
				printf("Programs the device with the contents of the loaded hex file.\n");
				printf("The entire device can be programmed, or just selected memory regions. If one\n");
				printf("or more selected regions are to be programmed, the program command must be\n");
				printf("immediately followed by a memory region parameter. Valid parameters are:\n");
				printf("     P - program memory\n");
				printf("     E - EEPROM\n");
				printf("     I - ID Memory\n");
				printf("     C - Configuration Memory\n");
				printf("If no memory region parameter is entered, the entire device will be erased and\n");
				printf("then programmed. Otherwise only the selected memory regions will be programmed\n");
				printf("without being first erased. Multiple program commands may be entered on one \n");
				printf("command line.\"Program Succeeded\" will be displayed if the operation is \n");
				printf("successful, otherwise the first address of the first memory region where \n");
				printf("programming failed will be displayed along with a description of the failure.\n");
				printf("NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'\n");
				printf("\n");
				printf("This command may be used without parameters or with a memory region.\n");
				printf("\n");
				printf("Syntax Examples -m\n");
				printf("                -mP -mI -mE\n");
				break;

			case 'n':
			case 'N':
				printf("Assigns the given string to the PICkit 2 unit as the Unit ID.  The Unit ID is\n");
				printf("useful in uniquely identifying a PICkit 2 unit.  When multiple PICkit 2 units\n");
				printf("are connected to a PC, a specific PICkit 2 may be selected using the -S\n");
				printf("option with the Unit ID. \n");
				printf("\n");
				printf("To assign a Unit ID to a PICkit 2, connect only that one unit to the PC and\n");
				printf("use this option.  To remove a Unit ID, do not include a string after the -N\n");
				printf("option.  A Unit ID may contain 14 characters maximum.  The Unit ID is stored\n");
				printf("in non-volatile memory in the PICkit 2 unit itself, and remains assigned\n");
				printf("changed by a user.\n");
				printf("\n");
				printf("Syntax Examples -nLab1B   (Set Unit ID = 'Lab1B')\n");
				printf("                -n        (clear Unit ID)\n");
				break;

			case 'p':
			case 'P':
				printf("There are three ways to use this option:\n");
				printf("  1 : -P<part>\n");
				printf("      Specify the part number of the device explicitly.  This is the\n");
				printf("      recommended use.  Example: -pPIC16F887\n");
				printf("  2 : -PF<id>\n");
				printf("      Auto-Detect a target part connected to PICkit 2 within a given family.\n");
                printf("      Use '-pf' for a list of auto-detectable families and their family ID\n");
				printf("      number.  Not all part families support detection.  No programming \n");
				printf("      operations are performed when -PF is used without an ID parameter.\n");
				printf("      Use '-pf<id>' to auto-detect a part within a given family using\n");
				printf("      the family ID from the listing.  Example: -pf2\n");
				printf("  3 : -P\n");
				printf("      Auto-Detect any part in all auto-detectable families when -p is\n");
				printf("      is used with no parameters.  Example: -p\n");
				printf("\n");
				printf("The -V and -X options may NOT be used with any form of auto-detect.\n");
				printf("During auto-detect, VDD is ALWAYS 3.0 Volts unless -W is used.  After a part\n");
				printf("is detected, the device VDD default or -A voltage is used for remaining\n");
                printf("operations.\n");
				printf("\n");
				printf("Auto-detecting can be slower than explicitly specifying the part name.\n");
                printf("\n");
				printf("WARNING: SOME DEVICE FAMILIES USE A VPP VOLTAGE OF 12 VOLTS ON THE MCLR\n");
				printf("PIN.  THIS VOLTAGE MAY DAMAGE DEVICES FROM OTHER FAMILIES.  NEVER USE\n");
				printf("AN AUTODETECT OPTION ON A TARGET WITHOUT A KNOWN GOOD PROGRAMMING\n");
				printf("CONNECTION.  IT IS SAFER TO AUTO-DETECT WITHIN A GIVEN FAMILY (-PF) THAN\n");
				printf("WITH ALL DETECTABLE FAMILIES.\n");
				printf("\n");
				printf("Auto-detecting in all families goes through a special sequence of searching\n");
				printf("each family to prevent placing damaging voltages on parts.  However, if a\n");
				printf("programming connection problem prevents a part from being found, it may be\n");
				printf("exposed to damaging high voltages as other families are searched.\n");
				printf("\n");
				printf("PK2CMD -?P may be used to list all supported devices and their families.\n");
				printf("PK2CMD -?P<str> may be used to list only devices matching the search string.\n");
				break;

			case 'q':
			case 'Q':
				printf("Disables use of a Programming Executive (PE) for PIC24 or dsPIC33 devices.\n");
				printf("Low-level ICSP is used instead (as in prior versions of PK2CMD).\n");
				printf("\n");
				printf("Using the PE results in much faster programming operations, and implements\n");
				printf("the Device ID Corruption workaround for PIC24H/dsPIC33 devices.  However,\n");
				printf("Blank Check, Programming, and Verify operations will not provide the address\n");
				printf("and data for failing locations for PIC24H/dsPIC33 as the PE only returns a\n");
				printf("Good/Bad response.  Disable the PE for address and data information.\n");
				printf("\n");
				printf("The Programming Executive (PE) for PIC24H and dsPIC33F parts may fail on\n");
				printf("certain programming ports of certain 44-Pin devices. Known problems exist\n");
				printf("with using the PGC3/PGD3 port on the following devices:\n");
				printf("PIC24HJ16GP304, PIC24HJ32GP204\n");
				printf("dsPIC33FJ16GP304, dsPIC33FJ32GP204, dsPIC33FJ16MC304, dsPIC33FJ32MC204\n");
				printf("\n");
				printf("Syntax Example -q\n");
				break;

			case 'r':
			case 'R':
				printf("Releases (3-states) the PICkit 2 /MCLR pin after programming operations\n");
				printf("complete.  If not specified, then /MCLR is asserted (driven low).\n");
				printf("\n");
				printf("There are no parameters for this command.\n");
				printf("\n");
				printf("Syntax Example -r\n");
				break;

			case 's':
			case 'S':
				printf("When more than one PICkit 2 unit is attached to a PC, this option allows\n");
				printf("a specific unit to be selected using its Unit ID.  The Unit ID is assigned\n");
				printf("with the -N option.\n");
				printf("\n");
				printf("When -S is used without an argument, all attached PICkit 2 units and their\n");
				printf("Unit IDs will be listed.  Units that do not have a Unit ID assigned will\n");
				printf("show a dash (-) in the Unit ID column.  When -S is used this way, all other\n");
				printf("options will be ignored.\n");
				printf("\n");
				printf("A single character argument of '#' may also be used.  This will list all\n");
				printf("PICkit 2 units with their Unit IDs and Firmware Versions.  NOTE that this\n");
				printf("is NOT safe to use when another application is already accessing a PICkit 2\n");
				printf("unit and may corrupt that USB connection. Also, a Unit ID of the single\n");
				printf("character '#' is not valid, but may used with longer IDs for example '#1'\n");
				printf("or '#two' are valid.\n");
				printf("\n");
				printf("To select a specific unit from among the attached PICkit 2 units to execute\n");
				printf("a given set of command options, follow the -S option with the Unit ID string\n");
				printf("of the intended PICkit 2 unit.\n");
				printf("\n");
				printf("This command may be used with or without a parameter.\n");
				printf("\n");
				printf("Syntax Example -s        (list connected PICkit 2 units - SAFE)\n");
				printf("               -s#       (list connected units with FW versions - UNSAFE)\n");
				printf("               -sLab1B   (use the PICkit 2 with Unit ID string 'Lab1B')\n");
				printf("               -#3       (use the PICkit 2 with Unit ID string '#3')\n");
				break;

			case 't':
			case 'T':
				printf("Enables the Vdd output pin after programming operations are complete.\n");
				printf("If not specified, then Vdd is turned off.  Use -a<> to set the voltage.\n");
				printf("\n");
				printf("There are no parameters for this command.\n");
				printf("\n");
				printf("Syntax Example -t\n");
				break;

			case 'u':
			case 'U':
				printf("Specifies a new OSCCAL value in hex. Used with a Program command, the device\n");
				printf("will be programmed with this new value. No error checking is done on the value.\n");
				printf("\n");
				printf("Syntax Example /uC80 or /u0x347C\n");
				break;

			case 'v':
			case 'V':
				printf("Specifies the Vpp value, in volts, that the device will be programmed with.\n");
				printf("If not entered, the default value for the device is used.  Normally this\n");
				printf("value should not be specified.\n");
				printf("\n");
				printf("The parameter for this command is the floating point value of the desired\n");
				printf("Vpp voltage.\n");
				printf("\n");
				printf("Syntax Example -v13.00\n");
				break;

			case 'w':
			case 'W':
				printf("If this switch is included, the target circuit will not be powered by the \n");
				printf("programmer and should be powered by an external power source. If this switch\n");
				printf("is not included, the target circuit will be powered by the programmer. The\n");
				printf("PICkit 2 is limited to an external power source voltage range of 2.5 Volts\n");
				printf("to 5.0 Volts.\n");
				printf("\n");
				printf("There are no parameters for this command.\n");
				printf("\n");
				printf("Syntax Example -w\n");
				break;

			case 'x':
			case 'X':
				printf("If this switch is included, PICkit 2 will attempt to program the device \n");
				printf("using the VPP first program entry method.  Not all families and devices\n");
				printf("support this feature.\n");
				printf("\n");
				printf("There are no parameters for this command.\n");
				printf("\n");
				printf("Syntax Example -x\n");

			case 'y':
			case 'Y':
				printf("Verifies the device against the selected hex file on the programmer.\n");
				printf("The entire device can be verified, or just selected memory regions. If one\n");
				printf("or more selected regions are to be verified, the verify command must be\n");
				printf("immediately followed by a memory region parameter. Valid parameters are:\n");
				printf("     P - program memory\n");
				printf("     E - EEPROM\n");
				printf("     I - ID Memory\n");
				printf("     C - Configuration Memory\n");
				printf("If no memory region parameter is entered, the entire device will be verified,\n");
				printf("otherwise only the selected memory regions will be verified. Multiple verify\n");
				printf("commands may be entered on one command line. \"Verify Succeeded\" will be\n");
				printf("displayed if the operation is successful, otherwise the first address of the\n");
				printf("first memory region where verification failed will be displayed along with a\n");
				printf("description of the failure.\n");
				printf("NOTE: For HCS and serial EEPROM devices, memory is considered region 'P'\n");
				printf("\n");
				printf("This command may be used without parameters or with a memory region.\n");
				printf("\n");
				printf("Syntax Examples -y\n");
				printf("                -yP -yI -yE\n");
				break;

			case 'z':
			case 'Z':
				printf("If this switch is included, then a complete device programming operation (-m)\n");
				printf("will preserve and not overwrite the existing EEPROM data memory on the device\n");
				printf("\n");
				printf("There are no parameters for this command.\n");
				printf("\n");
				printf("Syntax Example -z\n");
				break;

			default:
				return false; // may be one that needs the device file.

		}
	}
	else
	{ // just display main help
		displayHelp();
	}

	fflush(stdout);
	return true;
}

void Ccmd_app::displayHelp(void)
{
	printf("                        PICkit 2 COMMAND LINE HELP\n");
	printf("Options              Description                              Default\n");
	printf("----------------------------------------------------------------------------\n");
    printf("A<value>             Set Vdd voltage                          Device Specific\n");
	printf("B<path>              Specify the path to PK2DeviceFile.dat    Searches PATH\n");
	printf("                                                              and calling dir\n");
    printf("C                    Blank Check Device                       No Blank Check\n");
    printf("D<file>              OS Download                              None\n");
    printf("E                    Erase Flash Device                       Do Not Erase\n");
    printf("F<file>              Hex File Selection                       None\n");
    printf("G<Type><range/path>  Read functions                           None\n");
    printf("                     Type F: = read into hex file,\n");
    printf("                             path = full file path,\n");
    printf("                             range is not used\n");
    printf("                     Types P,E,I,C: = ouput read of Program,\n");
    printf("                             EEPROM, ID and/or Configuration\n");
    printf("                             Memory to the screen. P and E\n");
    printf("                             must be followed by an address\n");
    printf("                             range in the form of x-y where\n");
    printf("                             x is the start address and y is\n");
    printf("                             the end address both in hex,\n");
    printf("                             path is not used\n");
    printf("                             (Serial EEPROM memory is 'P')\n");
    printf("H<value>             Delay before Exit                        Exit immediately\n");
    printf("                         K = Wait on keypress before exit\n");
    printf("                         1 to 9 = Wait <value> seconds\n");
    printf("                                  before exit\n");
    printf("I                    Display Device ID & silicon revision     Do Not Display\n");
	printf("J<newlines>          Display operation percent complete       Rotating slash\n");
	printf("                         N = Each update on newline\n");
    printf("K                    Display Hex File Checksum                Do Not Display\n");
    printf("L<rate>              Set programming speed                    Fastest\n");
    printf("                     <rate> is a value of 1-16, with 1 being\n");
	printf("                     the fastest.\n");
    printf("M<memory region>     Program Device                           Do Not Program\n");
    printf("                     memory regions:\n");
    printf("                         P = Program memory\n");
    printf("                         E = EEPROM\n");
    printf("                         I = ID memory\n");
    printf("                         C = Configuration memory\n");
    printf("                         If no region is entered, the entire\n");
    printf("                         device will be erased & programmed.\n");
    printf("                         If a region is entered, no erase\n");
    printf("                         is performed and only the given\n");
    printf("                         region is programmed.\n");
    printf("                         All programmed regions are verified.\n");
    printf("			            (serial EEPROM memory is 'P')\n");
    printf("N<string>            Assign Unit ID string to first found     None\n");
    printf("                     PICkit 2 unit.  String is limited to 14\n");
    printf("                     characters maximum.  May not be used\n");
    printf("                     with other options.\n");
    printf("                     Example: -NLab1B\n");
    printf("P<part>              Part Selection. Example: -PPIC16f887     (Required)\n");
	printf("P                    Auto-Detect in all detectable families\n");
	printf("PF                   List auto-detectable part families\n");
	printf("PF<id>               Auto-Detect only within the given part\n");
	printf("                     family, using the ID listed with -PF\n");
	printf("                     Example: -PF2\n");
	printf("Q                    Disable PE for PIC24/dsPIC33 devices     Use PE\n");
    printf("R                    Release /MCLR after operations           Assert /MCLR\n");
    printf("S<string/#>          Use the PICkit 2 with the given Unit ID  First found unit\n");
    printf("                     string.  Useful when multiple PICkit 2\n");
    printf("                     units are connected.\n");
    printf("                     Example: -SLab1B\n");
    printf("                     If no <string> is entered, then the\n");
    printf("                     Unit IDs of all connected units will be\n");
    printf("                     displayed.  In this case, all other \n");
    printf("                     options are ignored. -S# will list units\n");
	printf("                     with their firmware versions.\n");
	printf("                     See help -s? for more info.\n");
    printf("T                    Power Target after operations            Vdd off\n");
    printf("U<value>             Program OSCCAL memory, where:            Do Not Program\n");
    printf("                      <value> is a hexadecimal number\n");
    printf("                      representing the OSCCAL value to be\n");
    printf("                      programmed. This may only be used in\n");
    printf("                      conjunction with a programming \n");
    printf("                      operation.\n");
    printf("V<value>             Vpp override                             Device Specific\n");
    printf("W                    Externally power target                  Power from Pk2\n");
	printf("X                    Use VPP first Program Entry Method       VDD first\n");
    printf("Y<memory region>     Verify Device                            Do Not Verify\n");
    printf("                         P = Program memory\n");
    printf("                         E = EEPROM\n");
    printf("                         I = ID memory\n");
    printf("                         C = Configuration memory\n");
    printf("                         If no region is entered, the entire\n");
    printf("                         device will be verified.\n");
    printf("                         (Serial EEPROM memory is 'P')\n");
    printf("Z                    Preserve EEData on Program               Do Not Preserve\n");
    printf("?                    Help Screen                              Not Shown\n");
    printf("\n");
    printf("     Each option must be immediately preceeded by a switch, Which can\n");
    printf("     be either a dash <-> or a slash </> and options must be separated\n");
    printf("     by a single space.\n");
    printf("\n");
    printf("     Example:   PK2CMD /PPIC16F887 /Fc:\\mycode /M\n");
    printf("                               or\n");
    printf("                PK2CMD -PPIC16F887 -Fc:\\mycode -M\n");
    printf("\n");
    printf("     Any option immediately followed by a question mark will invoke\n");
    printf("     a more detailed description of how to use that option.\n");
    printf("\n");
    printf("     Commands and their parameters are not case sensitive. Commands will\n");
    printf("     be processed according to command order of precedence, not the order\n");
    printf("     in which they appear on the command line. \n");
    printf("	Precedence:\n");
    printf("                -?      (first)\n");
	printf("                -B\n");
	printf("                -S\n");
    printf("                -D\n");
	printf("                -N\n");
    printf("                -P\n");
    printf("                -A -F -J -L -Q -V -W -X -Z\n");
    printf("                -C\n");
    printf("                -U\n");
    printf("                -E\n");
    printf("                -M\n");
    printf("                -Y\n");
    printf("                -G\n");
    printf("                -I -K\n");
    printf("                -R -T\n");
    printf("                -H      (last)\n");
    printf("		\n");
    printf("     The program will return an exit code upon completion which will\n");
    printf("     indicate either successful completion, or describe the reason for\n");
    printf("     failure. To view the list of exit codes and their descriptions,\n");
    printf("     type -?E on the command line.\n");
	printf("\n");
	printf("     type -?V on the command line for version information.\n");
	printf("\n");
	printf("     type -?L on the command line for license information.\n");
	printf("\n");
	printf("     type -?P on the command line for a listing of supported devices.\n");
	printf("     type -?P<string> to search for and display a list of supported devices\n");
	printf("                      beginning with <string>.\n");
	printf("\n");
	printf("     Special thanks to the following individuals for their critical\n");
	printf("     contributions to the development of this software:\n");
	printf("		Jeff Post, Xiaofan Chen, and Shigenobu Kimura\n");
}

bool Ccmd_app::checkHelp2(int argc, _TCHAR* argv[], bool loadDeviceFileFailed)
{  // helps that need the device file loaded
	int i;

	// look for '?' in all arguments.  Display help for first found
	for (i = 0; i < argc; i++)
	{
		if (_tcschr(argv[i], '?'))
			break;
	}
	
	if (i == argc) // none found
		return false;

	if (checkSwitch(argv[i]))
	{
		switch (argv[i][1])
		{
			case '?':
				if (_tcslen(argv[i]) > 2)
				{
					if ((argv[i][2] == 'v') || (argv[i][2] == 'V'))
					{
						printf ("\nExecutable Version:    %d.%02d.%02d", VERSION_MAJOR, VERSION_MINOR, VERSION_DOT);
						
						if (loadDeviceFileFailed)
							printf ("\nDevice File Version:   not found\n");
						else
							printf ("\nDevice File Version:   %d.%02d.%02d\n", PicFuncs.DevFile.Info.VersionMajor,
								PicFuncs.DevFile.Info.VersionMinor, PicFuncs.DevFile.Info.VersionDot);
						// Look for PICkit 2
						selectUnitArg(argc, argv);
						if (PicFuncs.DetectPICkit2Device(pk2UnitIndex, true))
						{
							printf ("OS Firmware Version:   %d.%02d.%02d\n\n", PicFuncs.FirmwareVersion.major,
								PicFuncs.FirmwareVersion.minor, PicFuncs.FirmwareVersion.dot);
						}
						else
						{
							printf ("OS Firmware Version:   PICkit 2 not found\n\n");
						}

					}
					else if ((argv[i][2] == 'p') || (argv[i][2] == 'P'))
					{
						if (loadDeviceFileFailed)
						{
							printf ("Unable to list parts: Device File Load Failed\n\n");
						}
						else
						{
							_TCHAR searchTerm[MAX_PATH];
							// get search term
							_tcsncpy_s(searchTerm, &argv[i][3], _tcslen(argv[i])-3);
							argv[i] = (char *) "";
							int j = 1;
							while (((i+j) < argc) && (!checkSwitch(argv[i+j])))
							{ // check for term with space(s) in it
								_tcscat_s(searchTerm, argv[i+j]);
								argv[i + j++] = (char *) "";
							}
							displayPartList(argc, argv, searchTerm);
						}
					}
					else
					{
						displayHelp();
					}
				}
				break;

			default:
				printf("Invalid command, or no Help available to for specified command.\n");

		}
	}

	fflush(stdout);
	return true;
}

void Ccmd_app::displayLicense(void)
{
	printf("IMPORTANT: \n");
	printf("YOU MUST ACCEPT THE TERMS AND CONDITIONS OF THIS LICENSE AGREEMENT\n");
	printf("TO RECEIVE A LICENSE FOR THE ACCOMPANYING SOFTWARE.  TO ACCEPT THE\n");
    printf("TERMS OF THIS LICENSE, OPEN THIS PACKAGE AND PROCEED WITH THE\n");
	printf("DOWNLOAD OR USE OF THE SOFTWARE.  IF YOU DO NOT ACCEPT THESE LICENSE\n");
	printf("TERMS, DO NOT OPEN THIS PACKAGE, DOWNLOAD, OR USE THIS SOFTWARE.\n");
	printf("\n");
	printf("PICkit(tm) 2 PK2CMD SOFTWARE LICENSE \n");
	printf("\n");
	printf("This License Agreement (Agreement) is a contract between You (as\n");
	printf("an individual or as a representative of your employer) and\n");
	printf("Microchip Technology Incorporated (\"Company\") for the PICkit(tm) 2\n");
	printf("PK2CMD software (including source code) accompanying this Agreement\n");
	printf("(the \"Software\").  In consideration for access to the Software, You\n");
	printf("agree to be bound by this Agreement. \n");
	printf("\n");
	printf("1.  LICENSE GRANT. Subject to all of the terms and conditions of\n");
	printf("this Agreement, Company grants You a non-exclusive, non-\n");
	printf("sublicensable, non-transferable license to use the Software with\n");
	printf("Company products, modify the Software for use with Company products,\n");
	printf("and market, sell or otherwise distribute: \n");
	printf("\n");
	printf("(a) Your end application that integrates Software and Company\n");
	printf("    products (\"Licensee Product\"); or \n");
	printf("\n");
	printf("(b) Your modifications to the Software provided that the modified\n");
	printf("    Software has the following copyright and disclaimer notice\n");
	printf("    prominently posted in a location where end users will see it\n");
	printf("    (e.g., installation program, program headers, About Box, etc.):\n");
	printf("\n");
	printf("\"Copyright (c) 2005-2009 Microchip Technology Inc. All rights\n");
	printf("reserved. This version of the PICkit(tm) 2 PK2CMD Software has been\n");
	printf("modified by [INSERT YOUR NAME, DATE OF SOFTWARE MODIFICATION HERE].\n");
	printf("You may use, copy, modify and distribute the Software for use with\n");
	printf("Microchip products only.  If you distribute the Software or its\n");
	printf("derivatives, the Software must have this copyright and disclaimer\n");
	printf("notice prominently posted in a location where end users will see it\n");
	printf("(e.g., installation program, program headers, About Box, etc.).  To\n");
	printf("the maximum extent permitted by law, this Software is distributed\n");
	printf("\"AS IS\" and WITHOUT ANY WARRANTY INCLUDING BUT NOT LIMITED TO ANY\n");
	printf("IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR PARTICULAR PURPOSE,\n");
	printf("or NON-INFRINGEMENT. IN NO EVENT WILL MICROCHIP OR ITS LICENSORS BE\n");
	printf("LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL\n");
	printf("DAMAGESOF ANY KIND ARISING FROM OR RELATED TO THE USE, MODIFICATION\n");
	printf("OR DISTRIBUTION OF THIS SOFTWARE OR ITS DERIVATIVES.\"\n");
	printf("\n");
	printf("You may not copy or reproduce all or any portion of Software, except\n");
	printf("to the extent that such activity is specifically allowed by this\n");
	printf("Agreement or expressly permitted by applicable law notwithstanding\n");
	printf("the foregoing limitations.\n");
	printf("\n");
	printf("All copies of the Software created by You or for You, including\n");
	printf("derivatives, must include the copyright, trademark and other\n");
	printf("proprietary notices as they appear on the original or, in the event\n");
	printf("You modified the Software, the notice listed above. You may not\n");
	printf("remove or alter any identifying screen that is produced by the\n");
	printf("Software.\n");
	printf("\n");
	printf("2.  OWNERSHIP AND TITLE. Software is licensed pursuant to the\n");
	printf("    Agreement, not sold.  All right, title and interest, including\n");
	printf("    intellectual property rights, in and to Software, derivatives\n");
	printf("    thereof, implementation of the Software in microcontrollers,\n");
	printf("    and hardware and software implementations of Software or\n");
	printf("    derivatives shall remain in Company. You will not obtain\n");
	printf("    ownership rights to derivatives of Software, and by accepting\n");
	printf("    the terms of this Agreement assign any such rights to Company\n");
	printf("    that You do receive.  Except as specifically stated in the\n");
	printf("    Agreement, you are granted no other rights, express or implied,\n");
	printf("    to the Software, derivatives thereof, or other Company\n");
	printf("    intellectual property such as trade secrets, patents, \n");
	printf("    copyrights, and trademarks.\n");
	printf("\n");
	printf("3.  CONFIDENTIALITY. You agree not to disclose Software to any\n");
	printf("    third party, except as permitted by this Agreement.  To the\n");
	printf("    extent that Software becomes part of the public domain, is\n");
	printf("    independently developed, or obtained free from any obligation\n");
	printf("    of confidentiality then the obligation of confidentiality\n");
	printf("    under this Agreement shall not apply.\n");
 	printf("\n");
	printf("4.  COPYRIGHT. The Software is protected by U.S. copyright laws\n");
	printf("    and international copyright treaties, as well as other\n");
	printf("    intellectual property laws and treaties.\n");
	printf("\n");
	printf("5.  TERMINATION OF AGREEMENT. Without prejudice to any other\n");
	printf("    rights, Company may terminate this Agreement if You fail to\n");
	printf("    comply with the terms and conditions of this Agreement.\n");
	printf("    Upon termination, You shall immediately: (a) stop using and\n");
	printf("    distributing the Software and derivatives thereof; (b) destroy\n");
	printf("    all copies of the Software and derivatives in your possession;\n");
	printf("    and (c) remove Software from any of Your tangible media and\n");
	printf("    from systems on which the Software exists.  Termination of\n");
	printf("    this License shall not affect the right of any end user or\n");
	printf("    consumer to use Licensee Product or modified Software;\n");
	printf("    provided that such product or modified Software was purchased\n");
	printf("    or distributed prior to the termination of this License.\n");
	printf("\n");
	printf("6.  DANGEROUS APPLICATIONS. You acknowledge that Software has not\n");
	printf("    been designed to be fault tolerant.  You warrant that You will\n");
	printf("    not use Software or derivatives in a dangerous, hazardous, or\n");
	printf("    life supporting application where the failure of such\n");
	printf("    application could lead directly to death, personal injury, or\n");
	printf("    environmental damage.\n");
	printf("\n");
	printf("7.  INDEMNITY. You will indemnify and hold Company and its\n");
	printf("    licensor(s), its related companies and its suppliers, harmless\n");
	printf("    for, from and against, any claims, costs (including attorney's\n");
	printf("    fees), damages or liabilities, including without limitation\n");
	printf("    product liability claims, arising out of: (a) Your use,\n");
	printf("    modification and distribution of the Software and its\n");
	printf("    derivatives; or (b) violation of this Agreement. COMPANY AND\n");
	printf("    ITS LICENSOR(S) ASSUME NO RESPONSIBILITY FOR, NOR INDEMNIFY\n");
	printf("    YOU AGAINST, ANY PATENT, COPYRIGHT OR OTHER INTELLECTUAL\n");
	printf("    PROPERTY CLAIMS BROUGHT AGAINST YOU RELATING TO THE SOFTWARE.\n");
	printf("\n");
	printf("8.  NO WARRANTY. TO THE MAXIMUM EXTENT PERMITTED BY LAW, COMPANY\n");
	printf("    AND ITS LICENSOR PROVIDE SOFTWARE \"AS IS\" AND EXPRESSLY\n");
	printf("    DISCLAIM ANY WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED,\n");
	printf("    INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF\n");
	printf("    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR\n");
	printf("    NON-INFRINGEMENT. YOU ASSUME THE ENTIRE RISK ARISING OUT OF\n");
	printf("    USE OR PERFORMANCE OF SOFTWARE, AS WELL AS ANY DERIVATIVES OF\n");
	printf("    THE SOFTWARE MADE FOR YOU OR ON YOUR BEHALF.  COMPANY AND ITS\n");
	printf("    LICENSOR(S) ASSUME NO RESPONSIBILITY FOR THE ACCURACY OR\n");
	printf("    ERRORS OR OMISSIONS OF SOFTWARE AND DO NOT WARRANT THE\n");
	printf("    FOLLOWING: (A) THE FUNCTIONS CONTAINED IN SOFTWARE WILL MEET\n");
	printf("    YOUR REQUIREMENTS; (B) THE OPERATION OF SOFTWARE WILL BE\n");
	printf("    UNINTERRUPTED OR ERROR-FREE; OR (C) ANY DEFECTS IN SOFTWARE\n");
	printf("    WILL BE CORRECTED. \n");
	printf("\n");
	printf("9.  LIMITATION OF LIABILITY. COMPANY AND ITS LICENSOR TOTAL\n");
	printf("    AGGREGATE LIABILITY IN CONTRACT, WARRANTY, TORT (INCLUDING\n");
	printf("    NEGLIGENCE OR BREACH OF STATUTORY DUTY), STRICT LIABILITY,\n");
	printf("    INDEMNITY, CONTRIBUTION, OR OTHERWISE, SHALL NOT EXCEED THE\n");
	printf("    LICENSE FEE YOU PAID FOR THE SOFTWARE. IN NO EVENT SHALL\n");
	printf("    COMPANY AND ITS LICENSOR BE LIABLE FOR ANY INCIDENTAL, SPECIAL,\n");
	printf("    INDIRECT OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,\n");
	printf("    HARM TO YOUR EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE\n");
	printf("    GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS BY THIRD PARTIES\n");
	printf("    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), ANY CLAIMS\n");
	printf("    FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS. If any\n");
	printf("    remedy is determined to have failed of its essential purpose,\n");
	printf("    all limitations of liability and exclusion of damages set forth\n");
	printf("    in the limited warranty shall remain in effect.\n");
	printf("\n");
	printf("10. SURVIVAL.  Sections 2-15 shall survive termination of this\n");
	printf("     Agreement. \n");
	printf("\n");
	printf("11. CHOICE OF LAW; VENUE; LIMITATIONS ON CLAIMS. You agree that\n");
	printf("    this Agreement and any conflicts regarding Software, shall be\n");
	printf("    construed, interpreted and governed by the laws, and subject\n");
	printf("    to the exclusive jurisdiction of the state or territory in the\n");
	printf("    Company Terms and Conditions of Sale (\"T&Cs\").  In the event\n");
	printf("    that the T&Cs do not apply or the choice of law or\n");
	printf("    jurisdiction are not indicated therein, then this Agreement\n");
	printf("    shall be construed, interpreted and governed by the laws, and\n");
	printf("    subject to the exclusive jurisdiction of the State of Arizona,\n");
	printf("    U.S.A. without regard to any conflict of laws principles. You\n");
	printf("    agree that regardless of any law to the contrary, any cause of\n");
	printf("    action related to or arising out of this Agreement or Software\n");
	printf("    must be filed within one year after such cause of action\n");
	printf("    arose, or be considered waived.\n");
	printf("\n");
	printf("12. EXPORT COMPLIANCE. You will not export or re-export Software,\n");
	printf("    technical data, direct products thereof or any other items\n");
	printf("    which would violate any applicable export control laws and\n");
	printf("    regulations including, but not limited to, those of the United\n");
	printf("    States and the United Kingdom.  You agree that it is Your\n");
	printf("    responsibility to obtain copies of and to familiarize yourself\n");
	printf("    fully with these laws and regulations to avoid violation.\n");
	printf("\n");
	printf("13. ASSIGNMENT. Neither this agreement nor any rights, licenses\n");
	printf("    or obligations hereunder, may be assigned by You without the\n");
	printf("    Company's prior written approval.\n");
	printf("\n");
	printf("14. ENTIRE AGREEMENT: MODIFICATIONS AND WAIVER. This Agreement\n");
	printf("    constitutes the entire agreement of the parties with respect\n");
	printf("    to the subject matter of this Agreement, and merges and\n");
	printf("    supersedes all communications relating to this subject matter,\n");
	printf("    whether written or oral. Except as expressly set forth in this\n");
	printf("    Agreement, no modification of this Agreement will be effective\n");
	printf("    unless made in writing signed by Company.  No failure or delay\n");
	printf("    by Company or its licensor(s) to assert any rights or remedies\n");
	printf("    arising from a breach of this Agreement shall be construed as a\n");
	printf("    waiver or a continuing waiver of such rights and remedies, nor\n");
	printf("    shall failure or delay to assert a breach be deemed to waive that\n");
	printf("    or any other breach. If any part of this Agreement is found by a\n");
	printf("    court of competent jurisdiction to be invalid, unlawful or\n");
	printf("    unenforceable then such part shall be severed from the remainder\n");
	printf("    of this Agreement and replaced with a valid provision that comes\n");
	printf("    closest to the intention underlying the invalid provision.\n");
	printf("\n");
	printf("Copyright (c) 2005-2009, Microchip Technology Inc. All rights\n");
	printf("reserved. \n");

}

void Ccmd_app::displayPartList(int argc, _TCHAR* argv[], _TCHAR* argSearch)
{
	_TCHAR *partlist_array[1024];
	int partNum, partIdx;

	string2Upper(argSearch, MAX_PATH);

	printf ("\nDevice File Version:   %d.%02d.%02d\n", PicFuncs.DevFile.Info.VersionMajor,
		PicFuncs.DevFile.Info.VersionMinor, PicFuncs.DevFile.Info.VersionDot);

	// display sorted parts by family, in family display order
	if (argSearch[0] == 0)
		printf("Number of devices = %i\n\n", PicFuncs.DevFile.Info.NumberParts - 1); // don't count "unsupported" device
	else
		printf("List of devices starting with '%s':\n\n", argSearch);
	printf ("Device Name                  Device Family\n");
	printf ("-----------                  -------------\n");
	for (int index = 0; index < PicFuncs.DevFile.Info.NumberFamilies ; index++)
	{
		for (int order = 0; order < PicFuncs.DevFile.Info.NumberFamilies; order++)
		{
			if (PicFuncs.DevFile.Families[order].FamilyType == index)
			{
				// get all the parts in this family
				partNum = 0;
				for (partIdx = 1; partIdx < (PicFuncs.DevFile.Info.NumberParts); partIdx++)
				{
					// skip first part, which is "unsupported part"
					if (PicFuncs.DevFile.PartsList[partIdx].Family == order)
						partlist_array[partNum++] = strdup(PicFuncs.DevFile.PartsList[partIdx].PartName);
				}
				// sort them
				qsort(partlist_array, partNum, sizeof(_TCHAR *), strnatcmpWrapper);
				// list them
				if (argSearch[0] == 0)
				{ // list all parts
					for (partIdx = 0; partIdx < partNum; partIdx++)
					{
						printf("%-28s %s\n", partlist_array[partIdx], PicFuncs.DevFile.Families[order].FamilyName);
					}
				}
				else
				{ // search parts
					int l = (int)_tcslen(argSearch);
					for (partIdx = 0; partIdx < partNum; partIdx++)
					{
						if (_tcsncmp(partlist_array[partIdx], argSearch, l) == 0)
							printf("%-28s %s\n", partlist_array[partIdx], PicFuncs.DevFile.Families[order].FamilyName);
					}
				}
			}
		}
	}

	printf("\nNOTE: If the name contains a space with additional info in parentheses, Ex:\n");
	printf("        PIC16F636 (639)\n        93LC46A (C X8)\n");
	printf("      then only the characters before the space are required for -P, Ex:\n");
	printf("        -pPIC16F636\n        -p93LC46A\n");
	fflush(stdout);
}

int Ccmd_app::strnatcmpWrapper(const void *a, const void *b)
{
	return strnatcmp(*(char const **)a, *(char const **)b);
}
