#pragma once
#include "usbhidioc.h"
#include "DeviceFile.h"
#include "DeviceData.h"

#define SCRIPT_REDIR_TABLE_LEN 32
#define UPLOAD_BUFFER_SIZE 128
#define DOWNLOAD_BUFFER_SIZE 256
#define READ_MEM 0
#define BLANK_CHECK 1
#define VERIFY_MEM 2
#define VERIFY_NOPRG_ENTRY 3
#define VERIFY_MEM_SHORT 4	
#define STATUS_VDD_ERROR 0x0010
#define STATUS_VPP_ERROR 0x0020
#define READ_BIT true
#define WRITE_BIT false
#define PROTOCOL_CFG 0
#define ADR_MASK_CFG 1
#define ADR_BITS_CFG 2
#define CS_PINS_CFG 3
#define I2C_BUS 1
#define SPI_BUS 2
#define MICROWIRE_BUS 3
#define UNIO_BUS 4
#define ERASE_EE true
#define WRITE_EE false

class CPICkitFunctions
{
	public:
		  CPICkitFunctions(void);
		  ~CPICkitFunctions(void);
		  int ReadPkStatus(void);
		  unsigned int ComputeChecksum(void);
		  unsigned int ReadDeviceID(void);
		  unsigned int GetDeviceRevision(void);
		  bool FamilyIsKeeloq(void);
		  bool FamilyIsEEPROM(void);
		  bool FamilyIsMCP(void);
		  bool FamilyIsPIC32(void);
		  bool WriteDevice(bool progmem, bool eemem, bool uidmem, bool cfgmem, bool useLowVoltageRowErase);
		  bool HCS360_361_VppSpecial(void);
		  void WriteConfigOutsideProgMem();
		  int DataClrAndDownload(unsigned char dataArray[], int arrayLength , int startIndex);
		  int DataDownload(unsigned char dataArray[], int arrayLength , int startIndex);
		  int FindLastUsedInBuffer(unsigned int bufferToSearch[], unsigned int blankValue, int startIndex);
		  bool EraseDevice(bool progmem, bool eemem, bool* useLowVoltageRowErase);
		  bool WriteOSSCAL();
		  void RowEraseDevice(void);
		  bool SerialEEPROMErase(void);
		  bool EepromWrite(bool eraseWrite);
		  bool ExecuteScript(int scriptArrayIndex);
		  bool ReadOSSCAL(void);
		  void ReadBandGap(void);
		  bool SetMCLR(bool nMCLR);
		  bool SendScript(unsigned char script[], int length);
		  bool ReadDevice(char function, bool progmem, bool eemem, bool uidmem, bool cfgmem);
		  bool Download3Multiples(int downloadBytes, int multiples, int increment);
		  bool DownloadAddress3MSBFirst(int address);
		  bool BusErrorCheck(void);
		  bool ReadConfigOutsideProgMem(char function);
		  void ArrayCopy(unsigned char* source, int sourceStart, unsigned char* dest, int destStart, int length);
		  bool DownloadAddress3(int address);
		  bool RunScript(int script, int repetitions);
		  bool RunScriptUploadNoLen(int script, int repetitions);
		  //bool RunScriptUploadNoLen2(int script, int repetitions);
		  bool UploadData(void);
		  bool UploadDataNoLen(void);
		  bool GetUpload(void);
		  bool VddOn(void);
		  bool VddOff(void);
		  void PrepPICkit2(void);
		  bool SetVDDVoltage(float voltage, float threshold);
		  bool SetVppVoltage(float voltage, float threshold);
		  void ResetBuffers(void);
		  bool DetectPICkit2Device(int unitNumber, bool readFWVer);
		  void ClosePICkit2Device(void);
		  void KillSpace(_TCHAR* name, int maxlength);
		  bool FindDevice(_TCHAR* device);
		  bool ReadDeviceFile(_TCHAR* path);
		  float GetDefaultVdd(void);
		  float GetDefaultVpp(void);
		  void SetVddSetPoint(float setPoint);
		  void SetVppSetPoint(float setPoint);
		  void SetVppFirstEnable(bool set);
		  bool GetVppFirstEnable(void);
		  void SetSelfPowered(bool set);
		  bool GetSelfPowered(void);
		  bool ReadPICkitVoltages(float* vdd, float* vpp);
		  void USBClose(void);
		  bool EnterBootloader(void);
		  bool VerifyBootloaderMode(void);
		  bool BL_EraseFlash(void);
		  bool BL_WriteFlash(unsigned char payload[]);
		  bool BL_WriteFWLoadedKey(void);
		  bool BL_ReadFlash16(int address);
		  bool BL_Reset(void);
		  void DelaySeconds(unsigned int seconds);
		  bool UnitIDWrite(_TCHAR* unitID);
		  bool UnitIDRead(_TCHAR* unitID);
		  _TCHAR* GetUnitID(void);
		  bool ResetPICkit2(void);
		  bool SetProgrammingSpeed(unsigned char speed);
		  void SetProgrammingSpeedDefault(unsigned char speed);
		  bool FamilyIsPIC24H(void);
		  bool FamilyIsdsPIC33F(void);
		  bool FamilyIsPIC24F(void);
		  void SetTimerFunctions(bool usePercent, bool useNewlines);
		  bool SearchDevice(int familyIndex);
		  unsigned int ReadVector(void);
		  void WriteVector(int vtop, int vbot);
		  void DisablePE33(void); // dsP33PE
		  bool useProgExec33(void); // dsP33PE
	protected:
		  void processKeeloqData(unsigned char downloadBuffer[], int wordsWritten);
		  bool readEEPROM(char function);
		  int eeprom24BitAddress(int wordsWritten, bool setReadBit);
		  unsigned int getEEBlank(void);
		  void timerStart(_TCHAR* operation, int steps);
		  void timerPrint(void);
		  void timerStop(void);
		  void readString(FILE *datfile, char stringArray[]);
		  void downloadPartScripts(int familyIndex);
		  unsigned int getScriptBufferChecksum(void);
		  bool downloadScript(unsigned char scriptBufferLocation, int scriptArrayIndex);
		  bool writeUSB(unsigned char commandList[], int commandLength);
		  bool readUSB(void);
		  void string2Upper(_TCHAR* lcstring, int maxLength);
          void writeConfigInsideProgramMem(void);
		  void EnterSerialExecution(void);					//PIC32
		  bool DownloadPE(void);							//PIC32
		  int ReadPEVersion(void);							//PIC32
		  bool PEBlankCheck(unsigned int startAddress, unsigned int lengthBytes); //PIC32
		  int PEGetCRC(unsigned int startAddress, unsigned int lengthBytes); //PIC32
		  int addInstruction(unsigned char* commandarray, unsigned int instruction, int offset); //PIC32
		  bool PE_DownloadAndConnect(void);					//PIC32
		  bool PIC32Read(bool progmem, bool uidmem, bool cfgmem); //PIC32
		  bool PIC32BlankCheck(void);						//PIC32
		  bool P32Write(bool progmem, bool uidmem, bool cfgmem);								//PIC32
		  void PEProgramHeader(unsigned int startAddress, unsigned int lengthBytes); //PIC32
		  void PEProgramSendBlock(int index, bool peResp);	//PIC32
		  bool P32Verify(bool writeVerify, bool progmem, bool uidmem, bool cfgmem); //PIC32
		  int p32CRC_buf(unsigned int* buffer, unsigned int startIdx, unsigned int len); //PIC32
		  bool DownloadPE33(void); //dsP33PE
		  bool PE33_Connect(void); // dsP33PE
		  bool PE33_DownloadAndConnect(void); // dsP33PE
		  bool PE33DoBlankCheck(unsigned int lengthWords); // dsP33PE
		  bool PE33BlankCheck(void); // dsP33PE
		  bool PE33Read(void); // dsP33PE
		  bool PE33Write(int endOfBuffer); // dsP33PE
		  bool PE33VerifyCRC(void); // dsP33PE
		  unsigned short CalcCRCProgMem(void); // dsP33PE
		  void CRC33_Calculate(unsigned char ByteValue , unsigned int* CRC_Value); // dsP33PE
		  bool DownloadPE24F(void); // PIC24FPE
		  bool PE24F_Connect(void); // PIC24FPE
		  bool PE24F_DownloadAndConnect(void); // PIC24FPE
		  bool PE24FBlankCheck(void); // PIC24FPE
		  bool PE24FRead(void); // PIC24FPE
		  bool PE24FWrite(int endOfBuffer); // PIC24FPE
		  bool PE24FVerify(bool writeVerify, int lastLocation, bool PEConnect); // PIC24FPE
		  bool useProgExec24F(void); // PIC24FPE

	public:
		  // Constants
		  static const int BUF_SIZE    = 65;
		  static const int MAX_BYTES   = 64;
		  static const unsigned char FW_MAJ_MIN = 2;
		  static const unsigned char FW_MNR_MIN = 32;
		  static const unsigned char FW_DOT_MIN = 0;
		  static const unsigned char BitReverseTable[256];

		  enum FWCommands
			 {
			 FWCMD_ENTER_BOOTLOADER           = 0x42,
			 FWCMD_NO_OPERATION               = 0x5A,
			 FWCMD_FIRMWARE_VERSION           = 0x76,
			 FWCMD_SETVDD                     = 0xA0,
			 FWCMD_SETVPP                     = 0xA1,
			 FWCMD_READ_STATUS                = 0xA2,
			 FWCMD_READ_VOLTAGES              = 0xA3,
			 FWCMD_DOWNLOAD_SCRIPT            = 0xA4,
			 FWCMD_RUN_SCRIPT                 = 0xA5,
			 FWCMD_EXECUTE_SCRIPT             = 0xA6,
			 FWCMD_CLR_DOWNLOAD_BUFFER        = 0xA7,
			 FWCMD_DOWNLOAD_DATA              = 0xA8,
			 FWCMD_CLR_UPLOAD_BUFFER          = 0xA9,
			 FWCMD_UPLOAD_DATA                = 0xAA,
			 FWCMD_CLR_SCRIPT_BUFFER          = 0xAB,
			 FWCMD_UPLOAD_DATA_NOLEN          = 0xAC,
			 FWCMD_END_OF_BUFFER              = 0xAD,
			 FWCMD_RESET                      = 0xAE,
			 FWCMD_SCRIPT_BUFFER_CHKSM        = 0xAF,
			 FWCMD_WR_INTERNAL_EE             = 0xB1,
			 FWCMD_RD_INTERNAL_EE             = 0xB2
			 };

		  enum ScriptCommands
			 {
			 SCMD_VDD_ON                      = 0xFF,
			 SCMD_VDD_OFF                     = 0xFE,
			 SCMD_VDD_GND_ON                  = 0xFD,
			 SCMD_VDD_GND_OFF                 = 0xFC,
			 SCMD_VPP_ON                      = 0xFB,
			 SCMD_VPP_OFF                     = 0xFA,
			 SCMD_VPP_PWM_ON                  = 0xF9,
			 SCMD_VPP_PWM_OFF                 = 0xF8,
			 SCMD_MCLR_GND_ON                 = 0xF7,
			 SCMD_MCLR_GND_OFF                = 0xF6,
			 SCMD_BUSY_LED_ON                 = 0xF5,
			 SCMD_BUSY_LED_OFF                = 0xF4,
			 SCMD_SET_ICSP_PINS               = 0xF3,
			 SCMD_WRITE_BYTE_LITERAL          = 0xF2,
			 SCMD_WRITE_BYTE_BUFFER           = 0xF1,
			 SCMD_READ_BYTE_BUFFER            = 0xF0,

			 SCMD_READ_BYTE                   = 0xEF,
			 SCMD_WRITE_BITS_LITERAL          = 0xEE,
			 SCMD_WRITE_BITS_BUFFER           = 0xED,
			 SCMD_READ_BITS_BUFFER            = 0xEC,
			 SCMD_READ_BITS                   = 0xEB,
			 SCMD_SET_ICSP_SPEED              = 0xEA,
			 SCMD_LOOP                        = 0xE9,
			 SCMD_DELAY_LONG                  = 0xE8,
			 SCMD_DELAY_SHORT                 = 0xE7,
			 SCMD_IF_EQ_GOTO                  = 0xE6,
			 SCMD_IF_GT_GOTO                  = 0xE5, 
			 SCMD_GOTO_INDEX                  = 0xE4,
			 SCMD_EXIT_SCRIPT                 = 0xE3,
			 SCMD_PEEK_SFR                    = 0xE2,
			 SCMD_POKE_SFR                    = 0xE1,
			 SCMD_ICDSLAVE_RX                 = 0xE0,

			 SCMD_ICDSLAVE_TX_LIT             = 0xDF,
			 SCMD_ICDSLAVE_TX_BUF             = 0xDE,
			 SCMD_LOOP_BUFFER                 = 0xDD,
			 SCMD_ICSP_STATES_BUFFER          = 0xDC,
			 SCMD_POP_DOWNLOAD                = 0xDB,
			 SCMD_COREINST18				  = 0xDA,
             SCMD_COREINST24		          = 0xD9,
             SCMD_NOP24                       = 0xD8,
             SCMD_VISI24                      = 0xD7,
             SCMD_RD2_BYTE_BUFFER             = 0xD6,
             SCMD_RD2_BITS_BUFFER             = 0xD5,
             SCMD_WRITE_BUFWORD_W             = 0xD4,
             SCMD_WRITE_BUFBYTE_W             = 0xD3,
             SCMD_CONST_WRITE_DL              = 0xD2,
             SCMD_WRITE_BITS_LIT_HLD          = 0xD1,
             SCMD_WRITE_BITS_BUF_HLD          = 0xD0,

             SCMD_SET_AUX                     = 0xCF,
             SCMD_AUX_STATE_BUFFER            = 0xCE,
             SCMD_I2C_START                   = 0xCD,
             SCMD_I2C_STOP                    = 0xCC,
             SCMD_I2C_WR_BYTE_LIT             = 0xCB,
             SCMD_I2C_WR_BYTE_BUF             = 0xCA,
             SCMD_I2C_RD_BYTE_ACK             = 0xC9,
             SCMD_I2C_RD_BYTE_NACK            = 0xC8,
             SCMD_SPI_WR_BYTE_LIT             = 0xC7,
             SCMD_SPI_WR_BYTE_BUF             = 0xC6,
             SCMD_SPI_RD_BYTE_BUF             = 0xC5,
             SCMD_SPI_RDWR_BYTE_LIT           = 0xC4,
             SCMD_SPI_RDWR_BYTE_BUF           = 0xC3,
             SCMD_ICDSLAVE_RX_BL              = 0xC2,
             SCMD_ICDSLAVE_TX_LIT_BL          = 0xC1,
             SCMD_ICDSLAVE_TX_BUF_BL          = 0xC0,

             SCMD_MEASURE_PULSE               = 0xBF,
             SCMD_UNIO_TX                     = 0xBE,
             SCMD_UNIO_TX_RX                  = 0xBD,
             SCMD_JT2_SETMODE                 = 0xBC,
             SCMD_JT2_SENDCMD                 = 0xBB,
             SCMD_JT2_XFERDATA8_LIT           = 0xBA,
             SCMD_JT2_XFERDATA32_LIT          = 0xB9,
             SCMD_JT2_XFRFASTDAT_LIT          = 0xB8,
             SCMD_JT2_XFRFASTDAT_BUF          = 0xB7,
             SCMD_JT2_XFERINST_BUF            = 0xB6,
             SCMD_JT2_GET_PE_RESP             = 0xB5,
             SCMD_JT2_WAIT_PE_RESP            = 0xB4
			 };

		  // Bootloader commands
		  enum BLCommands
			 {
			 BLCMD_READFWFLASH  = 0x01,
			 BLCMD_WRITEFWFLASH = 0x02,
			 BLCMD_ERASEFWFLASH = 0x03,//0x42, (typo?)BLCMD_ERASEFWFLASH
			 BLCMD_RESETFWDEVICE = 0xFF
			 };

		  // script buffer reserved locations
		  enum SBLocations
			{
			SCR_PROG_ENTRY         = 0,
			SCR_PROG_EXIT          = 1,
			SCR_RD_DEVID           = 2,
			SCR_PROGMEM_RD         = 3,
			SCR_ERASE_CHIP_PREP    = 4,
			SCR_PROGMEM_ADDRSET    = 5,
			SCR_PROGMEM_WR_PREP    = 6,
			SCR_PROGMEM_WR         = 7,
			SCR_EE_RD_PREP         = 8,
			SCR_EE_RD              = 9,
			SCR_EE_WR_PREP         = 10,
			SCR_EE_WR              = 11,
			SCR_CONFIG_RD_PREP     = 12,
			SCR_CONFIG_RD          = 13,
			SCR_CONFIG_WR_PREP     = 14,
			SCR_CONFIG_WR          = 15,
			SCR_USERID_RD_PREP     = 16,
			SCR_USERID_RD          = 17,
			SCR_USERID_WR_PREP     = 18,
			SCR_USERID_WR          = 19,
			SCR_OSSCAL_RD          = 20,
			SCR_OSSCAL_WR          = 21,
			SCR_ERASE_CHIP         = 22,
			SCR_ERASE_PROGMEM      = 23,
			SCR_ERASE_EE           = 24,
			//SCR_ERASE_EE           = 25,
			SCR_ROW_ERASE          = 26,
			SCR_TESTMEM_RD         = 27,
			SCR_EEROW_ERASE        = 28
			};

		  	// PICkit 2 internal EEPROM Locations
		  enum EELocations
			{
			EE_ADC_CAL_L         = 0x00,
			EE_ADC_CAL_H         = 0x01,
			EE_CPP_OFFSET        = 0x02,
			EE_CPP_CAL           = 0x03,
			EE_UNIT_ID           = 0xF0  //through 0xFF
			};

		  // vars
		struct PK2FWver {
			  unsigned char major;
			  unsigned char minor;
			  unsigned char dot; } FirmwareVersion;

		struct RdErr {
			  _TCHAR memoryType[16];
			  unsigned int address;
			  unsigned int expected;
			  unsigned int read; } ReadError;

		CDeviceFile DevFile;
		CDeviceData* DeviceBuffers;
		CDeviceData* BlankBuffers;
		CDeviceData DevBuffs;
		CDeviceData BlnkBuffs;
		int	ActivePart;
		int ActiveFamily;
		unsigned char Usb_write_array[BUF_SIZE];
		unsigned char Usb_read_array[BUF_SIZE];
		bool OverwriteOSCCAL;

	protected:
		CUsbhidioc m_Driver;
		float vddSetPoint;
		float vppSetPoint;
		unsigned int scriptBufferChecksum;
		bool vppFirstEnabled;
		bool targetSelfPowered;
		int LastVerifyLocation;
		unsigned char LastICSPSpeed;
		bool usePE33;
		bool printedUsingPE;
		bool usePercentTimer;
		bool useTimerNewlines;
		float timerIncrement;
		float timerValue;
		_TCHAR* timerOperation;
		unsigned int deviceRevision;
		
		struct scriptRedirect {
            unsigned char redirectToScriptLocation;
            int deviceFileScriptNumber; } scriptRedirectTable[SCRIPT_REDIR_TABLE_LEN];
};


