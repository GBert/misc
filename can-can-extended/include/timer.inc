;------------------------------------------------------------------------------
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
; Timer settings
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Init. Timer
;------------------------------------------------------------------------------
TIMERINIT
		; timer period = 1/16MHz = 0.0625 us
		; prescaler period = .0625us * 64 = 4 us
		; overflow period  = 4us * 256 = 1024 ms
		
                MOVLW   (1 << TMR0ON) + (1 << T08BIT) + (1 << T0PS2) + (1 << T0PS0)
                MOVWF   T0CON
                RETURN

SCHEDULE
		BCF	INTCON, TMR0IF
		BTG	LATA, 3
		RETURN
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
