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

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "usb_cdcacm.h"
#include "syscfg.h"
#include "ringb.h"

int glue_set_line_coding_cb(uint32_t baud, uint8_t databits,
			    enum usb_cdc_line_coding_bParityType cdc_parity,
			    enum usb_cdc_line_coding_bCharFormat cdc_stopbits) {
    int uart_parity;
    int uart_stopbits;

    if (databits < 8 || databits > 9) {
	return 0;
    }

    /* Be careful here, ST counts parity as a data bit */
    switch (cdc_parity) {
    case USB_CDC_NO_PARITY:
	uart_parity = USART_PARITY_NONE;
	break;
    case USB_CDC_ODD_PARITY:
	uart_parity = USART_PARITY_ODD;
	databits++;
	break;
    case USB_CDC_EVEN_PARITY:
	uart_parity = USART_PARITY_EVEN;
	databits++;
	break;
    default:
	return 0;
    }

    switch (cdc_stopbits) {
    case USB_CDC_1_STOP_BITS:
	uart_stopbits = USART_STOPBITS_1;
	break;
    case USB_CDC_2_STOP_BITS:
	uart_stopbits = USART_STOPBITS_2;
	break;
    default:
	return 0;
    }

#ifdef SPEED62500
    /* TODO: we are using an unusual fixed rate 62500 for Xpressnet */
    return 1;
#else

    /* Disable the UART while we mess with its settings */
    usart_disable(USART3);
    /* Set communication parameters */
    usart_set_baudrate(USART3, baud);
    usart_set_databits(USART3, databits);
    usart_set_parity(USART3, uart_parity);
    usart_set_stopbits(USART3, uart_stopbits);
    /* Back to work. */
    usart_enable(USART3);

    return 1;
#endif
}
