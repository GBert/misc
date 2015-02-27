#include<xc.h>

#pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
#pragma config LVP=ON, CLKOUTEN=OFF

#define FOSC    32000000

void system_init() {
  // switch off analog
  OSCCON = 0b11111000; // Configure oscillator
           //1------- use PLL to get 32Mhz (system clock)
           //-1110--- 8 MHz internal oscillator (insctruction clock)
           //------00 Oscillator selected with FOSC
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

void timer_init() {
  T1CON = 0b00110000;
          //00------ FOSC/4 as counting source
          //--11---- prescaler 1:8 (counting every us)
  T1GCONbits.TMR1GE = 0; // timer is not controlled by gate.
  TMR1H = 0; // reset timer1 High
  TMR1L = 0; // and Low bytes - prescaler automatic reset
  CCP1CON = 0b00000010; // set up capture and compare
            //----0010 Toggle CCP1 pin. Set CCP1IF (TODO check if CCP1IF is set)
            // set ccp1 register to the highest value to avoid useless interrupt
  CCPR1H = 0xFF;
  CCPR1L = 0xFF;
}

void main() {
  system_init();
  timer_init();
  // 4 MHZ 
  while(1) {
    LATA5 = 1;
    LATA5 = 1;  // add 3 CPU cycles to generate symetric signal
    LATA5 = 1;  //
    LATA5 = 1;  //
    LATA5 = 0;
  }
}
