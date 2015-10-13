;------------------------------------------------------------------------------
;
; Can-Can Rx/Tx ring
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
; Rx/Tx ring settings
;------------------------------------------------------------------------------

; End of Line Character
#DEFINE         EOLNCHAR    0x0D            ; CR

; Rx/Tx ring Flow Control
#DEFINE         OVERFLOW    (256 / 27)      ; Rx/Tx ring size / Max slcan size
#DEFINE         CONTROL     (3)             ; Flow Control

;------------------------------------------------------------------------------
; Rx/Tx ring variables
;------------------------------------------------------------------------------

                CBLOCK
                ; Rx ring
                RXGET   : 1                 ; GETC Main
                RXPUT   : 1                 ; PUTC ISR
                RXOVF   : 1                 ; ISR Overflow
                RXCNT   : 1                 ; Rx packet counter
                ; Tx ring
                TXGET   : 1                 ; GETC ISR
                TXPUT   : 1                 ; PUTC Main
                TXTEMP  : 1                 ; PUTC Temp
                TXCNT   : 1                 ; Tx packet counter
                ENDC

;------------------------------------------------------------------------------
; Rx/Tx ring
;------------------------------------------------------------------------------

RXBUF           EQU     0x0180              ; Rx ring FSR1

;0100:  80 81 82 83 84 85 86 87 88 89 8a 8b 8c 8d 8e 8f    ................
;0110:  90 91 92 93 94 95 96 97 98 99 9a 9b 9c 9d 9e 9f    ................
;0120:  a0 a1 a2 a3 a4 a5 a6 a7 a8 a9 aa ab ac ad ae af    ................
;0130:  b0 b1 b2 b3 b4 b5 b6 b7 b8 b9 ba bb bc bd be bf    ................
;0140:  c0 c1 c2 c3 c4 c5 c6 c7 c8 c9 ca cb cc cd ce cf    ................
;0150:  d0 d1 d2 d3 d4 d5 d6 d7 d8 d9 da db dc dd de df    ................
;0160:  e0 e1 e2 e3 e4 e5 e6 e7 e8 e9 ea eb ec ed ee ef    ................
;0170:  f0 f1 f2 f3 f4 f5 f6 f7 f8 f9 fa fb fc fd fe ff    ................
;0180:  00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f    ................
;0190:  10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f    ................
;01a0:  20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f     !"#$%&'()*+,-./
;01b0:  30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f    0123456789:;<=>?
;01c0:  40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f    @ABCDEFGHIJKLMNO
;01d0:  50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f    PQRSTUVWXYZ[\]^_
;01e0:  60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f    `abcdefghijklmno
;01f0:  70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f    pqrstuvwxyz.....

TXBUF           EQU     0x0280              ; Tx ring FSR2

;------------------------------------------------------------------------------
; Rx/Tx ring macros
;------------------------------------------------------------------------------

GETC            MACRO
                MOVF    RXGET,W
                MOVF    PLUSW1,W
                INCF    RXGET,F             ; RXGET++
                ENDM

PUTC            MACRO
                MOVWF   TXTEMP
                MOVF    TXPUT,W
                MOVFF   TXTEMP,PLUSW2
                INCF    TXPUT,F             ; TXPUT++
                ENDM

;------------------------------------------------------------------------------
; Rx ring ISR
; 
;  Affects RXPUT & uses FSR1; do not use these elsewhere.
;------------------------------------------------------------------------------
ISRHIGH
                ; Read Rx Data & Flush IRQ
                MOVF    RXPUT,W
                MOVFF   URCREG,PLUSW1       ; Won't clobber with Overflow On

                ; If Rx packet counter = trigger Then Drop Data
                MOVF    RXCNT,W
                XORLW   OVERFLOW
                BNZ     ISRHIGH1

                ; Overflow On
                SETF    RXOVF

                UARTOVFL                    ; UART packet status LED
                RETFIE  FAST

;------------------------------------------------------------------------------
ISRHIGH1
                ; If Overflow On Then Drop Data
                MOVF    RXOVF,W             ; Overflow On?
                BZ      ISRHIGH3            ; No...

                ; Disable Overflow When End of Line
                MOVF    RXPUT,W
                MOVF    PLUSW1,W            ; EOLN?
                XORLW   EOLNCHAR
                BNZ     ISRHIGH2

                ; Overflow Off
                CLRF    RXOVF
ISRHIGH2
                RETFIE  FAST
                
;------------------------------------------------------------------------------
ISRHIGH3
                ; Save Rx Data
                MOVF    RXPUT,W
                INCF    RXPUT,F             ; PUTC++

                MOVF    PLUSW1,W            ; EOLN?
                XORLW   EOLNCHAR
                BNZ     ISRHIGHEND

                INCF    RXCNT,F             ; Rx packet counter++
                MOVF    RXCNT,W             ; Flow control on?
                SUBLW   CONTROL
                BC      ISRHIGHEND

                ; Not Clear to Send; RXCNT > CONTROL
                RTSSET
ISRHIGHEND
                UARTRECV                    ; UART packet status LED
                RETFIE  FAST

;------------------------------------------------------------------------------
; Tx ring ISR
; 
;  Affects TXGET & uses FSR2; do not use these elsewhere.
;------------------------------------------------------------------------------
ISRLOW
                MOVFF   STATUS,S_SAVE       ; SAVE STATUS
                MOVWF   W_SAVE              ; SAVE WORKING REGISTER

                MOVF    TXGET,W
                MOVF    PLUSW2,W
                INCF    TXGET,F

                MOVWF   UTXREG
                XORLW   EOLNCHAR
                BNZ     ISRLOWEND

                DECF    TXCNT,F             ; Tx packet counter--
                BNZ     ISRLOWEND
                BCF     UPIE,UTXIE          ; Disable Tx ring ISR
ISRLOWEND
                MOVF    W_SAVE,W            ; RESTORE WORKING REGISTER
                MOVFF   S_SAVE,STATUS       ; RESTORE STATUS

                RETFIE

;------------------------------------------------------------------------------
; Init. Rx/Tx ring
;------------------------------------------------------------------------------
INITRING
                LFSR    FSR1,RXBUF          ; FSR1 H/L Reserved for Rx ring
                LFSR    FSR2,TXBUF          ; FSR2 H/L Reserved for Tx ring

                CLRF    RXGET               ; Rx index = 0
                CLRF    RXPUT
                CLRF    RXOVF

                CLRF    TXGET               ; Tx index = 0
                CLRF    TXPUT

                CLRF    RXCNT               ; Reset Rx packet counter
                CLRF    TXCNT               ; Reset Tx packet counter

                RETURN

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
