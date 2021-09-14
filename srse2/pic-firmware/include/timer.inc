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
                TIMER3 : 2
                ENDC

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
                MOVLW	8		; 8 * 128us = 1 ms
                MOVWF	PIOTCOUNTER
                RETURN

; Setup Timer 3 as free running counter

TIMER3INIT
                ; timer period = 1/16MHz = 0.0625 us
                ; overflow period  = 0.0625 us * 65536 = 4,096ms
                MOVLW   (1 << TMR3ON) + (1 << RD16)
                MOVWF   T3CON
                CLRF    TIMER3
                CLRF    TIMER3+1
                RETURN

;------------------------------------------------------------------------------
; Service routine
;------------------------------------------------------------------------------

SCHEDULE
                ; must reset Timer0 overflow flag
                BCF	INTCON, TMR0IF
                INFSNZ	TCOUNTER,F	; heartbeat routine
                RCALL	DO_HEARTBEAT
                CALL    S88_FSM

                ; update free running counter (4.096ms)
                BTFSS   PIR2, TMR3IF
                BRA     SCHEDULE_NEXT1
                BCF     PIR2,TMR3IF
                INCFSZ  TIMER3+1,F
                INCF    TIMER3,F
                CALL    XPNSLOT

SCHEDULE_NEXT1
                DECFSZ  PIOTCOUNTER,F	; PIO routine
                BRA     SCHEDULE_NEXT2
                MOVLW   8		; 8 * 128us = 1 ms
                MOVWF   PIOTCOUNTER
                ; look for events in the buffer
                CALL    SENDEVENT
SCHEDULE_NEXT2
                RETURN

DO_HEARTBEAT
                ; 128 us * 256 = 33 ms
                ; limit to 32 * 33 ms = 1.049 s
                INCF	HEARTBEAT,F
                MOVLW	31
                ANDWF	HEARTBEAT,F
                ; doing heartbeat - sequence starts on 1 to enable resetting HEARTBEAT var
                ; for other blinking events
                MOVLW	0
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_ON
                MOVLW	4
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_OFF
                MOVLW	8
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_ON
                MOVLW	12
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_OFF

                RETURN

STATUS_LED_OFF
                BSF	LATA, LED
                RETURN

STATUS_LED_ON
                BCF	LATA, LED
                RETURN

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
