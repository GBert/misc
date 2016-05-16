/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "main.h"


// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF)
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT)

void init_io(void) {
	/* ADC1 Digital Mode */
	/* AD1PCFGL = 0xFFFF; */

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
	LATAbits.LATA0 = 0;
}

void init_pps(void) {
	__builtin_write_OSCCONL(OSCCON & ~(1<<6));
	RPINR18bits.U1RXR = 34;
        RPOR0bits.RP35R = 1;	/* U1TXR */
	__builtin_write_OSCCONL(OSCCON | (1<<6));
}


int main(void) {
	/* Init Clock */
	PLLFBD = PLL_DIV;
	CLKDIVbits.PLLPOST = PLL_POST;
	CLKDIVbits.PLLPRE = PLL_PRE;

	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(OSCCON | 0x01);

	while (OSCCONbits.COSC != 0x03);
	while (OSCCONbits.LOCK != 1);

	/* Init PORT I/O */
	init_io();

	/* Blink Forever */
	while (true) {
		/* __builtin_btg((unsigned int*)&LATA, 0); */
		blink();
		__delay_us(10);
	}
}
