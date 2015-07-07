#include <xc.h>

#pragma config FOSC = INTOSC, PLLEN = OFF, MCLRE = OFF, WDTE = OFF, LVP = OFF

#define _XTAL_FREQ	32000000	// This is the speed your controller is running at
#define FCYC (_XTAL_FREQ/4L)		// target device instruction clock freqency

#define LED             (LATCbits.LATC0)

/* USART calculating Baud Rate Generator
 * if BRGH = 0 => FOSC/[64 (n + 1)]
 * if BRGH = 1 => FOSC/[16 (n + 1)]
 * avoid rounding errors
 */ 

#define BAUDRATE	57600
#define USE_BRG16	0
#define USE_BRGH	1

#if USE_BRGH == 0
#define	SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define	SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

void interrupt ISRCode();

int i = 0;
volatile unsigned char timer_ticks=0;

void init_osc(void) {
    OSCCONbits.IRCF   = 0b1110;
    OSCCONbits.SPLLEN = 0b1;
}

void init_port(void) {
    ANSELC = 0x0;		// Default all pins to digital
    TRISC = 0;
}

void init_uart (void) {
    // USART configuration
    TXSTAbits.TX9  = 0;    // 8-bit transmission
    TXSTAbits.TXEN = 1;    // transmit enabled
    TXSTAbits.SYNC = 0;    // asynchronous mode
    TXSTAbits.BRGH = 1;    // high speed
    RCSTAbits.SPEN = 1;    // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RCSTAbits.RX9  = 0;    // 8-bit reception
    RCSTAbits.CREN = USE_BRGH;    // enable receiver
    BAUDCON1bits.BRG16 = USE_BRG16; // 8-bit baud rate generator

    SPBRG = SBRG_VAL;      // calculated by defines
    
    // TRISCbits.TRISC6 = 0;     // make the TX pin a digital output
    // TRISCbits.TRISC7 = 1;     // make the RX pin a digital input
    
    /* don' use interrupts at the moment 
    // interrupts / USART interrupts configuration
    RCONbits.IPEN   = 0; // disable interrupt priority
    INTCONbits.GIE  = 1; // enable interrupts
    INTCONbits.PEIE = 1; // enable peripheral interrupts.
    PIE1bits.RCIE   = 1; // enable USART receive interrupt
    PIE1bits.TXIE   = 0; // disable USART TX interrupt
    */
    PIR1bits.RCIF = 0;
}

void putchar(char ch) {
  //Wait for TXREG Buffer to become available
  while(!TXIF);
  TXREG=ch;
}

void puts(const char *str) {
  while((*str)!='\0') {
    //Wait for TXREG Buffer to become available
    while(!TXIF);
    //Write data
    TXREG=(*str);
    //next char
    str++;
  }
}

void puthex(char data) {
  char hextemp = data;
  data >>= 4;
  data &= 0x0f;
  data += '0';
  if (data > '9')
    data += 7;
  putchar(data);
  hextemp &= 0x0f;
  hextemp += '0';
  if (hextemp > '9')
    hextemp += 7;
  putchar(hextemp);
}

void main(int argc, char** argv) {
    init_osc();
    init_port();
    init_uart();

    //infinite loop
    while(1) {
        __delay_ms(10);
	LED = 0;
        __delay_ms(10);
	LED = 1;
	putchar('U');
    }
}

