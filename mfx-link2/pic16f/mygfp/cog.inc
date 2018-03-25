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
INITCOG
		BANKSEL	COG1CON0
		CLRF	COG1CON0
		MOVLW	b'000010000'
		MOVWF	COG1RIS
		CLRF	COG1RSIM
		MOVWF	COG1FIS
		CLRF	COG1FSIM
		CLRF	COG1ASD0
		CLRF	COG1ASD1
		CLRF	COG1STR
		CLRF	COG1DBR
		CLRF	COG1DBF
		CLRF	COG1BLKR
		CLRF	COG1BLKF
		CLRF	COG1PHR
		CLRF	COG1PHF
		MOVLW	b'10001100'
		CLRF	COG1CON0
                RETURN

