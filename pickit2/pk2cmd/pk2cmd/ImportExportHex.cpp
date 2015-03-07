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
#include "ImportExportHex.h"
#include "PIC32PE.h"

CImportExportHex::CImportExportHex(void)
{
}

CImportExportHex::~CImportExportHex(void)
{
}

bool CImportExportHex::ImportHexFile(_TCHAR* filePath, CPICkitFunctions* picFuncs)
{
	bool ret = true;
	FILE *hexfile;
	errno_t err;

	picFuncs->ResetBuffers();

	if ((err = _tfopen_s(&hexfile, filePath, "rt")) == 0)
	{	
        int bytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].ProgMemHexBytes;
        int eeMemBytes = picFuncs->DevFile.Families[picFuncs->ActiveFamily].EEMemHexBytes;
        unsigned int eeAddr = picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEAddr;
        int progMemSizeBytes = (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem * bytesPerWord;
        int segmentAddress = 0;
        bool configRead = false;
        bool lineExceedsFlash = true;
        bool fileExceedsFlash = false;
        int userIDs = picFuncs->DevFile.PartsList[picFuncs->ActivePart].UserIDWords;
        unsigned int userIDAddr = picFuncs->DevFile.PartsList[picFuncs->ActivePart].UserIDAddr;
        if (userIDAddr == 0)
        {
            userIDAddr = 0xFFFFFFFF;
        }
        int userIDMemBytes = picFuncs->DevFile.Families[picFuncs->ActiveFamily].UserIDHexBytes;
        // need to set config words to memory blank.
        int configWords = picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigWords;
        for (int cw = 0; cw < configWords; cw++)
        {
            picFuncs->DeviceBuffers->ConfigWords[cw] = picFuncs->DevFile.Families[picFuncs->ActiveFamily].BlankValue;
        }
		int cfgBytesPerWord = bytesPerWord;
		unsigned int programMemStart = 0;
		unsigned int bootMemStart = 0;
		unsigned int bootMemSize = picFuncs->DevFile.PartsList[picFuncs->ActivePart].BootFlash;
		if (picFuncs->FamilyIsPIC32())
        { // PIC32
            programMemStart = K_P32_PROGRAM_FLASH_START_ADDR;
            bootMemStart = K_P32_BOOT_FLASH_START_ADDR;
            progMemSizeBytes -= (int)bootMemSize * bytesPerWord;
            progMemSizeBytes += (int)programMemStart;
            cfgBytesPerWord = 2;
        }
        unsigned int bootMemEnd = bootMemStart + (bootMemSize * (unsigned int)bytesPerWord);   
        int bootArrayStart = (int)(picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem - bootMemSize);

		_TCHAR fileLine[MAX_LINE_LEN] ="";
		while (!feof(hexfile))
		{
		   if (_fgetts(fileLine, MAX_LINE_LEN, hexfile) == NULL)
		   {
			   printf("\n Error reading hex file.\n");
			   	if (hexfile != NULL)
					fclose(hexfile);
				return false;
		   }
           if ((fileLine[0] == ':') && (_tcslen(fileLine) >= 11))
            { // skip line if not hex line entry,or not minimum length ":BBAAAATTCC"
                int byteCount = ParseHex(&fileLine[1], 2);
				int fileAddress = segmentAddress + ParseHex(&fileLine[3], 4);
				int recordType = ParseHex(&fileLine[7], 2);

                if (recordType == 0)
                { // Data Record}
                    if ((int)_tcslen(fileLine) >= (11+ (2* byteCount)))
                    { // skip if line isn't long enough for bytecount.                    

                        for (int lineByte = 0; lineByte < byteCount; lineByte++)
						{
                            int byteAddress = fileAddress + lineByte;
                            // compute array address from hex file address # bytes per memory location
                            int arrayAddress = (byteAddress - (int)programMemStart) / bytesPerWord;
                            // compute byte position withing memory word
                            int bytePosition = byteAddress % bytesPerWord;
                            // get the byte value from hex file
                            unsigned int wordByte = 0xFFFFFF00 | ParseHex(&fileLine[9 + (2 * lineByte)], 2); 
                            // shift the byte into its proper position in the word.
                            for (int shift = 0; shift < bytePosition; shift++)
							{ // shift byte into proper position
                                wordByte <<= 8;
                                wordByte |= 0xFF; // shift in ones.
                            }

                           lineExceedsFlash = true; // if not in any memory section, then error

                            // program memory section --------------------------------------------------
                            if ((byteAddress < progMemSizeBytes) && (byteAddress >= (int)programMemStart))
                            { 
                                picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] &= wordByte; // add byte.
                                lineExceedsFlash = false;
                                //NOTE: program memory locations containing config words may get modified
                                // by the config section below that applies the config masks.
                            }

                            // boot memory section --------------------------------------------------
                            if ((bootMemSize > 0) && (byteAddress >= (int)bootMemStart) && (byteAddress < (int)bootMemEnd))
                            {              
                                arrayAddress = (int)(bootArrayStart + ((byteAddress - bootMemStart) / bytesPerWord));
                                picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] &= wordByte; // add byte.
                                lineExceedsFlash = false;
                                //NOTE: program memory locations containing config words may get modified
                                // by the config section below that applies the config masks.
                            }  
                            
                            // EE data section ---------------------------------------------------------
                            if ((byteAddress >= (int)eeAddr) && (eeAddr > 0) && ((int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEMem > 0))
                            {
                                int eeAddress = (int)(byteAddress - eeAddr) / eeMemBytes;
                                if (eeAddress < picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEMem)
                                {
                                    lineExceedsFlash = false;
                                    if (eeMemBytes == bytesPerWord)
                                    { // same # hex bytes per EE location as ProgMem location
                                        picFuncs->DeviceBuffers->EEPromMemory[eeAddress] &= wordByte; // add byte.    
                                    }
                                    else
                                    {  // PIC18F/J
                                        int eeshift = (bytePosition / eeMemBytes) * eeMemBytes;
                                        for (int reshift = 0; reshift < eeshift; reshift++)
                                        { // shift byte into proper position
                                            wordByte >>= 8;
                                        }
                                        picFuncs->DeviceBuffers->EEPromMemory[eeAddress] &= wordByte; // add byte. 
                                    }
                                }
                            }
                            // Some 18F parts without EEPROM have hex files created with blank EEPROM by MPLAB
                            else if ((byteAddress >= (int)eeAddr) && (eeAddr > 0) && ((int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEMem == 0))
                            {
                                lineExceedsFlash = false; // don't give too-large file error.
                            }
                            // Config words section ----------------------------------------------------
                            if ((byteAddress >= (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigAddr)
                                    && (configWords > 0))
                            {
                                int configNum = (byteAddress - ((int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigAddr)) / cfgBytesPerWord;
                                if ((cfgBytesPerWord != bytesPerWord) && (bytePosition > 1))
                                { // PIC32
                                    wordByte = (wordByte >> 16) & picFuncs->DevFile.Families[picFuncs->ActiveFamily].BlankValue;
                                }
                                if (configNum < picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigWords)
                                {
                                    lineExceedsFlash = false;
                                    configRead = true;
                                    picFuncs->DeviceBuffers->ConfigWords[configNum] &= 
                                        (wordByte & picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigMasks[configNum]);
                                    if (byteAddress < progMemSizeBytes)
                                    { // also mask off the word if in program memory.
                                        picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] &= 
                                            (wordByte & picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigMasks[configNum]); // add byte.
										if (picFuncs->DevFile.Families[picFuncs->ActiveFamily].BlankValue == 0xFFFF)
										{// PIC18J
											picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] |= 0xF000; // set upper nibble
										}
										else
										{// PIC24FJ is currently only other case of config in program mem
											picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] |= (0xFF0000);
											picFuncs->DeviceBuffers->ProgramMemory[arrayAddress] |= 
												(picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigBlank[configNum] &
												~ picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigMasks[configNum]);
										}
                                    }
                                }                                    
                                
                            } 
                            
                            // User IDs section ---------------------------------------------------------
                            if (userIDs > 0)
                            {
                                if (byteAddress >= (int)userIDAddr)
                                {
                                    int uIDAddress = (int)(byteAddress - userIDAddr) / userIDMemBytes;
                                    if (uIDAddress < userIDs)
                                    {
                                        lineExceedsFlash = false;
                                        if (userIDMemBytes == bytesPerWord)
                                        { // same # hex bytes per EE location as ProgMem location
                                            picFuncs->DeviceBuffers->UserIDs[uIDAddress] &= wordByte; // add byte.    
                                        }
                                        else
                                        {  // PIC18F/J, PIC24H/dsPIC33
                                            int uIDshift = (bytePosition / userIDMemBytes) * userIDMemBytes;
                                            for (int reshift = 0; reshift < uIDshift; reshift++)
                                            { // shift byte into proper position
                                                wordByte >>= 8;
                                            }
                                            picFuncs->DeviceBuffers->UserIDs[uIDAddress] &= wordByte; // add byte. 
                                        }
                                    }
                                }
                            }
                            // ignore data in hex file 
                            if (picFuncs->DevFile.PartsList[picFuncs->ActivePart].IgnoreBytes > 0)
                            {
                                if (byteAddress >= (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].IgnoreAddress)
                                {
                                    if ( byteAddress < (int)(picFuncs->DevFile.PartsList[picFuncs->ActivePart].IgnoreAddress
                                                        + picFuncs->DevFile.PartsList[picFuncs->ActivePart].IgnoreBytes))
                                    { // if data is in the ignore region, don't do anything with it
                                      // but don't generate a "hex file larger than device" warning.
                                        lineExceedsFlash = false;
                                    }
                                }
                            }
                            
                        } 
                    } 
                    if (lineExceedsFlash)
                    {
                        fileExceedsFlash = true;
                    }                  
                } // end if (recordType == 0) 

                if ((recordType == 2) || (recordType == 4))
                { // Segment address
                    if ((int)_tcslen(fileLine) >= (11 + (2 * byteCount)))
                    { // skip if line isn't long enough for bytecount.                                                    
                        segmentAddress = ParseHex(&fileLine[9], 4);
                    } 
                    if (recordType == 2)
                    {
                        segmentAddress <<= 4;
                    }
                    else
                    {
                        segmentAddress <<= 16;
                    }
                    
                } // end if ((recordType == 2) || (recordType == 4)) 
                
                if (recordType == 1)
                { // end of file record
                    break;
                }                 
            }                  
        }
	}
	else
	{
		printf("\n Hex file not found.\n");
		ret = false;
	}
	if (hexfile != NULL)
		fclose(hexfile);

	return ret;
}

bool CImportExportHex::ImportBINFile(_TCHAR* filePath, CPICkitFunctions* picFuncs)
{ // for serial EEPROMS only
	FILE *binfile;
	errno_t err;

	picFuncs->ResetBuffers();

	if ((err = fopen_s(&binfile, filePath, "rb")) != 0)
	{
		if (binfile != NULL)
			fclose(binfile);
		printf("\n BIN file not found.\n");
		return false;
	}

    int bytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].ProgMemHexBytes;
    int memLoc = 0;
    int bytePosition = 0;
    unsigned char fileByte = 0;
    while ((long)fread(&fileByte, sizeof(fileByte), 1, binfile) == 1)
    {
        if (memLoc >= (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem)
            break; // ignore anything past the expected end of memory

        unsigned int memByte = 0xFFFFFF00 | (unsigned int)fileByte;
        for (int shift = 0; shift < bytePosition; shift++)
        { // shift byte into proper position
            memByte <<= 8;
            memByte |= 0xFF; // shift in ones.
        }
        picFuncs->DeviceBuffers->ProgramMemory[memLoc] &= memByte;
        if (++bytePosition >= bytesPerWord)
        {
            memLoc++;
            bytePosition = 0;
        }
    }

	if (binfile != NULL)
		fclose(binfile);

    return true;
}


int CImportExportHex::ParseHex(_TCHAR* characters, int length)
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



bool CImportExportHex::ExportHexFile(_TCHAR* filePath, CPICkitFunctions* picFuncs)
{
    bool ret = true;
	FILE *hexfile;
	errno_t err;

	_TCHAR hexLine[80] = "";
	_TCHAR hexWord[32] = "";

	if ((err = _tfopen_s(&hexfile, filePath, "wt")) == 0)
	{	
    
		// Start with segment zero
		if (picFuncs->FamilyIsPIC32())
		{
			_fputts(":020000041D00DD\n", hexfile);
		}
		else
		{
			_fputts(":020000040000FA\n", hexfile);
		}

		// Program Memory ----------------------------------------------------------------------------
		int fileSegment = 0;
		int fileAddress = 0;
		int programEnd = (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem;
		if (picFuncs->FamilyIsPIC32())
		{
                fileSegment = (int)(K_P32_PROGRAM_FLASH_START_ADDR >> 16);
                fileAddress = (int)(K_P32_PROGRAM_FLASH_START_ADDR & 0xFFFF);
                programEnd -= (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].BootFlash;
		}
		int arrayIndex = 0;
		int bytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].ProgMemHexBytes;
		int arrayIncrement = 16 / bytesPerWord;     // # array words per hex line.
		if (1)
		{
			do
			{
				_stprintf_s(hexLine, 80, ":10%04X00", fileAddress);
				for (int i = 0; i < arrayIncrement; i++)
				{
					// convert entire array word to hex string of 4 bytes.
					_stprintf_s(hexWord, 32, "%08X", picFuncs->DeviceBuffers->ProgramMemory[arrayIndex + i]);
					for (int j = 0; j < bytesPerWord; j++)
					{
						_tcsncat_s(hexLine, &hexWord[6 - 2 * j], 2);
					}
				}
				_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
				_tcsncat_s(hexLine, hexWord, 3);
				_fputts(hexLine, hexfile);
	            
				fileAddress += 16;
				arrayIndex += arrayIncrement;
	            
				// check for segment boundary
				if ((fileAddress > 0xFFFF) && (arrayIndex < (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem))
				{
					fileSegment += fileAddress >> 16;
					fileAddress &= 0xFFFF;
					_stprintf_s(hexLine, 80, ":02000004%04X", fileSegment);
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile);                       
				}
	        
			} while (arrayIndex < programEnd);
		} 
        // Boot Memory ----------------------------------------------------------------------------
        if ((picFuncs->DevFile.PartsList[picFuncs->ActivePart].BootFlash > 0) && picFuncs->FamilyIsPIC32())
        {
			_fputts(":020000041FC01B\n", hexfile);
            arrayIndex = programEnd;
            programEnd = (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem;          
            fileSegment = (int)(K_P32_BOOT_FLASH_START_ADDR >> 16);
            fileAddress = (int)(K_P32_BOOT_FLASH_START_ADDR & 0xFFFF);
            if (1)
            {
                do
                {
					_stprintf_s(hexLine, 80, ":10%04X00", fileAddress);
                    for (int i = 0; i < arrayIncrement; i++)
                    {
                        // convert entire array word to hex string of 4 bytes.
                        _stprintf_s(hexWord, 32, "%08X", picFuncs->DeviceBuffers->ProgramMemory[arrayIndex + i]);
                        for (int j = 0; j < bytesPerWord; j++)
                        {
                            _tcsncat_s(hexLine, &hexWord[6 - 2 * j], 2);
                        }
                    }
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile);
                    
                    fileAddress += 16;
                    arrayIndex += arrayIncrement;
                    
                    // check for segment boundary
                    if ((fileAddress > 0xFFFF) && (arrayIndex < (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem))
                    {
                        fileSegment += fileAddress >> 16;
                        fileAddress &= 0xFFFF;
					_stprintf_s(hexLine, 80, ":02000004%04X", fileSegment);
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile); 
                                          
                    }

                } while (arrayIndex < programEnd);
            }   
        }
		// EEPROM -------------------------------------------------------------------------------------
		if (1)
		{
			int eeSize = picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEMem;
			arrayIndex = 0;
			if (eeSize > 0)
			{
				unsigned int eeAddr = picFuncs->DevFile.PartsList[picFuncs->ActivePart].EEAddr;
				if ((eeAddr & 0xFFFF0000) > 0)
				{ // need a segment address
					_stprintf_s(hexLine, 80, ":02000004%04X", (eeAddr >> 16));
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile); 
				}
	            
				fileAddress = (int)eeAddr & 0xFFFF;
				int eeBytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].EEMemHexBytes;
				arrayIncrement = 16 / eeBytesPerWord;     // # array words per hex line.
				do
				{
					_stprintf_s(hexLine, 80, ":10%04X00", fileAddress);
					for (int i = 0; i < arrayIncrement; i++)
					{
						// convert entire array word to hex string of 4 bytes.
						_stprintf_s(hexWord, 32, "%08X", picFuncs->DeviceBuffers->EEPromMemory[arrayIndex + i]);
						for (int j = 0; j < eeBytesPerWord; j++)
						{
							_tcsncat_s(hexLine, &hexWord[6 - 2 * j], 2);
						}
					}
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile);

					fileAddress += 16;
					arrayIndex += arrayIncrement;                
				}while (arrayIndex < eeSize);
	        
			}
		} 
		// Configuration Words ------------------------------------------------------------------------
		if (1)
		{
			int cfgBytesPerWord = bytesPerWord;
			if (picFuncs->FamilyIsPIC32())
			{
				cfgBytesPerWord = 2;
			}
			int configWords = picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigWords;
			if ((configWords > 0) && (picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigAddr >
				(picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem * bytesPerWord)))
			{ // If there are Config words and they aren't at the end of program flash
				unsigned int configAddr = picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigAddr;
				if ((configAddr & 0xFFFF0000) > 0)
				{ // need a segment address
					_stprintf_s(hexLine, 80, ":02000004%04X", (configAddr >> 16));
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile); 
				}

				fileAddress = (int)configAddr & 0xFFFF;

				int cfgsWritten = 0;
				for (int lines = 0; lines < (((configWords * cfgBytesPerWord - 1)/16) + 1); lines++)
				{
					int cfgsLeft = configWords - cfgsWritten;
					if (cfgsLeft >= (16 / cfgBytesPerWord))
					{
						cfgsLeft = (16 / cfgBytesPerWord);
					}
					_stprintf_s(hexLine, 80, ":%02X%04X00", (cfgsLeft * cfgBytesPerWord), fileAddress);
					fileAddress += (cfgsLeft * cfgBytesPerWord);
					for (int i = 0; i < cfgsLeft; i++)
					{          
						// convert entire array word to hex string of 4 bytes.
                        unsigned int cfgWord = picFuncs->DeviceBuffers->ConfigWords[cfgsWritten + i];
                        if (picFuncs->FamilyIsPIC32())
                        {// PIC32
                            cfgWord |= ~(unsigned int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigMasks[cfgsWritten + i];
                            cfgWord &= picFuncs->DevFile.PartsList[picFuncs->ActivePart].ConfigBlank[cfgsWritten + i];
                        }
						_stprintf_s(hexWord, 32, "%08X", picFuncs->DeviceBuffers->ConfigWords[cfgsWritten + i]);
						for (int j = 0; j < cfgBytesPerWord; j++)
						{
							_tcsncat_s(hexLine, &hexWord[8 - ((j+1)*2)], 2);
						}
					}
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile);
					cfgsWritten += cfgsLeft;
			   }               
			}
		}
	    
		// UserIDs ------------------------------------------------------------------------------------
		if (1)
		{
			int userIDs = picFuncs->DevFile.PartsList[picFuncs->ActivePart].UserIDWords;
			arrayIndex = 0;
			if (userIDs > 0)
			{
				unsigned int uIDAddr = picFuncs->DevFile.PartsList[picFuncs->ActivePart].UserIDAddr;
				if ((uIDAddr & 0xFFFF0000) > 0)
				{ // need a segment address
					_stprintf_s(hexLine, 80, ":02000004%04X", (uIDAddr >> 16));
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile); 
				}

				fileAddress = (int)uIDAddr & 0xFFFF;
				int idBytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].UserIDHexBytes;
				arrayIncrement = 16 / idBytesPerWord;     // # array words per hex line.
				do
				{
					int remainingBytes = (userIDs - arrayIndex) * idBytesPerWord;
					if (remainingBytes < 16)
					{
						_stprintf_s(hexLine, 80, ":%02X%04X00", remainingBytes, fileAddress);
						arrayIncrement = (userIDs - arrayIndex);
					}
					else
					{
						_stprintf_s(hexLine, 80, ":10%04X00", fileAddress);
					}
					for (int i = 0; i < arrayIncrement; i++)
					{
						// convert entire array word to hex string of 4 bytes.
						_stprintf_s(hexWord, 32, "%08X", picFuncs->DeviceBuffers->UserIDs[arrayIndex + i]);
						for (int j = 0; j < idBytesPerWord; j++)
						{
							_tcsncat_s(hexLine, &hexWord[6 - 2 * j], 2);
						}
					}
					_stprintf_s(hexWord, 32, "%02X\n", computeChecksum(hexLine));
					_tcsncat_s(hexLine, hexWord, 3);
					_fputts(hexLine, hexfile);

					fileAddress += 16;
					arrayIndex += arrayIncrement;
				} while (arrayIndex < userIDs);
			}
		}
		//end of record line. 
		_fputts(":00000001FF\n", hexfile);
	}
	else
	{
		printf("\nError Opening save file.\n");
		ret = false;
	}

	if (hexfile != NULL)
		fclose(hexfile);

    return ret;
}

bool CImportExportHex::ExportBINFile(_TCHAR* filePath, CPICkitFunctions* picFuncs)
{ // for serial EEPROMS only
	unsigned char fileByte = 0;
	FILE *binfile;
	errno_t err;

	if ((err = fopen_s(&binfile, filePath, "wb")) != 0)
	{
		if (binfile != NULL)
			fclose(binfile);
		printf("\nError Opening save file.\n");
		return false;
	}

    int bytesPerWord = picFuncs->DevFile.Families[picFuncs->ActiveFamily].ProgMemHexBytes;
    for (int memLoc = 0; memLoc < (int)picFuncs->DevFile.PartsList[picFuncs->ActivePart].ProgramMem; memLoc++)
    {
        for (int byteNum = 0; byteNum < bytesPerWord; byteNum++)
        {
            fileByte = (unsigned char)((picFuncs->DeviceBuffers->ProgramMemory[memLoc] >> (8 * byteNum)) & 0xFF);
            if ((long)fwrite(&fileByte, sizeof(fileByte), 1, binfile) != 1)
			{
				printf("\nError writing to save file.\n");
				if (binfile != NULL)
					fclose(binfile);
				return false;
			}
        }
    }
	if (binfile != NULL)
		fclose(binfile);

    return true;
}


unsigned char CImportExportHex::computeChecksum(_TCHAR* fileLine)
{
	int byteCount = ParseHex(&fileLine[1], 2);
    if ((int)_tcslen(fileLine) >= (9 + (2* byteCount)))
    { // skip if line isn't long enough for bytecount.             
         int checksum = byteCount;
         for (int i = 0; i < (3 + byteCount); i++)
         {
			checksum += ParseHex(fileLine + 3 + (2*i), 2);
         }
         checksum = 0 - checksum;
         return (unsigned char)(checksum & 0xFF);
    }
    
    return 0;              
}
