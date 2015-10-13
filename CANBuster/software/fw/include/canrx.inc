;------------------------------------------------------------------------------
;
; Can-Can CAN RX
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
; Receive message on CAN bus and send on UART
;------------------------------------------------------------------------------
RXCAN
                ; Find an Rx slot
                CLRF    CANCON              ; Window mode off
                BTFSC   RXB0CON,RXFUL
                BRA     RXCANLIMIT

                MOVLW   CAN_WIN_RXB1        ; Window mode on RXB1
                MOVWF   CANCON
                BTFSS   RXB1CON,RXFUL
                RETURN

;------------------------------------------------------------------------------
RXCANLIMIT
                ; If Tx packet counter = trigger Then Drop Packet
                MOVF    TXCNT,W
                XORLW   OVERFLOW
                BNZ     RXPACKET

                CANOVFL                     ; CAN packet status LED
                BCF     RXB0CON,RXFUL       ; Packet dropped
                RETURN

;------------------------------------------------------------------------------
RXPACKET
                BTFSC   RXB0SIDL,EXID
                BRA     RXEXT

;------------------------------------------------------------------------------
RXSTD
                MOVLW   STDDATA             ; Standard data
                BTFSC   RXB0DLC,RXRTR
                MOVLW   STDRTR              ; Standard rtr
                PUTC                        ; Tx
 
                RXSID_10_8                  ; Get SID10..8
                CALL    NIB2ASC             ; Tx

                RXSID_7_0                   ; Get SID7..0
                CALL    BIN2ASC             ; Tx

                BRA     RXCANDLC

;------------------------------------------------------------------------------
RXEXT
                MOVLW   EXTDATA             ; Extended data
                BTFSC   RXB0DLC,RXRTR
                MOVLW   EXTRTR              ; Extended rtr
                PUTC                        ; Tx
 
                RXEXT_28_24                 ; Get EID28..EID24
                CALL    BIN2ASC             ; Tx

                RXEXT_23_16                 ; Get EID23..EID16
                CALL    BIN2ASC             ; Tx

                MOVF    RXB0EIDH,W          ; Get EID15..EID8
                CALL    BIN2ASC             ; Tx

                MOVF    RXB0EIDL,W          ; Get EID7..EID0
                CALL    BIN2ASC             ; Tx

;------------------------------------------------------------------------------
RXCANDLC
                MOVF    RXB0DLC,W           ; Get DLC
                ANDLW   0x0F
                MOVWF   CANCNT
                IORLW   0x30
                PUTC                        ; Tx

;------------------------------------------------------------------------------

                MOVF    CANCNT,W            ; Get message
                BZ      RXCANEOLN

                LFSR    FSR0,RXB0D0
RXCANMESG
                MOVF    POSTINC0,W
                RCALL   BIN2ASC             ; Tx
                DJNZ    CANCNT,RXCANMESG

;------------------------------------------------------------------------------
RXCANEOLN

                MOVLW   EOLNCHAR
                PUTC                        ; Tx

                INCF    TXCNT,F             ; Tx packet counter++
                BSF     UPIE,UTXIE          ; Enable Tx ring ISR

;------------------------------------------------------------------------------

                CANRECV                     ; CAN packet status LED
                BCF     RXB0CON,RXFUL       ; Packet received
                RETURN

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
