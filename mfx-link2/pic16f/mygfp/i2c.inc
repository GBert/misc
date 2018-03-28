;------------------------------------------------------------------------------
;
; My GFP I2C
;
; Copyright (c) 2018 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------

		LIST
		RADIX   DEC

#DEFINE         SSP1ADD_VAL     ( (((CLOCK / I2C_BAUDRATE) / 2) - 1) / 2)

		CBLOCK 0x0220
		I2C_ADDR	: 1
		I2C_DATA	: 1
		ENDC

;------------------------------------------------------------------------------
; Init. I2C
;------------------------------------------------------------------------------
INITI2C
		BANKSEL	SSP1CON1
		MOVLW	b'11010110'
		MOVWF	SSP1CON1
		CLRF	SSP1CON2
		CLRF	SSP1CON3
		MOVLW	SSP1ADD_VAL
		MOVWF	SSP1ADD
		MOVLW	b'11000000'
		MOVWF	SSP1STAT
		RETURN

I2C_WAIT
		BANKSEL	SSP1STAT
		MOVFW	SSP1STAT
		ANDLW	5
		BTFSS	STATUS,Z
		BRA	I2C_WAIT
I2C_WAIT2
		MOVFW	SSP1CON2
		ANDLW	31
		BTFSS	STATUS,Z
		BRA	I2C_WAIT2
		RETURN

I2C_START
		CALL	I2C_WAIT
		BANKSEL	SSP1CON2
		BSF	SSP1CON2,SEN
		RETURN
	
I2C_STOP
		CALL	I2C_WAIT
		BANKSEL	SSP1CON2
		BCF	SSP1CON2,SEN
		RETURN

I2C_WRITE
		CALL	I2C_WAIT
		BANKSEL	SSP1STAT
		MOVFW	I2C_DATA
		MOVWF	SSP1BUF
		MOVLW	255
		BTFSS	SSP1STAT,WCOL
		CLRF	0
		RETURN

