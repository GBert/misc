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
; Init. UART
;------------------------------------------------------------------------------
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

