/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "usart.h"

const char * sData = " Data: ";

void init_usart (void) {
    // USART configuration
    TRISCbits.TRISC6 = 0;	// make the TX pin a digital output
    TRISCbits.TRISC7 = 1;	// make the RX pin a digital input
    TXSTA1bits.BRGH = USE_BRGH;		// high speed
    BAUDCON1bits.BRG16 = USE_BRG16; 	// 8-bit baud rate generator
    SPBRG = SBRG_VAL;		// calculated by defines
 
    PIE1bits.RC1IE = 0;		// disbale RX interrupt
    PIE1bits.TX1IE = 0;		// disbale TX interrupt
 
    RCSTA1bits.RX9  = 0;	// 8-bit reception
    RCSTA1bits.SPEN = 1;	// enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RCSTA1bits.CREN = 1;	// enable receiver

    TXSTA1bits.TX9  = 0;	// 8-bit transmission
    TXSTA1bits.SYNC = 0;	// asynchronous mode
    TXSTA1bits.TXEN = 1;	// transmit enabled

    /* don' use interrupts at the moment 
    // interrupts / USART interrupts configuration
    RCONbits.IPEN   = 0; // disable interrupt priority
    INTCONbits.GIE  = 1; // enable interrupts
    INTCONbits.PEIE = 1; // enable peripheral interrupts.
    PIE1bits.RCIE   = 1; // enable USART receive interrupt
    PIE1bits.TXIE   = 0; // disable USART TX interrupt
    PIR1bits.RCIF = 0;
    */
}

/* prints char on USART if pssible */
char putchar(unsigned char c) {
    if ( TXSTA1bits.TRMT1 ) {
	TXREG1 = c;
        return 1;
    }
    return 0;
}

/* prints char on USART */
void putchar_wait(unsigned char c) {
   while (!TXSTA1bits.TRMT1);
   TXREG1 = c;
}

void puts_rom(const char *s) {
    char c;
    while ( ( c = *s++ ) ) {
	putchar_wait( c );
    }
}

void print_hex_wait(unsigned char c) {
    unsigned char nibble;
    nibble=((c & 0xf0) >> 4 ) + '0';
    if (nibble>=0x3a) nibble+=7;
    putchar_wait(nibble);

    nibble=(c & 0x0f) + '0';
    if (nibble>=0x3a) nibble+=7;
    putchar_wait(nibble);
}

void print_debug_value(char c, unsigned char value) {
    putchar_wait(c);
    putchar_wait(':');
    print_hex_wait(value);
}

void print_debug_fifo(struct serial_buffer_t * fifo) {
    unsigned char i;
    print_debug_value('S',SERIAL_BUFFER_SIZE);
    putchar_wait(' ');
    print_debug_value('M',SERIAL_BUFFER_SIZE_MASK);
    putchar_wait(' ');
//    print_debug_value('H',fifo->head);
//    putchar_wait(' ');
//    print_debug_value('T',fifo->tail);
//    putchar_wait(' ');
//    puts_rom(sData);
/*    for (i=0; i<SERIAL_BUFFER_SIZE; i++) {
        print_hex_wait(fifo->data[i]);
        putchar_wait(' ');
    }*/
    putchar_wait('\r');
    putchar_wait('\n');
}


/* put next char onto USART */
char fifo_putchar(struct serial_buffer_t * fifo) {
    unsigned char tail;
    tail=fifo->tail;
    print_debug_fifo(fifo);
    if (fifo->head != tail) {
	tail++;
	tail &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */

	putchar_wait('f');
	print_debug_value('T',fifo->tail);
	putchar_wait(' ');
	print_debug_value('T',tail);
	putchar_wait('\r');
	putchar_wait('\n');

	if (putchar(fifo->data[tail])) {
	    fifo->tail=tail;
	    return 1;
	}
    }
    return 0;
}

/* print into circular buffer */
char print_rom_fifo(const unsigned char * s, struct serial_buffer_t * fifo) {
    unsigned char head=fifo->head;
    char c;
    while ( ( c = *s++ ) ) {
	head++;
	head&=SERIAL_BUFFER_SIZE_MASK;		/* wrap around if neededd */
	if (head != fifo->tail) {		/* space left ? */ 
	    fifo->data[head]=c;
	} else {
	    return -1;
	}
    }
    fifo->head=head;				/* only store new pointer if all is OK */
    return 1;
}

