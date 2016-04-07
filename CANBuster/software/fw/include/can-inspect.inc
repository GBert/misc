;------------------------------------------------------------------------------
;
; CANBuster CAN inspect
;
; Copyright (c) 2016 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; This file is part of the CANBuster project.
;
; CANBuster is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; inspector
;------------------------------------------------------------------------------

CANINSPECT
                BTFSS	CANFRAME,6	; a new CAN frame ?
                RETURN

                BCF	CANFRAME,6	; mark as old
                BTFSS	CANFRAME,7	; extended CAN frame ?
                RETURN

                RETURN

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
