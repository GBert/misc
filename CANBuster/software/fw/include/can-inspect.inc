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

                BCF	CANFRAME,6	; mark as processed

                ; we are looking only for extended frames here
                BTFSS	CANFRAME,7	; extended CAN frame ?
                RETURN

                MOVF	CANFRAME+1,W
                ; CAN PING ?
                XORLW	0x36
                BZ	CAN_PING

                RETURN

;------------------------------------------------------------------------------
; answer CAN Ping
;------------------------------------------------------------------------------

EVENT_RING_NORMALIZE
                MOVLW	0x04            ; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F
                MOVFF	FSR0H,EVENTPP
                MOVFF	FSR0L,EVENTPP+1

                INCF	EVENTCNT,F
                RETURN
;------------------------------------------------------------------------------
; answer CAN Ping
;------------------------------------------------------------------------------

CAN_PING
                ; if DLC != 0 -> return
                MOVF	CANFRAME+4,F
                BZ	CAN_PING_CONT
                RETURN

CAN_PING_CONT
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x37 as CAN Ping response
                MOVLW	0x37
                MOVWF	POSTINC0
                MOVLW	0x8
                MOVWF	POSTINC0
                MOVLW	'U'
                MOVWF	POSTINC0
                MOVLW	'S'
                MOVWF	POSTINC0
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                ; this is a back detector (german Rueckmelder)
                MOVLW	0x40
                MOVWF	POSTINC0
                BRA	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; reboot - e.g. for updates
;------------------------------------------------------------------------------

CAN_REBOOT
                ; if DLC != 0 -> return
                MOVF	CANFRAME+4,F
                BZ	CAN_REBOOT_CONT
                RETURN

CAN_REBOOT_CONT
                GOTO	0

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
