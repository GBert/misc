;------------------------------------------------------------------------------
;
;   LED demo for The Wellington Boot Loader for PIC18
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

                RADIX   DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

                PROCESSOR 18f4520

; Compatible with the PIC18F4520, PIC18F4550, PIC18F4620 & PIC18F4685

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

ERRORLEVEL      -302
#INCLUDE        "devices.inc"

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

#DEFINE         CLOCK   40000000

;------------------------------------------------------------------------------
; Data EEPROM
;------------------------------------------------------------------------------

                ORG     0xF00000
                DE      "PIC18F4520",0,0xFF

;------------------------------------------------------------------------------
; Variables
;------------------------------------------------------------------------------

                CBLOCK  0
                temp1   : 1
                temp2   : 1
                temp3   : 1
                ENDC

;------------------------------------------------------------------------------
; Reset
;------------------------------------------------------------------------------

                ORG     0
                GOTO    INIT

;------------------------------------------------------------------------------
; Init.
;------------------------------------------------------------------------------

INIT            SETF    LATD
                CLRF    TRISD

;------------------------------------------------------------------------------
; Toggle LED
;------------------------------------------------------------------------------

LOOP            MOVLW   0xFF
                XORWF   LATD,F

                MOVLW   16
                MOVWF   temp1
                CLRF    temp2
                CLRF    temp3

DELAY           DECFSZ  temp3
                BRA     DELAY 
                DECFSZ  temp2
                BRA     DELAY
                DECFSZ  temp1
                BRA     DELAY

                BRA     LOOP

;------------------------------------------------------------------------------
                END
;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
