;------------------------------------------------------------------------------
;
; My GFP Timer
;
; Copyright (c) 2018 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------

		LIST
		RADIX   DEC

;------------------------------------------------------------------------------
; Init. UART
;------------------------------------------------------------------------------
INITTIMER
		; timer 0
		BANKSEL	OPTION_REG
		MOVLW	b'11010110'
		ANDWF	OPTION_REG,F
		
		MOVLW	TIMER0_VAL
		BANKSEL	TMR0
		MOVWF	TMR0

		; timer 1 & CCP1
		MOVLW	b'00110001'
			; 00------	Timer1 source FOSC/4
			; --11----	1:8 Prescale
			; -------1	enabel Timer1
		BANKSEL	T1CON
		MOVWF	T1CON
		BCF	PIE1,TMR1GIE
		CLRF	TMR1L
		CLRF	TMR1H
		MOVLW	b'00001000'
			; ----1000	Compare mode: set output on compare match
		BANKSEL	CCP1CON
		MOVWF	CCP1CON
		MOVLW	255
		MOVWF	CCPR1L
		MOVWF	CCPR1H
		BANKSEL	PIE1
		BSF	PIE1,CCP1IE
                RETURN

