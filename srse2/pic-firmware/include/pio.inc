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
                ; PIOTEMP : 1
                MASK : 1
                ACTUAL : 1
                ; PIOTEMP16 : 2
                BUT_CT0 : 1
                BUT_CT1 : 1
                BUT_VAL_NEW : 1
                BUT_VAL_CHG : 1
                BUT_VAL_STATE : 1
                TOP_CT0 : 1
                TOP_CT1 : 1
                TOP_VAL_NEW : 1
                TOP_VAL_CHG : 1
                TOP_VAL_STATE : 1
                CANHASH_LOW : 1
                CANHASH_HIGH: 1
                ID_H : 1
                ID_L : 1
                PIOC_H : 1
                PIOC_L : 1
                S88C_H : 1
                S88C_L : 1
                SCAN_INTERVAL : 1
                ENDC

PIOTIME         EQU   0x310
;------------------------------------------------------------------------------
; Macros
;------------------------------------------------------------------------------
                ; debouncing code from Peter Danegger
PIO_DEBOUNCE    MACRO   MVAL_NEW,MVAL_STATE,MVAL_CHG,MTEMP,MCT0,MCT1
                COMF	MVAL_NEW,W
                XORWF	MVAL_STATE,W		; port changed ?
                MOVWF	MTEMP
                ANDWF	MCT0,F	    	; reset or count ct0
                COMF	MCT0,F
                MOVF	MCT1,W		; reset or count ct1
                ANDWF	MTEMP,W
                XORWF	MCT0,W
                MOVWF	MCT1
                ANDWF	MCT0,W
                ANDWF	MTEMP,W
                MOVWF	MVAL_CHG
                XORWF	MVAL_STATE,F
                ENDM

PIOINIT
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
                MOVWF	TEMP
                LFSR	FSR0,PIOTIME
PTIMECLLOOP
                CLRF	POSTINC0
                DECFSZ	TEMP,F
                BRA	PTIMECLLOOP

                BRA	READCANHASH

PIODEBOUNCE
                PIO_DEBOUNCE   TOP_VAL_NEW, TOP_VAL_STATE, TOP_VAL_CHG, TEMP, TOP_CT0, TOP_CT1
                PIO_DEBOUNCE   BUT_VAL_NEW, BUT_VAL_STATE, BUT_VAL_CHG, TEMP, BUT_CT0, BUT_CT1

                RETURN
GENEVENT
                CALL	READ_UID
                MOVFF	PIOC_H,TEMP16
                MOVFF	PIOC_L,TEMP16+1

                MOVFF	BUT_VAL_NEW,ACTUAL
                MOVLW	1
                MOVWF	MASK
                MOVFF   BUT_VAL_CHG,TEMP

                RCALL	GENEVENTLOOP

                MOVFF	TOP_VAL_NEW,ACTUAL
                MOVLW	1
                MOVWF	MASK
                MOVFF	TOP_VAL_CHG,TEMP

                ; CALL	GENEVENTLOOP

GENEVENTLOOP
                INFSNZ	TEMP16+1,F
                INCF	TEMP16,F
                MOVF	TEMP,W
                ANDWF	MASK,W
                BTFSS	STATUS,Z
                RCALL	PUTEVENT

                BCF     STATUS,C
                RLCF    MASK,F
                BNC     GENEVENTLOOP

                RETURN

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
