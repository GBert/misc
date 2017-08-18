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

                PROCESSOR   16f15355

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; !MCLR      1-----28 RB7 PGD RX2
; RA0        2     27 RB6 PGC TX2
; RA1        3     26 RB5
; RA2        4     25 RB4
; RA3        5     24 RB3
; RA4        6     23 RB2
; RA5        7     22 RB1
; VSS GND    8     21 RB0
; RA7 CLKIN  9     20 VDD VCC
; RA6 CLKOUT 10    19 VSS GND
; RC0        11    18 RC7 RX1
; RC1        12    17 RC6 TX1
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
#DEFINE         CLOCK       32000000
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

  __CONFIG _CONFIG1, _FEXTOSC_OFF & _RSTOSC_HFINT32 & _CLKOUTEN_ON & _CSWEN_ON & _FCMEN_OFF
  __CONFIG _CONFIG2, _MCLRE_ON & _PWRTE_ON & _LPBOREN_OFF & _BOREN_OFF & _BORV_HI & _ZCD_OFF & _PPS1WAY_OFF & _STVREN_ON
  __CONFIG _CONFIG3, _WDTCPS_WDTCPS_5 & _WDTE_SWDTEN & _WDTCWS_WDTCWS_7 & _WDTCCS_HFINTOSC & _WDTCCS_SC
  __CONFIG _CONFIG4, _BBSIZE_BB512 & _BBEN_OFF & _SAFEN_OFF & _WRTAPP_OFF & _WRTB_OFF & _WRTC_OFF & _WRTSAF_OFF & _LVP_ON
  __CONFIG _CONFIG5, _CP_OFF

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

                BANKSEL ANSELA              ;BANK 62
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
                BANKSEL RX1DTPPS            ;BANK ?
                MOVLW   0x17
                MOVWF   RX1DTPPS            ;RC7 = RX1
; PPSOutput
                BANKSEL RC6PPS              ;BANK ?
                MOVLW   0x0F
                MOVWF   RC6PPS              ;RC6 = TX1
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
