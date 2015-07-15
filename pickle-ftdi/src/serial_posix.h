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

#ifndef _SERIAL_POSIX_H
#define _SERIAL_POSIX_H

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

/* Linux / CYGWIN */
#define SERIAL_DEVICE "/dev/ttyS0"

typedef struct {
	uint32_t baud;
	speed_t speed;
} baudrate_t;

int serial_open(const char *, speed_t);
void serial_close(int);
int get_cts(int);
void set_dtr(int, int);
void set_rts(int, int);
void set_tx(int, int);
int serial_get(int, char *, int, int);
int serial_read(int, char *, int, int);
int serial_write(int, char *, int, int);
speed_t serial_speed(uint32_t);

#endif
