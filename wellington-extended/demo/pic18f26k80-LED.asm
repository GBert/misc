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

                PROCESSOR 18f26k80

;------------------------------------------------------------------------------
; PIC18F26K80 Device Pinout
;------------------------------------------------------------------------------
;
; !MCLR        1-----28 RB7 PGD RX2
; RA0          2     27 RB6 PGC TX2
; RA1          3     26 RB5
; RA2          4     25 RB4
; RA3          5     24 RB3
; VDDCORE/VCAP 6     23 RB2
; RA5          7     22 RB1
; VSS GND      8     21 RB0
; RA7 CLKIN    9     20 VDD VCC
; RA6 CLKOUT   10    19 VSS GND
; RC0          11    18 RC7 RX1
; RC1          12    17 RC6 TX1
; RC2          13    16 RC5
; RC3          14----15 RC4

;------------------------------------------------------------------------------
; Device Constants
;------------------------------------------------------------------------------

ERRORLEVEL      -302
#INCLUDE        "devices.inc"

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

#DEFINE         CLOCK   64000000

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

INIT            SETF    LATA
                BCF     TRISA,0

;------------------------------------------------------------------------------
; Toggle LED
;------------------------------------------------------------------------------

LOOP            BTG     LATA,0

                MOVLW   50
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
