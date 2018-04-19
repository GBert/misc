;------------------------------------------------------------------------------
;
; My GFP UART
;
; Copyright (c) 2018 Gerhard Bertelsmann
; Copyright (c) 2014 Darron M Broad
;
;------------------------------------------------------------------------------

		LIST
		RADIX   DEC

;------------------------------------------------------------------------------
; UART Baud Rate Generator (FOSC / BAUDRATE / 4 - 1)
;------------------------------------------------------------------------------

#DEFINE UBAUD   ((((CLOCK / BAUDRATE) / 2) - 1) / 2)
#IF UBAUD > 65535
    ERROR       "UART BRG overflow"
#ENDIF
#IF UBAUD == 0
    ERROR       "UART BRG zero"
#ENDIF

;------------------------------------------------------------------------------
;		highly optimized - don't change !
		CBLOCK	0x01A0
		RX_RING	: 32	; 0x01A0
		TX_RING	: 32	; 0x01C0
		RX_HEAD	: 1	; 0x01E0
		RX_TAIL	: 1	; 0x01E1
		TX_HEAD	: 1	; 0x01E2
		TX_TAIL	: 1	; 0x01E3
		ENDC

;------------------------------------------------------------------------------
; Init. UART
;------------------------------------------------------------------------------

INITBUFFER	BANKSEL	RX_RING
		MOVLW	HIGH RX_RING
		MOVWF	FSR0H
		MOVWF	FSR1H
		MOVLW   LOW RX_RING
		MOVWF	RX_HEAD
		MOVWF	RX_TAIL
		MOVLW   LOW TX_RING
		MOVWF	TX_HEAD
		MOVWF	TX_TAIL
		RETURN

INITUART
		BANKSEl	BAUDCTL
                MOVLW   (1 << BRG16)
                MOVWF   BAUDCTL

                MOVLW   HIGH (UBAUD)
                MOVWF   SPBRGH
                MOVLW   LOW  (UBAUD)
                MOVWF   SPBRG

                ; Enable Transmit + High Speed Mode + BIT9 = 1
                MOVLW   (1 << TXEN) + (1 << BRGH) + (1 << TX9D)
                MOVWF   TXSTA
INITUARTAGAIN
                ; Disable Rx ring ISR
                BCF     PIE1,RCIE

                ; Enable Serial Port + Disable Receiver
                MOVLW   (1 << SPEN)
                MOVWF   RCSTA

                ; Enable Receiver
                BSF     RCSTA,CREN

                ; Flush Receiver
                MOVF    RCREG,W
                MOVF    RCREG,W
                MOVF    RCREG,W
                
                ; Enable Rx ring ISR
                BSF     PIE1,RCIE

                RETURN

;------------------------------------------------------------------------------
; Reset UART Conditionally
;------------------------------------------------------------------------------
RESETUART
		BANKSEL	RCSTA
                BTFSC   RCSTA,OERR         ; Detect UART Overrun Error
                BRA     INITUARTAGAIN

                BTFSC   RCSTA,FERR         ; Detect UART Framing Error
                BRA     INITUARTAGAIN

                RETURN

