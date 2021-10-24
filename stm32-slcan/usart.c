/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 * Copyright (C) 2011 Piotr Esden-Tempski <piotr@esden.net>
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

#include "stm32-slcan.h"

#define BUFFER_SIZE	1024
#define USART2_SPEED	500000

#define RING_SIZE(RING)  ((RING)->size - 1)
#define RING_DATA(RING)  (RING)->data
#define RING_EMPTY(RING) ((RING)->begin == (RING)->end)

extern volatile uint8_t commands_pending;

int _write(int file, char *ptr, int len);

static void ring_init(struct ring *ring, uint8_t * buf, ring_size_t size) { 
    ring->data = buf;
    ring->size = size;
    ring->begin = 0;
    ring->end = 0;
}

int32_t ring_write_ch(struct ring *ring, uint8_t ch) {
    if (((ring->end + 1) % ring->size) != ring->begin) {
	ring->data[ring->end++] = ch;
	ring->end %= ring->size;
	return (uint32_t) ch;
    }
    return -1;
}

int32_t ring_write(struct ring *ring, uint8_t * data, ring_size_t size) {
    int32_t i;

    for (i = 0; i < size; i++) {
	if (ring_write_ch(ring, data[i]) < 0)
	    return -i;
    }
    return i;
}

int32_t ring_read_ch(struct ring *ring, uint8_t * ch) {
    int32_t ret = -1;

    if (ring->begin != ring->end) {
	ret = ring->data[ring->begin++];
	ring->begin %= ring->size;
	if (ch)
	    *ch = ret;
    }
    return ret;
}

struct ring output_ring;
struct ring input_ring;
uint8_t output_ring_buffer[BUFFER_SIZE];
uint8_t input_ring_buffer[BUFFER_SIZE];

void usart_setup(void) {
    /* Initialize output ring buffer. */
    ring_init(&output_ring, output_ring_buffer, BUFFER_SIZE);
    ring_init(&input_ring, input_ring_buffer, BUFFER_SIZE);

    /* Enable the USART2 interrupt. */
    nvic_enable_irq(NVIC_USART2_IRQ);

    /* Setup GPIO pin GPIO_USART2_TX on GPIO port A for transmit. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART2_TX);

    /* Setup GPIO pin GPIO_USART2_RX on GPIO port A for receive. */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);

    /* Setup UART parameters. */
    usart_set_baudrate(USART2, USART2_SPEED);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_parity(USART2, USART_PARITY_NONE);
    /* TODO use hardware handshaking */
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART2, USART_MODE_TX_RX);

    /* Enable USART2 Receive interrupt. */
    USART_CR1(USART2) |= USART_CR1_RXNEIE;

    /* Finally enable the USART. */
    usart_enable(USART2);
}

void usart2_isr(void) {
    uint8_t c;
    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) && ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {
	c = usart_recv(USART2);

	/* TODO Indicate that we got data. */
	/* gpio_toggle(GPIOA, GPIO8); */

	/* TODO check befor overflow */
	/* Retrieve the data from the peripheral. */

        /* ignore \n */
	if (c != '\n')
	    ring_write_ch(&input_ring, c);
	if (c == '\r')
	    commands_pending++;
    }

    /* Check if we were called because of TXE. */
    if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) && ((USART_SR(USART2) & USART_SR_TXE) != 0)) {

	int32_t data;

	data = ring_read_ch(&output_ring, NULL);

	if (data == -1) {
	    /* Disable the TXE interrupt, it's no longer needed. */
	    USART_CR1(USART2) &= ~USART_CR1_TXEIE;
	} else {
	    /* Put data into the transmit register. */
	    usart_send(USART2, data);
	}
    }
}

int _write(int file, char *ptr, int len) {
    int ret;

    if (file == 1) {
	ret = ring_write(&output_ring, (uint8_t *) ptr, len);
	if (ret < 0)
	    ret = -ret;
	USART_CR1(USART2) |= USART_CR1_TXEIE;
	return ret;
    }
    errno = EIO;
    return -1;
}
