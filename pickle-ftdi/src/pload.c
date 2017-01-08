/*
 * Copyright (C) 2005-2017 Darron Broad
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

#include "pload.h"

/*
 * pload session
 */
struct pickle p = {0};

void
usage(char *execname, char *msg)
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

int
main(int argc, char *argv[])
{
	char *execdup = NULL, *execname = NULL;

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

	/* Perform operation */
	if (argc < 3)
		usage(execname, "Missing arg");
	if (argc > 5)
		usage(execname, "Too many args");

	/* Program or verify */
	int prog_mode = tolower((int)argv[1][0]);
	if (prog_mode != 'p' && prog_mode != 'v')
		usage(execname, "Invalid mode");

	/* Device or address */
	char *target = argv[2];

	/* File or stdin */
	char file[STRLEN];
	strcpy(file, "-");
	if (argc >= 4)
		strncpy(file, argv[3], STRMAX);

	/* Addressing */
	uint8_t size = 32;
	if (argc == 5) {
		size = (uint8_t)strtoul(argv[4], NULL, 0);
		if (size != 16 && size != 24 && size != 32)
			usage(execname, "Invalid size");
	}

	io_signal_on(); // !SIGPIPE
	stk500v2_load(prog_mode, target, file, size);

	io_exit(EX_OK);
}
