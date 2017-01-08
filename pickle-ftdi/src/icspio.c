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

extern int io_stop;

/******************************************************************************
 * ICSP I/O routine(s) for `pio'
 *****************************************************************************/

/*
 * Send an 8-bit byte
 */
int
icspio_out(int t1, int t2, uint8_t byte)
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
icspio_in(int t1, int t2, uint8_t *byte)
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
icspio_command(int t1, int t2, uint8_t *cmdarg, int cmdargc, uint32_t *res, int resw)
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
	icspio_out(t1, t2, cmdarg[0]);

	/* Get command ACK or NACK */
	err = icspio_in(t1, t2, &byte);
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
				icspio_out(t1, t2, cmdarg[i]);
				checksum += cmdarg[i];
			}
		}

		/* Get data byte(s) */
		if (resw > 0) {
			*res = 0;
			for (i = 0; !io_stop && i < resw; ++i) {
				err = icspio_in(t1, t2, &byte);
				if (err != ERRNONE) {
					return err;
				}
				*res = (*res << 8) | byte;
				checksum += byte;
			}
		}

		/* Get check-sum */
		err = icspio_in(t1, t2, &byte);
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
icspio_err(int err)
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

/******************************************************************************
 * ICSP I/O routine(s) for `ptest'
 *****************************************************************************/

/*
 * Read and output switches.
 */
#ifdef PTEST
int
icspio_switch(int t)
{
	int err;
	uint8_t cmd[1];

	uint32_t sw;
	static uint32_t lastsw = 0xFF;

	cmd[0] = CMD_SWITCH;
	err = icspio_command(t, t << 1, cmd, 0, &sw, 1);
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
icspio_lasterror(int t)
{
	int err;
	uint8_t cmd[1];

	uint32_t le;
	static uint32_t lastle = 0;

	cmd[0] = CMD_ERROR;
	err = icspio_command(t, t << 1, cmd, 0, &le, 1);
	if (err != ERRNONE) {
		return err;
	}
	if (!io_stop && le != lastle) {
		printf("%s: last error: %s [0x%02X]\n",
			__func__, icspio_err(le), le);
		lastle = le;
	}
	return ERRNONE;
}
#endif /* PTEST */
