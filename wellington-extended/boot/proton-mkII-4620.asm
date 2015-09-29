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

                PROCESSOR   18f4620

;------------------------------------------------------------------------------
; Device Pinout
;------------------------------------------------------------------------------
;
; !MCLR/VPP RE3            1-----40 RB7/PGD  KBI3
; RA0 AN0                  2     39 RB6/PGC  KBI2
; RA1 AN1                  3     38 RB5/PGM  KBI1
; RA2 AN2 VREF- CVREF      4     37 RB4 AN11 KBI0
; RA3 AN3 VREF+            5     36 RB3 AN9 CCP2
; RA4 T0CKI C1OUT          6     35 RB2 AN8 INT2
; RA5 AN4 !SS HLVDIN C2OUT 7     34 RB1 AN10 INT1
; RE0 AN5 !RD              8     33 RB0 AN12 INT0 FLT0
; RE1 AN6 !WR              9     32 VDD/VCC
; RE2 AN7 !CS              10    31 VSS/GND
; VDD/VCC                  11    30 RD7 PSP7 P1D
; VSS/GND                  12    29 RD6 PSP6 P1C
; RA7 OSC1 CLKI            13    28 RD5 PSP5 P1B
; RA6 OSC2 CLKO            14    27 RD4 PSP4
; RC0 T1OSO T13CKI         15    26 RC7 RX DT
; RC1 T1OSI CCP2           16    25 RC6 TX CK
; RC2 CCP1 P1A             17    24 RC5 SDO
; RC3 SCK SCL              18    23 RC4 SDI SDA
; RD0 PSP0                 19    22 RD3 PSP3
; RD1 PSP1                 20----21 RD2 PSP2

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
; MCLR Enable:
                CONFIG    MCLRE=ON
; T1 Oscillator Enable:
                CONFIG    LPT1OSC=OFF
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; CCP2 Pin Function:
                CONFIG    CCP2MX=PORTBE
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=OFF
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
