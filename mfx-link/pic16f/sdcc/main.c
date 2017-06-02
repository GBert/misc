/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <pic16f1705.h>
#include "main.h"
#include "i2c_lcd.h"
#include "lcd.h"

static __code uint16_t __at(_CONFIG1) configword1 = _FOSC_INTOSC & _PLLEN_OFF & _MCLRE_ON & _WDTE_OFF;
static __code uint16_t __at(_CONFIG2) configword2 = _LVP_ON & _CLKOUTEN_OFF;

// #pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
// #pragma config LVP=ON, CLKOUTEN=OFF

  /* DCC    58us */
  /* MM1    26us */
  /* MM2    52us */
  /* mfx    50us */

struct serial_buffer tx_fifo, rx_fifo;

volatile unsigned int pulse_high = 25;
volatile unsigned int pulse_low = 75;
volatile uint8_t timer0_counter;
volatile uint16_t adc_poti;
volatile uint16_t adc_sense;

void isr(void) __interrupt(0) {
    if (CCP1IF) {
	CCP1IF = 0;
	if (CCP1M0) {
	    CCP1M0 = 0;
	    CCPR1 += pulse_high;
	} else {
	    CCP1M0 = 1;
	    CCPR1 += pulse_low;
	}
    }
    if (TMR0IF && TMR0IE) {
	TMR0IF = 0;
	TMR0 = TIMER0_VAL;
	LATC4 ^= 1;

	timer0_counter++;
	/* kind of state machine
	   to sample two pins */
	if (timer0_counter & 0x02) {
	    if (timer0_counter & 0x01) {
		ADCON0 = (AD_POTI << 2) | 1;
		ADGO = 1;
	    /* check if ADC completed) */
	    } else if (!ADGO) {
		adc_poti += ADRESH;
		adc_poti >>= 1;
	    }
	} else {
	    if (timer0_counter & 0x01) {
		ADCON0 = (AD_SENSE << 2) | 1;
		ADGO = 1;
	    /* check if ADC completed) */
	    } else if (!ADGO) {
		adc_sense += ADRESH;
		adc_sense >>= 1;
	    }
	}
    }
}

/* RA4 SDA I2C
 * RA5 SCL I2C
 * RC1 RxD UART
 * RC2 TxD UART
 * // RC3 CCP1
 */

void pps_init(void) {
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 0;		// unlock PPS
    // set USART : RX on RC1 , TX on RA1 / 40001729B.pdf page 141
    RXPPS = 0b10001;		// input  EUSART RX -> RC1
    RC2PPS = 0b10100;		// RC2 output TX/CK
    /* I2C */
    SSPCLKPPS = 0x05;
    RA5PPS = 0b10000;		// RA5 output SCL
    SSPDATPPS = 0x04;
    RA4PPS = 0b10001;		// RA4 output SDA
    /* CCP1 - for standalone DCC/MM/mfx encoder */
    // RC3PPS = 0b01100

    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 1;		// lock PPS
}

void system_init(void) {
    // switch off analog
    OSCCON = 0b11110000;	// Configure oscillator
    //1------- use PLL to get 4x8 Mhz (system clock)
    //-1110--- 8 MHz internal oscillator (instruction clock)
    //------00 oscillator selected with INTOSC
    ANSELA = 0;
    ANSELC = 0;
    ADCON0 = 0;
    ADCON1 = 0;
    ADCON2 = 0;
    CM1CON0 = 0;
    CM1CON1 = 0;
    CM2CON0 = 0;
    CM2CON1 = 0;

    /* I2C MSSP 40001729B.pdf page 302 */
    TRISA4 = 1;
    TRISA5 = 1;
    /*
       TRISC2 = 0;
     */
    /* USART */
    TRISC0 = 0;
    TRISC1 = 1;
    /* RA2&RC0 analog input */
    TRISA2 = 1;
    TRISC0 = 1;
    TRISC4 = 0;			/* Enable */
    TRISC5 = 0;			/* LED */
    // setup interrupt events
    //clear all relevant interrupt flags
    SSP1IF = 0;
    TMR1IF = 0;
    CCP1IF = 0;
    //activate interrupt bits
    CCP1IE = 1;			// disable interrupt on CCP1 will be check by polling as of today
    PEIE = 1;
    GIE = 1;
}

void i2c_init(void) {
    /* Initialise I2C MSSP Master */
    SSP1CON1 = 0b00101000;
    SSP1CON2 = 0x00;
    SSP1CON3 = 0x00;
    /* Master at 100KHz */
    /* clock = FOSC/(4 * (SSPADD + 1)) */
    SSP1ADD = SSP1ADD_VAL;
    /* Slew rate disabled */
    SSP1STAT = 0b11000000;
}

void ad_init(void) {
    /* RA2&RC0 analog */
    ANSELA = 1 << 2;
    ANSELC = 1 << 0;
    /* right justified ; FOSC/64 ;VREF- GND & VREF+ VDD */
    ADCON1 = 0b01100000;
    /* RA2 analog input */
    ADCON0 = AD_POTI;
    /* RC0 (AN4) analog input */
    /* ADCON0 = AD_SENSE; */
}

void uart_init(void) {
    TX9 = 1;			// 8-bit transmission
    TX9D = 1;			//  one extra stop bit
    TXEN = 1;			// transmit enabled
    SYNC = 0;			// asynchronous mode
    BRGH = 1;			// high speed
    SPEN = 1;			// enable serial port (configures RX/DT and TX/CK pins as serial port pins)
    RX9 = 0;			// 8-bit reception
    CREN = 1;			// enable receiver
    BRG16 = USE_BRG16;		// 8-bit baud rate generator

    SPBRG = SBRG_VAL;		// calculated by defines

    RCIF = 0;
}

void timer0_init(void) {
    TMR0CS = 0;			// FOSC / 4
    PSA = 0;			// use prescaler
    PS1 = 1;			// prescaler 1:8
    TMR0 = TIMER0_VAL;
    TMR0IE = 1;
}

void timer1_init(void) {
    T1CON = 0b00110001;
    //00------ FOSC/4 as counting source
    //--11---- prescaler 1:8 (counting every us)
    //-------1 timer on
    TMR1GE = 0;			// timer is not controlled by gate.
    TMR1H = 0;			// reset timer1 high
    TMR1L = 0;			// and low bytes - prescaler automatic reset
    CCP1CON = 0b00001000;	// set up capture and compare
    //----1000   Compare mode: set output on compare match (set CCPxIF)
    // set ccp1 register to the highest value to avoid useless interrupt
    CCPR1H = 0xFF;
    CCPR1L = 0xFF;
    // don't use yet
    // CCP1IE = 1;
}

void timer2_init(void) {
    // default (FOSC/4)
    T2CON = 0b00000100;
    //-----1-- timer on
    //------00 prescaler 1:1 (overflow every 32us)
    TMR2 = 0;			// reset timer2
}

/* Instructions per millisecond. */
#define INSNS_PER_MS    (XTAL_FREQ / 8000U)
/* Delay loop is about 10 cycles per iteration. */
#define LOOPS_PER_MS    (INSNS_PER_MS / 10U)
void delay_ms(uint16_t ms) {
    uint16_t u;
    while (ms--) {
	/* Inner loop takes about 10 cycles per iteration + 4 cycles setup. */
	for (u = 0; u < LOOPS_PER_MS; u++) {
	    /* Prevent this loop from being optimized away. */
	    __asm nop __endasm;
	}
    }
}

uint8_t ad(uint8_t channel) {
    ADCON0 = (channel << 2) | 1;
    ADGO = 1;
    delay_ms(1);
    while (ADGO) ;
    return (ADRESH);
}

char nibble_to_hex(uint8_t c) {
    char nibble;
    nibble = (c & 0x0f) + '0';
    if (nibble >= 0x3a)
	nibble += 7;
    return (nibble);
}

void main(void) {
    uint8_t counter = 0;
    uint8_t temp;
    uint16_t ad_value;

    pps_init();
    system_init();
    uart_init();
    i2c_init();
    ad_init();
    timer0_init();
    //timer1_init();

    /* empty circular buffers */
    tx_fifo.head = 0;
    tx_fifo.tail = 0;
    rx_fifo.head = 0;
    rx_fifo.tail = 0;

    GIE = 1;
    LCD_init(LCD_01_ADDRESS);

    while (1) {
	if (counter == 0) {
	    // temp = ad(AD_SENSE);
	    /* 14mA per digit */
	    // GIE = 0;
	    ad_value = adc_sense * 14;
	    temp = adc_poti;
	    // GIE = 1;
	    LCD_putcmd(LCD_01_ADDRESS, LCD_CLEAR, 1);
	    LCD_puts(LCD_01_ADDRESS, "Booster Max=8.0A\0");
	    LCD_goto(LCD_01_ADDRESS, 2, 1);
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp >> 4));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp));
	    LCD_puts(LCD_01_ADDRESS, "  On      0.0%\0");
	    //LATCbits.LATC0 = 1;
	    //LATCbits.LATC0 ^= 1;
	    putchar_wait(0x55);
	}
	delay_ms(4);

	counter++;
    }
}
