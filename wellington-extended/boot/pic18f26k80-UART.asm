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

                PROCESSOR   18f26k80

;------------------------------------------------------------------------------
; Device Pinout
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
#DEFINE         TIMEOUT     0           ; Very long switched time-out

; UART PORTC or PORTB
#DEFINE         UART        PORTC

; UART Baud Rate
#DEFINE         BAUDRATE    115200

; BOOT/LED/SWITCH
#DEFINE         DDR         TRISA
#DEFINE         OUTPUT      LATA
#DEFINE         LED         0
#DEFINE         INPUT       PORTA
#DEFINE         SWITCH      1

;------------------------------------------------------------------------------
; Device Configuration
;------------------------------------------------------------------------------

; VREG Sleep Enable bit:
                CONFIG    RETEN=ON
; LF-INTOSC Low-power Enable bit:
;               CONFIG    INTOSCSEL=LOW
; SOSC Power Selection and mode Configuration bits:
                CONFIG    SOSCSEL=DIG
; Extended Instruction Set:
                CONFIG    XINST=OFF
; Oscillator:
                CONFIG    FOSC=INTIO2
; PLL x4 Enable bit:
                CONFIG    PLLCFG=OFF
; Fail-Safe Clock Monitor:
                CONFIG    FCMEN=OFF
; Internal External Oscillator Switch Over Mode:
                CONFIG    IESO=OFF
; Power-up Timer:
                CONFIG    PWRTEN=OFF
; Brown-out Detect:
                CONFIG    BOREN=OFF
; Brown-out Reset Voltage bits:
                CONFIG    BORV=1
; BORMV Power level:
;               CONFIG    BORPWR=LOW
; Watchdog Timer:
                CONFIG    WDTEN=OFF
; Watchdog Postscaler:
                CONFIG    WDTPS=1024
; ECAN Mux bit:
                CONFIG    CANMX=PORTB
; MSSP address masking:
                CONFIG    MSSPMSK=MSK5
; Master Clear Enable:
                CONFIG    MCLRE=ON
; Stack Overflow Reset:
                CONFIG    STVREN=ON
; Boot Block Size:
                CONFIG    BBSIZ=BB1K
; Code Protect 00800-03FFF:
                CONFIG    CP0=OFF
; Code Protect 04000-07FFF:
                CONFIG    CP1=OFF
; Code Protect 08000-0BFFF:
                CONFIG    CP2=OFF
; Code Protect 0C000-0FFFF:
                CONFIG    CP3=OFF
; Code Protect Boot:
                CONFIG    CPB=OFF
; Data EE Read Protect:
                CONFIG    CPD=OFF
; Table Write Protect 00800-03FFF:
                CONFIG    WRT0=OFF
; Table Write Protect 04000-07FFF:
                CONFIG    WRT1=OFF
; Table Write Protect 08000-0BFFF:
                CONFIG    WRT2=OFF
; Table Write Protect 0C000-0FFFF:
                CONFIG    WRT3=OFF
; Config. Write Protect:
                CONFIG    WRTC=OFF
; Table Write Protect Boot:
                CONFIG    WRTB=OFF
; Data EE Write Protect:
                CONFIG    WRTD=OFF
; Table Read Protect 00800-03FFF:
                CONFIG    EBTR0=OFF
; Table Read Protect 04000-07FFF:
                CONFIG    EBTR1=OFF
; Table Read Protect 08000-0BFFF:
                CONFIG    EBTR2=OFF
; Table Read Protect 0C000-0FFFF:
                CONFIG    EBTR3=OFF
; Table Read Protect Boot:
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

                CLRF    ADCON0          ;D
                CLRF    ANCON0          ;N
                CLRF    ANCON1          ;N

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
