/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <xc.h>

#define _XTAL_FREQ 32000000	// This is the speed your controller is running at
#define FCYC (_XTAL_FREQ/4L)	// target device instruction clock freqency

#define LED_TRIS        (TRISAbits.TRISA0)
#define LED             (LATAbits.LATA0)

/* BUFFER_SIZE must be power of two (16,32,64...) */
#define SERIAL_BUFFER_SIZE      32
#define SERIAL_BUFFER_SIZE_MASK (SERIAL_BUFFER_SIZE -1)

void interrupt ISRCode(void);
char putchar(unsigned char c);
void putchar_wait(unsigned char c);
void puts_rom(const char *c);
void init_usart(void);
char fifo_putchar(struct serial_buffer_t * fifo);
char print_rom_fifo(const unsigned char * s, struct serial_buffer_t *fifo);
void print_debug_value(char c, unsigned char value);
void print_debug_fifo(struct serial_buffer_t * fifo);

/* circular buffer */
typedef struct serial_buffer_t {
    unsigned char head;
    unsigned char tail;
    unsigned char data[SERIAL_BUFFER_SIZE];
};

#endif /* _MAIN_H_ */

