/*------------------------------------------------------------------------------
;
;   Proton MKII MEGA DEMO for The Wellington Boot Loader
;
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   This file is part of The Wellington Boot Loader.
;
;   The Wellington Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Wellington Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
;-----------------------------------------------------------------------------*/

#include "proton-mkII.h"

/*****************************************************************************/

/*
 * Delay
 *
 *  WREG x 4TCY + CALL(2TCY) + RETURN(2TCY) + SDCC(9TCY)
 *
 *  Eg. For 50us with FCY = 10,000,000, WREG = 122
 *
 *  .000050 / (1 / 10000000) = 500 INSTRUCTIONS
 *  500 - 13 = 487		SUBTRACT CALL OVERHEAD
 *  487 / 4  = 121.75		DETERMINE LOOP COUNT
 */
void
delay_tcy(int wreg) __wparam
{
__asm
loop:
	ADDLW	-1		;1TCY
	BTFSS	_STATUS,_Z	;1TCY | 2TCY skip
	GOTO	loop		;2TCY
	NOP			;1TCY
__endasm ;
}

/*
 * Delay > 10us . t
 */
void
delay_10us(uint16_t t)
{
	uint8_t i;

	while (t--) {
		delay_tcy(DELAY10U);
	}
}

/*
 * Delay > t ms
 */
void
delay_ms(uint16_t t)
{
	uint8_t i;

	while (t--) {
		for (i = 0; i < 20; ++i) {
			delay_tcy(DELAY50U);
		}
	}
}

/*****************************************************************************/

/*
 * Init UART
 */
void
init_uart(void)
{
	uint8_t r;

	TRISCbits.RC7 = 1;	/* RC7 I/P Rx */
	TRISCbits.RC6 = 1;	/* RC6 I/P Tx */

	BAUDCON = _BRG16;	/* 16-bit Baudrate */

	SPBRGH = UART_BRG >> 8;	/* Baudrate */
	SPBRG  = UART_BRG & 0xFF;

#if UART_2STOP == 1
	/* 9-Bit Enable + Enable Transmit + High Speed Mode + 9th Bit High */
	TXSTA = _TX9 + _TXEN + _BRGH + _TX9D;
#else
	/* Enable Transmit + High Speed Mode */
	TXSTA = _TXEN + _BRGH;
#endif
	/* Enable Serial Port + Receiver */
	RCSTA = _SPEN + _CREN;

	r = RCREG;
	r = RCREG;
	r = RCREG;

#if UART_STDIO == 1
	stdout = STREAM_USART;	/* Assign STDOUT to UART */
#endif
}

/*****************************************************************************/

/*
 * Init Analog Input
 */
void
init_analog(void)
{
	TRISAbits.RA0 = 1;	/* RA0 INPUT               */
	ADCON1 = 0x0E;		/* VSS/VDD/AN0 ENABLED     */
	ADCON2 = 0x86;		/* RIGHT JUSTIFIED FOSC/64 */
	ADCON0 = _ADON;		/* A/D ENABLED             */
}

/*
 * Read Analog Input (10-bit)
 */
uint16_t
read_analog(void)
{
	uint16_t res;

	ADCON0 |= _GO;

	while (ADCON0 & _GO)
		;

	res = ADRESH << 8;
	res |= ADRESL;

	return res;
}

/*****************************************************************************/

/*
 * Function set (8-bit interface, 2 lines, 5*7 Pixels)       0x38
 * Function set (8-bit interface, 1 line, 5*7 Pixels)        0x30
 * Function set (4-bit interface, 2 lines, 5*7 Pixels)       0x28
 * Function set (4-bit interface, 1 line, 5*7 Pixels)        0x20
 * Entry mode set command (left to right)                    0x06 
 * Scroll display one character right (all lines)            0x1E
 * Scroll display one character left (all lines)             0x18
 * Home (move cursor to top/left character position)         0x02
 * Move cursor one character left                            0x10
 * Move cursor one character right                           0x14
 * Turn on visible underline cursor                          0x0E
 * Turn on visible blinking-block cursor                     0x0F
 * Make cursor invisible                                     0x0C
 * Blank the display (without clearing)                      0x08
 * Restore the display (with cursor hidden)                  0x0C
 * Clear Screen                                              0x01
 * Set cursor position (DDRAM address)                       0x80 + addr
 * Set pointer in character-generator RAM (CG RAM address)   0x40 + addr
 * Read DDRAM/CGRAM & Check Busy Flag                        N/A
 */

/*
 * Write to LCD Instruction Register
 */
void
lcd_instruction(uint8_t i)
{
	uint8_t temp;

	LATEbits.LATE0 = 0;	/* RS = 0 */

	temp = LATD;
	LATD = i;

	LATEbits.LATE1 = 1;	/* EN = 1 */
__asm
	NOP
	NOP
__endasm ;
	LATEbits.LATE1 = 0;	/* EN = 0 */

	if (i == 0x01 || i == 0x02)
		delay_ms(2);
	else
		delay_tcy(DELAY50U);

	LATD = temp;
}

/*
 * Write to LCD Data Register
 */
void
lcd_data(uint8_t d)
{
	uint8_t temp;

	LATEbits.LATE0 = 1;	/* RS = 1 */

	temp = LATD;
	LATD = d;

	LATEbits.LATE1 = 1;	/* EN = 1 */
__asm
	NOP
	NOP
__endasm ;
	LATEbits.LATE1 = 0;	/* EN = 0 */

	delay_tcy(DELAY50U);

	LATD = temp;
}

/*
 * Init LCD
 */
void
init_lcd(void)
{
	delay_ms(30);		/* Minimum power on delay */

	LATD = 0;		/* LCD DB0 .. DB7 = 0 */
	LATE = 0;		/* LCD RS = 0 LCD EN = 0 */

	TRISD = 0;		/* RD0 .. RD7 O/P LCD DB0 .. DB7 */
	TRISEbits.RE0 = 0;	/* RE0 O/P LCD RS */
	TRISEbits.RE1 = 0;	/* RE1 O/P LCD EN */

	/* Function set (8-bit interface, 2 lines, 5*7 Pixels) */
	lcd_instruction(0x38);

	/* Restore the display (with cursor hidden) */
	lcd_instruction(0x0C);

	/* Entry mode set command (left to right) */
	lcd_instruction(0x06);
}

/*
 * LCD puts()
 */
void
lcd_puts(char *s)
{
	uint8_t i = 0;
#if 0
	/* Clear Screen */
	lcd_instruction(0x01);
#else
	/* Home (move cursor to top/left character position) */
	lcd_instruction(0x02);
#endif
	/* Send string... */
	while (s[i])
		lcd_data(s[i++]);
}

/*****************************************************************************/

/*
 * Init CAN Bus
 */
void
init_canbus(void)
{
	/* Enable CANRX */
	TRISBbits.RB3 = 1;

	/* Enter configuration mode */
	CANCONbits.REQOP2 = 1;
	while (!(CANSTAT & _OPMODE2))
		;

#if CAN_ID_ALL == 1
	/* Set standard receive masks */
	RXM0SIDH = 0;        	/* For RXB0 with filters 0/1     */
	RXM0SIDL = 0;        
	RXM1SIDH = 0;		/* For RXB1 with filters 2/3/4/5 */
	RXM1SIDL = 0;

	/* Set standard receive filters for RXB0 */
	RXF0SIDH = 0;
	RXF0SIDL = 0;
	RXF1SIDH = 0;
	RXF1SIDL = 0;

	/* Set standard receive filters for RXB1 */
	RXF2SIDH = 0;
	RXF2SIDL = 0;
	RXF3SIDH = 0;
	RXF3SIDL = 0;
	RXF4SIDH = 0;
	RXF4SIDL = 0;
	RXF5SIDH = 0;
	RXF5SIDL = 0;
#else
	/* Set standard receive masks */
	RXM0SIDH = 0xFF;	/* For RXB0 with filters 0/1     */
	RXM0SIDL = 0xE0;
	RXM1SIDH = 0xFF;	/* For RXB1 with filters 2/3/4/5 */
	RXM1SIDL = 0xE0;

	/* Set standard receive filters for RXB0 */
	RXF0SIDH = (uint8_t)(CAN_ID >> 3);
	RXF0SIDL = (uint8_t)(CAN_ID << 5);
	RXF1SIDH = (uint8_t)(CAN_ID >> 3);
	RXF1SIDL = (uint8_t)(CAN_ID << 5);

	/* Set standard receive filters for RXB1 */
	RXF2SIDH = (uint8_t)(CAN_ID >> 3);
	RXF2SIDL = (uint8_t)(CAN_ID << 5);
	RXF3SIDH = (uint8_t)(CAN_ID >> 3);
	RXF3SIDL = (uint8_t)(CAN_ID << 5);
	RXF4SIDH = (uint8_t)(CAN_ID >> 3);
	RXF4SIDL = (uint8_t)(CAN_ID << 5);
	RXF5SIDH = (uint8_t)(CAN_ID >> 3);
	RXF5SIDL = (uint8_t)(CAN_ID << 5);
#endif
	/* Reset receivers */
	RXB0CONbits.RXBODBEN = CAN_DOUBLE_BUFFER_RX;
	RXB0CONbits.RXFUL = 0;
	RXB1CONbits.RXFUL = 0;

	/* Set bit rate */
	BRGCON1 = CAN_BRG1;
	BRGCON2 = CAN_BRG2;
	BRGCON3 = CAN_BRG3;

	/* Enable CANTX */
	CIOCONbits.ENDRHI = 1;

	/* Enter normal mode */
	CANCON = 0;
	while (CANSTAT & (_OPMODE2 | _OPMODE1))
		;
}

/*
 * Can gets() from bufer n
 */
int8_t
can_gets_rxn(uint8_t *buffer, uint16_t *message_id, uint8_t rxn)
{
	uint8_t *rxb = (uint8_t *)&RXB0D0;
	uint8_t i = 0,  blen;

	/* Set window */
	if (rxn == 0)
		CANCON = CAN_WIN_RXB0;
	else if (rxn == 1)
		CANCON = CAN_WIN_RXB1;
	else
		return -1;

	/* Receive data? */
	if (!(RXB0CON & _RXFUL))
		return -1;	/* RX NOT READY */

	/* Get message id */
	(*message_id) = (uint16_t)(RXB0SIDH << 3);
	(*message_id) |= (uint16_t)(RXB0SIDL >> 5);

	/* Get data length code */
	blen = RXB0DLC & 0x0F;

	/* Receive input data */
	while (i < blen) {
		buffer[i] = rxb[i];
		++i;
	}

	/* Reset receiver */
	RXB0CONbits.RXFUL = 0;

	return blen;
}

/*
 * Can gets()
 */
int8_t
can_gets(uint8_t *buffer, uint16_t *message_id)
{
	static uint8_t r = 0;
	int8_t blen, i;

	/* Receive message... */
	for (i = 0; i < CAN_RING_RX; ++i) {
		blen = can_gets_rxn(buffer, message_id, r++);
		r %= CAN_RING_RX;
		if (blen != -1)
			return blen;
	}
	return -1;
}

/*
 * Can puts() to buffer n
 */
int8_t
can_puts_txn(uint8_t *buffer, uint8_t blen, uint16_t message_id, uint8_t txn)
{
	uint8_t *txb = (uint8_t *)&RXB0D0;
	int i = 0;

	/* Set window */
	if (txn == 0)
		CANCON = CAN_WIN_TXB0;
	else if (txn == 1)
		CANCON = CAN_WIN_TXB1;
	else if (txn == 2)
		CANCON = CAN_WIN_TXB2;
	else
		return -1;

	/* Write data? */
	if (RXB0CON & _TXREQ)
		return -1;	/* TX IN USE */

	/* Set standard transmit message id */
	RXB0SIDH = (uint8_t)(message_id >> 3);
	RXB0SIDL = (uint8_t)(message_id << 5);

	/* Set data length code */
	RXB0DLC = blen & 0x0F;

	/* Store output data */
	while (i < blen) {
		txb[i] = buffer[i];
		++i;
	}

	/* Mark for transmission */
	RXB0CON |= _TXREQ;

	return blen;
}

/*
 * Can puts()
 */
int8_t
can_puts(uint8_t *buffer, int8_t blen, uint16_t message_id)
{
	static uint8_t r = 0;
	int8_t slen;

	while (1) {
		slen = can_puts_txn(buffer, blen, message_id, r++);
		r %= CAN_RING_TX;
		if (slen != -1)
			break;
	}

	/* >1ms message delay for PICAN */
	delay_ms(2);

	return slen;
}

/*****************************************************************************/

/*
 * Hello world! This is the MEGA DEMO 8-)
 */
void
main(void)
{
	uint8_t canbuf[CAN_LEN] = {0};
	int8_t canlen;
	uint16_t canid = 0, potval;

	init_uart();
	init_analog();
	init_canbus();
	init_lcd();

	/* Clear Screen */
        lcd_instruction(0x01);

	/* Obligatory message */
	lcd_puts("Hello world!");

	while (1) {
		/* Get CAN? */
		canlen = can_gets(canbuf, &canid);
		if (canlen >= 0) {
			/* Get Analog... */
			potval = read_analog();
			canbuf[0] = (uint8_t)(potval >> 8);
			canbuf[1] = (uint8_t)(potval);

			/* Send Can... */
			can_puts(canbuf, 2, canid);
		}
	}
}

/*****************************************************************************/
