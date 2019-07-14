                RADIX       DEC

;-------------------------------------------------------------------------------
; Device
;-------------------------------------------------------------------------------

                PROCESSOR   PIC10F322

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
;  RxD/DE  ICSPDAT/RA0   1----6 RA3/MCLR/Vpp
;          VSS GND       2    5 VDD
;  TxD     ICSPCLK/RA1   3    4 RA2          -> RS485 DI
;
;-------------------------------------------------------------------------------
; Device Constants
;-------------------------------------------------------------------------------

ERRORLEVEL	-1302
#INCLUDE	"p10f322.inc"

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

; Clock Rate
#DEFINE		CLOCK		16000000

; UART Baud Rate
#DEFINE		RS485_BAUDRATE  62500

; BOOT/LED/SWITCH
#DEFINE		DDR		TRISA
#DEFINE		OUTPUT		LATA
#DEFINE		POLARITY	0
#DEFINE		INPUT		PORTA

CBLOCK		0x70
          START_BYTE:	1; 
ENDC

;-------------------------------------------------------------------------------

  __CONFIG	_FOSC_INTOSC & _BOREN_OFF & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _WRT_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT		ORG	0
		GOTO START

		ORG	8
START		
		MOVLW	0x70		; 16MHz clock
		MOVWF	OSCCON

					; switch off analog
		CLRF	ANSELA		; all digital
		BCF	LATA,0		; clear DE
		BSF	LATA,2		; set DI(TX)
		BSF	WPUA,1		; weak pullup RA1
		MOVLW	b'00000010'	; RA1 input / RA0&RA2 output
		MOVWF	TRISA

		; Timer0
		MOVLW	b'11010111'	; configure Timer0
			; --0-----	;  timer mode (T0CS = 0 -> FOSC/4)
			; ----0---	;  prescaler assigned to Timer0 (PSA = 0)
			; -----111	;  prescaler = 256
		MOVWF	OPTION_REG

		; Timer2
		CLRF	T2CON		; Prescale & Postscaler 1:1 / Timer2 = off
		BCF	PIR1,TMR2IF
		MOVLW	16*9		; 1 Startbit + 8 Databits
		MOVWF	PR2
		CLRF	TMR2

MAINLOOP
		BTFSC	PORTA,1
		GOTO	MAINLOOP
		; first falling edge
		; enable trasnmit
		BSF	LATA,0
		BSF	T2CON,TMR2ON

INNER_LOOP
		BTFSC	LATA,1
		BSF	PORTA,2
		GOTO	NEXT
		BCF	PORTA,2
		NOP

NEXT
		GOTO	INNER_LOOP

		CLRF	TMR0
		BCF	INTCON,TMR0IF

		GOTO	MAINLOOP
		

;-------------------------------------------------------------------------------
THE
                END
