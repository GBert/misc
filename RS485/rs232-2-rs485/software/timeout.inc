;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   Timeout macros
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

                LIST
                RADIX   DEC

;-------------------------------------------------------------------------------
; Timeout
;-------------------------------------------------------------------------------

TIMEBASE        EQU     (CLOCK / 275000) + 1

;-------------------------------------------------------------------------------
; Timeout macros
;
;  Modifies W, BSR
;-------------------------------------------------------------------------------

BEGINTO         MACRO   TO

                MOVLW   TO
                MOVWF   timer1

                MOVLW   TIMEBASE
                MOVWF   timer2

                CLRF    timer3

                ENDM

LOOPTO          MACRO   TO

                DJNZ    timer3,TO
                CLRWDT
                DJNZ    timer2,TO

                MOVLW   TIMEBASE
                MOVWF   timer2

                ;MOVLW   1<<LED
                ;BANKSEL OUTPUT              ; BANK 2 | 0
                ;XORWF   OUTPUT,F            ; TOGGLE BOOT LED

                DJNZ    timer1,TO

                ENDM

;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
