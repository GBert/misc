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
                XPNCALLCOUNT : 1
                XPNTMP : 1		; tmp buffer
                XPNCNT : 1		; event counter
                EVENTXPN : 1		; event counter
                XPN_SEND_DATA : 1
                XPNPP : 2		; event pointer
                XPNPG : 2		; event pointer
                ENDC


;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

XPNBUF          EQU 0x800		; event ring

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

                CLRF    XPNCALLCOUNT

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

INITXPNTRING

                ; CLRF	XPNCNT		; event counter
		MOVLW	0
		MOVFF	W,XPNCNT
                LFSR	FSR0,XPNBUF
                MOVFF	FSR0H,XPNPP
                MOVFF	FSR0H,XPNPG
                MOVFF	FSR0L,XPNPP+1
                MOVFF	FSR0L,XPNPG+1
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

XPNTESTREADY
		BTFSC	TXSTA1,TRMT
		BCF     PORTC,0
		RETURN

XPNTEST
		BSF	PORTC,0
		INCF	XPNCALLCOUNT,W
		ANDLW	0x07
		; set 0 to 1
		BTFSC	STATUS,Z
		IORLW	1
		MOVWF	XPNCALLCOUNT
		IORLW	XPN_CALL_INQUIRY
		MOVWF   XPN_SEND_DATA
		XPN_PARITY
		BSF	TXSTA1,TX9D
		MOVWF	TXREG1
		RETURN

;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
