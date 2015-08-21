#include<xc.h>

#pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
#pragma config LVP=ON, CLKOUTEN=OFF

#define _XTAL_FREQ	32000000
#define FCYC		(_XTAL_FREQ/4L) // target device instruction clock freqency

// USART calculating Baud Rate Generator
// if BRGH = 0 => FOSC/[64 (n + 1)]
// if BRGH = 1 => FOSC/[16 (n + 1)]
// avoid rounding errors

#define BAUDRATE        57600
#define USE_BRG16       0
#define USE_BRGH        1

#if USE_BRGH == 0
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

void pps_init(void) {
  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCK = 0;                // unlock PPS
  // set UASRT : TX on RA0 , RX on RA1
  RXPPS  = 0b00001;           // input  EUSART RX -> RA1
  RA0PPS = 0b00110;           // RA0 output TX/CK

  PPSLOCK = 0x55;
  PPSLOCK = 0xaa;
  PPSLOCK = 1;                // lock PPS
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
  TXSTAbits.TX9  = 1;         // 8-bit transmission
  TXSTAbits.TX9D = 1;         //  one extra stop bit
  TXSTAbits.TXEN = 1;         // transmit enabled
  TXSTAbits.SYNC = 0;         // asynchronous mode
  TXSTAbits.BRGH = 1;         // high speed
  RCSTAbits.SPEN = 1;         // enable serial port (configures RX/DT and TX/CK pins as serial port pins)
  RCSTAbits.RX9  = 0;         // 8-bit reception
  RCSTAbits.CREN = 1;         // enable receiver
  BAUDCON1bits.BRG16 = USE_BRG16; // 8-bit baud rate generator

  SPBRG = SBRG_VAL;           // calculated by defines

  TRISAbits.TRISA0 = 0;       // make the TX pin a digital output
  TRISAbits.TRISA1 = 1;       // make the RX pin a digital input

  PIR1bits.RCIF = 0;
}


void timer_init() {
  T1CON = 0b00110000;
          //00------ FOSC/4 as counting source
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

void main() {
  system_init();
  pps_init();
  uart_init();
  timer_init();
  // Fcyc 8 MHZ
  // 8 cycles per loop -> 1MHz square wave
  while(1) {
    LATA5 = 1;
    LATA5 = 1;  // add 3 CPU cycles to generate symetric signal
    LATA5 = 1;  //
    LATA5 = 1;  //
    LATA5 = 0;
  }
}
