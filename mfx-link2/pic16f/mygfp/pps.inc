;------------------------------------------------------------------------------
;
; My GFP UART
;
; Copyright (c) 2018 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------

		LIST
		RADIX   DEC

;------------------------------------------------------------------------------
; Init. PPS
;------------------------------------------------------------------------------
INITPPS
		; unlock PPS
		MOVLW	0x55
		BANKSEL	PPSLOCK
		MOVWF	PPSLOCK
		MOVLW	0xAA
		MOVWF	PPSLOCK
		BCF	PPSLOCK,PPSLOCKED

		; I2C
		MOVLW	0x05
		BANKSEL	SSPCLKPPS
		MOVWF	SSPCLKPPS
		MOVLW	b'10000'	; RA5 -> SCL
		BANKSEL RA5PPS
		MOVWF	RA5PPS
		MOVLW	0x04
		BANKSEL	SSPDATPPS
		MOVWF	SSPDATPPS
		MOVLW	b'10001'	; RA4 -> SDA
		BANKSEL	RA4PPS
		MOVWF	RA4PPS

		; USART
		MOVLW	b'10001'	; RC1 -> RxD
		BANKSEL	RXPPS
		MOVWF	RXPPS
		MOVLW	b'10100'	; RC2 -> TxD
		BANKSEL	RC2PPS
		MOVWF	RC2PPS

		MOVLW	b'01100'	; RC4PPS -> CCP1P
		MOVWF	RC4PPS
		MOVLW	b'01001'	; RB5PPS -> COG1B -> RPWM(IBT2)
		MOVWF	RB6PPS
		MOVLW	b'01000'	; RB6PPS -> COG1A -> LPWM(IBT2)
		MOVWF	RB6PPS

		; lock PPS
		MOVLW	0x55
		BANKSEL	PPSLOCK
		MOVWF	PPSLOCK
		MOVLW	0xAA
		MOVWF	PPSLOCK
		BSF	PPSLOCK,PPSLOCKED
		RETURN

