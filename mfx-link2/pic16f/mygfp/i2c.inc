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
		I2C_BUF		: 1
		I2C_ADDR	: 1
		I2C_DATA	: 1
		ENDC

;------------------------------------------------------------------------------
; Init. I2C
;------------------------------------------------------------------------------
INITI2C
		BANKSEL	SSP1CON1
		MOVLW	b'00101000'
			; --1-----   SSPEN Synchronous Serial Port Enable bit
			; ----1000   I2C Master mode, clock = F OSC / (4 * (SSPxADD+1))
		MOVWF	SSP1CON1
		CLRF	SSP1CON2
		CLRF	SSP1CON3
		MOVLW	SSP1ADD_VAL
		MOVWF	SSP1ADD
		MOVLW	b'11000000'
			; 1-------   1 = Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
			; -1------   1 = Enable input logic so that thresholds are compliant with SMBus specification
			; --0-----   0 = Indicates that the last byte received or transmitted was address
		MOVWF	SSP1STAT
		RETURN

I2C_WAIT
		BANKSEL	SSP1STAT
		MOVF	SSP1STAT,W
		ANDLW	5
		BTFSS	STATUS,Z
		BRA	I2C_WAIT
I2C_WAIT2
		MOVF	SSP1CON2,W
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
		MOVF	I2C_BUF,W
		MOVWF	SSP1BUF
		MOVLW	255
		BTFSS	SSP1STAT,WCOL
		CLRF	0
		RETURN

