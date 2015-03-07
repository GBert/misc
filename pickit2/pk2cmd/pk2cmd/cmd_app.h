#pragma once
#include "PICkitFunctions.h"
#include "ImportExportHex.h"
#include "Pk2BootLoader.h"

#define K_MAX_ARGS	32

// Return Codes
#define OPSUCCESS                   0  // operation completed as expected
#define VOLTAGE_ERROR               5  // Vdd or vpp error detected
#define OPFAILURE                   7  // operation failed
#define NO_PROGRAMMER               10 // Unable to find a PICkit 2 programmer
#define WRONG_OS                    11 // OS firmware must be upgraded
#define FILE_OPEN_ERROR             15 // returned if a file to read to (-gf..) cannot be opened.
#define DEVICEFILE_ERROR            24 // the device file was not found or an error occured while loading.
#define UPDGRADE_ERROR              28 // returned when OS upgrade fails. 
#define PGMVFY_ERROR                34 // returned if program or verify operation fails.
#define INVALID_CMDLINE_ARG         36
#define INVALID_HEXFILE             37 // Error reading hex file.
#define AUTODETECT_FAILED           39 // Couldn't find a part

class Ccmd_app
{
public:
	Ccmd_app(void);
	~Ccmd_app(void);
	void PK2_CMD_Entry(int argc, _TCHAR* argv[]);
	void ResetAtExit(void);

	// Updated to compat level 6, Aug 2010.
	static const unsigned char DevFileCompatLevel = 6;
	static const unsigned char DevFileCompatLevelMin = 0;

	CPICkitFunctions PicFuncs;
	CImportExportHex ImportExportFuncs;
	Pk2BootLoader Pk2BootLoadFuncs;
	int ReturnCode;

protected:
	void processArgvForSpaces(int argc, _TCHAR* argv[]);
	bool Pk2OperationCheck(int argc, _TCHAR* argv[]);
	bool bootloadArg(int argc, _TCHAR* argv[]);
	bool unitIDArg(int argc, _TCHAR* argv[]);
	bool selectUnitArg(int argc, _TCHAR* argv[]);
	int getPk2UnitIndex(void);
	void string2Upper(_TCHAR* lcstring, int maxLength);
	void processArgs(int argc, _TCHAR* argv[]);
	bool detectAllFamilies(int argc, _TCHAR* argv[]);
	void printFamilies(void);
	bool detectSpecificFamily(_TCHAR* idString, int argc, _TCHAR* argv[]);
	bool priority1Args(int argc, _TCHAR* argv[], bool preserveArgs);
	bool checkArgsForBlankCheck(int argc, _TCHAR* argv[]);
	bool priority2Args(int argc, _TCHAR* argv[]);
	bool priority3Args(int argc, _TCHAR* argv[]);
	bool priority4Args(int argc, _TCHAR* argv[]);
	bool delayArg(int argc, _TCHAR* argv[]);
	void printProgramRange(int startAddr, int stopAddr);
	void printEEDataRange(int startAddr, int stopAddr);
	void printUserIDs(void);
	void printConfiguration(void);
	bool getRange(int* start, int* stop, _TCHAR* str_range);
	bool getValue(unsigned int* value, _TCHAR* str_value);
	bool checkSwitch(_TCHAR* argv);
	bool findPICkit2(int unitIndex);
	void printMemError(void);
	bool checkDevFilePathOptionB(int argc, _TCHAR* argv[], _TCHAR* path_string);
	bool checkHelp1(int argc, _TCHAR* argv[]);
	bool checkHelp2(int argc, _TCHAR* argv[], bool loadDeviceFileFailed);
	void displayHelp(void);
	void displayLicense(void);
	void displayPartList(int argc, _TCHAR* argv[], _TCHAR* argSearch);
	static int strnatcmpWrapper(const void *a, const void *b);

	bool preserveEEPROM;
	bool hexLoaded;
	bool usingLowVoltageErase;
	bool resetOnExit;
	bool Pk2Operation;		// operation does require connecting to/using PICkit 2
	int	 pk2UnitIndex;
	int	 nargc;
	char* nargv[K_MAX_ARGS];
};
