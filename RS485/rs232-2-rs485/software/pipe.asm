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

#DEFINE		UOUT		LATA
#DEFINE		UDDR		TRISA
#DEFINE		UINP		PORTA
#DEFINE		UTXD1		0
#DEFINE		URXD1		1
#DEFINE		URXD2		4
#DEFINE		UTXD2		5
#DEFINE		UDERE		2

#DEFINE		TYPE		2
#DEFINE		UART		1

; Clock Rate
#DEFINE		CLOCK		32000000
#DEFINE		TIMEOUT		10

; UART Baud Rate
#DEFINE		RS485_BAUDRATE  62500
#DEFINE		BAUDRATE	115200

; BOOT/LED/SWITCH
#DEFINE		DDR		TRISA
#DEFINE		OUTPUT		LATA
#DEFINE		POLARITY	0
#DEFINE		INPUT		PORTA

; Device REG, goto LOC if result not ZERO
DJNZ		MACRO   REG,LOC
		DECFSZ  REG,F
		GOTO    LOC
		ENDM

CBLOCK          0x70
    RCOUNT	: 1
    TCOUNT	: 1
    UREG1	: 1
    UREG2	: 1
ENDC

;-------------------------------------------------------------------------------

  __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_SWDTEN & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF
  __CONFIG _CONFIG2, _WRT_OFF & _PLLEN_ON & _STVREN_ON  & _BORV_LO & _LPBOREN_OFF & _LVP_ON

;-------------------------------------------------------------------------------
; Device Init.
;-------------------------------------------------------------------------------

INIT		ORG	0
		GOTO START

		ORG	4
		RETFIE

START
		MOVLW	0xff
		MOVWF	UREG1
		MOVLW	0xff
		MOVWF	UREG2

LOOP
		BANKSEL	UINP		;    1T
		BSF	UREG1,7		;    1T
		BTFSS	UINP,URXD1	; |
		BCF     UREG1,7		; |- 2T
					; S	4T

                BANKSEL UOUT		;    1T
                RRF     UREG1,F		;    1T
                BTFSS   STATUS,C	; |
                BCF     UOUT,UTXD1	; |- 2T
                BTFSC   STATUS,C	; |
                BSF     UOUT,UTXD1	; |- 2T
					; S	6T


		BANKSEL	UINP		;    1T
		BSF	UREG2,7		;    1T
		BTFSS	UINP,URXD2	; |
		BCF     UREG2,7		; |- 2T
					; S	4T
		
		BANKSEL UOUT		;    1T
		RRF     UREG2,F		;    1T
		BTFSS   STATUS,C	; |
		BCF     UOUT,UTXD2	; |- 2T
		BTFSC   STATUS,C	; |
		BSF     UOUT,UTXD2	; |- 2T
					; S	6T

		GOTO	LOOP		; 2T

;-------------------------------------------------------------------------------
THE
                END

