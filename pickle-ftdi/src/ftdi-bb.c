/*
 * Copyright (C) 2005-2016 Darron Broad
 * Copyright (C) 2016 Gerhard Bertelsmann
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

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include <ftdi.h>
#include "ftdi-bb.h"

extern struct pickle p;

/* File descriptor */
struct ftdi_context ftdi;
int ftdi_bb_fd = -1;

/* Buffer for the bit-banged data */
#define FTDI_BB_MAX_BITS_TRANSFER (64)
uint8_t ftdi_buffer[FTDI_BB_MAX_BITS_TRANSFER * 4];

/* I/O */
static uint8_t pin_latch = 0, pin_mask = 0xFF;

/* Configuration */
static uint8_t clock_pin, data_pin_input, data_pin_output, clock_falling, msb_first;

int
ftdi_bb_open(const char *device)
{
#ifdef __linux
	/* Initialize and find device */
	if (ftdi_init(&ftdi) < 0) {
		printf("%s: ftdi_init failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}

	if (p.usb_serial[0]) {
		if ((ftdi_usb_open_desc(&ftdi, 0x0403, 0x6015, NULL, p.usb_serial) < 0) && (ftdi_usb_open_desc(&ftdi, 0x0403, 0x6001, NULL, p.usb_serial) < 0)) {
			printf("%s: can't open FT232R/FT230X device [%s] with serial ID %s\n", __func__, ftdi_get_error_string(&ftdi), p.usb_serial);
			ftdi_bb_fd = -1;
			return -1;
		}
	} else {
		if ((ftdi_usb_open(&ftdi, 0x0403, 0x6015) < 0) && (ftdi_usb_open(&ftdi, 0x0403, 0x6001) < 0)) {
			printf("%s: can't open FT230X device [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			ftdi_bb_fd = -1;
			return -1;
		}
	}

	/* All output */
	pin_mask = 0xFF;

	if (ftdi_set_bitmode(&ftdi, pin_mask, BITMODE_SYNCBB) < 0) {
		printf("%s: can't enable bitbang mode [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}

	if (ftdi_set_baudrate(&ftdi, 65536) < 0) {
		printf("%s: can't set baudrate [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}

	return 1;
#else
	return -1;
#endif
}

void
ftdi_bb_close(void)
{
#ifdef __linux
	ftdi_disable_bitbang(&ftdi);
	ftdi_usb_reset(&ftdi);
	ftdi_usb_close(&ftdi);
#endif
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
	clock_pin = config->clock_pin;
	clock_falling = config->clock_falling;
	data_pin_input = config->data_pin_input;
	data_pin_output = config->data_pin_output;
	msb_first = config->msb_first;

	return 1;
#else
	return -1
#endif
}

int
ftdi_bb_io(struct ftdi_bb_io *io)
{
#ifdef __linux
	uint8_t old_mask = pin_mask, pin_read;

	if (io->dir) {	/* In */
		pin_mask &= ~(1 << io->pin);
	} else {	/* Out */
		pin_mask |= (1 << io->pin);
		if (io->bit)	/* Set */
			pin_latch |= (1 << io->pin);
		else		/* Reset */
			pin_latch &= ~(1 << io->pin);
	}

	if (pin_mask != old_mask) {
		if (ftdi_set_bitmode(&ftdi, pin_mask, BITMODE_SYNCBB) < 0) {
			printf("%s: ftdi_set_bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			return -1;
		}
	}

	if (ftdi_write_data(&ftdi, &pin_latch, 1) < 0) {
		printf("%s: ftdi_write_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}

	if (ftdi_read_data(&ftdi, &pin_read, 1) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}

	if (io->dir) /* In */
		io->bit = (pin_read & (1 << io->pin)) != 0;

	return 1;
#endif
	return -1;
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
#ifdef __linux
	uint32_t index = 0;
	uint8_t old_mask;
	uint64_t value, value_mask;

	if (data_pin_input == data_pin_output) {
		old_mask = pin_mask;
		if (shift->dir) /* In */
			pin_mask &= ~(1 << data_pin_input);
		else		/* Out */
			pin_mask |= (1 << data_pin_input);
		if (pin_mask != old_mask) {
			if (ftdi_set_bitmode(&ftdi, pin_mask, BITMODE_SYNCBB) < 0) {
				printf("%s: ftdi_set_bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			}
		}
	}

	bzero(ftdi_buffer, FTDI_BB_MAX_BITS_TRANSFER * 4);
	value = shift->bits;
	value_mask = (msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
	for (int i = 0; i < shift->nbits; ++i) {
		if (!shift->dir) {	/* Out */
			if (value & value_mask)	/* Set */
				pin_latch |= (1 << data_pin_output);
			else			/* Reset */
				pin_latch &= ~(1 << data_pin_output);
		}
		pin_latch |= (1 << clock_pin);
		ftdi_buffer[index++] = pin_latch;
		ftdi_buffer[index++] = pin_latch;
		pin_latch &= ~(1 << clock_pin);
		ftdi_buffer[index++] = pin_latch;
		ftdi_buffer[index++] = pin_latch;
		value_mask = (msb_first) ? (value_mask >> 1) : (value_mask << 1);
	}

	if ((ftdi_write_data(&ftdi, ftdi_buffer, index)) < 0) {
		printf("%s: ftdi_wrire_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}

	if ((ftdi_read_data(&ftdi, ftdi_buffer, index)) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}

	if (shift->dir) { /* In */
		value = 0;
		value_mask = (msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
		for (int i = 0; i < shift->nbits; ++i) {
			if (ftdi_buffer[i * 4 + 2] & (1 << data_pin_input))
				value |= value_mask;
			value_mask = (msb_first) ? (value_mask >> 1) : (value_mask << 1);
		}
		shift->bits = value;
	}

	return 1;
#else
	return -1;
#endif
}
