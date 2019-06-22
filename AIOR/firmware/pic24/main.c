/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"

#pragma config JTAGEN   = OFF
#pragma config FNOSC    = PRIPLL
#pragma config IESO     = OFF
#pragma config FCKSM    = CSECMD
#pragma config OSCIOFNC = ON
#pragma config POSCMD   = XT

/* code for Output Compare 1 ISR*/
void __attribute__((__interrupt__, no_auto_psv)) _OC1Interrupt( void ) {
    /* Clear OC1 interrupt flag} */
    IFS0bits.OC1IF = 0;
}

/* code for Output Compare 2 ISR*/
void __attribute__((__interrupt__, no_auto_psv)) _OC2Interrupt( void ) {
    /* Clear OC2 interrupt flag} */
    IFS0bits.OC2IF = 0;
}

void init_io(void) {
    /* ADC1 Digital Mode */
    // AD1CON1.bits = 0x8000;
    //_ADON = 0;
    //_CVR1OE = 0;
    CM1CON = 0;
    CM2CON = 0;
    CM3CON = 0;
    CM5CON = 0;
    ANSELA = 0;
    ANSELB = 0;
    // AD1PCFG = 0xFFFF;

    /* RB9 LED */
    TRISB = 0;
}

void init_pps(void) {
    __builtin_write_OSCCONL(OSCCON & ~(1 << 6));
    RPINR18bits.U1RXR = 34;     /* PIN 23 RB2 RPI34 U1RxD */
    RPOR0bits.RP35R   =  1;     /* PIN 22 RB3 RP35  U1TxD (U1TX = 0b0000001) */
    RPINR26bits.C1RXR = 55;     /* PIN 3  RC7 RP55 CAN1 RxD */
    RPOR6bits.RP54R   = 14;     /* PIN 2  RC6 RP54 CAN1 TxD (C1TX = 0b001110) */
    RPINR19bits.U2RXR = 42;     /* PIN 8  RB10 RP42 U2RxD */
    RPOR4bits.RP42R   =  3;     /* PIN 9  RB11 RP43 U2TxD (U2TX = 0b0000011) */
    RPOR2bits.RP39R   = 16;	/* PIN 43 RB7  RP39 B_Left  (OC1 = 0b00010000) */
    RPOR3bits.RP40R   = 17;	/* PIN 44 RB8  RP40 B_Right (OC2 = 0b00010001) */
				/* PIN 42 RB6  RP38 B_Ena */
   __builtin_write_OSCCONL(OSCCON | (1 << 6));
}

int main(void) {
    /* Init Clock */
    PLLFBD = PLL_DIV;
    CLKDIVbits.PLLPOST = PLL_POST;
    CLKDIVbits.PLLPRE = PLL_PRE;

    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 3) ;
    while (OSCCONbits.LOCK != 1) ;

    init_io();

    while (true) {
	__delay_ms(500);
	__builtin_btg((unsigned int *)&LATB, 9);
    }
}
