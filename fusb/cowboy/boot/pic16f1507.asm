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

                PROCESSOR   16f1507

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; VDD VCC       1----20 VSS GND
; RA5           2    19 RA0 ICSPDAT
; RA4           3    18 RA1 ICSPCLK
; RA3 !MCLR VPP 4    17 RA2
; RC5           5    16 RC0 SOFTWARE TX
; RC4           6    15 RC1 SOFTWARE RX
; RC3           7    14 RC2
; RC6           8    13 RB4
; RC7           9    12 RB5
; RB7           10---11 RB6
;
;-------------------------------------------------------------------------------
; Device Constants
;-------------------------------------------------------------------------------

ERRORLEVEL      -1302
#INCLUDE        "devices.inc"

; Boot Loader Size
BOOTSIZE        EQU         17

; Boot Loader Ident
; __IDLOCS 0x6666

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       16000000
#DEFINE         TIMEOUT     10

; Hardware UART 1 or 2
;#DEFINE         UART        1

; Software UART
#DEFINE         USOFT       1
#DEFINE         UDDR        TRISC
#DEFINE         UOUT        LATC
#DEFINE         UINP        PORTC
#DEFINE         UTXD        0
#DEFINE         URXD        1
#DEFINE         USTP        0               ;0, 1 or 2 for RX. TX always 2

; UART Baud Rate
#DEFINE         BAUDRATE    19200

; BOOT/LED/SWITCH
#DEFINE         DDR         TRISA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         5
#DEFINE         POLARITY    0
#DEFINE         INPUT       PORTA
;#DEFINE         SWITCH      1

;-------------------------------------------------------------------------------
; Device Configuration
;-------------------------------------------------------------------------------

  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_ON
  __CONFIG _CONFIG2, _WRT_OFF & _STVREN_ON & _BORV_HI & _LPBOR_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

                BANKSEL OSCCON              ;BANK ?
                MOVLW   B'01111010'
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCSTAT,HFIOFS
                GOTO    INITHFIOFS

                BANKSEL ANSELA              ;BANK ?
                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC

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
