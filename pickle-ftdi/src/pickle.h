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

#ifndef _PICKLE_H
#define _PICKLE_H

#undef DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sysexits.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <search.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <time.h>
#include <errno.h> 
#include <libgen.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>

#define LOW   (0x00)
#define HIGH  (0x01)
#define FLOAT (0xFF)

#ifndef FALSE
#define FALSE (LOW)
#endif

#ifndef TRUE
#define TRUE (HIGH)
#endif

#define STRLEN (512)
#define STRMAX (STRLEN - 1)

#define BUFLEN (512)
#define BUFMAX (BUFLEN - 1)

#define UL_ON  ("\033[4m")
#define UL_OFF ("\033[0m")

#define ARCH12BIT (0x00000FFF)
#define ARCH14BIT (0x00003FFF)
#define ARCH16BIT (0x0000FFFF)
#define ARCH24BIT (0x00FFFFFF)
#define ARCH32BIT (0xFFFFFFFF) /* UNIMPLEMENTED */

#define NOKEY  (0) /* NO  KEY IN OPERATION  */
#define HVPKEY (1) /* HVP KEY IN OPERATION */
#define LVPKEY (2) /* LVP KEY IN OPERATION */

struct pickle;

/* OUTPUT BACKENDS */
#ifdef TTY
#include "rw.h"
#include "serial_posix.h"
#include "ip.h"
#endif
#ifdef RPI
#include "raspi.h"
#endif
#ifdef MCP23017
#include "mcp23017.h"
#endif
#ifdef BITBANG
#include "gpio-bb.h"
#endif

#include "util.h"
#include "dotconf.h"
#include "io.h"
#include "pic.h"
#include "pic12.h"
#include "pic14.h"
#include "pic16.h"
#include "pic24.h"
#include "pic32.h"

/* INPUT FRONTENDS */
#include "inhx32.h"
#ifdef TTY
#include "stk500v2.h"
#include "stk500v2_listen.h"
#include "stk500v2_load.h"
#endif

/* SESSION */
struct pickle {
	/* Message output stream (NULL = disable)                               */
	FILE *f;

	/* Command line options */
	char devicename[STRLEN];/* overridden PICMicro device name		*/ 
	uint32_t key;		/* MCHP LVP key					*/

	/* Dot file settings */
	char dotfile[STRLEN];	/* configuration file name			*/
	char device[STRLEN];	/* I/O device name: tty or rpi			*/
	char etc[STRLEN];	/* overridden etc directory path		*/ 
	uint16_t bitrules;	/* I/O bit rules				*/
	uint32_t busy;		/* I/O busy cursor speed			*/
	uint32_t sleep_low;	/* I/O clock low duration			*/
	uint32_t sleep_high;	/* I/O clock high duration			*/
	uint32_t fwsleep;	/* ICSPIO bit time				*/
	uint32_t debug;		/* default 0 (no debugging)			*/
	uint8_t clock_falling;	/* Clock falling edge for shifting in bits      */
#ifdef TTY
	uint32_t baudrate;	/* TTY baud rate                                */
#endif

	/* I/O backends */
	uint8_t iot;		/* I/O type (tty, rpi or i2c)			*/
#ifdef TTY
	int serial_port;	/* tty bit-bang port descriptor                 */
#endif
#ifdef MCP23017
	int mcp;		/* MCP23017 I2C address				*/
#endif
#if defined(RPI) || defined(BITBANG)
        uint8_t vpp;		/* TX/!MCLR/VPP     */
        uint8_t pgc;		/* RTS/PGC CLOCK    */
        uint8_t pgdo;		/* DTR/PGD DATA_OUT */
        uint8_t pgdi;		/* CTS/PGD DATA_IN  */
        uint8_t pgm;		/* PGM OUT          */
#endif
	/* Hardware operations */
	struct pic_ops *pic;
};

/* prototypes */
void usage_k8048(void);
void usage_kctrl(char *);
void usage_kload(char *);
void usage_ktest(char *);
void usage_k12(char *);
void usage_k14(char *);
void usage_k16(char *);
void usage_k24(char *);
void usage_k32(char *);
void usage(char *, char *);
int main(int, char **);

#endif /* !_PICKLE_H */
