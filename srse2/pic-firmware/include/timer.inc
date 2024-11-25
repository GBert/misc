;
; Can-Can TIMER
;
; Copyright (c) 2016 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; This file is part of the Can-Can CAN bus interface project.
;
; Can-Can is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; Timer variables
;------------------------------------------------------------------------------

                CBLOCK
                TCOUNTER : 1
                PIOTCOUNTER : 1
                HEARTBEAT : 1
                ENDC

#IFNDEF SRSE2_SLCAN
                CBLOCK
                TIMER3 : 2
                ENDC
#ENDIF

;------------------------------------------------------------------------------
; settings
;------------------------------------------------------------------------------

#DEFINE         LED         3

;------------------------------------------------------------------------------
; Init. Timer
;------------------------------------------------------------------------------

TIMER0INIT
                ; timer period = 1/16MHz = 0.0625 us
                ; prescaler period = .0625us * 8 = 0.5 us
                ; overflow period  = 0.5 us * 256 = 128 us

                MOVLW   (1 << TMR0ON) + (1 << T08BIT) + (1 << T0PS1)
                MOVWF   T0CON
                CLRF	HEARTBEAT
                MOVLW	8         ; 8 * 128us = 1 ms
                MOVWF	PIOTCOUNTER
                RETURN

; Setup Timer 3 as free running counter

TIMER3INIT
#IFNDEF SRSE2_SLCAN ;not used in SRSE2_SLCAN
                ; timer period = 1/16MHz = 0.0625 us
                ; overflow period  = 0.0625 us * 65536 = 4,096ms
                MOVLW   (1 << TMR3ON) + (1 << RD16)
                MOVWF   T3CON
                CLRF    TIMER3
                CLRF    TIMER3+1
#ENDIF
                RETURN

;------------------------------------------------------------------------------
; Service routine
;------------------------------------------------------------------------------

SCHEDULE
                ; must reset Timer0 overflow flag
                BCF     INTCON, TMR0IF
                INFSNZ	TCOUNTER,F      ; heartbeat routine
                RCALL	DO_HEARTBEAT
                ;do S88 pulse train if eventcount is low
                ;check pending event count
                ;skip S88 if too many events pending
                MOVLW   32
                SUBWF   EVENTCNT,W
                BTFSS   STATUS,C
                CALL    S88_FSM

#IFNDEF SRSE2_SLCAN
                ; update free running counter (4.096ms)
                BTFSS   PIR2, TMR3IF
                BRA     SCHEDULE_NEXT1
                BCF     PIR2,TMR3IF
                INCFSZ  TIMER3+1,F
                INCF    TIMER3,F
#IFDEF XPN_CODE
                CALL    XPNSLOT
#ENDIF

#ENDIF    ;ifndef SRSE2_SLCAN

SCHEDULE_NEXT1
                DECFSZ  PIOTCOUNTER,F	; PIO routine
                BRA     SCHEDULE_NEXT2
                BSF	PIOTCOUNTER, 3  ; 8 * 128us = 1 ms
                ; look for events in the event send buffer
                TSTFSZ  EVENTCNT
                GOTO    SENDEVENT
SCHEDULE_NEXT2
                ;look for forced S88 events to be sent
                ;only if eventcnt = 0, no events pending
                ;S88_DELAY_COUNT = 0
                TSTFSZ  EVENTCNT
                RETURN
                ;EVENTCNT = 0
                GOTO	S88_CHECK_FORCED_EVENTS

DO_HEARTBEAT
                ; 128 us * 256 = 33 ms
                ; limit to 32 * 33 ms = 1.049 s
                INCF	HEARTBEAT,F
;                MOVLW	31
;                ANDWF	HEARTBEAT,F
                ; doing heartbeat - sequence starts on 1 to enable resetting HEARTBEAT var
                ; for other blinking events
                MOVF	HEARTBEAT,W
                BZ	STATUS_P1
                XORLW	4
                BZ	STATUS_P2
                XORLW	8 ^ 4
                BZ	STATUS_P3
                XORLW	12 ^ 8
                BZ	STATUS_P4
                XORLW	31 ^ 12
                BTFSS	STATUS,Z
                RETURN
                MOVLW	0xFF
                MOVWF	HEARTBEAT
                RETURN

;                MOVLW	0
;                CPFSGT	HEARTBEAT
;                BRA	STATUS_P1
;                MOVLW	4
;                CPFSGT	HEARTBEAT
;                BRA	STATUS_P2
;                MOVLW	8
;                CPFSGT	HEARTBEAT
;                BRA	STATUS_P3
;                MOVLW	12
;                CPFSGT	HEARTBEAT
;                BRA	STATUS_P4

;                RETURN

STATUS_P1
                BTFSS   SYSTEM_FLAGS,SYSTEMFLAG_LED_TEST
                BRA     STATUS_P1_LEDEND
                BCF     SYSTEM_FLAGS,SYSTEMFLAG_LED_TEST
                ;All LEDs off
#IF LEDON == 0
                MOVLW   0x07
                IORWF   GPIO,F
#ELSE
               MOVLW    0xF8
               ANDWF    GPIO,F
#ENDIF

STATUS_P1_LEDEND
                BCF     LATA, LED
                BTFSC   OPTION_FLAGS, OPTION_LED_ON_EEP_WRITE
                LED_EEPWRITE_OFF
#IFDEF NO_CAN_LED
                POWER_LED_ON
#ENDIF
                RETURN

STATUS_P3
                BTFSC  SYSTEM_FLAGS,SYSTEMFLAG_LED_TEST
                RETURN
                BCF	LATA, LED
                RETURN
STATUS_P2
                BTFSC  SYSTEM_FLAGS,SYSTEMFLAG_LED_TEST
                RETURN
                BSF	LATA, LED
                RETURN
STATUS_P4
                BTFSC  SYSTEM_FLAGS,SYSTEMFLAG_LED_TEST
                RETURN
                BSF	LATA, LED

#IFDEF NO_CAN_LED
                BTFSS	SYSTEM_FLAGS,SYSTEM_FLAG_STOP_GO
                POWER_LED_OFF
#ENDIF
                RETURN


;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
