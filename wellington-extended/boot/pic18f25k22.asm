;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   Boot Loader Project
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

                RADIX       DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

                PROCESSOR   18f25k22

;------------------------------------------------------------------------------
; Device Pinout
;------------------------------------------------------------------------------
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

;------------------------------------------------------------------------------
; Device Constants
;------------------------------------------------------------------------------

; Boot Loader Size
BOOTSIZE        EQU     (8 * 64)

; Boot Loader Ident
ERRORLEVEL      -220
                ORG     0x200000
                DB      6,6,6
ERRORLEVEL      +220

ERRORLEVEL      -302
#INCLUDE        "devices.inc"

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       64000000
#DEFINE         TIMEOUT     5

; UART PORTC or PORTB
#DEFINE         UART        PORTC

; UART Baud Rate
#DEFINE         BAUDRATE    1000000

; BOOT/LED/SWITCH
#DEFINE         DDR         TRISA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         0
#DEFINE         INPUT       PORTA
;#DEFINE         SWITCH      1

;------------------------------------------------------------------------------
; Device Configuration
;------------------------------------------------------------------------------

; Oscillator Selection bits:
                CONFIG    FOSC=INTIO67 ; Internal oscillator block
; 4X PLL Enable:
                CONFIG    PLLCFG=OFF
; Primary clock enable bit:
                CONFIG    PRICLKEN=OFF
; Fail-Safe Clock Monitor Enable bit:
                CONFIG    FCMEN=OFF
; Internal/External Oscillator Switchover bit:
                CONFIG    IESO=OFF
; Power-up Timer Enable bit:
                CONFIG    PWRTEN=ON
; Brown-out Reset Enable bits:
                CONFIG    BOREN=SBORDIS
; Brown-out Reset Voltage bits:
                CONFIG    BORV=285
; Watchdog Timer Enable bits:
                CONFIG    WDTEN=SWON
; Watchdog Postscale Select bits:
                CONFIG    WDTPS=1024
; CCP2 MUX bit:
                CONFIG    CCP2MX=PORTC1 ;PORTB3 or PORTC1
; PORTB A/D Enable:
                CONFIG    PBADEN=OFF
; P3A/CCP3 Mux bit:
                CONFIG    CCP3MX=PORTC6 ;PORTC6 or PORTB5
; HFINTOSC Fast Start-up:
                CONFIG    HFOFST=OFF
; Timer3 Clock input mux bit:
                CONFIG    T3CMX=PORTB5  ;PORTB5 or PORTC0
; ECCP2 B output mux bit:
                CONFIG    P2BMX=PORTC0  ;PORTC0 or PORTB5
; MCLR Pin Enable bit:
                CONFIG    MCLRE=EXTMCLR
; Stack Full/Overflow Reset Enable bit:
                CONFIG    STVREN=ON
; Single-Supply ICSP Enable bit:
                CONFIG    LVP=ON
; Extended Instruction Set Enable bit:
                CONFIG    XINST=OFF
; Background Debug:
                CONFIG    DEBUG=OFF
; Code Protection Block 0:
                CONFIG    CP0=OFF
; Code Protection Block 1:
                CONFIG    CP1=OFF
; Code Protection Block 2:
                CONFIG    CP2=OFF
; Code Protection Block 3:
                CONFIG    CP3=OFF
; Boot Block Code Protection:
                CONFIG    CPB=OFF
; Data EEPROM Code Protection:
                CONFIG    CPD=OFF
; Write Protection Block 0:
                CONFIG    WRT0=OFF
; Write Protection Block 1:
                CONFIG    WRT1=OFF
; Write Protection Block 2:
                CONFIG    WRT2=OFF
; Write Protection Block 3:
                CONFIG    WRT3=OFF
; Boot Block Write Protection:
                CONFIG    WRTB=OFF
; Configuration Register Write Protection:
                CONFIG    WRTC=OFF
; Data EEPROM Write Protection:
                CONFIG    WRTD=OFF
; Table Read Protection Block 0:
                CONFIG    EBTR0=OFF
; Table Read Protection Block 1:
                CONFIG    EBTR1=OFF
; Table Read Protection Block 2:
                CONFIG    EBTR2=OFF
; Table Read Protection Block 3:
                CONFIG    EBTR3=OFF
; Boot Block Table Read Protection:
                CONFIG    EBTRB=OFF

;------------------------------------------------------------------------------
; Device Init.
;------------------------------------------------------------------------------

#DEFINE         _16MHZ  b'01110000'     ;64MHZ PLLx4
#DEFINE         _8MHZ   b'01100000'     ;32MHZ PLLx4
#DEFINE         _4MHZ   b'01010000'
#DEFINE         _2MHZ   b'01000000'
#DEFINE         _1MHZ   b'00110000'

INIT            MACRO

                MOVLB   0x0F

                BSF     OSCTUNE,PLLEN
                MOVLW   _16MHZ
                MOVWF   OSCCON
INITHFIOFS      BTFSS   OSCCON,HFIOFS
                GOTO    INITHFIOFS

                CLRF    ADCON0          ;DISABLE A/D
                CLRF    ANSELA
                CLRF    ANSELB
                CLRF    ANSELC

                ENDM

;------------------------------------------------------------------------------
; The Wellington Boot Loader
;------------------------------------------------------------------------------

#INCLUDE        "boot.inc"

;------------------------------------------------------------------------------    
THE
                END
;------------------------------------------------------------------------------    
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
