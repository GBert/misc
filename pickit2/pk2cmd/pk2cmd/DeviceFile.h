#pragma once

#define OSCCAL_MASK 7

class CDeviceFile
{
	public:
		CDeviceFile(void);
		~CDeviceFile(void);

    struct DeviceFileParams
        {
        // One instance of this structure is included at the start of the Device File
        int VersionMajor;    // Device file version number major.minor.dot
        int VersionMinor;
        int VersionDot;

        char VersionNotes[512]; // Max 512 characters

        int NumberFamilies;  // # number of DeviceFamilyParams sets
        int NumberParts;     // # number of DevicePartParams sets
        int NumberScripts;   // # number of DeviceScripts sets
        unsigned char Compatibility;
        unsigned char UNUSED1A;
        unsigned short int UNUSED1B;
        unsigned int UNUSED2;

        };

    struct DeviceFamilyParams
        {
        // a single struct instance describes the parameters for an entire part family.
        unsigned short int FamilyID;             // # essentially, its array index number.
        unsigned short int FamilyType;           // also used as the display order in the Device Family Menu - lower first
        unsigned short int SearchPriority;
        char FamilyName[24];           // 24 chars max
        unsigned short int ProgEntryScript;
        unsigned short int ProgExitScript;
        unsigned short int ReadDevIDScript;
        unsigned int DeviceIDMask;           // HEX
        unsigned int BlankValue;             // HEX
        unsigned char BytesPerLocation;
        unsigned char AddressIncrement; 
        bool PartDetect;
        unsigned short int ProgEntryVPPScript;   // program entry VPP first
        unsigned short int UNUSED1;
        unsigned char EEMemBytesPerWord;
        unsigned char EEMemAddressIncrement;
        unsigned char UserIDHexBytes;
        unsigned char UserIDBytes;
        unsigned char ProgMemHexBytes;   // added 7-10-06
        unsigned char EEMemHexBytes;   // added 7-10-06
        unsigned char ProgMemShift;   // added 7-10-06
        unsigned int TestMemoryStart;        // HEX
        unsigned short int TestMemoryLength;
        float Vpp;
        };

	struct DevicePartParams
        {
        // a single struct instance describes parameters for a single silicon part.
        char PartName[28];             // 28 chars max
        unsigned short int Family;               // references FamilyID in DeviceFamilyParams
        unsigned int DeviceID;
        unsigned int ProgramMem;
        unsigned short int EEMem;
        unsigned int EEAddr;
        unsigned char ConfigWords;
        unsigned int ConfigAddr;             // HEX
        unsigned char UserIDWords;
        unsigned int UserIDAddr;             // HEX
        unsigned int BandGapMask;            // HEX
        unsigned short int ConfigMasks[9];        // HEX always init array to size 9; note that word 9 comes from later in the device file
        unsigned short int ConfigBlank[9];        // HEX always init array to size 9; note that word 9 comes from later in the device file
        unsigned short int CPMask;               // HEX
        unsigned char CPConfig;
        bool OSSCALSave;
        unsigned int IgnoreAddress;          // HEX
        float VddMin;
        float VddMax;
        float VddErase;
        unsigned char CalibrationWords;
        unsigned short int ChipEraseScript;
        unsigned short int ProgMemAddrSetScript;
        unsigned char ProgMemAddrBytes;
        unsigned short int ProgMemRdScript;
        unsigned short int ProgMemRdWords;
        unsigned short int EERdPrepScript;
        unsigned short int EERdScript;
        unsigned short int EERdLocations;
        unsigned short int UserIDRdPrepScript;
        unsigned short int UserIDRdScript;
        unsigned short int ConfigRdPrepScript;
        unsigned short int ConfigRdScript;
        unsigned short int ProgMemWrPrepScript;
        unsigned short int ProgMemWrScript;
        unsigned short int ProgMemWrWords;
        unsigned char ProgMemPanelBufs;
        unsigned int ProgMemPanelOffset;
        unsigned short int EEWrPrepScript;
        unsigned short int EEWrScript;
        unsigned short int EEWrLocations;
        unsigned short int UserIDWrPrepScript;
        unsigned short int UserIDWrScript;
        unsigned short int ConfigWrPrepScript;
        unsigned short int ConfigWrScript;
        unsigned short int OSCCALRdScript;
        unsigned short int OSCCALWrScript;
        unsigned short int DPMask;
        bool WriteCfgOnErase;
        bool BlankCheckSkipUsrIDs;
        unsigned short int IgnoreBytes;            
        unsigned short int ChipErasePrepScript;
        unsigned int BootFlash;
        //unsigned int UNUSED4; // Removed for compat level 6
		unsigned short int Config9Mask; // Added for compat level 6
		unsigned short int Config9Blank; // Added for compat level 6
        unsigned short int ProgMemEraseScript; // added 7-10-06
        unsigned short int EEMemEraseScript; // added 7-10-06
        unsigned short int ConfigMemEraseScript; // added 7-10-06
        unsigned short int reserved1EraseScript; // added 7-10-06
        unsigned short int reserved2EraseScript; // added 7-10-06
        unsigned short int TestMemoryRdScript;
        unsigned short int TestMemoryRdWords;
        unsigned short int EERowEraseScript;
        unsigned short int EERowEraseWords;
        bool ExportToMPLAB;
        unsigned short int DebugHaltScript;
        unsigned short int DebugRunScript;
        unsigned short int DebugStatusScript;
        unsigned short int DebugReadExecVerScript;
        unsigned short int DebugSingleStepScript;
        unsigned short int DebugBulkWrDataScript;
        unsigned short int DebugBulkRdDataScript;
        unsigned short int DebugWriteVectorScript;
        unsigned short int DebugReadVectorScript;
        unsigned short int DebugRowEraseScript;
        unsigned short int DebugRowEraseSize;
        unsigned short int DebugReserved5Script;
        unsigned short int DebugReserved6Script;
        unsigned short int DebugReserved7Script;
        unsigned short int DebugReserved8Script;
        //unsigned short int DebugReserved9Script; // Removed for compat level 6
		unsigned short int LVPScript; // Added for compat level 6
        };

    struct DeviceScripts
        {
        unsigned short int ScriptNumber;         // # Essentially, its array index number + 1 based 0 reserved for no script
        // referred to in the XxxxxxScript fields of DevicePartParams
        char ScriptName[32];         
        unsigned short int ScriptVersion;        // increments on each change
        unsigned int UNUSED1;
        unsigned short int ScriptLength;
        unsigned short int Script[64];
        char Comment[128];         
        };

	DeviceFileParams Info;
	DeviceFamilyParams Families[32];
	DevicePartParams PartsList[1024];
	DeviceScripts Scripts[512];

};
