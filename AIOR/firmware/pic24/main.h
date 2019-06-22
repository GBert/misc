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

#ifndef _MAIN_H
#define _MAIN_H

#include <stdlib.h>		/* rand() */
#include <stdbool.h>		/* true || false */
#include <stdint.h>
#include <p33Exxxx.h>
// #include <pps.h>
#include <dsp.h>

/*
 *                   /            PLLDIV + 2            \
 * FOSC = F_pllin * { ---------------------------------- }
 *                   \ (PLLPRE + 2) * 2 * (PLLPOST + 1) /
 *
 * // HS 8 MHz
 * FOSC = Fin * M/(N1 * N2), FCY = FOSC/2
 * FOSC = 8 * (64)/(2 * 2) = 128 MHz for FOSC, FCY = 64 MHz
 */

#define PLL_PRE  (0)		/* PLLPRE divide by 2  */
#define PLL_DIV  (64 - 2)	/* PLLDIV = 64         */
#define PLL_POST (0)		/* PLLPOST divide by 2 */

/*
 * FCY = FOSC / 2 = 64MHz
 */
#define FOSC 128000000UL
#define FCY 64000000UL
#include <libpic30.h>		/* __delay32() */

#endif
