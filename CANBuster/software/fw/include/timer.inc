;
; Can-Can TIMER
;
; Copyright (c) 2015 Gerhard Bertelsmann
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

;------------------------------------------------------------------------------
; settings
;------------------------------------------------------------------------------

#IF PCBV < 1
  #DEFINE       LED         2
#ELSE
  #DEFINE       LED         1
#ENDIF

;------------------------------------------------------------------------------
; Init. Timer
;------------------------------------------------------------------------------
TIMER0INIT
                ; timer period = 1/16MHz = 0.0625 us
                ; prescaler period = .0625us * 32 = 2 us
                ; overflow period  = 2 us * 256 = 512 us

                MOVLW   (1 << TMR0ON) + (1 << T08BIT) + (1 << T0PS2)
                MOVWF   T0CON
                CLRF	HEARTBEAT
                MOVLW	20		; 20 * 512us = 10ms
                MOVWF	PIOTCOUNTER
                RETURN

SCHEDULE
                ; must reset Timer0 overflow flag
                BCF	INTCON, TMR0IF
                INFSNZ	TCOUNTER,F	; heartbeat routine
                RCALL	DO_HEARTBEAT

                DCFSNZ	PIOTCOUNTER,F	; PIO routine
                RCALL	DO_PIO

                ; look for events in the buffer
                RCALL   SENDEVENT
                RETURN

DO_PIO
                RCALL   PIOGET          ; PIO service routine - any PIO change ?
                RCALL   GENEVENT	; buffer events
                MOVLW   20		; 20 * 512us  - check PIO state in 10 ms next time
                ; MOVF    SCAN_INTERVAL,W	; x * 512us  - check PIO state in X ms next time
                MOVWF   PIOTCOUNTER
                RETURN



DO_HEARTBEAT
                ; 512 us * 256 = 131 ms
                ; limit to 8 * 131 ms = 1.049 s
                INCF	HEARTBEAT,F
                MOVLW	7
                ANDWF	HEARTBEAT,F
                ; doing heartbeat - sequence starts on 1 to enable resetting HEARTBEAT var
                ; for other blinking events
                MOVLW	0
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_ON
                MOVLW	1
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_OFF
                MOVLW	2
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_ON
                MOVLW	3
                CPFSGT	HEARTBEAT
                BRA	STATUS_LED_OFF

STATUS_LED_OFF
                BSF	LATC, LED
                RETURN

STATUS_LED_ON
                BCF	LATC, LED
                RETURN

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
