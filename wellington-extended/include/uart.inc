;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   UART functions
;
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   This file is part of The Wellington Boot Loader.
;
;   The Wellington Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Wellington Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
;------------------------------------------------------------------------------

                LIST
                RADIX       DEC

;------------------------------------------------------------------------------
; UART settings
;------------------------------------------------------------------------------

#IF UART != PORTC
    #IF UART != PORTB
        ERROR   "UART port not configured"
    #ENDIF
#ENDIF

#IFNDEF BAUDRATE
    ERROR       "UART baud rate not defined"
#ENDIF

#IF UART == PORTC
    #IFNDEF TXSTA1
        #DEFINE UPIR        PIR1
        #DEFINE URCIF       RCIF
        #DEFINE UTXIF       TXIF

        #DEFINE UPIE        PIE1
        #DEFINE URCIE       RCIE
        #DEFINE UTXIE       TXIE

        #DEFINE UIPR        IPR1
        #DEFINE URCIP       RCIP
        #DEFINE UTXIP       TXIP

        #DEFINE UTXSTA      TXSTA
        #DEFINE URCSTA      RCSTA
        #DEFINE UTXREG      TXREG
        #DEFINE URCREG      RCREG

        #DEFINE UBAUDCON    BAUDCON
        #DEFINE USPBRGH     SPBRGH
        #DEFINE USPBRG      SPBRG
    #ELSE
        #DEFINE UPIR        PIR1
        #DEFINE URCIF       RC1IF
        #DEFINE UTXIF       TX1IF

        #DEFINE UPIE        PIE1
        #DEFINE URCIE       RC1IE
        #DEFINE UTXIE       TX1IE

        #DEFINE UIPR        IPR1
        #DEFINE URCIP       RC1IP
        #DEFINE UTXIP       TX1IP

        #DEFINE UTXSTA      TXSTA1
        #DEFINE URCSTA      RCSTA1
        #DEFINE UTXREG      TXREG1
        #DEFINE URCREG      RCREG1

        #DEFINE UBAUDCON    BAUDCON1
        #DEFINE USPBRGH     SPBRGH1
        #DEFINE USPBRG      SPBRG1
    #ENDIF
#ENDIF

#IF UART == PORTB
        #DEFINE UPIR        PIR3
        #DEFINE URCIF       RC2IF
        #DEFINE UTXIF       TX2IF

        #DEFINE UPIE        PIE3
        #DEFINE URCIE       RC2IE
        #DEFINE UTXIE       TX2IE

        #DEFINE UIPR        IPR3
        #DEFINE URCIP       RC2IP
        #DEFINE UTXIP       TX2IP

        #DEFINE UTXSTA      TXSTA2
        #DEFINE URCSTA      RCSTA2
        #DEFINE UTXREG      TXREG2
        #DEFINE URCREG      RCREG2

        #DEFINE UBAUDCON    BAUDCON2
        #DEFINE USPBRGH     SPBRGH2
        #DEFINE USPBRG      SPBRG2
#ENDIF

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
InitCommsUART
                BANKSEL URCSTA

                MOVLW   (1 << BRG16)
                MOVWF   UBAUDCON

                MOVLW   HIGH (UBAUD)
                MOVWF   USPBRGH
                MOVLW   LOW  (UBAUD)
                MOVWF   USPBRG

                ; Enable 9-bit Mode + Enable Transmit + High Speed Mode + BIT9 = 1
                MOVLW   (1 << TX9) + (1 << TXEN) + (1 << BRGH) + (1 << TX9D)
                MOVWF   UTXSTA

                ; Enable Serial Port + Disable Receiver
                MOVLW   (1 << SPEN)
                MOVWF   URCSTA

                ; Enable Receiver
                BSF     URCSTA,CREN

                ; Flush Receiver
                MOVF    URCREG,W
                MOVF    URCREG,W
                MOVF    URCREG,W
                
                MOVLB   0
                RETURN

;------------------------------------------------------------------------------
; UART Receive
;------------------------------------------------------------------------------
Receive
                BEGINTO TIMEOUT
RecvLoop
                BTFSC   UPIR,URCIF
                BRA     Received
                LOOPTO  RecvLoop
                BRA     GOTOAPP             ; Timed-out
Received
                BANKSEL URCREG
                MOVF    URCREG,W
                MOVLB   0
                ADDWF   csum,F
                RETURN

;------------------------------------------------------------------------------
; UART Send
;------------------------------------------------------------------------------
Send
                BANKSEL UTXREG
                MOVWF   UTXREG
SendLoop        
                BTFSS   UTXSTA,TRMT
                BRA     SendLoop
                MOVLB   0
                RETURN

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
