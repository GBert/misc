;------------------------------------------------------------------------------
;
; CANBuster PIO
;
; Copyright (c) 2016 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; This file is part of the CANBuster project.
;
; CANBuster is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; PIO variables
;------------------------------------------------------------------------------

                CBLOCK
                PIOTIME : 16
                PIOTEMP : 1
                BUT_VAL_NEW: 1
                BUT_VAL_OLD: 1
                TOP_VAL_NEW: 1
		TOP_VAL_OLD: 1
                HASH_LOW : 1
                HASH_HIGH : 1
                ENDC

;------------------------------------------------------------------------------
; Init. PIO
;------------------------------------------------------------------------------
PIOINIT
		; set ports as inputs

		; RA0-3, RA5
		MOVF	TRISA,W
		IORLW	b'00101111'
		MOVWF	TRISA

		; RB0, RB1
		BSF	TRISB,0
		BSF	TRISB,1

#IFDEF PIO
		; RDO-5
		MOVF	TRISD,W
		IORLW	b'00111111'
		MOVWF	TRISD

		; RE0-2
		BSF	TRISE,0
		BSF	TRISE,1
		BSF	TRISE,2
#ENDIF
		; clear vars
		CLRF	BUT_VAL_NEW
		CLRF	BUT_VAL_OLD
		CLRF	TOP_VAL_NEW
		CLRF	TOP_VAL_OLD

		; TODO
		MOVLW	16
		MOVWF	PIOTEMP

		CLRF	PIOTIME

                RETURN

;------------------------------------------------------------------------------
; get status
;------------------------------------------------------------------------------
PIOGET
		MOVF	PORTA,W
		MOVF	PORTB,W

		RETURN
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
