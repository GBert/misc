/*
 * Copyright (C) 2005-2017 Darron Broad
 * Copyright (C) 2016-2017 Gerhard Bertelsmann
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

/*
 * Shadow output
 */
static uint8_t pin_latch = 0x00, pin_ddr = /* Magic */ 0xBB;

/*
 * FTDI handle
 */
static struct ftdi_context handle = {0};

/*
 * I/O buffer
 */
static uint8_t buffer[FTDI_BB_BUFFER_SIZE];

/*
 * Configuration
 */
static uint8_t clock_pin, data_pin_input, data_pin_output, msb_first;

#ifdef __linux
static int
ftdi_bb_bitmode(uint8_t new_ddr)
{
	if (pin_ddr != new_ddr) {
		int rc, retry = 5;

		pin_ddr = new_ddr;

		while ((rc = ftdi_set_bitmode(&handle, pin_ddr, BITMODE_SYNCBB)) < 0 && retry--)
			io_usleep(100000);

		if (rc < 0) {
			fprintf(stderr, "%s: ftdi_set_bimode failed [%s]\n", __func__,
				ftdi_get_error_string(&handle));
			return -1;
		}
	}

	return 0;
}
#endif

int
ftdi_bb_open(const char *usb_serial)
{
#ifdef __linux
	/* Initialize and find device */
	if (ftdi_init(&handle) < 0) {
		fprintf(stderr, "%s: ftdi_init failed [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (*usb_serial) {
		if ((ftdi_usb_open_desc(&handle, 0x0403, 0x6015, NULL, usb_serial) < 0) &&
			(ftdi_usb_open_desc(&handle, 0x0403, 0x6001, NULL, usb_serial) < 0)) {
			fprintf(stderr, "%s: can't open FT232R/FT230X device [%s] with serial ID %s\n",
				__func__, ftdi_get_error_string(&handle), usb_serial);
			return -1;
		}
	} else {
		if ((ftdi_usb_open(&handle, 0x0403, 0x6015) < 0) &&
			(ftdi_usb_open(&handle, 0x0403, 0x6001) < 0)) {
			fprintf(stderr, "%s: can't open FT230X device [%s]\n", __func__,
				ftdi_get_error_string(&handle));
			return -1;
		}
	}

	/* All input */
	if (ftdi_bb_bitmode(0x00) < 0)
		return -1;

	if (ftdi_set_baudrate(&handle, 65536) < 0) {
		fprintf(stderr, "%s: can't set baudrate [%s]\n", __func__,
			ftdi_get_error_string(&handle));
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
	ftdi_disable_bitbang(&handle);
	ftdi_usb_reset(&handle);
	ftdi_usb_close(&handle);
#endif
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
	clock_pin = config->clock_pin;
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
	uint8_t new_ddr = pin_ddr, pin_port;

	if (io->dir) {	/* In */
		new_ddr &= ~(1 << io->pin);
	} else {	/* Out */
		new_ddr |= (1 << io->pin);
		if (io->bit)	/* Set */
			pin_latch |= (1 << io->pin);
		else		/* Reset */
			pin_latch &= ~(1 << io->pin);
	}

	if (ftdi_bb_bitmode(new_ddr) < 0)
		return -1;

	if (ftdi_write_data(&handle, &pin_latch, 1) < 0) {
		fprintf(stderr, "%s: ftdi_write_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (ftdi_read_data(&handle, &pin_port, 1) < 0) {
		fprintf(stderr, "%s: ftdi_read_error [%s]\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (io->dir) /* In */
		io->bit = (pin_port & (1 << io->pin)) != 0;

	return 1;
#endif
	return -1;
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
#ifdef __linux
	uint32_t index = 0;
	uint64_t value, value_mask;
	int rc, retry;

	assert(shift->nbits > 0 && shift->nbits < 65);

	if (data_pin_input == data_pin_output) {
		uint8_t new_ddr = pin_ddr;

		if (shift->dir) /* In */
			new_ddr &= ~(1 << data_pin_input);
		else		/* Out */
			new_ddr |= (1 << data_pin_input);

		if (ftdi_bb_bitmode(new_ddr) < 0)
			return -1;
	}

	bzero(buffer, FTDI_BB_BUFFER_SIZE);
	value = shift->bits;
	value_mask = (msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
	for (int i = 0; i < shift->nbits; ++i) {
		if (!shift->dir) {	/* Out */
			if (value & value_mask)	/* Set */
				pin_latch |= (1 << data_pin_output);
			else			/* Reset */
				pin_latch &= ~(1 << data_pin_output);
		}
		buffer[index++] = pin_latch;
		pin_latch |= (1 << clock_pin);	/* CLOCK HIGH */
		buffer[index++] = pin_latch;
		buffer[index++] = pin_latch;
		pin_latch &= ~(1 << clock_pin);	/* CLOCK LOW */
		buffer[index++] = pin_latch;
		value_mask = (msb_first) ? (value_mask >> 1) : (value_mask << 1);
	}

	pin_latch &= ~(1 << clock_pin); /* CLOCK LOW */
	buffer[index++] = pin_latch;
	/* set data pin high */
	pin_latch |= (1 << data_pin_output);
	buffer[index++] = pin_latch;

	assert(index < FTDI_BB_BUFFER_SIZE);


	retry = 5;
	while ((rc = ftdi_write_data(&handle, buffer, index)) < 0 && retry--) {
		fprintf(stderr, "%s: ftdi_write_error [%s], retries left %d\n",
			__func__, ftdi_get_error_string(&handle), retry);
		io_usleep(100000);
	}
	if (rc < 0) {
		fprintf(stderr, "%s: ftdi_write_error [%s] abort\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	retry = 5;
	while ((rc = ftdi_read_data(&handle, buffer, index)) < 0 && retry--) {
		fprintf(stderr, "%s: ftdi_read_error [%s], retries left %d\n",
			__func__, ftdi_get_error_string(&handle), retry);
		io_usleep(100000);
	}
	if (rc < 0) {
		fprintf(stderr, "%s: ftdi_read_error [%s] abort\n", __func__,
			ftdi_get_error_string(&handle));
		return -1;
	}

	if (shift->dir) { /* In */
		value = 0;
		value_mask = (msb_first) ? (1U << (shift->nbits - 1)) : (1 << 0);
		for (int i = 0; i < shift->nbits; ++i) {
			if (buffer[i * 4 + 2] & (1 << data_pin_input))
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
