/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "VERSION"

#include "pickle.h"

/*
 * pickle session
 */
struct pickle p = {0};

/*
 * pickle help
 */
void
usage_pickle(void)
{
	printf("USAGE: pickle\n");
	printf("Usage information.\n\n");

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("FRONTENDS:\n"
		" FILE\n"
		"\t\tFile input (INHX32 format).\n"
		" STDIN\n"
		"\t\tStandard input (INHX32 format).\n"
#ifdef TTY
		" TTY\n"
		"\t\tPOSIX serial I/O.\n"
#endif
		"\n");

	printf("BACKENDS:\n"
#ifdef BITBANG
		" BIT-BANG\n"
		"\t\tLinux GPIO bit-bang.\n"
#endif
#ifdef FTDI
		" FTDI BIT-BANG\n"
		"\t\tLinux FTDI bit-bang.\n"
#endif
#ifdef MCP23017
		" MCP23017\n"
		"\t\tLinux MCP23017 I2C.\n"
#endif
#ifdef RPI
		" RPI\n"
		"\t\tRaspberry Pi GPIO.\n"
#endif
#ifdef TTY
		" TTY\n"
		"\t\tPOSIX serial I/O.\n"
#endif
		"\n");

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n"
#ifdef P12
		" p12 SELECT DEVICE OPERATION [ARG]\n"
		"\t\t12-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef P14
		" p14 [SELECT DEVICE] [LVP] OPERATION [ARG]\n"
		"\t\t14-bit word PIC10F/12F/16F operations.\n"
#endif
#ifdef P16
		" p16 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n"
		"\t\t16-bit word PIC18F operations.\n"
#endif
#ifdef P24
		" p24 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n"
		"\t\t24-bit word PIC24/dsPIC operations.\n"
#endif
#ifdef P32
		" p32 OPERATION [ARG]\n"
		"\t\t32-bit word PIC32 operations.\n"
#endif
#ifdef PCTRL
		" pctrl RUN|STOP|RESTORE\n"
		"\t\tControl master clear.\n"
#endif
#ifdef TTY
		" pload PROGRAM|VERIFY TTY|IP FILE [16|24|32]\n"
		"\t\tProgram or verify file on TTY or network.\n"
#endif
#ifdef PTEST
		" ptest TEST [ARG]\n"
		"\t\tHardware tests.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	io_exit(EX_OK);
}

/*
 * pctrl help
 */
#ifdef PCTRL
void
usage_pctrl(char *msg)
{
	printf("USAGE: pctrl RUN|STOP|RESTORE\n");
	printf("Control master clear.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n"
		" pctrl RUN\n"
		"\t\tRaise master clear to take the device out of reset.\n"
		" pctrl STOP\n"
		"\t\tLower master clear to put the device in reset.\n"
		" pctrl RESTORE\n"
		"\t\tLower then raise master clear to reset the device.\n"

		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * pload help
 */
#ifdef TTY
void
usage_pload(char *msg)
{
	printf("USAGE: pload PROGRAM|VERIFY TTY|IP FILE [16|24|32]\n");
	printf("Program or verify file on TTY or network.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n"
		" pload program /dev/ttyS0 file.hex\n"
		"\t\tProgram file.hex (INHX32 format) on /dev/ttyS0.\n"
		" pload verify /dev/ttyS0 file.hex\n"
		"\t\tVerify file.hex (INHX32 format) on /dev/ttyS0.\n"
		" pload program 192.168.1.100 < led.hex\n"
		"\t\tProgram stdin (INHX32 format) on 192.168.1.100 port 8048.\n"
		" pload program /dev/ttyAMA0 led.hex 24\n"
		"\t\tProgram led.hex (INHX32 format) on /dev/ttyAMA0 using 24-bit addressing.\n"

		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * ptest help
 */
#ifdef PTEST
void
usage_ptest(char *msg)
{
	printf("USAGE: ptest TEST ARG\n");
	printf("Hardware tests.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n"
		" ptest VPP|PGC|PGD|PGM 5\n"
		"\t\tVPP, PGC, PGD or PGM LOW->HIGH->LOW test with 5 seconds high time.\n"
#ifdef RPI
		" ptest 0 10\n"
		"\t\tR-PI GPIO test with 10 seconds mark time.\n"
#endif
		" ptest 1 10\n"
		"\t\tD-SUB-9 test with 10 seconds per step.\n"
		" ptest 2 10\n"
		"\t\tICSP test with 10 seconds per step.\n"
		" ptest 3 0\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with no mark time.\n"
		" ptest 3 1\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with SLEEP mark time.\n"
		" ptest 3 100\n"
		"\t\tD-SUB-9 RTS 7 (PGC) DTR 4 (PGD) test with 100 microseconds mark time.\n"
		" ptest 4 100\n"
		"\t\t16F627 debug test with 100 microseconds clock mark time.\n"
#if defined(PTEST) && defined(PIO)
		" ptest 5 100\n"
		"\t\tICSPIO demo test with 100 microseconds clock mark time.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p12 help
 */
#ifdef P12
void
usage_p12(char *msg)
{
	printf("USAGE: p12 SELECT DEVICE OPERATION [ARG]\n");
	printf("12-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p12 %ss%select\n"
		"\t\tDump supported devices.\n", UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sda%sta\n"
		"\t\tDisplay data flash content.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %so%ssccal\n"
		"\t\tDisplay oscillator calibration.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %so%ssccal 0x0c1a\n"
		"\t\tRestore oscillator calibration as 0x0c1a.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX %svi%sew [file.hex]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF, UL_ON, UL_OFF);
	printf(" p12 %ss%select PIC1XFXXX /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF, UL_ON, UL_OFF);

	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p14 help
 */
#ifdef P14
void
usage_p14(char *msg)
{
	printf("USAGE: p14 [SELECT DEVICE] [LVP] OPERATION [ARG]\n");
	printf("14-bit word PIC10F/12F/16F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p14 %ss%select\n"
		"\t\tDump supported devices.\n", UL_ON, UL_OFF);
	printf(" p14 %ss%select 16F84 OPERATION [ARG]\n"
		"\t\tSelect device PIC16F84.\n", UL_ON, UL_OFF);
	printf(" p14 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p14 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p14 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p14 %sc%sonfig 0x3000\n"
		"\t\tRestore band-gap configuration as 0x3000.\n", UL_ON, UL_OFF);
	printf(" p14 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p14 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
	printf(" p14 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p14 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
	printf(" p14 %so%ssccal\n"
		"\t\tDisplay oscillator calibration.\n", UL_ON, UL_OFF);
	printf(" p14 %so%ssccal 0x343c\n"
		"\t\tRestore oscillator calibration as 0x343c.\n", UL_ON, UL_OFF);
	printf(" p14 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 %svi%sew [file.hex]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p14 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);

	printf("\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p16 help
 */
#ifdef P16
void
usage_p16(char *msg)
{
	printf("USAGE: p16 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n");
	printf("16-bit word PIC18F operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p16 %ss%select\n"
		"\t\tDump supported devices.\n", UL_ON, UL_OFF);
	printf(" p16 %ss%select 18LF2539 OPERATION [ARG]\n"
		"\t\tSelect device PIC18LF2539.\n", UL_ON, UL_OFF);
	printf(" p16 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p16 %sh%svp OPERATION [ARG]\n"
		"\t\tHVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p16 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p16 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p16 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p16 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %ser%sase eeprom | flash | id | row [n]\n"
		"\t\tErase EEPROM, flash, id or flash at row for n rows.\n", UL_ON, UL_OFF);
	printf(" p16 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p16 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
	printf(" p16 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 %svi%sew [file.hex]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p16 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);

	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p24 help
 */
#ifdef P24
void
usage_p24(char *msg)
{
	printf("USAGE: p24 [SELECT DEVICE] [LVP|HVP] OPERATION [ARG]\n");
	printf("24-bit word PIC24/dsPIC operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p24 %ss%select\n"
		"\t\tDump supported devices.\n", UL_ON, UL_OFF);
	printf(" p24 %ss%select 33EP128GP502 OPERATION [ARG]\n"
		"\t\tSelect device dsPIC33EP128GP502.\n", UL_ON, UL_OFF);
	printf(" p24 %ss%select 24FJ128GB202 OPERATION [ARG]\n"
		"\t\tSelect device PIC24FJ128GB202.\n", UL_ON, UL_OFF);
	printf(" p24 %sl%svp OPERATION [ARG]\n"
		"\t\tLVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p24 %sh%svp OPERATION [ARG]\n"
		"\t\tHVP 32-bit key entry.\n", UL_ON, UL_OFF);
	printf(" p24 %sb%slank\n"
		"\t\tBlank device (disable protection and bulk erase).\n", UL_ON, UL_OFF);
	printf(" p24 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p24 %sda%sta\n"
		"\t\tDisplay data EEPROM content.\n", UL_ON, UL_OFF);
	printf(" p24 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %sex%sec [n] [address]\n"
		"\t\tDisplay all or n words of executive flash content from address.\n", UL_ON, UL_OFF);
	printf(" p24 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p24 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
	printf(" p24 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 %svi%sew [file.hex]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p24 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);

	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p32 help
 */
#ifdef P32
void
usage_p32(char *msg)
{
	printf("USAGE: p32 OPERATION [ARG]\n");
	printf("32-bit word PIC32 operations.\n\n");

	if (msg)
		printf("Error: %s.\n\n", msg);

	printf("FILES:\n"
		" %s\n"
		"\t\tConfiguration file.\n\n", p.dotfile);

	printf("ENVIRONMENT:\n"
		" PICKLE\n"
		"\t\tConfiguration file.\n\n");

	printf("EXAMPLES:\n");
	printf(" p32 %ss%select\n"
		"\t\tDump supported devices.\n", UL_ON, UL_OFF);
	printf(" p32 %sbo%sot [n] [address]\n"
		"\t\tDisplay all or n words of boot flash content from address.\n", UL_ON, UL_OFF);
	printf(" p32 %sb%slank\n"
		"\t\tBlank device (erase).\n", UL_ON, UL_OFF);
	printf(" p32 %sc%sonfig\n"
		"\t\tDisplay device configuration.\n", UL_ON, UL_OFF);
	printf(" p32 %sd%sump\n"
		"\t\tDump device content (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %sf%slash [n] [address]\n"
		"\t\tDisplay all or n words of program flash content from address.\n", UL_ON, UL_OFF);
	printf(" p32 %si%sd\n"
		"\t\tDisplay device identification.\n", UL_ON, UL_OFF);
	printf(" p32 %sp%srogram [file.hex] [noblank]\n"
		"\t\tBlank and program file.hex or stdin to flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %sv%serify [file.hex]\n"
		"\t\tVerify file.hex or stdin in flash (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 %svi%sew [file.hex]\n"
		"\t\tView file.hex or stdin (INHX32 format).\n", UL_ON, UL_OFF);
	printf(" p32 /dev/ttyUSB0 | %s8%s048\n"
		"\t\tListen on /dev/ttyUSB0 or network for remote programming.\n", UL_ON, UL_OFF);

	printf("\n");
	
	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}
#endif

/*
 * p12/p14/p16/p24/p32 help
 */
void
usage(char *execname, char *msg)
{
#ifdef P12
	if (strcmp("p12", execname) == 0)
		usage_p12(msg);
#endif
#ifdef P14
	if (strcmp("p14", execname) == 0)
		usage_p14(msg);
#endif
#ifdef P16
	if (strcmp("p16", execname) == 0)
		usage_p16(msg);
#endif
#ifdef P24
	if (strcmp("p24", execname) == 0)
		usage_p24(msg);
#endif
#ifdef P32
	if (strcmp("p32", execname) == 0)
		usage_p32(msg);
#endif
	usage_pickle();
}

/*
 * Reset user
 */
void
resetuid(void)
{
	if (getuid() != geteuid()) {
		if (setuid(getuid()) < 0) {
			printf("%s: fatal error: setuid failed\n", __func__);
			io_exit(EX_OSERR); /* Panic */
		}
	}
}

/*
 * Open device and perform command
 */
int
main(int argc, char **argv)
{
	char *execdup, *execname;

	/* Get exec name */
	execdup = (char *)strdup(argv[0]);
	if (execdup == NULL) {
		printf("%s: fatal error: strdup failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}
	execname = basename(execdup);
	if (execname == NULL) {
		printf("%s: fatal error: basename failed\n", __func__);
		io_exit(EX_OSERR); /* Panic */
	}

	/* Get configuration */
	getconf();
	
	/* Command: pickle */
	if (strcmp(execname, "pickle") == 0) {
		resetuid();
		usage_pickle();
	}
#ifdef TTY
	/* Command: pload */
	if (strcmp(execname, "pload") == 0) {
		resetuid();
		if (argc < 3)
			usage_pload("Missing arg");
		if (argc > 5)
			usage_pload("Too many args");

		int prog_mode = tolower((int)argv[1][0]);
		if (prog_mode != 'p' && prog_mode != 'v')
			usage_pload("Invalid mode");

		/* argv[2] = device or address */

		char file[STRLEN];
		strcpy(file, "-");
		if (argc >= 4)
			strncpy(file, argv[3], STRMAX);

                uint8_t size = 32;
                if (argc == 5) {
                        size = (uint8_t)strtoul(argv[4], NULL, 0);
                        if (size != 16 && size != 24 && size != 32)
                                usage_pload("Invalid size");
                }

		io_signal_on(); // !SIGPIPE
		stk500v2_load(prog_mode, argv[2], file, size);
		io_exit(EX_OK);
	}
#endif
	/* Open device */
	if (io_open() < 0) {
#ifdef PTEST
		if (strcmp(execname, "ptest") == 0)
			usage_ptest(io_error());
#endif
		usage(execname, io_error());
	}

	/* Raise priority */
	setpriority(PRIO_PROCESS, 0, -20);

	/* Reset user */
	resetuid();
#ifdef PCTRL
	/* Command: pctrl */
	if (strcmp(execname, "pctrl") == 0) {
		if (argc < 2)
			usage_pctrl("Missing arg");
		if (argc > 2)
			usage_pctrl("Too many args");
		if (strcasecmp(argv[1], "RUN") == 0) {
			io_close(HIGH);
			printf("RUN\n");
		} else if (strcasecmp(argv[1], "STOP") == 0) {
			io_close(LOW);
			printf("STOP\n");
		} else if (strcasecmp(argv[1], "RESTORE") == 0) {
			io_set_vpp(LOW);
			io_usleep(10);
			io_close(HIGH);
			printf("RESTORE\n");
		}
		io_exit(EX_OK);
	}
#endif
#ifdef PTEST
	/* Command: ptest */
	if (strcmp(execname, "ptest") == 0) {
		if (argc < 3)
			usage_ptest("Missing args");
		if (argc > 3)
			usage_ptest("Too many args");
		int32_t testarg = strtol(argv[2], NULL, 0);
		if (testarg < 0)
			usage_ptest("Invalid arg");

		if (strcasecmp(argv[1], "VPP") == 0) 
			io_test0(0, testarg);
		else if (strcasecmp(argv[1], "PGC") == 0)
			io_test0(1, testarg);
		else if (strcasecmp(argv[1], "PGD") == 0)
			io_test0(2, testarg);
		else if (strcasecmp(argv[1], "PGM") == 0)
			io_test0(3, testarg);
		else if (argv[1][0] >= '0' && argv[1][0] <= '9') {
			int32_t test = strtol(argv[1], NULL, 0);
			switch (test) {
#ifdef RPI
			case 0: gpio_test(testarg);break;
#endif
			case 1: io_test1(testarg); break;
			case 2: io_test2(testarg); break;
			case 3: io_test3(testarg); break;
			case 4: io_test4(testarg); break;
#if defined(PTEST) && defined(PIO)
			case 5: io_test5(testarg); break;
#endif
			default:usage_ptest("Invalid arg");break;
			}
		} else {
			usage_ptest("Invalid arg");
		}
		io_exit(EX_OK);
	}
#endif
	/* Determine arch: 12 | 14 | 16 | 24 | 32 */
	if (pic_arch(execname) == 0)
		usage_pickle();
	if (argc < 2)
		usage(execname, "Missing arg(s)");

	/* Device selection */
	int argv1 = tolower((int)argv[1][0]);
	if (argv1 == 's') { /* Select device */
		if (argc < 3) {
			pic_selector();
			io_exit(EX_OK);
		}
		if (mystrcasestr(argv[2], "dspic") == argv[2]) {
			strncpy(p.devicename, argv[2], STRLEN);
		} else if (mystrcasestr(argv[2], "pic") == argv[2]) {
			strncpy(p.devicename, argv[2], STRLEN);
		} else {
			int32_t temp = strtol(argv[2], NULL, 0);
			if (temp < 10 || temp > 33) {
				usage(execname, "Invalid arg [select]");
			}
			if (temp == 30 || temp == 33) {
				strcpy(p.devicename, "dspic");
				strncpy(&p.devicename[5], argv[2], STRLEN - 5);
			} else {
				strcpy(p.devicename, "pic");
				strncpy(&p.devicename[3], argv[2], STRLEN - 3);
			}
		}
		argc -= 2;
		argv += 2;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	} else if (p.pic->arch == ARCH12BIT) {
		usage(execname, "Missing select");
	}

	/* Key entry */
	argv1 = tolower((int)argv[1][0]);
	if (argv1 == 'l') {			/* LVP 32-bit key entry */
		if (p.pic->arch == ARCH12BIT) {
			usage(execname, "Invalid arg [lvp]");
		}
		/* ARCH14BIT || ARCH16BIT || ARCH24BIT || ARCH32BIT */
		p.key = LVPKEY;
		argc -= 1;
		argv += 1;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	}
	else if (argv1 == 'h') {		/* HVP 32-bit key entry */
		if (p.pic->arch == ARCH12BIT || p.pic->arch == ARCH14BIT || p.pic->arch == ARCH32BIT) {
			usage(execname, "Invalid arg [hvp]");
		}
		/* ARCH16BIT || ARCH24BIT */
		p.key = HVPKEY;
		argc -= 1;
		argv += 1;
		if (argc < 2)
			usage(execname, "Missing arg(s)");
	}
	else if (p.pic->arch == ARCH32BIT) {	/* LVP 32-bit key entry */
		/* ARCH32BIT */
		p.key = LVPKEY;
	}
	else {					/* No key entry */
		/* ARCH12BIT || ARCH14BIT || ARCH16BIT || ARCH24BIT */
		p.key = NOKEY;
	}

	/* Command */
	argv1 = tolower((int)argv[1][0]);
	int argv11 = tolower((int)argv[1][1]);
	switch (argv1) {
	case 'b':	if (argv11 == 'o') {		/* BOOT */
				uint32_t addr = UINT32_MAX, words = UINT32_MAX;
				if (argc > 4)
					usage(execname, "Too many args [boot]");
				if (argc >= 3) {
					words = strtoul(argv[2], NULL, 0);
					if (words == 0)
						usage(execname, "Invalid arg [boot]");
				}
				if (argc == 4) {
					addr = strtoul(argv[3], NULL, 0);
				}
				pic_dumpboot(addr, words);
			} else {			/* BLANK */
				int config = 1;
				if (argc > 3)
					usage(execname, "Too many args [blank]");
				if (argc == 3) switch (argv[2][0]) {
					case 'n':
					case 'N':
					case '0': config = 0;
						break;
					case 'y':
					case 'Y':
					case '1': config = 1;
						break;
					default:usage(execname, "invalid arg [blank]");
						break;
				}
				if (areyousure("Blank device")) {
					pic_blank(config);
				}
			}
			break;

	case 'c':	if (argc > 3)
				usage(execname, "Too many args [config]");
			if (argc == 2)
				pic_dumpconfig();
			else
				pic_writebandgap(strtoul(argv[2], NULL, 0));
			break;
	
	case 'd':	if (argv11 == 'a') {		/* DATA */
				if (argc > 2)
					usage(execname, "Too many args [data]");
				pic_dumpdata();
			} else if (argv11 == 'e') { 	/* DEBUG */
				printf("Hello world!\n");
			} else {			/* DUMP */
				if (argc > 2)
					usage(execname, "Too many args [dump]");
				pic_dumpdevice();
			}
			break;

	case 'e':	if (argv11 == 'r') {		/* ERASE FLASH | ID | ROW[NROWS] */
				uint32_t row = 0, nrows = 1;
				char prompt[STRLEN] = {0}, *endptr = NULL;

				if (argc < 3)
					usage(execname, "Missing arg [erase]");
				if (argc > 4)
					usage(execname, "Too many args [erase]");
				
				int argv2 = tolower((int)argv[2][0]);
				switch (argv2) {
				case 'i': /* IDLOCATION    */
				case 'u': /* USERID/CONFIG */
					row = PIC_ERASE_ID;
					strncpy(prompt, "Erase id", STRLEN);
					break;
				case 'c': /* CONFIG */
					row = PIC_ERASE_CONFIG;
					strncpy(prompt, "Erase config", STRLEN);
					break;
				case 'e': /* EEPROM */
					row = PIC_ERASE_EEPROM;
					strncpy(prompt, "Erase EEPROM", STRLEN);
					break;
				case 'f': /* FLASH */
					nrows = UINT32_MAX;
					strncpy(prompt, "Erase program flash", STRLEN);
					break;
				default:  /* FLASH ROW */
					row = strtoul(argv[2], &endptr, 0);
					if (endptr == argv[2])
						usage(execname, "Invalid arg [erase]");
					if (argc == 4) {
						nrows = strtoul(argv[3], NULL, 0);
						if (nrows == 0)
							usage(execname, "Invalid arg [erase]");
					}
					snprintf(prompt, STRLEN, "Erase %u row(s) at row %u",
						nrows, row);
					break;
				}
				if (areyousure(prompt))
					pic_erase(row, nrows);
			} else if (argv11 == 'x') {	/* EXECUTIVE */
				uint32_t addr = UINT32_MAX, words = UINT32_MAX;
				if (argc > 4)
					usage(execname, "Too many args [executive]");
				if (argc >= 3) {
					words = strtoul(argv[2], NULL, 0);
					if (words == 0)
						usage(execname, "Invalid arg [executive]");
				}
				if (argc == 4) {
					addr = strtoul(argv[3], NULL, 0);
				}
				pic_dumpexec(addr, words);
			} else {			/* EEPROM */
				if (argc > 2)
					usage(execname, "Too many args [eeprom]");
				pic_dumpdata();
			}
			break;

	case 'f':	{
			uint32_t words = UINT32_MAX, addr = UINT32_MAX;
			if (argc > 4)
				usage(execname, "Too many args [program flash]");
			if (argc >= 3) {
				words = strtoul(argv[2], NULL, 0);
				if (words == 0)
					usage(execname, "Invalid arg [program flash]");
			}
			if (argc == 4) {
				addr = strtoul(argv[3], NULL, 0);
			}
			pic_dumpprogram(addr, words);
			}
			break;

	case 'i':	if (argc > 2)
				usage(execname, "Too many args [id]");
			pic_dumpdeviceid();
			break;

	case 'o':	if (argc > 3)
				usage(execname, "Too many args [osccal]");
			if (argc == 2)
				pic_dumposccal();
			else
				pic_writeosccal(strtoul(argv[2], NULL, 0));
			break;

	case 'p':	{
			int blank = 1;
			if (argc > 4)
				usage(execname, "Too many args [program]");
			if (argc == 4) switch (argv[3][0]) {
				case 'n':
				case 'N':
				case '0': blank = 0;
					break;
				case 'y':
				case 'Y':
				case '1': blank = 1;
					break;
				default:usage(execname, "Invalid arg [program]");
					break;
			}
			if (argc < 3)
				pic_program("-", 1);
			else
				pic_program(argv[2], blank);
			}
			break;

	case 'v':	if (argv11 == 'i') {		/* VIEW */
				if (argc > 3)
					usage(execname, "Too many args [view]");
				if (argc < 3)
					pic_view("-");
				else
					pic_view(argv[2]);
			} else {			/* VERIFY */
				if (argc > 3)
					usage(execname, "Too many args [verify]");
				if (argc < 3)
					pic_verify("-");
				else
					pic_verify(argv[2]);
			}
			break;
#ifdef TTY
	case '/':	if (strstr(argv[1], "/dev/tty") != argv[1]) {
				usage(execname, "Invalid device [TTY]");
			}
			if (strstr(argv[1], p.device) != NULL) {
				usage(execname, "Device in use [TTY]");
			}
			stk500v2_listen(argv[1], 0);
			break;

	case '8':	stk500v2_listen("0.0.0.0", 8048);
			break;
#endif
	default:	usage(execname, "Unknown operation");
			break;
	}

	free(execdup);
	io_exit(EX_OK);
}
