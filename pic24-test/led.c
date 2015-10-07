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

#include "led.h"

_FOSCSEL(FNOSC_PRIPLL & IESO_OFF)
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_HS)
_FWDT(FWDTEN_OFF)

void
init_io(void)
{
	/* ADC1 Digital Mode */
	/* AD1PCFGL = 0xFFFF; */

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
	LATAbits.LATA0 = 0;
}

int
main(void)
{
	/* Init Clock */
	CLKDIVbits.PLLPRE = PLL_PRE;
	PLLFBD = PLL_DIV;
	CLKDIVbits.PLLPOST = PLL_POST;

	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);

	while (!OSCCONbits.COSC);
	while (!OSCCONbits.LOCK);

	/* Init PORT I/O */
	init_io();

	/* Blink Forever */
	while (true) {
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
		__builtin_btg((unsigned int*)&LATA, 0);
	}
}
