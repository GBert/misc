;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   Boot Loader Project
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

                RADIX       DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

                PROCESSOR   18fXXX

;------------------------------------------------------------------------------
; Device Constants
;------------------------------------------------------------------------------

; Bootloader size
BOOTSIZE        EQU         (5 * 64)

ERRORLEVEL      -302
#INCLUDE        "devices.inc"

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       0
#DEFINE         TIMEOUT     1

; UART PORTC or PORTB
#DEFINE         UART        PORTC

; UART Baud Rate
#DEFINE         BAUDRATE    115200

; BOOT LED/SWITCH
#DEFINE         DDR         DDRA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         0
#DEFINE         INPUT       PORTA
#DEFINE         SWITCH      1

;------------------------------------------------------------------------------
; Device Configuration
;------------------------------------------------------------------------------

; Oscillator Selection:
                CONFIG    OSC=XT
; Osc. Switch Enable:
                CONFIG    OSCS=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Reset:
                CONFIG    BOR=OFF
; Brown-out Voltage:
                CONFIG    BORV=27
; Watchdog Timer:
                CONFIG    WDT=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=128
; CCP2 MUX:
                CONFIG    CCP2MUX=OFF
; Stack Full/Overflow Reset:
                CONFIG    STVR=ON
; Low Voltage ICSP:
                CONFIG    LVP=OFF
; Background Debugger Enable:
                CONFIG    DEBUG=OFF

;------------------------------------------------------------------------------
; Device Init.
;------------------------------------------------------------------------------

INIT            MACRO

                ENDM

;------------------------------------------------------------------------------
; The Wellington Boot Loader
;------------------------------------------------------------------------------

#INCLUDE        "boot.inc"

;------------------------------------------------------------------------------    
THE
                END
;------------------------------------------------------------------------------    
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
