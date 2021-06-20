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

#include <pic16f1709.h>
#include <stdint.h>

#define LCD_01_ADDRESS  0x27
#define AD_POTI		2
#define AD_SENSE	4
#define AD_V		8
#define AD_TEMPERATURE	29	// 0b11101

#define _XTAL_FREQ	32000000
#define XTAL_FREQ	(_XTAL_FREQ)
#define FCYC		(_XTAL_FREQ/4L) // target device instruction clock freqency

#define I2C_BAUDRATE	100000

#define BAUDRATE	115200
#define USE_BRG16	0
#define USE_BRGH	1

// I2C baud rate calculation
// SCL pin clock period = ((ADD<7:0> + 1) *4)/FOSC

#define SSP1ADD_VAL	( (((_XTAL_FREQ / I2C_BAUDRATE) / 2) - 1) / 2)

// USART calculating Baud Rate Generator
// if BRGH = 0 => FOSC/[64 (n + 1)]
// if BRGH = 1 => FOSC/[16 (n + 1)]
// avoid rounding errors

#if USE_BRGH == 0
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

/* BUFFER_SIZE must be power of two (16,32,64...) */
#define SERIAL_BUFFER_SIZE      32
#define SERIAL_BUFFER_SIZE_MASK (SERIAL_BUFFER_SIZE -1)

// timer interval in 32 microseconds steps
#define INTERVAL        250
#define TIMER0_VAL      (255 - (INTERVAL-1))

/* circular buffer */
struct serial_buffer {
    unsigned char head;
    unsigned char tail;
    unsigned char data[SERIAL_BUFFER_SIZE];
};

void delay_ms(uint16_t ms);
char putchar(unsigned char c);
void putchar_wait(unsigned char c);
void puts_rom(const char *c);
void init_usart(void);
char fifo_putchar(struct serial_buffer *fifo);
char print_rom_fifo(const unsigned char * s, struct serial_buffer *fifo);
void print_debug_value(char c, unsigned char value);
void print_debug_fifo(struct serial_buffer *fifo);

#endif /* _MAIN_H_ */
