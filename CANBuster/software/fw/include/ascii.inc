;------------------------------------------------------------------------------
;
; Can-Can ASCII I/O
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
; ASCII I/O variables
;------------------------------------------------------------------------------

                CBLOCK
                ASCTEMP : 1
                ENDC

;------------------------------------------------------------------------------
; Convert ASCII Hex Byte or Nibble in <RXBUF> to Binary, Store In <FSR0> or W
;
; Nibble: RXGET += 1
; Byte  : RXGET += 2, FSR0 += 1
;------------------------------------------------------------------------------
ASC2BIN
                RCALL   ASC2NIB
                MOVWF   INDF0
                SWAPF   INDF0,F
                RCALL   ASC2NIB
                IORWF   POSTINC0,F
                RETURN               
ASC2NIB
                GETC                        ; W = <RXBUF>,RXGET++
                MOVWF   ASCTEMP
                ANDLW   0x0F
                BTFSS   ASCTEMP,6
                RETURN
                ADDLW   9
                RETURN

;------------------------------------------------------------------------------
; Convert Binary in W to ASCII Hex Nibble or Byte, Store in <TXBUF>
;
; Nibble: TXPUT += 1
; Byte  : TXPUT += 2
;------------------------------------------------------------------------------
NIB2ASC         MACRO
                ANDLW   0x0F
                ADDLW   6
                BTFSC   STATUS,DC
                ADDLW   7
                ADDLW   42
                PUTC                        ; <TXBUF> = W,TXPUT++
                ENDM

BIN2ASC
                MOVWF   ASCTEMP
                SWAPF   ASCTEMP,W
                NIB2ASC
                MOVF    ASCTEMP,W
                NIB2ASC
                RETURN

;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
