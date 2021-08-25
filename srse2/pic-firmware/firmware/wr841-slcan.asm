;------------------------------------------------------------------------------
;
; Can-Can Device
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

                RADIX   DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

                PROCESSOR   18f25k80

;------------------------------------------------------------------------------
; Device constants
;------------------------------------------------------------------------------

                ERRORLEVEL  -302,-1301
                LIST        P=PIC18F25K80
#INCLUDE        "p18f25k80.inc"
                LIST

;------------------------------------------------------------------------------
; Device settings
;------------------------------------------------------------------------------

#DEFINE         WR841_SLCAN 1
#DEFINE         PORTD       0xffff
; Clock Rate
#DEFINE         CLOCK       64000000

; UART PORTC or PORTB
#DEFINE         UART        PORTC

; UART Baud Rate
#DEFINE         BAUDRATE    500000

; I2C Rate
#DEFINE         I2CRATE     400000

; CAN PORTB or PORTC
#DEFINE         CAN         PORTB

; CAN Bus Rate
#DEFINE         CANRATE     250

;------------------------------------------------------------------------------
; Device hardware
;------------------------------------------------------------------------------

; LED1 RA0 CAN  OVERFLOW
; LED2 RA1 UART OVERFLOW
; RTS  RA2 CLEAR TO SEND
;
#DEFINE         GPIO        LATA
#DEFINE         DDR         DDRA

#DEFINE         LEDON       1

;------------------------------------------------------------------------------
; Device firmware
;------------------------------------------------------------------------------


#INCLUDE        "18f2xk80-mcu-config.inc"
#INCLUDE        "can-can.inc"

;------------------------------------------------------------------------------
THE
                END
;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
