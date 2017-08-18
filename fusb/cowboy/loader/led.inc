;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   LED macros
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
; LED macros
;
;  Modifies BSR
;-------------------------------------------------------------------------------

LEDON           MACRO
                BANKSEL OUTPUT              ; BANK 2 | 0
#IF POLARITY == 0
                BSF     OUTPUT,LED          ; BOOT LED ON
#ELSE
                BCF     OUTPUT,LED          ; BOOT LED ON
#ENDIF
                ENDM

LEDOFF          MACRO
                BANKSEL OUTPUT              ; BANK 2 | 0
#IF POLARITY == 0
                BCF     OUTPUT,LED          ; BOOT LED OFF
#ELSE
                BSF     OUTPUT,LED          ; BOOT LED OFF
#ENDIF
                ENDM

;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
