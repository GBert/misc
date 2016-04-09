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

                PROCESSOR   18f45k80

;------------------------------------------------------------------------------
; Device constants
;------------------------------------------------------------------------------

                ERRORLEVEL  -302,-1301
                LIST        P=PIC18F45K80
#INCLUDE        "p18f45k80.inc"
                LIST

;------------------------------------------------------------------------------
; Device settings
;------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       64000000

; UART PORTC, PORTB or PORTD
#DEFINE         UART        PORTD

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

; LED1 RB6 CAN  OVERFLOW
; LED2 RB7 UART OVERFLOW
; RTS  RB4 CLEAR TO SEND
;
#DEFINE         GPIO        LATB
#DEFINE         DDR         DDRB
#DEFINE		PIO	    1
#DEFINE		LEDHIGH     1
;#DEFINE		PIOINVERT   1

;------------------------------------------------------------------------------
; Device firmware
;------------------------------------------------------------------------------


#INCLUDE        "18f4xk80-mcu-config.inc"
#INCLUDE        "can-can.inc"

;------------------------------------------------------------------------------
THE
                END
;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
