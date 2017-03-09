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

                PROCESSOR   16f1705

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; VDD VCC       1----14 VSS GND
; RA5           2    13 RA0 ICSPDAT
; RA4           3    12 RA1 ICSPCLK
; RA3 !MCLR VPP 4    11 RA2
; RC5           5    10 RC0
; RC4           6     9 RC1 RX
; RC3           7-----8 RC2 TX
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
#DEFINE         CLOCK       32000000
#DEFINE         TIMEOUT     10

; UART 1 or 2
#DEFINE         UART        1

; UART Baud Rate
#DEFINE         BAUDRATE    115200

; BOOT/LED/SWITCH
#DEFINE         DDR         TRISC
#DEFINE         OUTPUT      LATC
#DEFINE         LED         5
#DEFINE         POLARITY    0
#DEFINE         INPUT       PORTC
;#DEFINE         SWITCH      1

;-------------------------------------------------------------------------------
; Device Configuration
;-------------------------------------------------------------------------------

  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
  __CONFIG _CONFIG2, _WRT_OFF & _PPS1WAY_OFF & _PLLEN_ON & _STVREN_ON & _BORV_HI & _LPBOR_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

                BANKSEL ANSELC              ;BANK ?
                CLRF    ANSELC
; PPSUnlock
                BANKSEL PPSLOCK
                MOVLW   0x55
                MOVWF   PPSLOCK
                MOVLW   0xAA
                MOVWF   PPSLOCK
                BCF     PPSLOCK,PPSLOCKED
; PPSInput
                MOVLW   0x11                ;RC1 = RX
                BANKSEL RXPPS
                MOVWF   RXPPS
; PPSOutput
                MOVLW   0x14                ;RC2 = TX
                BANKSEL RC2PPS
                MOVWF   RC2PPS
; PPSLock
                BANKSEL PPSLOCK
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
