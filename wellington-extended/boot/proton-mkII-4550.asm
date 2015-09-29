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

                PROCESSOR   18f4550

;------------------------------------------------------------------------------
; Device Pinout
;------------------------------------------------------------------------------
;
; !MCLR/VPP RE3            1-----40 RB7/PGD KBI3
; RA0 AN0                  2     39 RB6/PGC KBI2
; RA1 AN1                  3     38 RB5/PGM KBI1
; RA2 AN2 VREF- CVREF      4     37 RB4 AN11 KBI0 CSSPP
; RA3 AN3 VREF+            5     36 RB3 AN9 CCP2 VPO
; RA4 T0CKI C1OUT RCV      6     35 RB2 AN8 INT2 VMO
; RA5 AN4 !SS HLVDIN C2OUT 7     34 RB1 AN10 INT1 SCK SCL
; RE0 AN5 CK1SPP           8     33 RB0 AN12 INT0 FLT0 SDI SDA
; RE1 AN6 CK2SPP           9     32 VDD/VCC
; RE2 AN7 OESPP            10    31 VSS/GND
; VDD/VCC                  11    30 RD7 SPP7 P1D
; VSS/GND                  12    29 RD6 SPP6 P1C
; OSC1 CLKI                13    28 RD5 SPP5 P1B
; OSC2 CLKO RA6            14    27 RD4 SPP4
; RC0 T1OSO T13CKI         15    26 RC7 RX DT SDO
; RC1 T1OSI CCP2 !UOE      16    25 RC6 TX CK
; RC2 CCP1 P1A             17    24 RC5 D+ VP
; VUSB                     18    23 RC4 D- VM
; RD0 SPP0                 19    22 RD3 SPP3
; RD1 SPP1                 20----21 RD2 SPP2

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
#DEFINE         CLOCK       10000000
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

; PLL Prescaler Selection bits:
                CONFIG    PLLDIV=1
; CPU System Clock Postscaler:
                CONFIG    CPUDIV=OSC1_PLL2
; USB Clock Selection bit (used in Full Speed USB mode only; UCFG:FSEN = 1):
                CONFIG    USBDIV=1
; Oscillator Selection:
                CONFIG    FOSC=HS
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Switch Over mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRT=OFF
; Brown-out Reset:
                CONFIG    BOR=OFF
; Brown-out Voltage:
                CONFIG    BORV=2
; Watchdog Timer:
                CONFIG    WDT=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; MCLR Enable:
                CONFIG    MCLRE=ON
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; CCP2 Pin Function:
                CONFIG    CCP2MX=OFF
; Stack Full/Overflow Reset:
                CONFIG    STVREN=ON
; Low Voltage ICSP:
                CONFIG    LVP=OFF
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
