/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
                 Blue Pill

        B12        MICRO USB         GND
        B13                          GND
        B14                          3V3
        B15   RESET    GND    GND  RESET
        A8    BUTTON  *BOOT1 *BOOT0  B11 RX3
(TX0)   A9             3V3    3V3    B10 TX3
(RX0)   A10                           B1 DE
        A11                           B0
        A12                           A7
        A15                           A6
        B3       STM32F103C8T6        A5
        B4                            A4
        B5                            A3
        B6                            A2
        B7              8M            A1
        B8           32768            A0
        B9                           C15
        5V       PC13     POWER      C14
        GND      LED      LED        C13
        3V3           SWD           VBAT
          3V3 DIO  DCLK GND
              PA13 PA14

 */

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include <stdio.h>
#include "syscfg.h"
#include "usb_cdcacm.h"
#include "ringb.h"
#include "utils.h"

// #define ER_DEBUG
#ifdef ER_DEBUG
#define ER_DPRINTF(fmt, ...) \
    do { printf(fmt, ## __VA_ARGS__); } while (0)
#else
#define ER_DPRINTF(fmt, ...) \
    do { } while (0)
#endif

int de_set_first;

static inline void gpio_really(uint32_t port, uint16_t pin, const bool set) {
    int shift = set ? 0 : 16;
    GPIO_BSRR(port) = pin << shift;
}

extern struct ringb rx_ring, tx_ring;

static void usart_setup(void) {
    /* Enable the USART3 interrupt. */
    nvic_enable_irq(NVIC_USART3_IRQ);

    /* USART3 pins are on port B */
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO_USART3_RX);

    /* Enable clocks for USART3. */
    rcc_periph_clock_enable(RCC_USART3);

    /* Setup USART3 parameters. */
    usart_set_baudrate(USART3, 62500);
    usart_set_databits(USART3, 9);
    usart_set_stopbits(USART3, USART_STOPBITS_1);
    usart_set_mode(USART3, USART_MODE_TX_RX);
    usart_set_parity(USART3, USART_PARITY_NONE);
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

    /* Enable USART3 Receive interrupt. */
    usart_enable_rx_interrupt(USART3);

    /* Finally enable the USART. */
    usart_enable(USART3);
}

void usart3_isr(void) {
    uint16_t c;
    //gpio_really(GPIOA, GPIO5, 1);
    // usbser-rxne()
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
	((USART_SR(USART3) & USART_SR_RXNE) != 0)) {

	gpio_set(LED_RX_PORT, LED_RX_PIN);
	c = usart_recv(USART3);
	if (ringb_put(&rx_ring, (uint8_t) c ))  {
	    // good,
	} else {
	    // fatal, you should always have drained by now.
	    // (when you've got it all ironed out, _actually_
	    // just drop and count drops), but not yet...
	    ER_DPRINTF("rx buffer full\n");
	    // TODO
	    // while (1) ;
	}
	//trace_send8(STIMULUS_RING_PUSH, c);
	gpio_clear(LED_RX_PORT, LED_RX_PIN);
    }
    // usbser-irq-txe()
    if (((USART_CR1(USART3) & USART_CR1_TXEIE) != 0) &&
	((USART_SR(USART3) & USART_SR_TXE) != 0)) {

	if (ringb_depth(&tx_ring) == 0) {
	    // turn off tx empty interrupts, nothing left to send
	    cdcacm_arch_txirq(0, 0);
	    ER_DPRINTF("OFF\n");
	    // Turn on tx complete interrupts, for rs485 de
	    USART_CR1(USART3) |= USART_CR1_TCIE;
	} else {
	    /* TODO : check receiving before enabling */
	    if (de_set_first) {
		cdcacm_arch_pin(0, CDCACM_PIN_RS485DE, 1);
		de_set_first = 0;
		c = (ringb_get(&tx_ring) | 0x100);
	    } else {
		c = (ringb_get(&tx_ring) & 0xff);
	    }

	    usart_send(USART3, c);
	}
    }
    // usbser-irq-txc?  rs485 is auto on some devices, but can be emulated anyway

    if (((USART_CR1(USART3) & USART_CR1_TCIE) != 0) &&
	((USART_SR(USART3) & USART_SR_TC) != 0)) {

	ER_DPRINTF("TC");
	// turn off the complete irqs, we're done now.
	USART_CR1(USART3) &= ~USART_CR1_TCIE;
	USART_SR(USART3) &= ~USART_SR_TC;
	cdcacm_arch_pin(0, CDCACM_PIN_RS485DE, 0);
	de_set_first = 1;
    }
    //gpio_really(GPIOA, GPIO5, 0);
}

void usb_cdcacm_setup_pre_arch(void) {
    // Hack to reenumerate
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    gpio_clear(GPIOA, GPIO12);
    for (unsigned int i = 0; i < 800000; i++) {
	__asm__("nop");
    }
}

void usb_cdcacm_setup_post_arch(usbd_device * dev) {
    (void)dev;
}

void cdcacm_arch_pin(int port, enum cdcacm_pin pin, bool set) {
    (void)port;			// TODO if you want to handle multiple ports
    switch (pin) {
    case CDCACM_PIN_LED_TX:
	gpio_really(LED_TX_PORT, LED_TX_PIN, set);
	break;
    case CDCACM_PIN_LED_RX:
	gpio_really(LED_RX_PORT, LED_RX_PIN, set);
	break;
    case CDCACM_PIN_RS485DE:
	gpio_really(RS485DE_PORT, RS485DE_PIN, set);
	break;
    default:
	break;
    }
}

void cdcacm_arch_txirq(int port, bool set) {
    (void)port;			//FIXME if you make this multi port
    if (set) {
	usart_enable_tx_interrupt(USART3);
    } else {
	usart_disable_tx_interrupt(USART3);
    }
}

void cdcacm_arch_set_line_state(int port, uint8_t dtr, uint8_t rts) {
    (void)port;			// FIXME if you want multiple ports
    (void)dtr;
    (void)rts;
    // LM4f has an implementation of this if you're keen
}

int main(void) {

    de_set_first = 1;

    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    ER_DPRINTF("And we're alive!\n");
    /* Led */
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_RX_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_RX_PIN);
    // IRQ timing
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO5);
    rcc_periph_clock_enable(RCC_GPIOB);
    gpio_set_mode(RS485DE_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, RS485DE_PIN);

    usart_setup();

    usb_cdcacm_setup_pre_arch();
    usbd_device *usbd_dev = usb_cdcacm_init(&st_usbfs_v1_usb_driver, "stm32f103-generic");
    usb_cdcacm_setup_post_arch(usbd_dev);

    ER_DPRINTF("Looping...\n");
    while (1) {
	usbd_poll(usbd_dev);
	usb_cdcacm_poll(usbd_dev);
    }
}
