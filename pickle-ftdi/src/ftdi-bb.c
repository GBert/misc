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
#include <sys/ioctl.h>

#include <ftdi.h>
#include "ftdi-bb.h"

/*
 * File descriptor
 */
struct ftdi_context *ftdi;
int ftdi_bb_fd = -1;

/* buffer for the bitbanged data */
uint8_t ftdi_buf_out[64];
uint8_t ftdi_buf_in[64];

int
ftdi_bb_open(const char *device)
{
#ifdef __linux
	/* Initialize and find device */
	if (ftdi_init(ftdi) < 0) {
		printf("%s: ftdi_init failed [%s]\n", __func__, ftdi_get_error_string(ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}

	if (ftdi_usb_open(ftdi, 0x0403, 0x6015) < 0) {
		printf("%s: can't open FT230X device [%s]\n", __func__, ftdi_get_error_string(ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}
	if (ftdi_set_bitmode(ftdi, 0xff, BITMODE_BITBANG) < 0) {
		printf("%s: can't enable bitbang mode [%s]\n", __func__, ftdi_get_error_string(ftdi));
		ftdi_bb_fd = -1;
		return -1;
	}
	/* TODO: set baudrate (needed) - defines the bitbang speed (formula ?) */
	if(ftdi_set_baudrate(ftdi, 1562) < 0) {
		printf("%s: can't set baudrate [%s]\n", __func__, ftdi_get_error_string(ftdi));
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
	ftdi_usb_close(ftdi);
	ftdi_deinit(ftdi);
#endif
}

int
ftdi_bb_io(struct ftdi_bb_io *io)
{
#ifdef __linux
	return 1;
	// return ioctl(ftdi_bb_fd, GPIO_BB_IO, io);
#else
	return -1
#endif
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
	return 1;
	// return ioctl(ftdi_bb_fd, GPIO_BB_CONFIGURE, config);
#else
	return -1
#endif
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
#ifdef __linux
	return 1;
	// return ioctl(ftdi_bb_fd, GPIO_BB_SHIFT, shift);
#else
	return -1;
#endif
}
