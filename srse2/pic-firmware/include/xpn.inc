;------------------------------------------------------------------------------
;
; XpressNet frames send ring
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
                XPNTMP : 1		; tmp buffer
                XPNCNT : 1		; event counter
                ENDC

XPNPP   EQU   0x30c           ; event pointer
XPNPG   EQU   0x30d           ; event pointer

;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

XPNBUF          EQU 0x400		; event ring
                CBLOCK 0x800
                XPNBUFFER : 1024
                ENDC

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
; Init. UART
;------------------------------------------------------------------------------
INITXPNUART
                BCF     TRISC,6             ; UART1 TX
                BSF     TRISC,7             ; UART1 RX

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
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
