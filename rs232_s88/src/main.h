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

#define _XTAL_FREQ      32000000
#define FCYC            (_XTAL_FREQ/4L) // target device instruction clock freqency

// USART calculating Baud Rate Generator
// if BRGH = 0 => FOSC/[64 (n + 1)]
// if BRGH = 1 => FOSC/[16 (n + 1)]
// avoid rounding errors

#define BAUDRATE        115200
#define USE_BRG16       0
#define USE_BRGH        1

#if USE_BRGH == 0
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

/* BUFFER_SIZE must be power of two (16,32,64...) */
#define SERIAL_BUFFER_SIZE      32
#define SERIAL_BUFFER_SIZE_MASK (SERIAL_BUFFER_SIZE -1)

// timer interval in microseconds
#define INTERVAL        50
#define TIMER0_VAL      (256 - (INTERVAL-2))
#define S88BITS         16

#define S88_DATA_PIN    TRISA2
#define S88_DATA        PORTAbits.RA2
#define S88_CLOCK_PIN   TRISC0
#define S88_CLOCK       LATC0
#define S88_PS_PIN      TRISC1
#define S88_PS          LATC1
#define S88_RESET_PIN   TRISC2
#define S88_RESET       LATC2

enum s88_fsm_state { STATE_START = 0,
        STATE_PS_H,
        STATE_CLOCK_LOAD_H,
        STATE_CLOCK_LOAD_L,
        STATE_RESET_H,
        STATE_RESET_L,
        STATE_PS_L,
        STATE_CLOCK_H,
        STATE_CLOCK_L,
};

void interrupt ISR(void);
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
