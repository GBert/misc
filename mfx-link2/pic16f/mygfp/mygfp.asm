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

		CONFIG	FOSC=INTOSC
		CONFIG	PLLEN=OFF
		CONFIG	WDTE=OFF
		CONFIG	LVP=ON
		CONFIG	MCLRE=ON
		CONFIG	CLKOUTEN=OFF

;------------------------------------------------------------------------------

		ORG	0x0000
		GOTO	INIT

		ORG	0x0004

		BANKSEL	LATC
		BCF	LATC,3

		RETFIE

INIT		
		MOVLB   0x0F                ; Default to Bank 15

		BANKSEL	INTCON

		CLRF    INTCON              ; Reset ISR
		CLRF    PIE1
		CLRF    PIR1

		CALL	INITPPS
		CALL	INITUART

                BSF     INTCON,PEIE         ; Init. ISR
                BSF     INTCON,GIE

MAIN
		BRA	MAIN


#INCLUDE	"pps.inc"
#INCLUDE	"uart.inc"

;------------------------------------------------------------------------------
THE
                END
