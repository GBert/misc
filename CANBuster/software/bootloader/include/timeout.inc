;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   Timeout macros
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

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; Timeout
;------------------------------------------------------------------------------

#IF CLOCK < 1000000
    ERROR       "CLOCK Too Low"
#ENDIF

TIMEBASE        EQU     CLOCK / 1000000

;------------------------------------------------------------------------------
; Timeout Macros
;
;  Modifies W
;------------------------------------------------------------------------------

BEGINTO         MACRO   TO

                MOVLW   TO
                MOVWF   timer1

                MOVLW   TIMEBASE
                MOVWF   timer2

                CLRF    timer3
                CLRF    timer4

                ENDM

LOOPTO          MACRO   TO

                DECF    timer4,F
                BNZ     TO
                DECF    timer3,F
                BNZ     TO
                CLRWDT
                BTG     OUTPUT,LED

                DECF    timer2,F
                BNZ     TO

                MOVLW   TIMEBASE
                MOVWF   timer2

                DECF    timer1,F
                BNZ     TO
                BCF     OUTPUT,LED

                ENDM

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
