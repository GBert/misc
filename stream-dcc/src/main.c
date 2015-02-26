#include <pic16f1704.h>
 #include <stdint.h>

static __code uint16_t __at (_CONFIG1) configword1 = _FOSC_INTOSC & _WDTE_OFF & _MCLRE_ON & _CP_OFF & _PWRTE_OFF & _BOREN_OFF & _CLKOUTEN_OFF;
static __code uint16_t __at (_CONFIG2) configword2 = _WRT_OFF & _PLLEN_ON & _STVREN_ON & _LVP_ON;

#define FOSC    32000000

void init() {
  // switch off analog
  ANSELA  = 0;
  ANSELC  = 0;
  ADCON0  = 0;
  ADCON1  = 0;
  ADCON2  = 0;
  CM1CON0 = 0;
  CM1CON1 = 0;
  CM2CON0 = 0;
  CM2CON1 = 0;
  TRISA5 = 0;     // output for red led     - pin2
}

void main() {
  init();
  while(1) {
    LATA5 = 1;
    LATA5 = 1;  // add 3 CPU cycles to generate symetric signal
    LATA5 = 1;  //
    LATA5 = 1;  //
    LATA5 = 0;
  }
}
