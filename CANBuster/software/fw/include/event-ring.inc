;------------------------------------------------------------------------------
;
; Event ring
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
; event ring variables
;------------------------------------------------------------------------------

                CBLOCK
                EVENTCNT : 1                 ; event counter
                EVENTPNT : 2                 ; event pointer
                ENDC

;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

EVENTBUF        EQU	0x0400               ; event ring

;------------------------------------------------------------------------------
; Event ring macros
;------------------------------------------------------------------------------

INITEVENTRING
                CLRF	EVENTCNT             ; event counter
                MOVLW	HIGH EVENTBUF        ; set event pointer
                MOVF	EVENTPNT,F
                MOVLW	LOW EVENTBUF
                MOVF	EVENTPNT+1,F
                RETURN

PUTEVENT
                MOVFF	EVENTPNT,FSR0H
                MOVFF	EVENTPNT+1,FSR0L
                RETURN

GETEVENT
                MOVFF	EVENTPNT,FSR0H
                MOVFF	EVENTPNT+1,FSR0L
                RETURN

;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
