#include <xc.h>
#include "main.h"

#pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
#pragma config LVP=ON, CLKOUTEN=OFF

#define _XTAL_FREQ	32000000
#define FCYC		(_XTAL_FREQ/4L) // target device instruction clock freqency

// USART calculating Baud Rate Generator
// if BRGH = 0 => FOSC/[64 (n + 1)]
// if BRGH = 1 => FOSC/[16 (n + 1)]
// avoid rounding errors

#define BAUDRATE        115200
#define USE_BRG16       0
#define USE_BRGH        1

#if USE_BRGH == 0
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

// timer interval in microseconds
#define INTERVAL	50
#define TIMER0_VAL	(256 - (INTERVAL-2))
#define S88BITS		16

#define S88_DATA_PIN	TRISA2
#define S88_DATA	PORTAbits.RA2
#define S88_CLOCK_PIN	TRISC0
#define S88_CLOCK	LATC0
#define S88_PS_PIN	TRISC1
#define S88_PS		LATC1
#define S88_RESET_PIN	TRISC2
#define S88_RESET	LATC2

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

volatile unsigned char s88_state;
volatile unsigned char s88_bits;
volatile unsigned char s88_data_bit;

void interrupt ISR(void) {
  if(T0IE && T0IF) {
    T0IF=0;
    TMR0 = TIMER0_VAL;
    LATA5 ^= 1;
    // FSM
    switch (s88_state) {
    case STATE_START:
      S88_PS = 1;
      s88_state = STATE_PS_H;
      s88_bits = S88BITS;
      break;
    case STATE_PS_H:
      S88_CLOCK = 1;
      s88_state = STATE_CLOCK_LOAD_H;
      break;
    case STATE_CLOCK_LOAD_H:
      S88_CLOCK = 0;
      s88_state = STATE_CLOCK_LOAD_L;
      break;
    case STATE_CLOCK_LOAD_L:
      S88_RESET = 1;
      s88_state = STATE_RESET_H;
      break;
    case STATE_RESET_H:
      S88_RESET = 0;
      s88_state = STATE_RESET_L;
      break;
    case STATE_RESET_L:
      S88_PS = 0;
      s88_state = STATE_PS_L;
      break;
    case STATE_PS_L:
      S88_CLOCK = 1;
      s88_state = STATE_CLOCK_H;
      break;
    case STATE_CLOCK_H:
      s88_data_bit = S88_DATA;
      S88_CLOCK = 0;
      s88_state = STATE_CLOCK_L;
      break;
    case STATE_CLOCK_L:
      if (s88_bits) {
        s88_bits--;
        S88_CLOCK = 1;
        s88_state = STATE_CLOCK_H;
      } else {
        // S88 cyle finished - TODO do something with data
        s88_state = STATE_START;
      }
      break;
    default:
      s88_state = STATE_START;
      break;
    }
  }
}

void pps_init(void) {
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCK = 0;		// unlock PPS
  // set USART : RX on RA0 , TX on RA1 / 40001729B.pdf page 141
  RXPPS  = 0b00000;	// input  EUSART RX -> RA0
  RA1PPS = 0b10100;	// RA1 output TX/CK
  RA4PPS = 0b01100;	// RA4 output CCP1

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
  TRISC5 = 0;  // CCP1
  // setup interrupt events
  //clear all relevant interrupt flags
  PIR1bits.SSP1IF = 0;
  PIR1bits.TMR1IF = 0;
  PIR1bits.CCP1IF = 0;
  //activate interrupt bits
  PIE1bits.CCP1IE = 0; // disable interrupt on CCP1 will be check by polling as of today
  PIE1bits.SSP1IE = 1;
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

void timer0_init() {
  OPTION_REGbits.TMR0CS = 0;	// FOSC / 4
  OPTION_REGbits.PSA = 0;	// use prescaler
  OPTION_REGbits.PS = 0b010;	// prescaler 1:8
  TMR0 = TIMER0_VAL;
  T0IE = 1;
}


void timer1_init() {
  T1CON = 0b01110000;
          //01------ FOSC as counting source
          //--11---- prescaler 1:8 (counting every us)
  T1GCONbits.TMR1GE = 0; // timer is not controlled by gate.
  TMR1H = 0; // reset timer1 high
  TMR1L = 0; // and low bytes - prescaler automatic reset
  CCP1CON = 0b00000010; // set up capture and compare
            //----0010 Toggle CCP1 pin. Set CCP1IF (TODO check if CCP1IF is set)
            // set ccp1 register to the highest value to avoid useless interrupt
  CCPR1H = 0xFF;
  CCPR1L = 0xFF;
}

void timer2_init() {
  // default (FOSC/4)
  T2CON = 0b00000100;
          //-----1-- timer on
          //------00 prescaler 1:1 (overflow every 32us)
  TMR2 = 0; // reset timer2
}

void s88_init() {
  S88_PS_PIN = 0;
  S88_RESET_PIN = 0;
  S88_CLOCK_PIN = 0;
  S88_DATA_PIN = 1;
}

void main() {
  unsigned short counter=0;
  pps_init();
  system_init();
  uart_init();
  s88_init();
  timer0_init();
  GIE = 1;
  s88_state = STATE_START;
  while(1) {
    if ( counter == 0 )
	putchar_wait(0x55);
    counter++;
  }
}
