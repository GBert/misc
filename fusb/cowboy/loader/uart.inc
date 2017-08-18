;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   UART functions
;
;   Copyright (c) 2017 Darron M Broad
;
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;   This file is part of The Cowboy Boot Loader.
;
;   The Cowboy Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Cowboy Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Cowboy Boot Loader. If not, see http://www.gnu.org/licenses/
;-------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;-------------------------------------------------------------------------------
; UART Settings
;-------------------------------------------------------------------------------

#IF UART != 1
    #IF UART != 2
        ERROR   "UART port not configured"
    #ENDIF
#ENDIF

#IFNDEF BAUDRATE
    ERROR       "UART baud rate not defined"
#ENDIF

#IF UART == 1
    #IFNDEF TX1IF
        #IF TYPE == 3
            #DEFINE UPIR    PIR3
        #ELSE
            #DEFINE UPIR    PIR1
        #ENDIF
        #DEFINE URCIF       RCIF
        #DEFINE UTXIF       TXIF

        #DEFINE UPIE        PIE1
        #DEFINE URCIE       RCIE
        #DEFINE UTXIE       TXIE

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
        #DEFINE UPIR        PIR3
        #DEFINE URCIF       RC1IF
        #DEFINE UTXIF       TX1IF

        #DEFINE UPIE        PIE3
        #DEFINE URCIE       RC1IE
        #DEFINE UTXIE       TX1IE

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

#IF UART == 2
        #DEFINE UPIR        PIR3
        #DEFINE URCIF       RC2IF
        #DEFINE UTXIF       TX2IF

        #DEFINE UPIE        PIE3
        #DEFINE URCIE       RC2IE
        #DEFINE UTXIE       TX2IE

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

;-------------------------------------------------------------------------------
; UART Baud Rate Generator (FOSC / BAUDRATE / 4 - 1)
;-------------------------------------------------------------------------------

#DEFINE UBAUD   ((((CLOCK / BAUDRATE) / 2) - 1) / 2)
#IF UBAUD > 65535
    ERROR       "UART BRG overflow"
#ENDIF
#IF UBAUD == 0
    ERROR       "UART BRG zero"
#ENDIF

;-------------------------------------------------------------------------------
; Init. UART
;
;  Modifies W, BSR
;-------------------------------------------------------------------------------
InitComms
                BANKSEL UBAUDCON            ;BANK 3 | 2
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

                RETURN

;-------------------------------------------------------------------------------
; UART Receive
;
;  Modifies BSR
;-------------------------------------------------------------------------------
Receive
                BEGINTO TIMEOUT
RecvLoop
                BANKSEL UPIR                ; BANK 0 | 14
                BTFSC   UPIR,URCIF
                BRA     Received
                LOOPTO  RecvLoop
                BRA     RUNAPP              ; Timed-out
Received
                BANKSEL URCREG              ; BANK 3 | 2
                MOVF    URCREG,W
                LEDON                       ; BOOT LED ON
                ADDWF   checksum,F
                RETURN

;-------------------------------------------------------------------------------
; UART Send
;
;  Modifies BSR
;-------------------------------------------------------------------------------
Send
#IFDEF UART_TXIF
                BANKSEL UPIR                ; BANK 0 | 14
SendLoop1
                BTFSS   UPIR,UTXIF
                BRA     SendLoop1
                NOP
#ENDIF
                BANKSEL UTXREG              ; BANK 3 | 2
                MOVWF   UTXREG
#IFNDEF UART_TXIF
SendLoop2
                BTFSS   UTXSTA,TRMT
                BRA     SendLoop2
#ENDIF
                RETURN

;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
