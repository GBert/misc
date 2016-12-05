;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   CAN Bus functions macro
;
;   Copyright (c) 2016 Darron M Broad, Gerhard Bertelsmann
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
                RADIX       DEC

;------------------------------------------------------------------------------
; CAN Bus Wait until finished
;------------------------------------------------------------------------------

WAIT_CAN_RDY    MACRO   CANBUFFER
                BANKSEL CANBUFFER
WAIT_CAN_RDY_L
                BTFSC   CANBUFFER,TXREQ
                BRA     WAIT_CAN_RDY_L
                ENDM
 
;------------------------------------------------------------------------------    
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
