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
                ;
                ; System CMD 0x00 ?
                MOVF	CANFRAME+1,F
                BZ	SYSYTEM_CMD

                MOVF	CANFRAME+1,W
                ; CAN PING 0x30 ?
                XORLW	0x30
                BZ	CAN_PING

                RETURN

;------------------------------------------------------------------------------
; be sure the pointer is in the appropiate range
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
; answer System Command (0x00)
;------------------------------------------------------------------------------

SYSYTEM_CMD
                MOVF    CANFRAME+5,W
                XORLW	'C'
                BNZ	SYSYTEM_CMD_OTHER
                MOVF    CANFRAME+6,W
                XORLW	'B'
                BNZ	SYSYTEM_CMD_OTHER
                MOVF    CANFRAME+7,W
                XORLW	'U'
                BNZ	SYSYTEM_CMD_OTHER
                MOVF    CANFRAME+8,W
                XORLW	'S'
                BNZ	SYSYTEM_CMD_OTHER
		; Reboot ?
                ; DLC = 6
                MOVF	CANFRAME+4,W
                XORLW	6
                BNZ	SYSYTEM_CMD_OTHER
                MOVF    CANFRAME+9,W
                XORLW	0x80
                BNZ	SYSYTEM_CMD_OTHER
                BRA	CAN_REBOOT

SYSYTEM_CMD_OTHER
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x01 as System CMD Response
                MOVLW	0x01
                MOVWF	POSTINC0
                MOVF	CANFRAME+4,W
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	INDF0
                BRA	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; System Sub CMD Reset 0x80 -> reboot - e.g. for updates
;------------------------------------------------------------------------------

CAN_REBOOT
                GOTO	0

;------------------------------------------------------------------------------
; answer CAN Ping (0x30)
;------------------------------------------------------------------------------

CAN_PING
                ; if DLC != 0 -> return
                MOVF	CANFRAME+4,F
                BZ	CAN_PING_CONT
                RETURN

CAN_PING_CONT
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x31 as CAN Ping response
                MOVLW	0x31
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

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
