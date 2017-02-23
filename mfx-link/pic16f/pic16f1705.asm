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
; VDD 3V3       1----8 VSS GND
; RA5 RX        2    7 RA0 ICSPDAT
; RA4 TX        3    6 RA1 ICSPCLK
; RA3 !MCLR VPP 4----5 RA2
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

  __CONFIG _CONFIG1, _FOSC_INTOSC & _CLKOUTEN_OFF & _WDTE_OFF & _MCLRE_ON
  __CONFIG _CONFIG2, _LVP_ON & _PLLEN_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

                BANKSEL ANSELA              ;BANK ?
                CLRF    ANSELA
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
