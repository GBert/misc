;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   Software UART functions
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
; Software UART Settings
;-------------------------------------------------------------------------------

CBLOCK
    UREG        : 1
    UCNT        : 1
ENDC

WAIT            MACRO
#IF TYPE != 3
                BCF     INTCON,TMR0IF
                BTFSS   INTCON,TMR0IF
#ELSE
                BANKSEL PIR0                ; BANK 14
                BCF     PIR0,TMR0IF
                BTFSS   PIR0,TMR0IF
#ENDIF
                BRA     $-1
                ENDM

;-------------------------------------------------------------------------------
; Software UART Baud Rate Generator 256 - (FOSC / 4 / PRE / BAUDRATE) + 1
;-------------------------------------------------------------------------------

                CONSTANT UMICROS = CLOCK / 4 / 4 / RS485_BAUDRATE
                CONSTANT UBAUD   = 256 - UMICROS + 1
                CONSTANT UDELAY  = 256 - (UMICROS + UMICROS / 2) + 6
                CONSTANT USTOP   = 256 - (UMICROS + UMICROS)
#IF UBAUD > 255
    ERROR       "UBAUD overflow"
#ENDIF
#IF UBAUD < 0
    ERROR       "UBAUD underflow"
#ENDIF
#IF UDELAY > 255
    ERROR       "UDELAY overflow"
#ENDIF
#IF UDELAY < 0
    ERROR       "UDELAY underflow"
#ENDIF
#IF USTOP > 255
    ERROR       "USTOP overflow"
#ENDIF
#IF USTOP < 0
    ERROR       "USTOP underflow"
#ENDIF

;-------------------------------------------------------------------------------
; Init. Software UART
;
;  Modifies BSR
;-------------------------------------------------------------------------------
InitComms
#IF TYPE != 3
                BANKSEL OPTION_REG          ; BANK 1
                MOVLW   B'00000001'         ; PRE=1:4
                MOVWF   OPTION_REG
#ELSE
                BANKSEL T0CON1              ; BANK 11
                MOVLW   B'00000010'         ; PRE=1:4
                MOVWF   T0CON1
                BSF     T0CON0,T0EN
#ENDIF
                BANKSEL UOUT                ; BANK 2 | 0
                BSF     UOUT,UTXD
                BANKSEL UDDR                ; BANK 1 | 0
                BCF     UDDR,UTXD
                BSF     UDDR,URXD
                RETURN

;-------------------------------------------------------------------------------
; Software UART Receive
;
;  Modifies BSR
;-------------------------------------------------------------------------------
Receive
                MOVLW   8
                MOVWF   UCNT
                BEGINTO TIMEOUT
RecvLoop
                BANKSEL UINP                ; BANK 0
                BTFSS   UINP,URXD
                BRA     Received
                LOOPTO  RecvLoop
                ;BRA     RUNAPP              ; Timed-out
Received
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   UDELAY
                MOVWF   TMR0
                WAIT                        ; BANK 14?
RecvBit
                BANKSEL UINP                ; BANK 0
                BSF     STATUS,C
                BTFSS   UINP,URXD
                BCF     STATUS,C
                RRF     UREG,F
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   UBAUD
                ADDWF   TMR0,F
                WAIT                        ; BANK 14?
                DJNZ    UCNT,RecvBit
;RecvStop
#IF USTP == 1
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   UBAUD
                ADDWF   TMR0,F
                WAIT                        ; BANK 14?
#ENDIF
#IF USTP == 2
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   USTOP
                ADDWF   TMR0,F
                WAIT                        ; BANK 14?
#ENDIF
                MOVF    UREG,W
                ;LEDON                       ; BOOT LED ON
                ADDWF   checksum,F
                RETURN

;-------------------------------------------------------------------------------
; Software UART Send
;
;  Modifies BSR
;-------------------------------------------------------------------------------
Send
                MOVWF   UREG
                MOVLW   8
                MOVWF   UCNT
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   UBAUD
                MOVWF   TMR0
                BANKSEL UOUT                ; BANK 2 | 0
                BCF     UOUT,UTXD
                WAIT                        ; BANK 14?
SendBit
                BANKSEL UOUT                ; BANK 2 | 0
                RRF     UREG,F
                BTFSS   STATUS,C
                BCF     UOUT,UTXD
                BTFSC   STATUS,C
                BSF     UOUT,UTXD
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   UBAUD
                ADDWF   TMR0,F
                WAIT                        ; BANK 14?
                DJNZ    UCNT,SendBit
;SendStop
                BANKSEL UOUT                ; BANK 2 | 0
                BSF     UOUT,UTXD
                BANKSEL TMR0                ; BANK 0 | 11
                MOVLW   USTOP
                ADDWF   TMR0,F
                WAIT                        ; BANK 14?
                RETURN

;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
