/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _SNIFFER_H_
#define _SNIFFER_H_

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CAN_MAX_RETRY	10000

/* Use GPIO A0 as debug pin */
#define OSCI_PIN_TOGGLE	GPIOA_ODR ^= 1
#define OSCI_PIN_ON	GPIOA_BSRR = 1
#define OSCI_PIN_OFF	GPIOA_BSRR = 1 << 16


/******************************************************************************
 * Simple ringbuffer implementation from open-bldc's libgovernor that
 * you can find at:
 * https://github.com/open-bldc/open-bldc/tree/master/source/libgovernor
 *****************************************************************************/

typedef int32_t ring_size_t;

struct ring {
    uint8_t *data;
    ring_size_t size;
    uint32_t begin;
    uint32_t end;
};

uint8_t nibble2bin(uint8_t s);
void bin2hex(uint8_t *dst, unsigned char c);
uint8_t hex2bin(char *s);

uint32_t micros(void);

#endif  /*  _SNIFFER_H_ */

