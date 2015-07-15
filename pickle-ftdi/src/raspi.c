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
 * I/O Pins 0..31 on headers P1 & P5
 */
static uint8_t gpio_pins[32], gpio_dirs[32];

/*
 * Map Raspberry-Pi GPIO memory
 */
int
gpio_open(const char *device, uint8_t rev)
{
#ifdef RPI
	off_t gpio_base_addr;

	/* Determine GPIO base address */
	if (rev == '\0' || rev == '1') {
		gpio_base_addr = BCM2835_PERI_BASE_ADDR + GPIO_BASE_ADDR_OFFSET;
	}
	else if (rev == '2') {
		gpio_base_addr = BCM2836_PERI_BASE_ADDR + GPIO_BASE_ADDR_OFFSET;
	}
	else {
		return -1; /* Unknown */
	}

	/* Open /dev/mem */
	gpio_mem = open(device, O_RDWR | O_SYNC);
	if (gpio_mem < 0) {
		printf("%s: warning: open failed [%s]\n", __func__, strerror(errno));
		gpio_mem = -1;
		return -1;
	}

	/* Memory map GPIO */
	gpio_map = mmap(NULL, GPIO_MAP_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_mem, gpio_base_addr);
	if (gpio_map == MAP_FAILED) {
		printf("%s: warning: mmap failed [%s]\n", __func__, strerror(errno));
		close(gpio_mem);
		gpio_mem = -1;
		return -1;
	}

	/* Reset used pin flags */
	memset(&gpio_pins, 0, 32);

	return gpio_mem;
#else
	return -1; /* Unsupported */
#endif
}

/*
 * Un-map Raspberry-Pi GPIO memory
 */
void
gpio_close(void)
{
	if (gpio_map) {
		if (munmap(gpio_map, GPIO_MAP_LEN)) {
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
gpio_read(uint8_t gpio_reg, uint32_t *val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*val = *reg;
}

void
gpio_delay(void)
{
	uint32_t val;

	gpio_read(GPLEV0, &val);
}

static inline void
gpio_write(uint8_t gpio_reg, uint32_t val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*reg = val;
}

static inline void
gpio_pud(uint8_t pin, uint8_t pud)
{
	gpio_write(GPPUD, pud);
	usleep(10); /* ? */
	gpio_write(GPPUDCLK0, (1 << pin));
	usleep(10); /* ? */
	gpio_write(GPPUD, 0);
	gpio_write(GPPUDCLK0, 0);
}

/*
 * Determine GPFSEL register offset for GPIO
 *
 *  GPIO00 .. GPIO09 GPFSEL0
 *  GPIO10 .. GPIO19 GPFSEL1
 *  GPIO20 .. GPIO29 GPFSEL2
 *  GPIO30 .. GPIO39 GPFSEL3
 *  GPIO40 .. GPIO49 GPFSEL4
 *  GPIO50 .. GPIO59 GPFSEL5
 *
 * BCM2835-ARM-Peripherals Page 92
 */
static inline uint32_t
gpio_gpfsel(uint8_t pin)
{
	return (pin / 10) /* + GPFSEL0 */;
}

/*
 * Determine GPFSEL bit shift for GPIO
 *
 *  GPIO0 0	GPIO5 15
 *  GPIO1 3	GPIO6 18
 *  GPIO2 6	GPIO7 21
 *  GPIO3 9	GPIO8 24
 *  GPIO4 12	GPIO9 27
 *
 * BCM2835-ARM-Peripherals Page 92
 */
static inline uint32_t
gpio_lshift(uint8_t pin)
{
	return (pin % 10) * 3;
}

static inline void
gpio_select_input(uint8_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_gpfsel(pin);

	uint32_t val = ~(7 << gpio_lshift(pin));
	*reg &= val; /* 000 = Input */

	gpio_dirs[pin] = 1;
}

static inline void
gpio_select_output(uint8_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_gpfsel(pin);
	
	uint32_t val = 1 << gpio_lshift(pin);
	*reg |= val; /* 001 = Output */

	gpio_dirs[pin] = 0;
}

static inline void
gpio_select_alt(uint8_t pin, uint8_t alt)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_gpfsel(pin);

	uint32_t val = alt << gpio_lshift(pin);
	*reg |= val; /* ALT0 .. ALT5 */
}

int
gpio_get(uint8_t pin, uint8_t *level)
{
	if (pin > 31)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_pud(pin, GPPUD_UP);
		gpio_select_input(pin);
	}
	else if (gpio_dirs[pin] == 0) {
		gpio_select_input(pin);
	}

	uint32_t val;

	gpio_read(GPLEV0, &val);

	*level = (val & (1 << pin)) ? (HIGH) : (LOW);

	return 0;
}

int
gpio_set(uint8_t pin, uint8_t level)
{
	if (pin > 31)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_pud(pin, GPPUD_UP);
		gpio_select_input(pin);
		gpio_select_output(pin);
	}
	else if (gpio_dirs[pin] == 1) {
		gpio_select_output(pin);
	}

	gpio_write((level) ? (GPSET0) : (GPCLR0), (1 << pin));

	return 0;
}

/*
 * Select pin as input, re-enable ALT0 for UART
 */
int
gpio_release(uint8_t pin, uint8_t alt)
{
	if (pin > 31)
		return -1;

	gpio_pud(pin, GPPUD_OFF);
	gpio_select_input(pin);
	if (alt) {
		if (pin == 14 || pin == 15)
			gpio_select_alt(pin, GPIO_ALT0);
	}
	return 0;
}

/*
 * Test GPIO pins
 */
void
gpio_test(int seconds)
{
	uint8_t output_level = 0, input_level;

	printf("\nTEST MODE 4 [RPI GPIO] CTRL-C TO STOP\n");

	while (!io_stop) {
		printf("\n");

		gpio_set(p.vpp, output_level);
		printf("GPIO %-3d (VPP) (TX)  = %d\n", p.vpp, output_level);

		if (p.pgm != GPIO_PGM_DISABLED) {
			gpio_set(p.pgm, output_level);
			printf("GPIO %-3d (PGM)       = %d\n", p.pgm, output_level);
		}

		gpio_set(p.pgc, output_level);
		printf("GPIO %-3d (PGC) (RTS) = %d\n", p.pgc, output_level);

		gpio_set(p.pgdo, output_level);
		printf("GPIO %-3d (PGD) (DTR) = %d\n", p.pgdo,output_level);

		if (p.pgdi != p.pgdo) {
			gpio_get(p.pgdi, &input_level);
			printf("GPIO %02d (PGD) (CTS) = %d\n", p.pgdi, input_level);
		}

		fflush(stdout);
		
		sleep(seconds);
		output_level = 1 - output_level;
	}
}
