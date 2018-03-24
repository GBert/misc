;------------------------------------------------------------------------------
;
; My GFP
;
; Copyright (c) 2018 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------

		RADIX   DEC

;------------------------------------------------------------------------------
; Device
;------------------------------------------------------------------------------

		PROCESSOR   16f1709

;------------------------------------------------------------------------------
; Device constants
;------------------------------------------------------------------------------

		ERRORLEVEL  -302,-1301
		LIST        P=PIC16F1709
#INCLUDE	"p16f1709.inc"
		LIST

;------------------------------------------------------------------------------
; Device settings
;------------------------------------------------------------------------------

; Clock Rate
#DEFINE		CLOCK		32000000

; UART Baud Rate
#DEFINE		BAUDRATE	115200

#DEFINE         INTERVAL        250
#DEFINE         TIMER0_VAL      (255 - (INTERVAL-1))

;------------------------------------------------------------------------------

		CONFIG	FOSC=INTOSC
		CONFIG	PLLEN=OFF
		CONFIG	WDTE=OFF
		CONFIG	LVP=ON
		CONFIG	MCLRE=ON
		CONFIG	CLKOUTEN=OFF

;------------------------------------------------------------------------------


CLR_C3		MACRO
		BANKSEL LATC
		BCF	LATC,3
		ENDM

SET_C3		MACRO
		BANKSEL LATC
		BSF	LATC,3
		ENDM

TOGGLE_C3	MACRO
		BANKSEL LATC
		MOVLW   b'000001000'
		XORWF   LATC,F
		ENDM
		
;------------------------------------------------------------------------------

		ORG	0x0000
		GOTO	INIT

		ORG	0x0004
ISR
		; TIMER 0
		BTFSS	INTCON,TMR0IF
		GOTO	ISRNEXT1
		BCF	INTCON,TMR0IF
		; load Timer0 value again
		MOVLW	TIMER0_VAL
		BANKSEL	TMR0
		MOVWF	TMR0

ISRNEXT1
		TOGGLE_C3
		RETFIE

INITGPIO
		BANKSEL	ANSELA
		MOVLW	b'00000100'	; RA2 analog
		MOVWF	ANSELA
		CLRF	ANSELB
		MOVLW	b'00000001'	; RC0 analog
		MOVWF	ANSELC
		BANKSEL	CM1CON0
		CLRF	CM1CON0
		CLRF	CM1CON1
		CLRF	CM2CON0
		CLRF	CM2CON1
		BANKSEL	ADCON0
		CLRF	ADCON0
		MOVLW	b'11100000'	; right justified ; FOSC/64 ;VREF- GND & VREF+ VDD
		MOVWF	ADCON1
		CLRF	ADCON2

		MOVLW	b'00110100'
		BANKSEL	TRISA
		MOVWF	TRISA

		MOVLW	b'00000000'
		MOVWF	TRISB

		MOVLW	b'00000011'
		MOVWF	TRISC

		RETURN

INIT
		MOVLW	b'11110000';
			 ;1-------	use PLL to get 4x8 Mhz (system clock)
			 ;-1110---	8 MHz internal oscillator (instruction clock)
			 ;------00	oscillator selected with INTOSC
		BANKSEL	OSCCON
		MOVWF	OSCCON

		CLRF    INTCON		; Reset ISR
		BANKSEL	PIE1
		CLRF    PIE1
		BANKSEL	PIR1
		CLRF    PIR1


		CALL	INITGPIO
		CALL	INITPPS
		CALL	INITUART
		CALL	INITTIMER

		BSF	INTCON,TMR0IE
		BSF     INTCON,PEIE	; Init. ISR
		BSF     INTCON,GIE

		CLR_C3
MAIN
		;TOGGLE_C3
		BRA	MAIN


#INCLUDE	"pps.inc"
#INCLUDE	"uart.inc"
#INCLUDE	"timer.inc"

;------------------------------------------------------------------------------
THE
                END
