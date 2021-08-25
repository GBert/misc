;------------------------------------------------------------------------------
;
; Can-Can CAN
;
; Copyright (c) 2014 Darron M Broad
;
;------------------------------------------------------------------------------
;
; This file is part of the Can-Can CAN bus interface project.
;
; Can-Can is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; CAN Bus variables
;------------------------------------------------------------------------------
    
                CBLOCK
                CANCMD  : 1
                CANID   : 1
                CANCNT  : 1
                ENDC

;------------------------------------------------------------------------------
; CAN Bus settings
;------------------------------------------------------------------------------

#IF CAN != PORTB
    #IF CAN != PORTC
        ERROR   "CAN port not configured"
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

#DEFINE         CAN_WIN_OFF  0x00           ; 000X
#DEFINE         CAN_WIN_RXB0 0x0C           ; 110X
#DEFINE         CAN_WIN_RXB1 0x0A           ; 101X
#DEFINE         CAN_WIN_TXB0 0x08           ; 100X
#DEFINE         CAN_WIN_TXB1 0x06           ; 011X
#DEFINE         CAN_WIN_TXB2 0x04           ; 010X

#INCLUDE        "canrates-rom.inc"
#INCLUDE        "canid.inc"

;------------------------------------------------------------------------------
; Init. CAN bus
;------------------------------------------------------------------------------
INITCAN
                BANKSEL LATC
#IF CAN == PORTC
                BSF     LATC,6              ; set to high before switching to output
                BCF     TRISC,6             ; CANTX I/P
                BSF     TRISC,7             ; CANRX I/P
#ELSE
                BSF     LATB,2              ; set to high before switching to output
                BCF     TRISB,2             ; CANTX I/P
                BSF     TRISB,3             ; CANRX I/P
#ENDIF

                MOVLW   UPPER CANRATE_ROM_DEFAULT
                MOVWF   TBLPTRU
                MOVLW   HIGH CANRATE_ROM_DEFAULT
                MOVWF   TBLPTRH
                MOVLW   LOW CANRATE_ROM_DEFAULT
                MOVWF   TBLPTRL

SETUP_BAUDRATE
                ; Set configuration mode
                MOVLW   (1 << REQOP2)
                MOVWF   CANCON
INITCAN1
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2)
                BZ      INITCAN1

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
                CLRF    RXM0SIDH            ; RXB0 extended filters 0/1
                CLRF    RXM0SIDL
                CLRF    RXM0EIDH
                CLRF    RXM0EIDL

                CLRF    RXM1SIDH            ; RXB1 standard filters 2/3/4/5
                CLRF    RXM1SIDL

                ; Set extended receive filters for RXB0
                BANKSEL RXF0SIDH
                CLRF    RXF0SIDH
                CLRF    RXF0EIDH
                CLRF    RXF0EIDL
                CLRF    RXF1SIDH
                CLRF    RXF1EIDH
                CLRF    RXF1EIDL
                MOVLW   (1 << EXIDEN)       ; EXIDEN = 1
                MOVWF   RXF0SIDL
                MOVWF   RXF1SIDL

                ; Set standard receive filters for RXB1
                BANKSEL RXF2SIDH
                CLRF    RXF2SIDH
                CLRF    RXF3SIDH
                CLRF    RXF4SIDH
                CLRF    RXF5SIDH
                CLRF    RXF2SIDL            ; EXIDEN = 0
                CLRF    RXF3SIDL
                CLRF    RXF4SIDL
                CLRF    RXF5SIDL

                ; Set bit rate
                BANKSEL BRGCON1
                TBLRD+*                     ; ignore first entry
                MOVFF   TABLAT,BRGCON1
                TBLRD+*
                MOVFF   TABLAT,BRGCON2
                TBLRD+*
                MOVFF   TABLAT,BRGCON3

                ; Enable CANTX
                BANKSEL CIOCON
                BSF     CIOCON,ENDRHI

                ; Reset receivers
                CLRF    RXB0CON
                CLRF    RXB1CON

                ; Set normal mode
                CLRF    CANCON
INITCAN2
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2) + (1 << OPMODE1)
                BNZ     INITCAN2

                RETURN

;------------------------------------------------------------------------------
; Serial Line CAN (SLCAN) Protocol 
;------------------------------------------------------------------------------

; TSSSEEEEEX1122334455667788\r
; |   |    |       |        |
; |   ID   DLC     DATA     EOLN
; |
; t--data frame with 11-bit ID
; r--RTR  frame with 11-bit ID
; T--data frame with 29-bit ID
; R--RTR  frame with 29-bit ID

#DEFINE         STDDATA     't'             ; 0x74 0111 0100
#DEFINE         STDRTR      'r'             ; 0x72 0111 0010
#DEFINE         EXTDATA     'T'             ; 0x54 0101 0100
#DEFINE         EXTRTR      'R'             ; 0x52 0101 0010
#DEFINE         CANSET      'S'             ; 0x53 0101 0001

; C\r
; |
; O | C | F

#DEFINE         OPENCMD     'O'
#DEFINE         CLOSECMD    'C'
#DEFINE         READSTATUS  'F'

;------------------------------------------------------------------------------
; CAN bus functions
;------------------------------------------------------------------------------

#INCLUDE        "canrx.inc"
#INCLUDE        "cantx.inc"

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
