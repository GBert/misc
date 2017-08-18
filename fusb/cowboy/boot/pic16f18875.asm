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

                PROCESSOR   16f18875

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
; RE3 !MCLR/VPP 1-----40 RB7 PGD
; RA0           2     39 RB6 PGC
; RA1           3     38 RB5
; RA2           4     37 RB4
; RA3           5     36 RB3
; RA4           6     35 RB2
; RA5           7     34 RB1
; RE0           8     33 RB0
; RE1           9     32 VDD/VCC
; RE2           10    31 VSS/GND
; VDD/VCC       11    30 RD7
; VSS/GND       12    29 RD6
; RA7           13    28 RD5
; RA6           14    27 RD4
; RC0           15    26 RC7 RX
; RC1           16    25 RC6 TX
; RC2           17    24 RC5
; RC3           18    23 RC4
; RD0           19    22 RD3
; RD1           20----21 RD2
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

  __CONFIG _CONFIG1, _FEXTOSC_OFF & _RSTOSC_HFINT32 & _CLKOUTEN_OFF & _CSWEN_ON & _FCMEN_OFF
  __CONFIG _CONFIG2, _MCLRE_ON & _PWRTE_OFF & _LPBOREN_OFF & _BOREN_OFF & _BORV_HI & _ZCD_OFF & _ZCDDIS_ON & _PPS1WAY_OFF & _STVREN_ON & _DEBUG_OFF
  __CONFIG _CONFIG3, _WDTCPS_WDTCPS_5 & _WDTE_SWDTEN & _WDTCWS_WDTCWS_7 & _WDTCCS_HFINTOSC & _WDTCCS_SC
  __CONFIG _CONFIG4, _WRT_OFF & _SCANE_available & _LVP_ON
  __CONFIG _CONFIG5, _CP_OFF & _CPD_OFF

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT            MACRO

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
                BANKSEL RXPPS               ;BANK ?
                MOVLW   0x17
                MOVWF   RXPPS               ;RC7 = RX
; PPSOutput
                BANKSEL RC6PPS              ;BANK ?
                MOVLW   0x10
                MOVWF   RC6PPS              ;RC6 = TX
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
