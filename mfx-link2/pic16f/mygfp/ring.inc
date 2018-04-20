;------------------------------------------------------------------------------
;
; My GFP ring buffer
;
; Copyright (c) 2018 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------

		LIST
		RADIX   DEC

;------------------------------------------------------------------------------
;		highly optimized - don't change !

		; use Bank3 to avoid BANKSEL
		CBLOCK	0x01A0
		RX_HEAD	: 1
		RX_TAIL	: 1
		TX_HEAD	: 1
		TX_TAIL	: 1
		RX_TEMP : 1
		TX_TEMP : 1
		ENDC

		; 0x020 - 0x06F	Bank0	80 bytes
		; 0x0A0 - 0x0EF	Bank1	80 bytes
		; 0x120 - 0x16F	Bank2	80 bytes
		; 0x1A0 - 0x1AF	Bank3	16 bytes
		; mapped to 0x2100 FSR
		; 0x1B0	- 0x1EF	Bank4	64 bytes
		
		CBLOCK	0x01B0
		RX_RING	: 32	; 0x01B0
		TX_RING	: 32	; 0x01D0
		ENDC
;------------------------------------------------------------------------------

		; destroy BANK
PUTC		MACRO
		BANKSEL	TX_TEMP
		MOVWF	TX_TEMP
		INCF	TX_HEAD,W
		ANDLW	0x1F		; limit to 31
		MOVWF	FSR1L
		MOVFW	TX_TEMP
		MOVWI	-0x20[FSR1]
		ENDM

		; destroy BANK
GETC		MACRO
		BANKSEL	RX_TAIL
		INCF	RX_TAIL,W
		ANDLW	0x1F		; limit to 31
		MOVWF	FSR1L
		MOVIW	0x00[FSR1]
		CLRF	FSR1L
		ENDM

