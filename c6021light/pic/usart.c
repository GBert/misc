/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "usart.h"

const char *sData = " Data: ";
extern struct serial_buffer rx_fifo, tx_fifo;

/* prints char on USART if possible */
char putchar(unsigned char c) {
    if (TRMT) {
	TXREG = c;
	return 1;
    }
    return 0;
}

/* prints char on USART */
void putchar_wait(unsigned char c) {
    while (!TRMT) ;
    TXREG = c;
}

void puts_rom(const char *s) {
    char c;
    while ((c = *s++)) {
	putchar_wait(c);
    }
}

void print_hex_wait(unsigned char c) {
    unsigned char nibble;
    nibble = ((c & 0xf0) >> 4) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    putchar_wait(nibble);

    nibble = (c & 0x0f) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    putchar_wait(nibble);
}

void print_debug_value(char c, unsigned char value) {
    putchar_wait(c);
    putchar_wait(':');
    print_hex_wait(value);
}

void print_debug_fifo(struct serial_buffer *fifo) {
    unsigned char tail;
    tail = fifo->tail;
    print_debug_value('S', SERIAL_BUFFER_SIZE);
    putchar_wait(' ');
    print_debug_value('M', SERIAL_BUFFER_SIZE_MASK);
    putchar_wait(' ');
    putchar_wait('\r');
    putchar_wait('\n');
}

/* put next char onto USART */
char fifo_putchar(struct serial_buffer *fifo) {
    unsigned char tail;
    tail = fifo->tail;
    // print_debug_fifo(fifo);
    if (fifo->head != tail) {
	tail++;
	tail &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */

	if (putchar(fifo->data[tail])) {
	    fifo->tail = tail;
	    return 1;
	}
    }
    return 0;
}

/* place char into fifo */
char putchar_fifo(char c, struct serial_buffer *fifo) {
    unsigned char head;
    head = fifo->head;
    head++;
    head &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
    if (head != fifo->tail) {
        fifo->head = head;
        fifo->data[head] = c;
        return 1;
    };
    return 0;
}

/* simplfied for ISR */
void putc(char c) {
    unsigned char head;
    head = rx_fifo.head;
    head++;
    head &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
    if (head != rx_fifo.tail) {
        rx_fifo.head = head;
        rx_fifo.data[head] = c;
    };
}

char getchar_fifo(struct serial_buffer *fifo) {
    unsigned char tail;
    char c;
    tail = fifo->tail;
    if (tail != fifo->head) {
        tail++;
        tail &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
	fifo->tail = tail;
	c = fifo->data[tail];
	return c;
    }
    return 0;
}

/* simplified for ISR */
char getc() {
    unsigned char tail;
    tail = tx_fifo.tail;
    if (tail != tx_fifo.head) {
        tail++;
        tail &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
	tx_fifo.tail = tail;
	return tx_fifo.data[tail];
    }
    return 0;
}

/* print into circular buffer */
char print_rom_fifo(const unsigned char *s, struct serial_buffer *fifo) {
    unsigned char head = fifo->head;
    char c;
    while ((c = *s++)) {
	head++;
	head &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
	if (head != fifo->tail) {		/* space left ? */
	    fifo->data[head] = c;
	} else {
	    return -1;
	}
    }
    fifo->head = head;		/* only store new pointer if all is OK */
    return 1;
}
