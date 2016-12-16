/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 *
 *  all errros designed by Gerhard Bertelsmann
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

/*
 * File descriptor
 */
struct ftdi_context ftdi;
int ftdi_bb_fd = -1;

#define MAX_BITS_TRANSFER	128

/* buffer for the bitbanged data */
uint8_t ftdi_buf_out[MAX_BITS_TRANSFER * 4];
uint8_t ftdi_buf_in[MAX_BITS_TRANSFER * 4];

static uint16_t pin_state = 0;
static uint8_t clock_pin, data_pin_input, data_pin_output, clock_falling;
static uint8_t actual_mask;

void
print_buffer(uint8_t *buffer, int len)
{
	for (int i = 0; i < len; i++) {
		printf("0x%02x ", buffer[i]);
		if ( ((i+1) % 16 ) == 0)
			printf("\n");
	}
}

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

	if (strlen(p.usb_serial)) {
		if ((ftdi_usb_open_desc(&ftdi, 0x0403, 0x6015, NULL, p.usb_serial) < 0) && (ftdi_usb_open_desc(&ftdi, 0x0403, 0x6001, NULL, p.usb_serial) < 0)) {
			if (p.usb_serial)
				printf("%s: can't open FT232R/FT230X device [%s] with serial ID %s\n", __func__, ftdi_get_error_string(&ftdi), p.usb_serial);
			else
				printf("%s: can't open FT232R/FT230X device [%s]\n", __func__, ftdi_get_error_string(&ftdi));
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

	/* all output */
	actual_mask = 0xff;
	if (ftdi_set_bitmode(&ftdi, actual_mask, BITMODE_SYNCBB) < 0) {
		printf("%s: can't enable bitbang mode [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}
	/* TODO: set baudrate (needed) - defines the bitbang speed (formula ?) */
	if(ftdi_set_baudrate(&ftdi, 1024) < 0) {
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
ftdi_bb_io(struct ftdi_bb_io *io)
{
#ifdef __linux
	uint8_t value;
	/* TODO: change mask if needed */
	io->mask = actual_mask;
	if (io->dir == 0) {
		io->mask  |=  (1 << io->pin);
		if (io->bit == 1)
			pin_state |=  (1 << io->pin);
		else
			pin_state &= ~(1 << io->pin);
	} else {
		io->mask  &= ~(1 << io->pin);
        }
		
	if ( io->mask != actual_mask ) {
		if (ftdi_set_bitmode(&ftdi, io->mask, BITMODE_SYNCBB) < 0) {
			printf("%s: ftdi_set_bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			return -1;
		}
	}
	actual_mask = io->mask;
	value = pin_state & 0xff;
	if (ftdi_write_data(&ftdi, &value, 1) < 0) {
		printf("%s: ftdi_write_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
	if (ftdi_read_data(&ftdi, &value, 1) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
	return 1;
#endif
	return -1;
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
	clock_pin = config->clock_pin;
	clock_falling = config->clock_falling;
	data_pin_input = config->data_pin_input;
	data_pin_output = config->data_pin_output;

	return 1;
#else
	return -1
#endif
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
#ifdef __linux
	uint32_t index = 0;
	uint8_t mask;
	uint64_t value;
	int ret;
	value = shift->bits;

	if (data_pin_input == data_pin_output) {
		mask = actual_mask;
		if (shift->dir)
			mask &= ~(1 << data_pin_input);
		else
			mask |= ( 1 << data_pin_input);
		if (mask != actual_mask) {
			if (ftdi_set_bitmode(&ftdi, mask, BITMODE_SYNCBB) < 0) {
				printf("%s: ftdi set bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			}
			actual_mask = mask;
		}
	}

	/* prepare buffer - simple delete for now (maybe wrong if MCLR or PGM set) */
	bzero(ftdi_buf_out, MAX_BITS_TRANSFER * 4);

	for (int i = 0; i< shift->nbits; i++) {
		ftdi_buf_out[index] = pin_state;
		if (value & 1UL)
			ftdi_buf_out[index] |= 1 << data_pin_output | ( 1 << clock_pin) ;
                else
			ftdi_buf_out[index] |= ( 1 << clock_pin) ;

		index++;
		ftdi_buf_out[index] = ftdi_buf_out[index-1] & ~( 1 << clock_pin);
		index++;
		value = value >> 1;
	}

	/* if last bit is high - add down bit */
	if (shift->dir == 0) {
		ftdi_buf_out[index] = ftdi_buf_out[index-1] & ~( 1 << clock_pin) & ~(1 << data_pin_output);
		index++;
	}

	if ((ret = ftdi_write_data(&ftdi, ftdi_buf_out, index )) < 0) {
		printf("%s: ftdi_wrire_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
	if ((ret = ftdi_read_data(&ftdi, ftdi_buf_in, index )) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
	value = 0;
	int mask_value = 1;
	if (shift->dir) {
		for (int i = 0; i < shift->nbits; i++ ) {
		if (ftdi_buf_in[i*2 + 1] & (1 << data_pin_input))
				value |= mask_value;
			mask_value = mask_value << 1;
		}
	}
	shift->bits = value;
	return 1;
#else
	return -1;
#endif
}
