/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _USART_H_
#define _USART_H_

#include "main.h"

/* circular serial buffer */

typedef struct serial_buffer_t {
    unsigned char head;
    unsigned char tail;
    unsigned char data[SERIAL_BUFFER_SIZE];
};

char putchar(unsigned char c);
void putchar_wait(unsigned char c);
void print_hex_wait(unsigned char c);
void puts_rom(const char *s);
void print_debug_value(char c, unsigned char value);
char putchar_fifo(char c, struct serial_buffer_t * fifo);
char fifo_getchar(struct serial_buffer_t * fifo);
char fifo_putchar(struct serial_buffer_t * fifo);
char print_rom_fifo(const unsigned char * s, struct serial_buffer_t * fifo);

#endif		/* _USART_H_ */
