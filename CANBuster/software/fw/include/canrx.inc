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
; CAN frame storage
;------------------------------------------------------------------------------

                CBLOCK
                CANFRAME : 13
                ENDC

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
                LFSR    FSR0,CANFRAME
                BTFSC   RXB0SIDL,EXID
                BRA     RXEXT

;------------------------------------------------------------------------------
RXSTD
                MOVLW   STDDATA             ; Standard data
                BTFSC   RXB0DLC,RXRTR
                MOVLW   STDRTR              ; Standard rtr
                PUTC                        ; Tx
 
                RXSID_10_8                  ; Get SID10..8
                MOVWF   POSTINC0
                IORLW   0x30                ; BIN2ASC
                PUTC                        ; Tx

                RXSID_7_0                   ; Get SID7..0
                MOVWF   POSTINC0
                CALL    BIN2ASC             ; Tx

                BRA     RXCANDLC

;------------------------------------------------------------------------------
RXEXT
                MOVLW   EXTDATA             ; Extended data
                BTFSC   RXB0DLC,RXRTR
                MOVLW   EXTRTR              ; Extended rtr
                PUTC                        ; Tx
 
                RXEXT_28_24                 ; Get EID28..EID24
                MOVWF   POSTINC0
                CALL    BIN2ASC             ; Tx

                RXEXT_23_16                 ; Get EID23..EID16
                MOVWF   POSTINC0
                CALL    BIN2ASC             ; Tx

                MOVF    RXB0EIDH,W          ; Get EID15..EID8
                MOVWF   POSTINC0
                CALL    BIN2ASC             ; Tx

                MOVF    RXB0EIDL,W          ; Get EID7..EID0
                MOVWF   POSTINC0
                CALL    BIN2ASC             ; Tx

                BSF     CANFRAME,7          ; mark as extended frame

;------------------------------------------------------------------------------
RXCANDLC
                BSF     CANFRAME,6          ; mark as new frame

                MOVF    RXB0DLC,W           ; Get DLC
                ANDLW   0x0F
                MOVWF   POSTINC0
                MOVWF   CANCNT
                IORLW   0x30                ; BIN2ASC
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
;

                LFSR    FSR0,CANFRAME+5
                MOVFF   RXB0D0,POSTINC0
                MOVFF   RXB0D1,POSTINC0
                MOVFF   RXB0D2,POSTINC0
                MOVFF   RXB0D3,POSTINC0
                MOVFF   RXB0D4,POSTINC0
                MOVFF   RXB0D5,POSTINC0
                MOVFF   RXB0D6,POSTINC0
                MOVFF   RXB0D7,POSTINC0

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
