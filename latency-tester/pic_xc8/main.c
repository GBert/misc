#include <xc.h>

#pragma config FOSC = INTOSC, PLLEN = OFF, MCLRE = OFF, LVP = OFF
#pragma config WRT = OFF, WDTCPS = WDTCPS1F, WDTE = OFF, WDTCWS = WDTCWSSW, WDTCCS = SWC

#define _XTAL_FREQ	32000000	// This is the speed the controller is running at
#define FCYC		(_XTAL_FREQ/4L)	// target device instruction clock freqency

#define LED             LATCbits.LATC0

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


/*  PIN13 RA0/ICSPDAT -> TX
 *  PIN12 RA1/ICSPCLK -> RX
 *  PIN10 RC0 LED
 */

void interrupt ISRCode();

int i = 0;
volatile unsigned char timer_ticks=0;

void init_osc(void) {
    OSCCONbits.IRCF   = 0b1110;
    OSCCONbits.SPLLEN = 0b1;
}

void init_port(void) {
    ANSELA = 0x0;		// Default all pins to digital
    ANSELC = 0x0;		// Default all pins to digital
    TRISC = 0;
}

void init_pps(void) {
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 0;	/* unlock PPS */
    /* set UASRT : TX on RA0 , RX on RA1 */ 
    RXPPS  = 0b00001;	/* input  EUSART RX -> RA1 */
    RA0PPS = 0b00110;	/* output TX/CK */
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 1;	/* lock PPS */
}

void init_uart (void) {
    TXSTAbits.TX9  = 0;    // 8-bit transmission
    TXSTAbits.TXEN = 1;    // transmit enabled
    TXSTAbits.SYNC = 0;    // asynchronous mode
    TXSTAbits.BRGH = 1;    // high speed
    RCSTAbits.SPEN = 1;    // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RCSTAbits.RX9  = 0;    // 8-bit reception
    RCSTAbits.CREN = 1;    // enable receiver
    BAUDCON1bits.BRG16 = USE_BRG16; // 8-bit baud rate generator

    SPBRG = SBRG_VAL;      // calculated by defines
    
    TRISAbits.TRISA0 = 0;     // make the TX pin a digital output
    TRISAbits.TRISA1 = 1;     // make the RX pin a digital input
    
    PIR1bits.RCIF = 0;
}

void init_smt (void) {
    SMT1CON0 = 0b10000000;              	// STM Enable
    SMT1CON1 = 0b01000010;              	// Single Acquisition
    SMT1SIG = 0b00000001;               	// Comp1 Signal
    SMT1CLK = 0b00000000;               	// clock input FOSC/4

    SMT1TMR = 0x000000;                 	// Clear all Counter Registers
    SMT1CPR = 0x000000;
    SMT1CPW = 0x000000;
    SMT1PR = 0x000000;
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

void puthex(unsigned char data) {
  unsigned char hextemp = data;
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

void main(void) {
    GIE = 0;
    init_osc();
    init_port();
    init_pps();
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

