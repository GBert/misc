;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   Boot Loader Project
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

                RADIX       DEC

;-------------------------------------------------------------------------------
; Device
;-------------------------------------------------------------------------------

                PROCESSOR   16f1776

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; !MCLR      1-----28 RB7 PGD
; RA0        2     27 RB6 PGC
; RA1        3     26 RB5
; RA2        4     25 RB4
; RA3        5     24 RB3
; RA4        6     23 RB2
; RA5        7     22 RB1
; VSS GND    8     21 RB0
; RA7 CLKIN  9     20 VDD VCC
; RA6 CLKOUT 10    19 VSS GND
; RC0        11    18 RC7 RX
; RC1        12    17 RC6 TX
; RC2        13    16 RC5
; RC3 SCL    14----15 RC4 SDA
;
;-------------------------------------------------------------------------------
; Device Constants
;-------------------------------------------------------------------------------

ERRORLEVEL      -1302
#INCLUDE        "devices.inc"

; Boot Loader Size
BOOTSIZE        EQU         8

; Boot Loader Ident
; __IDLOCS 0x6666

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       16000000
#DEFINE         TIMEOUT     10

; Hardware UART 1 or 2
#DEFINE         UART        1

; UART Baud Rate
#DEFINE         BAUDRATE    115200

; BOOT/LED/SWITCH
#DEFINE         DDR         TRISA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         0
#DEFINE         POLARITY    0
#DEFINE         INPUT       PORTA
;#DEFINE         SWITCH      1

;-------------------------------------------------------------------------------
; Device Configuration
;-------------------------------------------------------------------------------

  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
  __CONFIG _CONFIG2, _WRT_OFF & _PPS1WAY_OFF & _ZCD_OFF & _PLLEN_OFF & _STVREN_ON & _BORV_HI & _LPBOR_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

                BANKSEL OSCCON              ;BANK ?
                MOVLW   b'01111010'
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCSTAT,HFIOFS
                GOTO    INITHFIOFS

                BANKSEL ANSELA              ;BANK ?
                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC
; PPSUnlock
                BANKSEL PPSLOCK             ;BANK ?
                MOVLW   0x55
                MOVWF   PPSLOCK
                MOVLW   0xAA
                MOVWF   PPSLOCK
                BCF     PPSLOCK,PPSLOCKED
; PPSInput
                BANKSEL RC6PPS              ;BANK ?
                MOVLW   0x24
                MOVWF   RC6PPS              ;RC6 = TX
; PPSOutput
                BANKSEL RXPPS               ;BANK ?
                MOVLW   0x17
                MOVWF   RXPPS               ;RC7 = RX
; PPSLock
                BANKSEL PPSLOCK             ;BANK ?
                MOVLW   0x55
                MOVWF   PPSLOCK
                MOVLW   0xAA
                MOVWF   PPSLOCK
                BSF     PPSLOCK,PPSLOCKED

                ENDM

;-------------------------------------------------------------------------------
; The Cowboy Boot Loader
;-------------------------------------------------------------------------------

#INCLUDE        "boot.inc"

;-------------------------------------------------------------------------------
THE
                END
;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
