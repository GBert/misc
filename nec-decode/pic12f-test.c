#include <pic12f1572.h>
#include <stdint.h>

static __code uint16_t __at (_CONFIG1) configword1 = _FOSC_INTOSC & _WDTE_OFF & _MCLRE_ON & _CP_OFF & _PWRTE_OFF & _BOREN_OFF & _CLKOUTEN_OFF;
static __code uint16_t __at (_CONFIG2) configword2 = _WRT_OFF & _PLLEN_ON & _STVREN_ON & _DEBUG_OFF & _LVP_OFF;

enum nec_state { STATE_INACTIVE,
        STATE_HEADER_SPACE,
        STATE_BIT_PULSE,
        STATE_BIT_SPACE,
        STATE_TRAILER_PULSE,
        STATE_TRAILER_SPACE
};

volatile uint32_t nec_code;
volatile uint32_t new_nec_code;
volatile uint8_t ir_nec_decode_state;
volatile uint8_t ir_nec_decode_bits;
volatile uint8_t ir_nec_data_valid;
volatile uint8_t stopwatch;

//IR decode defines
#define NEC_NBITS		32
#define NEC_UNIT		562500 /* ns */
#define NEC_HEADER_PULSE	(16 * NEC_UNIT)
#define NECX_HEADER_PULSE	(8 * NEC_UNIT) /* Less common NEC variant */
#define NEC_HEADER_SPACE	(8 * NEC_UNIT)
#define NEC_REPEAT_SPACE	(4 * NEC_UNIT)
#define NEC_BIT_PULSE		(1 * NEC_UNIT)
#define NEC_BIT_0_SPACE		(1 * NEC_UNIT)
#define NEC_BIT_1_SPACE		(3 * NEC_UNIT)
#define NEC_TRAILER_PULSE	(1 * NEC_UNIT)
#define NEC_TRAILER_SPACE	(10 * NEC_UNIT) /* even longer in reality */
#define NECX_REPEAT_BITS	1

// USART defines
#define _XTAL_FREQ 32000000     // This is the speed your controller is running at
#define FCYC (_XTAL_FREQ/4L)    // target device instruction clock freqency

#ifndef BAUDRATE
#define BAUDRATE        57600
#endif
#define USE_BRG16       0
#define USE_BRGH        1

/* USART calculating Baud Rate Generator
* if BRGH = 0 => FOSC/[64 (n + 1)]
* if BRGH = 1 => FOSC/[16 (n + 1)]
* avoid rounding errors
*/

#if USE_BRGH == 0
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define SBRG_VAL        ( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif

void init_usart (void) {
  // USART configuration
  //TXSTA REG
  TXEN=1;
  BRGH=USE_BRGH;
  BRG16=USE_BRG16;
  SPBRG=SBRG_VAL;

  //RCSTA
  SPEN=1;
  //CREN=1;	//Enable Receiver (RX)

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

    //Next goto char
    str++;
  }
}

void puthex(uint8_t data) {
  uint8_t hextemp = data;
  data &= 0xf0;
  data >>= 4;
  data += 0x30;
  if (data > 0x39)
    data += 7;
  putchar(data);
  hextemp &= 0x0f;
  hextemp += 0x30;
  if (hextemp > 0x39)
    hextemp += 7;
  putchar(hextemp);
}

void init() {
  INTCON  = 0;	// Clear interrupt flag bits
  GIE     = 1;	// Global irq enable
  PEIE    = 1;  // enable Periphiral Interrupt for Timer1
  TMR1IE  = 1;  // enebale Timer1 overflow interrupt
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

  TRISA0  = 0;	// output for UART TX     - pin7
  TRISA5  = 0;	// output for red led     - pin2
  TRISA2  = 1;	// input  for ir receiver - pin5

  TMR1CS0 = 0;	// 8MHz (Fosc/4 -> 32MHz/4 ; timer 1 use internal clock)
  TMR1CS1 = 0;  // "

  T1CKPS0 = 1;  // prescaler 1:2
  T1CKPS1 = 0;  // "

  TMR1ON  = 1;  // Timer on
}

void isr (void) __interrupt (1){
  GIE = 0;
  if(IOCIF) {	// IOC?
    IOCAF = 0;
    // we only use the high byte
    stopwatch = TMR1H;
    TMR1L = 0;
    TMR1H = 0;
  }
  // overflow every 64us * 256 = 16.384ms
  if(TMR1IF) {
  // TODO: overflow - stop timer and restart on pin change ?
    stopwatch = 255;
    TMR1IF = 0;
    // LATA ^= 0x20;
  }

  LATA5 = 1;
  // NEC IR decode FSM
  // 3.14t idea - it's fast enough so we can use it in the ISR

  switch (ir_nec_decode_state) {

  case STATE_INACTIVE:
    if ((stopwatch > 125 ) && (stopwatch < 156))
      ir_nec_decode_state = STATE_HEADER_SPACE;
    goto END_OF_INTERRUPT;

  case STATE_HEADER_SPACE:
    if ((stopwatch > 62 ) && (stopwatch < 78)) {
      // we got the start sequence -> old data is now invalid
      ir_nec_data_valid = 0;
      ir_nec_decode_bits = 0;
      ir_nec_decode_state = STATE_BIT_PULSE;
      // is this a repeat sequence ?
    } else if ((stopwatch > 30 ) && (stopwatch < 41)) {
      // if ir_nec_decode_bits == 32 the repeat sequence could be valid
      ir_nec_decode_state = STATE_TRAILER_PULSE;
    } else
       break;
    goto END_OF_INTERRUPT;

  case STATE_BIT_PULSE:
    if ((stopwatch > 6 ) && (stopwatch < 11))
      ir_nec_decode_state = STATE_BIT_SPACE;
    else
      break;
    goto END_OF_INTERRUPT;

  case STATE_BIT_SPACE:
    if ((stopwatch > 6 ) && (stopwatch < 11))
      nec_code >>=1;
    else if ((stopwatch > 22 ) && (stopwatch < 30)) {
      nec_code >>=1;
      nec_code |= 0x80000000;
    } else
      break;

    ir_nec_decode_bits++;

    if (ir_nec_decode_bits == NEC_NBITS)
       ir_nec_decode_state = STATE_TRAILER_PULSE;
    else
      ir_nec_decode_state = STATE_BIT_PULSE;
    goto END_OF_INTERRUPT;

  case STATE_TRAILER_PULSE:
    if ((stopwatch > 6 ) && (stopwatch < 11))
      ir_nec_decode_state = STATE_TRAILER_SPACE;
    else
      break;
    goto END_OF_INTERRUPT;

  case STATE_TRAILER_SPACE:
    // 255 means timer overflow - we assume that this is the pause after a complete sequence
    if (stopwatch == 255) {
      ir_nec_decode_state = STATE_INACTIVE;
      // we got valid data if the sequence before was valid - needed for repeat
      if (ir_nec_decode_bits == NEC_NBITS)
        ir_nec_data_valid = 1;
      goto END_OF_INTERRUPT;
    }
  }
  // if something went wrong -> back to start
  ir_nec_decode_state = STATE_INACTIVE;

END_OF_INTERRUPT:
  GIE = 1;
  LATA5 = 0;
}

void main() {
  uint8_t *data;
  init();
  init_usart();
  ir_nec_data_valid=0;

  while(1){
    //LATA ^= 0x20;
    if (ir_nec_data_valid) {
      // atomic acces to vars
      GIE = 0;
      ir_nec_data_valid=0;
      new_nec_code = nec_code;
      // we got vars so back to normal 
      GIE = 1;
      puts("0x\0");
      // Aiiieeee : pointer mess
      data = (uint8_t *) &new_nec_code;
      puthex(*data++);
      puthex(*data++);
      puthex(*data++);
      puthex(*data);
      putchar('\n');
    }
  }
}
