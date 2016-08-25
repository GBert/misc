;************************************************************************/
;*                                                                      */
;*                      I2C Sniffer					*/
;*                                                                      */
;*              Author: Peter Dannegger                                 */
;*                                                                      */
;************************************************************************/
;-------------------------------------------------------------------------
i2c_sniff:
	in	a0, TIFR			; 1ms time base
	andi	a0, 1<<OCF0A
	breq	_t1ms1
	out	TIFR, a0			; clear compare flag
	adiw	cnt1:cnt0, 1
_t1ms1:
	in	a0, USISR
	sbrs	a0, USIPF			; STOP received ?
	rjmp	_isn1
	ldi	a0, 1<<USIWM1^1<<USIWM0		; I2C, no counter
	out	USISR, a0
	ldi	a0, 1<<USIPF			; clear USISIF
	out	USISR, a0
	ldi	a0, STOP
	rcall	putchar
	rjmp	i2c_sniff
;-------------------------------------------------------------------------
_isn1:
	sbrs	a0, USIOIF			; Data / ACK received ?
	rjmp	_isn3
	in	b0, USIDR
	dec	get_ack				; toggle get ACK / Data
	sbrs	get_ack, 0
	rjmp	_isn2
	ldi	a0, 1<<USIOIF^(0x0F & -2)	; count 2 edges
	out	USISR, a0
	mov	a0, b0
	rcall	puthex
	rjmp	i2c_sniff
_isn2:
	ldi	a0, 1<<USIOIF^(0x0F & -16)	; count 16 edges
	out	USISR, a0
	ldi	a0, NACK
	sbrs	b0, 0
	ldi	a0, ACK 
	rcall	putchar
	rjmp	i2c_sniff
;-------------------------------------------------------------------------
_isn3:
	sbrs	a0, USISIF			; START received ?
	rjmp	i2c_sniff
_isn4:
	sbic	I2C_PIN, SCL			; wait until SCL = low
	rjmp	_isn4
	ldi	a0, 1<<USIWM1^1<<USIWM0^1<<USICS1^1<<USICS0;
	out	USICR, a0			; I2C, get data on neg. edge
	ldi	a0, 1<<USISIF^(0x0F & -16)
	out	USISR, a0			; clear USISIF, count 16 edges
	clr	get_ack
	ldi	a0, 13				; new line
	rcall	putchar
	ldi	a0, 10
	rcall	putchar
	movw	zh:zl, cnt1:cnt0		; time stamp of start
	rcall	bin16_ascii
	rcall	puts
	.db	": ", START, 0
	rjmp	i2c_sniff
;-------------------------------------------------------------------------
