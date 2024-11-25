;------------------------------------------------------------------------------
;
; Can-Can CAN TX
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
; Receive message on UART and send on CAN bus
;------------------------------------------------------------------------------
TXCAN
;  Check is done in MAIN (can-can.inc)
;                MOVF    RXCNT,W             ; If Rx packet counter > 0
;                BNZ     TXCAN1
;                RETURN

;------------------------------------------------------------------------------
TXCAN1
                ; Find a Tx slot
                MOVLW   CAN_WIN_TXB0        ; Window mode on TXB0
                MOVWF   CANCON
                BTFSS   TXB0CON,TXREQ
                BRA     TXCAN2

                MOVLW   CAN_WIN_TXB1        ; Window mode on TXB1
                MOVWF   CANCON
                BTFSS   TXB1CON,TXREQ
                BRA     TXCAN2

                MOVLW   CAN_WIN_TXB2        ; Window mode on TXB2
                MOVWF   CANCON
                BTFSC   TXB2CON,TXREQ
                RETURN

;------------------------------------------------------------------------------
TXCAN2
                GETC                        ; W = <RXBUF>,RXGET++

                MOVWF   CANCMD              ; T?
                XORLW   EXTDATA
                BZ      TXCANEXT

                MOVWF   CANCMD              ; t?
                XORLW   STDDATA
                BZ      TXCANSTD

                MOVF    CANCMD,W            ; R?
                XORLW   EXTRTR
                BZ      TXCANEXT

                MOVF    CANCMD,W            ; r?
                XORLW   STDRTR
                BZ      TXCANSTD

                MOVF    CANCMD,W            ; S?
                XORLW   CANSET
                BZ      CANSETBAUD

                BRA     TXCANEND            ; Unsupported

;------------------------------------------------------------------------------
TXCANSTD
                RCALL   ASC2NIB             ; GETC
                MOVWF   CANFRAME
                TXSID_10_8                  ; Set SID10..8

                LFSR    FSR0,CANID
                RCALL   ASC2BIN             ; GETC, GETC

                MOVF    CANID,W
                MOVWF   CANFRAME+1
                TXSID_7_3                   ; Set SID7..3

                MOVF    CANID,W
                TXSID_2_0                   ; Set SID2..0

                BRA     TXCAN3

;------------------------------------------------------------------------------
TXCANEXT
                LFSR    FSR0,CANID
                RCALL   ASC2BIN             ; GETC, GETC

                MOVF    CANID,W
                MOVWF   CANFRAME
                BSF     CANFRAME,7          ; mark as extended frame
                TXEXT_28_24                 ; Set EID28..24

                LFSR    FSR0,CANID
                RCALL   ASC2BIN             ; GETC, GETC

                MOVF    CANID,W
                MOVWF   CANFRAME+1
                TXEXT_23_21                 ; Set EID23..21

                MOVF    CANID,W
                TXEXT_20_18                 ; Set EID20..18

                MOVF    CANID,W
                TXEXT_17_16                 ; Set EID17..16

                BSF     RXB0SIDL,EXIDEN     ; Enable Extended Id

                LFSR    FSR0,RXB0EIDH       ; Set EID15..8
                RCALL   ASC2BIN             ; GETC, GETC
                MOVWF   CANFRAME+2

                LFSR    FSR0,RXB0EIDL       ; Set EID7..0
                RCALL   ASC2BIN             ; GETC, GETC
                MOVWF   CANFRAME+3

;------------------------------------------------------------------------------
TXCAN3
                CLRF    CANFRAME+4
                CLRF    RXB0DLC             ; Set RTR
                BTFSC   CANCMD,1            ; r or R?
                BSF     RXB0DLC,TXRTR

                RCALL   ASC2NIB             ; GETC
                BZ      TXCAN5

                MOVWF   CANCNT
                MOVWF   CANFRAME+4
                IORWF   RXB0DLC,F           ; Set DLC

;------------------------------------------------------------------------------

                LFSR    FSR0,RXB0D0
TXCAN4
                RCALL   ASC2BIN             ; Set data
                DJNZ    CANCNT,TXCAN4

                MOVFF   RXB0D0,CANFRAME+5
                MOVFF   RXB0D1,CANFRAME+6
                MOVFF   RXB0D2,CANFRAME+7
                MOVFF   RXB0D3,CANFRAME+8
                MOVFF   RXB0D4,CANFRAME+9
                MOVFF   RXB0D5,CANFRAME+10
                MOVFF   RXB0D6,CANFRAME+11
                MOVFF   RXB0D7,CANFRAME+12

;------------------------------------------------------------------------------
TXCAN5
                BSF     RXB0CON,TXREQ       ; Tx

;------------------------------------------------------------------------------
TXCANEND
                BSF     CANFRAME,6          ; mark as new frame
                GETC                        ; W = <RXBUF>,RXGET++
                XORLW   EOLNCHAR
                BNZ     TXCANEND

                DECF    RXCNT,F             ; Rx packet counter--
                BNZ     TXCANRET            ; Flow control off?

                ; Clear to Send; RXCNT = 0
                RTSCLR
TXCANRET
                RETURN

;------------------------------------------------------------------------------
CANSETBAUD
                ; disable interrupts
                BCF     INTCON,GIE

                ; set TBL pointer
                MOVLW   UPPER CANRATE
                MOVWF   TBLPTRU
                MOVLW   HIGH CANRATE
                MOVWF   TBLPTRH
                MOVLW   LOW CANRATE
                MOVWF   TBLPTRL

                GETC                        ; W = <RXBUF>,RXGET++
                ANDLW   0x0f
                RLNCF   WREG,W
                RLNCF   WREG,W
                ADDWF   TBLPTRL,F
                BTFSC   STATUS,C
                INCF    TBLPTRH,F

                ; Set configuration mode
                BANKSEL CANCON
                MOVLW   (1 << REQOP2)
                MOVWF   CANCON
WAIT_SETUP_MODE
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2)
                BZ      WAIT_SETUP_MODE

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
CAN_WAIT_NORMAL
                MOVF    CANSTAT,W
                ANDLW   (1 << OPMODE2) + (1 << OPMODE1)
                BNZ     CAN_WAIT_NORMAL

                MOVLB   15

CANSETINITEND
                GETC                        ; W = <RXBUF>,RXGET++
                XORLW   EOLNCHAR
                BNZ     CANSETINITEND
	        ; enable interrupts again
                BSF     INTCON,GIE
                RETURN

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
