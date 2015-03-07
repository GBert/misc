#pragma once

#define	MAX_MEM	262144
#define	MAX_EE	8192
#define	MAX_CFG	9 // Changed for compat level 6
#define	MAX_UID	128

class CDeviceData
{
	public:
		CDeviceData(void);
		~CDeviceData(void);
		void ClearAllData(unsigned int progMemSize, unsigned short int eeMemSize, unsigned char numConfigs, 
							   unsigned char  numIDs, unsigned int memBlankVal, int eeBytes, int idBytes, 
							   unsigned short int configBlank[], unsigned int OSCCALInit);
		void ClearProgramMemory(unsigned int progMemSize, unsigned int memBlankVal);
		void ClearConfigWords(unsigned char numConfigs, unsigned short int configBlank[]);
		void ClearUserIDs(unsigned char numIDs, int idBytes, unsigned int memBlankVal);
		void ClearEEPromMemory(unsigned short int eeMemSize, int eeBytes, unsigned int memBlankVal);

		unsigned int ProgramMemory[MAX_MEM];
		unsigned int EEPromMemory[MAX_EE];
		unsigned int ConfigWords[MAX_CFG];
		unsigned int UserIDs[MAX_UID];
		unsigned int OSCCAL;
		unsigned int BandGap;
};
