;*************************************************************************
;*                                                                       *
;*                      Convert unsigned 16bit to ASCII                  *
;*                                                                       *
;*              Author: Peter Dannegger                                  *
;*                      danni@specs.de                                   *
;*                                                                       *
;*************************************************************************
;
;input: ZH, ZL = 16 bit value 0 ... 65535
;output: UARTR: 5 digits (ASCII)
;
bin16_ascii:
	ldi	a0, -1 + '0'
_bcd1:	inc	a0
	subi	zl, low(10000)
	sbci	zh, high(10000)
	brcc	_bcd1
	rcall	putchar
	ldi	a0, 10 + '0'
_bcd2:	dec	a0
	subi	zl, low(-1000)
	sbci	zh, high(-1000)
	brcs	_bcd2
	rcall	putchar
	ldi	a0, -1 + '0'
_bcd3:	inc	a0
	subi	zl, low(100)
	sbci	zh, 0
	brcc	_bcd3
	rcall	putchar
	ldi	a0, 10 + '0'
_bcd4:	dec	a0
	subi	zl, -10
	brcs	_bcd4
	rcall	putchar
	mov	a0, zl
	subi	a0, -'0'
	rjmp	putchar
;-------------------------------------------------------------------------
