/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"
#include "i2c_lcd.h"
#include "lcd.h"

static __code uint16_t __at(_CONFIG1) configword1 = _FOSC_INTOSC & _PLLEN_OFF & _MCLRE_ON & _WDTE_OFF;
static __code uint16_t __at(_CONFIG2) configword2 = _LVP_ON & _CLKOUTEN_OFF;

// #pragma config FOSC=INTOSC, PLLEN=OFF, MCLRE=ON, WDTE=OFF
// #pragma config LVP=ON, CLKOUTEN=OFF

struct serial_buffer tx_fifo, rx_fifo;

union adc_value_u {
    uint16_t c;
    uint8_t bt[2];
};

volatile union adc_value_u adc_value;

volatile uint8_t adc_channel;
volatile uint8_t rail_data;
volatile uint8_t timer0_counter;

volatile uint16_t adc_poti;
volatile uint16_t adc_temperature;
volatile uint16_t adc_v;
volatile uint16_t adc_sense_left;
volatile uint16_t adc_sense_right;

volatile uint16_t adc_sense_left_average;
volatile uint16_t adc_sense_right_average;

volatile uint16_t adc_sense_left_count;
volatile uint16_t adc_sense_right_count;

void isr(void) __interrupt(0) {
    LATB7 = 1;
    if (IOCIF) {
	TMR2 = 0;
	// according to 40001729C.pdf it is needed to set post and pre scaler if TMR2 is modified
	T2CON = 0x3f; // FOSC/4 Postscaler 1:8 Prescaler 1:64 -> 1/8 * 8 * 64 * 256 = 16384us
	// enable H-Bridge
	LATB4 = 1;
	rail_data = 1;
	if ((adc_channel < 2) && (TMR4ON == 0)) {
	    //LATB7 = 0;
	    //LATB7 = 1;
	    TMR4 = 0;
	    PR4 = 8 * 20;	// FOSC/4 * 20 = 20us
	    TMR4IE = 1;
	    TMR4ON = 1;
	}
	IOCCF = 0;
    }
    if (TMR2IF) {
	// we got a 16ms timeout
	// disable H-Bridge
	LATB4 = 0;
	rail_data = 0;
	// left and right leg are off
	adc_sense_left_average = 0;
	adc_sense_right_average = 0;

	if ((adc_channel < 2) && (TMR4ON == 0)) {
	    TMR4 = 0;
	    PR4 = 8 * 30;	// FOSC/4 * 30 = 30us
	    TMR4IE = 1;
	    TMR4ON = 1;
	}
	TMR2IF = 0;
    }
    if (ADIE && ADIF) {
	ADCON0 = 0;
	ADCON2 = 0;
	adc_value.bt[0] = ADRESL;
	adc_value.bt[1] = ADRESH;
	TMR4ON = 0;
	TMR4IF = 0;
	LATC7 = 0;
	switch (adc_channel) {
	// sense left or right
	case 0:
	case 1:
	    if (PORTC & 0b00001000) {
		adc_sense_left_count++;
		adc_sense_left += adc_value.c;
		if (adc_sense_left_count > 63) {
		    adc_sense_left_count = 0;
		    adc_sense_left_average = adc_sense_left;
		    adc_sense_left = 0;
		}
	    } else {
		adc_sense_right_count++;
		adc_sense_right += adc_value.c;
		if (adc_sense_right_count > 63) {
		    adc_sense_right_count = 0;
		    adc_sense_right_average = adc_sense_right;
		    adc_sense_right = 0;
		}
	    }
	    break;
	// poti
	case 2:
	    adc_poti += adc_value.c;
	    adc_poti >>= 1;
	    break;
	// power supply
	case 3:
	    adc_v += adc_value.c;
	    adc_v >>=1;
	    break;
	// temperature
	case 4:
	    adc_temperature += adc_value.c;
	    adc_temperature >>=1;
	    break;
	}
	adc_channel++;
	if (adc_channel > 4) {	// only channel 0 - 4
	   if (rail_data)
		adc_channel = 0;
	   else
		adc_channel = 2;
	}

	if (adc_channel > 1) {
	    TMR4 = 0;
	    PR4 = 8 * 30;	// FOSC/4 * 30 = 30us
	    TMR4IE = 1;
	    TMR4ON = 1;
	}
	ADIF = 0;
    }
    if (TMR0IF && TMR0IE) {
	TMR0 = TIMER0_VAL;
	timer0_counter++;
	TMR0IF = 0;
    }
    if (TMR4IF && TMR4IE) {
	LATC7 = 1;
	TMR4ON = 0;
	switch (adc_channel) {
	case 0:
	case 1:
	    ADCON0 = (AD_SENSE << 2) | 1;
	    break;
	case 2:
	    ADCON0 = (AD_POTI << 2) | 1;
	    break;
	case 3:
	    ADCON0 = (AD_V << 2) | 1;
	    break;
	case 4:
	    ADCON0 = (AD_TEMPERATURE << 2) | 1;
	    break;
	default:
	    adc_channel = 0;
	    break;
	}
	LATC7 = 0;
	LATC7 = 1;
	// we need to wait Tacq = 5us - use Timer4 again w/o interrupt to trigger ADC
	TMR4IF = 0;
	TMR4IE = 0;
	TMR4 = 0;
	PR4 = 8 * 5;	// FOSC/4 * 5 = 5us
	TMR4ON = 1;
	// ADC will start automatically after Timer4 match
	ADCON2 = 0b11000000;
    }
    LATB7 = 0;
}

/* RA4 SDA I2C
 * RA5 SCL I2C
 * RC1 RxD UART
 * RC2 TxD UART
 * RC3 Rail Signal
 * RB5 RPWM
 * RB6 LPWM
 */

void pps_init(void) {
    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 0;		// unlock PPS
    /* I2C */
    SSPCLKPPS = 0x05;
    RA5PPS = 0b10000;		// RA5 output SCL
    SSPDATPPS = 0x04;
    RA4PPS = 0b10001;		// RA4 output SDA
    /* USART */
    RXPPS = 0b10001;		// input  EUSART RX -> RC1
    RC2PPS = 0b10100;		// RC2 output TX/CK
    /* CLC */
    /* RB4PPS = 0b00100;		// LC1OUT -> ENABLE */
    /* COG */
    COGINPPS = 0b10011;		// RC3 PULSE
    RB5PPS = 0b01001;		// COG1B -> RPWM
    RB6PPS = 0b01000;		// COG1A -> LPWM

    PPSLOCK = 0x55;
    PPSLOCK = 0xaa;
    PPSLOCK = 1;		// lock PPS
}

void system_init(void) {
    // switch off analog
    OSCCON = 0b11110000;	// Configure oscillator
    //         1------- use PLL to get 4x8 Mhz (system clock)
    //         -1110--- 8 MHz internal oscillator (instruction clock)
    //         ------00 oscillator selected with INTOSC
    ANSELA = 0;
    ANSELB = 0;
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
    /* USART */
    TRISC1 = 1;
    TRISC2 = 0;
    /* RA2&RC0 analog input */
    TRISA2 = 1;
    TRISB5 = 0;
    TRISB6 = 0;
    TRISC0 = 1;
    TRISC6 = 1;
    TRISC3 = 1;		/* Rail Data */
    TRISB4 = 0;		/* Enable */
    TRISC5 = 0;		/* LED */
    // debug
    TRISB7 = 0;
    TRISC7 = 0;
    // setup interrupt events
    //clear all relevant interrupt flags
    SSP1IF = 0;
    TMR1IF = 0;
    CCP1IF = 0;
    // activate IOC on Rail Data
    IOCCP3 = 1;
    IOCCN3 = 1;
    //activate interrupt bits
    IOCIE = 1;
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
    /* RA2&RC0&RC6 analog */
    ANSELA = 1 << 2;
    ANSELC = 0b01000001;
    /* right justified ; FOSC/32 ;VREF- GND & VREF+ VDD */
    ADCON1 = 0b10100000;
    ADIE = 1;
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

void timer2_init(void) {
    // default (FOSC/4)
    T2CON = 0b00111111;
    //        -0111--- postscaler 1:8
    //        -----1-- timer on
    //        ------11 prescaler 1:64 (overflow every 16ms)
    TMR2 = 0;			// reset timer2
    TMR2IE = 1;
}

void clc_init(void) {
    CLC1SEL0 = 0;
    CLC1SEL1 = 0;

    //LC1OE = 1;
    //LC1EN = 1;
}

void cog_init(void) {
    COG1CON0 = 0;
    COG1CON1 = 0;

    COG1RIS = 0x1;	/* COG1PPS rising event		*/
    COG1RSIM = 0x0;	/* no rising delay		*/
    COG1FIS = 0x1;	/* COG1PPS falling event	*/
    COG1FSIM = 0x0;	/* no falling delay		*/

    COG1ASD0 = 0;	/* don't use shutdown		*/
    COG1ASD1 = 0;

    COG1STR = 0;	/* don't use steering control	*/
    COG1DBR = 0;	/* no dead band			*/
    COG1DBF = 0;
    COG1BLKR = 0;	/* don't use blanking		*/
    COG1BLKF = 0;
    COG1PHR = 0;	/* normal phase			*/
    COG1PHF = 0;

    COG1CON0 = 0b10001100;
	      /* 1-------	G1EN COG1 Enable
		 -0------	G1LD0 no buffer
		 ---01---	G1CS Fosc clock source
		 -----100	G1MD COG Half Bridge mode */

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
    uint8_t temp1l_left, temp1h_left, temp1l_right, temp1h_right, temp2;
    uint16_t leg_left, leg_right;
    //uint16_t ad_value;

    pps_init();
    system_init();
    uart_init();
    i2c_init();
    ad_init();
    timer0_init();
    timer2_init();
    cog_init();

    /* empty circular buffers */
    tx_fifo.head = 0;
    tx_fifo.tail = 0;
    rx_fifo.head = 0;
    rx_fifo.tail = 0;

    rail_data = 0;
    adc_channel = 0;
    GIE = 1;
    LCD_init(LCD_01_ADDRESS);

    while (1) {
	if (counter == 0) {
	    /* 14mA per digit / atomic read */
	    GIE = 0;
	    leg_left = adc_sense_left_average;
	    leg_right = adc_sense_right_average;
	    temp2 = adc_poti;
	    //temp2 = adc_temperature >> 2;
	    GIE = 1;

	    leg_left = leg_left >> 6;
	    leg_right = leg_right >> 6;
	    temp1l_left = leg_left & 0xff;
	    temp1h_left = leg_left >> 8;
	    temp1l_right = leg_right & 0xff ;
	    temp1h_right= leg_right >> 8;
	    temp2 = adc_poti >> 2;

	    LCD_putcmd(LCD_01_ADDRESS, LCD_CLEAR, 1);
	    LCD_puts(LCD_01_ADDRESS, "Booster Max=8.0A\0");
	    LCD_goto(LCD_01_ADDRESS, 2, 1);
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1h_right));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1l_right >> 4));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1l_right));
	    LCD_putch(LCD_01_ADDRESS, 0x20);
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1h_left));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1l_left >> 4));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp1l_left));
	    LCD_putch(LCD_01_ADDRESS, 0x20);
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp2 >> 4));
	    LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp2));
	    if (rail_data)
		LCD_puts(LCD_01_ADDRESS, " 0.0%\0");
	    else
		LCD_puts(LCD_01_ADDRESS, " Off\0");
	    putchar_wait(0x55);
	    LATC5 ^= 1;
	}
	delay_ms(2);

	counter++;
    }
}
