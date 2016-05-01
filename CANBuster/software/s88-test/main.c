/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */


/* S88 Test Connections
   RA2 Data Output
   RC0 RESET -> ISR
   RC1 LOAD  -> ISR
   RC2 CLOCK -> ISR
 */

#include <xc.h>
#include "main.h"

#pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
#pragma config LVP=ON, CLKOUTEN=OFF

struct serial_buffer_t tx_fifo, rx_fifo;

void interrupt ISR(void) {
  if (IOCIF) {
    IOCAF = 0;
  }
}

void pps_init(void) {
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCK = 0;		// unlock PPS
  // set USART : RX on RA0 , TX on RA1 / 40001729B.pdf page 141
  RXPPS  = 0b00000;	// input  EUSART RX -> RA0
  RA1PPS = 0b10100;	// RA1 output TX/CK

  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCK = 1;		// lock PPS
}

void system_init() {
  // switch off analog
  OSCCON = 0b11110000; // Configure oscillator
           //1------- use PLL to get 4x8 Mhz (system clock)
           //-1110--- 8 MHz internal oscillator (instruction clock)
           //------00 oscillator selected with INTOSC
  ANSELA  = 0;
  ANSELC  = 0;
  ADCON0  = 0;
  ADCON1  = 0;
  ADCON2  = 0;
  CM1CON0 = 0;
  CM1CON1 = 0;
  CM2CON0 = 0;
  CM2CON1 = 0;
  TRISA5 = 0;
  TRISA4 = 0;
  TRISC5 = 0;  // CCP1
  // setup interrupt events
  //clear all relevant interrupt flags
  PIR1bits.SSP1IF = 0;
  PIR1bits.TMR1IF = 0;
  PIR1bits.CCP1IF = 0;
  //activate interrupt bits
  PIE1bits.CCP1IE = 0; // disable interrupt on CCP1 will be check by polling as of today
  PIE1bits.SSP1IE = 0;
  INTCONbits.PEIE = 1;
  INTCONbits.GIE = 1;
}

void uart_init (void) {
  TXSTAbits.TX9  = 1;		// 8-bit transmission
  TXSTAbits.TX9D = 1;		//  one extra stop bit
  TXSTAbits.TXEN = 1;		// transmit enabled
  TXSTAbits.SYNC = 0;		// asynchronous mode
  TXSTAbits.BRGH = 1;		// high speed
  RCSTAbits.SPEN = 1;		// enable serial port (configures RX/DT and TX/CK pins as serial port pins)
  RCSTAbits.RX9  = 0;		// 8-bit reception
  RCSTAbits.CREN = 1;		// enable receiver
  BAUDCON1bits.BRG16 = USE_BRG16; // 8-bit baud rate generator

  SPBRG = SBRG_VAL;		// calculated by defines

  // TRISAbits.TRISA0 = 1;	// make the TX pin a digital output
  // TRISAbits.TRISA1 = 0;	// make the RX pin a digital input

  PIR1bits.RCIF = 0;
}

void pio_init(void) {
  TRISA2 = 0;
  TRISC0 = 1;
  TRISC1 = 1;
  TRISC2 = 1;
}

void timer0_init() {
  OPTION_REGbits.TMR0CS = 0;	// FOSC / 4
  OPTION_REGbits.PSA = 0;	// use prescaler
  OPTION_REGbits.PS = 0b010;	// prescaler 1:8
  TMR0 = TIMER0_VAL;
  T0IE = 1;
}

void main() {
  unsigned short counter=0;

  pps_init();
  system_init();
  uart_init();

  /* empty circular buffers */
  tx_fifo.head=0;
  tx_fifo.tail=0;
  rx_fifo.head=0;
  rx_fifo.tail=0;

  GIE = 1;
  while(1) {
    if ( counter == 0 )
      putchar_wait(0x55);
    counter++;
  }
}
