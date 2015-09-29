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

                PROCESSOR   18f4685

;------------------------------------------------------------------------------
; Device Constants
;------------------------------------------------------------------------------

; Boot Loader Size
BOOTSIZE        EQU         (6 * 64)

ERRORLEVEL      -302
#INCLUDE        "devices.inc"

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       40000000
#DEFINE         TIMEOUT     1

; UART PORTC or PORTB
#DEFINE         UART        PORTC

; UART Baud Rate
#DEFINE         BAUDRATE    115200

; BOOT/LED/SWITCH
#DEFINE         DDR         DDRA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         0
#DEFINE         INPUT       PORTA
;#DEFINE         SWITCH      1

;------------------------------------------------------------------------------
; Device Configuration
;------------------------------------------------------------------------------

; Oscillator Selection:
                CONFIG    OSC=HSPLL
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Reset:
                CONFIG    BOREN=OFF
; Brown-out Voltage:
                CONFIG    BORV=0
; Watchdog Timer:
                CONFIG    WDT=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; Low-Power Timer 1 Oscillator Enable bit:
                CONFIG    LPT1OSC=ON
; MCLR Enable:
                CONFIG    MCLRE=ON
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=OFF
; Boot Block Size Select bits:
                CONFIG    BBSIZ=1024 
; XINST Enable:
                CONFIG    XINST=OFF
; Background Debugger Enable:
                CONFIG    DEBUG=OFF
; Code Protection Block 0:
                CONFIG    CP0=OFF
; Code Protection Block 1:
                CONFIG    CP1=OFF
; Code Protection Block 2:
                CONFIG    CP2=OFF
; Code Protection Block 3:
                CONFIG    CP3=OFF
; Boot Block Code Protection:
                CONFIG    CPB=OFF
; Data EEPROM Code Protection:
                CONFIG    CPD=OFF
; Write Protection Block 0:
                CONFIG    WRT0=OFF
; Write Protection Block 1:
                CONFIG    WRT1=OFF
; Write Protection Block 2:
                CONFIG    WRT2=OFF
; Write Protection Block 3:
                CONFIG    WRT3=OFF
; Boot Block Write Protection:
                CONFIG    WRTB=OFF
; Configuration Register Write Protection:
                CONFIG    WRTC=OFF
; Data EEPROM Write Protection:
                CONFIG    WRTD=OFF
; Table Read Protection Block 0:
                CONFIG    EBTR0=OFF
; Table Read Protection Block 1:
                CONFIG    EBTR1=OFF
; Table Read Protection Block 2:
                CONFIG    EBTR2=OFF
; Table Read Protection Block 3:
                CONFIG    EBTR3=OFF
; Boot Block Table Read Protection:
                CONFIG    EBTRB=OFF

;------------------------------------------------------------------------------
; Device Init.
;------------------------------------------------------------------------------

INIT            MACRO

                MOVLW   b'00000111'
                MOVWF   CMCON
                MOVLW   b'00000000'
                MOVWF   ADCON0
                MOVLW   b'00001111'
                MOVWF   ADCON1

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
