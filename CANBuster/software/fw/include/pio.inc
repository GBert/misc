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
                MASK : 1
                ACTUAL : 1
                PIOTEMP16 : 2
                BUT_CT0 : 1
                DEBUG_BUT : 1
                BUT_CT1 : 1
                TOP_CT0 : 1
                DEBUG_TOP : 1
                TOP_CT1 : 1
                BUT_VAL_NEW : 1
                BUT_VAL_CHG : 1
                BUT_VAL_STATE : 1
                TOP_VAL_NEW : 1
                TOP_VAL_CHG : 1
                TOP_VAL_STATE : 1
                CANHASH_LOW : 1
                CANHASH_HIGH: 1
                ID_H : 1
                ID_L : 1
                ENDC

;------------------------------------------------------------------------------
; Init. PIO
;------------------------------------------------------------------------------

PIOINIT
                ; set ports as inputs

                ; RA0-3, RA5
                MOVLW	b'00101111'
                IORWF	TRISA,F

                ; RB0, RB1
                MOVLW	b'00000011'
                IORWF	TRISB,F

#IFDEF PIO
                ; RDO-5
                MOVLW	b'00111111'
                IORWF	TRISD,F

                ; RE0-2
                MOVLW	b'00000111'
                IORWF	TRISE,F
#ENDIF
                ; set vars to 0xff
                SETF	BUT_VAL_NEW
                SETF	BUT_VAL_CHG
                SETF	BUT_VAL_STATE
                SETF	TOP_VAL_NEW
                SETF	TOP_VAL_CHG
                SETF	TOP_VAL_STATE
                SETF	BUT_CT0
                SETF	BUT_CT1
                SETF	TOP_CT0
                SETF	TOP_CT1

                ; delete timers
                MOVLW	16
                MOVWF	PIOTEMP
                LFSR	FSR0,PIOTIME
PTIMECLLOOP
                CLRF	POSTINC0
                DECFSZ	PIOTEMP,F
                BRA	PTIMECLLOOP

                BRA	READCANHASH

                ; debouncing code from Peter Danegger
PIODEBOUNCE
                COMF	TOP_VAL_NEW,W
                XORWF	TOP_VAL_STATE,W		; port changed ?
                MOVWF	PIOTEMP
                ANDWF	TOP_CT0,W	    	; reset or count ct0
                XORLW	255
                MOVWF	TOP_CT0
                MOVF	TOP_CT1,W		; reset or count ct1
                ANDWF	PIOTEMP,W
                XORWF	TOP_CT0,W
                MOVWF	TOP_CT1
                ANDWF	TOP_CT0,W
                ANDWF	PIOTEMP,W
                MOVWF	TOP_VAL_CHG
                XORWF	TOP_VAL_STATE,F

                COMF	BUT_VAL_NEW,W
                XORWF	BUT_VAL_STATE,W		; port changed ?
                MOVWF	PIOTEMP
                ANDWF	BUT_CT0,W		; reset or count ct0
                XORLW	255
                MOVWF	BUT_CT0
                MOVF	BUT_CT1,W		; reset or count ct1
                ANDWF	PIOTEMP,W
                XORWF	BUT_CT0,W
                MOVWF	BUT_CT1
                ANDWF	BUT_CT0,W
                ANDWF	PIOTEMP,W
                MOVWF	BUT_VAL_CHG
                XORWF	BUT_VAL_STATE,F

                RETURN

GENEVENT
                MOVLW	'U'
                MOVWF	ID_H
                MOVLW	'S'
                MOVWF	ID_L
                CLRF	PIOTEMP16
                CLRF	PIOTEMP16+1

                MOVF	BUT_VAL_NEW,W
                MOVWF	ACTUAL
                MOVLW	1
                MOVWF	MASK
                MOVF    BUT_VAL_CHG,W
                MOVWF	PIOTEMP
                RCALL	GENEVENTLOOP

                MOVF	TOP_VAL_NEW,W
                MOVWF	ACTUAL
                MOVLW	1
                MOVWF	MASK
                MOVF	TOP_VAL_CHG,W
                MOVWF	PIOTEMP
                RCALL	GENEVENTLOOP

                RETURN

GENEVENTLOOP
                INFSNZ	PIOTEMP16+1,F
                INCF	PIOTEMP16,F
                MOVF	PIOTEMP,W
                ANDWF	MASK,W
                BTFSS	STATUS,Z
                RCALL	PUTEVENT

                BCF     STATUS,C
                RLCF    MASK,F
                BNC     GENEVENTLOOP

                RETURN


;------------------------------------------------------------------------------
; get status
;------------------------------------------------------------------------------
PIOGET
                ; RA5 is IO5
                RRNCF	PORTA,W
                ANDLW	b'00010000'
                MOVWF	BUT_VAL_NEW
                ; RA0-RA3 are IO1-4
                MOVF    PORTA,W
                ANDLW   b'00001111'
                IORWF	BUT_VAL_NEW,F
                ; RE0-2 are IO6-8
                SWAPF	PORTE,W
                RLNCF	WREG,F
                ANDLW	b'11100000'
                IORWF	BUT_VAL_NEW,F

                ; RD0-5 are IO9-14
                MOVF    PORTD,W
                ANDLW	b'00111111'
                MOVWF   TOP_VAL_NEW
                ; RB0-1 are IO15-16
                RRNCF	PORTB,W
                RRNCF	WREG,W
                ANDLW	b'11000000'
                IORWF	TOP_VAL_NEW,F

#IFDEF PIOINVERT
                COMF	BUT_VAL_NEW,F
                COMF	TOP_VAL_NEW,F
#ENDIF

                BRA	PIODEBOUNCE

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
