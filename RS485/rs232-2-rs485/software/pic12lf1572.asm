                RADIX       DEC

;-------------------------------------------------------------------------------
; Device
;-------------------------------------------------------------------------------

                PROCESSOR   PIC12F1572

;-------------------------------------------------------------------------------
; Device Pinout
;-------------------------------------------------------------------------------
;
;        VDD 3V3       1----8 VSS GND
; RO'    RA5           2    7 RA0 ICSPDAT RO
; DI'    RA4           3    6 RA1 ICSPCLK DI
; DE/RE  RA3 !MCLR VPP 4----5 RA2         DE/RE'
;
;-------------------------------------------------------------------------------
; Device Constants
;-------------------------------------------------------------------------------

ERRORLEVEL	-1302
#INCLUDE	"p12f1572.inc"

;-------------------------------------------------------------------------------
; Device Settings
;-------------------------------------------------------------------------------

; Clock Rate
#DEFINE         CLOCK       32000000
#DEFINE         TIMEOUT     10

; UART Baud Rate
#DEFINE		RS485_BAUDRATE	62500
#DEFINE		BAUDRATE	115200

; BOOT/LED/SWITCH
#DEFINE		DDR		TRISA
#DEFINE		OUTPUT		LATA
#DEFINE		LED		2
#DEFINE		POLARITY	0
#DEFINE		INPUT		PORTA

;-------------------------------------------------------------------------------
; Device Configuration
;-------------------------------------------------------------------------------

  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF
  __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_ON & _STVREN_ON  & _BORV_LO & _LPBOREN_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT		ORG	0

;                BANKSEL OSCCON              ;BANK ?
;                MOVLW   B'01111010'
;                MOVWF   OSCCON
;INITHFIOFS      BTFSS   OSCSTAT,HFIOFS
;                GOTO    INITHFIOFS

		; switch off analog
                BANKSEL ANSELA              ;BANK ?
                CLRF	ANSELA

		; RO	RAO	out
		; DI	RA1	in
		; DE'	RA2	out
		; 	RA3
		; DI'	RA4	out
		; RO'   RA5	in
		
		BANKSEL TRISA
                MOVLW   B'00100010'
                MOVWF   TRISA

                BANKSEL	APFCON              ;BANK ?
                CLRF	APFCON

MAINLOOP
		BANKSEL	LATA
		GOTO	MAINLOOP

;-------------------------------------------------------------------------------
THE
                END
