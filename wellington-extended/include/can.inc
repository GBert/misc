;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   CAN Bus functions
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
; CAN Bus settings
;------------------------------------------------------------------------------

#IF CAN != PORTB
    #IF CAN != PORTC
        ERROR   "CAN port not configured"
    #ENDIF
#ENDIF

#IFNDEF CANRATE
    ERROR       "CAN Bus rate not defined"
#ENDIF

#IF CLOCK != 40000000
    #IF CLOCK != 64000000
        ERROR   "CAN Bus rate not calculated"
    #ENDIF
#ENDIF

#IF CLOCK == 40000000
    ; http://www.port.de/en/bit-timing.html
    ;
    ; CAN Rate 20, Sample Point is 87.5%
    ; Fcan = 0.5 * Fosc, SEG2PHTS = 1, SAM = 0
    ;
    ; 500   5   8   6   1   87.5    0x04    0x84    0x00
    ; 250   5   16  13  2   87.5    0x04    0xbc    0x01
    ; 125   10  16  13  2   87.5    0x09    0xbc    0x01
    ; 100   25  8   6   1   87.5    0x18    0x84    0x00
    ; 50    25  16  13  2   87.5    0x18    0xbc    0x01
    #IF CANRATE == 500
        #DEFINE BRG1 0x04
        #DEFINE BRG2 0x84
        #DEFINE BRG3 0x00
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x04
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x09
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x18
        #DEFINE BRG2 0x84
        #DEFINE BRG3 0x00
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x18
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
#ENDIF

#IF CLOCK == 64000000
    ; http://www.port.de/en/bit-timing.html
    ;
    ; CAN Rate 32, Sample Point is 87.5%
    ; Fcan = 0.5 * Fosc, SEG2PHTS = 1, SAM = 0
    ;
    ; 1000  2   16  13  2   87.5    0x01    0xbc    0x01
    ; 500   4   16  13  2   87.5    0x03    0xbc    0x01
    ; 250   8   16  13  2   87.5    0x07    0xbc    0x01
    ; 125   16  16  13  2   87.5    0x0f    0xbc    0x01
    ; 100   20  16  13  2   87.5    0x13    0xbc    0x01
    ; 50    40  16  13  2   87.5    0x27    0xbc    0x01
    #IF CANRATE == 1000
        #DEFINE BRG1 0x01
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 500
        #DEFINE BRG1 0x03
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x07
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x0F
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x13
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x27
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
#ENDIF

#IFNDEF REQOP0
REQOP0          EQU     H'0005'
REQOP1          EQU     H'0006'
REQOP2          EQU     H'0007'
#ENDIF
#IFNDEF RXB0DBEN
RXB0DBEN        EQU     H'0002'
#ENDIF

;------------------------------------------------------------------------------
; Init. CAN Bus
;------------------------------------------------------------------------------
InitComms
                ; The CAN module uses the RB2/CANTX and RB3/CANRX pins to
                ; interface with the CAN bus. In normal mode, the CAN module
                ; automatically overrides TRISB<2>. The user must ensure that
                ; TRISB<3> is set.
#IF CAN == PORTC
                CLRF    TRISC
                BSF     TRISC,7             ; CANRX I/P
#ELSE
                CLRF    TRISB
                BSF     TRISB,3             ; CANRX I/P
#ENDIF
                ; Set configuration mode
                BANKSEL CANCON
                MOVLW   (1 << REQOP2)
                MOVWF   CANCON
InitComms1
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2)
                BZ      InitComms1

                ; Set mask/filter
                ;
                ; Mask  Filter  Message
                ; 0     x       x       accept
                ; 1     0       0       accept
                ; 1     0       1       reject
                ; 1     1       0       reject
                ; 1     1       1       accept

                ; Set receive masks
                BANKSEL RXM0SIDH
                MOVLW   0xFF                ; RXB0 standard filters 0/1
                MOVWF   RXM0SIDH
                MOVLW   0xE0
                MOVWF   RXM0SIDL

                CLRF    RXM1SIDH            ; RXB1 extended filters 2/3/4/5 
                CLRF    RXM1SIDL
                CLRF    RXM1EIDH            
                CLRF    RXM1EIDL

                ; Set standard receive filters for RXB0
                BANKSEL RXF0SIDH
                MOVLW   (CANID >> 3)
                MOVWF   RXF0SIDH
                MOVWF   RXF1SIDH
                MOVLW   LOW (CANID << 5)    ; EXIDEN = 0
                MOVWF   RXF0SIDL
                MOVWF   RXF1SIDL

                ; Set extended receive filters for RXB1
                BANKSEL RXF2SIDH
                MOVLW   (1 << EXIDEN)       ; EXIDEN = 1
                MOVWF   RXF2SIDL    
                MOVWF   RXF3SIDL
                MOVWF   RXF4SIDL
                MOVWF   RXF5SIDL

                ; Set bit rate
                BANKSEL BRGCON1
                MOVLW   BRG1
                MOVWF   BRGCON1
                MOVLW   BRG2
                MOVWF   BRGCON2
                MOVLW   BRG3
                MOVWF   BRGCON3

                ; Enable CANTX
                BANKSEL CIOCON
                BSF     CIOCON,ENDRHI

                ; Set normal mode
                CLRF    CANCON
InitComms2
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2) + (1 << OPMODE1)
                BNZ     InitComms2

                MOVLB   0
                RETURN

;------------------------------------------------------------------------------
; CAN Bus Receive
;------------------------------------------------------------------------------
Receive
                BEGINTO TIMEOUT
RecvLoop
                BANKSEL RXB0CON
                BTFSC   RXB0CON,RXFUL
                BRA     Recv0
                MOVLB   0
                LOOPTO  RecvLoop
                BRA     GOTOAPP             ; Timed-out
Recv0
                MOVF    RXB0D0,W
                BCF     RXB0CON,RXFUL
                MOVLB   0
                RETURN

;------------------------------------------------------------------------------
; CAN Bus Send
;------------------------------------------------------------------------------
Send
                MOVWF   wtemp               ; Save W

                BEGINTO TIMEOUT
SendLoop
                BANKSEL TXB0CON
                BTFSS   TXB0CON,TXREQ
                BRA     SendData
                MOVLB   0
                LOOPTO  SendLoop
                BRA     GOTOAPP             ; Timed-out
SendData
                MOVLB   0
                MOVF    wtemp,W             ; Restore W

                ; Store Output Data
                BANKSEL TXB0D0
                MOVWF   TXB0D0

                ; Set Standard Transmit Id
                BANKSEL TXB0SIDH
                MOVLW   (CANID >> 3)
                MOVWF   TXB0SIDH
                MOVLW   LOW (CANID << 5)
                MOVWF   TXB0SIDL

                ; Set DLC To 1 Byte
                BANKSEL TXB0DLC
                MOVLW   1
                MOVWF   TXB0DLC

                ; Mark For Transmission
                BANKSEL TXB0CON
                MOVLW   (1 << TXREQ)
                MOVWF   TXB0CON

                MOVLB   0

                MOVLW   CANDELAY
                MOVWF   timer1
SendDelay
                DECF    timer1,F
                BNZ     SendDelay
                CLRWDT

                RETURN

;------------------------------------------------------------------------------    
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
