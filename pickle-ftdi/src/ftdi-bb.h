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

#ifndef _FTDI_BB_H
#define _FTDI_BB_H

#include <ftdi.h>

#define FTDI_BB_MAX_BITS_TRANSFER (64)
#define FTDI_BB_BUFFER_SIZE (FTDI_BB_MAX_BITS_TRANSFER * 8) /* 512 */

struct ftdi_bb_io {
	uint8_t dir;
	uint8_t pin;
	uint8_t bit;
} __attribute__((packed));

struct ftdi_bb_config {
	uint16_t clock_pin;
	uint16_t data_pin_input;
	uint16_t data_pin_output;
	uint8_t clock_falling;
	uint8_t msb_first;
	uint8_t clock_delay_low;
	uint8_t clock_delay_high;
} __attribute__((packed));

struct ftdi_bb_shift {
	uint8_t dir;
	uint8_t nbits;
	uint64_t bits;
} __attribute__((packed));

int ftdi_bb_open(const char *);
void ftdi_bb_close(void);
int ftdi_bb_io(struct ftdi_bb_io *);
int ftdi_bb_configure(struct ftdi_bb_config *);
int ftdi_bb_shift(struct ftdi_bb_shift *);

#endif /* _FTDI_BB_H */
