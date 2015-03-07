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
#include "DeviceData.h"

CDeviceData::CDeviceData(void)
{
}

CDeviceData::~CDeviceData(void)
{
}

void CDeviceData::ClearAllData(unsigned int progMemSize, unsigned short int eeMemSize, unsigned char numConfigs, 
						   unsigned char  numIDs, unsigned int memBlankVal, int eeBytes, int idBytes, 
						   unsigned short int configBlank[], unsigned int OSCCALInit)
{
	ClearProgramMemory(progMemSize, memBlankVal);
	ClearEEPromMemory(eeMemSize, eeBytes, memBlankVal);
	ClearConfigWords(numConfigs, configBlank);
	ClearUserIDs(numIDs, idBytes, memBlankVal);
	OSCCAL = OSCCALInit | 0xFF;
    BandGap = memBlankVal;
}

void CDeviceData::ClearProgramMemory(unsigned int progMemSize, unsigned int memBlankVal)
{
    if (progMemSize > 0)
    {
        for (unsigned int i = 0; i < progMemSize; i++)
        {
            ProgramMemory[i] = memBlankVal;
        }
    }
}


void CDeviceData::ClearConfigWords(unsigned char numConfigs, unsigned short int configBlank[])
{
    if (numConfigs > 0)
    {
        //init configuration to blank
        for (unsigned int i = 0; i < numConfigs; i++)
        {
            ConfigWords[i] = configBlank[i];
        }
    }
}

void CDeviceData::ClearUserIDs(unsigned char numIDs, int idBytes, unsigned int memBlankVal)
{
    if (numIDs> 0)
    {
        //init user ids to blank
        unsigned int idBlank = memBlankVal;
        if (idBytes == 1)
        {
            idBlank = 0xFF;
        }
        for (unsigned int i = 0; i < numIDs; i++)
        {
            UserIDs[i] = idBlank;
        }
    }
}

void CDeviceData::ClearEEPromMemory(unsigned short int eeMemSize, int eeBytes, unsigned int memBlankVal)
{
    if (eeMemSize > 0)
    {
        //init eeprom to blank
        unsigned int eeBlankVal = 0xFF;
        if (eeBytes == 2)
        {
            eeBlankVal = 0xFFFF;
        }
        if (memBlankVal == 0xFFF)
        { // baseline dataflash
            eeBlankVal = 0xFFF;
        }
        for (unsigned int i = 0; i < eeMemSize; i++)
        {
            EEPromMemory[i] = eeBlankVal;                  // 8-bit eeprom will just use 8 LSBs
        }
    }
}
