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
#include "usart.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF)
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT)

struct serial_buffer_t tx_fifo, rx_fifo;

void init_io(void) {
    /* ADC1 Digital Mode */
    /* AD1PCFGL = 0xFFFF; */

    /* RA0 O/P */
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 0;

    TRISBbits.TRISB2 = 1;	/* Pin 6 as input pin for UART RxD */
    TRISBbits.TRISB3 = 0;	/* Pin 7 as output pin for UART TxD */
}

void init_pps(void) {
    __builtin_write_OSCCONL(OSCCON & ~(1 << 6));
    RPINR18bits.U1RXR = 34;
    RPOR0bits.RP35R = 1;	/* U1TXR */
    __builtin_write_OSCCONL(OSCCON | (1 << 6));
}

int main(void) {
    int counter = 0;
    /* Init Clock */
    PLLFBD = PLL_DIV;
    CLKDIVbits.PLLPOST = PLL_POST;
    CLKDIVbits.PLLPRE = PLL_PRE;

    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC != 0x03) ;
    while (OSCCONbits.LOCK != 1) ;

    tx_fifo.head=0;
    tx_fifo.tail=0;
    rx_fifo.head=0;
    rx_fifo.tail=0;

    /* Init PORT I/O */
    init_io();
    init_pps();
    init_uart();

    while (true) {
	counter++;
	__builtin_btg((unsigned int *)&LATA, 0);
	fifo_putchar(&tx_fifo);
	if (counter == 50000) {
	    print_rom_fifo("Hello dsPIC33 !\r\n", &tx_fifo);
	    counter = 0;
	}
	__delay_us(10);
    }
}
