//                            Software License Agreement
//
// Copyright (c) 2005-2009, Microchip Technology Inc. All rights reserved.
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
#include "PICkitFunctions.h"
#include "PIC32PE.h"
#include "dsP33_PE.h"
#include "P24F_PE.h"
#include "string.h"
#ifdef WIN32
#include "Windows.h"
#endif

CPICkitFunctions::CPICkitFunctions(void)
{
	vppFirstEnabled = false;
	targetSelfPowered = false;
	OverwriteOSCCAL = false;
	scriptBufferChecksum = 0xFFFFFFFF;
	DeviceBuffers = &DevBuffs;
	BlankBuffers = &BlnkBuffs;
	LastICSPSpeed = 0;
	usePE33 = true;
	printedUsingPE = false;
	deviceRevision = 0;
	usePercentTimer = false;
	useTimerNewlines = false;
	timerOperation = NULL;
	timerIncrement = 0;
	timerValue = 0;
}

CPICkitFunctions::~CPICkitFunctions(void)
{
}

const unsigned char CPICkitFunctions::BitReverseTable[256] =
	{
		0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
		0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
		0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
		0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
		0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
		0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
		0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
		0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
		0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
		0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
		0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
		0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
		0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
		0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
		0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
		0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
	};

int CPICkitFunctions::ReadPkStatus(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_READ_STATUS;
    if (writeUSB(commandArray, 1))
	{
		if (readUSB())
		{
			return (int)(Usb_read_array[1] * 256 + Usb_read_array[0]);
		}
	}
    return 0xFFFF;
}


unsigned int CPICkitFunctions::ComputeChecksum(void)
{
    unsigned int checksum = 0;
    
    if (DevFile.Families[ActiveFamily].BlankValue < 0xFFFF)
    { // 16F and baseline parts are caculated a word at a time.
        // prog mem first
        int progMemEnd = (int)DevFile.PartsList[ActivePart].ProgramMem;
        
        if (DevFile.PartsList[ActivePart].OSSCALSave)
        { // don't include last location for devices with OSSCAL 
            progMemEnd--;
        }

        if (((DevFile.PartsList[ActivePart].CPMask & DeviceBuffers->ConfigWords[DevFile.PartsList[ActivePart].CPConfig - 1])
                != DevFile.PartsList[ActivePart].CPMask))
        {
            if (DevFile.Families[ActiveFamily].BlankValue < 0x3FFF)
            {
                progMemEnd = 0x40; // BASELINE - last location of unprotected mem
            }
            else
            {
                progMemEnd = 0; // no memory included for midrange.
            }
        }

        for (int idx = 0; idx < progMemEnd; idx++)
        {
            checksum += DeviceBuffers->ProgramMemory[idx];
        }

        if (DevFile.PartsList[ActivePart].ConfigWords > 0)
        {
            if (((DevFile.PartsList[ActivePart].CPMask & DeviceBuffers->ConfigWords[DevFile.PartsList[ActivePart].CPConfig - 1])
                    != DevFile.PartsList[ActivePart].CPMask)) // || codeProtectOn)
            { // if a code protect bit is set, the checksum is computed differently.
                //checksum = 0; // don't include memory (moved above)
                for (int idx = 0; idx < DevFile.PartsList[ActivePart].UserIDWords; idx++)
                { // add last nibble of UserIDs in decreasing nibble positions of checksum
                    int idPosition = 1;
                    for (int factor = 0; factor < idx; factor++)
                    {
                        idPosition <<= 4;
                    }
                    checksum += (unsigned int)((0xF & DeviceBuffers->UserIDs[DevFile.PartsList[ActivePart].UserIDWords - idx - 1])
                         * idPosition);
                }
            }                
            
            // config words
            for (int idx = 0; idx < DevFile.PartsList[ActivePart].ConfigWords; idx++)
            {
                checksum += (DeviceBuffers->ConfigWords[idx] & DevFile.PartsList[ActivePart].ConfigMasks[idx]);
            }
        }
        return (checksum & 0xFFFF);
    }
    else
    { //PIC18, PIC24 are computed a byte at a time.
        int progMemEnd = (int)DevFile.PartsList[ActivePart].ConfigAddr / DevFile.Families[ActiveFamily].ProgMemHexBytes;
        if (progMemEnd > (int)DevFile.PartsList[ActivePart].ProgramMem)
        {
            progMemEnd = (int)DevFile.PartsList[ActivePart].ProgramMem;
        }
        
        for (int idx = 0; idx < progMemEnd; idx++)
        {
            unsigned int memWord = DeviceBuffers->ProgramMemory[idx];
            checksum += (memWord & 0x000000FF);
            for (int bite = 1; bite < DevFile.Families[ActiveFamily].BytesPerLocation; bite++)
            {
                memWord >>= 8;
                checksum += (memWord & 0x000000FF);
            }    
        }

        if (DevFile.PartsList[ActivePart].ConfigWords > 0)
        {
            if (((DevFile.PartsList[ActivePart].CPMask & DeviceBuffers->ConfigWords[DevFile.PartsList[ActivePart].CPConfig - 1])
                != DevFile.PartsList[ActivePart].CPMask)) // || codeProtectOn)
            { // if a code protect bit is set, the checksum is computed differently.
              // NOTE: this will only match MPLAB checksum if ALL CP bits are set or ALL CP bits are clear.
                checksum = 0; // don't include memory
                for (int idx = 0; idx < DevFile.PartsList[ActivePart].UserIDWords; idx++)
                { // add UserIDs to checksum
                    unsigned int memWord = DeviceBuffers->UserIDs[idx];
                    checksum += (memWord & 0x000000FF);
                    checksum += ((memWord >> 8) & 0x000000FF);
                }
            }   

            // config words
            for (int idx = 0; idx < DevFile.PartsList[ActivePart].ConfigWords; idx++)
            {
                unsigned int memWord = (DeviceBuffers->ConfigWords[idx] & DevFile.PartsList[ActivePart].ConfigMasks[idx]);
                checksum += (memWord & 0x000000FF);
                checksum += ((memWord >> 8) & 0x000000FF);
            }
        }
        return (checksum & 0xFFFF);
    }
    
}

unsigned int CPICkitFunctions::ReadDeviceID(void)
{
	SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

	RunScript(SCR_PROG_ENTRY, 1);
	RunScript(SCR_RD_DEVID, 1);
	UploadData();
	RunScript(SCR_PROG_EXIT, 1);
    unsigned int deviceID = (unsigned int)(Usb_read_array[2] * 256 + Usb_read_array[1]);
    for (int shift = 0; shift < DevFile.Families[ActiveFamily].ProgMemShift; shift++)
    {
        deviceID >>= 1;         // midrange/baseline part results must be shifted by 1
    }
	deviceID &= DevFile.Families[ActiveFamily].BlankValue;

	// Get device revision
	if (Usb_read_array[0] == 4)  // 16-bit/32-bit parts have Rev in separate word
	{
		deviceRevision = (unsigned int)(Usb_read_array[4] * 256 + Usb_read_array[3]);
		if (DevFile.Families[ActiveFamily].BlankValue == 0xFFFFFFFF) // PIC32
			deviceRevision >>= 4;
	}
	else
	{
		deviceRevision = (unsigned int)(deviceID & ~DevFile.Families[ActiveFamily].DeviceIDMask);
	}

	deviceID &= DevFile.Families[ActiveFamily].DeviceIDMask; // mask off version bits.

	VddOff();
	SetMCLR(false);   

	return deviceID;
}

unsigned int CPICkitFunctions::GetDeviceRevision(void)
{ // NOTE: ReadDeviceID() *MUST* be called first
	return deviceRevision;
}

bool CPICkitFunctions::FamilyIsKeeloq(void)
{
	int x = _tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "KEELOQ", 6);
	if (x == 0)
		return true;
	return false;
}


bool CPICkitFunctions::FamilyIsEEPROM(void)
{  
	int x = _tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "EEPROM", 6);
	if (x == 0)
		return true;
	return false;
}

bool CPICkitFunctions::FamilyIsMCP(void)
{  
	int x = _tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "MCP", 3);
	if (x == 0)
		return true;
	return false;
}

bool CPICkitFunctions::FamilyIsPIC32(void)
{  
	int x = _tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "PIC32", 5);
	if (x == 0)
		return true;
	return false;
}

bool CPICkitFunctions::WriteDevice(bool progmem, bool eemem, bool uidmem, bool cfgmem, bool useLowVoltageRowErase)
{
    bool configInProgramSpace = false;

	if (FamilyIsPIC32())
	{
		P32Write(progmem, uidmem, cfgmem);
		return true; // configs in program space
	}
    
    // compute configration information.
    int configLocation = (int)DevFile.PartsList[ActivePart].ConfigAddr /
        DevFile.Families[ActiveFamily].ProgMemHexBytes;
    int configWords = DevFile.PartsList[ActivePart].ConfigWords;
    int endOfBuffer = DevFile.PartsList[ActivePart].ProgramMem;
    //unsigned int configBackups[MAX_CFG];  // use because DeviceBuffers are masked & won't verify later
    if ((configLocation < (int)DevFile.PartsList[ActivePart].ProgramMem) && (configWords > 0))
    {// if config in program memory, set them to clear.
        configInProgramSpace = true;
        // PIC24F and PIC18 J-series always program & verify the entire flash memory, as PK2CMD does not
		// have the logic yet to skip a blank section and program flash at the end.
        /*for (int cfg = configWords; cfg > 0; cfg--)
        {
            configBackups[cfg - 1] = DeviceBuffers->ProgramMemory[endOfBuffer - cfg];
            DeviceBuffers->ProgramMemory[endOfBuffer - cfg] =
                        DevFile.Families[ActiveFamily].BlankValue;
        }*/
    } 
    endOfBuffer--;

	SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();
	Sleep(50);

	unsigned char downloadBuffer[DOWNLOAD_BUFFER_SIZE];

    // Write Program Memory --------------------------------------------------------------------------------------------
    if (progmem)
    {
		// Get OSCCAL if need be
		if (DevFile.PartsList[ActivePart].OSSCALSave)
		{
			if (!OverwriteOSCCAL)
				ReadOSSCAL();
			// put OSCCAL into part memory so it doesn't have to be written seperately.
			DeviceBuffers->ProgramMemory[DevFile.PartsList[ActivePart].ProgramMem - 1] =
				DeviceBuffers->OSCCAL;                  
		}

        RunScript(SCR_PROG_ENTRY, 1);

        if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
        { // if prog mem address set script exists for this part
            DownloadAddress3(0);
            RunScript(SCR_PROGMEM_WR_PREP, 1);
        }
        if (FamilyIsKeeloq())
        {
            HCS360_361_VppSpecial();
        }               
        
        int wordsPerWrite = DevFile.PartsList[ActivePart].ProgMemWrWords;
        int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
        int scriptRunsToUseDownload = DOWNLOAD_BUFFER_SIZE /
            (wordsPerWrite * bytesPerWord);
        int wordsPerLoop = scriptRunsToUseDownload * wordsPerWrite;
        int wordsWritten = 0;

        // Find end of used memory
        endOfBuffer = FindLastUsedInBuffer(DeviceBuffers->ProgramMemory,
                                    DevFile.Families[ActiveFamily].BlankValue, endOfBuffer);
        if (wordsPerWrite == (endOfBuffer +1))
        { // very small memory sizes (like HCS parts)
            scriptRunsToUseDownload = 1;
            wordsPerLoop = wordsPerWrite;
        }
        // align end on next loop boundary                 
        int writes = (endOfBuffer + 1) / wordsPerLoop;
        if (((endOfBuffer + 1) % wordsPerLoop) > 0)
        {
            writes++;
        }
        endOfBuffer = writes * wordsPerLoop;    
		LastVerifyLocation = endOfBuffer;

		if (useProgExec33())
        {
            if (!PE33Write(endOfBuffer))
            {
				VddOff();
				SetMCLR(false);
                return false;
            }
			timerStop();
        }
		else if (useProgExec24F())
        {
            if (!PE24FWrite(endOfBuffer))
            {
				VddOff();
				SetMCLR(false);
                return false;
            }
			timerStop();
        }
		else
		{
			timerStart((_TCHAR*)"Write Flash", endOfBuffer / wordsPerLoop);
			do
			{
				int downloadIndex = 0;
				for (int word = 0; word < wordsPerLoop; word++)
				{
					if (wordsWritten == endOfBuffer)
					{
						break; // for cases where ProgramMemSize%WordsPerLoop != 0
					}                             
					unsigned int memWord = DeviceBuffers->ProgramMemory[wordsWritten++];
					if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
					{
						memWord = memWord << 1;
					}
	                
					downloadBuffer[downloadIndex++] = (unsigned char) (memWord & 0xFF);
	                
					for (int bite = 1; bite < bytesPerWord; bite++)
					{
						memWord >>= 8;
						downloadBuffer[downloadIndex++] = (unsigned char) (memWord & 0xFF);
					}                             

				}
				// download data
				if (FamilyIsKeeloq())
				{
					processKeeloqData(downloadBuffer, wordsWritten);
				}
				int dataIndex = DataClrAndDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, 0);
				while (dataIndex < downloadIndex)
				{
					dataIndex = DataDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, dataIndex);
				}
	            
				RunScript(SCR_PROGMEM_WR, scriptRunsToUseDownload);

				if (((wordsWritten % 0x8000) == 0) && (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0))
				{ //PIC24 must update TBLPAG
					DownloadAddress3(0x10000 * (wordsWritten / 0x8000));
					RunScript(SCR_PROGMEM_WR_PREP, 1);
				}
	       
				timerPrint();
			} while (wordsWritten < endOfBuffer);

			if (!configInProgramSpace)
				RunScript(SCR_PROG_EXIT, 1);	// don't exit programming mode if configs in program memory.
		}
		timerStop();
    }

/*    if (configInProgramSpace)
    {// if config in program memory, restore prog memory to proper values.
        for (int cfg = configWords; cfg > 0; cfg--)
        {
            DeviceBuffers->ProgramMemory[DevFile.PartsList[ActivePart].ProgramMem - cfg] 
                    = configBackups[cfg - 1];
        }
    } */

    // Write EEPROM ----------------------------------------------------------------------------------------------------
    if (eemem && (DevFile.PartsList[ActivePart].EEMem > 0))
    {

        RunScript(SCR_PROG_ENTRY, 1);
        
        if (DevFile.PartsList[ActivePart].EEWrPrepScript > 1)
        {
            if (DevFile.Families[ActiveFamily].EEMemHexBytes == 4)
            { // 16-bit parts
                DownloadAddress3((int)(DevFile.PartsList[ActivePart].EEAddr / 2));
            }
            else
            {
                DownloadAddress3(0);
            }
            RunScript(SCR_EE_WR_PREP, 1);
        }

        int bytesPerWord = DevFile.Families[ActiveFamily].EEMemBytesPerWord;
        unsigned int eeBlank = getEEBlank();

        // write at least 16 locations per loop
        int locationsPerLoop = DevFile.PartsList[ActivePart].EEWrLocations;
        if (locationsPerLoop < 16)
        {
            locationsPerLoop = 16;
        }
        
        // find end of used EE
        if (progmem && !useLowVoltageRowErase)
        { // we're writing all, so EE is erased first, we can skip blank locations at end
          // unless we're using LVRowErase in which case we need to write all as EE isn't erased first.
            endOfBuffer = FindLastUsedInBuffer(DeviceBuffers->EEPromMemory, eeBlank,
                                    DevFile.PartsList[ActivePart].EEMem - 1);
        }
        else
        { // if we're only writing EE, must write blanks in case they aren't blank on device
            endOfBuffer = DevFile.PartsList[ActivePart].EEMem - 1;
        }
        // align end on next loop boundary                 
        int writes = (endOfBuffer + 1) / locationsPerLoop;
        if (((endOfBuffer + 1) % locationsPerLoop) > 0)
        {
            writes++;
        }
        endOfBuffer = writes * locationsPerLoop;                                                                           

		int bufferSize = locationsPerLoop * bytesPerWord;

		int scriptRunsPerLoop = locationsPerLoop / DevFile.PartsList[ActivePart].EEWrLocations;
		int locationsWritten = 0;

		timerStart((_TCHAR*)"Write EEDATA", endOfBuffer / locationsPerLoop);

		do
		{
			int downloadIndex = 0;
			for (int word = 0; word < locationsPerLoop; word++)
			{
				unsigned int eeWord = DeviceBuffers->EEPromMemory[locationsWritten++];
				if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
				{
					eeWord = eeWord << 1;
				}

				downloadBuffer[downloadIndex++] = (unsigned char)(eeWord & 0xFF);

				for (int bite = 1; bite < bytesPerWord; bite++)
				{
					eeWord >>= 8;
					downloadBuffer[downloadIndex++] = (unsigned char)(eeWord & 0xFF);
				}  
			}
			// download data
			DataClrAndDownload(downloadBuffer, bufferSize, 0);
			RunScript(SCR_EE_WR, scriptRunsPerLoop);
            
			timerPrint();
		} while (locationsWritten < endOfBuffer);
		RunScript(SCR_PROG_EXIT, 1);
		timerStop();
    }

    // Write UserIDs ---------------------------------------------------------------------------------------------------
    if (uidmem && (DevFile.PartsList[ActivePart].UserIDWords > 0))
    { // do not write if EE unselected as PIC18F cannot erase/write UserIDs except with ChipErase
        RunScript(SCR_PROG_ENTRY, 1);

        if (DevFile.PartsList[ActivePart].UserIDWrPrepScript > 0)
        {
            RunScript(SCR_USERID_WR_PREP, 1);
        }             
        
        int bytesPerID = DevFile.Families[ActiveFamily].UserIDBytes;
        int bufferSize = DevFile.PartsList[ActivePart].UserIDWords * bytesPerID;

        int downloadIndex = 0;
        int idWritten = 0;
        for (int word = 0; word < DevFile.PartsList[ActivePart].UserIDWords; word++)
        {
            unsigned int memWord = DeviceBuffers->UserIDs[idWritten++];
            if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
            {
                memWord = memWord << 1;
            }

            downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);

            for (int bite = 1; bite < bytesPerID; bite++)
            {
                memWord >>= 8;
                downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);
            }

        }
        // download data
        int dataIndex = DataClrAndDownload(downloadBuffer, bufferSize, 0);
        while (dataIndex < downloadIndex)
        {
            dataIndex = DataDownload(downloadBuffer, bufferSize, dataIndex);
        }

        RunScript(SCR_USERID_WR, 1);

        RunScript(SCR_PROG_EXIT, 1);
		
    }

    
    // WRITE CONFIGURATION ---------------------------------------------------------------------------------------------
    if (cfgmem)
    { // 
        // Write Configuration
        if ((configWords > 0) && (!configInProgramSpace))
        { // Write config words differently for any part where they are stored in program memory.
			// Get BandGap if need be
			if (DevFile.PartsList[ActivePart].BandGapMask > 0)
			{
				ReadBandGap();
			}
            // 18F devices create a problem as the WRTC bit in the next to last config word
            // is effective immediately upon being written, which if asserted prevents the 
            // last config word from being written.
            // To get around this, we're using a bit of hack.  Detect PIC18F or PIC18F_K_parts,
            // and look for WRTC = 0.  If found, write config words once with CONFIG6 = 0xFFFF
            // then re-write it with the correct value.
            if (_tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "PIC18F", 9) ||
                _tcsncmp(DevFile.Families[ActiveFamily].FamilyName, "PIC18F_K_", 9))
            {
                if (DevFile.PartsList[ActivePart].ConfigWords > 5)
                { // don't blow up if part doesn't have enough config words
                    if ((DeviceBuffers->ConfigWords[5] & ~0x2000) == DeviceBuffers->ConfigWords[5])
                    { // if WRTC is asserted
                        unsigned int saveConfig6 = DeviceBuffers->ConfigWords[5];
                        DeviceBuffers->ConfigWords[5] = 0xFFFF;
                        WriteConfigOutsideProgMem();
                        DeviceBuffers->ConfigWords[5] = saveConfig6;
                    }
                
                }
            }
            WriteConfigOutsideProgMem();

        }
        
    } 

	if ((!configInProgramSpace) && progmem && (!FamilyIsKeeloq()))
	{
		VddOff();
		SetMCLR(false);     // don't execute if configs in program space or Keeloq.
	}
    return configInProgramSpace;
}

bool CPICkitFunctions::HCS360_361_VppSpecial(void)
{
    if (DevFile.PartsList[ActivePart].DeviceID != 0xFFFFFF36)
    { // only HCS360, 361 need this.
        return true;
    }

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = 10;
    if ((DeviceBuffers->ProgramMemory[0] & 0x1) == 0)
    { // bit 0 word 0 is 0
        commandArray[2] = SCMD_SET_ICSP_PINS; // data goes low with VPP staying low, clock high
        commandArray[3] = 0x04;
        commandArray[4] = SCMD_MCLR_GND_ON;
        commandArray[5] = SCMD_VPP_OFF;                
        commandArray[6] = SCMD_DELAY_LONG;
        commandArray[7] = 0x05;                
        commandArray[8] = SCMD_SET_ICSP_PINS; // data set to bit 0 word 0, clock high
        commandArray[9] = 0x04;
        commandArray[10] = SCMD_SET_ICSP_PINS; // clock low, data keeps value
        commandArray[11] = 0x00;
    }
    else
    { // bit 0 word 0 is 1
        commandArray[2] = SCMD_SET_ICSP_PINS; // data goes low with VPP high, clock high
        commandArray[3] = 0x04;
        commandArray[4] = SCMD_MCLR_GND_OFF;
        commandArray[5] = SCMD_VPP_ON;                
        commandArray[6] = SCMD_DELAY_LONG;
        commandArray[7] = 0x05;
        commandArray[8] = SCMD_SET_ICSP_PINS; // data set to bit 0 word 0, clock high
        commandArray[9] = 0x0C;
        commandArray[10] = SCMD_SET_ICSP_PINS; // clock low, data keeps value
        commandArray[11] = 0x08;                
    }
    return writeUSB(commandArray, 12);

}

void CPICkitFunctions::processKeeloqData(unsigned char downloadBuffer[], int wordsWritten)
{
    if (DevFile.PartsList[ActivePart].DeviceID == 0xFFFFFF36)
    { // do nothing unless it's the HCS360 or 361
        for (int i = (wordsWritten/2); i > 0; i--)
        {
            downloadBuffer[i * 4 - 1] = (unsigned char)~downloadBuffer[i * 2 - 1];
            downloadBuffer[i * 4 - 2] = (unsigned char)~downloadBuffer[i * 2 - 2];
            downloadBuffer[i * 4 - 3] = downloadBuffer[i * 2 -1 ];   // 360,361 need complements
            downloadBuffer[i * 4 - 4] = downloadBuffer[i * 2 - 2];
        }
        downloadBuffer[0] >>= 1; // first buffer should only contain 7MSBs of byte.
    }
}

void CPICkitFunctions::WriteConfigOutsideProgMem()
{
    int configWords = DevFile.PartsList[ActivePart].ConfigWords;
    unsigned char configBuffer[MAX_CFG * 2];
    
    if (DevFile.PartsList[ActivePart].BandGapMask > 0)
    {
        DeviceBuffers->ConfigWords[0] &= ~DevFile.PartsList[ActivePart].BandGapMask;
        DeviceBuffers->ConfigWords[0] |= DeviceBuffers->BandGap;
    }
    if (FamilyIsMCP())
    {
        DeviceBuffers->ConfigWords[0] |= 0x3FF8;
    }

    RunScript(SCR_PROG_ENTRY, 1);

    if (DevFile.PartsList[ActivePart].ConfigWrPrepScript > 0)
    {
        DownloadAddress3(0);
        RunScript(SCR_CONFIG_WR_PREP, 1);
    }    

    for (int i = 0, j = 0; i < configWords; i++)
    {
        unsigned int configWord = DeviceBuffers->ConfigWords[i];
        if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
        {
            configWord = configWord << 1;
        }
        configBuffer[j++] = (unsigned char)(configWord & 0xFF);
        configBuffer[j++] = (unsigned char)((configWord >> 8) & 0xFF);
    }
    DataClrAndDownload(configBuffer, MAX_CFG * 2, 0);
    RunScript(SCR_CONFIG_WR, 1);
    RunScript(SCR_PROG_EXIT, 1);        
}

int CPICkitFunctions::DataClrAndDownload(unsigned char dataArray[], int arrayLength , int startIndex)
// returns index of next byte to be transmitted. 0 = failed
{
    if (startIndex >= arrayLength)
    {
        return 0;
    }
    int length = arrayLength - startIndex;
    if (length > 61)
    {
        length = 61;
    }
	unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArray[1] = FWCMD_DOWNLOAD_DATA;
    commandArray[2] = (unsigned char)(length & 0xFF);
    for (int i = 0; i < length; i++)
    {
        commandArray[3 + i] = dataArray[startIndex + i];
    }
    if (writeUSB(commandArray, 3+length))
    {
        return (startIndex + length);
    }
    else
    {
        return 0;
    }
}

int CPICkitFunctions::DataDownload(unsigned char dataArray[], int arrayLength , int startIndex)
// returns index of next byte to be transmitted. 0 = failed
{
    if (startIndex >= arrayLength)
    {
        return 0;
    }
    int length = arrayLength - startIndex;
    if (length > 62)
    {
        length = 62;
    }
	unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_DOWNLOAD_DATA;
    commandArray[1] = (unsigned char)(length & 0xFF);
    for (int i = 0; i < length; i++)
    {
        commandArray[2 + i] = dataArray[startIndex + i];
    }
    if (writeUSB(commandArray, 2 + length))
    {
        return (startIndex + length);
    }
    else
    {
        return 0;
    }
} 

int CPICkitFunctions::FindLastUsedInBuffer(unsigned int bufferToSearch[], unsigned int blankValue, int startIndex)
{   // go backwards from the start entry to find the last non-blank entry
    if (!FamilyIsKeeloq())
    {
        for (int index = startIndex; index > 0; index--)
        {
            if (bufferToSearch[index] != blankValue)
            {
                return index;
            }
        }
    }
    else
    {
        return startIndex;
    }
    
    return 0;          
}

bool CPICkitFunctions::EraseDevice(bool progmem, bool eemem, bool* useLowVoltageRowErase)
{
    unsigned int configLocation;
    int configWords;
    int endOfBuffer;
	unsigned int orMask;
	int cfg;

	*useLowVoltageRowErase = false;

    if (vddSetPoint < DevFile.PartsList[ActivePart].VddErase)
    { // below bulk erase voltage
        if (DevFile.PartsList[ActivePart].DebugRowEraseScript > 0)
        { // if device supports row erases, use them
            *useLowVoltageRowErase = true;
        }
        else
        {
			printf("WARNING: VDD below device bulk erase voltage\n- Erase/Program may not be successful.\n");
			fflush(stdout);
        }
    }

    SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

    // Get OSCCAL if need be
    if (DevFile.PartsList[ActivePart].OSSCALSave)
    {
		if (!OverwriteOSCCAL)
			ReadOSSCAL();
        // put OSCCAL into part memory so it doesn't have to be written seperately.
        DeviceBuffers->ProgramMemory[DevFile.PartsList[ActivePart].ProgramMem - 1] =
            DeviceBuffers->OSCCAL;                  
    }
    
    // Get BandGap if need be
    if (DevFile.PartsList[ActivePart].BandGapMask > 0)
    {
        ReadBandGap();
    }
    
    // Erase Device First
    bool reWriteEE = false;
    if (progmem && (eemem || DevFile.PartsList[ActivePart].EEMem == 0))
    { // chip erase when programming all
        if (*useLowVoltageRowErase)
        { // use row erases
            //printf"Erasing Part with Low Voltage Row Erase...\n";
            RowEraseDevice();
        }
        else
        { // bulk erase
            // dsPIC30F5011, 5013 need configs cleared before erase
            // but don't run this script if a row erase is defined
            if ((DevFile.PartsList[ActivePart].ConfigMemEraseScript > 0)
                && (DevFile.PartsList[ActivePart].DebugRowEraseSize == 0))
            {
                RunScript(SCR_PROG_ENTRY, 1);
                if (DevFile.PartsList[ActivePart].ConfigWrPrepScript > 0)
                {
                    DownloadAddress3(0);
                    RunScript(SCR_CONFIG_WR_PREP, 1);
                }
                ExecuteScript(DevFile.PartsList[ActivePart].ConfigMemEraseScript);
                RunScript(SCR_PROG_EXIT, 1);
            }
            RunScript(SCR_PROG_ENTRY, 1);
            if (DevFile.PartsList[ActivePart].ChipErasePrepScript > 0)
            {
                RunScript(SCR_ERASE_CHIP_PREP, 1);
            } 
            RunScript(SCR_ERASE_CHIP, 1);
            RunScript(SCR_PROG_EXIT, 1);
        }
    }
    else if (progmem &&(DevFile.PartsList[ActivePart].ProgMemEraseScript != 0))
    { // Don't erase EE when not selected.
        if (*useLowVoltageRowErase)
        { // use row erases
            RowEraseDevice();
        }
        else
        { // bulk erases
            RunScript(SCR_PROG_ENTRY, 1);
            RunScript(SCR_ERASE_PROGMEM, 1);
            RunScript(SCR_PROG_EXIT, 1);
        }
    }
    else if (eemem &&(DevFile.PartsList[ActivePart].EEMemEraseScript != 0))
    { // Some parts must have EE bulk erased before being re-written.
        RunScript(SCR_PROG_ENTRY, 1);
        RunScript(SCR_ERASE_EE, 1);
        RunScript(SCR_PROG_EXIT, 1);
    }
    else if ((!eemem && (DevFile.PartsList[ActivePart].EEMem != 0))
        && (DevFile.PartsList[ActivePart].ProgMemEraseScript == 0))
    {// Some parts cannot erase ProgMem, UserID, & config without erasing EE
     // so must read & re-write EE.
        readEEPROM(READ_MEM);
        if (*useLowVoltageRowErase)
        { // use row erases 
            RowEraseDevice();
        }
        else
        {
            // bulk erase
            // dsPIC30F5011, 5013 need configs cleared before erase
            // but don't run this script if a row erase is defined
            if ((DevFile.PartsList[ActivePart].ConfigMemEraseScript > 0)
                && (DevFile.PartsList[ActivePart].DebugRowEraseSize == 0))
            {
                RunScript(SCR_PROG_ENTRY, 1);
                if (DevFile.PartsList[ActivePart].ConfigWrPrepScript > 0)
                {
                    DownloadAddress3(0);
                    RunScript(SCR_CONFIG_WR_PREP, 1);
                }
                ExecuteScript(DevFile.PartsList[ActivePart].ConfigMemEraseScript);
                RunScript(SCR_PROG_EXIT, 1);
            }
            RunScript(SCR_PROG_ENTRY, 1);
            if (DevFile.PartsList[ActivePart].ChipErasePrepScript > 0)
            {
                RunScript(SCR_ERASE_CHIP_PREP, 1);
            } 
            RunScript(SCR_ERASE_CHIP, 1);
            RunScript(SCR_PROG_EXIT, 1);   
            reWriteEE = true;   
        }
    } 

	            // restore OSCCAL if need be
    if (DevFile.PartsList[ActivePart].OSSCALSave)
    {
        //DeviceBuffers->OSCCAL = oscCal;
        WriteOSSCAL();
    }
            
	BlankBuffers->OSCCAL = DeviceBuffers->OSCCAL;
	BlankBuffers->BandGap = DeviceBuffers->BandGap;

	CDeviceData* saveBuffers = DeviceBuffers;
	DeviceBuffers = BlankBuffers;	
	// for row erasing, call WRITE function
	if (*useLowVoltageRowErase)
	{
		WriteDevice(false, eemem, true, true, *useLowVoltageRowErase);
	}
	else
	{
		// restore BandGap if need be
		if (DevFile.PartsList[ActivePart].BandGapMask > 0)
		{
			//DeviceBuffers->BandGap = bandGap;
			WriteConfigOutsideProgMem();
		}
            
        // write "erased" config words for parts that don't bulk erase configs (ex 18F6520)
		// also for PIC18J/PIC24FJ to match MPLAB
        if (DevFile.PartsList[ActivePart].WriteCfgOnErase)
        {
            // compute configration information.
            configLocation = DevFile.PartsList[ActivePart].ConfigAddr /
                DevFile.Families[ActiveFamily].ProgMemHexBytes;
            configWords = DevFile.PartsList[ActivePart].ConfigWords;
            endOfBuffer = DevFile.PartsList[ActivePart].ProgramMem;
            if ((configLocation < DevFile.PartsList[ActivePart].ProgramMem) && (configWords > 0))
            {// if config in program memory, set them to clear.
                orMask = 0;
                if (DevFile.Families[ActiveFamily].BlankValue == 0xFFFF)
                { // PIC18J
                    orMask = 0xF000;
                }
                else
                { // PIC24FJ
                    orMask = 0xFF0000;
                }
                for (cfg = configWords; cfg > 0; cfg--)
                {
                    DeviceBuffers->ProgramMemory[endOfBuffer - cfg] =
                                DevFile.PartsList[ActivePart].ConfigBlank[configWords - cfg] | orMask;
                }
				writeConfigInsideProgramMem();
			}
			else
			{
				WriteConfigOutsideProgMem();
			}
        }
	}
	DeviceBuffers = saveBuffers;

	VddOff();
	SetMCLR(false);     // deassert /MCLR 
	return reWriteEE;
}

bool CPICkitFunctions::WriteOSSCAL()
{
    if (RunScript(SCR_PROG_ENTRY, 1))
    {
        unsigned int calWord = DeviceBuffers->OSCCAL;
        unsigned int calAddress = DevFile.PartsList[ActivePart].ProgramMem - 1;
        if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
        {
            calWord <<= 1;
        }
		unsigned char addressData[BUF_SIZE];
        addressData[0] = (unsigned char)(calAddress & 0xFF);
        addressData[1] = (unsigned char)((calAddress >> 8) & 0xFF);
        addressData[2] = (unsigned char)((calAddress >> 16) & 0xFF);
        addressData[3] = (unsigned char)(calWord & 0xFF);
        addressData[4] = (unsigned char)((calWord >> 8) & 0xFF);
        DataClrAndDownload(addressData, 5, 0);
        if (RunScript(SCR_OSSCAL_WR, 1))
        {
            if (RunScript(SCR_PROG_EXIT, 1))
            {
                return true;
            }
        }
    }
    return false;
} 

void CPICkitFunctions::RowEraseDevice(void)
{
    // row erase script automatically increments PC by number of locations erased.
    // --- Erase Program Memory  ---
    int memoryRows = (int)DevFile.PartsList[ActivePart].ProgramMem / DevFile.PartsList[ActivePart].DebugRowEraseSize;
    RunScript(SCR_PROG_ENTRY, 1);
    if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
    { // if prog mem address set script exists for this part
        DownloadAddress3(0);
        RunScript(SCR_PROGMEM_WR_PREP, 1);
    }            
	timerStart((_TCHAR*)"Row Erase", memoryRows/256);
    do
    {
        if (memoryRows >= 256)
        { // erase up to 256 rows at a time               
            RunScript(SCR_ROW_ERASE, 0);  // 0 = 256 times
            memoryRows -= 256;
        }
        else
        {
            RunScript(SCR_ROW_ERASE, memoryRows);
            memoryRows = 0;
        }
    timerPrint();
    } while (memoryRows > 0); 
    RunScript(SCR_PROG_EXIT, 1);
    
    // --- Erase EEPROM Data ---
    // only dsPIC30 currently needs this done
    if (DevFile.PartsList[ActivePart].EERowEraseScript > 0)
    {
        int eeRows = (int)DevFile.PartsList[ActivePart].EEMem / DevFile.PartsList[ActivePart].EERowEraseWords;
        RunScript(SCR_PROG_ENTRY, 1);
        if (DevFile.PartsList[ActivePart].EERdPrepScript != 0)
        { // if ee mem address set script exists for this part
            DownloadAddress3((int)DevFile.PartsList[ActivePart].EEAddr
                                /DevFile.Families[ActiveFamily].EEMemBytesPerWord);
            RunScript(SCR_EE_RD_PREP, 1);
        }
        do
        {
            if (eeRows >= 256)
            { // erase up to 256 rows at a time               
                RunScript(SCR_EEROW_ERASE, 0);  // 0 = 256 times
                eeRows -= 256;
            }
            else
            {
                RunScript(SCR_EEROW_ERASE, eeRows);
                eeRows = 0;
            }

        } while (eeRows > 0);
        RunScript(SCR_PROG_EXIT, 1);            
    
    }
    
    // --- Erase Config Memory  ---
    if (DevFile.PartsList[ActivePart].ConfigMemEraseScript > 0)
    {
        RunScript(SCR_PROG_ENTRY, 1);
        if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
        { // if prog mem address set script exists for this part
            DownloadAddress3((int)DevFile.PartsList[ActivePart].UserIDAddr);
            RunScript(SCR_PROGMEM_WR_PREP, 1);
        }
        ExecuteScript(DevFile.PartsList[ActivePart].ConfigMemEraseScript);
        RunScript(SCR_PROG_EXIT, 1);
    }
	timerStop();
}

bool CPICkitFunctions::SerialEEPROMErase(void)
{
	CDeviceData* saveBuffers = DeviceBuffers;
	DeviceBuffers = BlankBuffers;
    if (!EepromWrite(ERASE_EE)) // write blank values.
    {
		DeviceBuffers = saveBuffers;
        return false; //abort.
    }
	DeviceBuffers = saveBuffers;
    return true;
}

bool CPICkitFunctions::EepromWrite(bool eraseWrite)
{
    VddOn();

    int endOfBuffer = (int)DevFile.PartsList[ActivePart].ProgramMem;

    // Write  Memory
    RunScript(SCR_PROG_ENTRY, 1);

    int extraBytes = 3; // 3 extra bytes for address.
    if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == UNIO_BUS)
    {
        extraBytes = 4;
    }

    int wordsPerWrite = DevFile.PartsList[ActivePart].ProgMemWrWords;                
    int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;                
    int dataDownloadSize = DOWNLOAD_BUFFER_SIZE;
    if (endOfBuffer < dataDownloadSize)
    {
        dataDownloadSize = endOfBuffer + (endOfBuffer / (wordsPerWrite * bytesPerWord)) * extraBytes;
    }
    if (dataDownloadSize > DOWNLOAD_BUFFER_SIZE)
    {
        dataDownloadSize = DOWNLOAD_BUFFER_SIZE;
    }

    int scriptRunsToUseDownload = dataDownloadSize /
        ((wordsPerWrite * bytesPerWord) + extraBytes); // 3 or 4 extra bytes for address.
    int wordsPerLoop = scriptRunsToUseDownload * wordsPerWrite;
    int wordsWritten = 0;

	LastVerifyLocation = endOfBuffer;

	timerStart((_TCHAR*)"Write EEPROM", endOfBuffer / wordsPerLoop);

    unsigned char downloadBuffer[DOWNLOAD_BUFFER_SIZE];

    if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
    { // if prog mem write prep  script exists for this part
        RunScript(SCR_PROGMEM_WR_PREP, 1);
    }

    do
    {
        int downloadIndex = 0;

        for (int word = 0; word < wordsPerLoop; word++)
        {
            if (wordsWritten == endOfBuffer)
            {
                // we may not have filled download buffer, so adjust number of script runs
                scriptRunsToUseDownload = downloadIndex / ((wordsPerWrite * bytesPerWord) + extraBytes);
                break; // for cases where ProgramMemSize%WordsPerLoop != 0
            }

            if ((wordsWritten % wordsPerWrite) == 0)
            { // beginning of each script call
                // EEPROM address in download buffer
				int eeAddress = eeprom24BitAddress(wordsWritten, WRITE_BIT);
                if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == UNIO_BUS)
                {
                    downloadBuffer[downloadIndex++] = 0x96; //WREN
                }
                downloadBuffer[downloadIndex++] = (unsigned char)((eeAddress >> 16) & 0xFF); // upper byte
                downloadBuffer[downloadIndex++] = (unsigned char)((eeAddress >> 8) & 0xFF); // high byte
                downloadBuffer[downloadIndex++] = (unsigned char)(eeAddress & 0xFF); // low byte  
            }

            unsigned int memWord = DeviceBuffers->ProgramMemory[wordsWritten++];

            downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);

            for (int bite = 1; bite < bytesPerWord; bite++)
            {
                memWord >>= 8;
                downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);
            }
            
            if ((DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == MICROWIRE_BUS)
                 && (bytesPerWord == 2))
            { // "Endian-ness" of Microwire 16-bit words need to be swapped
                unsigned char swapTemp = downloadBuffer[downloadIndex - 2];
                downloadBuffer[downloadIndex - 2] = downloadBuffer[downloadIndex - 1];
                downloadBuffer[downloadIndex - 1] = swapTemp;
            }

        }
        // download data
        int dataIndex = DataClrAndDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, 0);
        while (dataIndex < downloadIndex)
        {
            dataIndex = DataDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, dataIndex);
        }

        RunScript(SCR_PROGMEM_WR, scriptRunsToUseDownload);

        if ((DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == I2C_BUS)
			|| (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == UNIO_BUS))
        {
            if (BusErrorCheck())
            {
                RunScript(SCR_PROG_EXIT, 1);
                VddOff();
				timerStop();
				if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == I2C_BUS)
				{
					printf("I2C Bus Error (No Acknowledge) - Aborted.\n");
					_tcsncpy_s(ReadError.memoryType, "I2C Bus", 7);
				}
				else
				{
					printf("UNI/O Bus Error (NoSAK) - Aborted.\n");
					_tcsncpy_s(ReadError.memoryType, "UNI/O Bus", 9);
				}
				ReadError.address = 0;
				ReadError.expected = 0;
				ReadError.read = 0;
                return false;
            }
        }
        timerPrint();
    } while (wordsWritten < endOfBuffer);


    RunScript(SCR_PROG_EXIT, 1);

	VddOff();
    timerStop();
    return true;

}

bool CPICkitFunctions::ExecuteScript(int scriptArrayIndex)
{
    // IMPORTANT NOTE: THIS ALWAYS CLEARS THE UPLOAD BUFFER FIRST!

    int scriptLength;
    if (scriptArrayIndex == 0)
        return false;

    scriptLength = DevFile.Scripts[--scriptArrayIndex].ScriptLength;

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArray[1] = FWCMD_EXECUTE_SCRIPT;
    commandArray[2] = (unsigned char)scriptLength;
    for (int n = 0; n < scriptLength; n++)
    {
        commandArray[3 + n] = (unsigned char)DevFile.Scripts[scriptArrayIndex].Script[n];
    }
    return writeUSB(commandArray, 3 + scriptLength);
}

bool CPICkitFunctions::ReadOSSCAL(void)
{
    if (RunScript(SCR_PROG_ENTRY, 1))
    {
        if (DownloadAddress3((int)(DevFile.PartsList[ActivePart].ProgramMem - 1)))
        {
            if (RunScript(SCR_OSSCAL_RD, 1))
            {
                if (UploadData())
                {
                    if (RunScript(SCR_PROG_EXIT, 1))
                    {
                        DeviceBuffers->OSCCAL = (unsigned int)(Usb_read_array[1] + (Usb_read_array[2] * 256));
                        if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
                        {
                            DeviceBuffers->OSCCAL >>= 1;
                        }
                        DeviceBuffers->OSCCAL &= DevFile.Families[ActiveFamily].BlankValue;
                        //DeviceBuffers->OSCCAL = 0xc00;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void CPICkitFunctions::ReadBandGap(void)
{
    RunScript(SCR_PROG_ENTRY, 1);
    RunScript(SCR_CONFIG_RD, 1);
    UploadData();
    RunScript(SCR_PROG_EXIT, 1);
    unsigned int config = (unsigned int)Usb_read_array[1];
    config |= (unsigned int)Usb_read_array[2] << 8;
    if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
    {
        config = (config >> 1) & DevFile.Families[ActiveFamily].BlankValue;
    }
    DeviceBuffers->BandGap = config & DevFile.PartsList[ActivePart].BandGapMask;
}

bool CPICkitFunctions::SetMCLR(bool nMCLR)
{
	unsigned char scriptArray[BUF_SIZE];
	if (nMCLR)
	{
		scriptArray[0] = SCMD_MCLR_GND_ON;
	}
	else
	{
		scriptArray[0] = SCMD_MCLR_GND_OFF;
	}
	return SendScript(scriptArray, 1);             
}

bool CPICkitFunctions::SendScript(unsigned char script[], int length)
{

	unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = (unsigned char)length;
    for (int n = 0; n < length; n++)
    {
        commandArray[2 + n] = script[n];
    }
    return writeUSB(commandArray, 2 + length);
}  

bool CPICkitFunctions::ReadDevice(char function, bool progmem, bool eemem, bool uidmem, bool cfgmem)
{
	if ((function == VERIFY_MEM) || (function == BLANK_CHECK) || (function == READ_MEM))
	{
		LastVerifyLocation = (int)DevFile.PartsList[ActivePart].ProgramMem;
	}
	// ELSE it should be set in WriteDevice 

	if (FamilyIsPIC32())
	{
		bool ret = false;
		if (function == READ_MEM)
		{
			ret = PIC32Read(progmem, uidmem, cfgmem);
			VddOff();
		}
		else if (function == BLANK_CHECK)
		{
			ret = PIC32BlankCheck();
		}
		else if (function == VERIFY_NOPRG_ENTRY)
		{
			ret = P32Verify(true, progmem, uidmem, cfgmem);
		}
		else // other verify
		{
			ret = P32Verify(false, progmem, uidmem, cfgmem);	
		}
		return ret;
	}

	if (function == BLANK_CHECK)
	{
		ResetBuffers();
		int configLocation = (int)DevFile.PartsList[ActivePart].ConfigAddr / 
			DevFile.Families[ActiveFamily].ProgMemHexBytes;
		int configWords = DevFile.PartsList[ActivePart].ConfigWords;
		if ((configWords > 0) && (configLocation < (int)DevFile.PartsList[ActivePart].ProgramMem))
		{
			// put config word blank values in program memory array
			for (int i = 0; i < configWords; i++)
			{
				DeviceBuffers->ProgramMemory[configLocation + i] = DevFile.PartsList[ActivePart].ConfigBlank[i]
					+ (DevFile.Families[ActiveFamily].BlankValue & 0xFFFF0000);
				if (DevFile.Families[ActiveFamily].BlankValue == 0xFFFF) // PIC18 J-Series
				{
					DeviceBuffers->ProgramMemory[configLocation + i] |= 0xF000;
				}
			}
		}
		if (DevFile.PartsList[ActivePart].BlankCheckSkipUsrIDs)
			uidmem = false;
	}

	unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
	VddOn();

	// Read Program Memory ---------------------------------------------------------------------------------------------
	if (progmem)
	{
		if (useProgExec33())
		{
			switch (function)
			{
				case READ_MEM:
                    if (!PE33Read())
					{
						VddOff();
						timerStop();
						return false;
					}
					break;

				case BLANK_CHECK:
					if (!PE33BlankCheck())
					{
						VddOff();
						timerStop();
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						return false;
					}
					break;

				case VERIFY_MEM_SHORT:
				case VERIFY_MEM:
				case VERIFY_NOPRG_ENTRY:
                    if (!PE33VerifyCRC())
                    {
						VddOff();
						timerStop();
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						return false;
                    }
					break;

				default:
					printf("Uknown Read Function Error\n");
					fflush(stdout);
					break;
			}
			timerStop();
		}
		else if (useProgExec24F())
		{
			switch (function)
			{
				case READ_MEM:
                    if (!PE24FRead())
					{
						VddOff();
						timerStop();
						return false;
					}
					break;

				case BLANK_CHECK:
					if (!PE24FBlankCheck())
					{
						VddOff();
						timerStop();
						return false;
					}
					break;

				case VERIFY_MEM_SHORT:
				case VERIFY_MEM:
                    if (!PE24FVerify(true, LastVerifyLocation, true))
                    {
						VddOff();
						timerStop();
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						return false;
                    }
					break;
				case VERIFY_NOPRG_ENTRY:
                    if (!PE24FVerify(true, LastVerifyLocation, false))
                    {
						VddOff();
						timerStop();
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						return false;
                    }
					break;

				default:
					printf("Uknown Read Function Error\n");
					fflush(stdout);
					break;
			}
			timerStop();
		}
		else
		{

			if (function != VERIFY_NOPRG_ENTRY)
				RunScript(SCR_PROG_ENTRY, 1);

			if ((DevFile.PartsList[ActivePart].ProgMemAddrSetScript != 0)
					&& (DevFile.PartsList[ActivePart].ProgMemAddrBytes != 0))
			{ // if prog mem address set script exists for this part & # bytes is not zero
			  // (MPLAB uses script on some parts when PICkit 2 does not)
				if (FamilyIsEEPROM())
				{
					DownloadAddress3MSBFirst(eeprom24BitAddress(0, WRITE_BIT));
					RunScript(SCR_PROGMEM_ADDRSET, 1);
					if (BusErrorCheck())
					{
						RunScript(SCR_PROG_EXIT, 1);
						VddOff();
						timerStop();
						if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == I2C_BUS)
							_tcsncpy_s(ReadError.memoryType, "I2C Bus", 7);
						else
							_tcsncpy_s(ReadError.memoryType, "UNI/O Bus", 9);
						ReadError.address = 0;
						ReadError.expected = 0;
						ReadError.read = 0;
						return false;
					}
				}
				else 
				{
					DownloadAddress3(0);
					RunScript(SCR_PROGMEM_ADDRSET, 1);
				}
			}

			int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
			int scriptRunsToFillUpload = UPLOAD_BUFFER_SIZE /
				(DevFile.PartsList[ActivePart].ProgMemRdWords * bytesPerWord);
			int wordsPerLoop = scriptRunsToFillUpload * DevFile.PartsList[ActivePart].ProgMemRdWords;
			int wordsRead = 0;

			if (function == BLANK_CHECK)
				timerStart((_TCHAR*)"Blank Check", LastVerifyLocation / wordsPerLoop);
			else if (FamilyIsEEPROM())
			{
				if (function == READ_MEM)
					timerStart((_TCHAR*)"Read EEPROM", LastVerifyLocation / wordsPerLoop);
				else // verify
					timerStart((_TCHAR*)"Verify EEPROM", LastVerifyLocation / wordsPerLoop);
			}
			else if (function == READ_MEM)
				timerStart((_TCHAR*)"Read Flash", LastVerifyLocation / wordsPerLoop);
			else // verify
				timerStart((_TCHAR*)"Verify Flash", LastVerifyLocation / wordsPerLoop);

			do
			{
				if (FamilyIsEEPROM())
				{
					if ((DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == I2C_BUS)
						&& (wordsRead > DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG])
						&& (wordsRead % (DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG] + 1) == 0))
					{ // must resend address to EE every time we cross a bank border.
						DownloadAddress3MSBFirst(eeprom24BitAddress(wordsRead, WRITE_BIT));
						RunScript(SCR_PROGMEM_ADDRSET, 1);
					}                    
					Download3Multiples(eeprom24BitAddress(wordsRead, READ_BIT), scriptRunsToFillUpload,
								DevFile.PartsList[ActivePart].ProgMemRdWords);
				}
				//RunScriptUploadNoLen2(SCR_PROGMEM_RD, scriptRunsToFillUpload);
				RunScriptUploadNoLen(SCR_PROGMEM_RD, scriptRunsToFillUpload);
				ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				//GetUpload();
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
				int uploadIndex = 0;                                 
				for (int word = 0; word < wordsPerLoop; word++)
				{
					int bite = 0;
					unsigned int memWord = (unsigned int)upload_buffer[uploadIndex + bite++];
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 8;
					}
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 16;
					}
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 24;
					}
					uploadIndex += bite;
					// shift if necessary
					if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
					{
						memWord = (memWord >> 1) & DevFile.Families[ActiveFamily].BlankValue;
					}
					// swap "Endian-ness" of 16 bit 93LC EEPROMs
					if ((FamilyIsEEPROM()) && (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == MICROWIRE_BUS)
						 && (bytesPerWord == 2))
					{
						int memTemp = (memWord << 8) & 0xFF00;
						memWord >>= 8;
						memWord |= memTemp;
					}
					if (function == READ_MEM)
					{
						DeviceBuffers->ProgramMemory[wordsRead++] = memWord;	
					}
					else
					{ // BLANK_CHECK or VERIFY_MEM
						if (DeviceBuffers->ProgramMemory[wordsRead++] != memWord)
						{ // location failed verify
							if (!DevFile.PartsList[ActivePart].OSSCALSave
								|| (DevFile.PartsList[ActivePart].ProgramMem != (unsigned int)wordsRead))
							{ // skip last byte in memory if it is osccal
								RunScript(SCR_PROG_EXIT, 1);
								VddOff();
								timerStop();
								_tcsncpy_s(ReadError.memoryType, "Program", 7);
								ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
								ReadError.expected = DeviceBuffers->ProgramMemory[wordsRead];
								ReadError.read = memWord;
								return false;
							}
						}
					}
					if (wordsRead == LastVerifyLocation)
					{
						break; // for cases where ProgramMemSize%WordsPerLoop != 0
					}
					if (((wordsRead % 0x8000) == 0)
							&& (DevFile.PartsList[ActivePart].ProgMemAddrSetScript != 0)
							&& (DevFile.PartsList[ActivePart].ProgMemAddrBytes != 0)
							&& (DevFile.Families[ActiveFamily].BlankValue > 0xFFFF))
					{ //PIC24 must update TBLPAG
						DownloadAddress3(0x10000 * (wordsRead / 0x8000));
						RunScript(SCR_PROGMEM_ADDRSET, 1);
						break;
					} 
				}
				timerPrint();
			} while (wordsRead < LastVerifyLocation);

			RunScript(SCR_PROG_EXIT, 1);
			timerStop();
		}
    } 

    // Read EEPROM -----------------------------------------------------------------------------------------------------
    if ((eemem)  && (DevFile.PartsList[ActivePart].EEMem > 0))
    {
        if (!readEEPROM(function))
			return false;
    }

    // Read UserIDs ----------------------------------------------------------------------------------------------------
    if ((DevFile.PartsList[ActivePart].UserIDWords > 0) && (uidmem))
    {                  
        RunScript(SCR_PROG_ENTRY, 1);
        if (DevFile.PartsList[ActivePart].UserIDRdPrepScript > 0)
        {
            RunScript(SCR_USERID_RD_PREP, 1);
        }
        int bytesPerWord = DevFile.Families[ActiveFamily].UserIDBytes;
        int wordsRead = 0;   
        int bufferIndex = 0;
        RunScriptUploadNoLen(SCR_USERID_RD, 1);
        ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
        if ((DevFile.PartsList[ActivePart].UserIDWords * bytesPerWord) > MAX_BYTES)
        {
            UploadDataNoLen();
            ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);                
        }
        RunScript(SCR_PROG_EXIT, 1);
        do
        {
            int bite = 0;
            unsigned int memWord = (unsigned int)upload_buffer[bufferIndex + bite++];
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[bufferIndex + bite++] << 8;
            }
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[bufferIndex + bite++] << 16;
            }
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[bufferIndex + bite++] << 24;
            }
            bufferIndex += bite;                    
            
            // shift if necessary
            if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
            {
                memWord = (memWord >> 1) & DevFile.Families[ActiveFamily].BlankValue;
            }

			if (function == READ_MEM)
			{
				DeviceBuffers->UserIDs[wordsRead++] = memWord;
			}
			else
			{ // BLANK_CHECK or VERIFY_MEM
				if (DeviceBuffers->UserIDs[wordsRead++] != memWord)
				{ // location failed verify
					RunScript(SCR_PROG_EXIT, 1);
					VddOff();
					timerStop();
					_tcsncpy_s(ReadError.memoryType, "ID", 2);
					ReadError.address = (DevFile.PartsList[ActivePart].UserIDAddr / DevFile.Families[ActiveFamily].UserIDHexBytes)
											* DevFile.Families[ActiveFamily].AddressIncrement;
					ReadError.address += --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
					ReadError.expected = DeviceBuffers->UserIDs[wordsRead];
					ReadError.read = memWord;
					return false;
				}
			}

        } while (wordsRead < DevFile.PartsList[ActivePart].UserIDWords);
    }

    // Read Configuration ------------------------------------------------------------------------------------------------
    int configLocation = (int)DevFile.PartsList[ActivePart].ConfigAddr / 
        DevFile.Families[ActiveFamily].ProgMemHexBytes;
    int configWords = DevFile.PartsList[ActivePart].ConfigWords;
    if ((configWords > 0) && (configLocation >= (int)DevFile.PartsList[ActivePart].ProgramMem)
                && (cfgmem))
    { // Don't read config words for any part where they are stored in program memory.     
        if (!ReadConfigOutsideProgMem(function))
			return false;
        
        // save bandgap if necessary
        if (DevFile.PartsList[ActivePart].BandGapMask > 0)
        {
            DeviceBuffers->BandGap = DeviceBuffers->ConfigWords[0] &
                    DevFile.PartsList[ActivePart].BandGapMask;
        }
        
    }
    else if ((configWords > 0) && (cfgmem))
    { // pull them out of program memory.
        for (int word = 0; word < configWords; word++)
        {
            DeviceBuffers->ConfigWords[word] = DeviceBuffers->ProgramMemory[configLocation + word];    
        }
    }
    
    // Read OSCCAL if exists
    /*if (Pk2.DevFile.PartsList[Pk2.ActivePart].OSSCALSave)
    {
        Pk2.ReadOSSCAL();
    }*/

	VddOff();
	//timerStop();
	return true;
}

bool CPICkitFunctions::readEEPROM(char function)
{
    unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];     
	bool ret = true;

    RunScript(SCR_PROG_ENTRY, 1);

    if (DevFile.PartsList[ActivePart].EERdPrepScript > 0)
    {
        if (DevFile.Families[ActiveFamily].EEMemHexBytes == 4)
        { // 16-bit parts
            DownloadAddress3((int)(DevFile.PartsList[ActivePart].EEAddr / 2));
        }
        else
        {
            DownloadAddress3(0);
        }
        RunScript(SCR_EE_RD_PREP, 1);
    }

    int bytesPerWord = DevFile.Families[ActiveFamily].EEMemBytesPerWord;
    int scriptRunsToFillUpload = UPLOAD_BUFFER_SIZE /
        (DevFile.PartsList[ActivePart].EERdLocations * bytesPerWord);
    int wordsPerLoop = scriptRunsToFillUpload * DevFile.PartsList[ActivePart].EERdLocations;
    int wordsRead = 0;

	if (function == BLANK_CHECK)
		timerStart((_TCHAR*)"Blank Check EE", DevFile.PartsList[ActivePart].EEMem / wordsPerLoop);
	else if (function == READ_MEM)
		timerStart((_TCHAR*)"Read EEDATA", DevFile.PartsList[ActivePart].EEMem / wordsPerLoop);
	else // verify
		timerStart((_TCHAR*)"Verify EEDATA", DevFile.PartsList[ActivePart].EEMem / wordsPerLoop);

    unsigned int eeBlank = getEEBlank();

    do
    {
        //RunScriptUploadNoLen2(SCR_EE_RD, scriptRunsToFillUpload);
		RunScriptUploadNoLen(SCR_EE_RD, scriptRunsToFillUpload);
        ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
        //GetUpload();
		UploadDataNoLen();
        ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
        int uploadIndex = 0;
        for (int word = 0; word < wordsPerLoop; word++)
        {
            int bite = 0;
            unsigned int memWord = (unsigned int)upload_buffer[uploadIndex + bite++];
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 8;
            }
            uploadIndex += bite;
            // shift if necessary
            if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
            {
                memWord = (memWord >> 1) & eeBlank;
            }

			if (function == READ_MEM)
			{
				 DeviceBuffers->EEPromMemory[wordsRead++] = memWord;	
			}
			else
			{ // BLANK_CHECK or VERIFY_MEM
				if (DeviceBuffers->EEPromMemory[wordsRead++] != memWord)
				{ // location failed verify
					RunScript(SCR_PROG_EXIT, 1);
					VddOff();
					timerStop();
					_tcsncpy_s(ReadError.memoryType, "EEData", 6);
					ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].EEMemAddressIncrement;
					ReadError.expected = DeviceBuffers->EEPromMemory[wordsRead];
					ReadError.read = memWord;
					return false;
				}
			}


            if (wordsRead >= DevFile.PartsList[ActivePart].EEMem)
            {
                break; // for cases where ProgramMemSize%WordsPerLoop != 0
            }
        }
        timerPrint();
    } while (wordsRead < DevFile.PartsList[ActivePart].EEMem);
    RunScript(SCR_PROG_EXIT, 1);
	timerStop();
	return ret;
}


bool CPICkitFunctions::Download3Multiples(int downloadBytes, int multiples, int increment)
{
    unsigned char firstCommand = FWCMD_CLR_DOWNLOAD_BUFFER;
    
    do
    {
        int thisWrite = multiples;
        if (multiples > 20) // can only write 20 per USB packet. (20 * 3 = 60 bytes)
        {
            thisWrite = 20;
            multiples -= 20;   
        }    
        else
        {
            multiples = 0;
        }    
		unsigned char commandArray[BUF_SIZE];
        commandArray[0] = firstCommand;
        commandArray[1] = FWCMD_DOWNLOAD_DATA;
        commandArray[2] = (unsigned char) (3* thisWrite);
        for (int i = 0; i < thisWrite; i++)
        {
            commandArray[3 + (3 * i)] = (unsigned char)(downloadBytes >> 16);
            commandArray[4 + (3 * i)] = (unsigned char)(downloadBytes >> 8);
            commandArray[5 + (3 * i)] = (unsigned char)downloadBytes;
            
            downloadBytes += increment;
        }

        if (!writeUSB(commandArray, (3 * thisWrite) + 3))
        {
            return false;
        }
        
        firstCommand = FWCMD_NO_OPERATION;
    } while (multiples > 0);
    
    return true;
}

bool CPICkitFunctions::DownloadAddress3MSBFirst(int address)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArray[1] = FWCMD_DOWNLOAD_DATA;
    commandArray[2] = 3;
    commandArray[3] = (unsigned char )(0xFF & (address >> 16));
    commandArray[4] = (unsigned char )(0xFF & (address >> 8));
    commandArray[5] = (unsigned char )(address & 0xFF);
    
    return writeUSB(commandArray, 6);
}

int CPICkitFunctions::eeprom24BitAddress(int wordsWritten, bool setReadBit)
{
    if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == I2C_BUS)
    {      
        int tempAddress = wordsWritten;
        int address = 0;
        int chipSelects = DevFile.PartsList[ActivePart].ConfigMasks[CS_PINS_CFG];
        
        // I2C
        // Low & mid bytes
        address = wordsWritten & DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG] & 0xFFFF;
        // block address
        tempAddress >>= (DevFile.PartsList[ActivePart].ConfigMasks[ADR_BITS_CFG]);
        tempAddress <<= 17 + chipSelects; // 2 words plus R/W bit
        /*if (chipSelects > 0)   // always set to '0' in pk2cmd
        {
            if (checkBoxA0CS.Checked)
            {
                tempAddress |= 0x00020000;
            }
            if (checkBoxA1CS.Checked)
            {
                tempAddress |= 0x00040000;
            }
            if (checkBoxA2CS.Checked)
            {
                tempAddress |= 0x00080000;
            }
        }*/ 
        
        address += (tempAddress & 0x000E0000) + 0x00A00000;
        if (setReadBit)
        {
            address |= 0x00010000;
        }
        
        return address;
    }
    else if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == SPI_BUS)
    {
        int tempAddress = wordsWritten;
        int address = 0;
        
        //SPI
        // Low & Mid bytes
        if (DevFile.PartsList[ActivePart].ProgramMem <= 0x10000)
        {
            address = wordsWritten & DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG] & 0xFFFF;
            tempAddress >>= (DevFile.PartsList[ActivePart].ConfigMasks[ADR_BITS_CFG]);
            tempAddress <<= 19; // 2 words plus 3 instruction bits
            address += (tempAddress & 0x00080000) + 0x00020000; // write instruction
            if (setReadBit)
            {
                address |= 0x00010000; // add read instruction bit.
            }
        }
        else
        {
            address = wordsWritten;
        }

        return address;
    }
    else if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == MICROWIRE_BUS)
    {
        int tempAddress = 0x05; // start bit and write opcode
        int address = 0;

        // Microwire
        // Low & mid bytes
        address = wordsWritten & DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG] & 0xFFFF;
        if (setReadBit)
        {
            tempAddress = 0x06; // start bit and read opcode
        }
        tempAddress <<= (DevFile.PartsList[ActivePart].ConfigMasks[ADR_BITS_CFG]);
        
        address |= tempAddress;

        return address;
    }
    else if (DevFile.PartsList[ActivePart].ConfigMasks[PROTOCOL_CFG] == UNIO_BUS)
    {
        int address = 0;
        
        address = wordsWritten & DevFile.PartsList[ActivePart].ConfigMasks[ADR_MASK_CFG] & 0xFFFF;
        
        if (setReadBit)
        {
            address |= 0x030000; // READ command
        }
        else
        {
            address |= 0x6C0000; // WRITE command
        }
        return address;
    }
	return 0;
}

bool CPICkitFunctions::BusErrorCheck(void)
{
    int status = ReadPkStatus();
    if ((status & 0x0400) == 0x0400)
    {          
        return true; //error
    }

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = 1;
    commandArray[2] = SCMD_BUSY_LED_ON;
    writeUSB(commandArray, 3);  
    
    return false; // no error
}  

void CPICkitFunctions::DelaySeconds(unsigned int seconds)
{
    unsigned int i, j;

	usePercentTimer = false;  // always use rotating slash
	timerStart((_TCHAR*)" ", 1);
	for (i = 0; i < (5 * seconds); i++)
	{
		Sleep(200);
		for (j = 0; j < 8; j++)
		{
			timerPrint();
		}
	}
	timerStop();
}


unsigned int CPICkitFunctions::getEEBlank(void)
{
    unsigned int eeBlank = 0xFF;
    if (DevFile.Families[ActiveFamily].EEMemAddressIncrement > 1)
    {
        eeBlank = 0xFFFF;
    }
    if (DevFile.Families[ActiveFamily].BlankValue == 0xFFF)
    {
        eeBlank = 0xFFF;
    }
    return eeBlank;
}

bool CPICkitFunctions::ReadConfigOutsideProgMem(char function)
{
	bool ret = true;

    RunScript(SCR_PROG_ENTRY, 1);
    RunScript(SCR_CONFIG_RD, 1);
    UploadData();
    RunScript(SCR_PROG_EXIT, 1);
    int configWords = DevFile.PartsList[ActivePart].ConfigWords;
    int bufferIndex = 1;                    // report starts on index 1, which is #bytes uploaded.
    for (int word = 0; word < configWords; word++)
    {
        unsigned int config = (unsigned int)Usb_read_array[bufferIndex++];
        config |= (unsigned int)Usb_read_array[bufferIndex++] << 8;
        if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
        {
            config = (config >> 1) & DevFile.Families[ActiveFamily].BlankValue;
        }

		if (function == READ_MEM)
		{
			DeviceBuffers->ConfigWords[word] = config;	
		}
		else
		{ // BLANK_CHECK or VERIFY_MEM
			if ((DeviceBuffers->ConfigWords[word] & DevFile.PartsList[ActivePart].ConfigMasks[word])
					!= (config & DevFile.PartsList[ActivePart].ConfigMasks[word]))
			{ // location failed verify
				RunScript(SCR_PROG_EXIT, 1);
				VddOff();
				timerStop();
				_tcsncpy_s(ReadError.memoryType, "Configuration", 13);
				ReadError.address = (DevFile.PartsList[ActivePart].ConfigAddr / DevFile.Families[ActiveFamily].ProgMemHexBytes)
											* DevFile.Families[ActiveFamily].AddressIncrement;
				ReadError.address += (word * DevFile.Families[ActiveFamily].AddressIncrement);
				ReadError.expected = DeviceBuffers->ConfigWords[word] & DevFile.PartsList[ActivePart].ConfigMasks[word];
				ReadError.read = config & DevFile.PartsList[ActivePart].ConfigMasks[word];
				return false;
			}
		}
    }  

	return ret;
}

void CPICkitFunctions::timerStart(_TCHAR* operation, int steps)
{
    if (usePercentTimer) 
    {
        timerOperation = operation;
        timerValue = 0;

	 if (useTimerNewlines)
	{
		printf("\nOperation: %s\n", timerOperation);
        	printf("  0%%\n");
	}
	else
		printf("  0%% %s", timerOperation); 
        timerIncrement = (float)100 / (float)steps;
    }
    else
    {
        printf("\\");
    }
    fflush(stdout);
}

void CPICkitFunctions::timerPrint(void)    
{
	static char timer = '\\';
	static char count = 0;
	static float lastTimerValue = 0;

	if (usePercentTimer)
	{
		timerValue += timerIncrement;
		if ((int)lastTimerValue != (int)timerValue)
		{
            lastTimerValue = timerValue; // only print when the % changes
            if (useTimerNewlines)
            {
                printf("%3.0f%%\n", timerValue); // ++gib
            }
            else
            {
                printf("\r%3.f%% %s", timerValue, timerOperation);
            }
		fflush(stdout);
		}
	}
	else
	{
		if (count++ < 8) // slows it down a little
			return;
		else
			count = 0;

		switch (timer)
		{
			case '\\':
				timer = '|';
				break;

			case '|':
				timer = '/';
				break;

			case '/':
				timer = '-';
				break;

			default:
				timer = '\\';
				break;
		}
		printf("\b%c", timer);
	}
	fflush(stdout);
}

void CPICkitFunctions::timerStop(void)
{
	if (usePercentTimer)
	{
		if (!useTimerNewlines)
		{
			printf("\r                              \r");
		}
	}
	else
	{
		printf("\b \b");
	}
	fflush(stdout);
}

void CPICkitFunctions::ArrayCopy(unsigned char* source, int sourceStart, unsigned char* dest, int destStart, int length)    
{
	for (int i = 0; i < length; i++)
	{
		*(dest + destStart + i) = *(source + sourceStart + i);
	}
}

bool CPICkitFunctions::DownloadAddress3(int address)    
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArray[1] = FWCMD_DOWNLOAD_DATA;
    commandArray[2] = 3;
    commandArray[3] = (unsigned char)(address & 0xFF);
    commandArray[4] = (unsigned char)(0xFF & (address >> 8));
    commandArray[5] = (unsigned char)(0xFF & (address >> 16));
    return writeUSB(commandArray, 6);
}

bool CPICkitFunctions::RunScript(int script, int repetitions)
{
    // IMPORTANT NOTE: THIS ALWAYS CLEARS THE UPLOAD BUFFER FIRST!

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArray[1] = FWCMD_RUN_SCRIPT;
    commandArray[2] = scriptRedirectTable[script].redirectToScriptLocation;
    commandArray[3] = (unsigned char)repetitions;
    if (writeUSB(commandArray, 4))
    {
        return true;
    }
    else
    {
        return false;   
    }
} 

bool CPICkitFunctions::RunScriptUploadNoLen(int script, int repetitions)
{
    // IMPORTANT NOTE: THIS ALWAYS CLEARS THE UPLOAD BUFFER FIRST!

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArray[1] = FWCMD_RUN_SCRIPT;
    commandArray[2] = scriptRedirectTable[script].redirectToScriptLocation;
    commandArray[3] = (unsigned char)repetitions;
    commandArray[4] = FWCMD_UPLOAD_DATA_NOLEN;
    bool result = writeUSB(commandArray, 5);
    if (result)
    {
        result = readUSB();
    }
    return result;
}

/* deprecated - causes problems with MacOS X
bool CPICkitFunctions::RunScriptUploadNoLen2(int script, int repetitions)
{
    // IMPORTANT NOTE: THIS ALWAYS CLEARS THE UPLOAD BUFFER FIRST!

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArray[1] = FWCMD_RUN_SCRIPT;
    commandArray[2] = scriptRedirectTable[script].redirectToScriptLocation;
    commandArray[3] = (unsigned char)repetitions;
    commandArray[4] = FWCMD_UPLOAD_DATA_NOLEN;
    commandArray[5] = FWCMD_UPLOAD_DATA_NOLEN;
    bool result = writeUSB(commandArray, 6);
    if (result)
    {
        result = readUSB();
    }
    return result;
} */


bool CPICkitFunctions::UploadData(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_UPLOAD_DATA;
    bool result = writeUSB(commandArray, 1);
    if (result)
    {
        result = readUSB();
    }
    return result;
}


bool CPICkitFunctions::UploadDataNoLen(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_UPLOAD_DATA_NOLEN;
    bool result = writeUSB(commandArray, 1);
    if (result)
    {
        result = readUSB();
    }
    return result;
}

bool CPICkitFunctions::GetUpload(void)
{
    return readUSB();
}  

bool CPICkitFunctions::VddOn(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = 2;
    commandArray[2] = SCMD_VDD_GND_OFF;
	if (targetSelfPowered)
		commandArray[3] = SCMD_VDD_OFF;
	else
		commandArray[3] = SCMD_VDD_ON;
    bool result = writeUSB(commandArray, 4);
    return result;    
}

bool CPICkitFunctions::VddOff(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = 2;
    commandArray[2] = SCMD_VDD_OFF;
	if (targetSelfPowered)
		commandArray[3] = SCMD_VDD_GND_OFF;
	else
		commandArray[3] = SCMD_VDD_GND_ON;
    bool result = writeUSB(commandArray, 4);
    return result;
}

void CPICkitFunctions::PrepPICkit2(void)
{
	// Set VDD voltage
	SetVDDVoltage(vddSetPoint, (float)0.85);
	// Set VPP voltage (if vpp = 0, then set to Vdd)
	if (vppSetPoint < 1)
		vppSetPoint = vddSetPoint;
	SetVppVoltage(vppSetPoint, (float)0.7);
	// check for need to download scripts
	if (scriptBufferChecksum != getScriptBufferChecksum())
	{
		downloadPartScripts(ActiveFamily);
	}
	SetProgrammingSpeed(LastICSPSpeed);
}

bool CPICkitFunctions::SetVDDVoltage(float voltage, float threshold)
{
    if (voltage < 2.5F)
    {
        voltage = 2.5F;  // minimum, as when forcing VDD Target can get set very low (last reading)
                        // and too low prevents VPP pump from working.
    }
    unsigned short int ccpValue = (unsigned short int)(voltage * 32 + 10.5);
	ccpValue <<= 6;
    unsigned char vFault = (unsigned char)(((threshold * voltage) / 5) * 255);
    if (vFault > 210)
    {
        vFault = 210; // ~4.12v maximum.  Because of diode droop, limit threshold on high side.
    }
    
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_SETVDD;
    commandArray[1] = (unsigned char) (ccpValue & 0xFF);
    commandArray[2] = (unsigned char) (ccpValue / 256);
    commandArray[3] = vFault;
    return writeUSB(commandArray, 4);
}

bool CPICkitFunctions::SetVppVoltage(float voltage, float threshold)
{
    unsigned char ccpValue = 0x40;
    unsigned char vppADC = (unsigned char)(voltage * 18.61F);
    unsigned char vFault = (unsigned char)(threshold * voltage * 18.61F);

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_SETVPP;
    commandArray[1] = ccpValue;
    commandArray[2] = vppADC;
    commandArray[3] = vFault;
    return writeUSB(commandArray, 4);
}
        
void CPICkitFunctions::ResetBuffers(void)
{
    DeviceBuffers->ClearAllData(DevFile.PartsList[ActivePart].ProgramMem,
                               DevFile.PartsList[ActivePart].EEMem,
                               DevFile.PartsList[ActivePart].ConfigWords,
                               DevFile.PartsList[ActivePart].UserIDWords,
                               DevFile.Families[ActiveFamily].BlankValue,
                               DevFile.Families[ActiveFamily].EEMemAddressIncrement,
                               DevFile.Families[ActiveFamily].UserIDBytes,
                               DevFile.PartsList[ActivePart].ConfigBlank,
                               DevFile.PartsList[ActivePart].ConfigMasks[OSCCAL_MASK]);
}   

float CPICkitFunctions::GetDefaultVdd(void)
{ // reads default VDD for active part from device file int vddSetPoint
	vddSetPoint = DevFile.PartsList[ActivePart].VddMax;
	// 3.6 volt devices default to 3.3 V
	if ((vddSetPoint >= 3.59) && (vddSetPoint <= 3.61))
		vddSetPoint = (float)3.3;
	return vddSetPoint;
}

float CPICkitFunctions::GetDefaultVpp(void)
{ // reads default VPP for active part from device file into vppSetPoint
	vppSetPoint = DevFile.Families[ActiveFamily].Vpp;
	return vppSetPoint;
}

void CPICkitFunctions::SetVddSetPoint(float setPoint)
{ 
	vddSetPoint = setPoint;
}

void CPICkitFunctions::SetVppSetPoint(float setPoint)
{ 
	vppSetPoint = setPoint;
}

void CPICkitFunctions::SetVppFirstEnable(bool set)
{ 
	vppFirstEnabled = set;
}

bool CPICkitFunctions::GetVppFirstEnable(void)
{ 
	return vppFirstEnabled;
}

void CPICkitFunctions::SetSelfPowered(bool set)
{ 
	targetSelfPowered = set;
}

bool CPICkitFunctions::GetSelfPowered(void)
{ 
	return targetSelfPowered;
}

bool CPICkitFunctions::ReadPICkitVoltages(float* vdd, float* vpp)
{
    unsigned char  commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_READ_VOLTAGES;
    if (writeUSB(commandArray, 1))
    {
        if (readUSB())
        {
            float valueADC = (float)((Usb_read_array[1] * 256) + Usb_read_array[0]);
            *vdd = (valueADC / 65536) * 5.0F;
            valueADC = (float)((Usb_read_array[3] * 256) + Usb_read_array[2]);
            *vpp = (valueADC / 65536) * 13.7F;
            return true;
        }
    }
    return false;
}

bool CPICkitFunctions::DetectPICkit2Device(int unitNumber, bool readFWVer)
{
	bool result = false;
	unsigned char  commandArray[BUF_SIZE];
	unsigned char  usbReadArray[BUF_SIZE];
	
	// is there a device with this index?
	if (!m_Driver.FindTheHID(unitNumber))
	{ // no
		return false;
	}

	if (!readFWVer)
	{
		FirmwareVersion.major = 0;
		FirmwareVersion.minor = 0;
		FirmwareVersion.dot = unitNumber;
		return true;	// found device, but don't read FW
	}

	// Try to read firmware version
	commandArray[0] = FWCMD_FIRMWARE_VERSION;

	result = m_Driver.WriteReport((char *)commandArray, 1);
	if (result)
	{
		result = m_Driver.ReadReport((char *)usbReadArray);
		if (result)
		{
			FirmwareVersion.major = usbReadArray[0];
			FirmwareVersion.minor = usbReadArray[1];
			FirmwareVersion.dot = usbReadArray[2];
		}
	}
	return result;
}

void CPICkitFunctions::ClosePICkit2Device(void)
{
	m_Driver.CloseReport();
}

void CPICkitFunctions::KillSpace(_TCHAR* name, int maxlength)
{
	for (int i = 0; i < maxlength; i++)
	{
		if (*(name + i) == ' ')
		{
			*(name + i) = 0;
			i = maxlength;
		}
	}
}

bool CPICkitFunctions::FindDevice(_TCHAR* device)
{ 
	int i, x;
	bool result = false;
	for (i = 0; i < DevFile.Info.NumberParts; i++)
	{
		KillSpace(DevFile.PartsList[i].PartName, 28); // needed for 93 series eeprom names
		x=_tcsncmp(device, DevFile.PartsList[i].PartName, 28);
		if (x==0)
		{
			// found a match!
			ActivePart = i;
			ActiveFamily = DevFile.PartsList[i].Family;
			result = true;
			break;
		}
	}
	DeviceBuffers = &BlnkBuffs;
	ResetBuffers();
	DeviceBuffers = &DevBuffs;
	return result;
}

#ifndef WIN32
// Endian independent fread. This is meant to read from PK2DeviceFile.dat which is little endian.
void freadbin(void *inbuf, int size, int count, FILE *stream)
{
	int				i, j;
	unsigned int	data;
	unsigned char	temp;
	unsigned char	*bptr = (unsigned char *) inbuf;
	size_t dummyVar;

	for (i=0; i<count; i++)
	{
		data = 0;

		for (j=0; j<size; j++)
		{
			dummyVar = fread(&temp, 1, 1, stream);		// read one byte
			dummyVar++;

			data |= ((temp & 0xff) << j * 8);

		}

		switch (size)
		{
			case 1:
				*bptr = (unsigned char) data;
				bptr++;
				break;

			case 2:
				*(unsigned short *) bptr = (unsigned short) data;
				bptr += 2;
				break;

			case 4:
				*(unsigned int *) bptr = data;
				bptr += 4;
				break;

			default:
				fprintf(stderr, "INVALID SIZE IN freadbin: %d\n", size);
				fflush(stderr);
				break;
		}
	}
}

#define fread(a,b,c,d) freadbin(a,b,c,d)
#endif

bool CPICkitFunctions::ReadDeviceFile(_TCHAR* path)
{
	bool result = false;
	int i, j;
	FILE *datfile;
	errno_t err;

	if ((err = fopen_s(&datfile, path, "rb")) == 0)
	{	// Load device file params
		fread(&DevFile.Info.VersionMajor, sizeof(DevFile.Info.VersionMajor), 1, datfile);
		fread(&DevFile.Info.VersionMinor, sizeof(DevFile.Info.VersionMinor), 1, datfile);
		fread(&DevFile.Info.VersionDot, sizeof(DevFile.Info.VersionDot), 1, datfile);
		readString(datfile, DevFile.Info.VersionNotes);
		fread(&DevFile.Info.NumberFamilies, sizeof(DevFile.Info.NumberFamilies), 1, datfile);
		fread(&DevFile.Info.NumberParts, sizeof(DevFile.Info.NumberParts), 1, datfile);
		fread(&DevFile.Info.NumberScripts, sizeof(DevFile.Info.NumberScripts), 1, datfile);
		fread(&DevFile.Info.Compatibility, sizeof(DevFile.Info.Compatibility), 1, datfile);
		fread(&DevFile.Info.UNUSED1A, sizeof(DevFile.Info.UNUSED1A), 1, datfile);
		fread(&DevFile.Info.UNUSED1B, sizeof(DevFile.Info.UNUSED1B), 1, datfile);
		fread(&DevFile.Info.UNUSED2, sizeof(DevFile.Info.UNUSED2), 1, datfile);

		// load device family params
		for (i = 0; i < DevFile.Info.NumberFamilies; i++)
		{
			fread(&DevFile.Families[i].FamilyID, sizeof(DevFile.Families[i].FamilyID), 1, datfile);
			fread(&DevFile.Families[i].FamilyType, sizeof(DevFile.Families[i].FamilyType), 1, datfile);
			fread(&DevFile.Families[i].SearchPriority, sizeof(DevFile.Families[i].SearchPriority), 1, datfile);
			readString(datfile, DevFile.Families[i].FamilyName);
			fread(&DevFile.Families[i].ProgEntryScript, sizeof(DevFile.Families[i].ProgEntryScript), 1, datfile);
			fread(&DevFile.Families[i].ProgExitScript, sizeof(DevFile.Families[i].ProgExitScript), 1, datfile);
			fread(&DevFile.Families[i].ReadDevIDScript, sizeof(DevFile.Families[i].ReadDevIDScript), 1, datfile);
			fread(&DevFile.Families[i].DeviceIDMask, sizeof(DevFile.Families[i].DeviceIDMask), 1, datfile);
			fread(&DevFile.Families[i].BlankValue, sizeof(DevFile.Families[i].BlankValue), 1, datfile);
			fread(&DevFile.Families[i].BytesPerLocation, sizeof(DevFile.Families[i].BytesPerLocation), 1, datfile);
			fread(&DevFile.Families[i].AddressIncrement, sizeof(DevFile.Families[i].AddressIncrement), 1, datfile);
			fread(&DevFile.Families[i].PartDetect, sizeof(DevFile.Families[i].PartDetect), 1, datfile);
			fread(&DevFile.Families[i].ProgEntryVPPScript, sizeof(DevFile.Families[i].ProgEntryVPPScript), 1, datfile);
			fread(&DevFile.Families[i].UNUSED1, sizeof(DevFile.Families[i].UNUSED1), 1, datfile);
			fread(&DevFile.Families[i].EEMemBytesPerWord, sizeof(DevFile.Families[i].EEMemBytesPerWord), 1, datfile);
			fread(&DevFile.Families[i].EEMemAddressIncrement, sizeof(DevFile.Families[i].EEMemAddressIncrement), 1, datfile);
			fread(&DevFile.Families[i].UserIDHexBytes, sizeof(DevFile.Families[i].UserIDHexBytes), 1, datfile);
			fread(&DevFile.Families[i].UserIDBytes, sizeof(DevFile.Families[i].UserIDBytes), 1, datfile);
			fread(&DevFile.Families[i].ProgMemHexBytes, sizeof(DevFile.Families[i].ProgMemHexBytes), 1, datfile);
			fread(&DevFile.Families[i].EEMemHexBytes, sizeof(DevFile.Families[i].EEMemHexBytes), 1, datfile);
			fread(&DevFile.Families[i].ProgMemShift, sizeof(DevFile.Families[i].ProgMemShift), 1, datfile);
			fread(&DevFile.Families[i].TestMemoryStart, sizeof(DevFile.Families[i].TestMemoryStart), 1, datfile);
			fread(&DevFile.Families[i].TestMemoryLength, sizeof(DevFile.Families[i].TestMemoryLength), 1, datfile);
			fread(&DevFile.Families[i].Vpp, sizeof(DevFile.Families[i].Vpp), 1, datfile);
		}

		// load device part params
		for (i = 0; i < DevFile.Info.NumberParts; i++)
		{
			readString(datfile, DevFile.PartsList[i].PartName);
			string2Upper(DevFile.PartsList[i].PartName, 28);
			fread(&DevFile.PartsList[i].Family, sizeof(DevFile.PartsList[i].Family), 1, datfile);
			fread(&DevFile.PartsList[i].DeviceID, sizeof(DevFile.PartsList[i].DeviceID), 1, datfile);
			fread(&DevFile.PartsList[i].ProgramMem, sizeof(DevFile.PartsList[i].ProgramMem), 1, datfile);
			fread(&DevFile.PartsList[i].EEMem, sizeof(DevFile.PartsList[i].EEMem), 1, datfile);
			fread(&DevFile.PartsList[i].EEAddr, sizeof(DevFile.PartsList[i].EEAddr), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigWords, sizeof(DevFile.PartsList[i].ConfigWords), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigAddr, sizeof(DevFile.PartsList[i].ConfigAddr), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDWords, sizeof(DevFile.PartsList[i].UserIDWords), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDAddr, sizeof(DevFile.PartsList[i].UserIDAddr), 1, datfile);
			fread(&DevFile.PartsList[i].BandGapMask, sizeof(DevFile.PartsList[i].BandGapMask), 1, datfile);
			for (j = 0; j < 8; j++)
			{
				fread(&DevFile.PartsList[i].ConfigMasks[j], sizeof(DevFile.PartsList[i].ConfigMasks[j]), 1, datfile);
			}
			for (j = 0; j < 8; j++)
			{
				fread(&DevFile.PartsList[i].ConfigBlank[j], sizeof(DevFile.PartsList[i].ConfigBlank[j]), 1, datfile);
			}
			fread(&DevFile.PartsList[i].CPMask, sizeof(DevFile.PartsList[i].CPMask), 1, datfile);
			fread(&DevFile.PartsList[i].CPConfig, sizeof(DevFile.PartsList[i].CPConfig), 1, datfile);
			fread(&DevFile.PartsList[i].OSSCALSave, sizeof(DevFile.PartsList[i].OSSCALSave), 1, datfile);
			fread(&DevFile.PartsList[i].IgnoreAddress, sizeof(DevFile.PartsList[i].IgnoreAddress), 1, datfile);
			fread(&DevFile.PartsList[i].VddMin, sizeof(DevFile.PartsList[i].VddMin), 1, datfile);
			fread(&DevFile.PartsList[i].VddMax, sizeof(DevFile.PartsList[i].VddMax), 1, datfile);
			fread(&DevFile.PartsList[i].VddErase, sizeof(DevFile.PartsList[i].VddErase), 1, datfile);
			fread(&DevFile.PartsList[i].CalibrationWords, sizeof(DevFile.PartsList[i].CalibrationWords), 1, datfile);
			fread(&DevFile.PartsList[i].ChipEraseScript, sizeof(DevFile.PartsList[i].ChipEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemAddrSetScript, sizeof(DevFile.PartsList[i].ProgMemAddrSetScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemAddrBytes, sizeof(DevFile.PartsList[i].ProgMemAddrBytes), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemRdScript, sizeof(DevFile.PartsList[i].ProgMemRdScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemRdWords, sizeof(DevFile.PartsList[i].ProgMemRdWords), 1, datfile);
			fread(&DevFile.PartsList[i].EERdPrepScript, sizeof(DevFile.PartsList[i].EERdPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].EERdScript, sizeof(DevFile.PartsList[i].EERdScript), 1, datfile);
			fread(&DevFile.PartsList[i].EERdLocations, sizeof(DevFile.PartsList[i].EERdLocations), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDRdPrepScript, sizeof(DevFile.PartsList[i].UserIDRdPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDRdScript, sizeof(DevFile.PartsList[i].UserIDRdScript), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigRdPrepScript, sizeof(DevFile.PartsList[i].ConfigRdPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigRdScript, sizeof(DevFile.PartsList[i].ConfigRdScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemWrPrepScript, sizeof(DevFile.PartsList[i].ProgMemWrPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemWrScript, sizeof(DevFile.PartsList[i].ProgMemWrScript), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemWrWords, sizeof(DevFile.PartsList[i].ProgMemWrWords), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemPanelBufs, sizeof(DevFile.PartsList[i].ProgMemPanelBufs), 1, datfile);
			fread(&DevFile.PartsList[i].ProgMemPanelOffset, sizeof(DevFile.PartsList[i].ProgMemPanelOffset), 1, datfile);
			fread(&DevFile.PartsList[i].EEWrPrepScript, sizeof(DevFile.PartsList[i].EEWrPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].EEWrScript, sizeof(DevFile.PartsList[i].EEWrScript), 1, datfile);
			fread(&DevFile.PartsList[i].EEWrLocations, sizeof(DevFile.PartsList[i].EEWrLocations), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDWrPrepScript, sizeof(DevFile.PartsList[i].UserIDWrPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].UserIDWrScript, sizeof(DevFile.PartsList[i].UserIDWrScript), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigWrPrepScript, sizeof(DevFile.PartsList[i].ConfigWrPrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigWrScript, sizeof(DevFile.PartsList[i].ConfigWrScript), 1, datfile);
			fread(&DevFile.PartsList[i].OSCCALRdScript, sizeof(DevFile.PartsList[i].OSCCALRdScript), 1, datfile);
			fread(&DevFile.PartsList[i].OSCCALWrScript, sizeof(DevFile.PartsList[i].OSCCALWrScript), 1, datfile);
			fread(&DevFile.PartsList[i].DPMask, sizeof(DevFile.PartsList[i].DPMask), 1, datfile);
			fread(&DevFile.PartsList[i].WriteCfgOnErase, sizeof(DevFile.PartsList[i].WriteCfgOnErase), 1, datfile);
			fread(&DevFile.PartsList[i].BlankCheckSkipUsrIDs, sizeof(DevFile.PartsList[i].BlankCheckSkipUsrIDs), 1, datfile);
			fread(&DevFile.PartsList[i].IgnoreBytes, sizeof(DevFile.PartsList[i].IgnoreBytes), 1, datfile);
			fread(&DevFile.PartsList[i].ChipErasePrepScript, sizeof(DevFile.PartsList[i].ChipErasePrepScript), 1, datfile);
			fread(&DevFile.PartsList[i].BootFlash, sizeof(DevFile.PartsList[i].BootFlash), 1, datfile);
			//fread(&DevFile.PartsList[i].UNUSED4, sizeof(DevFile.PartsList[i].UNUSED4), 1, datfile); // Removed for compat level 6
			fread(&DevFile.PartsList[i].Config9Mask, sizeof(DevFile.PartsList[i].Config9Mask), 1, datfile); // Added for compat level 6
			DevFile.PartsList[i].ConfigMasks[8] = DevFile.PartsList[i].Config9Mask;
			fread(&DevFile.PartsList[i].Config9Blank, sizeof(DevFile.PartsList[i].Config9Blank), 1, datfile); // Added for compat level 6
			DevFile.PartsList[i].ConfigBlank[8] = DevFile.PartsList[i].Config9Blank;
			fread(&DevFile.PartsList[i].ProgMemEraseScript, sizeof(DevFile.PartsList[i].ProgMemEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].EEMemEraseScript, sizeof(DevFile.PartsList[i].EEMemEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].ConfigMemEraseScript, sizeof(DevFile.PartsList[i].ConfigMemEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].reserved1EraseScript, sizeof(DevFile.PartsList[i].reserved1EraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].reserved2EraseScript, sizeof(DevFile.PartsList[i].reserved2EraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].TestMemoryRdScript, sizeof(DevFile.PartsList[i].TestMemoryRdScript), 1, datfile);
			fread(&DevFile.PartsList[i].TestMemoryRdWords, sizeof(DevFile.PartsList[i].TestMemoryRdWords), 1, datfile);
			fread(&DevFile.PartsList[i].EERowEraseScript, sizeof(DevFile.PartsList[i].EERowEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].EERowEraseWords, sizeof(DevFile.PartsList[i].EERowEraseWords), 1, datfile);
			fread(&DevFile.PartsList[i].ExportToMPLAB, sizeof(DevFile.PartsList[i].ExportToMPLAB), 1, datfile);
			fread(&DevFile.PartsList[i].DebugHaltScript, sizeof(DevFile.PartsList[i].DebugHaltScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugRunScript, sizeof(DevFile.PartsList[i].DebugRunScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugStatusScript, sizeof(DevFile.PartsList[i].DebugStatusScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReadExecVerScript, sizeof(DevFile.PartsList[i].DebugReadExecVerScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugSingleStepScript, sizeof(DevFile.PartsList[i].DebugSingleStepScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugBulkWrDataScript, sizeof(DevFile.PartsList[i].DebugBulkWrDataScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugBulkRdDataScript, sizeof(DevFile.PartsList[i].DebugBulkRdDataScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugWriteVectorScript, sizeof(DevFile.PartsList[i].DebugWriteVectorScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReadVectorScript, sizeof(DevFile.PartsList[i].DebugReadVectorScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugRowEraseScript, sizeof(DevFile.PartsList[i].DebugRowEraseScript), 1, datfile);
			fread(&DevFile.PartsList[i].DebugRowEraseSize, sizeof(DevFile.PartsList[i].DebugRowEraseSize), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReserved5Script, sizeof(DevFile.PartsList[i].DebugReserved5Script), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReserved6Script, sizeof(DevFile.PartsList[i].DebugReserved6Script), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReserved7Script, sizeof(DevFile.PartsList[i].DebugReserved7Script), 1, datfile);
			fread(&DevFile.PartsList[i].DebugReserved8Script, sizeof(DevFile.PartsList[i].DebugReserved8Script), 1, datfile);
			//fread(&DevFile.PartsList[i].DebugReserved9Script, sizeof(DevFile.PartsList[i].DebugReserved9Script), 1, datfile); // Removed for compat level 6
			fread(&DevFile.PartsList[i].LVPScript, sizeof(DevFile.PartsList[i].LVPScript), 1, datfile); // Added for compat level 6
		}

		// load device scripts
		for (i = 0; i < DevFile.Info.NumberScripts; i++)
		{
			fread(&DevFile.Scripts[i].ScriptNumber, sizeof(DevFile.Scripts[i].ScriptNumber), 1, datfile);
			readString(datfile, DevFile.Scripts[i].ScriptName);
			fread(&DevFile.Scripts[i].ScriptVersion, sizeof(DevFile.Scripts[i].ScriptVersion), 1, datfile);
			fread(&DevFile.Scripts[i].UNUSED1, sizeof(DevFile.Scripts[i].UNUSED1), 1, datfile);
			fread(&DevFile.Scripts[i].ScriptLength, sizeof(DevFile.Scripts[i].ScriptLength), 1, datfile);
			for (j = 0; j < DevFile.Scripts[i].ScriptLength; j++)
			{
				fread(&DevFile.Scripts[i].Script[j], sizeof(DevFile.Scripts[i].Script[j]), 1, datfile);
			}
			readString(datfile, DevFile.Scripts[i].Comment);
		}

		result = true;
		fclose(datfile);
	}

	return result;
}

void CPICkitFunctions::readString(FILE *datfile, char stringArray[])
{
	unsigned char fileByte;
	int stringLength, i;

	// determine string length
	fread(&fileByte, sizeof(fileByte), 1, datfile);
	stringLength = fileByte & 0x7F;
	if (fileByte > 0x7F)
	{
		fread(&fileByte, sizeof(fileByte), 1, datfile);
		stringLength += fileByte * 0x80;
	}
	for (i = 0; i < stringLength; i++)
	{
		fread(&fileByte, sizeof(fileByte), 1, datfile);
		stringArray[i] = fileByte;
	}

	stringArray[i] = 0;

}

void CPICkitFunctions::downloadPartScripts(int familyIndex)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_CLR_SCRIPT_BUFFER;      // clear script buffer- we're loading new scripts
    writeUSB(commandArray, 1);
    
    // clear the script redirect table
    for (int i = 0; i < SCRIPT_REDIR_TABLE_LEN; i++)
    {
        scriptRedirectTable[i].redirectToScriptLocation = 0;
        scriptRedirectTable[i].deviceFileScriptNumber = 0;
    }

    // program entry
    if (DevFile.Families[familyIndex].ProgEntryScript != 0) // don't download non-existant scripts
    {
        if (vppFirstEnabled && (DevFile.Families[familyIndex].ProgEntryVPPScript != 0))
        { // download VPP first program mode entry
            downloadScript(SCR_PROG_ENTRY, DevFile.Families[familyIndex].ProgEntryVPPScript);
        }
        else
        { // standard program entry
            downloadScript(SCR_PROG_ENTRY, DevFile.Families[familyIndex].ProgEntryScript);
        }
    }
    // program exit
    if (DevFile.Families[familyIndex].ProgExitScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_PROG_EXIT, DevFile.Families[familyIndex].ProgExitScript);
    }
    // read device id
    if (DevFile.Families[familyIndex].ReadDevIDScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_RD_DEVID, DevFile.Families[familyIndex].ReadDevIDScript);
    }
    // read program memory
    if (DevFile.PartsList[ActivePart].ProgMemRdScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_PROGMEM_RD, DevFile.PartsList[ActivePart].ProgMemRdScript);
    }
    // chip erase prep
    if (DevFile.PartsList[ActivePart].ChipErasePrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_ERASE_CHIP_PREP, DevFile.PartsList[ActivePart].ChipErasePrepScript);
    }            
    // set program memory address
    if (DevFile.PartsList[ActivePart].ProgMemAddrSetScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_PROGMEM_ADDRSET, DevFile.PartsList[ActivePart].ProgMemAddrSetScript);
    }
    // prepare for program memory write
    if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_PROGMEM_WR_PREP, DevFile.PartsList[ActivePart].ProgMemWrPrepScript);
    }
    // program memory write                 
    if (DevFile.PartsList[ActivePart].ProgMemWrScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_PROGMEM_WR, DevFile.PartsList[ActivePart].ProgMemWrScript);
    }
    // prep for ee read               
    if (DevFile.PartsList[ActivePart].EERdPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_EE_RD_PREP, DevFile.PartsList[ActivePart].EERdPrepScript);
    }
    // ee read               
    if (DevFile.PartsList[ActivePart].EERdScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_EE_RD, DevFile.PartsList[ActivePart].EERdScript);
    }
    // prep for ee write               
    if (DevFile.PartsList[ActivePart].EEWrPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_EE_WR_PREP, DevFile.PartsList[ActivePart].EEWrPrepScript);
    }
    // ee write               
    if (DevFile.PartsList[ActivePart].EEWrScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_EE_WR, DevFile.PartsList[ActivePart].EEWrScript);
    }
    // prep for config read       
    if (DevFile.PartsList[ActivePart].ConfigRdPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_CONFIG_RD_PREP, DevFile.PartsList[ActivePart].ConfigRdPrepScript);
    }
    // config read       
    if (DevFile.PartsList[ActivePart].ConfigRdScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_CONFIG_RD, DevFile.PartsList[ActivePart].ConfigRdScript);
    }
    // prep for config write       
    if (DevFile.PartsList[ActivePart].ConfigWrPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_CONFIG_WR_PREP, DevFile.PartsList[ActivePart].ConfigWrPrepScript);
    }
    // config write       
    if (DevFile.PartsList[ActivePart].ConfigWrScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_CONFIG_WR, DevFile.PartsList[ActivePart].ConfigWrScript);
    }
    // prep for user id read      
    if (DevFile.PartsList[ActivePart].UserIDRdPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_USERID_RD_PREP, DevFile.PartsList[ActivePart].UserIDRdPrepScript);
    }
    // user id read      
    if (DevFile.PartsList[ActivePart].UserIDRdScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_USERID_RD, DevFile.PartsList[ActivePart].UserIDRdScript);
    }
    // prep for user id write      
    if (DevFile.PartsList[ActivePart].UserIDWrPrepScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_USERID_WR_PREP, DevFile.PartsList[ActivePart].UserIDWrPrepScript);
    }
    // user id write      
    if (DevFile.PartsList[ActivePart].UserIDWrScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_USERID_WR, DevFile.PartsList[ActivePart].UserIDWrScript);
    }
    // read osscal      
    if (DevFile.PartsList[ActivePart].OSCCALRdScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_OSSCAL_RD, DevFile.PartsList[ActivePart].OSCCALRdScript);
    }
    // write osscal      
    if (DevFile.PartsList[ActivePart].OSCCALWrScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_OSSCAL_WR, DevFile.PartsList[ActivePart].OSCCALWrScript);
    }
    // chip erase      
    if (DevFile.PartsList[ActivePart].ChipEraseScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_ERASE_CHIP, DevFile.PartsList[ActivePart].ChipEraseScript);
    }
    // program memory erase 
    if (DevFile.PartsList[ActivePart].ProgMemEraseScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_ERASE_PROGMEM, DevFile.PartsList[ActivePart].ProgMemEraseScript);
    }
    // ee erase 
    if (DevFile.PartsList[ActivePart].EEMemEraseScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_ERASE_EE, DevFile.PartsList[ActivePart].EEMemEraseScript);
    }
    // row erase
    if (DevFile.PartsList[ActivePart].DebugRowEraseScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_ROW_ERASE, DevFile.PartsList[ActivePart].DebugRowEraseScript);
    }            
    // EE Row Erase
    if (DevFile.PartsList[ActivePart].EERowEraseScript != 0) // don't download non-existant scripts
    {
        downloadScript(SCR_EEROW_ERASE, DevFile.PartsList[ActivePart].EERowEraseScript);
    }            
    
    // get script buffer checksum
    scriptBufferChecksum = getScriptBufferChecksum();
}

unsigned int CPICkitFunctions::getScriptBufferChecksum(void)
{
	unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_SCRIPT_BUFFER_CHKSM;
    if (writeUSB(commandArray, 1))
    {
        if (readUSB())
        {
            unsigned int checksum = (unsigned int)Usb_read_array[4];
            checksum += (unsigned int)(Usb_read_array[3] << 8);
            checksum += (unsigned int)(Usb_read_array[2] << 16);
            checksum += (unsigned int)(Usb_read_array[1] << 24);
            
            return checksum;
        }
        return 0;
    }
    return 0;        
}

bool CPICkitFunctions::downloadScript(unsigned char scriptBufferLocation, int scriptArrayIndex)
{        
	unsigned char commandArray[BUF_SIZE];
	int	cmdOffset;
    // see if we've already downloaded the script.  Some devices use the same script
    // for different functions.  Not downloading it several times saves space in the script buffer
    unsigned char redirectTo = scriptBufferLocation;  // default doesn't redirect; calls itself
    for (int i = 0; i < SCRIPT_REDIR_TABLE_LEN; i++)
    {
        if (scriptArrayIndex == scriptRedirectTable[i].deviceFileScriptNumber)
        {
            redirectTo = (unsigned int)i; // redirect to this buffer location
            break;
        }
    }
    scriptRedirectTable[scriptBufferLocation].redirectToScriptLocation = redirectTo; // set redirection
    scriptRedirectTable[scriptBufferLocation].deviceFileScriptNumber = scriptArrayIndex;
    // note: since the FOR loop above always finds the first instance of a script, we don't have to
    // worry about redirecting to a redirect.
    if (scriptBufferLocation != redirectTo)
    {  // we're redirecting
        return true;  // we're all done
    }

    int scriptLength = DevFile.Scripts[--scriptArrayIndex].ScriptLength;

	cmdOffset = 0;
    commandArray[cmdOffset++] = FWCMD_DOWNLOAD_SCRIPT;
    commandArray[cmdOffset++] = scriptBufferLocation;
    commandArray[cmdOffset++] = (unsigned char)scriptLength;
    for (int n = 0; n < scriptLength; n++)
    {
        unsigned short int scriptEntry = DevFile.Scripts[scriptArrayIndex].Script[n];
        commandArray[cmdOffset++] = (unsigned char)scriptEntry;
    }
    return writeUSB(commandArray, cmdOffset);
}

bool CPICkitFunctions::writeUSB(unsigned char commandList[], int commandLength)
{
	for (int i = 0; i < BUF_SIZE; i++)
	{
		if (i < commandLength)
			Usb_write_array[i] = commandList[i];
		else
			Usb_write_array[i] = FWCMD_END_OF_BUFFER;
	}

	Usb_write_array[64] = 0;
	return m_Driver.WriteReport((char *)Usb_write_array, 64);
}

bool CPICkitFunctions::readUSB(void)
{
	return m_Driver.ReadReport((char *)Usb_read_array);
}

void CPICkitFunctions::USBClose(void)
{
	m_Driver.CloseReport();
}

void CPICkitFunctions::string2Upper(_TCHAR* lcstring, int maxLength)
{
	for (int i = 0; i < maxLength; i++)
	{
		if (*(lcstring + i) == 0)
			break;
		else
			*(lcstring + i) = _totupper(*(lcstring + i));
	}
}

bool CPICkitFunctions::EnterBootloader(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_ENTER_BOOTLOADER;
    return writeUSB(commandArray, 1);
} 

bool CPICkitFunctions::VerifyBootloaderMode(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_FIRMWARE_VERSION;
    if (writeUSB(commandArray, 1))
    {
        if (readUSB())
        {
            if (Usb_read_array[0] == 118) // ASCII 'B'
            {
                return true;
            }
            return false;
        }
        return false;
    }
    return false;
}

bool CPICkitFunctions::BL_EraseFlash(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = BLCMD_ERASEFWFLASH;
    commandArray[1] = 0xC0;
    commandArray[2] = 0x00;
    commandArray[3] = 0x20;
    commandArray[4] = 0x00;
    if (writeUSB(commandArray, 5))
    {
        commandArray[3] = 0x50;
        return writeUSB(commandArray, 5);
    }
    return false;
}

bool CPICkitFunctions::BL_WriteFlash(unsigned char payload[])
{
	unsigned char commandArray[BUF_SIZE];
    commandArray[0] = BLCMD_WRITEFWFLASH;
    commandArray[1] = 32;
    for (int i=0; i< 35; i++)
    {
        commandArray[2+i] = payload[i];
    }
    return writeUSB(commandArray, 2+35);

}
        
bool CPICkitFunctions::BL_WriteFWLoadedKey(void)
{
    // 3 address bytes plus 32 data bytes.
	unsigned char flashWriteData[35];
    flashWriteData[0] = 0xE0;
    flashWriteData[1] = 0x7F;
    flashWriteData[2] = 0x00;   // Address = 0x007FE0
    for (int i = 3; i < 35; i++)
    {
        flashWriteData[i] = 0xFF;
    }
    flashWriteData[35 - 2] = 0x55;
    flashWriteData[35 - 1] = 0x55;
    return BL_WriteFlash(flashWriteData);
}

bool CPICkitFunctions::BL_ReadFlash16(int address)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = BLCMD_READFWFLASH;
    commandArray[1] = 16;
    commandArray[2] = (unsigned char)(address & 0xFF);
    commandArray[3] = (unsigned char)((address >> 8) & 0xFF);
    commandArray[4] = 0x00;
    if (writeUSB(commandArray, 5))
    {
        return readUSB();
    }
    return false;

}

bool CPICkitFunctions::BL_Reset(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = BLCMD_RESETFWDEVICE;

    return writeUSB(commandArray, 1);

}

bool CPICkitFunctions::UnitIDWrite(_TCHAR* unitID)
{
    int length;
	for (length = 0; length < 14; length++)
	{
		if (unitID[length] == 0)
			break;
	}
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_WR_INTERNAL_EE;
    commandArray[1] = EE_UNIT_ID;
    commandArray[2] = 0x10;
    if (length > 0)
    {
        commandArray[3] = 0x23; // '#' first byte is always ASCII pound to indicate valid UnitID
    }
    else
    {
        commandArray[3] = 0xFF; // clear UnitID
    }

    for (int i = 0; i < 14; i++)
    {
        if (i < length)
        {
            commandArray[4 + i] = unitID[i];
        }
        else
        {
            commandArray[4 + i] = 0;
        }
    }
    
    return writeUSB(commandArray, (4+16));

}

bool CPICkitFunctions::UnitIDRead(_TCHAR* unitID)
{ // returns a zero-length string if no ID.
	 int i;

    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_RD_INTERNAL_EE;
    commandArray[1] = EE_UNIT_ID;
    commandArray[2] = 16; 
    if (writeUSB(commandArray, 3))
    {
        if (readUSB())
        {
            if (Usb_read_array[0] == 0x23)
            {
                for (i = 0; i < 15; i++)
                {
					unitID[i] = Usb_read_array[1 + i];
                    if (Usb_read_array[1 + i] == 0)
                    {
                        break;
                    }
                }
				return true;
            }
        
        }
    }

    return false;        
}

_TCHAR* CPICkitFunctions::GetUnitID(void)
{
	return m_Driver.GetPK2UnitID();
}

bool CPICkitFunctions::ResetPICkit2(void)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_RESET;

    return writeUSB(commandArray, 1);

}

void CPICkitFunctions::writeConfigInsideProgramMem(void)
{
	int lastBlock;
	int word, bite;
	unsigned int memWord;

    RunScript(SCR_PROG_ENTRY, 1);
    lastBlock = DevFile.PartsList[ActivePart].ProgramMem -
                    DevFile.PartsList[ActivePart].ProgMemWrWords;
    if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
    { // if prog mem address set script exists for this part
        DownloadAddress3(lastBlock * DevFile.Families[ActiveFamily].AddressIncrement);
        RunScript(SCR_PROGMEM_WR_PREP, 1);
    }
    unsigned char downloadBuffer[DOWNLOAD_BUFFER_SIZE];
    int downloadIndex = 0;
    for (word = 0; word < DevFile.PartsList[ActivePart].ProgMemWrWords; word++)
    {
        memWord = DeviceBuffers->ProgramMemory[lastBlock++];
        if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
        {
            memWord = memWord << 1;
        }
        downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);
        for (bite = 1; bite < DevFile.Families[ActiveFamily].BytesPerLocation; bite++)
        {
            memWord >>= 8;
            downloadBuffer[downloadIndex++] = (unsigned char)(memWord & 0xFF);
        }

    }
    // download data
    int dataIndex = DataClrAndDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, 0);
    while (dataIndex < downloadIndex)
    {
        dataIndex = DataDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, dataIndex);
    }

    RunScript(SCR_PROGMEM_WR, 1);
    RunScript(SCR_PROG_EXIT, 1);
}

bool CPICkitFunctions::SetProgrammingSpeed(unsigned char speed)
{
    unsigned char commandArray[BUF_SIZE];
    commandArray[0] = FWCMD_EXECUTE_SCRIPT;
    commandArray[1] = 2;
    commandArray[2] = SCMD_SET_ICSP_SPEED; 
    commandArray[3] = speed;
    return writeUSB(commandArray, 4);
}

void CPICkitFunctions::SetProgrammingSpeedDefault(unsigned char speed)
{
	LastICSPSpeed = speed;
}

bool CPICkitFunctions::FamilyIsPIC24H(void)
        {
			int x = _tcsncmp(DevFile.PartsList[ActivePart].PartName, "PIC24H", 6);
			if (x == 0)
				return true;
			return false;
        }

bool CPICkitFunctions::FamilyIsdsPIC33F(void)
        {
			int x = _tcsncmp(DevFile.PartsList[ActivePart].PartName, "DSPIC33F", 8);
			if (x == 0)
				return true;
			return false;
        }  

bool CPICkitFunctions::FamilyIsPIC24F(void)
        {
			int x = _tcsncmp(DevFile.PartsList[ActivePart].PartName, "PIC24F", 6);
			if (x == 0)
				return true;
			return false;
        }

void CPICkitFunctions::SetTimerFunctions(bool usePercent, bool useNewlines)
{
	usePercentTimer = usePercent; // give progress info with % completion instead of rotating backslash
	useTimerNewlines = useNewlines; // each update is on a newline - intended for GUI interfaces.
}

bool CPICkitFunctions::SearchDevice(int familyIndex)
        {
			ActiveFamily = familyIndex;
			// set VDD voltage:
			SetVDDVoltage(vddSetPoint, (float)0.85);

            // Set VPP voltage by family
			GetDefaultVpp();
            if (vppSetPoint < 1)  
            { // When nominally zero, use VDD voltage
                SetVppVoltage(vddSetPoint, 0.7F);       
            }
            else
            {
                SetVppVoltage(vppSetPoint, 0.7F);
            }

			SetProgrammingSpeed(LastICSPSpeed);
            
			SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
			VddOn();
			Sleep(50);

            // use direct execute scripts when checking for a part
            if (vppFirstEnabled && (DevFile.Families[familyIndex].ProgEntryVPPScript > 0))
            {
                ExecuteScript(DevFile.Families[familyIndex].ProgEntryVPPScript);
            }
            else
            {
                ExecuteScript(DevFile.Families[familyIndex].ProgEntryScript);
            }
            ExecuteScript(DevFile.Families[familyIndex].ReadDevIDScript);
            UploadData();
            ExecuteScript(DevFile.Families[familyIndex].ProgExitScript);
            
            // Turn off Vdd 
			VddOff();
			SetMCLR(false); 

            // NOTE: parts that only return 2 bytes for DevID will have junk in upper word.  This is OK - it gets masked off
            unsigned int deviceID = (unsigned int)(Usb_read_array[4] * 0x1000000 + Usb_read_array[3] * 0x10000 + Usb_read_array[2] * 256 + Usb_read_array[1]);
            for (int shift = 0; shift < DevFile.Families[familyIndex].ProgMemShift; shift++)
            {
                deviceID >>= 1;         // midrange/baseline part results must be shifted by 1
            }
            deviceID &= (unsigned int)DevFile.Families[familyIndex].DeviceIDMask; // mask off version bits.

            // Search through the device file to see if we find the part
            ActivePart = 0; // no device is default.
            for (int partEntry = 0; partEntry < DevFile.Info.NumberParts; partEntry++)
            {
                if (DevFile.PartsList[partEntry].Family == familyIndex)
                { // don't check device ID if in a different family
                    if (DevFile.PartsList[partEntry].DeviceID == deviceID)
                    {
                        ActivePart = partEntry;
                        break;  // found a match - get out of the loop.
                    }
                }
            }

            if (ActivePart == 0) // not a known part
            {  
                return false;   // we're done
            }

            return true;

        }

unsigned int CPICkitFunctions::ReadVector(void)
{
	SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

	RunScript(SCR_PROG_ENTRY, 1);
	ExecuteScript(DevFile.PartsList[ActivePart].DebugReadVectorScript);
	UploadData();
	RunScript(SCR_PROG_EXIT, 1);
    unsigned int vector = (unsigned int)(Usb_read_array[2] * 256 + Usb_read_array[1]);
    for (int shift = 0; shift < DevFile.Families[ActiveFamily].ProgMemShift; shift++)
    {
        vector >>= 1;         // midrange/baseline part results must be shifted by 1
    }
	vector &= DevFile.Families[ActiveFamily].BlankValue;

	unsigned int vectortop = (unsigned int)(Usb_read_array[4] * 256 + Usb_read_array[3]);
    for (int shift = 0; shift < DevFile.Families[ActiveFamily].ProgMemShift; shift++)
    {
        vectortop >>= 1;         // midrange/baseline part results must be shifted by 1
    }
	vectortop &= DevFile.Families[ActiveFamily].BlankValue;
	vector |= (vectortop << 16);

	VddOff();
	SetMCLR(false);   

	return vector;
}

void CPICkitFunctions::WriteVector(int vtop, int vbot)
{
	SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

	RunScript(SCR_PROG_ENTRY, 1);
    if (DevFile.Families[ActiveFamily].ProgMemShift > 0)
    {
        vtop = vtop << 1;
		vbot = vbot << 1;
    }
	unsigned char downloadBuffer[4];
	downloadBuffer[0] = (unsigned char)(vbot & 0xFF);
	downloadBuffer[1] = (unsigned char)((vbot >> 8) & 0xFF);
	downloadBuffer[2] = (unsigned char)(vtop & 0xFF);
	downloadBuffer[3] = (unsigned char)((vtop >> 8) & 0xFF);
	DataClrAndDownload(downloadBuffer, 4, 0);

	ExecuteScript(DevFile.PartsList[ActivePart].DebugWriteVectorScript);

	RunScript(SCR_PROG_EXIT, 1);

	VddOff();
	SetMCLR(false);   
}

/*********************************************************************************************
 *********************************************************************************************
 *****                              PIC32MX Functions                                    *****
 *********************************************************************************************
 ********************************************************************************************/


void CPICkitFunctions::EnterSerialExecution(void)
{ // assumes already in programming mode
    int commOffSet = 0;

    unsigned char commandArrayp[BUF_SIZE];
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 27;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x04;                 // MTAP_SW_MTAP
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x07;                 // MTAP_COMMAND
    commandArrayp[commOffSet++] = SCMD_JT2_XFERDATA8_LIT;
    commandArrayp[commOffSet++] = 0x00;                 // MCHP_STATUS
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x04;                 // MTAP_SW_MTAP
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x07;                 // MTAP_COMMAND
    commandArrayp[commOffSet++] = SCMD_JT2_XFERDATA8_LIT;
    commandArrayp[commOffSet++] = 0xD1;                 // MCHP_ASSERT_RST
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x05;                 // MTAP_SW_ETAP
    commandArrayp[commOffSet++] = SCMD_JT2_SETMODE;
    commandArrayp[commOffSet++] = 6;
    commandArrayp[commOffSet++] = 0x1F;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0C;                 // ETAP_EJTAGBOOT
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x04;                 // MTAP_SW_MTAP       
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x07;                 // MTAP_COMMAND
    commandArrayp[commOffSet++] = SCMD_JT2_XFERDATA8_LIT;
    commandArrayp[commOffSet++] = 0xD0;                 // MCHP_DE_ASSERT_RST
    commandArrayp[commOffSet++] = SCMD_JT2_XFERDATA8_LIT;
    commandArrayp[commOffSet++] = 0xFE;                 // MCHP_EN_FLASH            

    writeUSB(commandArrayp, commOffSet);
}


bool CPICkitFunctions::DownloadPE(void)
{
	CPIC32PE	P32PE;

    // Serial execution mode must already be entered
    int commOffSet = 0;
    unsigned char commandArrayp[BUF_SIZE];
    commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
    commandArrayp[commOffSet++] = 28;
    // STEP 1
    commOffSet = addInstruction(commandArrayp, 0x3c04bf88, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x34842000, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x3c05001f, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x34a50040, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0xac850000, commOffSet);
    // STEP 2
    commOffSet = addInstruction(commandArrayp, 0x34050800, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0xac850010, commOffSet);
    // execute
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 12;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x05;                 // MTAP_SW_ETAP
    commandArrayp[commOffSet++] = SCMD_JT2_SETMODE;
    commandArrayp[commOffSet++] = 6;
    commandArrayp[commOffSet++] = 0x1F;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return false;           // yes - abort
    }

    commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
    commandArrayp[commOffSet++] = 20;
    // STEP 3
    commOffSet = addInstruction(commandArrayp, 0x34058000, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0xac850020, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0xac850030, commOffSet);
    // STEP 4
    commOffSet = addInstruction(commandArrayp, 0x3c04a000, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x34840800, commOffSet);
    // execute
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 5;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return false;           // yes - abort
    }
    
    // Download the PE loader
    for (int i = 0; i < K_PE_LOADER_LEN; i+=2)
    {
        commOffSet = 0;
        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
        commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
        commandArrayp[commOffSet++] = 16;
        // STEP 5
        commOffSet = addInstruction(commandArrayp, (0x3c060000 | P32PE.pe_Loader[i]), commOffSet);
        commOffSet = addInstruction(commandArrayp, (0x34c60000 | P32PE.pe_Loader[i + 1]), commOffSet);
        commOffSet = addInstruction(commandArrayp, 0xac860000, commOffSet);
        commOffSet = addInstruction(commandArrayp, 0x24840004, commOffSet);
        // execute
        commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
        commandArrayp[commOffSet++] = 4;
        commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
        writeUSB(commandArrayp, commOffSet);
        if (BusErrorCheck())    // Any timeouts?
        {
            return false;           // yes - abort
        }
    }

    // jump to PE loader
    commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
    commandArrayp[commOffSet++] = 16;
    // STEP 6
    commOffSet = addInstruction(commandArrayp, 0x3c19a000, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x37390800, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x03200008, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0x00000000, commOffSet);
    // execute
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 21;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFERINST_BUF;
    // STEP 7-A
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x05;                 // MTAP_SW_ETAP
    commandArrayp[commOffSet++] = SCMD_JT2_SETMODE;
    commandArrayp[commOffSet++] = 6;
    commandArrayp[commOffSet++] = 0x1F;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = 0x00;                 // PE_ADDRESS = 0xA000_0900
    commandArrayp[commOffSet++] = 0x09;
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0xA0;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(K_PE_LEN & 0xFF);// PE_SIZE 
    commandArrayp[commOffSet++] = (unsigned char)((K_PE_LEN >> 8) & 0xFF);
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x00; 
    
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return false;           // yes - abort
    }

    // Download the PE itself (STEP 7-B)
    int numLoops = K_PE_LEN / 10;
    for (int i = 0, j = 0; i < numLoops; i++)
    { // download 10 at a time
        commOffSet = 0;
        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
        commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
        commandArrayp[commOffSet++] = 40;
        // download the PE instructions
        j = i * 10;
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 1], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 2], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 3], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 4], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 5], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 6], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 7], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 8], commOffSet);
        commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[j + 9], commOffSet);
        // execute
        commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
        commandArrayp[commOffSet++] = 10;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        writeUSB(commandArrayp, commOffSet);
        if (BusErrorCheck())    // Any timeouts?
        {
            return false;           // yes - abort
        }
    }
    // Download the remaining words
    Sleep(100);
    int arrayOffset = numLoops * 10;
	int i;
    numLoops = K_PE_LEN % 10;
    if (numLoops > 0)
    {
        commOffSet = 0;
        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
        commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
        commandArrayp[commOffSet++] = (unsigned char)(4 * numLoops);
        // download the PE instructions
        for (i = 0; i < numLoops; i++)
        {
            commOffSet = addInstruction(commandArrayp, P32PE.PIC32_PE[i + arrayOffset], commOffSet);
        }
        // execute
        commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
        commandArrayp[commOffSet++] = (unsigned char)numLoops;
        for (i = 0; i < numLoops; i++)
        {            
            commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
        }
        writeUSB(commandArrayp, commOffSet);
        if (BusErrorCheck())    // Any timeouts?
        {
            return false;           // yes - abort
        } 
    }
    
    // STEP 8 - Jump to PE
    commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
    commandArrayp[commOffSet++] = 8;
    // download the PE instructions
    commOffSet = addInstruction(commandArrayp, 0x00000000, commOffSet);
    commOffSet = addInstruction(commandArrayp, 0xDEAD0000, commOffSet);
    // execute
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 2;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_BUF;

    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return false;           // yes - abort
    }
    Sleep(100);
    return true;
}

int CPICkitFunctions::ReadPEVersion(void)
{
    unsigned char commandArrayp[BUF_SIZE];
    int commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 8;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = 0x00;     // Length = 0
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x07;     // EXEC_VERSION
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = SCMD_JT2_GET_PE_RESP;
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return 0;           // yes - abort
    }
    if (!UploadData())
    {
        return 0;
    }
    int version = (Usb_read_array[3] + (Usb_read_array[4] * 0x100));
    if (version != 0x0007) // command echo
    {
        return 0;
    }
    version = (Usb_read_array[1] + (Usb_read_array[2] * 0x100));
    return version;
}

bool CPICkitFunctions::PEBlankCheck(unsigned int startAddress, unsigned int lengthBytes)
{
    unsigned char commandArrayp[BUF_SIZE];
    int commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 18;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = 0x00;     
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x06;     // BLANK_CHECK
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(startAddress & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 24) & 0xFF);              
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(lengthBytes & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 24) & 0xFF);                                
    commandArrayp[commOffSet++] = SCMD_JT2_GET_PE_RESP;
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return false;           // yes - abort
    }
    if (!UploadData())
    {
        return false;
    }
    if ((Usb_read_array[3] != 6) || (Usb_read_array[1] != 0)) // response code 0 = success
    {
        return false;
    }

    return true;
}

int CPICkitFunctions::PEGetCRC(unsigned int startAddress, unsigned int lengthBytes)
{
    unsigned char commandArrayp[BUF_SIZE];
    int commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 19;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x08;     // GET_CRC
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(startAddress & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 24) & 0xFF);
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(lengthBytes & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 24) & 0xFF);
    commandArrayp[commOffSet++] = SCMD_JT2_GET_PE_RESP;
    commandArrayp[commOffSet++] = SCMD_JT2_GET_PE_RESP;
    writeUSB(commandArrayp, commOffSet);
    if (BusErrorCheck())    // Any timeouts?
    {
        return 0;           // yes - abort
    }
    if (!UploadData())
    {
        return 0;
    }
    if ((Usb_read_array[3] != 8) || (Usb_read_array[1] != 0)) // response code 0 = success
    {
        return 0;
    }

    int crc = (int)(Usb_read_array[5] + (Usb_read_array[6] << 8));

    return crc;
}

int CPICkitFunctions::addInstruction(unsigned char* commandarray, unsigned int instruction, int offset)
{
    commandarray[offset++] = (unsigned int)(instruction & 0xFF);
    commandarray[offset++] = (unsigned int)((instruction >> 8) & 0xFF);
    commandarray[offset++] = (unsigned int)((instruction >> 16) & 0xFF);
    commandarray[offset++] = (unsigned int)((instruction >> 24) & 0xFF);
    return offset;
}
        
bool CPICkitFunctions::PE_DownloadAndConnect(void)
{
    // VDD must already be on!
    printf("Downloading Programming Executive...\n");
	fflush(stdout);
    
    RunScript(SCR_PROG_ENTRY, 1);
    UploadData();
    if ((Usb_read_array[1] & 0x80) == 0)
    {
        printf("Device is Code Protected and must be Erased first.\n");
		fflush(stdout);
        RunScript(SCR_PROG_EXIT, 1);
        return false;
    }
    
    EnterSerialExecution();
    DownloadPE();
    int PEVersion = ReadPEVersion();
    if (PEVersion != K_PIC32_PE_VERSION)
    {
        printf("...FAILED\n");
		fflush(stdout);
        RunScript(SCR_PROG_EXIT, 1);
        return false;
    }
	printf("...done\n");
	fflush(stdout);
    return true;
}

bool CPICkitFunctions::PIC32Read(bool progmem, bool uidmem, bool cfgmem)
{
	int i,j;
	int word, bite;
	unsigned char commandArrayp[BUF_SIZE];
	int commOffSet;

    SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();      
    
    if (!PE_DownloadAndConnect())
    {
        return false;
    }  

    unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];

    int progMemP32 = (int)DevFile.PartsList[ActivePart].ProgramMem;
    int bootMemP32 = (int)DevFile.PartsList[ActivePart].BootFlash;
    progMemP32 -= bootMemP32; // boot flash at upper end of prog mem.

    // Read Program Memory =====================================================================================  
	int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
	int scriptRunsToFillUpload = UPLOAD_BUFFER_SIZE /
		(DevFile.PartsList[ActivePart].ProgMemRdWords * bytesPerWord);
	int wordsPerLoop = scriptRunsToFillUpload * DevFile.PartsList[ActivePart].ProgMemRdWords;
	int wordsRead = 0;
	int uploadIndex = 0;
	if (progmem)
	{
		timerStart((_TCHAR*)"Read Flash", progMemP32 / wordsPerLoop);
		do
		{
			// Download address for up to 15 script runs.
			int runs = (progMemP32 - wordsRead) / wordsPerLoop;
			if (runs > 15)
				runs = 15;
			unsigned int address = (unsigned int)(wordsRead * bytesPerWord) + K_P32_PROGRAM_FLASH_START_ADDR;
			commOffSet = 0;
			commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
			commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
			commandArrayp[commOffSet++] = (unsigned char)(runs * 4);
			for (i = 0; i < runs; i++)
			{
				commOffSet = addInstruction(commandArrayp, (address + (unsigned int)(i * DevFile.PartsList[ActivePart].ProgMemRdWords * bytesPerWord)), commOffSet);
			}
			writeUSB(commandArrayp, commOffSet);
	    
			for (j = 0; j < runs; j++)
			{
				//RunScriptUploadNoLen2(SCR_PROGMEM_RD, scriptRunsToFillUpload);
				RunScriptUploadNoLen(SCR_PROGMEM_RD, scriptRunsToFillUpload);
				ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				//GetUpload();
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
				uploadIndex = 0;
				for (word = 0; word < wordsPerLoop; word++)
				{
					bite = 0;
					unsigned int memWord = (unsigned int)upload_buffer[uploadIndex + bite++];
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 8;
					}
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 16;
					}
					if (bite < bytesPerWord)
					{
						memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 24;
					}                  
					uploadIndex += bite;	
					DeviceBuffers->ProgramMemory[wordsRead++] = memWord;
					if (wordsRead == progMemP32)
					{
						j = runs;
						break; // for cases where ProgramMemSize%WordsPerLoop != 0
					}
				}
				timerPrint();
			}
		} while (wordsRead < progMemP32);
		timerStop();
	}
    // Read Boot Memory ========================================================================================
    wordsRead = 0;
	if (!progmem)
	{ // need to read last block to get UserIDs and Config
		scriptRunsToFillUpload = 1;
		wordsPerLoop = DevFile.PartsList[ActivePart].ProgMemRdWords;
		wordsRead = bootMemP32 - wordsPerLoop + 4;
	}
	else
	{
		timerStart((_TCHAR*)"Read Boot", bootMemP32 / wordsPerLoop);
	}
    do
    {
        // Download address.
        unsigned int address = (unsigned int)(wordsRead * bytesPerWord) + K_P32_BOOT_FLASH_START_ADDR;
        commOffSet = 0;
        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
        commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
        commandArrayp[commOffSet++] = (unsigned char)(scriptRunsToFillUpload * 4);
        for (i = 0; i < scriptRunsToFillUpload; i++)
        {
            commOffSet = addInstruction(commandArrayp, address, commOffSet);
        }
        writeUSB(commandArrayp, commOffSet);

        //RunScriptUploadNoLen2(SCR_PROGMEM_RD, scriptRunsToFillUpload);
		RunScriptUploadNoLen(SCR_PROGMEM_RD, scriptRunsToFillUpload);
        ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
        //GetUpload();
		UploadDataNoLen();
        ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
        uploadIndex = 0;
        for (word = 0; word < wordsPerLoop; word++)
        {
            bite = 0;
            unsigned int memWord = (unsigned int)upload_buffer[uploadIndex + bite++];
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 8;
            }
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 16;
            }
            if (bite < bytesPerWord)
            {
                memWord |= (unsigned int)upload_buffer[uploadIndex + bite++] << 24;
            }
            uploadIndex += bite;
            DeviceBuffers->ProgramMemory[progMemP32 + wordsRead++] = memWord;
            if (wordsRead == bootMemP32)
            {
                break; // for cases where ProgramMemSize%WordsPerLoop != 0
            }
        }
        timerPrint();
    } while (wordsRead < bootMemP32);

    // User ID Memory ========================================================================================
    // User IDs & Configs are in last block of boot mem
    DeviceBuffers->UserIDs[0] = (unsigned int)upload_buffer[uploadIndex];
    DeviceBuffers->UserIDs[1] = (unsigned int)upload_buffer[uploadIndex + 1];
    uploadIndex += bytesPerWord;

    // Config Memory ========================================================================================
    for (int cfg = 0; cfg < DevFile.PartsList[ActivePart].ConfigWords; cfg++)
    {
        DeviceBuffers->ConfigWords[cfg] = (unsigned int)upload_buffer[uploadIndex++];
        DeviceBuffers->ConfigWords[cfg] |= (unsigned int)(upload_buffer[uploadIndex++] << 8);
    }
             

    RunScript(SCR_PROG_EXIT, 1);

	timerStop();
    return true; // success
}        

bool CPICkitFunctions::PIC32BlankCheck(void)
{
    SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

    if (!PE_DownloadAndConnect())
    {
        return false;
    }

    int progMemP32 = (int)DevFile.PartsList[ActivePart].ProgramMem;
    int bootMemP32 = (int)DevFile.PartsList[ActivePart].BootFlash;
    progMemP32 -= bootMemP32; // boot flash at upper end of prog mem.
    int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;

    // Check Program Memory ====================================================================================    
    if (!PEBlankCheck(K_P32_PROGRAM_FLASH_START_ADDR, (unsigned int)(progMemP32*bytesPerWord)))
    {
        _tcsncpy_s(ReadError.memoryType, "Program", 7);
		RunScript(SCR_PROG_EXIT, 1);
		VddOff();
        return false;
    }

    // Check Boot Memory ====================================================================================

    if (!PEBlankCheck(K_P32_BOOT_FLASH_START_ADDR, (unsigned int)(bootMemP32*bytesPerWord)))
    {
        _tcsncpy_s(ReadError.memoryType, "Boot", 4);
		RunScript(SCR_PROG_EXIT, 1);
		VddOff();
        return false;
    }

    // Check Config Memory ====================================================================================
    if (!PEBlankCheck(K_P32_BOOT_FLASH_START_ADDR
        + (unsigned int)(bootMemP32 * bytesPerWord), (unsigned int)16))
    {
        _tcsncpy_s(ReadError.memoryType, "ID/Config", 9);
		RunScript(SCR_PROG_EXIT, 1);
		VddOff();
        return false;
    }

    RunScript(SCR_PROG_EXIT, 1);
	VddOff();
    
    return true;            
}
    
bool CPICkitFunctions::P32Write(bool progmem, bool uidmem, bool cfgmem)
{
	unsigned char commandArrayp[BUF_SIZE];
	int commOffSet;

    SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
    VddOn();

    if (!PE_DownloadAndConnect())
    {
        return false;
    }

	int progMemP32 = (int)DevFile.PartsList[ActivePart].ProgramMem;
	int bootMemP32 = (int)DevFile.PartsList[ActivePart].BootFlash;
	progMemP32 -= bootMemP32; // boot flash at upper end of prog mem.
    
    // Write Program Memory ====================================================================================
    
	// Write 512 bytes (128 words) per memory row - so need 2 downloads per row.
	int wordsPerLoop = 128;
    
	if (progmem)
	{
		// First, find end of used Program Memory
		int endOfBuffer = FindLastUsedInBuffer(DeviceBuffers->ProgramMemory,
										DevFile.Families[ActiveFamily].BlankValue, progMemP32 - 1);
		// align end on next loop boundary                 
		int writes = (endOfBuffer + 1) / wordsPerLoop;
		if (((endOfBuffer + 1) % wordsPerLoop) > 0)
		{
			writes++;
		}
		if (writes < 2)
			writes = 2; // 1024 bytes min

		timerStart((_TCHAR*)"Write Flash", endOfBuffer / wordsPerLoop);
	    
		// Send PROGRAM command header
		PEProgramHeader(K_P32_PROGRAM_FLASH_START_ADDR, (unsigned int)(writes * 512));
	    
		// First block of data
		int index = 0;
		PEProgramSendBlock(index, false); // no response
		writes--;
		timerPrint();
	    
		do
		{
			index += wordsPerLoop;
			PEProgramSendBlock(index, true); // response
			timerPrint();
		} while (--writes > 0);
	    
		// get last response
		commOffSet = 0;
		commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
		commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
		commandArrayp[commOffSet++] = 1;
		commandArrayp[commOffSet++] = SCMD_JT2_GET_PE_RESP;
		writeUSB(commandArrayp, commOffSet);

		timerStop();

		// Write Boot Memory ====================================================================================

		// Write 512 bytes (128 words) per memory row - so need 2 downloads per row.
		wordsPerLoop = 128;

		// First, find end of used Program Memory
		endOfBuffer = FindLastUsedInBuffer(DeviceBuffers->ProgramMemory,
										DevFile.Families[ActiveFamily].BlankValue, (int)DevFile.PartsList[ActivePart].ProgramMem - 1);
        if (endOfBuffer < progMemP32)
            endOfBuffer = 1;
        else
            endOfBuffer -= progMemP32;
		// align end on next loop boundary                 
		writes = (endOfBuffer + 1) / wordsPerLoop;
		if (((endOfBuffer + 1) % wordsPerLoop) > 0)
		{
			writes++;
		}
		if (writes < 2)
			writes = 2; // 1024 bytes min

		timerStart((_TCHAR*)"Write Boot", endOfBuffer / wordsPerLoop);

		// Send PROGRAM command header
		PEProgramHeader(K_P32_BOOT_FLASH_START_ADDR, (unsigned int)(writes * 512));

		// First block of data
		index = progMemP32;
		PEProgramSendBlock(index, false); // no response
		writes--;

		do
		{
			index += wordsPerLoop;
			PEProgramSendBlock(index, true); // response
			timerPrint();
		} while (--writes > 0);

		// get last response
		writeUSB(commandArrayp, commOffSet);
		timerStop();
	}
	// Write User ID Memory ===================================================================================
	if (uidmem)
	{
		 unsigned int cfgBuf[1];
		cfgBuf[0] = DeviceBuffers->UserIDs[0] & 0xFF;
		cfgBuf[0] |= (DeviceBuffers->UserIDs[1] & 0xFF) << 8;
		cfgBuf[0] |= 0xFFFF0000;

		unsigned int startAddress = K_P32_BOOT_FLASH_START_ADDR + (unsigned int)(bootMemP32 * 4);
		commOffSet = 0;
		commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
		commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
		commandArrayp[commOffSet++] = 18;
		commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
		commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x03;     // WORD_PROGRAM
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(startAddress & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(cfgBuf[0] & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_WAIT_PE_RESP;      
		writeUSB(commandArrayp, commOffSet);         
	}

    // Write Config Memory ====================================================================================
	if (cfgmem)
		{
		unsigned int cfgBuf[3];
		cfgBuf[0] = DeviceBuffers->ConfigWords[0] | (DeviceBuffers->ConfigWords[1] << 16);
		cfgBuf[0] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[0] & DevFile.PartsList[ActivePart].ConfigBlank[0])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[1] & DevFile.PartsList[ActivePart].ConfigBlank[1]) << 16);
		cfgBuf[1] = DeviceBuffers->ConfigWords[2] | (DeviceBuffers->ConfigWords[3] << 16);
		cfgBuf[1] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[2] & DevFile.PartsList[ActivePart].ConfigBlank[2])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[3] & DevFile.PartsList[ActivePart].ConfigBlank[3]) << 16);
		cfgBuf[2] = DeviceBuffers->ConfigWords[4] | (DeviceBuffers->ConfigWords[5] << 16);
		cfgBuf[2] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[4] & DevFile.PartsList[ActivePart].ConfigBlank[4])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[5] & DevFile.PartsList[ActivePart].ConfigBlank[5]) << 16);
	    
		unsigned int startAddress = K_P32_BOOT_FLASH_START_ADDR + (unsigned int)((bootMemP32 + 1) * 4);
		commOffSet = 0;
		commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
		commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
		commandArrayp[commOffSet++] = 36;
		commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
		commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x03;     // WORD_PROGRAM
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(startAddress & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(cfgBuf[0] & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[0] >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_WAIT_PE_RESP;
		startAddress += 4;
		commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
		commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x03;     // WORD_PROGRAM
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(startAddress & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(cfgBuf[1] & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[1] >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[1] >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[1] >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_WAIT_PE_RESP;  
		startAddress += 4;          
		writeUSB(commandArrayp, commOffSet);         

		commOffSet = 0;
		commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
		commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
		commandArrayp[commOffSet++] = 18;
		commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
		commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = 0x03;     // WORD_PROGRAM
		commandArrayp[commOffSet++] = 0x00;
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(startAddress & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((startAddress >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
		commandArrayp[commOffSet++] = (unsigned int)(cfgBuf[2] & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[2] >> 8) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[2] >> 16) & 0xFF);
		commandArrayp[commOffSet++] = (unsigned int)((cfgBuf[2] >> 24) & 0xFF);
		commandArrayp[commOffSet++] = SCMD_JT2_WAIT_PE_RESP;       
		writeUSB(commandArrayp, commOffSet);
	}

    
    return true;
}

void CPICkitFunctions::PEProgramHeader(unsigned int startAddress, unsigned int lengthBytes)
{
    unsigned char commandArrayp[BUF_SIZE];
    int commOffSet = 0;
    commandArrayp[commOffSet++] = FWCMD_CLR_UPLOAD_BUFFER;
    commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
    commandArrayp[commOffSet++] = 17;
    commandArrayp[commOffSet++] = SCMD_JT2_SENDCMD;
    commandArrayp[commOffSet++] = 0x0E;                 // ETAP_FASTDATA
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = 0x02;     // PROGRAM
    commandArrayp[commOffSet++] = 0x00;
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(startAddress & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((startAddress >> 24) & 0xFF);
    commandArrayp[commOffSet++] = SCMD_JT2_XFRFASTDAT_LIT;
    commandArrayp[commOffSet++] = (unsigned char)(lengthBytes & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 8) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 16) & 0xFF);
    commandArrayp[commOffSet++] = (unsigned char)((lengthBytes >> 24) & 0xFF);
    writeUSB(commandArrayp, commOffSet);
}

void CPICkitFunctions::PEProgramSendBlock(int index, bool peResp)
{ // Assumes DL buffer is 256!
    unsigned char downloadBuffer[256];
    unsigned int memWord = 0;
    int dnldIndex = 0;
    int memMax = (int)DevFile.PartsList[ActivePart].ProgramMem;
    int i;

    // first half
    for (i = 0; i < 64; i++)
    {
        if (index < memMax)
            memWord = DeviceBuffers->ProgramMemory[index++];
        else
            memWord = 0xFFFFFFFF;
        downloadBuffer[dnldIndex++] = (unsigned char)(memWord & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 8) & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 16) & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 24) & 0xFF);
    }
    // Download first half of block
    int dataIndex = DataClrAndDownload(downloadBuffer, 256, 0);
    while ((dnldIndex - dataIndex) > 62) // DataDownload send 62 bytes per call
    {
        dataIndex = DataDownload(downloadBuffer, 256, dataIndex);
    }
    // send rest of data with script cmd
    int length = dnldIndex - dataIndex;
    unsigned char commandArray[BUF_SIZE];
    int commOffset = 0;
    commandArray[commOffset++] = FWCMD_DOWNLOAD_DATA;
    commandArray[commOffset++] = (unsigned char)(length & 0xFF);
    for (i = 0; i < length; i++)
    {
        commandArray[commOffset++] = downloadBuffer[dataIndex + i];
    }
    commandArray[commOffset++] = FWCMD_RUN_SCRIPT;
    commandArray[commOffset++] = SCR_PROGMEM_WR_PREP; // should not be remapped
    commandArray[commOffset++] = 1; // once
    writeUSB(commandArray, commOffset);
    
    // 2nd half
    dnldIndex = 0;
    for (i = 0; i < 64; i++)
    {
        if (index < memMax)
            memWord = DeviceBuffers->ProgramMemory[index++];
        else
            memWord = 0xFFFFFFFF;
        downloadBuffer[dnldIndex++] = (unsigned char)(memWord & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 8) & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 16) & 0xFF);
        downloadBuffer[dnldIndex++] = (unsigned char)((memWord >> 24) & 0xFF);
    }
    // Download 2nd half of block
    dataIndex = DataClrAndDownload(downloadBuffer, 256, 0);
    while ((dnldIndex - dataIndex) > 62) // DataDownload send 62 bytes per call
    {
        dataIndex = DataDownload(downloadBuffer, 256, dataIndex);
    }
    // send rest of data with script cmd
    length = dnldIndex - dataIndex;
    commOffset = 0;
    commandArray[commOffset++] = FWCMD_DOWNLOAD_DATA;
    commandArray[commOffset++] = (unsigned char)(length & 0xFF);
    for (i = 0; i < length; i++)
    {
        commandArray[commOffset++] = downloadBuffer[dataIndex + i];
    }
    commandArray[commOffset++] = FWCMD_RUN_SCRIPT;
    if (peResp)
        commandArray[commOffset++] = SCR_PROGMEM_WR; // should not be remapped
    else
        commandArray[commOffset++] = SCR_PROGMEM_WR_PREP; // should not be remapped
    commandArray[commOffset++] = 1; // once
    writeUSB(commandArray, commOffset);        
}

bool CPICkitFunctions::P32Verify(bool writeVerify, bool progmem, bool uidmem, bool cfgmem)
{
	int bufferCRC;
	int deviceCRC;

    if (!writeVerify)
    { // not necessary on post-program verify
        SetMCLR(true);     // assert /MCLR to prevent code execution before programming mode entered.
        VddOn();

        if (!PE_DownloadAndConnect())
        {
            return false;
        }
    }

    int progMemP32 = (int)DevFile.PartsList[ActivePart].ProgramMem;
    int bootMemP32 = (int)DevFile.PartsList[ActivePart].BootFlash;
    progMemP32 -= bootMemP32; // boot flash at upper end of prog mem.
    int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;

	if (progmem)
	{
		// Verify Program Memory ====================================================================================
	    
		bufferCRC = p32CRC_buf(DeviceBuffers->ProgramMemory, 0, (unsigned int)progMemP32);
	    
		deviceCRC = PEGetCRC(K_P32_PROGRAM_FLASH_START_ADDR, (unsigned int)(progMemP32 * bytesPerWord));
	    
		if (bufferCRC != deviceCRC)
		{
			_tcsncpy_s(ReadError.memoryType, "Program", 7);
			RunScript(SCR_PROG_EXIT, 1);
			VddOff();
			return false;
		}

		// Verify Boot Memory ====================================================================================
		bufferCRC = p32CRC_buf(DeviceBuffers->ProgramMemory, (unsigned int)progMemP32, (unsigned int)bootMemP32);

		deviceCRC = PEGetCRC(K_P32_BOOT_FLASH_START_ADDR, (unsigned int)(bootMemP32 * bytesPerWord));

		if (bufferCRC != deviceCRC)
		{
			_tcsncpy_s(ReadError.memoryType, "Boot", 4);
			RunScript(SCR_PROG_EXIT, 1);
			VddOff();
			return false;

		}            
	}

    // Verify User ID Memory ====================================================================================  
    if (uidmem)
	{
		unsigned int cfgBuf[1];
		cfgBuf[0] = DeviceBuffers->UserIDs[0] & 0xFF;
		cfgBuf[0] |= (DeviceBuffers->UserIDs[1] & 0xFF) << 8;
		cfgBuf[0] |= 0xFFFF0000;

		bufferCRC = p32CRC_buf(cfgBuf, (unsigned int)0, (unsigned int)1);

		deviceCRC = PEGetCRC(K_P32_BOOT_FLASH_START_ADDR + (unsigned int)(bootMemP32 * bytesPerWord), (unsigned int)(1 * bytesPerWord));

		if (bufferCRC != deviceCRC)
		{
			_tcsncpy_s(ReadError.memoryType, "User ID", 7);
			RunScript(SCR_PROG_EXIT, 1);
			VddOff();
			return false;
		}             
	}

    // Verify Config Memory ====================================================================================  
    if (cfgmem)
	{
		unsigned int cfgBuf[3];
		cfgBuf[0] = DeviceBuffers->ConfigWords[0] | (DeviceBuffers->ConfigWords[1] << 16);
		cfgBuf[0] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[0] & DevFile.PartsList[ActivePart].ConfigBlank[0])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[1] & DevFile.PartsList[ActivePart].ConfigBlank[1]) << 16);
		cfgBuf[1] = DeviceBuffers->ConfigWords[2] | (DeviceBuffers->ConfigWords[3] << 16);
		cfgBuf[1] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[2] & DevFile.PartsList[ActivePart].ConfigBlank[2])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[3] & DevFile.PartsList[ActivePart].ConfigBlank[3]) << 16);
		cfgBuf[2] = DeviceBuffers->ConfigWords[4] | (DeviceBuffers->ConfigWords[5] << 16);
		cfgBuf[2] |= (~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[4] & DevFile.PartsList[ActivePart].ConfigBlank[4])
					| ((~(unsigned int)DevFile.PartsList[ActivePart].ConfigMasks[5] & DevFile.PartsList[ActivePart].ConfigBlank[5]) << 16);


		bufferCRC = p32CRC_buf(cfgBuf, (unsigned int)0, (unsigned int)3);

		deviceCRC = PEGetCRC(K_P32_BOOT_FLASH_START_ADDR + (unsigned int)((bootMemP32 + 1) * bytesPerWord), (unsigned int)(3 * bytesPerWord));

		if (bufferCRC != deviceCRC)
		{
			_tcsncpy_s(ReadError.memoryType, "Configuration", 13);
			RunScript(SCR_PROG_EXIT, 1);
			VddOff();
			return false;
		}             
	}

    RunScript(SCR_PROG_EXIT, 1);
	VddOff();
    return true;
}

int CPICkitFunctions::p32CRC_buf(unsigned int* buffer, unsigned int startIdx, unsigned int len)
{
    unsigned int CRC_POLY = 0x11021;
    unsigned int CRC_SEED = 0xFFFF; //0x84CF;
    
    unsigned int A, B1, B2;
    unsigned int CurByte;
    unsigned int CurCRC = CRC_SEED;
    unsigned int CurCRCHighBit;
    unsigned int CurWord;

    unsigned int bytesPerWord = (unsigned int)DevFile.Families[ActiveFamily].BytesPerLocation;

    // Loop through entire address range
    for (A = startIdx; A < (startIdx + len); A++)
    {
        CurWord = buffer[A];

        // Process each byte in this word
        for (B1 = 0; B1 < bytesPerWord; B1++)
        {
            CurByte = (CurWord & 0xFF) << 8;
            CurWord >>= 8;

            // Process each bit in this byte
            for (B2 = 0; B2 < 8; B2++)
            {
                CurCRCHighBit = (CurCRC ^ CurByte) & 0x8000;
                CurCRC <<= 1;
                //CurCRC |= (CurByte >> 7) & 0x1;
                CurByte <<= 1;
                if (CurCRCHighBit > 0)
                    CurCRC ^= CRC_POLY;
            }
        }
    }            

   return (int)(CurCRC & 0xFFFF);
    
}

/*********************************************************************************************
 *********************************************************************************************
 *****                          PIC24H/dsPIC PE Functions                                *****
 *********************************************************************************************
 ********************************************************************************************/
void CPICkitFunctions::DisablePE33(void)
{
	usePE33 = false;
}

bool CPICkitFunctions::DownloadPE33(void)
        {
			CdsP33_PE P33PE;
        
            RunScript(SCR_PROG_ENTRY, 1);
            // Erase Executive Memory
            ExecuteScript(DevFile.PartsList[ActivePart].DebugWriteVectorScript);
            
            // Set address
            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x800000); // start of exec memory
                RunScript(SCR_PROGMEM_WR_PREP, 1);
            }
            
            int instruction = 0;
            unsigned int workaround = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            
            // Program the exec in 16 rows
			int row, section, word;
            for (row = 0; row < 16; row++)
            {
                // Download a 64-instruction row 
                for (section = 0; section < 4; section++)
                {
                    commOffSet = 0;
                    if (section == 0)
                    {
                        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
                    }
                    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
                    commandArrayp[commOffSet++] = 48; // 16 instructions.
                    for (word = 0; word < 16; word++)
                    {
                        commandArrayp[commOffSet++] = (unsigned char)(P33PE.dsPIC33_PE_Code[instruction] & 0xFF);
                        commandArrayp[commOffSet++] = (unsigned char)((P33PE.dsPIC33_PE_Code[instruction] >> 8) & 0xFF);
                        commandArrayp[commOffSet++] = (unsigned char)((P33PE.dsPIC33_PE_Code[instruction] >> 16) & 0xFF); 
                        instruction++;
                    }
                    workaround = P33PE.dsPIC33_PE_Code[instruction - 4];
                    writeUSB(commandArrayp, commOffSet);
                }
                // Program the row
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0; // fill in later
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFWORD_W;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFBYTE_W;
                commandArrayp[commOffSet++] = 1;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFWORD_W;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFBYTE_W;
                commandArrayp[commOffSet++] = 2;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTL W0, [W7]
                commandArrayp[commOffSet++] = 0x80;
                commandArrayp[commOffSet++] = 0x0B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTH W1, [W7++]
                commandArrayp[commOffSet++] = 0x81;
                commandArrayp[commOffSet++] = 0x9B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTH W2, [W7]
                commandArrayp[commOffSet++] = 0x82;
                commandArrayp[commOffSet++] = 0x8B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTL W3, [W7++]
                commandArrayp[commOffSet++] = 0x83;
                commandArrayp[commOffSet++] = 0x1B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 32;
                commandArrayp[commOffSet++] = 31;
                // Workaround for Device ID errata
                commandArrayp[commOffSet++] = SCMD_COREINST24; // MOV.W #8, W8
                commandArrayp[commOffSet++] = 0x88;
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // SUBR.W W8, W7, W8
                commandArrayp[commOffSet++] = 0x07;
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = 0x14;
                commandArrayp[1] = (unsigned char)(commOffSet-2);  // script length
                writeUSB(commandArrayp, commOffSet);
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0; // fill in later
                commandArrayp[commOffSet++] = SCMD_COREINST24; // MOV.W #(workaround & 0xFFFF), W0
                commandArrayp[commOffSet++] = (unsigned char)(workaround << 4);
                commandArrayp[commOffSet++] = (unsigned char)(workaround >> 4);
                commandArrayp[commOffSet++] = (unsigned char)(0x20 | (0x0F & (workaround >> 12)));
                commandArrayp[commOffSet++] = SCMD_COREINST24; // MOV.W #(workaround >> 16), W1
                workaround >>= 16;
                commandArrayp[commOffSet++] = (unsigned char)(0x01 | (workaround << 4));
                commandArrayp[commOffSet++] = (unsigned char)(workaround >> 4);
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTL.W W0, [W8]
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = 0x0C;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTH.B W1, [W8]
                commandArrayp[commOffSet++] = 0x01;
                commandArrayp[commOffSet++] = 0x8C;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                // END Device ID errata workaround
                commandArrayp[commOffSet++] = SCMD_COREINST24; // BSET.B 0x0761, #7
                commandArrayp[commOffSet++] = 0x61;
                commandArrayp[commOffSet++] = 0xE7;
                commandArrayp[commOffSet++] = 0xA8;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // GOTO 0x200
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = 0x02;
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 1;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 72;
                commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
                writeUSB(commandArrayp, commOffSet);
            }
            
            // VERIFY PE
            // Set address
            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x800000); // start of exec memory
                RunScript(SCR_PROGMEM_ADDRSET, 1);
            }
            
            // verify the exec in 32 sections
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            instruction = 0;
			int uploadIndex;
			unsigned int memWord;
            for (section = 0; section < 32; section++)
            {
                RunScriptUploadNoLen(SCR_PROGMEM_RD, 1);
                ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
                UploadDataNoLen();
                ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
                for (word = 0; word < 32; word++)
                {
                    memWord = (unsigned int)upload_buffer[uploadIndex++];
                    memWord |= (unsigned int)upload_buffer[uploadIndex++] << 8;
                    memWord |= (unsigned int)upload_buffer[uploadIndex++] << 16;
                    if (memWord != P33PE.dsPIC33_PE_Code[instruction++])
                    {
                        RunScript(SCR_PROG_EXIT, 1);
                        return false;
                    }
                }
            }
            
            RunScript(SCR_PROG_EXIT, 1);
            
            return true;
        }
        
bool CPICkitFunctions::PE33_Connect(void)
        {
            RunScript(SCR_PROG_ENTRY, 1);

            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x8007C0); // last 32 words of exec memory
                RunScript(SCR_PROGMEM_ADDRSET, 1);
            }
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            RunScriptUploadNoLen(SCR_PROGMEM_RD, 1);
            ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
            UploadDataNoLen();
            ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
            // check ID word
            int memValue = (int)upload_buffer[72]; // addresss 0x8007F0
            memValue |= (int)(upload_buffer[73] << 8);
            if (memValue != K_DSPIC33_PE_ID)
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            // check PE version
            memValue = (int)upload_buffer[75]; // addresss 0x8007F2
            memValue |= (int)(upload_buffer[76] << 8);
			RunScript(SCR_PROG_EXIT, 1);
            if (memValue != K_DSPIC33_PE_VERSION)
            {
				RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            
			RunScript(SCR_PROG_EXIT, 1);

            // It looks like there is a PE there.  Try talking to the PE directly:
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            // entering programming mode with PE (4D434850)
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 0; // fill in later
            commandArrayp[commOffSet++] = SCMD_VPP_OFF;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_ON;
            commandArrayp[commOffSet++] = SCMD_VPP_PWM_ON;
            commandArrayp[commOffSet++] = SCMD_BUSY_LED_ON;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_DELAY_LONG;
            commandArrayp[commOffSet++] = 20;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_OFF;
            commandArrayp[commOffSet++] = SCMD_VPP_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 23;
            commandArrayp[commOffSet++] = SCMD_VPP_OFF;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 47;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xB2;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xC2;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x12;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x0A;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_OFF;
            commandArrayp[commOffSet++] = SCMD_VPP_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_LONG;
            commandArrayp[commOffSet++] = 6;    //32ms
            commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
            writeUSB(commandArrayp, commOffSet);
            // Try sanity Check
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 12;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // sanity check = 0x00 01
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x80;                     // PE talks MSB first, script routines are LSB first.
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 5;                        //100+ us
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
            if (!readUSB())
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x10 00 00 02
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if ((Usb_read_array[1] != 0x08) || (Usb_read_array[2] != 0x00)
                || (Usb_read_array[3] != 0x00) || (Usb_read_array[4] != 0x40))
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            
            // Passed sanity check; verify version.
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 14;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00; // PGD is output
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // QVER = 0xB0 01
            commandArrayp[commOffSet++] = 0x0D;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x80;                     // PE talks MSB first, script routines are LSB first.
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 5;                        //100+ us
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
            if (!readUSB())
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x1B <Ver> 00 02
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if ((Usb_read_array[1] != 0xD8) || (BitReverseTable[Usb_read_array[2]] != (unsigned char)K_DSPIC33_PE_VERSION)
                || (Usb_read_array[3] != 0x00) || (Usb_read_array[4] != 0x40))
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            
            // Do not exit programming mode if we successfully find a PE
            return true;
        }

bool CPICkitFunctions::PE33_DownloadAndConnect(void)
        {
            // VDD must already be on!
            // reduce PE comm speed to 500kHz max
            //ICSPSpeedRestore = LastICSPSpeed;
            if (LastICSPSpeed < 2)
                SetProgrammingSpeed(2);
            
            // See if the PE already exists
            if (!PE33_Connect())
            { // it doesn't, download it    
                printf("Downloading Programming Executive...\n");
				fflush(stdout);
                if (!DownloadPE33())
                { // download failed
                    printf("...FAILED\n");
					fflush(stdout);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if (!PE33_Connect())
                { // try connecting
                    printf("...FAILED\n");
					fflush(stdout);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
				printf("...done\n");
				fflush(stdout);
            }
			else
			{
				if (!printedUsingPE)
				{ // only print first time
					printedUsingPE = true;
					printf("Using PE\n");
					fflush(stdout);
				}
			}
            return true;
        }

bool CPICkitFunctions::PE33DoBlankCheck(unsigned int lengthWords)
        {
            // Use QBLANK (0xA)
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            
            lengthWords++; // command arg is length + 1

            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 0;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00; // PGD is output
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0xA0 03
            commandArrayp[commOffSet++] = 0x05;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xC0;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // lengthWords
            commandArrayp[commOffSet++] = BitReverseTable[((lengthWords >> 16) & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // lengthWords
            commandArrayp[commOffSet++] = BitReverseTable[((lengthWords >> 8) & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = BitReverseTable[(lengthWords & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
            writeUSB(commandArrayp, commOffSet);

            // wait 2 seconds for the results.
            Sleep(2000);
                
            // get results
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 4;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
            if (!readUSB())
            {
                return false;
            }
            if ((Usb_read_array[0] != 4)
              || (Usb_read_array[1] != BitReverseTable[0x1A]) || (Usb_read_array[2] != BitReverseTable[0xF0])
              || (Usb_read_array[3] != 0x00) || (Usb_read_array[4] != BitReverseTable[0x02]))
                return false; // device not blank or error

            return true;
        }

bool CPICkitFunctions::PE33BlankCheck(void)
        {
            if (!PE33_DownloadAndConnect())
            {
                return false;
            }

            // Check Program Memory ====================================================================================
            if (!PE33DoBlankCheck((unsigned int)DevFile.PartsList[ActivePart].ProgramMem))
            {
                RunScript(SCR_PROG_EXIT, 1);
                SetProgrammingSpeed(LastICSPSpeed);
                return false;
            }

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::PE33Read(void)
        {
            if (!PE33_DownloadAndConnect())
            {
                return false;
            }

            // Read Program Memory ====================================================================================
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            //int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
            int wordsPerLoop = 32;
            int wordsRead = 0;
            int uploadIndex = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
			timerStart((_TCHAR*)"Read Flash", (int)DevFile.PartsList[ActivePart].ProgramMem / wordsPerLoop);

            do
            {
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0x20 04
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // Length N
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[wordsPerLoop];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 5;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;         // Read & toss 2 response words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read 32 3-byte words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 31;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA_NOLEN;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);  // script length

                writeUSB(commandArrayp, commOffSet);

                GetUpload();
                ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
				int word;
				unsigned int memWord1, memWord2;
                for (word = 0; word < wordsPerLoop; word+= 2)
                {
                    // two word2 of packed instructions
                    memWord1 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    memWord2 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    DeviceBuffers->ProgramMemory[wordsRead++] = memWord1;
                    DeviceBuffers->ProgramMemory[wordsRead++] = memWord2;
                    if (wordsRead >= (int)DevFile.PartsList[ActivePart].ProgramMem)
                    {
                        break; // for cases where ProgramMemSize%WordsPerLoop != 0
                    }
                }
                timerPrint();
            } while (wordsRead < (int)DevFile.PartsList[ActivePart].ProgramMem);

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::PE33Write(int endOfBuffer)
        {
            if (!PE33_DownloadAndConnect())
            {
                return false;
            }
        
            unsigned char downloadBuffer[DOWNLOAD_BUFFER_SIZE];
            int wordsPerLoop = 64;
            int wordsWritten = 0;

			timerStart((_TCHAR*)"Write Flash", endOfBuffer / wordsPerLoop);

            do
            {
                int downloadIndex = 0;
				int word;
				unsigned int memWord;
                for (word = 0; word < wordsPerLoop; word+=2)
                {
                    // Put in packed format for PE  
                    memWord = DeviceBuffers->ProgramMemory[wordsWritten++];
                    downloadBuffer[downloadIndex + 1] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte) (memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 3] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);

                    memWord = DeviceBuffers->ProgramMemory[wordsWritten++];
                    downloadBuffer[downloadIndex + 5] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte) (memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 4] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 2] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    
                    downloadIndex += 6;
                            
                }
                // download data
                int dataIndex = DataClrAndDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, 0);
                while (dataIndex < downloadIndex)
                {
                    dataIndex = DataDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, dataIndex);
                }

                int commOffSet = 0;
                unsigned char commandArrayp[BUF_SIZE];
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0; // fill in later
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // PROGP = 0x50 63
                commandArrayp[commOffSet++] = 0x0A;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0xC6;                      // PE talks MSB first, script routines are LSB first.
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;  // write 64 3-byte words
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 63;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 118;                        //2.5ms
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read response
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);
                writeUSB(commandArrayp, commOffSet);
                if (!readUSB())
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x15 00 00 02
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if ((BitReverseTable[Usb_read_array[1]] != 0x15) || (Usb_read_array[2] != 0x00)
                    || (Usb_read_array[3] != 0x00) || (BitReverseTable[Usb_read_array[4]] != 0x02))
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }

                timerPrint();
            } while (wordsWritten < endOfBuffer);

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }


bool CPICkitFunctions::PE33VerifyCRC(void)
        {
            if (!PE33_DownloadAndConnect())
            {
                return false;
            }
			timerStart((_TCHAR*)"Verify Flash", 1);
        
            // Use CRCP (0xC)
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];

            unsigned short bufferCRC = CalcCRCProgMem();
            unsigned int lengthWords = DevFile.PartsList[ActivePart].ProgramMem;

            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 0;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00; // PGD is output
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0xC0 05
            commandArrayp[commOffSet++] = 0x03;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xA0;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // word 2
            commandArrayp[commOffSet++] = 0x80;                      // Program Memory
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // start address MSB
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // word 3
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // word 4
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // lengthWords MSB
            commandArrayp[commOffSet++] = BitReverseTable[((lengthWords >> 16) & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // word5 lengthWords LSW
            commandArrayp[commOffSet++] = BitReverseTable[((lengthWords >> 8) & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = BitReverseTable[(lengthWords & 0xFF)];
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
            writeUSB(commandArrayp, commOffSet);
            // wait (memsize) * (1.5 seconds / 128k) for the results.
            float sleepTime = (float)DevFile.PartsList[ActivePart].ProgramMem * ((float)1500 / (float)44032);
            Sleep((int)sleepTime);

            // get results
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 6;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
			timerPrint();
			timerStop();
            if (!readUSB())
            {
				RunScript(SCR_PROG_EXIT, 1);
				SetProgrammingSpeed(LastICSPSpeed);
                return false;
            }
            if (Usb_read_array[0] != 6)
            {
				RunScript(SCR_PROG_EXIT, 1);
				SetProgrammingSpeed(LastICSPSpeed);
                return false; // error
            }

			RunScript(SCR_PROG_EXIT, 1);
			SetProgrammingSpeed(LastICSPSpeed);

            unsigned short deviceCRC = (unsigned short)BitReverseTable[Usb_read_array[6]];
            deviceCRC += (unsigned short)(BitReverseTable[Usb_read_array[5]] << 8);

            if (deviceCRC == bufferCRC)
                return true;
            
            return false;
        }

unsigned short CPICkitFunctions::CalcCRCProgMem(void)
        {
            unsigned int CRC_Value = 0xFFFF; // seed
            int word;
			unsigned int memWord;
            for (word = 0; word < (int)DevFile.PartsList[ActivePart].ProgramMem; word+=2)
            {
                memWord = DeviceBuffers->ProgramMemory[word];
                CRC33_Calculate((unsigned char)(memWord & 0xFF), &CRC_Value);
                CRC33_Calculate((unsigned char)((memWord >> 8) & 0xFF), &CRC_Value);
                CRC33_Calculate((unsigned char)((memWord >> 16) & 0xFF), &CRC_Value);
                memWord = DeviceBuffers->ProgramMemory[word+1];
                CRC33_Calculate((unsigned char)((memWord >> 16) & 0xFF), &CRC_Value);
                CRC33_Calculate((unsigned char)(memWord & 0xFF), &CRC_Value);
                CRC33_Calculate((unsigned char)((memWord >> 8) & 0xFF), &CRC_Value);
            }
            
            return (unsigned short)(CRC_Value & 0xFFFF);
        }

void CPICkitFunctions::CRC33_Calculate(unsigned char ByteValue , unsigned int* CRC_Value)
        {
			CdsP33_PE P33PE;
            unsigned char  value;

            value = (unsigned char)((*CRC_Value >> (8)) ^ ByteValue);
            *CRC_Value = P33PE.CRC_LUT_Array[value] ^ (*CRC_Value << 8);
        }

bool CPICkitFunctions::useProgExec33(void)
        {
            if (FamilyIsdsPIC33F() || FamilyIsPIC24H())
            {
                if (DevFile.PartsList[ActivePart].ProgramMem >= 4096)
                // don't use PE on the smallest parts.
                    return usePE33;
            }
            return false;
        }

/*********************************************************************************************
 *********************************************************************************************
 *****                               PIC24F PE Functions                                 *****
 *********************************************************************************************
 ********************************************************************************************/

bool CPICkitFunctions::DownloadPE24F(void)
        {
			CP24F_PE P24FPE;
        
            RunScript(SCR_PROG_ENTRY, 1);
            // Erase Executive Memory
            ExecuteScript(DevFile.PartsList[ActivePart].DebugWriteVectorScript);
            
            // Set address
            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x800000); // start of exec memory
                RunScript(SCR_PROGMEM_WR_PREP, 1);
            }
            
            int instruction = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            
            // Program the exec in 8 rows
			int row, section, word;
            for (row = 0; row < 8; row++)
            {
                // Download a 64-instruction row 
                for (section = 0; section < 4; section++)
                {
                    commOffSet = 0;
                    if (section == 0)
                    {
                        commandArrayp[commOffSet++] = FWCMD_CLR_DOWNLOAD_BUFFER;
                    }
                    commandArrayp[commOffSet++] = FWCMD_DOWNLOAD_DATA;
                    commandArrayp[commOffSet++] = 48; // 16 instructions.
                    for (word = 0; word < 16; word++)
                    {
                        commandArrayp[commOffSet++] = (unsigned char)(P24FPE.PIC24F_PE_Code[instruction] & 0xFF);
                        commandArrayp[commOffSet++] = (unsigned char)((P24FPE.PIC24F_PE_Code[instruction] >> 8) & 0xFF);
                        commandArrayp[commOffSet++] = (unsigned char)((P24FPE.PIC24F_PE_Code[instruction] >> 16) & 0xFF); 
                        instruction++;
                    }
                    writeUSB(commandArrayp, commOffSet);
                }
                // Program the row
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0; // fill in later
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFWORD_W;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFBYTE_W;
                commandArrayp[commOffSet++] = 1;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFWORD_W;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = SCMD_WRITE_BUFBYTE_W;
                commandArrayp[commOffSet++] = 2;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTL W0, [W7]
                commandArrayp[commOffSet++] = 0x80;
                commandArrayp[commOffSet++] = 0x0B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTH W1, [W7++]
                commandArrayp[commOffSet++] = 0x81;
                commandArrayp[commOffSet++] = 0x9B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTH W2, [W7]
                commandArrayp[commOffSet++] = 0x82;
                commandArrayp[commOffSet++] = 0x8B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // TBLWTL W3, [W7++]
                commandArrayp[commOffSet++] = 0x83;
                commandArrayp[commOffSet++] = 0x1B;
                commandArrayp[commOffSet++] = 0xBB;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 32;
                commandArrayp[commOffSet++] = 31;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // BSET.B 0x0761, #7
                commandArrayp[commOffSet++] = 0x61;
                commandArrayp[commOffSet++] = 0xE7;
                commandArrayp[commOffSet++] = 0xA8;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_COREINST24; // GOTO 0x200
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = 0x02;
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_NOP24;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 1;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 72;
                commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
                writeUSB(commandArrayp, commOffSet);
            }
            
            // VERIFY PE
            // Set address
            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x800000); // start of exec memory
                RunScript(SCR_PROGMEM_ADDRSET, 1);
            }
            
            // verify the exec in 16 sections
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            instruction = 0;
			int uploadIndex;
			unsigned int memWord;
            for (section = 0; section < 16; section++)
            {
                RunScriptUploadNoLen(SCR_PROGMEM_RD, 1);
                ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
                UploadDataNoLen();
                ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
                for (word = 0; word < 32; word++)
                {
                    memWord = (unsigned int)upload_buffer[uploadIndex++];
                    memWord |= (unsigned int)upload_buffer[uploadIndex++] << 8;
                    memWord |= (unsigned int)upload_buffer[uploadIndex++] << 16;
                    if (memWord != P24FPE.PIC24F_PE_Code[instruction++])
                    {
                        RunScript(SCR_PROG_EXIT, 1);
                        return false;
                    }
                }
            }
            
            RunScript(SCR_PROG_EXIT, 1);
            
            return true;
        }

bool CPICkitFunctions::PE24F_Connect(void)
        {
            RunScript(SCR_PROG_ENTRY, 1);

            if (DevFile.PartsList[ActivePart].ProgMemWrPrepScript != 0)
            { // if prog mem address set script exists for this part
                DownloadAddress3(0x8003C0); // last 32 words of exec memory
                RunScript(SCR_PROGMEM_ADDRSET, 1);
            }
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            RunScriptUploadNoLen(SCR_PROGMEM_RD, 1);
            ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
            UploadDataNoLen();
            ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
            // check ID word
            int memValue = (int)upload_buffer[72]; // addresss 0x8003F0
            memValue |= (int)(upload_buffer[73] << 8);
            if (memValue != K_PIC24F_PE_ID)
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            // check PE version
            memValue = (int)upload_buffer[75]; // addresss 0x8003F2
            memValue |= (int)(upload_buffer[76] << 8);
			RunScript(SCR_PROG_EXIT, 1);
            if (memValue != K_PIC24F_PE_VERSION)
            {
				RunScript(SCR_PROG_EXIT, 1);
                return false;
            }

			RunScript(SCR_PROG_EXIT, 1);
            
            // It looks like there is a PE there.  Try talking to the PE directly:
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            // entering programming mode with PE (4D434850)
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 0; // fill in later
            commandArrayp[commOffSet++] = SCMD_VPP_OFF;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_ON;
            commandArrayp[commOffSet++] = SCMD_VPP_PWM_ON;
            commandArrayp[commOffSet++] = SCMD_BUSY_LED_ON;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_DELAY_LONG;
            commandArrayp[commOffSet++] = 20;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_OFF;
            commandArrayp[commOffSet++] = SCMD_VPP_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 23;
            commandArrayp[commOffSet++] = SCMD_VPP_OFF;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 47;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xB2;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0xC2;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x12;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x0A;
            commandArrayp[commOffSet++] = SCMD_MCLR_GND_OFF;
            commandArrayp[commOffSet++] = SCMD_VPP_ON;
            commandArrayp[commOffSet++] = SCMD_DELAY_LONG;
            commandArrayp[commOffSet++] = 6;    //32ms
            commandArrayp[1] = (unsigned char)(commOffSet - 2);  // script length
            writeUSB(commandArrayp, commOffSet);
            // Try sanity Check
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 12;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // sanity check = 0x00 01
            commandArrayp[commOffSet++] = 0x00;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x80;                     // PE talks MSB first, script routines are LSB first.
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 5;                        //100+ us
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
            if (!readUSB())
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x10 00 00 02
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if ((Usb_read_array[1] != 0x08) || (Usb_read_array[2] != 0x00)
                || (Usb_read_array[3] != 0x00) || (Usb_read_array[4] != 0x40))
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            
            // Passed sanity check; verify version.
            commOffSet = 0;
            commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
            commandArrayp[commOffSet++] = 14;
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x00; // PGD is output
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // QVER = 0xB0 01
            commandArrayp[commOffSet++] = 0x0D;
            commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
            commandArrayp[commOffSet++] = 0x80;                     // PE talks MSB first, script routines are LSB first.
            commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
            commandArrayp[commOffSet++] = 0x02; // PGD is input
            commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
            commandArrayp[commOffSet++] = 5;                        //100+ us
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
            commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
            writeUSB(commandArrayp, commOffSet);
            if (!readUSB())
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x1B <Ver> 00 02
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            if ((Usb_read_array[1] != 0xD8) || (BitReverseTable[Usb_read_array[2]] != (unsigned char)K_PIC24F_PE_VERSION)
                || (Usb_read_array[3] != 0x00) || (Usb_read_array[4] != 0x40))
            {
                RunScript(SCR_PROG_EXIT, 1);
                return false;
            }
            
            // Do not exit programming mode if we successfully find a PE
            return true;
        }

bool CPICkitFunctions::PE24F_DownloadAndConnect(void)
        {
            // VDD must already be on!
            // reduce PE comm speed to 500kHz max
            //ICSPSpeedRestore = LastICSPSpeed;
            if (LastICSPSpeed < 2)
                SetProgrammingSpeed(2);
            
            // See if the PE already exists
            if (!PE24F_Connect())
            { // it doesn't, download it    
                printf("Downloading Programming Executive...\n");
				fflush(stdout);
                if (!DownloadPE24F())
                { // download failed
                    printf("...FAILED\n");
					fflush(stdout);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if (!PE24F_Connect())
                { // try connecting
                    printf("...FAILED\n");
					fflush(stdout);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
				printf("...done\n");
				fflush(stdout);
            }
			else
			{
				if (!printedUsingPE)
				{ // only print first time
					printedUsingPE = true;
					printf("Using PE\n");
					fflush(stdout);
				}
			}
            return true;
        }

bool CPICkitFunctions::PE24FBlankCheck(void)
        {
            if (!PE24F_DownloadAndConnect())
            {
                return false;
            }

            // to deal with configs in program mem
            int configsStart = (int)DevFile.PartsList[ActivePart].ProgramMem - DevFile.PartsList[ActivePart].ConfigWords;


            // Check Program Memory ====================================================================================
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            //int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
            int wordsPerLoop = 32;
            int wordsRead = 0;
            int uploadIndex = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
            unsigned int blankVal = DevFile.Families[ActiveFamily].BlankValue;

			timerStart((_TCHAR*)"Blank Check", DevFile.PartsList[ActivePart].ProgramMem / wordsPerLoop);

            do
            {
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0x20 04
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // Length N
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[wordsPerLoop];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 5;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;         // Read & toss 2 response words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read 32 3-byte words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 31;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA_NOLEN;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);  // script length
                writeUSB(commandArrayp, commOffSet);

                GetUpload();
				ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
				int word;
				unsigned int memWord1, memWord2;
                for (word = 0; word < wordsPerLoop; word += 2)
                {
                    // two word2 of packed instructions
                    memWord1 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    memWord2 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    if (wordsRead >= configsStart)
                    {
                        blankVal = (0xFF0000 | (unsigned int)DevFile.PartsList[ActivePart].ConfigBlank[wordsRead - configsStart]);
                    }
                    if (blankVal != memWord1)
                    {
						RunScript(SCR_PROG_EXIT, 1);
						SetProgrammingSpeed(LastICSPSpeed);
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
						ReadError.expected = DeviceBuffers->ProgramMemory[wordsRead];
						ReadError.read = memWord1;
						return false;
                    }
                    wordsRead++;
                    if (wordsRead >= configsStart)
                    {
                        blankVal = (0xFF0000 | (unsigned int)DevFile.PartsList[ActivePart].ConfigBlank[wordsRead - configsStart]);
                    }
                    if (blankVal != memWord2)
                    {
						RunScript(SCR_PROG_EXIT, 1);
						SetProgrammingSpeed(LastICSPSpeed);
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
						ReadError.expected = DeviceBuffers->ProgramMemory[wordsRead];
						ReadError.read = memWord2;
						return false;
                    }
                    wordsRead++;
                    if (wordsRead >= (int)DevFile.PartsList[ActivePart].ProgramMem)
                    {
                        break; // for cases where ProgramMemSize%WordsPerLoop != 0
                    }
                }
                timerPrint();
            } while (wordsRead < (int)DevFile.PartsList[ActivePart].ProgramMem);

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::PE24FRead(void)
        {
            if (!PE24F_DownloadAndConnect())
            {
                return false;
            }

            // Read Program Memory ====================================================================================
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            //int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
            int wordsPerLoop = 32;
            int wordsRead = 0;
            int uploadIndex = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
			timerStart((_TCHAR*)"Read Flash", DevFile.PartsList[ActivePart].ProgramMem / wordsPerLoop);

            do
            {
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0x20 04
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // Length N
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[wordsPerLoop];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 5;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;         // Read & toss 2 response words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read 32 3-byte words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 31;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA_NOLEN;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);  // script length

                writeUSB(commandArrayp, commOffSet);

                GetUpload();
                ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
				int word;
				unsigned int memWord1, memWord2;
                for (word = 0; word < wordsPerLoop; word+= 2)
                {
                    // two word2 of packed instructions
                    memWord1 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    memWord2 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    DeviceBuffers->ProgramMemory[wordsRead++] = memWord1;
                    DeviceBuffers->ProgramMemory[wordsRead++] = memWord2;
                    if (wordsRead >= (int)DevFile.PartsList[ActivePart].ProgramMem)
                    {
                        break; // for cases where ProgramMemSize%WordsPerLoop != 0
                    }
                }
                timerPrint();
            } while (wordsRead < (int)DevFile.PartsList[ActivePart].ProgramMem);

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::PE24FWrite(int endOfBuffer)
        {
            if (!PE24F_DownloadAndConnect())
            {
                return false;
            }

            // Since the PE actually verifies words when it writes, we need the config words
            // filled with valid blank values or the PE PROGP command on them will fail.
            if (endOfBuffer == (int)DevFile.PartsList[ActivePart].ProgramMem)
            {// if we'll be writing configs
				int cfg;
                for (cfg = DevFile.PartsList[ActivePart].ConfigWords; cfg > 0; cfg--)
                {
                    DeviceBuffers->ProgramMemory[endOfBuffer - cfg] &= 
                                (0xFF0000 | (unsigned int)DevFile.PartsList[ActivePart].ConfigBlank[DevFile.PartsList[ActivePart].ConfigWords - cfg]);
                }
            }

            unsigned char downloadBuffer[DOWNLOAD_BUFFER_SIZE];
            int wordsPerLoop = 64;
            int wordsWritten = 0;

			timerStart((_TCHAR*)"Write Flash", endOfBuffer / wordsPerLoop);

            do
            {
                int downloadIndex = 0;
				int word;
				unsigned int memWord;
                for (word = 0; word < wordsPerLoop; word+=2)
                {
                    // Put in packed format for PE  
                    memWord = DeviceBuffers->ProgramMemory[wordsWritten++];
                    downloadBuffer[downloadIndex + 1] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte) (memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 3] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);

                    memWord = DeviceBuffers->ProgramMemory[wordsWritten++];
                    downloadBuffer[downloadIndex + 5] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte) (memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 4] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    memWord >>= 8;
                    downloadBuffer[downloadIndex + 2] = BitReverseTable[(memWord & 0xFF)];
                    //checksumPk2Go += (byte)(memWord & 0xFF);
                    
                    downloadIndex += 6;
                            
                }
                // download data
                int dataIndex = DataClrAndDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, 0);
                while (dataIndex < downloadIndex)
                {
                    dataIndex = DataDownload(downloadBuffer, DOWNLOAD_BUFFER_SIZE, dataIndex);
                }

                int commOffSet = 0;
                unsigned char commandArrayp[BUF_SIZE];
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0; // fill in later
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // PROGP = 0x50 63
                commandArrayp[commOffSet++] = 0x0A;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0xC6;                      // PE talks MSB first, script routines are LSB first.
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[(((wordsWritten - 64) << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;  // write 64 3-byte words
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 63;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 118;                        //2.5ms
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read response
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);
                writeUSB(commandArrayp, commOffSet);
                if (!readUSB())
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if (Usb_read_array[0] != 4) // expect 4 bytes back : 0x15 00 00 02
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }
                if ((BitReverseTable[Usb_read_array[1]] != 0x15) || (Usb_read_array[2] != 0x00)
                    || (Usb_read_array[3] != 0x00) || (BitReverseTable[Usb_read_array[4]] != 0x02))
                {
                    printf("Programming Executive Error during Write.\n");
					fflush(stdout);
                    RunScript(SCR_PROG_EXIT, 1);
                    SetProgrammingSpeed(LastICSPSpeed);
                    return false;
                }

                timerPrint();
            } while (wordsWritten < endOfBuffer);

			// Don't exit programming mode - if CP bits are set, verify will fail.
            //RunScript(SCR_PROG_EXIT, 1);
            //SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::PE24FVerify(bool writeVerify, int lastLocation, bool PEConnect)
        {
			if (PEConnect)
			{
				if (!PE24F_DownloadAndConnect())
				{
					return false;
				}
			}
            
            if (!writeVerify)
                lastLocation = (int)DevFile.PartsList[ActivePart].ProgramMem;


            // Check Program Memory ====================================================================================
            unsigned char upload_buffer[UPLOAD_BUFFER_SIZE];
            //int bytesPerWord = DevFile.Families[ActiveFamily].BytesPerLocation;
            int wordsPerLoop = 32;
            int wordsRead = 0;
            int uploadIndex = 0;
            int commOffSet = 0;
            unsigned char commandArrayp[BUF_SIZE];
			timerStart((_TCHAR*)"Verify Flash", DevFile.PartsList[ActivePart].ProgramMem / wordsPerLoop);

            do
            {
                commOffSet = 0;
                commandArrayp[commOffSet++] = FWCMD_EXECUTE_SCRIPT;
                commandArrayp[commOffSet++] = 0;
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x00; // PGD is output
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; //0x20 04
                commandArrayp[commOffSet++] = 0x04;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = 0x20;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // Length N
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[wordsPerLoop];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address MSW
                commandArrayp[commOffSet++] = 0x00;
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 15) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL; // address LSW
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead >> 7) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_WRITE_BYTE_LITERAL;
                commandArrayp[commOffSet++] = BitReverseTable[((wordsRead << 1) & 0xFF)];
                commandArrayp[commOffSet++] = SCMD_SET_ICSP_PINS;
                commandArrayp[commOffSet++] = 0x02; // PGD is input
                commandArrayp[commOffSet++] = SCMD_DELAY_SHORT;
                commandArrayp[commOffSet++] = 5;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;         // Read & toss 2 response words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;  // read 32 3-byte words
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_READ_BYTE_BUFFER;
                commandArrayp[commOffSet++] = SCMD_LOOP;
                commandArrayp[commOffSet++] = 3;
                commandArrayp[commOffSet++] = 31;
                commandArrayp[commOffSet++] = FWCMD_UPLOAD_DATA_NOLEN;
                commandArrayp[1] = (unsigned char)(commOffSet - 3);  // script length
                writeUSB(commandArrayp, commOffSet);

                GetUpload();
				ArrayCopy(Usb_read_array, 0, upload_buffer, 0, MAX_BYTES);
				UploadDataNoLen();
				ArrayCopy(Usb_read_array, 0, upload_buffer, MAX_BYTES, MAX_BYTES);
                uploadIndex = 0;
				int word;
				unsigned int memWord1, memWord2;
                for (word = 0; word < wordsPerLoop; word += 2)
                {
                    // two word2 of packed instructions
                    memWord1 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    memWord2 = (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord1 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 16;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]] << 8;
                    memWord2 |= (unsigned int)BitReverseTable[upload_buffer[uploadIndex++]];
                    if (DeviceBuffers->ProgramMemory[wordsRead++] != memWord1)
                    {
						RunScript(SCR_PROG_EXIT, 1);
						SetProgrammingSpeed(LastICSPSpeed);
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
						ReadError.expected = DeviceBuffers->ProgramMemory[wordsRead];
						ReadError.read = memWord1;
						return false;
                    }
                    if (DeviceBuffers->ProgramMemory[wordsRead++] != memWord2)
                    {
						RunScript(SCR_PROG_EXIT, 1);
						SetProgrammingSpeed(LastICSPSpeed);
						_tcsncpy_s(ReadError.memoryType, "Program", 7);
						ReadError.address = --wordsRead * DevFile.Families[ActiveFamily].AddressIncrement;
						ReadError.expected = DeviceBuffers->ProgramMemory[wordsRead];
						ReadError.read = memWord2;
						return false;
                    }
                    if (wordsRead >= lastLocation)
                    {
                        break; // for cases where ProgramMemSize%WordsPerLoop != 0
                    }
                }
                timerPrint();
            } while (wordsRead < lastLocation);

            RunScript(SCR_PROG_EXIT, 1);
            SetProgrammingSpeed(LastICSPSpeed);
            return true;
        }

bool CPICkitFunctions::useProgExec24F(void)
        {
            if (FamilyIsPIC24F())
            {
                if (DevFile.PartsList[ActivePart].ProgramMem >= 4096)
                // don't use PE on the smallest parts.
                    return usePE33;
            }
            return false;
        }

