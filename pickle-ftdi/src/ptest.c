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

#include "ptest.h"

/*
 * ptest session
 */
struct pickle p = {0};

extern int io_stop;

void
usage(char *execname, char *msg)
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
		"\t\tVPP, PGC, PGD or PGM LOW->HIGH->LOW output test with 5 seconds mark time.\n"
		" ptest 0 10\n"
		"\t\tPGD input test with 10 iterations of 1 second per step.\n"
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
#ifdef PIO
		" ptest 5 100\n"
		"\t\tICSPIO test with 100 microseconds clock mark time.\n"
#endif
#ifdef RPI
		" ptest 6 10\n"
		"\t\tRPi GPIO test with 10 seconds mark time.\n"
#endif
#ifdef BPI
		" ptest 7 10\n"
		"\t\tBPi GPIO test with 10 seconds mark time.\n"
#endif
		"\n");

	printf("VERSION:\n %s\n", VERSION);

	if (msg)
		io_exit(EX_USAGE);
	io_exit(EX_OK);
}

/******************************************************************************
 * Hardware test routines for `ptest'
 *****************************************************************************/

/*
 * Test VPP, PGC or PGD output
 */
void
test_pin(int pin, int t)
{
	printf("\nTEST MODE VPP|PGD|PGC|PGM\n\n");

	switch (pin) {
	case 0: printf("VPP LOW  (ICSP 1) (D-SUB-9 TX 3) [3 seconds]\n");
		io_set_vpp(LOW);
		break;
	case 1: printf("PGC LOW  (ICSP 5) (D-SUB-9 RTS 7) [3 seconds]\n");
		io_set_pgc(LOW);
		break;
	case 2: printf("PGD LOW  (ICSP 4) (D-SUB-9 DTR 3) [3 seconds]\n");
		io_set_pgd(LOW);
		break;
	case 3: printf("PGM LOW  [3 seconds]\n");
		io_set_pgm(LOW);
		break;
	}
	if (!io_stop)
		sleep(3);

	switch (pin) {
	case 0: printf("VPP HIGH (ICSP 1) (D-SUB-9 TX 3) [%d seconds]\n", t);
		io_set_vpp(HIGH);
		break;
	case 1: printf("PGC HIGH (ICSP 5) (D-SUB-9 RTS 7) [%d seconds]\n", t);
		io_set_pgc(HIGH);
		break;
	case 2: printf("PGD HIGH (ICSP 4) (D-SUB-9 DTR 3) [%d seconds]\n", t);
		io_set_pgd(HIGH);
		break;
	case 3: printf("PGM HIGH [%d seconds]\n", t);
		io_set_pgm(HIGH);
		break;
	}
	if (!io_stop)
		sleep(t);

	switch (pin) {
	case 0: printf("VPP LOW  (ICSP 1) (D-SUB-9 TX 3) [3 seconds]\n");
		io_set_vpp(LOW);
		break;
	case 1: printf("PGC LOW  (ICSP 5) (D-SUB-9 RTS 7) [3 seconds]\n");
		io_set_pgc(LOW);
		break;
	case 2: printf("PGD LOW  (ICSP 4) (D-SUB-9 DTR 3) [3 seconds]\n");
		io_set_pgd(LOW);
		break;
	case 3: printf("PGM LOW  [3 seconds]\n");
		io_set_pgm(LOW);
		break;
	}
	if (!io_stop)
		sleep(3);
	
	printf("\nTEST DONE\n\n");
}

/*
 * Test PGD input
 */
void
test_in(int t)
{
	printf("\nTEST PGD INPUT\n\n");
	
	while (t-- && !io_stop) {
		printf("PGD IN: %d\n", io_get_pgd());
		sleep(1);
	}

	printf("\nTEST DONE\n\n");
}

/*
 * Test D-SUB-9
 */
void
test_dsub9(int t)
{
	printf("\nTEST MODE 1 [D-SUB-9]\n\n");

	p.bitrules = 0; /* Disable BITRULES */

	printf("Tx  SET  (+VE) (D-SUB-9 3) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp(HIGH);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n", io_get_pgd());

	printf("Tx  CLR  (-VE) (D-SUB-9 3) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp(LOW);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd());

	printf("DTR SET  (+VE) (D-SUB-9 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(HIGH);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n", io_get_pgd());

	printf("DTR CLR  (-VE) (D-SUB-9 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(LOW);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd());
	
	printf("RTS SET  (+VE) (D-SUB-9 7) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(HIGH);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n", io_get_pgd());

	printf("RTS CLR  (-VE) (D-SUB-9 7) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(LOW);
	if (!io_stop)
		sleep(t);
	printf("CTS IN: %d\n\n", io_get_pgd());
	
	printf("TEST DONE\n\n");
}

/*
 * Test ICSP
 */
void
test_icsp(int t)
{
	printf("\nTEST MODE 2 [ICSP]\n\n");

	printf("VPP LOW  (0V)  (ICSP 1) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp(LOW);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n", io_get_pgd());

	printf("VPP HIGH (12V) (ICSP 1) [%d seconds] ", t);
	fflush(stdout);
	io_set_vpp(HIGH);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd());

	printf("PGD LOW  (0V)  (ICSP 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(LOW);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n", io_get_pgd());

	printf("PGD HIGH (5V)  (ICSP 4) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgd(HIGH);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd());

	printf("PGC LOW  (0V)  (ICSP 5) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(LOW);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n", io_get_pgd());

	printf("PGC HIGH (5V)  (ICSP 5) [%d seconds] ", t);
	fflush(stdout);
	io_set_pgc(HIGH);
	if (!io_stop)
		sleep(t);
	printf("PGD IN: %d\n\n", io_get_pgd());

	printf("TEST DONE\n\n");
}

/*
 * Test D-SUB-9 RTS 7 (PGC) DTR 4 (PGD)
 */
void
test_toggle(int t)
{
	printf("\nTEST MODE 3 [D-SUB-9 RTS 7 (PGC) DTR 4 (PGD)] CTRL-C TO STOP\n\n");

	while (!io_stop) {
		io_set_pgd(0);
		io_set_pgc(1);
		io_usleep(t);
		io_set_pgd(1);
		io_set_pgc(0);
		io_usleep(t);
	}

	printf("\nTEST DONE\n\n");
}

/*
 * Test PIC16F627 on Velleman K8048 (PIC16F628A is compatible)
 *
 * Program:
 *	p14 program pickles/examples/pic16f627/debug.hex
 *
 * Note: This is the PICMicro which came with the Velleman K8048 kit.
 */
void
test_debug(int t)
{
	int i, j = 0;
	uint8_t c, line[STRLEN];

	printf("\nTEST MODE 4 [16F627 debug.asm] CTRL-C TO STOP\n\n");

	/* Initialise for data input */
	io_data_input();

	while (!io_stop) {
		/* Handshake */
		io_set_pgc(LOW);			/* PGC LOW */

		while (io_get_pgd() != HIGH && !io_stop)/* WAIT FOR PGD HIGH */
			io_usleep(1);
		
		io_set_pgc(HIGH);			/* PGC HIGH */
		
		while (io_get_pgd() != LOW && !io_stop)/* WAIT FOR PGD LOW */
			io_usleep(1);

		/* Byte input */
		c = 0;
		for (i = 0; i < 8; i++) {
			io_set_pgc(LOW);		/* PGC LOW */
			io_usleep(t);

			c = c >> 1;			/* CLOCK BIT IN */
			if (io_get_pgd())
				c |= 0x80;

			io_set_pgc(HIGH);		/* PGC HIGH */
			io_usleep(t);
		}

		printf("%s: read byte: %02X %c\n", __func__, c, PIC_CHAR(c));

		if (c == 0x03) {			/* ETX */
			if (j > 1 && line[0] == 0x02) {	/* STX */
				line[j] = '\0';
				printf("%s: read line: %s", __func__,
					&line[1]);
			}
			j = 0;
		}
		else if (j < STRMAX)
			line[j++] = c;
	}

	printf("\nTEST DONE\n\n");
}

/*
 * Test ICSP I/O LEDs and switches
 */
#ifdef PIO
void
test_icspio(int t)
{
	int err;
	uint8_t cmd[2];
	uint32_t ld = 0;

	printf("\nTEST MODE 5 [ICSPIO] CTRL-C TO STOP\n\n");

	/* VPP LOW */
	io_set_vpp(LOW);
	io_usleep(10000); /* 10ms */

	/* VPP HIGH */
	io_set_vpp(HIGH);
	io_usleep(10000); /* 10ms */

	while (!io_stop) {
		cmd[0] = CMD_LED;
		cmd[1] = ld++;
		err = icspio_command(t, t << 1, cmd, 1, NULL, 0);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, icspio_err(err), err);
			break;
		}
		err = icspio_switch(t);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, icspio_err(err), err);
			break;
		}
		err = icspio_lasterror(t);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, icspio_err(err), err);
			break;
		}
	}

	printf("\nTEST DONE\n\n");
}
#endif /* PIO */

/*
 * Test RPi GPIO pins
 */
#ifdef RPI
void
test_rpi(int seconds)
{
	uint8_t output_level = 0, input_level;

	printf("\nTEST MODE 6 [RPI GPIO] CTRL-C TO STOP\n");

	while (!io_stop) {
		printf("\n");

		gpio_rpi_set(p.vpp, output_level);
		printf("GPIO %-3d (VPP) (TX)  = %d\n", p.vpp, output_level);

		if (p.pgm != GPIO_PGM_DISABLED) {
			gpio_rpi_set(p.pgm, output_level);
			printf("GPIO %-3d (PGM)       = %d\n", p.pgm, output_level);
		}

		gpio_rpi_set(p.pgc, output_level);
		printf("GPIO %-3d (PGC) (RTS) = %d\n", p.pgc, output_level);

		gpio_rpi_set(p.pgdo, output_level);
		printf("GPIO %-3d (PGD) (DTR) = %d\n", p.pgdo,output_level);

		if (p.pgdi != p.pgdo) {
			gpio_rpi_get(p.pgdi, &input_level);
			printf("GPIO %02d (PGD) (CTS) = %d\n", p.pgdi, input_level);
		}

		fflush(stdout);
		
		sleep(seconds);
		output_level = 1 - output_level;
	}
}
#endif /* RPI */

/*
 * Test BPi GPIO pins
 */
#ifdef BPI
void
test_bpi(int seconds)
{
	uint8_t output_level = 0, input_level;

	printf("\nTEST MODE 7 [BPI GPIO] CTRL-C TO STOP\n");

	while (!io_stop) {
		printf("\n");

		gpio_bpi_set(p.vpp, output_level);
		printf("GPIO %-3d (VPP) = %d\n", p.vpp, output_level);

		if (p.pgm != GPIO_PGM_DISABLED) {
		        gpio_bpi_set(p.pgm, output_level);
		        printf("GPIO %-3d (PGM) = %d\n", p.pgm, output_level);
		}

		gpio_bpi_set(p.pgc, output_level);
		printf("GPIO %-3d (PGC) = %d\n", p.pgc, output_level);

		gpio_bpi_set(p.pgdo, output_level);
		printf("GPIO %-3d (PGD) = %d\n", p.pgdo,output_level);

		if (p.pgdi != p.pgdo) {
		        gpio_bpi_get(p.pgdi, &input_level);
		        printf("GPIO %02d (PGD) = %d\n", p.pgdi, input_level);
		}

		fflush(stdout);

		sleep(seconds);
		output_level = 1 - output_level;
	}
}
#endif /* BPI */

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
	if (argc < 3)
		usage(execname, "Missing args");
	if (argc > 3)
		usage(execname, "Too many args");

	int32_t testarg = strtol(argv[2], NULL, 0);
	if (testarg < 0)
		usage(execname, "Invalid arg");

	if (strcasecmp(argv[1], "VPP") == 0) 
		test_pin(0, testarg);
	else if (strcasecmp(argv[1], "PGC") == 0)
		test_pin(1, testarg);
	else if (strcasecmp(argv[1], "PGD") == 0)
		test_pin(2, testarg);
	else if (strcasecmp(argv[1], "PGM") == 0)
		test_pin(3, testarg);
	else if (argv[1][0] >= '0' && argv[1][0] <= '9') {
		int32_t test = strtol(argv[1], NULL, 0);
		switch (test) {
		case 0: test_in(testarg);
			break;
		case 1: test_dsub9(testarg);
			break;
		case 2: test_icsp(testarg);
			break;
		case 3: test_toggle(testarg);
			break;
		case 4: test_debug(testarg);
			break;
#ifdef PIO
		case 5: test_icspio(testarg);
			break;
#endif
#ifdef RPI
		case 6: test_rpi(testarg);
			break;
#endif
#ifdef BPI
		case 7: test_bpi(testarg);
			break;
#endif
		default:usage(execname, "Invalid arg");
			break;
		}
	} else {
		usage(execname, "Invalid arg");
	}
	io_exit(EX_OK);
}
