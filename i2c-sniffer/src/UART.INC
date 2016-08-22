
;-------------------------------------------------------------------------
;		Software UART transmitter up to 460800 Baud
;-------------------------------------------------------------------------
.org	OC1Aaddr
	in	sregi, sreg
	cpi	tx_state, 0			; byte finished ?
	brne	_txi3
	cp	txipl, xl			; new data to send ?
	cpc	txiph, xh
	breq	_txi2
	ld	tx_data, x+
	ldi	tx_state, 9			; start next byte
	cpi	xl, low( fifo_end )		; end of FIFO
	cpc	xh, fifo_end_hi
	brne	_txi1
	movw	xh:xl, fifoh:fifol		; wrap around
_txi1:
	out	TCCR1, tx_low			; send start bit (low)
_txi2:
	out	SREG, sregi
	reti					; max 23 cycle
_txi3:
	dec	tx_state
	lsr	tx_data
	brcs	_txi1				; bit inverted
	out	TCCR1, tx_high
	out	SREG, sregi
	reti
;-------------------------------------------------------------------------
init_uart:
	ldi	tx_low, 1<<CTC1^1<<COM1A1^1<<CS10^INVERT
	rjmp	_inu2
_inu1:
	inc	tx_low
	lsr	zh
	ror	zl
_inu2:
	cpi	zh, 0
	brne	_inu1
	dec	zl
	out	OCR1A, zl
	out	OCR1C, zl
	ldi	tx_high, 1<<COM1A0
	eor	tx_high, tx_low
	out	TCCR1, tx_high
	ldi	a0, 1<<FOC1A
	out	GTCCR, a0			; force output level high
	sbi	TXD_DDR, TXD			; output
	ldi	xl, low ( fifo_start )
	ldi	xh, high( fifo_start )
	movw	txiph:txipl, xh:xl
	movw	fifoh:fifol, xh:xl
	ldi	fifo_end_hi, high( fifo_end )
	ldi	tx_state, 0
	ldi	a0, 1<<OCIE1A
	out	TIMSK, a0
	ret
;-------------------------------------------------------------------------
;                               transmit byte hexadecimal
;-------------------------------------------------------------------------
;input: a0 = byte
;use:	a0, b0, b1, b2, yl, yh
;
puthex:
	mov     b2, a0
	swap    a0
	rcall   _phe1
	mov     a0, b2
_phe1:
	andi    a0, 0x0F
	cpi     a0, 10
	brcs    _phe2
	subi    a0, -('A' - '0' - 10)
_phe2:
	subi    a0, -'0'
;	rjmp    putchar
;-------------------------------------------------------------------------
;                               transmit byte
;-------------------------------------------------------------------------
;input: a0 = byte
;use:	a0, b0, b1, yl, yh
;
putchar:
	movw	yh:yl, txiph:txipl
	com	a0				; invert byte
	st	y+, a0				; store byte in FIFO
	cpi	yl, low( fifo_end )		; end of FIFO ?
	cpc	yh, fifo_end_hi
	brne	_pch1
	movw	yh:yl, fifoh:fifol		; roll over
_pch1:
	movw	b1:b0, xh:xl			; atomic word read !
	cp	b0, yl
	cpc	b1, yh
	breq	_pch1				; until not equal
	movw	txiph:txipl, yh:yl		; atomic word write !
	ret
;-------------------------------------------------------------------------
;                       send zero terminated string after call
;-------------------------------------------------------------------------
;input:	constant string after call
;use:   a0, b0, b1, yl, yh, zl, zh
;
puts:
	pop     zh                      ; get address after call
	pop     zl
	lsl     zl                      ; *2 to get byte pointer
	rol     zh
	rjmp    _pts2
_pts1:
	rcall   putchar
_pts2:
	lpm     a0, Z+
	cpi	a0, 0			; check zero byte
	brne    _pts1
	lsr     zh                      ; /2 to get word pointer
	ror     zl
	ijmp				; return after string
;-------------------------------------------------------------------------
