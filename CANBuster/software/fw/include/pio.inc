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
                CANHASH_LOW : 1
                CANHASH_HIGH : 1
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
		IORLW	03h		; make sure that's a valid hash
		MOVWF	CANHASH_HIGH
		TBLRD*
		MOVF	TABLAT,W
		MOVWF   CANHASH_LOW
		RETURN

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

;------------------------------------------------------------------------------
; get status
;------------------------------------------------------------------------------
PIOGET
		; RA5 is IO5
		MOVF	PORTA,W
		ANDLW   b'00100000'
		RRNCF	WREG,F
		MOVWF	BUT_VAL_NEW
		; RA0-RA3 are IO1-4
		MOVF    PORTA,W
		ANDLW   b'00001111'
		IORWF	BUT_VAL_NEW,F
		; RE0-2 are IO6-8
		SWAPF	PORTE,W
		ANDLW	b'01110000'
		RLNCF	WREG,F
		IORWF	BUT_VAL_NEW,F

		; RD0-5 are IO9-14
		MOVF    PORTD,W
		ANDLW	b'00111111'
		MOVWF   TOP_VAL_NEW
		; RB0-1 are IO15-16
		SWAPF	PORTB,W
		ANDLW	b'00110000'
		RLNCF	WREG,F
		RLNCF	WREG,F
		IORWF   TOP_VAL_NEW,F

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

; prepare CAN ID 0x00220300 Maerklin S88 Event

PIOTXCAN2
		MOVLW	0
		TXEXT_28_24
		MOVLW	22h
		TXEXT_23_21
		MOVLW	22h
		TXEXT_20_18
		MOVLW	22h
		TXEXT_17_16
		BSF	RXB0SIDL,EXIDEN     ; Enable Extended Id
		MOVF	CANHASH_HIGH,W
		MOVWF	RXB0EIDH
		MOVF	CANHASH_LOW,W
		MOVWF	RXB0EIDL
		; DLC = 8
		MOVLW	8
		MOVWF	RXB0DLC
		MOVLW	0
		MOVWF	RXB0D0
		MOVWF	RXB0D1
		MOVWF	RXB0D2
		MOVWF	RXB0D3
		MOVWF	RXB0D4
		MOVWF	RXB0D5
		MOVF	TOP_VAL_NEW,W
		MOVWF	RXB0D6
		MOVF	BUT_VAL_NEW,W
		MOVWF	RXB0D7

		BSF	RXB0CON,TXREQ       ; transmit
		RETURN

PIOTXUART	MOVLW	'T'
		PUTC
		MOVLW	'0'
		PUTC
		MOVLW	'0'
		PUTC
		MOVLW	'2'
		PUTC
		MOVLW	'2'
		PUTC

		MOVF	CANHASH_HIGH,W
		CALL	BIN2ASC
		MOVF	CANHASH_LOW,W
		CALL	BIN2ASC
		MOVLW	'8'
		PUTC
		RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
