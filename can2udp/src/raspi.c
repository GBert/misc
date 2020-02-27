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

#include "raspi.h"

static unsigned get_dt_ranges(const char *filename, unsigned offset) {
   unsigned address = ~0;
   FILE *fp = fopen(filename, "rb");
   if (fp)
   {
      unsigned char buf[4];
      fseek(fp, offset, SEEK_SET);
      if (fread(buf, 1, sizeof buf, fp) == sizeof buf)
         address = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;
      fclose(fp);
   }
   return address;
}

unsigned bcm_host_get_peripheral_address(void) {
   unsigned address = get_dt_ranges("/proc/device-tree/soc/ranges", 4);
   if (address == 0)
      address = get_dt_ranges("/proc/device-tree/soc/ranges", 8);
   return address == ~0U ? 0x20000000 : address;
}

/******************************************************************************
 *
 * Back-end
 *
 *****************************************************************************/

static int gpio_mem = -1;	/* Memory handle */

static void *gpio_map = NULL;	/* Memory mapped I/O pointer */

/* I/O Pins 0..31 on headers P1 & P5 */
static uint8_t gpio_pins[GPIO_RPI_NPINS], gpio_dirs[GPIO_RPI_NPINS];

/*******************************************************************************
 *
 * I/O operations
 *
 ******************************************************************************/

int
gpio_rpi_open(const char *device)
{
	off_t gpio_base_addr;

	/* Determine GPIO base address */
	gpio_base_addr = bcm_host_get_peripheral_address() + GPIO_BASE_ADDR_OFFSET;

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
	memset(&gpio_pins, 0, GPIO_RPI_NPINS);

	return 0;
}

void
gpio_rpi_close(void)
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

char *
gpio_rpi_error(void)
{
	return "Can't open RPi I/O";
}

void
gpio_rpi_usleep(int n)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + GPLEV0;
	uint32_t val;

	while (n--)
		val = *reg;

	(void)val;
}

static inline void
gpio_rpi_read(uint8_t gpio_reg, uint32_t *val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*val = *reg;
}

static inline void
gpio_rpi_write(uint8_t gpio_reg, uint32_t val)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_reg;

	*reg = val;
}

static inline void
gpio_rpi_pud(uint8_t pin, uint8_t pud)
{
	gpio_rpi_write(GPPUD, pud);
	usleep(10); /* ? */
	gpio_rpi_write(GPPUDCLK0, (1 << pin));
	usleep(10); /* ? */
	gpio_rpi_write(GPPUD, 0);
	gpio_rpi_write(GPPUDCLK0, 0);
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
gpio_rpi_gpfsel(uint8_t pin)
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
gpio_rpi_lshift(uint8_t pin)
{
	return (pin % 10) * 3;
}

void
gpio_rpi_select_input(uint8_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_rpi_gpfsel(pin);

	uint32_t val = ~(7 << gpio_rpi_lshift(pin));
	*reg &= val; /* 000 = Input */

	gpio_dirs[pin] = 1;
}

void
gpio_rpi_select_output(uint8_t pin)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_rpi_gpfsel(pin);
	
	uint32_t val = 1 << gpio_rpi_lshift(pin);
	*reg |= val; /* 001 = Output */

	gpio_dirs[pin] = 0;
}

static inline void
gpio_rpi_select_alt(uint8_t pin, uint8_t alt)
{
	GPIO_ADDR reg = (GPIO_ADDR)(gpio_map) + gpio_rpi_gpfsel(pin);

	uint32_t val = alt << gpio_rpi_lshift(pin);
	*reg |= val; /* ALT0 .. ALT5 */
}

int
gpio_rpi_get(uint16_t pin, uint8_t *level)
{
	if (pin >= GPIO_RPI_NPINS)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_rpi_pud(pin, GPPUD_UP);
		gpio_rpi_select_input(pin);
	}
	else if (gpio_dirs[pin] == 0) {
		gpio_rpi_select_input(pin);
	}

	uint32_t val;

	gpio_rpi_read(GPLEV0, &val);

	*level = (val & (1 << pin)) ? (HIGH) : (LOW);

	return 0;
}

int
gpio_rpi_set(uint16_t pin, uint8_t level)
{
	if (pin >= GPIO_RPI_NPINS)
		return -1;

	if (gpio_pins[pin] == 0) {
		gpio_pins[pin] = 1;
		gpio_rpi_pud(pin, GPPUD_UP);
		gpio_rpi_select_input(pin);
		gpio_rpi_select_output(pin);
	}
	else if (gpio_dirs[pin] == 1) {
		gpio_rpi_select_output(pin);
	}

	gpio_rpi_write((level) ? (GPSET0) : (GPCLR0), (1 << pin));

	return 0;
}

/*
 * Select pin as input, re-enable ALT0 for UART
 */
int
gpio_rpi_release(uint16_t pin, uint8_t alt)
{
	if (pin >= GPIO_RPI_NPINS)
		return -1;

	gpio_rpi_pud(pin, GPPUD_OFF);
	gpio_rpi_select_input(pin);
	if (alt) {
		if (pin == 14 || pin == 15)
			gpio_rpi_select_alt(pin, GPIO_ALT0);
	}
	return 0;
}
