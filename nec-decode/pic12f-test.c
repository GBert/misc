#include <pic12f1572.h>
#include <stdint.h>

static __code uint16_t __at (_CONFIG1) configword1 = _FOSC_INTOSC & _WDTE_OFF & _MCLRE_ON & _CP_OFF & _PWRTE_OFF & _BOREN_OFF & _CLKOUTEN_OFF;
static __code uint16_t __at (_CONFIG2) configword2 = _WRT_OFF & _PLLEN_ON & _STVREN_ON & _DEBUG_OFF & _LVP_OFF;

volatile uint32_t nec_code;

void init() {
  INTCON  = 0;	// Clear interrupt flag bits
  GIE     = 1;	// Global irq enable
  IOCIE   = 1;	// interrupt on change enable
  IOCAN2  = 1;	// irq on negative edge (RA2/pin5)
  IOCAP2  = 1;	// irq on positive edge (RA2/pin5)
  // switch off analog
  ANSELA  = 0;
  ADCON0  = 0;
  ADCON1  = 0;
  ADCON2  = 0;
  CM1CON0 = 0;
  CM1CON1 = 0;

  IRCF3   = 1;	// 8MHz internal oscillator x 4 PLL -> 32 MHz
  IRCF2   = 1;	// "
  IRCF1   = 1;	// "
  IRCF0   = 0;	// "

  TRISA5  = 0;	// output for red led     - pin2
  TRISA2  = 1;	// input  for ir receiver - pin5

  TMR1CS0 = 0;	// 8MHz (Fosc/4 -> 32MHz/4 ; timer 1 use internal clock)
  TMR1CS1 = 0;  // "

  T1CKPS0 = 0;  // prescaler 1:2
  T1CKPS1 = 1;  // "

  TMR1ON  = 1;  // Timer on
}

void isr (void) __interrupt (1){
  GIE = 0;
  if(IOCIF) {	// IOC?
    IOCAF = 0;
  }
  if(INTF) {
    TMR1H = 0;
    TMR1L = 0;
  }
  GIE = 1;
}

void main() {
  init();

  nec_code |= 0x80000000;
  nec_code >>= 1;

  while(1){
    // should toggle with 4 MHz within loop at Fosc = 32 MHz
    LATA5 = 0;
    LATA5 = 1;
    LATA5 = 0;
    LATA5 = 1;
    LATA5 = 0;
    LATA5 = 1;
    LATA5 = 0;
    LATA5 = 1;
  }
}
