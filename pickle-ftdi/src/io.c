/*
 * Copyright (C) 2005-2017 Darron Broad
 * Copyright (C) 2015 Gerhard Bertelsmann
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
 * CTRL-C signal handler
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
#if defined(RPI) || defined(BITBANG) || defined(FTDI) || defined(BPI)
	/* rpi etc */
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
		if (gpio_rpi_open("/dev/mem", p.device[3]) < 0)
			return -1;
		p.iot = IORPI;
		io_signal_on();
		return 0;
	} 
#endif
#ifdef BPI
	if (mystrcasestr(p.device, "bpi") == p.device) {
		/* bpi */
		if (gpio_bpi_open("/dev/mem") < 0)
			return -1;
		p.iot = IOBPI;
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
#ifdef FTDI
	if (mystrcasestr(p.device, "ftdi") == p.device) {
		/* ftdi bit-bang */
		if (ftdi_bb_open(p.usb_serial) < 0)
			return -1;
		p.iot = IOFTDIBB;
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
			gpio_rpi_release(p.pgdo, alt);
		if (p.bitrules & PGC_RELEASE)
			gpio_rpi_release(p.pgc, alt);
		if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_PGM_DISABLED)
			gpio_rpi_release(p.pgm, alt);
		if (p.bitrules & VPP_RELEASE)
			gpio_rpi_release(p.vpp, alt);
		}
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		{
		uint8_t alt = (p.bitrules & ALT_RELEASE) != 0;

		if (p.bitrules & PGD_RELEASE)
			gpio_bpi_release(p.pgdo, alt);
		if (p.bitrules & PGC_RELEASE)
			gpio_bpi_release(p.pgc, alt);
		if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_PGM_DISABLED)
			gpio_bpi_release(p.pgm, alt);
		if (p.bitrules & VPP_RELEASE)
			gpio_bpi_release(p.vpp, alt);
		}
		break;
#endif
#ifdef BITBANG
	case IOBB:	/* gpio bit-bang */
		{
		struct gpio_bb_io io = {.dir = 1, .pin = 0, .bit = 0};

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
#ifdef FTDI
	case IOFTDIBB:	/* ftdi bit-bang */
		{
		struct ftdi_bb_io io = {.dir = 1, .pin = 0, .bit = 0};

		if (p.bitrules & PGD_RELEASE) {
			io.pin = p.pgdo;
			ftdi_bb_io(&io);
		}
		if (p.bitrules & PGC_RELEASE) {
			io.pin = p.pgc;
			ftdi_bb_io(&io);
		}
		if (p.bitrules & PGM_RELEASE && p.pgm != GPIO_PGM_DISABLED) {
			io.pin = p.pgm;
			ftdi_bb_io(&io);
		}
		if (p.bitrules & VPP_RELEASE) {
			io.pin = p.vpp;
			ftdi_bb_io(&io);
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
#if defined(RPI) || defined(MCP23017) || defined(BITBANG) || defined(FTDI) || defined(BPI)
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
		gpio_rpi_close();
		break;
#endif
#ifdef BPI
	case IOBPI:
		/* bpi */
		gpio_bpi_close();
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
#ifdef FTDI
	case IOFTDIBB:
		/* FTDI bit-bang */
		ftdi_bb_close();
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
#ifdef BPI
	case IOBPI:	/* bpi */
		msg = "Can't open BPI I/O";
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
#ifdef FTDI
	case IOFTDIBB:	/* FTDI bit-bang */
		msg = "Can't open FTDI bit-bang I/O";
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
			gpio_rpi_delay();
		}
		return;
	}
#endif
#ifdef BPI
	/* I/O sleep */
	if (p.iot == IOBPI && n < 10) {
		for (uint32_t i = 0; i < n; ++i) {
			gpio_bpi_delay();
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
			gpio_rpi_set(p.pgm, pgm);
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		if (p.pgm != GPIO_PGM_DISABLED)
			gpio_bpi_set(p.pgm, pgm);
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
			struct gpio_bb_io io = {.dir = 0, .pin = p.pgm, .bit = pgm};

			gpio_bb_io(&io);
		}
		break;
#endif
#ifdef FTDI
	case IOFTDIBB:	/* gpio bit-bang */
		if (p.pgm != GPIO_PGM_DISABLED) {
			struct ftdi_bb_io io = {.dir = 0, .pin = p.pgm, .bit = pgm};

			ftdi_bb_io(&io);
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
		gpio_rpi_set(p.vpp, vpp);
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		gpio_bpi_set(p.vpp, vpp);
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
		struct gpio_bb_io io = {.dir = 0, .pin = p.vpp, .bit = vpp};

		gpio_bb_io(&io);
		}
		break;
#endif
#ifdef FTDI
	case IOFTDIBB:	/* ftdi bit-bang */
		{
		struct ftdi_bb_io io = {.dir = 0, .pin = p.vpp, .bit = vpp};

		ftdi_bb_io(&io);
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
		gpio_rpi_set(p.pgdo, pgd);
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		gpio_bpi_set(p.pgdo, pgd);
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
		struct gpio_bb_io io = {.dir = 0, .pin = p.pgdo, .bit = pgd};

		gpio_bb_io(&io);
		}
		break;
#endif
#ifdef FTDI
	case IOFTDIBB:	/* FTDI bit-bang */
		{
		struct ftdi_bb_io io = {0, p.pgdo, pgd};

		ftdi_bb_io(&io);
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
		gpio_rpi_set(p.pgc, pgc);
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		gpio_bpi_set(p.pgc, pgc);
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
		struct gpio_bb_io io = {.dir = 0, .pin = p.pgc, .bit = pgc};

		gpio_bb_io(&io);
		}
		break;
#endif
#ifdef FTDI
	case IOFTDIBB:	/* ftdi bit-bang */
		{
		struct ftdi_bb_io io = {.dir = 0, .pin = p.pgc, .bit = pgc};

		ftdi_bb_io(&io);
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
		gpio_rpi_get(p.pgdi, &pgd);
		break;
#endif
#ifdef BPI
	case IOBPI:	/* bpi */
		gpio_bpi_get(p.pgdi, &pgd);
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
		struct gpio_bb_io io = {.dir = 1, .pin = p.pgdi, .bit = 0};

		gpio_bb_io(&io);
		pgd = io.bit;
		}
		break;
#endif
#ifdef FTDI
	case IOFTDIBB:	/* FTDI bit-bang */
		{
		struct ftdi_bb_io io = {.dir = 1, .pin = p.pgdi, .bit = 0};

		ftdi_bb_io(&io);
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
 * Configure chip I/O
 *
 * Clock falling is to satisfy LVP mode PIC24F devices which output data
 * on the falling edge of the clock and not on the rising edge.
 *
 * msb_first is to satisfy the next generation 8-bit chip programming.
 */
void
io_configure(uint8_t clock_falling, uint8_t msb_first)
{
	p.clock_falling = clock_falling;
	p.msb_first = msb_first;
#ifdef BITBANG
	if (p.iot == IOBB) {
		struct gpio_bb_config config;
		config.clock_pin = p.pgc;
		config.clock_falling = clock_falling;
		config.msb_first = msb_first;
		config.data_pin_input = p.pgdi;
		config.data_pin_output = p.pgdo;
		config.clock_delay_low = p.sleep_low;
		config.clock_delay_high = p.sleep_high;
		config.lock = (p.bitrules & BB_LOCK) ? 1 : 0;
		gpio_bb_configure(&config);
	}
#endif
#ifdef FTDI
	if (p.iot == IOFTDIBB) {
		struct ftdi_bb_config config;
		config.clock_pin = p.pgc;
		config.clock_falling = clock_falling;
		config.msb_first = msb_first;
		config.data_pin_input = p.pgdi;
		config.data_pin_output = p.pgdo;
		config.clock_delay_low = p.sleep_low;
		config.clock_delay_high = p.sleep_high;
		ftdi_bb_configure(&config);
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
#ifdef BPI
	if (p.iot == IOBPI && p.pgdi == p.pgdo)
		return;
#endif
#ifdef BITBANG
	if (p.iot == IOBB && p.pgdi == p.pgdo)
		return;
#endif
#ifdef FTDI
	if (p.iot == IOFTDIBB && p.pgdi == p.pgdo)
		return;
#endif
	/* Pull-up PGD output for PGD input (Eg. Velleman K8048) */
	io_set_pgd((p.bitrules & PGD_IN_PULLUP) ? (HIGH) : (LOW));
}

/*
 * Return first bit mask
 */
static inline
uint32_t io_first_mask(uint8_t nbits)
{
	return (p.msb_first) ? (1U << (nbits - 1)) : (1 << 0);
}

/*
 * Return next bit mask
 */
static inline
uint32_t io_next_mask(uint32_t mask)
{
	return (p.msb_first) ? (mask >> 1) : (mask << 1);
}

/*
 * Clock in bit(s)
 */
uint32_t
io_clock_in_bits(uint32_t ldly, uint32_t hdly, uint8_t nbits)
{
	uint32_t bits = 0, mask = io_first_mask(nbits);

	/* Initialise for data input */
	io_data_input();

	/* Clock in bits 0..N */
	while (nbits--) {
		if (!p.clock_falling)
			io_clock_bit(ldly, hdly);

		if (io_get_pgd())
			bits |= mask;

		if (p.clock_falling)
			io_clock_bit(ldly, hdly);

		mask = io_next_mask(mask);
	}
	return bits;
}

/*
 * Clock out bit(s)
 */
void
io_clock_out_bits(uint32_t ldly, uint32_t hdly, uint32_t bits, uint8_t nbits)
{
	uint32_t mask = io_first_mask(nbits);

	/* Clock out bits 0..N */
	while (nbits--) {
		io_set_pgd((bits & mask) != 0);
		io_clock_bit(ldly, hdly);

		mask = io_next_mask(mask);
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
#ifdef FTDI
	case IOFTDIBB:
		/* FTDI bit-bang */
		{
		struct ftdi_bb_shift shift = {1, nbits, 0};

		ftdi_bb_shift(&shift);
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
#ifdef FTDI
	case IOFTDIBB:
		/* FTDI bit-bang */
		{
		struct ftdi_bb_shift shift = {0, nbits, (uint64_t)bits};

		ftdi_bb_shift(&shift);
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
