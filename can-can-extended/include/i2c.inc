;------------------------------------------------------------------------------
;
; Can-Can I2C
;
; Copyright (c) 2015 Gerhard Bertelsmann
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
; I2C settings
;------------------------------------------------------------------------------

#DEFINE I2CBAUD ((((CLOCK / I2CRATE) / 2) - 1) / 2)

;------------------------------------------------------------------------------
; Init. I2C
;------------------------------------------------------------------------------
I2CINIT
                ; clear the upper two bits
                MOVLW   0x3F
                ANDWF   SSPSTAT,F
                ; set I2C rate
                MOVLW   I2CBAUD
                MOVWF   SSPADD
                CLRF    SSPCON1
                CLRF    SSPCON2
                BCF     SSPSTAT,SMP
                BSF     SSPSTAT,CKE
                ; configure SCL and SDL as input
		BSF     TRISC,3
		BSF     TRISC,4
                ; master mode
                MOVLW   (1 << SSPEN) + (1 << SSPM3)
                IORWF   SSPCON1,F
                RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
