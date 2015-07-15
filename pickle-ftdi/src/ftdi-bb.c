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
int ftdi_bb_fd = -1;

int
ftdi_bb_open(const char *device)
{
#ifdef __linux
	struct ftdi_context ftdi;

	ftdi_bb_fd = open(device, O_RDWR);
	if (ftdi_bb_fd < 0) {
		printf("%s: warning: open failed [%s]\n", __func__, strerror(errno));
		ftdi_bb_fd = -1;
		return -1;
	}
	return ftdi_bb_fd;
#else
	return -1;
#endif
}

void
ftdi_bb_close(void)
{
#ifdef __linux
	close(ftdi_bb_fd);
	ftdi_bb_fd = -1;
#endif
}

int
ftdi_bb_io(struct ftdi_bb_io *io)
{
#ifdef __linux
	return ioctl(ftdi_bb_fd, GPIO_BB_IO, io);
#else
	return -1
#endif
}

int
ftdi_bb_configure(struct ftdi_bb_config *config)
{
#ifdef __linux
	return ioctl(ftdi_bb_fd, GPIO_BB_CONFIGURE, config);
#else
	return -1
#endif
}

int
ftdi_bb_shift(struct ftdi_bb_shift *shift)
{
#ifdef __linux
	return ioctl(ftdi_bb_fd, GPIO_BB_SHIFT, shift);
#else
	return -1;
#endif
}
