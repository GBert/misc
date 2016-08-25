;************************************************************************/
;*                                                                      */
;*                      I2C Sniffer					*/
;*                                                                      */
;*              Author: Peter Dannegger                                 */
;*                                                                      */
;************************************************************************/
.nolist
.include "tn85def.inc"

.equ	TXD_PORT	= PORTB
.equ	TXD		= PB1			; must be OC1A !

.equ	I2C_PORT	= PORTB
.equ	SCL		= PB2			; must be USI !
.equ	SDA		= PB0

.equ	START	= 's'
.equ	ACK	= 'a'
.equ	NACK	= 'n'
.equ	STOP	= 'p'

.equ	XTAL		= 14745600
;.equ	BAUD		= 115200
;.equ	BAUD		= 230400
.equ	BAUD		= 460800
.equ	INVERT		= 1<<COM1A0		; inverted
;.equ	INVERT		= 0			; non inverted

.equ	TXD_DDR		= TXD_PORT - 1
.equ	I2C_DDR		= I2C_PORT - 1
.equ	I2C_PIN		= I2C_PORT - 2
;-------------------------------------------------------------------------
;				Register usage
;
.def	sregi		= r0		; save sreg in interrupt
.def	tx_data		= r1		; UART data byte
.def	fifol		= r2
.def	fifoh		= r3
.def	txipl		= r4		; tx uart in pointer
.def	txiph		= r5
.def	b0		= r6		; scratch register
.def	b1		= r7
.def	b2		= r8
.def	get_ack		= r9
					; r10 .. r15 free

.def	fifo_end_hi	= r16
.def	tx_low		= r17		; send high bit
.def	tx_high		= r18		; send low bit
.def	tx_state	= r19
.def	a0		= r20		; scratch register
.def	cnt0		= r24
.def	cnt1		= r25
					; r21 .. r23 free
;-------------------------------------------------------------------------
.dseg
.org	SRAM_START
fifo_start:
	.byte	502
fifo_end:
;-------------------------------------------------------------------------
.cseg
.list
	rjmp	init				; must be rjmp !
;-------------------------------------------------------------------------
.include "uart.inc"
.list
;-------------------------------------------------------------------------
init:
	ldi	zl, low (RAMEND)
	ldi	zh, high(RAMEND)
	out	SPL, zl
	out	SPH, zh
	ldi	zl,  low(XTAL / BAUD)
	ldi	zh, high(XTAL / BAUD)
	rcall	init_uart
	sei
	rcall	puts
	.db	"I2C-Sniffer:", 13, 10, 0, 0
init_i2c:
	ldi	a0, 1<<USIWM1^1<<USIWM0		; I2C, no counter
	out	USICR, a0
	sbi	I2C_PORT, SCL
	sbi	I2C_DDR, SCL
	sbi	I2C_PORT, SDA
	cbi	I2C_DDR, SDA
init_t0:
	ldi	a0, 1<<WGM01			; claer on compare A
	out	TCCR0A, a0
	ldi	a0, 1<<CS01^1<<CS00		; XTAL / 64
	out	TCCR0B, a0	
	ldi	a0, (XTAL / 64 + 500) / 1000 - 1	; 1ms
	out	OCR0A, a0
	ldi	cnt0, 0
	ldi	cnt1, 0
;-------------------------------------------------------------------------
.include "i2csniff.inc"
.include "bcd16.inc"
;-------------------------------------------------------------------------
