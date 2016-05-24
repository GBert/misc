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

#ifndef _LED_H
#define _LED_H

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

/* BUFFER_SIZE must be power of two (16,32,64...) */
#define SERIAL_BUFFER_SIZE	256
#define SERIAL_BUFFER_SIZE_MASK (SERIAL_BUFFER_SIZE -1)

/* circular buffer */
struct serial_buffer_t {
    uint8_t head;
    uint8_t tail;
    unsigned char data[SERIAL_BUFFER_SIZE];
};

void init_uart(void);
char uart_putchar(unsigned char c);
void uart_putchar_wait(unsigned char c);
void uart_puts_rom(const char *s);
char print_rom_fifo(const char *s, struct serial_buffer_t *fifo);
char fifo_putchar(struct serial_buffer_t *fifo);
void print_debug_fifo(struct serial_buffer_t *fifo);
void print_hex_fifo(char c, struct serial_buffer_t *fifo);

#endif
