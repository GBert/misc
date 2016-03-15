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
                COMF	TOP_CT0,W
                MOVWF	TOP_CT0
                MOVF	TOP_CT1,W		; reset or count ct1
                ANDWF	PIOTEMP
                XORWF	TOP_CT0,W
                MOVF	TOP_CT1
                ANDWF	TOP_CT0,W
                ANDWF	PIOTEMP,W
                XORWF	TOP_VAL_STATE,F
                ANDWF	TOP_VAL_STATE,W
                IORWF	TOP_VAL_CHG,F

                COMF	BUT_VAL_NEW,W
                XORWF	BUT_VAL_STATE,W		; port changed ?
                MOVWF	PIOTEMP
                ANDWF	BUT_CT0,W		; reset or count ct0
                MOVWF   DEBUG_BUT
                COMF	BUT_CT0,W
                MOVWF	BUT_CT0
                MOVF	BUT_CT1,W		; reset or count ct1
                ANDWF	PIOTEMP
                XORWF	BUT_CT0,W
                MOVF	BUT_CT1
                ANDWF	BUT_CT0,W
                ANDWF	PIOTEMP,W
                XORWF	BUT_VAL_STATE,F
                ANDWF	BUT_VAL_STATE,W
                IORWF	BUT_VAL_CHG,F

                RETURN

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

		GOTO	PIODEBOUNCE

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
                MOVF	BUT_VAL_NEW,W
                MOVWF	RXB0D0
                MOVF	BUT_VAL_CHG,W
                MOVWF	RXB0D1
                MOVF	BUT_VAL_STATE,W
                MOVWF	RXB0D2
                MOVF	BUT_CT0,W
                MOVWF	RXB0D3
                MOVF	BUT_CT1,W
                MOVWF	RXB0D4
                MOVF	DEBUG_BUT,W
                MOVWF	RXB0D5
                MOVF	TOP_VAL_NEW,W
                MOVWF	RXB0D6
                MOVF	BUT_VAL_NEW,W
                MOVWF	RXB0D7

                BSF	RXB0CON,TXREQ       ; transmit
                RETURN

                ; test PIO -> SLCAN
PIOUARTSEND
                MOVLW	'T'
                PUTC
                MOVLW	00h
                CALL	BIN2ASC
                MOVLW	22h
                CALL	BIN2ASC
                MOVF	CANHASH_HIGH,W
                CALL	BIN2ASC
                MOVF	CANHASH_LOW,W
                CALL	BIN2ASC
                MOVLW	'8'
                PUTC
                MOVF	TOP_VAL_NEW,W
                CALL	BIN2ASC
                MOVF	TOP_VAL_CHG,W
                CALL	BIN2ASC
                MOVF	TOP_VAL_STATE,W
                CALL	BIN2ASC
                MOVF	TOP_CT0,W
                CALL	BIN2ASC
                MOVF	TOP_CT1,W
                CALL	BIN2ASC
                MOVF	DEBUG_TOP,W
                CALL	BIN2ASC
                MOVF	TOP_VAL_NEW,W
                CALL	BIN2ASC
                MOVF	BUT_VAL_NEW,W
                CALL	BIN2ASC
                MOVLW	EOLNCHAR
                PUTC
                INCF    TXCNT,F             ; Tx packet counter++
                BSF     UPIE,UTXIE          ; Enable Tx ring ISR

                RETURN
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
