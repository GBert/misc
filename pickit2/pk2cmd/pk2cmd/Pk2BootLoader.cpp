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
#include "Pk2BootLoader.h"
#ifdef WIN32
#include "Windows.h"
#endif

Pk2BootLoader::Pk2BootLoader(void)
{
}

Pk2BootLoader::~Pk2BootLoader(void)
{
}

bool Pk2BootLoader::ReadHexAndDownload(_TCHAR* fileName, CPICkitFunctions* picFuncs, int unitIndex)
{
	FILE *hexfile;
	errno_t err;
	_TCHAR fileLine[MAX_BLLINE_LEN] ="";
	unsigned char flashWriteData[3+32];  // 3 address bytes plus 32 data bytes.
	int i;

    if ((err = _tfopen_s(&hexfile, fileName, "rt")) == 0)
	{          
		printf("Downloading OS...\n");
        if (!feof(hexfile))
        {
            picFuncs->EnterBootloader();
			picFuncs->ClosePICkit2Device(); //needed for Macs
            Sleep(3000);                 
            for (i = 0; i < 10; i++)
            {
                if (picFuncs->DetectPICkit2Device(unitIndex, true) && picFuncs->VerifyBootloaderMode())
                {
                    break;
                }
				printf("no pk2\n");
                Sleep(500);                                   
            }
            if (i == 10)
            {
            	if (hexfile != NULL)
					fclose(hexfile);
                return false;
            }
        }
        // erase PICkit 2 firmware flash
        picFuncs->BL_EraseFlash();
        
        bool second16 = false;

        while (!feof(hexfile))
        {
			if(_fgetts(fileLine, MAX_BLLINE_LEN, hexfile) == NULL)
		   {
			   printf("\n Error reading OS hex file.\n");
		   		if (hexfile != NULL)
					fclose(hexfile);
				return false;
		   }
            if ((fileLine[0] == ':') && (_tcslen(fileLine) >= 11))
            { // skip line if not hex line entry,or not minimum length ":BBAAAATTCC"
				int byteCount = ParseHex(&fileLine[1], 2);
				int fileAddress = ParseHex(&fileLine[3], 4);
				int recordType = ParseHex(&fileLine[7], 2);

                if ((second16) && ((fileAddress & 0x00000010) == 0))
                {// if just moved to new 32-byte boundary.
                    picFuncs->BL_WriteFlash(flashWriteData);
                    for (int x = 0; x < 35; x++)
                    { // clear array for skipped bytes in hex file
                        flashWriteData[x] = 0xFF;
                    }
                }
                
                second16 = ((fileAddress & 0x00000010) == 0x10);
                
                if (recordType == 0)
                { // Data Record}
                    if ((fileAddress >= 0x2000) && (fileAddress < 0x7FE0))
                    { // don't program 5555 key at last address until after verification.
                        if (!second16)
                        {
                            int rowAddress = fileAddress & 0xFFE0;
                            flashWriteData[0] = (unsigned char)(rowAddress & 0xFF);
                            flashWriteData[1] = (unsigned char)((rowAddress >> 8) & 0xFF);
                            flashWriteData[2] = 0x00;  // address upper
                        }
                    
                        if ((int)_tcslen(fileLine) >= (11 + (2 * byteCount)))
                        { // skip if line isn't long enough for bytecount.                    
                            int startByte = fileAddress & 0x000F;
                            int endByte = startByte + byteCount;

                            int offset = 3;
                            if (second16)
                            {
                                offset = 19;
                            }
                            for (int rowByte = 0; rowByte < 16; rowByte++)
                            {
                                if ((rowByte >= startByte) && (rowByte < endByte))
                                {
                                    // get the byte value from hex file
									unsigned int wordByte = ParseHex(&fileLine[9 + (2 * (rowByte - startByte))], 2);
                                    flashWriteData[offset + rowByte] = (unsigned char)(wordByte & 0xFF);
                                }

                            }

                        }
                    }
                    
                } // end if (recordType == 0)  

                if (recordType == 1)
                { // end of record
                    break;
                }
            }              
		}
        picFuncs->BL_WriteFlash(flashWriteData); // write last row
    	if (hexfile != NULL)
		{
			fclose(hexfile);
		}
        return true;
	}
	else
    {
        if (hexfile != NULL)
			fclose(hexfile);
        return false;
    }
}   

bool Pk2BootLoader::ReadHexAndVerify(_TCHAR* fileName, CPICkitFunctions* picFuncs)
{

	FILE *hexfile;
	errno_t err;
	bool verified = true;       
    int lastAddress = 0;  
	_TCHAR fileLine[MAX_BLLINE_LEN] ="";

    if ((err = _tfopen_s(&hexfile, fileName, "rt")) == 0)
	{          
		printf("Verifying new OS...\n");
        while (!feof(hexfile))
        {
		   if (_fgetts(fileLine, MAX_BLLINE_LEN, hexfile) == NULL)
		   {
			   printf("\n Error reading OS hex file.\n");
		   		if (hexfile != NULL)
					fclose(hexfile);
				return false;
		   }
			if ((fileLine[0] == ':') && (_tcslen(fileLine) >= 11))
            { // skip line if not hex line entry,or not minimum length ":BBAAAATTCC"
				int byteCount = ParseHex(&fileLine[1], 2);
				int fileAddress = ParseHex(&fileLine[3], 4);
				int recordType = ParseHex(&fileLine[7], 2);

                if (recordType == 0)
                { // Data Record}
                    if ((fileAddress >= 0x2000) && (fileAddress < 0x7FE0))
                    { // don't check bootloader stuff.
                        int startByte = fileAddress & 0x000F; // read 16 bytes at a time.
                        int firstAddress = fileAddress & 0xFFF0;
                        if (lastAddress != firstAddress)
                        { // only read if next line in different 16-byte block
                            picFuncs->BL_ReadFlash16(firstAddress);
                        }
                        if ((int)_tcslen(fileLine) >= (11 + (2 * byteCount)))
                        { // skip if line isn't long enough for bytecount.                    
                            for (int lineByte = 0; lineByte < byteCount; lineByte++)
                            {
                                // get the byte value from hex file
                                unsigned int wordByte = ParseHex(&fileLine[9 + (2 * lineByte)], 2);
                                if (picFuncs->Usb_read_array[5 + startByte + lineByte] != (unsigned char)(wordByte & 0xFF))
                                {
                                    verified  = false;
                                    recordType = 1;
                                    break;   
                                }
                            }
                        }
                        lastAddress = firstAddress;
                    }
                } // end if (recordType == 0)  



                if (recordType == 1)
                { // end of record
                    break;
                }
            }            

        }
        if (hexfile != NULL)
			fclose(hexfile);
        return verified;
    }
	else
    {
        return false;
    }                

}

int Pk2BootLoader::ParseHex(_TCHAR* characters, int length)
{
	int integer = 0;
	
	for (int i = 0; i < length; i++)
	{
		integer *= 16;
		switch(*(characters + i))
		{
			case '1':
				integer += 1;
				break;

			case '2':
				integer += 2;
				break;

			case '3':
				integer += 3;
				break;

			case '4':
				integer += 4;
				break;

			case '5':
				integer += 5;
				break;

			case '6':
				integer += 6;
				break;

			case '7':
				integer += 7;
				break;

			case '8':
				integer += 8;
				break;

			case '9':
				integer += 9;
				break;

			case 'A':
			case 'a':
				integer += 10;
				break;

			case 'B':
			case 'b':
				integer += 11;
				break;

			case 'C':
			case 'c':
				integer += 12;
				break;

			case 'D':
			case 'd':
				integer += 13;
				break;

			case 'E':
			case 'e':
				integer += 14;
				break;

			case 'F':
			case 'f':
				integer += 15;
				break;
		}
	}
	return integer;
}
