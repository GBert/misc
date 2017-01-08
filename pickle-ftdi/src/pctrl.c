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

#include "pctrl.h"

/*
 * pctrl session
 */
struct pickle p = {0};

void
usage(char *execname, char *msg)
{
	printf("USAGE: %s RUN|STOP|RESTORE\n", execname);
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

	/* Open device */
	if (io_open() < 0) {
		usage(execname, io_error());
	}

	/* Reset uid */
	if (getuid() != geteuid()) {
		if (setuid(getuid()) < 0) {
			printf("%s: fatal error: setuid failed\n", __func__);
			io_exit(EX_OSERR); /* Panic */
		}
	}

	/* Perform operation */
	if (argc < 2)
		usage(execname, "Missing arg");
	if (argc > 2)
		usage(execname, "Too many args");

	/* RUN/STOP RESTORE */
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
	} else {
		io_close(HIGH);
		printf("RUN [DEFAULT]\n");
	}

	io_exit(EX_OK);
}
