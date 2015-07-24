/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 *  errros designed by Gerhard Bertelsmann
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

/*
 * File descriptor
 */
struct ftdi_context ftdi;
int ftdi_bb_fd = -1;

#define MAX_BITS_TRANSFER	64

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
#ifdef DRYRUN
	return 1;
#endif
#ifdef __linux
	/* Initialize and find device */
	if (ftdi_init(&ftdi) < 0) {
		printf("%s: ftdi_init failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}

	if (ftdi_usb_open(&ftdi, 0x0403, 0x6015) < 0) {
		printf("%s: can't open FT230X device [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		ftdi_bb_fd = -1;
		return -1;
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
#ifdef DRYRUN
	return;
#endif
#ifdef __linux
	ftdi_usb_close(&ftdi);
	ftdi_deinit(&ftdi);
#endif
}

int
ftdi_bb_io(struct ftdi_bb_io *io)
{
#ifdef __linux
	uint8_t value;
/*	printf("set io dir %d\n", io->dir); 
	printf("set io pin %d\n", io->pin);
	printf("set io bit %d\n", io->bit);
	printf("\n");
*/

	/* pins > 16 are CBUS - asynch write */
	if (io->pin >= 16) {
		if ((io->dir == 0) && (io->bit == 1)) {
			pin_state |= (1 << io->pin);
			io->cbus_mask &= ~(1 << (io->pin - 16));
		} else {
			pin_state &= ~(1 << io->pin);
			io->cbus_mask |= (1 << (io->pin - 16));
		}
		printf(" **** change to CBUS \n");
#ifndef DRYRUN 
		if (ftdi_set_bitmode(&ftdi, io->cbus_mask, BITMODE_CBUS) <= 0) {
			printf("%s: ftdi_set_bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			return -1;
		}
#endif
	} else {
		/* TODO: change mask if needed */
		io->mask = actual_mask;
/*		printf("        mask is 0x%02x pin %d bit %d pin_state 0x%02x\n",io->mask, io->pin, io->bit, pin_state); */
		if (io->dir == 0) {
			io->mask  |=  (1 << io->pin);
			if (io->bit == 1) {
				pin_state |=  (1 << io->pin);
				// printf("set pin\n");
			} else {
				pin_state &= ~(1 << io->pin);
				// printf("clr pin\n");
			}
		} else 
			io->mask  &= ~(1 << io->pin);
		
/*		printf("        mask is 0x%02x pin %d bit %d pin_state 0x%02x\n",io->mask, io->pin, io->bit, pin_state); */
		
		if ( io->mask != actual_mask ) {
#ifndef DRYRUN
			if (ftdi_set_bitmode(&ftdi, io->mask, BITMODE_SYNCBB) < 0) {

				printf("%s: ftdi_set_bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			}
#endif

/*			printf("changed mask to 0x%02x\n",io->mask); */
		}
		actual_mask = io->mask;
		value = pin_state & 0xff;
#ifndef DRYRUN 
		if (ftdi_write_data(&ftdi, &value, 1) < 0) {
			printf("%s: ftdi_write_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			return -1;
		}
		if (ftdi_read_data(&ftdi, &value, 1) < 0) {
			printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
			return -1;
		}
#endif
	}
	return 1;
#else
	return -1
#endif
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
/*	printf("clock pin: %d\n", config->clock_pin);
	printf("clock falling: %d\n", config->clock_falling);
	printf("data pin input: %d\n", config->data_pin_input);
	printf("data pin output: %d\n", config->data_pin_output);
	printf("clock delay low: %d\n", config->clock_delay_low);
	printf("clock delay high: %d\n", config->clock_delay_high);
	printf("\n");
*/	clock_pin = config->clock_pin;
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
/*	printf("shift direction %d\n", shift->dir);
	printf("number bits %d\n", shift->nbits);
	printf("value 0x%016lX\n\n", shift->bits);
*/	value = shift->bits;

#if 0
	if (value == 0xA12C2B2)
		value++;
#endif

	/* TODO: maybe data_pin direction changed */
	/* io struct and mask ? */
	if (data_pin_input == data_pin_output) {
		mask = actual_mask;
		if (shift->dir)
			mask &= ~(1 << data_pin_input);
		else
			mask |= ( 1 << data_pin_input);
		if (mask != actual_mask) {
#ifndef DRYRUN
			if (ftdi_set_bitmode(&ftdi, mask, BITMODE_SYNCBB) < 0) {
				printf("%s: ftdi set bimode failed [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			}
/*			uint8_t temp = 0x04;
			if (ftdi_write_data(&ftdi, &temp, 1) < 0) {
				printf("%s: ftdi_write_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			}
			if (ftdi_read_data(&ftdi, &temp, 1) < 0) {
				printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
				return -1;
			} */
#endif
/*			printf("mask changed: 0x%02x -> 0x%02x\n", actual_mask, mask); */
			actual_mask = mask;
		}
	}


	/* TODO: prepare buffer - simple delete for now (maybe wrong if MCLR or PGM set) */
	bzero(ftdi_buf_out, MAX_BITS_TRANSFER * 4);

	for (int i = 0; i< shift->nbits; i++) {
		ftdi_buf_out[index] = pin_state;
		if (value & 1UL)
			ftdi_buf_out[index] |= 1 << data_pin_output;
		index++;
		ftdi_buf_out[index] = ftdi_buf_out[index-1] | ( 1 << clock_pin);
		index++;
		/* repeat */
		ftdi_buf_out[index] = ftdi_buf_out[index-1];
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

	/* print_buffer(ftdi_buf_out, index);
	if (shift->dir) {
		printf("read:\n");
		print_buffer(ftdi_buf_in, index);
	}
	*/

#ifndef DRYRUN 
	if ((ret = ftdi_write_data(&ftdi, ftdi_buf_out, index )) < 0) {
		printf("%s: ftdi_wrire_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
	if ((ret = ftdi_read_data(&ftdi, ftdi_buf_in, index )) < 0) {
		printf("%s: ftdi_read_error [%s]\n", __func__, ftdi_get_error_string(&ftdi));
		return -1;
	}
#endif
	value = 0;
	int mask_value = 1;
	if (shift->dir) {
		for (int i = 0; i < shift->nbits; i++ ) {
/*			printf("data 0x%02x pin 0x%02x\n", ftdi_buf_in[i*4 + 2], 1 << data_pin_input); */
			if (ftdi_buf_in[i*4 + 2] & (1 << data_pin_input))
				value |= mask_value;
			mask_value = mask_value << 1;
		}
/*		printf("%s: value 0x%016lX\n", __func__, value); */
	}
	shift->bits = value;
	return 1;
#else
	return -1;
#endif
}
