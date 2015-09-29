;------------------------------------------------------------------------------
;
;   Demo for The Wellington Boot Loader for PIC18
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

                RADIX   DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

                PROCESSOR 18fXXX

;------------------------------------------------------------------------------
; Device Constants
;------------------------------------------------------------------------------

#INCLUDE        "devices.inc"
ERRORLEVEL      -302

;------------------------------------------------------------------------------
; Device Settings
;------------------------------------------------------------------------------

#DEFINE         CLOCK   0

;------------------------------------------------------------------------------
; Reset
;------------------------------------------------------------------------------

                ORG     0
                GOTO    INIT

;------------------------------------------------------------------------------
; Init.
;------------------------------------------------------------------------------

INIT
                GOTO    INIT

;------------------------------------------------------------------------------
                END
;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
