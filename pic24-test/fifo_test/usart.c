/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"
#include "usart.h"

const char *sData = " Data: ";

void init_uart(void) {
    U1MODEbits.STSEL = 1;	/* 2-stop bit             */
    U1MODEbits.PDSEL = 0;	/* No Parity, 8-data bits */
    U1MODEbits.ABAUD = 0;	/* Auto-Baud disabled     */
    U1MODEbits.BRGH = 0;	/* normal Speed mod       */

    U1BRG = SBRG_VAL;		/* calculated by defines  */

    U1STAbits.URXISEL = 0;	/* Interrupt after one RX character is received */
    U1MODEbits.UARTEN = 1;	/* Enable UART */
    U1STAbits.UTXEN = 1;	/* enable UART */
}

/* prints char on USART if pssible */
char uart_putchar(unsigned char c) {
    if (!U1STAbits.UTXBF) {
	if (U1STAbits.OERR)
	    U1STAbits.OERR = 0;
	U1TXREG = c;
	return 1;
    }
    return 0;
}

/* place char into fifo */
char putchar_fifo(char c, struct serial_buffer_t *fifo) {
    unsigned char head;
    head = fifo->head;
    head++;
    if (head != fifo->tail) {
	fifo->head = head;
	fifo->data[head] = c;
	return 1;
    };
    return 0;
}

/* prints char on USART */
void uart_putchar_wait(unsigned char c) {
    while (U1STAbits.UTXBF) {
	if (U1STAbits.OERR)
	    U1STAbits.OERR = 0;
    }
    U1TXREG = c;
}

void uart_puts_rom(const char *s) {
    char c;
    while ((c = *s++)) {
	uart_putchar_wait(c);
    }
}

void uart_print_hex_wait(unsigned char c) {
    unsigned char nibble;
    nibble = ((c & 0xf0) >> 4) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    uart_putchar_wait(nibble);

    nibble = (c & 0x0f) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    uart_putchar_wait(nibble);
}

void print_hex_fifo(char c, struct serial_buffer_t *fifo) {
    unsigned char nibble;
    nibble = ((c & 0xf0) >> 4) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    putchar_fifo(nibble, fifo);

    nibble = (c & 0x0f) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    putchar_fifo(nibble, fifo);
}

void print_debug_value(char c, unsigned char value) {
    uart_putchar_wait(c);
    uart_putchar_wait(':');
    uart_print_hex_wait(value);
}

bool getchar(unsigned char *c) {
    if (U1STAbits.OERR)
	U1STAbits.OERR = 0;

    if (U1STAbits.URXDA) {
	*c = U1RXREG;
	return (true);
    }
    return (false);
}

void print_debug_fifo(struct serial_buffer_t *fifo) {
    print_debug_value('S', 255);
    uart_putchar_wait(' ');
    print_debug_value('M', SERIAL_BUFFER_SIZE_MASK);
    uart_putchar_wait(' ');
    print_debug_value('H',fifo->head);
    uart_putchar_wait(' ');
    print_debug_value('T',fifo->tail);
    uart_putchar_wait(' ');
//    puts_rom(sData);
/*    for (i=0; i<SERIAL_BUFFER_SIZE; i++) {
        print_hex_wait(fifo->data[i]);
        uart_putchar_wait(' ');
    }*/
    uart_putchar_wait('\r');
    uart_putchar_wait('\n');
}

/* put next char onto USART */
char fifo_uart_putchar(struct serial_buffer_t *fifo) {
    unsigned char tail;
    tail = fifo->tail;
    print_debug_fifo(fifo);
    if (fifo->head != tail) {
	tail++;

	uart_putchar_wait('f');
	print_debug_value('T', fifo->tail);
	uart_putchar_wait(' ');
	print_debug_value('T', tail);
	uart_putchar_wait('\r');
	uart_putchar_wait('\n');

	if (uart_putchar(fifo->data[tail])) {
	    fifo->tail = tail;
	    return 1;
	}
    }
    return 0;
}

/* print into circular buffer */
char print_rom_fifo(const char *s, struct serial_buffer_t *fifo) {
    unsigned char head = fifo->head;
    char c;
    while ((c = *s++)) {
	head++;
	if (head != fifo->tail)		/* space left ? */
	    fifo->data[head] = c;
	else
	    return -1;
    }
    fifo->head = head;		/* only store new pointer if all is OK */
    return 1;
}

/* put next char onto USART */
char fifo_putchar(struct serial_buffer_t *fifo) {
    unsigned char tail;
    tail = fifo->tail;
    if (fifo->head != tail) {
	tail++;
	if (uart_putchar(fifo->data[tail])) {
	    fifo->tail = tail;
	    return 1;
	}
    }
    return 0;
}

char copy_char_fifo(struct serial_buffer_t *source_fifo, struct serial_buffer_t *destination_fifo) {
    unsigned char source_tail;
    unsigned char destination_head;
    source_tail = source_fifo->tail;
    /* check if there is nothing todo first
     * because this will happen most of the time */
    if (source_tail == source_fifo->head) {
	return 0;
    } else {
	destination_head = destination_fifo->head;
	destination_head++;
	while ((destination_head != destination_fifo->tail) && (source_tail != source_fifo->head)) {
	    source_tail++;
	    destination_fifo->data[destination_head] = source_fifo->data[source_tail];
	    destination_fifo->head = destination_head;	/* store new pointer destination head before increment */
	    destination_head++;
	}
	source_fifo->tail = source_tail;	/* store new pointer source tail */
	return source_fifo->data[source_tail];
    }
}
