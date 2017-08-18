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

                PROCESSOR 16f1936

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; RA2           1----18 RA1
; RA3           2    17 RA0
; RA4           3    16 RA7
; RA5 !MCLR VPP 4    15 RA6
; VSS GND       5    14 VDD VCC
; RB0           6    13 RB7 ICSPDAT
; RB1 RX        7    12 RB6 ICSPCLK
; RB2 TX        8    11 RB5 TX
; RB3           9----10 RB4
;
;-------------------------------------------------------------------------------
; Device Constants
;-------------------------------------------------------------------------------

ERRORLEVEL      -1302
#INCLUDE        "devices.inc"

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

#DEFINE         CLOCK   16000000

;-------------------------------------------------------------------------------
; Data EEPROM
;-------------------------------------------------------------------------------

                ORG     0xF000
                DE      "PIC16F1847",0

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
                BCF     TRISA,0

;-------------------------------------------------------------------------------
; Toggle LED
;-------------------------------------------------------------------------------

LOOP            MOVLW   1
                BANKSEL LATA
                XORWF   LATA,F

                MOVLW   10
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
