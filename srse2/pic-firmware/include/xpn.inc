;------------------------------------------------------------------------------
;
; XpressNet routines
;
; Copyright (c) 2021 Gerhard Bertelsmann
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
                XPNSLOTNUMBER : 1
                XPNTMP : 1		; tmp buffer
                XPN_RCNT : 1		; byte receive counter
                XPN_SCNT : 1		; byte send counter
                XPN_SEND_DATA : 1
                XPN_XOR : 1
                EVENTXPN: 1
                ENDC


;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

XPNREADBUF	EQU 0x800		; event buffer
XPNSENDBUF	EQU 0x900		; send buffer
XPNREADP	EQU 0xA00
XPNSENDP	EQU 0xA02

;------------------------------------------------------------------------------
; XPressNet
;------------------------------------------------------------------------------

#DEFINE	XPN_CALL_TRANSFER_ERR	0x00
#DEFINE	XPN_CALL_REQUEST_ACK	0x00
#DEFINE	XPN_CALL_INQUIRY	0x40
#DEFINE	XPN_CALL_RESPONSE	0x60
#DEFINE	XPN_CALL_BROADCAST	0x60

;------------------------------------------------------------------------------
; UART Baud Rate Generator (FOSC / BAUDRATE / 4 - 1)
;------------------------------------------------------------------------------

#DEFINE XPNBAUD   ((((CLOCK / XPNBAUDRATE) / 2) - 1) / 2)
#IF XPNBAUD > 65535
    ERROR       "XPN UART BRG overflow"
#ENDIF
#IF XPNBAUD == 0
    ERROR       "XPN UART BRG zero"
#ENDIF

;------------------------------------------------------------------------------
; Init. XpressNet Interface (RS485 9N1 62500 Baud)
;------------------------------------------------------------------------------
INITXPN
                BCF	TRISC,0		; DE/RE Pin
                BCF	PORTC,0		;  set it
                BCF	TRISC,1		; test pin
                BCF	TRISC,6		; UART1 TX
                BSF	TRISC,7		; UART1 RX

                CLRF    XPNSLOTNUMBER

                MOVLW   (1 << BRG16)
                MOVWF   BAUDCON1

                MOVLW   HIGH (XPNBAUD)
                MOVWF   SPBRGH1
                MOVLW   LOW  (XPNBAUD)
                MOVWF   SPBRG1

                ; Enable 9-bit Mode + Enable Transmit + High Speed Mode + BIT9 = 0
                MOVLW   (1 << TX9) + (1 << TXEN) + (1 << BRGH) + (0 << TX9D)
                MOVWF   TXSTA1
INITXPNAGAIN
                ; Enable Serial Port + Disable Receiver
                MOVLW   (1 << SPEN)
                MOVWF   RCSTA1

                ; Enable Receiver
                BSF     RCSTA1,CREN

                ; Flush Receiver
                MOVF    RCREG1,W
                MOVF    RCREG1,W
                MOVF    RCREG1,W


                RETURN

;------------------------------------------------------------------------------
; Event ring macros
;------------------------------------------------------------------------------

INITXPNBUFFER
		CLRF	POWER_ONOFF
		CLRF	XPN_RCNT
		CLRF	XPN_SCNT
		CLRF	XPNSLOTNUMBER
		LFSR	FSR0,XPNREADBUF
		MOVFF	FSR0H,XPNREADP
		MOVFF	FSR0L,XPNREADP+1

CLEARXPNREADBUF
		DCFSNZ	XPN_RCNT
		BRA	INITXPNBUFFER_NEXT
		CLRF	POSTINC0
		BRA	CLEARXPNREADBUF

INITXPNBUFFER_NEXT
		LFSR	FSR0,XPNSENDBUF
		MOVFF	FSR0H,XPNSENDP
		MOVFF	FSR0L,XPNSENDP+1

                RETURN

;------------------------------------------------------------------------------
; Parity computing (8th bit)
;------------------------------------------------------------------------------

; 7-bit parity
;
; This routine will calculate the parity of a 7-bit
; integer and place the result in the 8-position

XPN_PARITY	MACRO

                SWAPF	XPN_SEND_DATA,W		;W = efghabcd
                XORWF	XPN_SEND_DATA,W		;W = ea.fb.gc.hd.ea.fb.gc.hd
						; where ea means e^a, etc
                MOVWF	XPNTMP
                RLCF	XPNTMP,F		;XPNTMP = fb.gc.hd.ea.fb.gc.hd.??
                RLCF	XPNTMP,F		;XPNTMP = gc.hd.ea.fb.gc.hd.??.ea
                XORWF	XPNTMP,F		;XPNTMP = gcea.hdfb.gcea.hdfb.gcea.?.?
						;again, gcea means g^c^e^a
                RLCF	XPNTMP,W		;W = hdfb.gcea.hdfb.gcea.hdfb.?.fb
                XORWF   XPNTMP,W		;W = abcdefgh.abcdefgh.....
						;ie, the upper 5-bits of w each contain
						;the parity calculation.
                ANDLW	0x80			;We only need one of them
                IORWF	XPN_SEND_DATA,W		;copy it to the MSB of the byte to send
                ENDM

;------------------------------------------------------------------------------
; send XpressNet to CAN/UART
;------------------------------------------------------------------------------

SENDXPNCANUART
                ; check if an event is in the buffer
                MOVF	EVENTXPN,F
                BTFSC	STATUS,Z
                RETURN
                ; check if UART send buffer is available
                MOVLW	OVERFLOW - 5    ; 5 buffers spare for CAN -> UART
                SUBWF	TXCNT,W
                BTFSC	STATUS,C
                RETURN

                ; last: check for CAN buffer
                ; Find a Tx slot - check if TX buffer 1 is empty
                MOVLW	CAN_WIN_TXB1        ; Window mode on TXB1
                MOVWF	CANCON
                BTFSS	TXB1CON,TXREQ
                BRA	XPNTXCAN

                RETURN

XPNTXCAN
                ; ....
                RETURN


; -------------
;
;

XPNSERVICE	BTFSS	PIR1,TX1IF		; return if we already send data
		RETURN

		; check: data to send ?
		MOVF	XPN_SCNT,W
		BZ	XPNS_NEXT1

		BCF	TXSTA1,TX9D		; data bytes (9th bit off)
		MOVFF	XPNSENDP,FSR0H
		MOVFF	XPNSENDP+1,FSR0L
		MOVFF	POSTINC0,WREG
		MOVWF	TXREG1

		DECF	XPN_SCNT
		MOVFF	FSR0L,XPNSENDP+1

XPNS_NEXT1	BTFSC   TXSTA1,TRMT		; last bit send ?
		BCF     PORTC,0			;  free RS485 (DE) transceiver

		; check incoming data
		BTFSS	PIR1,RC1IF		; check for incoming data
		RETURN

		MOVFF	XPNREADP,FSR0H		; we have some new incoming data
		MOVFF	XPNREADP+1,FSR0L	;  restore receiving pointer
		MOVF	RCREG1,W
		MOVWF	POSTINC0
		MOVFF	FSR0L,XPNREADP+1	; store the lower poniter part only
		INCF	XPN_RCNT
		MOVFF	XPNREADBUF,WREG		; check if received frame is complete
		ANDLW	0x0F
		ADDLW   2
		SUBWF   XPN_RCNT,W
		BTFSS	STATUS,Z		; back if incomplete
		RETURN
		; TODO: looking for checksum
		CALL	DEBUG_XPNREADBUF_CAN

		; check the received frame
		MOVFF	XPNREADBUF,WREG
		ANDLW	0xF0
		XORLW	0x20
		BZ	XPN_INFO
		MOVFF   XPNREADBUF,WREG
		XORLW   0xE3
		BZ      XPN_LOCO_INFO

		RETURN
; ----------------------------
; XpressNet different types


XPN_INFO	; start/stop ?
		MOVFF   XPNREADBUF+1,WREG
		ANDLW	0xFE
		XORLW	0x80
		BTFSS   STATUS,Z
		BRA	XPN_INFO_NEXT1
		MOVLW   3
		MOVWF   XPN_SCNT
		MOVLW   0x61
		MOVFF   WREG,XPNSENDBUF
		MOVFF	XPNREADBUF+1,WREG
		ANDLW	0x01
		MOVFF   WREG,XPNSENDBUF+1
		XORLW	0x61
		MOVFF   WREG,XPNSENDBUF+2
		CALL	DEBUG_XPNSENDBUF_CAN
		BRA	XPN_BROADCAST_SEND
		;

XPN_INFO_NEXT1	; software version
		MOVFF   XPNREADBUF+1,WREG
		XORLW	0x21
		BTFSS   STATUS,Z
		BRA	XPN_INFO_NEXT2
		MOVLW	5
		MOVWF	XPN_SCNT
		MOVLW	0x63
		MOVFF	WREG,XPNSENDBUF
		MOVLW	0x21
		MOVFF	WREG,XPNSENDBUF+1
		MOVLW	0x30
		MOVFF	WREG,XPNSENDBUF+2
		MOVLW	0x00
		MOVFF	WREG,XPNSENDBUF+3
		MOVLW	0x72
		MOVFF	WREG,XPNSENDBUF+4
		CALL	DEBUG_XPNSENDBUF_CAN
		;
		BRA	XPN_DATA_SEND

XPN_INFO_NEXT2	; status central
		MOVFF   XPNREADBUF+1,WREG
		XORLW	0x24
		BTFSS   STATUS,Z
		RETURN
		; TODO
		MOVLW	4
		MOVWF	XPN_SCNT
		MOVLW	0x62
		MOVFF	WREG,XPNSENDBUF
		MOVLW	0x22
		MOVFF	WREG,XPNSENDBUF+1
		MOVF	POWER_ONOFF,W
		ANDLW	0x1
		XORLW	1			; Maerklin uses 1 for Go
		MOVFF	WREG,XPNSENDBUF+2
		XORLW	0x40			; 0x62 ^ 0x22
		MOVFF	WREG,XPNSENDBUF+3
		CALL	DEBUG_XPNSENDBUF_CAN
		;
		BRA	XPN_DATA_SEND

XPN_LOCO_INFO
		MOVLW	0x6
		MOVWF	XPN_SCNT
		MOVLW	0xE4
		MOVWF	XPN_XOR
		MOVFF	WREG,XPNSENDBUF
		MOVLW	0x02
		XORWF	XPN_XOR
		MOVFF	WREG,XPNSENDBUF+1
		MOVLW	0x00
		XORWF	XPN_XOR
		MOVFF	WREG,XPNSENDBUF+2
		; loco speed
		MOVFF	XPNREADBUF+2,WREG
		XORWF	XPN_XOR
		MOVFF	WREG,XPNSENDBUF+3
		; loco id
		MOVFF	XPNREADBUF+3,WREG
		MOVFF	WREG,XPNSENDBUF+4
		XORWF	XPN_XOR,W
		MOVFF	WREG,XPNSENDBUF+5
		CALL	DEBUG_XPNSENDBUF_CAN
		;
		BRA	XPN_DATA_SEND

XPN_DATA_SEND
		; we assume that the RS485 bus is free so let's do it
		BSF     PORTC,0		; enable RS485 (DE) transceiver
		MOVF	XPNSLOTNUMBER,W
		IORLW	XPN_CALL_RESPONSE
		MOVWF	XPN_SEND_DATA
		XPN_PARITY		; calculate parity
		BSF	TXSTA1,TX9D	; address byte (9th bit set)
		MOVWF	TXREG1
		RETURN

		; Slot              XOR
		; 0x160  0x61 0x00 0x61  Broadcast all off
		; 0x160  0x61 0x01 0x60  Broadcast all on
		; 0x160  0x81 0x00 0x81  Broadcast all locos off
XPN_BROADCAST_SEND
		; we assume that the RS485 bus is free so let's do it
		BSF     PORTC,0
		MOVLW	XPN_CALL_BROADCAST
		BSF	TXSTA1,TX9D	; address byte (set)
		MOVWF	TXREG1
		RETURN

;---------------------------------------------------
; periodic task - will be called every ~4 ms

XPNSLOT
		BSF	PORTC,0		; set RS485 transceiver (DE) on
		CLRF	XPN_RCNT
		CLRF	XPN_SCNT
		LFSR	FSR0,XPNREADBUF
		MOVFF	FSR0H,XPNREADP
		MOVFF	FSR0L,XPNREADP+1
		LFSR	FSR0,XPNSENDBUF
		MOVFF	FSR0H,XPNSENDP
		MOVFF	FSR0L,XPNSENDP+1

		; check if a new power on/off message arrived (bit 7 set)
		BTFSS	POWER_ONOFF,7
		BRA	XPNSLOT_NORMAL

		BCF	POWER_ONOFF,7
		; there is a power change -> send a broadcast (address 0)
		MOVLW	3
		MOVWF	XPN_SCNT
		MOVLW	0x61
		MOVFF	WREG,XPNSENDBUF
		MOVLW	1
		BTFSS	POWER_ONOFF,0
		XORLW	1
		MOVFF	WREG,XPNSENDBUF+1
		XORLW	0x61
		MOVFF	WREG,XPNSENDBUF+2
		CALL	DEBUG_XPNSENDBUF_CAN
		BRA	XPN_BROADCAST_SEND

XPNSLOT_NORMAL
		INCF	XPNSLOTNUMBER,W
		ANDLW	0x07
		; set 0 to 1
		BTFSC	STATUS,Z
		IORLW	1
		MOVWF	XPNSLOTNUMBER
		IORLW	XPN_CALL_INQUIRY
		MOVWF   XPN_SEND_DATA
		XPN_PARITY
		BSF	TXSTA1,TX9D
		MOVWF	TXREG1
		RETURN

; ----------------------------------


DEBUG_XPNREADBUF_CAN
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x84 as Debug Message
                CLRF	POSTINC0
                MOVLW	0x84
                MOVWF	POSTINC0
                MOVFF	CANHASH_HIGH,POSTINC0
                MOVFF	CANHASH_LOW,POSTINC0
                MOVLW	8
                MOVWF	POSTINC0
                MOVFF	XPNREADBUF,POSTINC0
                MOVFF	XPNREADBUF+1,POSTINC0
                MOVFF	XPNREADBUF+2,POSTINC0
                MOVFF	XPNREADBUF+3,POSTINC0
                MOVFF	XPNREADBUF+4,POSTINC0
                MOVFF	XPNREADBUF+5,POSTINC0
                MOVFF	XPN_SCNT,POSTINC0
                MOVFF	XPN_RCNT,INDF0
                GOTO	EVENT_RING_NORMALIZE

DEBUG_XPNSENDBUF_CAN
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x84 as Debug Message
                CLRF	POSTINC0
                MOVLW	0x85
                MOVWF	POSTINC0
                MOVFF	CANHASH_HIGH,POSTINC0
                MOVFF	CANHASH_LOW,POSTINC0
                MOVLW	8
                MOVWF	POSTINC0
                MOVFF	XPNSENDBUF,POSTINC0
                MOVFF	XPNSENDBUF+1,POSTINC0
                MOVFF	XPNSENDBUF+2,POSTINC0
                MOVFF	XPNSENDBUF+3,POSTINC0
                MOVFF	XPNSENDBUF+4,POSTINC0
                MOVFF	XPNSENDBUF+5,POSTINC0
                MOVFF	XPNSENDBUF+6,POSTINC0
                MOVFF	XPN_SCNT,INDF0
                GOTO	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
