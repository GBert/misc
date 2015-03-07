// pk2cmd.cpp : Defines the entry point for the console application.
//
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
// version 1.00.00  -  16 October 2007 WEK
// Initial release 
//
// version 1.01.00  -  6 December 2007 WEK
// Added -T and -H options.
//  
// version 1.10.00  -  27 June 2008 WEK, JP, XC, SK
// Source changes for multi-OS support
//      Linux and MacOS platform support was made possible through the contributions 
//      of the following individuals:
//          Jeff Post
//          Xiaofan Chen
//          Shigenobu Kimura
//          Francis Perea
// Add -N option
// Add -S option
// Min FW version is 2.31.00
// Add LastVerifyLocation and VERIFY_MEM_SHORT so the verify on a -M program command of
//     program memory only verifies the portion of memory that was written.  This speeds
//     up programming.  (Excludes PIC18 J-series and PIC24FJ)
// Updated config handling for PIC18 J-Series and PIC24FJ on programming / erasing to
//     match PICkit 2 Programmer v2.5x and MPLAB.
// device file compatibility upped to 5, to use DF v1.51
//     Added support for MCP250xx, 11LCxxx, and PIC32 devices.
// Allows spacing between options and parameters, with the following exceptions:
//     -G option memory ranges must not include a space, i.e. x-y. 
//
// version 1.12.00  -  July 2008 WEK, JP, XC
// Changes to read UnitID via USB Serial Number String descriptor and no longer get FW version
//     when polling for multiple units.  This prevents HID transactions when polling from
//     messing up other software already communicating with a PICkit 2 unit.
// Bug fix - handles programming PIC32 with blank Boot Flash section without error or crash.
// Now executes commands not requiring PICkit 2 hardware (such as checksum on hex file) 
//     without requiring a Pk2 unit.
//
// version 1.20.00 - 7 January 2008 WEK
// Bug fix - deprecated RunScriptUploadNoLen2() as it was causing problems on MacOS X systems
//     due to the handling of the multiple blocking reads.
// Added PE support for PIC24 and dsPIC33 devices.  "-Q" option added to disable use of the PE.
// -B option added to specify the device file location.
// Added "fflush(stdout);" to most printf sections to assist with GUI integration.
// -i option now also shows device revison and part name.
// Bug Fix: fixed an issue with displaying 14 character UnitID strings, and limits Unit IDs
//     to 14 characters.
// -j option added: Provides a % completion on Writes, Reads, Verifies, and Blank Checks of
//     Program Memory (Flash) and Data EEPROM. -jn prints each % update on a newline for GUI
//     interfaces (requested by Alain Gibaud).
// -l (L) option added to allow ICSP clock rate to be specified.
// Bug Fix: LastVerifyLocation was not getting set in CPICkitFunctions::EepromWrite, which
//     caused serial EEPROM devices to not be verified.
// Bug Fix: ComputeChecksum change to correctly compute Baseline checksums when CP is enabled.
// Increased the size of the array "DevicePartParams PartsList[1024]" from 512 in DeviceFile.h
//     as the latest device file has exceeded 512 parts.
// -P modified to support autodetection.  auto detection cannot be used with -V or -X.
//     Added new return code AUTODETECT_FAILED, which means no known part was found.
//     -P with no -T or -R will now always turn VDD off, hold MCLR. (-p requires Pk2 operation)
// Updated PIC32 PE from version 0106 to 0109.  This fixes verify problems with some devices,
//     including PIC32MX320F parts.
// Broke up checkHelp into checkHelp1 and checkHelp2.  The latter has help commands that need
//     the device file, like -?v and -?p
// Added help command "-?P<string>" which allows listing of supported parts and listing of
//     supported parts beginning with the given string.  This uses the natural string sorting
//     algorithm developed by Martin Pool <mbp sourcefrog net>
//     See algorithm files strnatcmp.c/h
// Added -mv, -gv (undocumented)
// Added support for import/export of BIN files for serial EEPROMs.
// Fixed a bug that may have appended a space at the end of -gf file names on Linux.

#include "stdafx.h"
#include "stdio.h"
#include "cmd_app.h"

Ccmd_app pk2app;

#ifdef WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char *argv[])
#endif
{
	pk2app.PK2_CMD_Entry(argc, argv);

	printf("\n");

	if (pk2app.ReturnCode == 0)
	{
		printf("Operation Succeeded\n");
	}

	pk2app.ResetAtExit();
	pk2app.PicFuncs.USBClose();

	//while(1);
	return pk2app.ReturnCode;
}


