/*
 * Copyright (C) 2015 Darron Broad
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

#include "pickle.h"

/*
 * Session
 */
extern struct pickle p;

extern int io_stop;

/*
 * Memory handle
 */
static int gpio_mem = -1;

/*
 * Memory mapped I/O pointer
 */
static void *gpio_map = NULL;

/*
 * I/O Pins 0..287
 */
static uint8_t gpio_pins[GPIO_BPI_NPINS], gpio_dirs[GPIO_BPI_NPINS];
static uint32_t gpio_cfg[GPIO_BPI_NPINS], gpio_dat[GPIO_BPI_NPINS], gpio_pull[GPIO_BPI_NPINS];

/*
 * Map Banana Pi GPIO memory
 */
int
gpio_bpi_open(const char *device)
{
	/* Open /dev/mem */
	gpio_mem = open(device, O_RDWR | O_SYNC);
	if (gpio_mem < 0) {
		printf("%s: warning: open failed [%s]\n", __func__, strerror(errno));
		gpio_mem = -1;
		return -1;
	}

	/* Memory map GPIO */
	gpio_map = mmap(NULL, BPI_MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_mem, BPI_BASE_ADDR);
	if (gpio_map == MAP_FAILED) {
		printf("%s: warning: mmap failed [%s]\n", __func__, strerror(errno));
		close(gpio_mem);
		gpio_mem = -1;
		return -1;
	}

	/* Reset used pin flags */
	memset(&gpio_pins, 0, GPIO_BPI_NPINS);

	/* Initialise CFG / DAT / PULL offset tables */
	for (uint16_t i = 0; i < GPIO_BPI_NPINS; ++i) {
		uint16_t ofs;

		ofs = BPI_BASE_OFS + (i >> 5) * 36 + ((i & 0x18) >> 1);
		gpio_cfg[i] = (ofs >> 2);

		ofs = BPI_BASE_OFS + (i >> 5) * 36 + 16;
		gpio_dat[i] = (ofs >> 2);

		ofs = BPI_BASE_OFS + (i >> 5) * 36 + ((i & 0x10) >> 2) + 28;
		gpio_pull[i] = (ofs >> 2);
	}

	return gpio_mem;
}

/*
 * Un-map Banana Pi GPIO memory
 */
void
gpio_bpi_close(void)
{
	if (gpio_map) {
		if (munmap(gpio_map, BPI_MAP_LEN)) {
			printf("%s: warning: munmap failed\n", __func__);
		}
		gpio_map = NULL;
	}
	if (gpio_mem >= 0) {
		if (close(gpio_mem)) {
			printf("%s: warning: close failed\n", __func__);
		}
		gpio_mem = -1;
	}
}

static inline void
gpio_bpi_read(uint32_t gpio_reg, uint32_t *val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*val = *reg;
}

void
gpio_bpi_delay(void)
{
	uint32_t val;

	gpio_bpi_read(gpio_dat[256], &val); /* PI */
}

static inline void
gpio_bpi_write(uint32_t gpio_reg, uint32_t val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*reg = val;
}

/*
 * Determine PULL bit shift for GPIO
 */
static inline uint32_t
gpio_bpi_pshift(uint16_t pin)
{
	return (pin & 0x0F) << 1;
}

static inline void
gpio_bpi_pud(uint16_t pin, uint8_t pud)
{
	uint32_t val;

	gpio_bpi_read(gpio_pull[pin], &val);

	val &= ~(3UL << gpio_bpi_pshift(pin));
	val |= (uint32_t)pud << gpio_bpi_pshift(pin);

	gpio_bpi_write(gpio_pull[pin], val);
}

/*
 * Determine CFG SEL bit shift for GPIO
 */
static inline uint32_t
gpio_bpi_lshift(uint16_t pin)
{
	return (pin & 0x07) << 2;
}

static inline void
gpio_bpi_select_input(uint16_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_cfg[pin];

	uint32_t val = ~(0x7 << gpio_bpi_lshift(pin));
	*reg &= val; /* X000 = Input */

	gpio_dirs[pin] = 1;
}

static inline void
gpio_bpi_select_output(uint16_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_cfg[pin];
	
	uint32_t val = 0x1 << gpio_bpi_lshift(pin);
	*reg |= val; /* X001 = Output */

	gpio_dirs[pin] = 0;
}

static inline void
gpio_bpi_select_alt(uint16_t pin, uint8_t alt)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_cfg[pin];

	uint32_t val = alt << gpio_bpi_lshift(pin);
	*reg |= val; /* MULTIPLEX FUNCTION SELECT */
}

/*
 * Determine DAT bit for GPIO
 */
static inline uint32_t
gpio_bpi_bit(uint16_t pin)
{
	return (1UL << (pin & 31));
}

int
gpio_bpi_get(uint16_t pin, uint8_t *level)
{
	if (pin >= GPIO_BPI_NPINS)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_bpi_pud(pin, PX_PULL_UP);
		gpio_bpi_select_input(pin);
	}
	else if (gpio_dirs[pin] == 0) {
		gpio_bpi_select_input(pin);
	}

	uint32_t val;

	gpio_bpi_read(gpio_dat[pin], &val);

	*level = (val & gpio_bpi_bit(pin)) ? (HIGH) : (LOW);

	return 0;
}

int
gpio_bpi_set(uint16_t pin, uint8_t level)
{
	if (pin >= GPIO_BPI_NPINS)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_bpi_pud(pin, PX_PULL_UP);
		gpio_bpi_select_input(pin);
		gpio_bpi_select_output(pin);
	}
	else if (gpio_dirs[pin] == 1) {
		gpio_bpi_select_output(pin);
	}

	uint32_t val;

	gpio_bpi_read(gpio_dat[pin], &val);

	if (!level)
		val &= ~gpio_bpi_bit(pin);
	else
		val |= gpio_bpi_bit(pin);

	gpio_bpi_write(gpio_dat[pin], val);

	return 0;
}

/*
 * Select pin as input, may reconfigure UART3
 */
int
gpio_bpi_release(uint16_t pin, uint8_t alt)
{
	if (pin >= GPIO_BPI_NPINS)
		return -1;

	gpio_bpi_pud(pin, PX_PULL_DIS);
	gpio_bpi_select_input(pin);
	if (alt) {
		if (pin == 224 || pin == 225)
			gpio_bpi_select_alt(pin, PX_SELECT4);
	}

	return 0;
}
