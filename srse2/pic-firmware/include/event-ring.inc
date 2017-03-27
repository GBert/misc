;------------------------------------------------------------------------------
;
; CAN frames send ring
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
; event ring variables
;------------------------------------------------------------------------------

                CBLOCK
                EVENTTMP : 1		; tmp buffer
                EVENTCNT : 1		; event counter
                ENDC

EVENTPP   EQU   0x308           ; event pointer
EVENTPG   EQU   0x30a           ; event pointer

;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

EVENTBUF        EQU 0x400		; event ring
                CBLOCK 0x400
                EVENTBUFFER : 1024
                ENDC

;------------------------------------------------------------------------------
; Event ring macros
;------------------------------------------------------------------------------

INITEVENTRING
                CLRF	EVENTCNT	; event counter
                LFSR	FSR0,EVENTBUF
                MOVFF	FSR0H,EVENTPP
                MOVFF	FSR0H,EVENTPG
                MOVFF	FSR0L,EVENTPP+1
                MOVFF	FSR0L,EVENTPG+1
                RETURN

PUTEVENT
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                MOVLW	0x00
                MOVWF	POSTINC0
                ; mark with 0x23 as S88 event
                MOVLW	0x23
                MOVWF	POSTINC0

                MOVFF	CANHASH_HIGH,POSTINC0
                MOVFF	CANHASH_LOW,POSTINC0
                ; DLC
                MOVLW	0x8
                MOVWF	POSTINC0
                ; DATA
                MOVF    ID_H,W
                ADDLW   -'U'
                MOVWF	POSTINC0
                MOVF    ID_L,W
                ADDLW   -'S'
                MOVWF	POSTINC0
                MOVFF	TEMP16,POSTINC0
                MOVFF	TEMP16+1,POSTINC0
                MOVF	ACTUAL,W
                ANDWF	MASK,W
                BTFSS	STATUS,Z
                MOVLW   1		; either W is zero or it's set to one

                XORLW	1
                MOVWF	POSTINC0
                XORLW	1
                MOVWF	POSTINC0
                CLRF	POSTINC0	; time - unused
                CLRF	POSTINC0

                ;;;;;;;;;;;;;;;;;;;;;;
                MOVLW	15
                ADDWF   FSR0L,F
                BNC     NORM_C1
                INCF    FSR0H,F
NORM_C1
                MOVLW   0xF0
                ANDWF   FSR0L,F

                MOVLW	0x04		; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F

                MOVFF   FSR0H,EVENTPP
                MOVFF   FSR0L,EVENTPP+1

                INCF	EVENTCNT,F
                RETURN


GETEVENT_P_NORM MACRO
                MOVLW	15
                ADDWF   FSR0L,F
                BNC     NORM_C2
                INCF    FSR0H,F
NORM_C2
                MOVLW   0xF0
                ANDWF   FSR0L,F

                MOVLW	0x04		; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F
                MOVFF   FSR0H,EVENTPG
                MOVFF   FSR0L,EVENTPG+1

                RETURN
                ENDM

GETEVENT_P      MACRO
                MOVFF	EVENTPG,FSR0H
                MOVFF	EVENTPG+1,FSR0L
                ENDM

SENDEVENT
                ; check if an event is in the buffer
                MOVF	EVENTCNT,F
                BTFSC	STATUS,Z
                RETURN
                ; check if UART send buffer is available
                MOVLW	OVERFLOW - 5	; 5 buffers spare for CAN -> UART
                SUBWF	TXCNT,W	
                BTFSC	STATUS,C
                RETURN

                ; last: check for CAN buffer
                ; Find a Tx slot - check if TX buffer 2 is empty 
                MOVLW   CAN_WIN_TXB2        ; Window mode on TXB2
                MOVWF   CANCON
                BTFSS   TXB2CON,TXREQ
                BRA     EVENTTXCAN

                RETURN

EVENTTXCAN
                GETEVENT_P
                MOVF	POSTINC0,W
                TXEXT_28_24
                MOVF	INDF0,W
                TXEXT_23_21
                MOVF	INDF0,W
                TXEXT_20_18
                MOVF	POSTINC0,W
                TXEXT_17_16
                BSF	RXB0SIDL,EXIDEN     ; Enable Extended Id
                MOVFF	POSTINC0,RXB0EIDH
                MOVFF   POSTINC0,RXB0EIDL
                ; DLC
                MOVFF	POSTINC0,RXB0DLC
                ; DATA
                MOVFF	POSTINC0,RXB0D0
                MOVFF	POSTINC0,RXB0D1
                MOVFF	POSTINC0,RXB0D2
                MOVFF	POSTINC0,RXB0D3
                MOVFF	POSTINC0,RXB0D4
                MOVFF	POSTINC0,RXB0D5
                MOVFF	POSTINC0,RXB0D6
                MOVFF	POSTINC0,RXB0D7

                ; test
                ; MOVFF	FSR0H,RXB0D5
                ; MOVFF	FSR0L,RXB0D6
                ; MOVFF	EVENTCNT,RXB0D7

                BSF	RXB0CON,TXREQ       ; start CAN transmit

                ; and now the SLCAN packet
                GETEVENT_P
                MOVLW	'T'
                PUTC
                MOVF	POSTINC0,W
                CALL	BIN2ASC
                MOVF	POSTINC0,W
                CALL	BIN2ASC
                MOVF	POSTINC0,W
                CALL	BIN2ASC
                MOVF	POSTINC0,W
                CALL	BIN2ASC
                MOVF	POSTINC0,W
                ANDLW	0x0f
                BNZ	EVENTUARTCONT

                MOVLW   '0'
                PUTC
                BRA	EVENTUARTFIN

EVENTUARTCONT
                ; DLC
                MOVWF	EVENTTMP
                ADDLW	0x30
                PUTC
EVENTUARTLOOP
                MOVFF	POSTINC0,WREG
                CALL	BIN2ASC
                DECFSZ	EVENTTMP,F
                BRA	EVENTUARTLOOP

EVENTUARTFIN
                MOVLW	EOLNCHAR
                PUTC

                INCF	TXCNT,F
                BSF	UPIE,UTXIE		; enable Tx ring ISR

                ; one event done
                DECF	EVENTCNT,F
                ; fix the pointer if necessary
                GETEVENT_P_NORM
                RETURN

;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
