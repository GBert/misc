/*
 * Copyright (C) 2013-2017 Darron Broad
 * All rights reserved.
 * 
 * This file is part of GPIO bit-bang driver
 * 
 * GPIO bit-bang driver is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * GPIO bit-bang driver is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with GPIO bit-bang driver. If not, see http://www.gnu.org/licenses/
 */

#ifndef _GPIO_BB_H
#define _GPIO_BB_H

#define GPIO_BB_VERSION_MAJ (1)
#define GPIO_BB_VERSION_MIN (3)

#ifdef __KERNEL__
#include <linux/delay.h>
#include <linux/slab.h>
#endif

struct gpio_bb_io {
	uint16_t pin;
	uint8_t dir;
	uint8_t bit;
} __attribute__((packed));

struct gpio_bb_config {
	uint16_t clock_pin;
	uint16_t data_pin_input;
	uint16_t data_pin_output;
	uint8_t clock_falling;
	uint8_t msb_first;
	uint8_t clock_delay_low;
	uint8_t clock_delay_high;
	uint8_t lock;
} __attribute__((packed));

struct gpio_bb_shift {
	uint8_t dir;
	uint8_t nbits;
	uint64_t bits;
} __attribute__((packed));

#ifdef GPIO_BB_AFSK
struct gpio_bb_tone {
	uint16_t pin;
	uint16_t entries;
	uint32_t delay_mark;
	uint32_t delay_space;
	uint32_t duration_mark;
	uint32_t duration_space;
} __attribute__((packed));

struct gpio_bb_tx {
	uint8_t nbits;
	uint64_t bits;
} __attribute__((packed));
#endif /* GPIO_BB_AFSK */

#define GPIO_BB_MAX (512)
#define GPIO_BB_MASK (GPIO_BB_MAX - 1)

#define GPIO_BB_MAJOR (180)
#define GPIO_BB_IO		_IOWR(GPIO_BB_MAJOR, 205, struct gpio_bb_io *)
#define GPIO_BB_CONFIGURE	_IOW(GPIO_BB_MAJOR,  206, struct gpio_bb_config *)
#define GPIO_BB_SHIFT		_IOWR(GPIO_BB_MAJOR, 207, struct gpio_bb_shift *)

#ifdef GPIO_BB_AFSK
#define GPIO_BB_TONE		_IOW(GPIO_BB_MAJOR,  203, struct gpio_bb_tone *)
#define GPIO_BB_TX		_IOW(GPIO_BB_MAJOR,  204, struct gpio_bb_tx *)
#endif /* GPIO_BB_AFSK */

#ifndef __KERNEL__
int gpio_bb_open(const char *);
void gpio_bb_close(void);
int gpio_bb_io(struct gpio_bb_io *);
int gpio_bb_configure(struct gpio_bb_config *);
int gpio_bb_shift(struct gpio_bb_shift *);

#ifdef GPIO_BB_AFSK
int gpio_bb_tone(struct gpio_bb_tone *);
int gpio_bb_tx(void);
#endif /* GPIO_BB_AFSK */
#endif /* __KERNEL__ */

#endif /* _GPIO_BB_H */
