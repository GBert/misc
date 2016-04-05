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

READCANHASH
                ; read location ID - CAN hash ID is stored at 200000h + 200001h
                MOVLW	0
                MOVWF	TBLPTRL
                MOVWF   TBLPTRH
                MOVLW	20h
                MOVWF   TBLPTRU
                TBLRD*+
                MOVF	TABLAT,W
                IORLW	0x03		; make sure that's a valid hash
                MOVWF	CANHASH_HIGH
                TBLRD*
                MOVF	TABLAT,W
                ANDLW	0x7F
                MOVWF   CANHASH_LOW
                RETURN

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
                ; set vars
                MOVLW	255
                MOVWF	BUT_VAL_NEW
                MOVWF	BUT_VAL_CHG
                MOVWF	BUT_VAL_STATE
                MOVWF	TOP_VAL_NEW
                MOVWF	TOP_VAL_CHG
                MOVWF	TOP_VAL_STATE
                MOVWF	BUT_CT0
                MOVWF	BUT_CT1
                MOVWF	TOP_CT0
                MOVWF	TOP_CT1

                ; delete timers
                MOVLW	16
                MOVWF	PIOTEMP
                CLRW
                LFSR	FSR0,PIOTIME
PTIMECLLOOP
                ANDWF	POSTINC0,F
                DECFSZ	PIOTEMP,1
                BNZ	PTIMECLLOOP

                GOTO	READCANHASH

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
                MOVLW	'C'
                MOVWF	ID_H
                MOVLW	'B'
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
                ; RCALL	SENDEVENT
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
		MOVLW	255
		XORWF	BUT_VAL_NEW,F
		XORWF   TOP_VAL_NEW,F
#ENDIF

		GOTO	PIODEBOUNCE

PIOSENDEVENT
		RCALL	PIOUARTSEND
		RCALL	PIOCANSEND
                RETURN

PIOCANSEND
                ; Find a Tx slot - start with TX buffer 2 down to 0
                MOVLW   CAN_WIN_TXB2        ; Window mode on TXB2
                MOVWF   CANCON
                BTFSS   TXB2CON,TXREQ
                BRA     PIOTXCAN2

                MOVLW   CAN_WIN_TXB1        ; Window mode on TXB1
                MOVWF   CANCON
                BTFSS   TXB1CON,TXREQ
                BRA     PIOTXCAN2

                MOVLW   CAN_WIN_TXB0        ; Window mode on TXB0
                MOVWF   CANCON
                BTFSC   TXB0CON,TXREQ
                RETURN

                ; prepare CAN ID 0x00230300 Maerklin S88 Event
PIOTXCAN2
                MOVLW	0
                TXEXT_28_24
                MOVLW	23h
                TXEXT_23_21
                MOVLW	23h
                TXEXT_20_18
                MOVLW	23h
                TXEXT_17_16
                BSF	RXB0SIDL,EXIDEN     ; Enable Extended Id
                MOVF	CANHASH_HIGH,W
                MOVWF	RXB0EIDH
                MOVF	CANHASH_LOW,W
                MOVWF	RXB0EIDL
                ; DLC = 8
                MOVLW	8
                MOVWF	RXB0DLC
                MOVF	ID_H,W
                MOVWF	RXB0D0
                MOVF	ID_L,W
                MOVWF	RXB0D1
                MOVF	PIOTEMP16,W
                MOVWF	RXB0D2
                MOVF	PIOTEMP16+1,W
                MOVWF	RXB0D3

                MOVF	ACTUAL,W
                ANDWF	MASK,W
                BTFSS	STATUS,Z
                MOVLW	1		; either W is zero or it's set to one

                MOVWF	RXB0D5
                XORLW	1
                MOVWF	RXB0D4

                MOVLW	0
                MOVWF	RXB0D6
                MOVWF	RXB0D7
                BSF	RXB0CON,TXREQ       ; transmit
                RETURN

                ; test PIO -> SLCAN
PIOUARTSEND
                ; If Tx packet counter = trigger then drop packet
                MOVF    TXCNT,W
                XORLW   OVERFLOW
                BTFSC	STATUS,Z
                RETURN

                MOVLW	'T'
                PUTC
                MOVLW	00h
                CALL	BIN2ASC
                MOVLW	23h
                CALL	BIN2ASC
                MOVF	CANHASH_HIGH,W
                CALL	BIN2ASC
                MOVF	CANHASH_LOW,W
                CALL	BIN2ASC
                MOVLW	'8'
                PUTC
                MOVF	ID_H,W
                CALL	BIN2ASC
                MOVF	ID_L,W
                CALL	BIN2ASC
                MOVF	PIOTEMP16,W
                CALL	BIN2ASC
                MOVF	PIOTEMP16+1,W
                CALL	BIN2ASC

                MOVF	ACTUAL,W
                ANDWF	MASK,W
                BZ	PIOUART1
                ;
                MOVLW	0
                CALL	BIN2ASC
                MOVLW	1
                CALL	BIN2ASC
                GOTO	PIOUART2
PIOUART1
                MOVLW	1
                CALL	BIN2ASC
                MOVLW	0
                CALL	BIN2ASC
PIOUART2
                MOVLW	0
                CALL	BIN2ASC
                MOVLW	0
                CALL	BIN2ASC
                MOVLW	EOLNCHAR
                PUTC

                INCF    TXCNT,F             ; Tx packet counter++
                BSF     UPIE,UTXIE          ; Enable Tx ring ISR

                RETURN
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
