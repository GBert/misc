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

#IFNDEF SRSE2_SLCAN
                CBLOCK
                PWM_VALUE1 : 2
                PWM_VALUE2 : 2
                PWM_VALUE3 : 2
                PWM_VALUE4 : 2
                PWM_CHANNEL : 1
                ENDC

PWM_VALUE1_LOW    EQU     0x300
PWM_VALUE1_HIGH   EQU     0x301
PWM_VALUE2_LOW    EQU     0x302
PWM_VALUE2_HIGH   EQU     0x303
PWM_VALUE3_LOW    EQU     0x304
PWM_VALUE3_HIGH   EQU     0x305
PWM_VALUE4_LOW    EQU     0x306
PWM_VALUE5_HIGH   EQU     0x307

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


#DEFINE PWMMAX (40000)

                ; substract value from PWMMAX and load CCPs
                ; calculate PWM values
CCP_LOAD_LOW    MACRO   PWM_HIGH, CCP_HIGH, CCP_LOW
                MOVLW   HIGH PWMMAX
                MOVWF   TEMP16
                MOVLW   LOW PWMMAX
                MOVWF   TEMP16+1

                MOVFF   PWM_HIGH,TEMP     ; MSB
                MOVF    PWM_HIGH+1,W      ; LSB
                SUBWF   TEMP16+1,F
                BTFSS   STATUS,C
                INCF    TEMP,W	          ; MSB
                SUBWF   TEMP16,F

                ; RCALL   DEBUG_CAN
                MOVF    CCP_LOW,W
                ADDWF   TEMP16+1,F
                MOVF    TEMP16,W
                ADDWFC  CCP_HIGH,F        ; load new CCPRxH &
                MOVFF   TEMP16+1,CCP_LOW  ; CCPRxL

                ENDM

PWMINIT
#IF PCBV > 1
                BCF     TRISC,2		; CCP2 PWM1
#ENDIF
                BCF     TRISC,6		; CCP3 PWM2
                BCF     TRISC,7		; CCP4 PWM3
                BCF     TRISB,5		; CCP5 PWM4
#IF PCBV > 1
                BSF     LATC,2		; CCP2 PWM1
#ENDIF
                BSF     LATC,6		; CCP3 PWM2
                BSF     LATC,7		; CCP4 PWM3
                BSF     LATB,5		; CCP5 PWM4

#IF PCBV > 1
                BCF     PIR3,CCP2IF
#ENDIF
                BCF     PIR4,CCP3IF
                BCF     PIR4,CCP4IF
                BCF     PIR4,CCP5IF

                CLRF    PWM_CHANNEL
                MOVLW   HIGH PWM_10_HIGH
                MOVWF   PWM_VALUE1
                MOVLW   LOW PWM_10_HIGH
                MOVWF   PWM_VALUE1+1

                MOVLW   HIGH PWM_10_HIGH
                MOVWF   PWM_VALUE2
                MOVLW   LOW PWM_10_HIGH
                MOVWF   PWM_VALUE2+1

                MOVLW   HIGH PWM_15_HIGH
                MOVWF   PWM_VALUE3
                MOVLW   LOW PWM_15_HIGH
                MOVWF   PWM_VALUE3+1

                MOVLW   HIGH PWM_20_HIGH
                MOVWF   PWM_VALUE4
                MOVLW   LOW PWM_20_HIGH
                MOVWF   PWM_VALUE4+1

                ; using Timer1 as Compare
                CLRF    TMR1H
                CLRF    TMR1L

                ; TEST TEST
                MOVLW   LOW PWM_20_LOW
                MOVWF	CCPR2L
                MOVWF	CCPR3L
                MOVWF	CCPR4L
                MOVWF	CCPR5L
                MOVLW   HIGH PWM_20_LOW
                MOVWF	CCPR2H
                MOVWF	CCPR3H
                MOVWF	CCPR4H
                MOVWF	CCPR5H

                ; CCP setup

                ; use Timer1 for all CCPs
                CLRF    CCPTMRS
                ; Compare Mode
                MOVLW   (1 << CCP2M3) + (1 << CCP2M0)
#IF PCBV ==  2
                MOVWF   CCP2CON
#ENDIF
                MOVWF   CCP3CON
                MOVWF   CCP4CON
                MOVWF   CCP5CON

                ; using Fosc/4 and 1:8 prescaler
                ; timer period = (4*8)/64MHz = 0.5 us

                MOVLW   (1 << TMR1ON) + (1 << T1CKPS1) + (1 << T1CKPS0)
                MOVWF   T1CON

                RETURN

PWM_TEST
#IF PCBV < 2
                BRA     PWM_TEST2
#ENDIF
                BTFSS   PIR3,CCP2IF
                BRA     PWM_TEST2
                BCF     PIR3,CCP2IF
                BTFSC   CCP2CON,CCP2M0
                BRA     PWM1_LOAD_LOW
                BSF     CCP2CON,CCP2M0		; clear bit next time
                MOVF    PWM_VALUE1+1,W
                ADDWF   CCPR2L,F
                MOVF    PWM_VALUE1,W
                ADDWFC  CCPR2H,F
                BRA     PWM_TEST2
PWM1_LOAD_LOW
                BCF     CCP2CON,CCP2M0		; set bit next time
                CCP_LOAD_LOW  PWM_VALUE1, CCPR2H, CCPR2L

PWM_TEST2
                BTFSS   PIR4,CCP3IF
                BRA     PWM_TEST3
                BCF     PIR4,CCP3IF
                BTFSC   CCP3CON,CCP3M0
                BRA     PWM2_LOAD_LOW
                BSF     CCP3CON,CCP3M0		; clear bit next time
                MOVF    PWM_VALUE2+1,W
                ADDWF   CCPR3L,F
                MOVF    PWM_VALUE2,W
                ADDWFC  CCPR3H,F
                BRA     PWM_TEST3
PWM2_LOAD_LOW
                BCF     CCP3CON,CCP3M0		; set bit next time
                CCP_LOAD_LOW  PWM_VALUE2, CCPR3H, CCPR3L

PWM_TEST3
                BTFSS   PIR4,CCP4IF
                BRA     PWM_TEST4
                BCF     PIR4,CCP4IF
                BTFSC   CCP4CON,CCP4M0
                BRA     PWM3_LOAD_LOW
                BSF     CCP4CON,CCP4M0		; clear bit next time
                MOVF    PWM_VALUE3+1,W
                ADDWF   CCPR4L,F
                MOVF    PWM_VALUE3,W
                ADDWFC  CCPR4H,F
                BRA     PWM_TEST4
PWM3_LOAD_LOW
                BCF     CCP4CON,CCP4M0		; set bit next time
                CCP_LOAD_LOW  PWM_VALUE3, CCPR4H, CCPR4L

PWM_TEST4
                BTFSS   PIR4,CCP5IF
                BRA     PWM_END
                BCF     PIR4,CCP5IF
                BTFSC   CCP5CON,CCP5M0
                BRA     PWM4_LOAD_LOW
                BSF     CCP5CON,CCP5M0		; clear bit next time
                MOVF    PWM_VALUE4+1,W
                ADDWF   CCPR5L,F
                MOVF    PWM_VALUE4,W
                ADDWFC  CCPR5H,F
                BRA     PWM_END
PWM4_LOAD_LOW
                BCF     CCP5CON,CCP5M0		; set bit next time
                CCP_LOAD_LOW  PWM_VALUE4, CCPR5H, CCPR5L

PWM_END
                RETURN


DEBUG_CAN
                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L

                MOVLW   0
                MOVFF   WREG,POSTINC0
                MOVFF   WREG,POSTINC0
                MOVFF   WREG,POSTINC0
                MOVFF   WREG,POSTINC0
                MOVLW   8
                MOVFF   WREG,POSTINC0
                MOVLW   HIGH PWMMAX
                MOVFF   WREG,POSTINC0
                MOVLW   LOW PWMMAX
                MOVFF   WREG,POSTINC0

                MOVFF   PWM_VALUE1,POSTINC0
                MOVFF   PWM_VALUE1+1,POSTINC0
                MOVFF   TEMP,POSTINC0
                MOVFF   TEMP+1,POSTINC0
                MOVFF   TEMP16,POSTINC0
                MOVFF   TEMP16+1,POSTINC0

                BRA     EVENT_RING_NORMALIZE

                RETURN
#ENDIF ;IFNDEF SRSE2_SLCAN
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
