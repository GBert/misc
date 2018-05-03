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

#DEFINE		I2C_BAUDRATE	100000

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

#INCLUDE	ring.inc


;------------------------------------------------------------------------------

		CBLOCK	0x0070
		PULSE_LOW	: 2
		PULSE_HIGH	: 2
		B_SAVE		: 1
		TMR0_COUNT	: 1
		ENDC

;------------------------------------------------------------------------------

		ORG	0x0000
		GOTO	INIT

		ORG	0x0004
ISR
		SET_C3
		; CCP1
		BANKSEL	PIR1
		BTFSS	PIR1,CCP1IF
		GOTO    ISRNEXT1
		;
		BCF	PIR1,CCP1IF
		BANKSEL	CCP1CON
		BTFSS	CCP1CON,CCP1M0
		GOTO	ISRCCP1LOW
		BCF	CCP1CON,CCP1M0
		MOVF	LOW PULSE_LOW,W
		ADDWF	CCPR1L,F
		MOVF	HIGH PULSE_LOW,W
		GOTO	ISRCCP1END
ISRCCP1LOW
		;
		BANKSEL	CCP1CON
		BSF	CCP1CON,CCP1M0
		MOVF	LOW PULSE_HIGH,W
		ADDWF	CCPR1L,F
		MOVF	HIGH PULSE_HIGH,W
ISRCCP1END
		ADDWFC	CCPR1H,F

ISRNEXT1
		; UART RX
		BANKSEL	PIR1
		BTFSS	PIR1,RCIF
		GOTO	ISRNEXT2
		BANKSEL	RC1REG
		MOVF	RC1REG,W
		; put char onto rx ring buffer
		MOVWF	RX_TEMP
		INCF	RX_HEAD,W
		ANDLW	0x1F		; limit to 31
		MOVWF	RX_HEAD
		MOVWF	FSR1L
		MOVF	RX_TEMP,W
		MOVWI	0x00[FSR1]

		; TODO test shortcut
		MOVWF	TX1REG
ISRNEXT2
		; UART TX
		BANKSEL	PIR1
		BTFSS   PIR1,TXIF
		GOTO	ISRNEXT3
		BANKSEL	PIE1
		BTFSS   PIR1,TXIE
		GOTO	ISRNEXT3
		; send next char from tx ring buffer
		BANKSEL	TX_TAIL
		INCF	TX_TAIL,W
		ANDLW	0x1F
		MOVWF	TX_TAIL
		MOVWF	FSR1L
		MOVIW	-0x20[FSR1]
		; send 
		MOVWF	TX1REG
		; check for empty buffer 
		MOVFW	TX_TAIL
		XORWF	TX_HEAD,W
		BTFSS	STATUS,Z
		GOTO	ISRNEXT3
		BANKSEL	PIE1
		BCF	PIE1,TXIE

ISRNEXT3
		; TIMER 0
		BTFSS	INTCON,TMR0IF
		GOTO	ISRNEXT4
		BCF	INTCON,TMR0IF
		; load Timer0 value again
		MOVLW	TIMER0_VAL
		BANKSEL	TMR0
		MOVWF	TMR0
		INCF	TMR0_COUNT

ISRNEXT4
		CLR_C3
		RETFIE

;---------------------------------------

		; RA4 SDA I2C
		; RA5 SCL I2C
		; RC1 RxD UART
		; RC2 TxD UART
		; RC3 Rail Signal
		; RB5 RPWM
		; RB6 LPWM
		; RC3 CCP1  -> debugging

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

		; define RA4&RA5 as inputs for I2C as described in the docs
		MOVLW	b'00110100'
		BANKSEL	TRISA
		MOVWF	TRISA

		MOVLW	b'00000000'
		MOVWF	TRISB

		BANKSEL	LATC
		BSF	LATC,2
		BANKSEL	TRISC
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

		; init all peripherals
		CALL	INITGPIO
		CALL	INITPPS
		CALL	INITBUFFER
		CALL	INITUART
		CALL	INITTIMER
		CALL	INITCOG
		CALL	INITI2C

		CLRF	TMR0_COUNT
		CLRF	HIGH PULSE_LOW
		MOVLW	10
		MOVWF	LOW PULSE_LOW
		CLRF	HIGH PULSE_HIGH
		MOVLW	30
		MOVWF	LOW PULSE_HIGH

		BSF	INTCON,TMR0IE
		BSF	INTCON,PEIE	; Init. ISR
		BSF	INTCON,GIE
		BANKSEL	PIE1
		BSF	PIE1,RCIE
		BSF	PIE1,CCP1IE

MAIN
		; ELO
		BTFSS	TMR0_COUNT,7
		BRA	LED_N1
		BANKSEL	LATC
		BSF	LATC,5
		BRA	MAIN_NEXT1
LED_N1
		BANKSEL	LATC
		BTFSS	LATC,5		; look for change
		BRA	MAIN_NEXT1	; used as trigger 128 * 4ms = 512 ms
		BCF	LATC,5
		MOVLW	'a'
		PUTC
		MOVLW	'b'
		PUTC
		MOVLW	'c'
		PUTC
		BANKSEL	PIE1
		BSF	PIE1,TXIE
		CALL	I2C_TEST

MAIN_NEXT1
		GOTO	MAIN


#INCLUDE	"pps.inc"
#INCLUDE	"uart.inc"
#INCLUDE	"timer.inc"
#INCLUDE	"cog.inc"
#INCLUDE	"i2c.inc"
#INCLUDE	"i2c_lcd.inc"

;------------------------------------------------------------------------------
THE
                END
