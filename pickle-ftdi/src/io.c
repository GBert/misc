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

#include "pickle.h"

/******************************************************************************
 * Session
 *****************************************************************************/
extern struct pickle p;

/******************************************************************************
 * CTRL-C signal handlers
 *****************************************************************************/

int io_stop = 0;

void
io_signal()
{
	io_stop = 1;
}

void
io_signal_on()
{
	io_stop = 0;
	signal(SIGINT, io_signal);
	signal(SIGPIPE, SIG_IGN);
}

void
io_signal_off()
{
	signal(SIGINT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
}

/******************************************************************************
 * I/O
 *****************************************************************************/

/*
 * Configure defaults
 */
void
io_config(void)
{
	/* Reset */
	bzero(&p, sizeof(struct pickle));

	p.f = stderr;			/* Messages go here */

	p.iot = IONONE;
#ifdef TTY
	/* Velleman K8048/K8076 tty */
	strncpy(p.device, SERIAL_DEVICE, STRLEN);
	p.baudrate = 115200;		/* not speed_t      */
#endif
#if defined(RPI) || defined(BITBANG)
	/* rpi */
	p.vpp  = GPIO_VPP;		/* TX/!MCLR/VPP     */
	p.pgm  = GPIO_PGM_DISABLED;	/* PGM              */
	p.pgc  = GPIO_PGC;		/* RTS/PGC CLOCK    */
	p.pgdo = GPIO_PGDO;		/* DTR/PGD DATA_OUT */
	p.pgdi = GPIO_PGDI;		/* CTS/PGD DATA_IN  */
#endif
#ifdef MCP23017
	/* mcp23017 */
	p.mcp = MCP_ADDR;
#endif
	/* default:Velleman K8048 */
        p.bitrules = PGD_IN_PULLUP | PGD_OUT_FLIP | PGC_OUT_FLIP | VPP_OUT_FLIP | PGD_IN_FLIP;

	/* Mark/space time */
        p.sleep_high = 1;
        p.sleep_low = 1;

	/* ICSPIO mark/space time */
        p.fwsleep = 30;
}

/*
 * Open I/O
 */
int
io_open(void)
{
	if (p.iot != IONONE) {
		printf("%s: fatal error: device already open.\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
	}
#ifdef TTY
	if (strstr(p.device, "/dev/tty") == p.device) {
		/* tty */
		p.serial_port = serial_open(p.device, B0);
		if (p.serial_port < 0)
			return -1;
		p.iot = IOTTY;
		io_signal_on();
		return 0;
	} 
#endif
#ifdef RPI
	if (mystrcasestr(p.device, "rpi") == p.device) {
		/* rpi */
		if (gpio_open("/dev/mem", p.device[3]) < 0)
			return -1;
		p.iot = IORPI;
		io_signal_on();
		return 0;
	} 
#endif
#ifdef MCP23017
	if (strstr(p.device, "/dev/i2c") == p.device) {
		/* mcp23017 */
		if (mcp_open(p.device, p.mcp) < 0)
			return -1;
		p.iot = IOI2C;
		io_signal_on();
		return 0;
	} 
#endif
#ifdef BITBANG
	if (strstr(p.device, "/dev/gpio-bb") == p.device) {
		/* gpio bit-bang */
		if (gpio_bb_open("/dev/gpio-bb") < 0)
			return -1;
		p.iot = IOBB;
		io_signal_on();
		return 0;
	}
#endif
	return -1; /* Unsupported */
}

/*
 * Release GPIO
 */
void
io_release(void)
{
	switch (p.iot) {
#ifdef RPI
	case IORPI:	/* rpi */
		{
		uint8_t alt = (p.bitrules & ALT_RELEASE) != 0;

		if (p.bitrules & PGD_RELEASE)
			gpio_release(p.pgdo, alt);
		if (p.bitrules & PGC_RELEASE)
			gpio_release(p.pgc, alt);
		if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_PGM_DISABLED)
			gpio_release(p.pgm, alt);
		if (p.bitrules & VPP_RELEASE)
			gpio_release(p.vpp, alt);
		}
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {1, 0, 0};

		if (p.bitrules & PGD_RELEASE) {
			io.pin = p.pgdo;
			gpio_bb_io(&io);
		}
		if (p.bitrules & PGC_RELEASE) {
			io.pin = p.pgc;
			gpio_bb_io(&io);
		}
		if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_PGM_DISABLED) {
			io.pin = p.pgm;
			gpio_bb_io(&io);
		}
		if (p.bitrules & VPP_RELEASE) {
			io.pin = p.vpp;
			gpio_bb_io(&io);
		}
		}
		break;
#endif
	default:break;
	}
}

void
io_close(int run)
{
	if (p.iot == IONONE) {
		printf("%s: fatal error: device not open.\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
	}
#if defined(RPI) || defined(MCP23017) || defined(BITBANG)
	io_usleep(10);
	if (run) {
		io_set_vpp(HIGH);
	} else {
		io_set_vpp(LOW);
	}
	io_release();
#endif
	switch (p.iot) {
#ifdef TTY
	case IOTTY:
		/* tty */
		serial_close(p.serial_port);
		break;
#endif
#ifdef RPI
	case IORPI:
		/* rpi */
		gpio_close();
		break;
#endif
#ifdef MCP23017
	case IOI2C:
		/* mcp23017 */
		mcp_close();
		break;
#endif
#ifdef BITBANG
	case IOBB:
		/* gpio bit-bang */
		gpio_bb_close();
		break;
#endif
	default:break;
	}
	p.iot = IONONE;
	io_signal_off();
}

void
io_exit(int err)
{
	if (p.iot != IONONE) {
		io_close(p.bitrules & VPP_RUN);
	}
	exit(err);
}

char *
io_error(void)
{
	static char *msg;

	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty */
		msg = "Can't open serial I/O";
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		msg = "Can't open RPI I/O";
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		msg = "Can't open MCP23017 I2C I/O";
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		msg = "Can't open GPIO bit-bang I/O";
		break;
#endif
	default:msg = "Unsupported I/O";
		break;
	}
	return msg;
}

char *
io_fault(int errorcode)
{
	static char *msg;

	if (errorcode == 0)
		msg = "device not detected";
	else
		msg = "device not compatible";

	return msg;
}

void
io_usleep(uint32_t n)
{
	/* No sleep */
	if (n == 0) {
		return;
	}
#ifdef TTY
	/* I/O sleep */
	if (p.iot == IOTTY && n < 10) {
		for (uint32_t i = 0; i < n; ++i) {
			/* 1us ttyS, 3ms ttyUSB */
			get_cts(p.serial_port);
		}
		return;
	}
#endif
#ifdef RPI
	/* I/O sleep */
	if (p.iot == IORPI && n < 10) {
		for (uint32_t i = 0; i < n; ++i) {
			gpio_delay();
		}
		return;
	}
#endif
	/* Busy sleep */
	if (n < 100) {
		struct timeval tv1, tv2;
		int diff;
		gettimeofday(&tv1, NULL);
		do {
			gettimeofday(&tv2, NULL);
			diff = tv2.tv_usec - tv1.tv_usec;
			if (diff < 0)
				diff += 1000000;	
		} while (diff < n);
		return;
	}
	/* System sleep */
	usleep(n);
}

/******************************************************************************
 * BIT BANGING (nb. output is positive when feature is enabled)
 *****************************************************************************/

/*
 * Set PGM (LVP PROGRAM OUT)
 */
void
io_set_pgm(uint8_t pgm)
{
	/* BITRULES: K8048/tty = inverting, K8076/tty = non-inverting */
	if (p.bitrules & PGM_OUT_FLIP) {
		pgm = HIGH - pgm;
	}
	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty (PGM not supported) */
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		if (p.pgm != GPIO_PGM_DISABLED)
			gpio_set(p.pgm, pgm);
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		mcp_set_pgm(pgm);
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		if (p.pgm != GPIO_PGM_DISABLED) {
			struct gpio_bb_io io = {0, p.pgm, pgm};

			gpio_bb_io(&io);
		}
		break;
#endif
	default:printf("%s: fatal error: unsupported\n", __func__);
	       	io_exit(EX_SOFTWARE); /* Panic */
		break;
	}
}

/*
 * Set VPP (PROGRAM OUT)
 */
void
io_set_vpp(uint8_t vpp)
{
	/* BITRULES: K8048/tty = inverting, K8076/tty = non-inverting */
	if (p.bitrules & VPP_OUT_FLIP) {
		vpp = HIGH - vpp;
	}
	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty (pgm not supported) */
		if (p.bitrules & VPP_OUT_CLOCK) {
			static uint8_t level = FLOAT;
			if (vpp != level) {
				char b = 0;
				if (write(p.serial_port, &b, 1) < 0) {
					printf("%s: fatal error: write failed\n", __func__);
					io_exit(EX_OSERR); /* Panic */
				}
				level = vpp;
			}
		} else
			set_tx(p.serial_port, vpp);
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		gpio_set(p.vpp, vpp);
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		mcp_set_vpp(vpp);
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {0, p.vpp, vpp};

		gpio_bb_io(&io);
		}
		break;
#endif
	default:printf("%s: fatal error: unsupported\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
		break;
	}
}

/*
 * Set PGD (DATA OUT)
 */
void
io_set_pgd(uint8_t pgd)
{
	/* BITRULES: K8048/tty = inverting, K8076/tty = non-inverting */
	if (p.bitrules & PGD_OUT_FLIP) {
		pgd = HIGH - pgd;
	}
	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty */
		set_dtr(p.serial_port, pgd);
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		gpio_set(p.pgdo, pgd);
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		mcp_set_pgd(pgd);
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {0, p.pgdo, pgd};

		gpio_bb_io(&io);
		}
		break;
#endif
	default:printf("%s: fatal error: unsupported\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
		break;
	}
}

/*
 * Set PGC (CLOCK OUT)
 */
void
io_set_pgc(uint8_t pgc)
{
	/* BITRULES: K8048/tty = inverting, K8076/tty = non-inverting */
	if (p.bitrules & PGC_OUT_FLIP) {
		pgc = HIGH - pgc;
	}
	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty */
		set_rts(p.serial_port, pgc);
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		gpio_set(p.pgc, pgc);
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		mcp_set_pgc(pgc);
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {0, p.pgc, pgc};

		gpio_bb_io(&io);
		}
		break;
#endif
	default:printf("%s: fatal error: unsupported\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
		break;
	}
}

/*
 * Get PGD (DATA IN)
 */
uint8_t
io_get_pgd(void)
{
	uint8_t pgd = 0;

	switch (p.iot) {
#ifdef TTY
	case IOTTY:	/* tty */
		pgd = get_cts(p.serial_port);
		break;
#endif
#ifdef RPI
	case IORPI:	/* rpi */
		gpio_get(p.pgdi, &pgd);
		break;
#endif
#ifdef MCP23017
	case IOI2C:	/* mcp23017 */
		pgd = mcp_get_pgd();
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {1, p.pgdi, 0};

		gpio_bb_io(&io);
		pgd = io.bit;
		}
		break;
#endif
	default:printf("%s: fatal error: unsupported\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
		break;
	}
	/* BITRULES: K8048/tty = inverting, K8076/tty = non-inverting */
	if (p.bitrules & PGD_IN_FLIP) {
		pgd = HIGH - pgd;
	}
	return pgd;
}

/******************************************************************************
 * BIT BANGING FOR PROGRAM/VERIFY MODE, HARDWARE TESTS AND ICSPIO
 *****************************************************************************/

/*
 * Configure chip input
 *
 * Clock falling is to satisfy LVP mode PIC24F devices which output data
 * on the falling edge of the clock and not on the rising edge.
 */
void
io_configure(uint8_t clock_falling)
{
	p.clock_falling = clock_falling;
#ifdef BITBANG
	if (p.iot == IOBB) {
		struct gpio_bb_config config;
		config.clock_pin = p.pgc;
		config.clock_falling = clock_falling;
		config.data_pin_input = p.pgdi;
		config.data_pin_output = p.pgdo;
		config.clock_delay_low = p.sleep_low;
		config.clock_delay_high = p.sleep_high;
		config.lock = (p.bitrules & BB_LOCK) ? 1 : 0;
		gpio_bb_configure(&config);
	}
#endif
}

/*
 * Clock bit in or out
 */
static inline void
io_clock_bit(uint32_t ldly, uint32_t hdly)
{
	io_set_pgc(HIGH);		/* Clock high */
	io_usleep(hdly);

	io_set_pgc(LOW);		/* Clock low */
	io_usleep(ldly);
}

/*
 * Initialise data input from chip (PGD=IN, SDATA=OUTPUT)
 */
void
io_data_input(void)
{
#ifdef RPI
	if (p.iot == IORPI && p.pgdi == p.pgdo)
		return;
#endif
#ifdef BITBANG
	if (p.iot == IOBB && p.pgdi == p.pgdo)
		return;
#endif
	/* Pull-up PGD output for PGD input (Eg. Velleman K8048) */
	io_set_pgd((p.bitrules & PGD_IN_PULLUP) ? (HIGH) : (LOW));
}

/*
 * Clock in bit(s)
 */
uint32_t
io_clock_in_bits(uint32_t ldly, uint32_t hdly, uint8_t nbits)
{
	uint32_t bits = 0;

	/* Initialise for data input */
	io_data_input();

	/* Clock in bits 0..N */
	for (int i = 0; i < nbits; i++) {
		if (!p.clock_falling)
			io_clock_bit(ldly, hdly);
		bits |= io_get_pgd() << i;
		if (p.clock_falling)
			io_clock_bit(ldly, hdly);
	}
	return bits;
}

/*
 * Clock out bit(s)
 */
void
io_clock_out_bits(uint32_t ldly, uint32_t hdly, uint32_t bits, uint8_t nbits)
{
	/* Clock out bits 0..N */
	for (int i = 0; i < nbits; ++i) {
		io_set_pgd((bits >> i) & 1);
		io_clock_bit(ldly, hdly);
	}
}

/*
 * Clock 1 bit in and 2 bits out (2-wire 4-phase)
 */
#ifdef P32
uint8_t
io_clock_bit_4phase(uint8_t tms, uint8_t tdi)
{
	uint8_t tdo;

	/*********
	 * PHASE 1
	 */
	io_set_pgd(tdi);	/* TDI OUTPUT */
	io_clock_bit(p.sleep_low, p.sleep_high);

	/*********
	 * PHASE 2
	 */
	io_set_pgd(tms);	/* TMS OUTPUT */
	io_clock_bit(p.sleep_low, p.sleep_high);

	/*********
	 * PHASE 3
	 */
	io_data_input();
	io_get_pgd();		/* DISCARD INPUT */
	io_clock_bit(p.sleep_low, p.sleep_high);

	/*********
	 * PHASE 4
	 */
	tdo = io_get_pgd();	/* TDO INPUT (0 or 1) */
	io_clock_bit(p.sleep_low, p.sleep_high);

	return tdo;
}

/*
 * Clock multiple bits in and multiple bits out (2-wire 4-phase)
 */
uint32_t
io_clock_bits_4phase(uint8_t nbits, uint32_t tms, uint32_t tdi)
{
	uint32_t tdo = 0;

	/* FEEDBACK IN/OUT */
	if (p.busy)
		io_program_feedback('+');

	for (int i = 0; i < nbits; ++i)
		tdo |= io_clock_bit_4phase((tms >> i) & 1, (tdi >> i) & 1) << i;

	return tdo;
}
#endif /* P32 */

/******************************************************************************
 * BIT-BANG SHIFTING FOR PROGRAM/VERIFY MODE
 *****************************************************************************/

/*
 * Shift in 1..N bits for program/verify mode
 */
uint32_t
io_program_in(uint8_t nbits)
{
	uint32_t bits;
	
	/* FEEDBACK IN */
	if (p.busy)
		io_program_feedback('i');

	/* INPUT BITS */
	switch (p.iot) {
	default:/* tty */
		/* rpi */
		/* mcp23017 */
		bits = io_clock_in_bits(p.sleep_low, p.sleep_high, nbits);
		break;
#ifdef BITBANG
	case IOBB:
		/* gpio bit-bang */
		{
		struct gpio_bb_shift shift = {1, nbits, 0};

		gpio_bb_shift(&shift);
		bits = (uint32_t)shift.bits;
		}
		break;
#endif
	}
	return bits;
}

/*
 * Shift out 1..N bits for program/verify mode
 */
void
io_program_out(uint32_t bits, uint8_t nbits)
{
	/* FEEDBACK OUT */
	if (p.busy)
		io_program_feedback('o');

	/* OUTPUT BITS */
	switch (p.iot) {
	default:/* tty */
		/* rpi */
		/* mcp23017 */
		io_clock_out_bits(p.sleep_low, p.sleep_high, bits, nbits);
		break;
#ifdef BITBANG
	case IOBB:
		/* gpio bit-bang */
		{
		struct gpio_bb_shift shift = {0, nbits, (uint64_t)bits};

		gpio_bb_shift(&shift);
		}
		break;
#endif
	}
}

/*
 * Visual feedback for program/verify mode
 */
void
io_program_feedback(char c)
{
	static int c_count = 0, a_count = 0;
	static const char arrow[] = "|/-\\";
	static char c_last = 0;

	if ((c_last != c) || (c_count++ > p.busy)) {
		fprintf(stderr, "%c[%c]\r", arrow[a_count++ % strlen(arrow)], c);
		c_last = c;
		c_count = 0;
	}
}

/******************************************************************************
 * Hardware test routines for `ktest'
 *****************************************************************************/

/*
 * Test VPP, PGC or PGD
 */
#ifdef PTEST
void
io_test0(int pin, int t)
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
 * Test D-SUB-9
 */
void
io_test1(int t)
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
io_test2(int t)
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
io_test3(int t)
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
io_test4(int t)
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
#endif /* PTEST */

/******************************************************************************
 * Software test routines for `pio' (ICSP I/O)
 *****************************************************************************/

#ifdef PIO
/*
 * Send an 8-bit byte
 */
int
io_test_out(int t1, int t2, uint8_t byte)
{
	int i, bit;
	int parity = 0; /* 0=even 1=odd */

	if (p.debug >= 10) {
		fprintf(stderr, "%s(t1=%d, t2=%d, byte=0x%02X)\n",
			__func__, t1, t2, byte);
	}

	/* Start bit */
	io_clock_out_bits(t1, t1, LOW, 1);

	/* Send 8-bit byte */
	for (i = 0; !io_stop && i < 8; ++i) {
		bit = (byte >> i) & 1;
		parity ^= bit;
		io_clock_out_bits(t1, t1, bit, 1);
	}

	/* Send parity bit */	
	io_clock_out_bits(t1, t1, parity, 1);

	/* Send stop bit */
	io_clock_out_bits(t1, t1, HIGH, 1);

	/* Firmware delay */
	io_usleep(t2);
	
	return ERRNONE;
}

/*
 * Get an 8-bit byte
 */
int
io_test_in(int t1, int t2, uint8_t *byte)
{
	int i, bit;
	int parity = 0; /* 0=even 1=odd */
	
	*byte = '\0';

	if (p.debug >= 10) {
		fprintf(stderr, "%s(t1=%d, t2=%d, %p)\n",
			__func__, t1, t2, byte);
	}

	/* Get start bit */
	if (io_clock_in_bits(t1, t1, 1) != LOW) {
		if (p.debug >= 10)
			fprintf(stderr, "%s: INVALID START BIT (NOT LOW)\n", __func__);
		return ERRPROTOCOL;
	}

	/* Get 8-bit byte */
	for (i = 0; !io_stop && i < 8; i++) {
		bit = io_clock_in_bits(t1, t1, 1);
		parity ^= bit;
		*byte |= (bit << i);
	}

	/* Get parity bit */	
	bit = io_clock_in_bits(t1, t1, 1);
	if (bit != parity) {
		if (p.debug >= 10)
			fprintf(stderr, "%s: INVALID PARITY BIT\n", __func__);
		return ERRPARITY;
	}

	/* Get stop bit */
	if (io_clock_in_bits(t1, t1, 1) != HIGH) {
		if (p.debug >= 10)
			fprintf(stderr, "%s: INVALID STOP BIT (NOT HIGH)\n", __func__);
		return ERRPROTOCOL;
	}

	/* Firmware processing delay */
	io_usleep(t2);

	if (p.debug >= 10)
		fprintf(stderr, "%s: OKAY [0x%02X]\n", __func__, *byte);
	return ERRNONE;
}

/*
 * Send command and arg(s), get result
 */
int
io_test_command(int t1, int t2, uint8_t *cmdarg, int cmdargc, uint32_t *res, int resw)
{
	int i, err;
	uint8_t byte;

	if (p.debug >= 10)
		fprintf(stderr, "%s(t1=%d, t2=%d, cmdarg[0]=%02X, cmdargc=%d, %p, %d)\n", __func__,
			t1, t2, cmdarg[0], cmdargc, (void *)res, resw);

	if (cmdargc < 0) {
		printf("%s: fatal error: invalid length: %d\n", __func__, cmdargc);
		io_exit(EX_SOFTWARE); /* Panic */
	}
	
	if (resw < 0 || resw > 4) {
		printf("%s: fatal error: invalid width: %d\n", __func__, resw);
		io_exit(EX_SOFTWARE); /* Panic */
	}

	if (resw > 0 && res == NULL) {
		printf("%s: fatal error: invalid result pointer: (NULL)\n", __func__);
		io_exit(EX_SOFTWARE); /* Panic */
	}

	/* Send command */
	io_test_out(t1, t2, cmdarg[0]);

	/* Get command ACK or NACK */
	err = io_test_in(t1, t2, &byte);
	if (err != ERRNONE) {
		return err;
	}
	if (byte != ACK) {
		return ERRNOTSUP;
	}

	/* Send arg(s), get data byte(s) */
	if (cmdargc > 0 || resw > 0)
	{
		uint8_t checksum = cmdarg[0] + byte;

		/* Send arg(s) */
		if (cmdargc > 0) {
			for (i = 1; !io_stop && i <= cmdargc; ++i) {
				io_test_out(t1, t2, cmdarg[i]);
				checksum += cmdarg[i];
			}
		}

		/* Get data byte(s) */
		if (resw > 0) {
			*res = 0;
			for (i = 0; !io_stop && i < resw; ++i) {
				err = io_test_in(t1, t2, &byte);
				if (err != ERRNONE) {
					return err;
				}
				*res = (*res << 8) | byte;
				checksum += byte;
			}
		}

		/* Get check-sum */
		err = io_test_in(t1, t2, &byte);
		if (err != ERRNONE) {
			return err;
		}
		if (byte != checksum) {
			if (p.debug >= 10)
				fprintf(stderr, "%s: INVALID CHECKSUM 0x%02X != 0x%02X\n",
					__func__, byte, checksum);
			return ERRINVALID;
		}
	}

	return ERRNONE; /* OK */
}

/*
 * Get error string
 */
char *
io_test_err(int err)
{
	static char *errmsg;

	switch (err) {
	case ERRNONE:
		errmsg = "ERRNONE";
		break;
	case ERRTIMEOUT:
		errmsg = "ERRTIMEOUT";
		break;
	case ERRPROTOCOL:
		errmsg = "ERRPROTOCOL";
		break;
	case ERRPARITY:
		errmsg = "ERRPARITY";
		break;
	case ERRNOTSUP:
		errmsg = "ERRNOTSUP";
		break;
	case ERRINVALID:
		errmsg = "ERRINVALID";
		break;
	default:errmsg = "ERRUNKNOWN";
		break;
	}

	return errmsg;
}
#endif /* PIO */

/******************************************************************************
 * Software test routines for `ktest'
 *****************************************************************************/

#if defined(PTEST) && defined(PIO)
/*
 * Read and output switches.
 */
int
io_test_switch(int t)
{
	int err;
	uint8_t cmd[1];

	uint32_t sw;
	static uint32_t lastsw = 0xFF;

	cmd[0] = CMD_SWITCH;
	err = io_test_command(t, t << 1, cmd, 0, &sw, 1);
	if (err != ERRNONE) {
		return err;
	}
	if (!io_stop && sw != lastsw) {
		printf("%s: SW=0x%02X SW1=%d SW2=%d SW3=%d SW4=%d\n",
			__func__, sw,
			(sw & 0x01) >> 0, (sw & 0x02) >> 1,
			(sw & 0x04) >> 2, (sw & 0x08) >> 3);
		lastsw = sw;
	}
	return ERRNONE;
}

/*
 * Read and output last error.
 */
int
io_test_lasterror(int t)
{
	int err;
	uint8_t cmd[1];

	uint32_t le;
	static uint32_t lastle = 0;

	cmd[0] = CMD_ERROR;
	err = io_test_command(t, t << 1, cmd, 0, &le, 1);
	if (err != ERRNONE) {
		return err;
	}
	if (!io_stop && le != lastle) {
		printf("%s: last error: %s [0x%02X]\n",
			__func__, io_test_err(le), le);
		lastle = le;
	}
	return ERRNONE;
}

/*
 * Test LEDs and switches
 */
void
io_test5(int t)
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
		err = io_test_command(t, t << 1, cmd, 1, NULL, 0);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, io_test_err(err), err);
			break;
		}
		err = io_test_switch(t);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, io_test_err(err), err);
			break;
		}
		err = io_test_lasterror(t);
		if (err != ERRNONE) {
			fprintf(stderr, "%s: error: %s [0x%02X]\n",
				__func__, io_test_err(err), err);
			break;
		}
	}

	printf("\nTEST DONE\n\n");
}
#endif /* PTEST && PIO */
