
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
                HEARTBEAT : 1
                ENDC

;------------------------------------------------------------------------------
; Timer settings
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Init. Timer
;------------------------------------------------------------------------------
TIMERINIT
		; timer period = 1/16MHz = 0.0625 us
		; prescaler period = .0625us * 32 = 2 us
		; overflow period  = 2 us * 256 = 512 us

                BANKSEL T0CON1
                MOVLW   0x45            ; Fosc/4 & Prescale 32
                MOVWF   T0CON1
                MOVLW   (1 << EN)
                MOVWF   T0CON0
                RETURN

SCHEDULE
                ; must reset Timer1 overflow flag
                BANKSEL PIR3
		BCF	PIR3, TMR0IF
		INCFSZ	TCOUNTER,F
		RETURN

		; 512 us * 256 = 131 ms
		; limit to 8 * 131 ms = 1.049 s
		INCF	HEARTBEAT,F
		MOVLW	7
		ANDWF	HEARTBEAT,F
		; doing heartbeat - sequence starts on 1 to enable resetting HEARTBEAT var
		; for other blinking events
		MOVLW	1
		CPFSGT	HEARTBEAT
		GOTO	STATUS_LED_ON
		MOVLW	2
		CPFSGT	HEARTBEAT
		GOTO	STATUS_LED_OFF
		MOVLW	3
		CPFSGT	HEARTBEAT
		GOTO	STATUS_LED_ON
		MOVLW	4
		CPFSGT	HEARTBEAT
		GOTO	STATUS_LED_OFF

		RETURN

STATUS_LED_OFF
                BANKSEL LATA
		BCF	LATA, 3
		RETURN

STATUS_LED_ON
                BANKSEL LATA
		BSF	LATA, 3
		RETURN


;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
