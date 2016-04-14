;
; Can-Can PWM
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
; PWM variables
;------------------------------------------------------------------------------

                CBLOCK
                PWM_TEMP : 2
                PWM_STATUS : 1
                ENDC

#DEFINE         PWM_PERIOD      (40000)

#DEFINE         PWM_10_HIGH     ((PWM_PERIOD * 50) / 1000)
#DEFINE         PWM_10_LOW      (PWM_PERIOD - PWM_10_HIGH)

#DEFINE         PWM_15_HIGH     ((PWM_PERIOD * 75) / 1000)
#DEFINE         PWM_15_LOW      (PWM_PERIOD - PWM_15_HIGH)

#DEFINE         PWM_20_HIGH     ((PWM_PERIOD * 100) / 1000)
#DEFINE         PWM_20_LOW      (PWM_PERIOD - PWM_20_HIGH)

;------------------------------------------------------------------------------
; PWM settings
;------------------------------------------------------------------------------

PWMINIT
                ;; BCF     TRISC,2		; CCP2 PWM1
                BCF     TRISC,6		; CCP3 PWM2
                BCF     TRISC,7		; CCP4 PWM3
                BCF     TRISB,5		; CCP5 PWM4

                ;; BSF     LATC,2		; CCP2 PWM1
                BSF     LATC,6		; CCP3 PWM2
                BSF     LATC,7		; CCP4 PWM3
                BSF     LATB,5		; CCP5 PWM4

                ;; BCF     PIR3,CCP2IF
                BCF     PIR4,CCP3IF
                BCF     PIR4,CCP4IF
                BCF     PIR4,CCP5IF

                CLRF    PWM_STATUS

                ; using Timer1 as Compare
                CLRF    TMR1H
                CLRF    TMR1L

                ; TEST TEST
                MOVLW   LOW PWM_20_LOW
                ;; MOVWF	CCPR2L
                MOVWF	CCPR3L
                MOVWF	CCPR4L
                MOVWF	CCPR5L
                MOVLW   HIGH PWM_20_LOW
                ;; MOVWF	CCPR2H
                MOVWF	CCPR3H
                MOVWF	CCPR4H
                MOVWF	CCPR5H

                ; CCP setup

                ; use Timer1 for all CCPs
                CLRF    CCPTMRS
                ; Compare Mode
                MOVLW   (1 << CCP2M3) + (1 << CCP2M0)
                ; MOVWF   CCP2CON
                MOVWF   CCP3CON
                MOVWF   CCP4CON
                MOVWF   CCP5CON

                ; using Fosc/4 and 1:8 prescaler
                ; timer period = (4*8)/64MHz = 0.5 us

                MOVLW   (1 << TMR1ON) + (1 << T1CKPS1) + (1 << T1CKPS0)
                MOVWF   T1CON

                RETURN

PWM_TEST
                BRA     PWM_TEST2	;;;;;;;;;; for testing until PCB fixed (RC2 is double used)
                BTFSS   PIR3,CCP2IF
                BCF     PIR3,CCP2IF
                BRA     PWM_TEST2
                BTFSC   CCP2CON,CCP2M0
                BRA     PWM1_LOAD_LOW
                BSF     CCP2CON,CCP2M0		; clear bit next time
                MOVLW   LOW PWM_10_HIGH
                ADDWF   CCPR2L,F
                MOVLW   HIGH PWM_10_HIGH
                ADDWFC  CCPR2H,F
                BRA     PWM_TEST2
PWM1_LOAD_LOW
                BCF     CCP2CON,CCP2M0		; set bit next time
                MOVLW   LOW PWM_10_LOW
                ADDWF   CCPR2L,F
                MOVLW   HIGH PWM_10_LOW
                ADDWFC  CCPR2H,F

PWM_TEST2
                BTFSS   PIR4,CCP3IF
                BRA     PWM_TEST3
                BCF     PIR4,CCP3IF
                BTFSC   CCP3CON,CCP3M0
                BRA     PWM2_LOAD_LOW
                BSF     CCP3CON,CCP3M0		; clear bit next time
                MOVLW   LOW PWM_10_HIGH
                ADDWF   CCPR3L,F
                MOVLW   HIGH PWM_10_HIGH
                ADDWFC  CCPR3H,F
                BRA     PWM_TEST3
PWM2_LOAD_LOW
                BCF     CCP3CON,CCP3M0		; set bit next time
                MOVLW   LOW PWM_10_LOW
                ADDWF   CCPR3L,F
                MOVLW   HIGH PWM_10_LOW
                ADDWFC  CCPR3H,F

PWM_TEST3
                BTFSS   PIR4,CCP4IF
                BRA     PWM_TEST4
                BCF     PIR4,CCP4IF
                BTFSC   CCP4CON,CCP4M0
                BRA     PWM3_LOAD_LOW
                BSF     CCP4CON,CCP4M0		; clear bit next time
                MOVLW   LOW PWM_15_HIGH
                ADDWF   CCPR4L,F
                MOVLW   HIGH PWM_15_HIGH
                ADDWFC  CCPR4H,F
                BRA     PWM_TEST4
PWM3_LOAD_LOW
                BCF     CCP4CON,CCP4M0		; set bit next time
                MOVLW   LOW PWM_15_LOW
                ADDWF   CCPR4L,F
                MOVLW   HIGH PWM_15_LOW
                ADDWFC  CCPR4H,F

PWM_TEST4
                BTFSS   PIR4,CCP5IF
                BRA     PWM_END
                BCF     PIR4,CCP5IF
                BTFSC   CCP5CON,CCP5M0
                BRA     PWM4_LOAD_LOW
                BSF     CCP5CON,CCP5M0		; clear bit next time
                MOVLW   LOW PWM_20_HIGH
                ADDWF   CCPR5L,F
                MOVLW   HIGH PWM_20_HIGH
                ADDWFC  CCPR5H,F
                BRA     PWM_END
PWM4_LOAD_LOW
                BCF     CCP5CON,CCP5M0		; set bit next time
                MOVLW   LOW PWM_20_LOW
                ADDWF   CCPR5L,F
                MOVLW   HIGH PWM_20_LOW
                ADDWFC  CCPR5H,F
PWM_END
                RETURN

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;