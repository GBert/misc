;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   Device Specific Constants
;
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;
;   This file is derived from ds30 Loader
;
;   Copyright (c) 2009-2012 Mikael Gustafsson
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
; Includes
;------------------------------------------------------------------------------

#INCLUDE        "devices_PIC18F.inc"
#INCLUDE        "devices_PIC18FJ.inc"

;------------------------------------------------------------------------------
;
;------------------------------------------------------------------------------

#IFNDEF IS_PIC18F
    #IFNDEF IS_PIC18FJ
        ERROR   "Device unknown"
    #ENDIF
#ENDIF

#IF ROWSIZE > 128
    ERROR       "Device unsupported"
#ENDIF

#IFDEF IS_PIC18F
    #DEFINE     ERASE_FLASH (1 << EEPGD) + (1 << FREE) + (1 << WREN)
    #DEFINE     WRITE_FLASH (1 << EEPGD) + (1 << WREN)
#ENDIF

#IFDEF IS_PIC18FJ
    #DEFINE     ERASE_FLASH (1 << FREE) + (1 << WREN)
    #DEFINE     WRITE_FLASH (1 << WREN)
#ENDIF

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
