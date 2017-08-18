;-------------------------------------------------------------------------------
;
;   LED demo for The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   Copyright (c) 2017 Darron M Broad
;
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;   This file is part of The Cowboy Boot Loader.
;
;   The Cowboy Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Cowboy Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Cowboy Boot Loader. If not, see http://www.gnu.org/licenses/
;-------------------------------------------------------------------------------

                RADIX   DEC

;-------------------------------------------------------------------------------
; Device
;-------------------------------------------------------------------------------

                PROCESSOR 16f1455

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; VDD VCC       1----14 VSS GND
; RA5           2    13 RA0 ICSPDAT D+
; RA4           3    12 RA1 ICSPCLK D-
; RA3 !MCLR VPP 4    11 VUSB3V3
; RC5 RX        5    10 RC0 ICSPDAT
; RC4 TX        6     9 RC1 ICSPCLK
; RC3           7-----8 RC2
;
;-------------------------------------------------------------------------------

ERRORLEVEL      -1302
#INCLUDE        "devices.inc"

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

#DEFINE         CLOCK   4000000             ;ERRATA DS80000546F-page 3

;-------------------------------------------------------------------------------
; Variables
;-------------------------------------------------------------------------------

                CBLOCK  0x70
                temp1   : 1
                temp2   : 1
                temp3   : 1
                ENDC

;-------------------------------------------------------------------------------
; Reset
;-------------------------------------------------------------------------------

                ORG     0
                GOTO    INIT

;-------------------------------------------------------------------------------
; Init.
;-------------------------------------------------------------------------------

                ORG     8
INIT 
                BANKSEL LATA
                CLRF    LATA
                BANKSEL TRISA
                BCF     TRISA,5

;-------------------------------------------------------------------------------
; Toggle LED
;-------------------------------------------------------------------------------

LOOP            MOVLW   (1 << 5)
                BANKSEL LATA
                XORWF   LATA,F

                MOVLW   3
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

;-------------------------------------------------------------------------------
                END
;-------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
