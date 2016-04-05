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
                EVENTPS : 2                 ; event pointer
                EVENTPG : 2                 ; event pointer
                ENDC

;------------------------------------------------------------------------------
; Event ring
;------------------------------------------------------------------------------

EVENTBUF        CBLOCK 0x0400               ; event ring
                EVENTBUFFER : 1024
                ENDC

;------------------------------------------------------------------------------
; Event ring macros
;------------------------------------------------------------------------------

INITEVENTRING
                CLRF	EVENTCNT	; event counter
                MOVLW	HIGH EVENTBUF	; set event pointer
                MOVF	EVENTPS,F
                MOVF	EVENTPG,F
                MOVLW	LOW EVENTBUF
                MOVF	EVENTPS+1,F
                MOVF	EVENTPG+1,F
                RETURN

PUTEVENT
                MOVFF	EVENTPS,FSR0H
                MOVFF	EVENTPS+1,FSR0L
                MOVLW	'C'
                MOVWF	POSTINC0
                MOVLW	'B'
                MOVWF	POSTINC0
                MOVFF	ID_H,POSTINC0
                MOVFF	ID_L,POSTINC0
                MOVFF	PIOTEMP16,POSTINC0
                MOVFF	PIOTEMP16+1,POSTINC0
                MOVF	ACTUAL,W
                ANDWF	MASK,W
                BTFSS	STATUS,Z
                MOVLW   1		; either W is zero or it's set to one

                XORLW	1
                MOVWF	POSTINC0
                XORLW	1
                MOVWF	POSTINC0
                CLRF	POSTINC0
                CLRF	POSTINC0

                MOVLW	0x04		; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F

                INCF	EVENTCNT,F
                RETURN

GETEVENT_P
                MOVFF	EVENTPG,FSR0H
                MOVFF	EVENTPG+1,FSR0L
                RETURN

GETEVENT_P_NORM
                DECF	EVENTCNT,F
                MOVLW	0x04		; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F

                RETURN

_SENDEVENT_
                MOVF	EVENTCNT,F
                BTFSC	STATUS,Z
                RETURN

                RETURN
;------------------------------------------------------------------------------
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
