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

SYSTEM_FLAG_STOP_GO           EQU 0 ; bit in SYSTEM_FLAGS
SYSTEM_FLAG_WARM_STATE        EQU 1 ; bit in SYSTEM_FLAGS
SYSTEMFLAG_LED_TEST           EQU 2 ; bit in SYSTEM_FLAGS
SYSTEMFLAG_S88SENDALL_ONFIRST EQU 3 ; bit in SYSTEM_FLAGS

OPTION_FLAG_S88ALLONFIRST     EQU 0 ; bitindex in OPTION_FLAGS
OPTION_FLAG_S88ALLONFIRSTGO   EQU 1 ; bitindex in OPTION_FLAGS
OPTION_FLAG_LED_TEST          EQU 2 ; bitindex in OPTION_FLAGS
OPTION_LED_ON_EEP_WRITE       EQU 3 ; bitindex in OPTION_FLAGS
OPTION_2EQUALSS88             EQU 7 ; bitindex in OPTION_FLAGS

PIOTIME         EQU   0x310


                CBLOCK
                MASK : 1        ; 0x26
                ACTUAL : 1      ; 0x27
                CANHASH_LOW : 1 ; 0x28
                CANHASH_HIGH: 1 ; 0x29
                ID_H : 1    ; 0x2A
                ID_L : 1    ; 0x2B
                PIOC_H : 1  ; 0x2C
                PIOC_L : 1  ; 0x2D
                S88C_H : 1  ; 0x2E
                S88C_L : 1  ; 0x2F
                SCAN_INTERVAL : 1 ;0x30
                SYSTEM_FLAGS : 1  ;0x31 bit 0=stop(0)/go(1)
                OPTION_FLAGS : 1  ;0x32
                ENDC

                CBLOCK
                UID_H : 1   ; 0x33
                UID_L : 1   ; 0x34
                ENDC


#IFNDEF	SRSE2_SLCAN   ;PIO debounce not used in SRSE2
                CBLOCK
                PIOTEMP16 : 2
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
                ENDC


;------------------------------------------------------------------------------
; Macros
;------------------------------------------------------------------------------
                ; debouncing code from Peter Danegger
PIO_DEBOUNCE    MACRO   MVAL_NEW,MVAL_STATE,MVAL_CHG,MTEMP,MCT0,MCT1
                COMF	MVAL_NEW,W
                XORWF	MVAL_STATE,W	; port changed ?
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

#ENDIF ;IFNDEF	SRSE2_SLCAN
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
