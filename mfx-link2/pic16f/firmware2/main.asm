;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #10289 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"main.c"
	list	p=16f1709
	radix dec
	include "p16f1709.inc"
;--------------------------------------------------------
; config word(s)
;--------------------------------------------------------
	__config _CONFIG1, 0x3ee4
	__config _CONFIG2, 0x3fff
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_putchar
	extern	_putchar_wait
	extern	_puts_rom
	extern	_init_usart
	extern	_fifo_putchar
	extern	_print_rom_fifo
	extern	_print_debug_value
	extern	_print_debug_fifo
	extern	_LCD_init
	extern	_LCD_putcmd
	extern	_LCD_putch
	extern	_LCD_puts
	extern	_LCD_goto
	extern	_STATUSbits
	extern	_BSRbits
	extern	_INTCONbits
	extern	_PORTAbits
	extern	_PORTBbits
	extern	_PORTCbits
	extern	_PIR1bits
	extern	_PIR2bits
	extern	_PIR3bits
	extern	_T1CONbits
	extern	_T1GCONbits
	extern	_T2CONbits
	extern	_TRISAbits
	extern	_TRISBbits
	extern	_TRISCbits
	extern	_PIE1bits
	extern	_PIE2bits
	extern	_PIE3bits
	extern	_OPTION_REGbits
	extern	_PCONbits
	extern	_WDTCONbits
	extern	_OSCTUNEbits
	extern	_OSCCONbits
	extern	_OSCSTATbits
	extern	_ADCON0bits
	extern	_ADCON1bits
	extern	_ADCON2bits
	extern	_LATAbits
	extern	_LATBbits
	extern	_LATCbits
	extern	_CM1CON0bits
	extern	_CM1CON1bits
	extern	_CM2CON0bits
	extern	_CM2CON1bits
	extern	_CMOUTbits
	extern	_BORCONbits
	extern	_FVRCONbits
	extern	_DAC1CON0bits
	extern	_DAC1CON1bits
	extern	_ZCD1CONbits
	extern	_ANSELAbits
	extern	_ANSELBbits
	extern	_ANSELCbits
	extern	_PMCON1bits
	extern	_VREGCONbits
	extern	_RC1STAbits
	extern	_RCSTAbits
	extern	_RCSTA1bits
	extern	_TX1STAbits
	extern	_TXSTAbits
	extern	_TXSTA1bits
	extern	_BAUD1CONbits
	extern	_BAUDCONbits
	extern	_BAUDCON1bits
	extern	_BAUDCTLbits
	extern	_BAUDCTL1bits
	extern	_WPUAbits
	extern	_WPUBbits
	extern	_WPUCbits
	extern	_SSP1BUFbits
	extern	_SSPBUFbits
	extern	_SSP1ADDbits
	extern	_SSPADDbits
	extern	_SSP1MSKbits
	extern	_SSPMSKbits
	extern	_SSP1STATbits
	extern	_SSPSTATbits
	extern	_SSP1CONbits
	extern	_SSP1CON1bits
	extern	_SSPCONbits
	extern	_SSPCON1bits
	extern	_SSP1CON2bits
	extern	_SSPCON2bits
	extern	_SSP1CON3bits
	extern	_SSPCON3bits
	extern	_ODCONAbits
	extern	_ODCONBbits
	extern	_ODCONCbits
	extern	_CCP1CONbits
	extern	_ECCP1CONbits
	extern	_CCP2CONbits
	extern	_ECCP2CONbits
	extern	_CCPTMRSbits
	extern	_SLRCONAbits
	extern	_SLRCONBbits
	extern	_SLRCONCbits
	extern	_INLVLAbits
	extern	_INLVLBbits
	extern	_INLVLCbits
	extern	_IOCAPbits
	extern	_IOCANbits
	extern	_IOCAFbits
	extern	_IOCBPbits
	extern	_IOCBNbits
	extern	_IOCBFbits
	extern	_IOCCPbits
	extern	_IOCCNbits
	extern	_IOCCFbits
	extern	_T4CONbits
	extern	_T6CONbits
	extern	_OPA1CONbits
	extern	_OPA2CONbits
	extern	_PWM3DCLbits
	extern	_PWM3DCHbits
	extern	_PWM3CONbits
	extern	_PWM3CON0bits
	extern	_PWM4DCLbits
	extern	_PWM4DCHbits
	extern	_PWM4CONbits
	extern	_PWM4CON0bits
	extern	_COG1PHRbits
	extern	_COG1PHFbits
	extern	_COG1BLKRbits
	extern	_COG1BLKFbits
	extern	_COG1DBRbits
	extern	_COG1DBFbits
	extern	_COG1CON0bits
	extern	_COG1CON1bits
	extern	_COG1RISbits
	extern	_COG1RSIMbits
	extern	_COG1FISbits
	extern	_COG1FSIMbits
	extern	_COG1ASD0bits
	extern	_COG1ASD1bits
	extern	_COG1STRbits
	extern	_PPSLOCKbits
	extern	_CLCDATAbits
	extern	_CLC1CONbits
	extern	_CLC1POLbits
	extern	_CLC1SEL0bits
	extern	_CLC1SEL1bits
	extern	_CLC1SEL2bits
	extern	_CLC1SEL3bits
	extern	_CLC1GLS0bits
	extern	_CLC1GLS1bits
	extern	_CLC1GLS2bits
	extern	_CLC1GLS3bits
	extern	_CLC2CONbits
	extern	_CLC2POLbits
	extern	_CLC2SEL0bits
	extern	_CLC2SEL1bits
	extern	_CLC2SEL2bits
	extern	_CLC2SEL3bits
	extern	_CLC2GLS0bits
	extern	_CLC2GLS1bits
	extern	_CLC2GLS2bits
	extern	_CLC2GLS3bits
	extern	_CLC3CONbits
	extern	_CLC3POLbits
	extern	_CLC3SEL0bits
	extern	_CLC3SEL1bits
	extern	_CLC3SEL2bits
	extern	_CLC3SEL3bits
	extern	_CLC3GLS0bits
	extern	_CLC3GLS1bits
	extern	_CLC3GLS2bits
	extern	_CLC3GLS3bits
	extern	_ICDBK0Hbits
	extern	_STATUS_SHADbits
	extern	_INDF0
	extern	_INDF1
	extern	_PCL
	extern	_STATUS
	extern	_FSR0
	extern	_FSR0L
	extern	_FSR0H
	extern	_FSR1
	extern	_FSR1L
	extern	_FSR1H
	extern	_BSR
	extern	_WREG
	extern	_PCLATH
	extern	_INTCON
	extern	_PORTA
	extern	_PORTB
	extern	_PORTC
	extern	_PIR1
	extern	_PIR2
	extern	_PIR3
	extern	_TMR0
	extern	_TMR1
	extern	_TMR1L
	extern	_TMR1H
	extern	_T1CON
	extern	_T1GCON
	extern	_TMR2
	extern	_PR2
	extern	_T2CON
	extern	_TRISA
	extern	_TRISB
	extern	_TRISC
	extern	_PIE1
	extern	_PIE2
	extern	_PIE3
	extern	_OPTION_REG
	extern	_PCON
	extern	_WDTCON
	extern	_OSCTUNE
	extern	_OSCCON
	extern	_OSCSTAT
	extern	_ADRES
	extern	_ADRESL
	extern	_ADRESH
	extern	_ADCON0
	extern	_ADCON1
	extern	_ADCON2
	extern	_LATA
	extern	_LATB
	extern	_LATC
	extern	_CM1CON0
	extern	_CM1CON1
	extern	_CM2CON0
	extern	_CM2CON1
	extern	_CMOUT
	extern	_BORCON
	extern	_FVRCON
	extern	_DAC1CON0
	extern	_DAC1CON1
	extern	_ZCD1CON
	extern	_ANSELA
	extern	_ANSELB
	extern	_ANSELC
	extern	_PMADR
	extern	_PMADRL
	extern	_PMADRH
	extern	_PMDAT
	extern	_PMDATL
	extern	_PMDATH
	extern	_PMCON1
	extern	_PMCON2
	extern	_VREGCON
	extern	_RC1REG
	extern	_RCREG
	extern	_RCREG1
	extern	_TX1REG
	extern	_TXREG
	extern	_TXREG1
	extern	_SP1BRG
	extern	_SP1BRGL
	extern	_SPBRG
	extern	_SPBRG1
	extern	_SPBRGL
	extern	_SP1BRGH
	extern	_SPBRGH
	extern	_SPBRGH1
	extern	_RC1STA
	extern	_RCSTA
	extern	_RCSTA1
	extern	_TX1STA
	extern	_TXSTA
	extern	_TXSTA1
	extern	_BAUD1CON
	extern	_BAUDCON
	extern	_BAUDCON1
	extern	_BAUDCTL
	extern	_BAUDCTL1
	extern	_WPUA
	extern	_WPUB
	extern	_WPUC
	extern	_SSP1BUF
	extern	_SSPBUF
	extern	_SSP1ADD
	extern	_SSPADD
	extern	_SSP1MSK
	extern	_SSPMSK
	extern	_SSP1STAT
	extern	_SSPSTAT
	extern	_SSP1CON
	extern	_SSP1CON1
	extern	_SSPCON
	extern	_SSPCON1
	extern	_SSP1CON2
	extern	_SSPCON2
	extern	_SSP1CON3
	extern	_SSPCON3
	extern	_ODCONA
	extern	_ODCONB
	extern	_ODCONC
	extern	_CCPR1
	extern	_CCPR1L
	extern	_CCPR1H
	extern	_CCP1CON
	extern	_ECCP1CON
	extern	_CCPR2
	extern	_CCPR2L
	extern	_CCPR2H
	extern	_CCP2CON
	extern	_ECCP2CON
	extern	_CCPTMRS
	extern	_SLRCONA
	extern	_SLRCONB
	extern	_SLRCONC
	extern	_INLVLA
	extern	_INLVLB
	extern	_INLVLC
	extern	_IOCAP
	extern	_IOCAN
	extern	_IOCAF
	extern	_IOCBP
	extern	_IOCBN
	extern	_IOCBF
	extern	_IOCCP
	extern	_IOCCN
	extern	_IOCCF
	extern	_TMR4
	extern	_PR4
	extern	_T4CON
	extern	_TMR6
	extern	_PR6
	extern	_T6CON
	extern	_OPA1CON
	extern	_OPA2CON
	extern	_PWM3DCL
	extern	_PWM3DCH
	extern	_PWM3CON
	extern	_PWM3CON0
	extern	_PWM4DCL
	extern	_PWM4DCH
	extern	_PWM4CON
	extern	_PWM4CON0
	extern	_COG1PHR
	extern	_COG1PHF
	extern	_COG1BLKR
	extern	_COG1BLKF
	extern	_COG1DBR
	extern	_COG1DBF
	extern	_COG1CON0
	extern	_COG1CON1
	extern	_COG1RIS
	extern	_COG1RSIM
	extern	_COG1FIS
	extern	_COG1FSIM
	extern	_COG1ASD0
	extern	_COG1ASD1
	extern	_COG1STR
	extern	_PPSLOCK
	extern	_INTPPS
	extern	_T0CKIPPS
	extern	_T1CKIPPS
	extern	_T1GPPS
	extern	_CCP1PPS
	extern	_CCP2PPS
	extern	_COGINPPS
	extern	_SSPCLKPPS
	extern	_SSPDATPPS
	extern	_SSPSSPPS
	extern	_RXPPS
	extern	_CKPPS
	extern	_CLCIN0PPS
	extern	_CLCIN1PPS
	extern	_CLCIN2PPS
	extern	_CLCIN3PPS
	extern	_RA0PPS
	extern	_RA1PPS
	extern	_RA2PPS
	extern	_RA4PPS
	extern	_RA5PPS
	extern	_RB4PPS
	extern	_RB5PPS
	extern	_RB6PPS
	extern	_RB7PPS
	extern	_RC0PPS
	extern	_RC1PPS
	extern	_RC2PPS
	extern	_RC3PPS
	extern	_RC4PPS
	extern	_RC5PPS
	extern	_RC6PPS
	extern	_RC7PPS
	extern	_CLCDATA
	extern	_CLC1CON
	extern	_CLC1POL
	extern	_CLC1SEL0
	extern	_CLC1SEL1
	extern	_CLC1SEL2
	extern	_CLC1SEL3
	extern	_CLC1GLS0
	extern	_CLC1GLS1
	extern	_CLC1GLS2
	extern	_CLC1GLS3
	extern	_CLC2CON
	extern	_CLC2POL
	extern	_CLC2SEL0
	extern	_CLC2SEL1
	extern	_CLC2SEL2
	extern	_CLC2SEL3
	extern	_CLC2GLS0
	extern	_CLC2GLS1
	extern	_CLC2GLS2
	extern	_CLC2GLS3
	extern	_CLC3CON
	extern	_CLC3POL
	extern	_CLC3SEL0
	extern	_CLC3SEL1
	extern	_CLC3SEL2
	extern	_CLC3SEL3
	extern	_CLC3GLS0
	extern	_CLC3GLS1
	extern	_CLC3GLS2
	extern	_CLC3GLS3
	extern	_ICDBK0H
	extern	_STATUS_SHAD
	extern	_WREG_SHAD
	extern	_BSR_SHAD
	extern	_PCLATH_SHAD
	extern	_FSR0L_SHAD
	extern	_FSR0H_SHAD
	extern	_FSR1L_SHAD
	extern	_FSR1H_SHAD
	extern	_STKPTR
	extern	_TOSL
	extern	_TOSH
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_delay_ms
	global	_isr
	global	_pps_init
	global	_system_init
	global	_i2c_init
	global	_ad_init
	global	_uart_init
	global	_timer0_init
	global	_timer1_init
	global	_timer2_init
	global	_clc_init
	global	_cog_init
	global	_ad
	global	_nibble_to_hex
	global	_main
	global	_tx_fifo
	global	_rx_fifo
	global	_timer0_counter
	global	_adc_poti
	global	_adc_sense
	global	_pulse_high
	global	_pulse_low

	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0070
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
UD_main_0	udata
_tx_fifo	res	34

UD_main_1	udata
_rx_fifo	res	34

UD_main_2	udata
_timer0_counter	res	1

UD_main_3	udata
_adc_poti	res	2

UD_main_4	udata
_adc_sense	res	2

;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_main_0	udata
r0x105C	res	1
r0x105D	res	1
r0x105E	res	1
r0x105F	res	1
r0x104E	res	1
r0x104D	res	1
r0x104F	res	1
r0x1050	res	1
r0x1051	res	1
r0x1052	res	1
r0x1058	res	1
r0x1059	res	1
r0x105A	res	1
r0x1053	res	1
r0x1054	res	1
r0x1056	res	1
r0x1055	res	1
r0x1057	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

ID_main_0	code
___str_0
	retlw 0x42 ; 'B'
	retlw 0x6f ; 'o'
	retlw 0x6f ; 'o'
	retlw 0x73 ; 's'
	retlw 0x74 ; 't'
	retlw 0x65 ; 'e'
	retlw 0x72 ; 'r'
	retlw 0x20 ; ' '
	retlw 0x4d ; 'M'
	retlw 0x61 ; 'a'
	retlw 0x78 ; 'x'
	retlw 0x3d ; '='
	retlw 0x38 ; '8'
	retlw 0x2e ; '.'
	retlw 0x30 ; '0'
	retlw 0x41 ; 'A'
	retlw 0x00 ; '.'

ID_main_1	code
___str_1
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x4f ; 'O'
	retlw 0x6e ; 'n'
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x20 ; ' '
	retlw 0x30 ; '0'
	retlw 0x2e ; '.'
	retlw 0x30 ; '0'
	retlw 0x25 ; '%'
	retlw 0x00 ; '.'

ID_main_2	idata	0x0078
_pulse_high
	db	0x19, 0x00


ID_main_3	idata	0x007A
_pulse_low
	db	0x4b, 0x00

;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; interrupt and initialization code
;--------------------------------------------------------
c_interrupt	code	0x0004
__sdcc_interrupt:
;***
;  pBlock Stats: dbName = I
;***
;4 compiler assigned registers:
;   r0x105C
;   r0x105D
;   r0x105E
;   r0x105F
;; Starting pCode block
_isr:
; 0 exit points
;	.line	35; "main.c"	void isr(void) __interrupt(0) {
	CLRF	PCLATH
;	.line	36; "main.c"	if (CCP1IF) {
	BANKSEL	_PIR1bits
	BTFSS	_PIR1bits,2
	GOTO	_00109_DS_
;	.line	37; "main.c"	CCP1IF = 0;
	BCF	_PIR1bits,2
;	.line	38; "main.c"	if (CCP1M0) {
	BANKSEL	_CCP1CONbits
	BTFSS	_CCP1CONbits,0
	GOTO	_00106_DS_
;	.line	39; "main.c"	CCP1M0 = 0;
	BCF	_CCP1CONbits,0
;	.line	40; "main.c"	CCPR1 += pulse_high;
	BANKSEL	_pulse_high
	MOVF	_pulse_high,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	BANKSEL	_CCPR1
	ADDWF	_CCPR1,F
	GOTO	_00109_DS_
_00106_DS_:
;	.line	42; "main.c"	CCP1M0 = 1;
	BANKSEL	_CCP1CONbits
	BSF	_CCP1CONbits,0
;	.line	43; "main.c"	CCPR1 += pulse_low;
	BANKSEL	_pulse_low
	MOVF	_pulse_low,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	BANKSEL	_CCPR1
	ADDWF	_CCPR1,F
_00109_DS_:
;	.line	46; "main.c"	if (TMR0IF && TMR0IE) {
	BANKSEL	_INTCONbits
	BTFSS	_INTCONbits,2
	GOTO	END_OF_INTERRUPT
	BTFSS	_INTCONbits,5
	GOTO	END_OF_INTERRUPT
;	.line	47; "main.c"	TMR0IF = 0;
	BCF	_INTCONbits,2
;	.line	48; "main.c"	TMR0 = TIMER0_VAL;
	MOVLW	0x06
	MOVWF	_TMR0
;	.line	49; "main.c"	LATC4 ^= 1;
	BANKSEL	r0x105C
	CLRF	r0x105C
	BANKSEL	_LATCbits
	BTFSS	_LATCbits,4
	GOTO	_00004_DS_
	BANKSEL	r0x105C
	INCF	r0x105C,F
_00004_DS_:
	BANKSEL	r0x105C
	MOVF	r0x105C,W
	MOVWF	r0x105D
	CLRF	r0x105E
	MOVLW	0x01
	XORWF	r0x105D,W
	MOVWF	r0x105D
	MOVWF	r0x105C
	MOVLW	0x00
	MOVWF	r0x105F
;;103	MOVF	r0x105C,W
	RRF	r0x105D,W
	BTFSC	STATUS,0
	GOTO	_00005_DS_
	BANKSEL	_LATCbits
	BCF	_LATCbits,4
_00005_DS_:
	BTFSS	STATUS,0
	GOTO	_00006_DS_
	BANKSEL	_LATCbits
	BSF	_LATCbits,4
_00006_DS_:
;	.line	51; "main.c"	timer0_counter++;
	BANKSEL	_timer0_counter
	INCF	_timer0_counter,F
;	.line	54; "main.c"	if (timer0_counter & 0x02) {
	MOVF	_timer0_counter,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	CLRF	r0x105D
	BTFSS	r0x105C,1
	GOTO	_00121_DS_
;	.line	55; "main.c"	if (timer0_counter & 0x01) {
	BANKSEL	_timer0_counter
	MOVF	_timer0_counter,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	CLRF	r0x105D
	BTFSS	r0x105C,0
	GOTO	_00113_DS_
;	.line	56; "main.c"	ADCON0 = (AD_POTI << 2) | 1;
	MOVLW	0x09
	BANKSEL	_ADCON0
	MOVWF	_ADCON0
;	.line	57; "main.c"	ADGO = 1;
	BSF	_ADCON0bits,1
	GOTO	END_OF_INTERRUPT
_00113_DS_:
;	.line	59; "main.c"	} else if (!ADGO) {
	BANKSEL	_ADCON0bits
	BTFSC	_ADCON0bits,1
	GOTO	END_OF_INTERRUPT
;	.line	61; "main.c"	adc_poti += ADRESL;
	MOVF	_ADRESL,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	CLRF	r0x105D
;;101	MOVF	r0x105C,W
	MOVLW	0x00
	MOVWF	r0x105F
;;100	MOVF	r0x105E,W
	MOVF	r0x105C,W
	MOVWF	r0x105E
	BANKSEL	_adc_poti
	ADDWF	_adc_poti,F
	BANKSEL	r0x105F
	MOVF	r0x105F,W
	BTFSC	STATUS,0
	INCF	r0x105F,W
	BTFSC	STATUS,2
	GOTO	_00007_DS_
	BANKSEL	_adc_poti
	ADDWF	(_adc_poti + 1),F
;;shiftRight_Left2ResultLit:5323: shCount=1, size=2, sign=0, same=1, offr=0
_00007_DS_:
;	.line	62; "main.c"	adc_poti >>= 1;
	BANKSEL	_adc_poti
	LSRF	(_adc_poti + 1),F
	RRF	_adc_poti,F
	GOTO	END_OF_INTERRUPT
_00121_DS_:
;	.line	65; "main.c"	if (timer0_counter & 0x01) {
	BANKSEL	_timer0_counter
	MOVF	_timer0_counter,W
	BANKSEL	r0x105C
	MOVWF	r0x105C
	CLRF	r0x105D
	BTFSS	r0x105C,0
	GOTO	_00118_DS_
;	.line	66; "main.c"	ADCON0 = (AD_SENSE << 2) | 1;
	MOVLW	0x11
	BANKSEL	_ADCON0
	MOVWF	_ADCON0
;	.line	67; "main.c"	ADGO = 1;
	BSF	_ADCON0bits,1
	GOTO	END_OF_INTERRUPT
_00118_DS_:
;	.line	69; "main.c"	} else if (!ADGO) {
	BANKSEL	_ADCON0bits
	BTFSC	_ADCON0bits,1
	GOTO	END_OF_INTERRUPT
;	.line	71; "main.c"	adc_sense += ADRESL;
	MOVF	_ADRESL,W
	BANKSEL	r0x105E
	MOVWF	r0x105E
	MOVWF	r0x105C
	CLRF	r0x105D
;;99	MOVF	r0x105C,W
	MOVLW	0x00
	MOVWF	r0x105F
	MOVF	r0x105E,W
	BANKSEL	_adc_sense
	ADDWF	_adc_sense,F
	BANKSEL	r0x105F
	MOVF	r0x105F,W
	BTFSC	STATUS,0
	INCF	r0x105F,W
	BTFSC	STATUS,2
	GOTO	_00008_DS_
	BANKSEL	_adc_sense
	ADDWF	(_adc_sense + 1),F
;;shiftRight_Left2ResultLit:5323: shCount=1, size=2, sign=0, same=1, offr=0
_00008_DS_:
;	.line	72; "main.c"	adc_sense >>= 1;
	BANKSEL	_adc_sense
	LSRF	(_adc_sense + 1),F
	RRF	_adc_sense,F
END_OF_INTERRUPT:
;	.line	76; "main.c"	}
	RETFIE	

;--------------------------------------------------------
; code
;--------------------------------------------------------
code_main	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _pps_init
;   _system_init
;   _uart_init
;   _i2c_init
;   _ad_init
;   _timer0_init
;   _cog_init
;   _timer1_init
;   _LCD_init
;   _LCD_putcmd
;   _LCD_puts
;   _LCD_goto
;   _nibble_to_hex
;   _LCD_putch
;   _nibble_to_hex
;   _LCD_putch
;   _LCD_puts
;   _putchar_wait
;   _delay_ms
;   _pps_init
;   _system_init
;   _uart_init
;   _i2c_init
;   _ad_init
;   _timer0_init
;   _cog_init
;   _timer1_init
;   _LCD_init
;   _LCD_putcmd
;   _LCD_puts
;   _LCD_goto
;   _nibble_to_hex
;   _LCD_putch
;   _nibble_to_hex
;   _LCD_putch
;   _LCD_puts
;   _putchar_wait
;   _delay_ms
;8 compiler assigned registers:
;   r0x1053
;   r0x1054
;   STK01
;   STK00
;   r0x1055
;   r0x1056
;   r0x1057
;   STK02
;; Starting pCode block
S_main__main	code
_main:
; 2 exit points
;	.line	307; "main.c"	pps_init();
	PAGESEL	_pps_init
	CALL	_pps_init
	PAGESEL	$
;	.line	308; "main.c"	system_init();
	PAGESEL	_system_init
	CALL	_system_init
	PAGESEL	$
;	.line	309; "main.c"	uart_init();
	PAGESEL	_uart_init
	CALL	_uart_init
	PAGESEL	$
;	.line	310; "main.c"	i2c_init();
	PAGESEL	_i2c_init
	CALL	_i2c_init
	PAGESEL	$
;	.line	311; "main.c"	ad_init();
	PAGESEL	_ad_init
	CALL	_ad_init
	PAGESEL	$
;	.line	312; "main.c"	timer0_init();
	PAGESEL	_timer0_init
	CALL	_timer0_init
	PAGESEL	$
;	.line	313; "main.c"	cog_init();
	PAGESEL	_cog_init
	CALL	_cog_init
	PAGESEL	$
;	.line	314; "main.c"	timer1_init();
	PAGESEL	_timer1_init
	CALL	_timer1_init
	PAGESEL	$
;;gen.c:6571: size=0, offset=0, AOP_TYPE(res)=8
;	.line	317; "main.c"	tx_fifo.head = 0;
	BANKSEL	_tx_fifo
	CLRF	(_tx_fifo + 0)
;;gen.c:6571: size=0, offset=0, AOP_TYPE(res)=8
;	.line	318; "main.c"	tx_fifo.tail = 0;
	CLRF	(_tx_fifo + 1)
;;gen.c:6571: size=0, offset=0, AOP_TYPE(res)=8
;	.line	319; "main.c"	rx_fifo.head = 0;
	BANKSEL	_rx_fifo
	CLRF	(_rx_fifo + 0)
;;gen.c:6571: size=0, offset=0, AOP_TYPE(res)=8
;	.line	320; "main.c"	rx_fifo.tail = 0;
	CLRF	(_rx_fifo + 1)
;	.line	322; "main.c"	LATB4 = 1;		/* enable	*/
	BANKSEL	_LATBbits
	BSF	_LATBbits,4
;	.line	323; "main.c"	GIE = 1;
	BANKSEL	_INTCONbits
	BSF	_INTCONbits,7
;	.line	324; "main.c"	LCD_init(LCD_01_ADDRESS);
	MOVLW	0x27
	PAGESEL	_LCD_init
	CALL	_LCD_init
	PAGESEL	$
;	.line	326; "main.c"	while (1) {
	BANKSEL	r0x1053
	CLRF	r0x1053
_00197_DS_:
;	.line	327; "main.c"	if (counter == 0) {
	MOVLW	0x00
	BANKSEL	r0x1053
	IORWF	r0x1053,W
	BTFSS	STATUS,2
	GOTO	_00003_DS_
;	.line	330; "main.c"	GIE = 0;
	BANKSEL	_INTCONbits
	BCF	_INTCONbits,7
;	.line	332; "main.c"	temp = adc_poti;
	BANKSEL	_adc_poti
	MOVF	_adc_poti,W
	BANKSEL	r0x1054
	MOVWF	r0x1054
;	.line	333; "main.c"	GIE = 1;
	BANKSEL	_INTCONbits
	BSF	_INTCONbits,7
;	.line	334; "main.c"	LCD_putcmd(LCD_01_ADDRESS, LCD_CLEAR, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x01
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	335; "main.c"	LCD_puts(LCD_01_ADDRESS, "Booster Max=8.0A\0");
	MOVLW	high (___str_0 + 0)
	BANKSEL	r0x1055
	MOVWF	r0x1055
	MOVLW	(___str_0 + 0)
	MOVWF	r0x1056
	MOVLW	0x80
	MOVWF	r0x1057
	MOVF	r0x1056,W
	MOVWF	STK02
	MOVF	r0x1055,W
	MOVWF	STK01
	MOVF	r0x1057,W
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_puts
	CALL	_LCD_puts
	PAGESEL	$
;	.line	336; "main.c"	LCD_goto(LCD_01_ADDRESS, 2, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x02
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_goto
	CALL	_LCD_goto
	PAGESEL	$
;	.line	337; "main.c"	LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp >> 4));
	BANKSEL	r0x1054
	SWAPF	r0x1054,W
	ANDLW	0x0f
	MOVWF	r0x1056
	PAGESEL	_nibble_to_hex
	CALL	_nibble_to_hex
	PAGESEL	$
	BANKSEL	r0x1056
	MOVWF	r0x1056
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_putch
	CALL	_LCD_putch
	PAGESEL	$
;	.line	338; "main.c"	LCD_putch(LCD_01_ADDRESS, nibble_to_hex(temp));
	BANKSEL	r0x1054
	MOVF	r0x1054,W
	PAGESEL	_nibble_to_hex
	CALL	_nibble_to_hex
	PAGESEL	$
	BANKSEL	r0x1054
	MOVWF	r0x1054
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_putch
	CALL	_LCD_putch
	PAGESEL	$
;	.line	339; "main.c"	LCD_puts(LCD_01_ADDRESS, "  On      0.0%\0");
	MOVLW	high (___str_1 + 0)
	BANKSEL	r0x1056
	MOVWF	r0x1056
	MOVLW	(___str_1 + 0)
	MOVWF	r0x1054
	MOVLW	0x80
	MOVWF	r0x1055
	MOVF	r0x1054,W
	MOVWF	STK02
	MOVF	r0x1056,W
	MOVWF	STK01
	MOVF	r0x1055,W
	MOVWF	STK00
	MOVLW	0x27
	PAGESEL	_LCD_puts
	CALL	_LCD_puts
	PAGESEL	$
;	.line	342; "main.c"	putchar_wait(0x55);
	MOVLW	0x55
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	344; "main.c"	LATC5 ^= 1;
	BANKSEL	r0x1054
	CLRF	r0x1054
	BANKSEL	_LATCbits
	BTFSS	_LATCbits,5
	GOTO	_00001_DS_
	BANKSEL	r0x1054
	INCF	r0x1054,F
_00001_DS_:
	BANKSEL	r0x1054
	MOVF	r0x1054,W
	MOVWF	r0x1056
	CLRF	r0x1055
	MOVLW	0x01
	XORWF	r0x1056,W
	MOVWF	r0x1056
	MOVWF	r0x1054
	MOVLW	0x00
	MOVWF	r0x1057
;;102	MOVF	r0x1054,W
	RRF	r0x1056,W
	BTFSC	STATUS,0
	GOTO	_00002_DS_
	BANKSEL	_LATCbits
	BCF	_LATCbits,5
_00002_DS_:
	BTFSS	STATUS,0
	GOTO	_00003_DS_
	BANKSEL	_LATCbits
	BSF	_LATCbits,5
_00003_DS_:
;	.line	346; "main.c"	delay_ms(4);
	MOVLW	0x04
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay_ms
	CALL	_delay_ms
	PAGESEL	$
;	.line	348; "main.c"	counter++;
	BANKSEL	r0x1053
	INCF	r0x1053,F
	GOTO	_00197_DS_
;	.line	350; "main.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;2 compiler assigned registers:
;   r0x104D
;   r0x104E
;; Starting pCode block
S_main__nibble_to_hex	code
_nibble_to_hex:
; 2 exit points
;	.line	294; "main.c"	char nibble_to_hex(uint8_t c) {
	BANKSEL	r0x104D
	MOVWF	r0x104D
;	.line	296; "main.c"	nibble = (c & 0x0f) + '0';
	MOVWF	r0x104E
	MOVLW	0x0f
	ANDWF	r0x104E,W
	MOVWF	r0x104D
	MOVLW	0x30
	ADDWF	r0x104D,F
;;unsigned compare: left < lit(0x3A=58), size=1
;	.line	297; "main.c"	if (nibble >= 0x3a)
	MOVLW	0x3a
	SUBWF	r0x104D,W
	BTFSS	STATUS,0
	GOTO	_00189_DS_
;;genSkipc:3257: created from rifx:0x7fffaa47e720
;	.line	298; "main.c"	nibble += 7;
	MOVF	r0x104D,W
	MOVWF	r0x104E
	MOVLW	0x07
	ADDWF	r0x104E,W
	MOVWF	r0x104D
_00189_DS_:
;	.line	299; "main.c"	return (nibble);
	BANKSEL	r0x104D
	MOVF	r0x104D,W
;	.line	300; "main.c"	}
	RETURN	
; exit point of _nibble_to_hex

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _delay_ms
;   _delay_ms
;5 compiler assigned registers:
;   r0x1058
;   r0x1059
;   r0x105A
;   r0x105B
;   STK00
;; Starting pCode block
S_main__ad	code
_ad:
; 2 exit points
;	.line	286; "main.c"	uint8_t ad(uint8_t channel) {
	BANKSEL	r0x1058
	MOVWF	r0x1058
;	.line	287; "main.c"	ADCON0 = (channel << 2) | 1;
	MOVWF	r0x1059
	LSLF	r0x1059,W
	MOVWF	r0x1058
	LSLF	r0x1058,F
	MOVF	r0x1058,W
	MOVWF	r0x1059
	CLRF	r0x105A
	BTFSC	r0x1059,7
	DECF	r0x105A,F
	MOVLW	0x01
	IORWF	r0x1059,W
	MOVWF	r0x1058
	MOVF	r0x105A,W
;;1	MOVWF	r0x105B
	MOVF	r0x1058,W
	BANKSEL	_ADCON0
	MOVWF	_ADCON0
;	.line	288; "main.c"	ADGO = 1;
	BSF	_ADCON0bits,1
;	.line	289; "main.c"	delay_ms(1);
	MOVLW	0x01
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay_ms
	CALL	_delay_ms
	PAGESEL	$
_00183_DS_:
;	.line	290; "main.c"	while (ADGO) ;
	BANKSEL	_ADCON0bits
	BTFSC	_ADCON0bits,1
	GOTO	_00183_DS_
;	.line	291; "main.c"	return (ADRESH);
	MOVF	_ADRESH,W
;	.line	292; "main.c"	}
	RETURN	
; exit point of _ad

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;7 compiler assigned registers:
;   r0x104D
;   STK00
;   r0x104E
;   r0x104F
;   r0x1050
;   r0x1051
;   r0x1052
;; Starting pCode block
S_main__delay_ms	code
_delay_ms:
; 2 exit points
;	.line	275; "main.c"	void delay_ms(uint16_t ms) {
	BANKSEL	r0x104D
	MOVWF	r0x104D
	MOVF	STK00,W
	MOVWF	r0x104E
_00172_DS_:
;	.line	277; "main.c"	while (ms--) {
	BANKSEL	r0x104E
	MOVF	r0x104E,W
	MOVWF	r0x104F
	MOVF	r0x104D,W
	MOVWF	r0x1050
	MOVLW	0xff
	ADDWF	r0x104E,F
	BTFSS	STATUS,0
	DECF	r0x104D,F
	MOVF	r0x104F,W
	IORWF	r0x1050,W
	BTFSC	STATUS,2
	GOTO	_00178_DS_
;	.line	279; "main.c"	for (u = 0; u < LOOPS_PER_MS; u++) {
	MOVLW	0x90
	MOVWF	r0x104F
	MOVLW	0x01
	MOVWF	r0x1050
_00177_DS_:
	nop	
	MOVLW	0xff
	BANKSEL	r0x104F
	ADDWF	r0x104F,W
	MOVWF	r0x1051
	MOVLW	0xff
	MOVWF	r0x1052
	MOVF	r0x1050,W
	BTFSC	STATUS,0
	INCFSZ	r0x1050,W
	ADDWF	r0x1052,F
	MOVF	r0x1051,W
	MOVWF	r0x104F
	MOVF	r0x1052,W
	MOVWF	r0x1050
;	.line	279; "main.c"	for (u = 0; u < LOOPS_PER_MS; u++) {
	MOVF	r0x1051,W
	IORWF	r0x1052,W
	BTFSS	STATUS,2
	GOTO	_00177_DS_
	GOTO	_00172_DS_
_00178_DS_:
;	.line	284; "main.c"	}
	RETURN	
; exit point of _delay_ms

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__cog_init	code
_cog_init:
; 2 exit points
;	.line	244; "main.c"	COG1CON0 = 0;
	BANKSEL	_COG1CON0
	CLRF	_COG1CON0
;	.line	245; "main.c"	COG1CON1 = 0;
	CLRF	_COG1CON1
;	.line	247; "main.c"	COG1RIS = 0b00001000;	/* CCP1 rising event	*/
	MOVLW	0x08
	MOVWF	_COG1RIS
;	.line	248; "main.c"	COG1RSIM = 0x0;		/* no rising delay	*/
	CLRF	_COG1RSIM
;	.line	249; "main.c"	COG1FIS = 0b00001000;	/* CCP1 falling event	*/
	MOVLW	0x08
	MOVWF	_COG1FIS
;	.line	250; "main.c"	COG1FSIM = 0x0;		/* no falling delay	*/
	CLRF	_COG1FSIM
;	.line	252; "main.c"	COG1ASD0 = 0;	/* don't use shutdown		*/
	CLRF	_COG1ASD0
;	.line	253; "main.c"	COG1ASD1 = 0;
	CLRF	_COG1ASD1
;	.line	255; "main.c"	COG1STR = 0;	/* don't use steering control	*/
	CLRF	_COG1STR
;	.line	256; "main.c"	COG1DBR = 0;	/* no dead band			*/
	CLRF	_COG1DBR
;	.line	257; "main.c"	COG1DBF = 0;
	CLRF	_COG1DBF
;	.line	258; "main.c"	COG1BLKR = 0;	/* don't use blanking		*/
	CLRF	_COG1BLKR
;	.line	259; "main.c"	COG1BLKF = 0;
	CLRF	_COG1BLKF
;	.line	260; "main.c"	COG1PHR = 0;	/* normal phase			*/
	CLRF	_COG1PHR
;	.line	261; "main.c"	COG1PHF = 0;
	CLRF	_COG1PHF
;	.line	263; "main.c"	COG1CON0 = 0b10001100;
	MOVLW	0x8c
	MOVWF	_COG1CON0
;	.line	269; "main.c"	}
	RETURN	
; exit point of _cog_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__clc_init	code
_clc_init:
; 2 exit points
;	.line	236; "main.c"	CLC1SEL0 = 0;
	BANKSEL	_CLC1SEL0
	CLRF	_CLC1SEL0
;	.line	237; "main.c"	CLC1SEL1 = 0;
	CLRF	_CLC1SEL1
;	.line	241; "main.c"	}
	RETURN	
; exit point of _clc_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__timer2_init	code
_timer2_init:
; 2 exit points
;	.line	229; "main.c"	T2CON = 0b00000100;
	MOVLW	0x04
	BANKSEL	_T2CON
	MOVWF	_T2CON
;	.line	232; "main.c"	TMR2 = 0;			// reset timer2
	CLRF	_TMR2
;	.line	233; "main.c"	}
	RETURN	
; exit point of _timer2_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__timer1_init	code
_timer1_init:
; 2 exit points
;	.line	211; "main.c"	T1CON = 0b00110001;
	MOVLW	0x31
	BANKSEL	_T1CON
	MOVWF	_T1CON
;	.line	215; "main.c"	TMR1GE = 0;			// timer is not controlled by gate.
	BCF	_T1GCONbits,7
;	.line	216; "main.c"	TMR1H = 0;			// reset timer1 high
	CLRF	_TMR1H
;	.line	217; "main.c"	TMR1L = 0;			// and low bytes - prescaler automatic reset
	CLRF	_TMR1L
;	.line	218; "main.c"	CCP1CON = 0b00001000;	// set up capture and compare
	MOVLW	0x08
	BANKSEL	_CCP1CON
	MOVWF	_CCP1CON
;	.line	221; "main.c"	CCPR1H = 0xFF;
	MOVLW	0xff
	MOVWF	_CCPR1H
;	.line	222; "main.c"	CCPR1L = 0xFF;
	MOVLW	0xff
	MOVWF	_CCPR1L
;	.line	224; "main.c"	CCP1IE = 1;
	BANKSEL	_PIE1bits
	BSF	_PIE1bits,2
;	.line	225; "main.c"	}
	RETURN	
; exit point of _timer1_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__timer0_init	code
_timer0_init:
; 2 exit points
;	.line	203; "main.c"	TMR0CS = 0;			// FOSC / 4
	BANKSEL	_OPTION_REGbits
	BCF	_OPTION_REGbits,5
;	.line	204; "main.c"	PSA = 0;			// use prescaler
	BCF	_OPTION_REGbits,3
;	.line	205; "main.c"	PS1 = 1;			// prescaler 1:8
	BSF	_OPTION_REGbits,1
;	.line	206; "main.c"	TMR0 = TIMER0_VAL;
	MOVLW	0x06
	BANKSEL	_TMR0
	MOVWF	_TMR0
;	.line	207; "main.c"	TMR0IE = 1;
	BSF	_INTCONbits,5
;	.line	208; "main.c"	}
	RETURN	
; exit point of _timer0_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__uart_init	code
_uart_init:
; 2 exit points
;	.line	187; "main.c"	TX9 = 1;			// 8-bit transmission
	BANKSEL	_TX1STAbits
	BSF	_TX1STAbits,6
;	.line	188; "main.c"	TX9D = 1;			//  one extra stop bit
	BSF	_TX1STAbits,0
;	.line	189; "main.c"	TXEN = 1;			// transmit enabled
	BSF	_TX1STAbits,5
;	.line	190; "main.c"	SYNC = 0;			// asynchronous mode
	BCF	_TX1STAbits,4
;	.line	191; "main.c"	BRGH = 1;			// high speed
	BSF	_TX1STAbits,2
;	.line	192; "main.c"	SPEN = 1;			// enable serial port (configures RX/DT and TX/CK pins as serial port pins)
	BSF	_RC1STAbits,7
;	.line	193; "main.c"	RX9 = 0;			// 8-bit reception
	BCF	_RC1STAbits,6
;	.line	194; "main.c"	CREN = 1;			// enable receiver
	BSF	_RC1STAbits,4
;	.line	195; "main.c"	BRG16 = USE_BRG16;		// 8-bit baud rate generator
	BCF	_BAUD1CONbits,3
;	.line	197; "main.c"	SPBRG = SBRG_VAL;		// calculated by defines
	MOVLW	0x13
	MOVWF	_SPBRG
;	.line	199; "main.c"	RCIF = 0;
	BANKSEL	_PIR1bits
	BCF	_PIR1bits,5
;	.line	200; "main.c"	}
	RETURN	
; exit point of _uart_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__ad_init	code
_ad_init:
; 2 exit points
;	.line	180; "main.c"	ANSELA = 1 << 2;
	MOVLW	0x04
	BANKSEL	_ANSELA
	MOVWF	_ANSELA
;	.line	181; "main.c"	ANSELC = 1 << 0;
	MOVLW	0x01
	MOVWF	_ANSELC
;	.line	183; "main.c"	ADCON1 = 0b11100000;
	MOVLW	0xe0
	BANKSEL	_ADCON1
	MOVWF	_ADCON1
;	.line	184; "main.c"	}
	RETURN	
; exit point of _ad_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__i2c_init	code
_i2c_init:
; 2 exit points
;	.line	168; "main.c"	SSP1CON1 = 0b00101000;
	MOVLW	0x28
	BANKSEL	_SSP1CON1
	MOVWF	_SSP1CON1
;	.line	169; "main.c"	SSP1CON2 = 0x00;
	CLRF	_SSP1CON2
;	.line	170; "main.c"	SSP1CON3 = 0x00;
	CLRF	_SSP1CON3
;	.line	173; "main.c"	SSP1ADD = SSP1ADD_VAL;
	MOVLW	0x4f
	MOVWF	_SSP1ADD
;	.line	175; "main.c"	SSP1STAT = 0b11000000;
	MOVLW	0xc0
	MOVWF	_SSP1STAT
;	.line	176; "main.c"	}
	RETURN	
; exit point of _i2c_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__system_init	code
_system_init:
; 2 exit points
;	.line	123; "main.c"	OSCCON = 0b11110000;	// Configure oscillator
	MOVLW	0xf0
	BANKSEL	_OSCCON
	MOVWF	_OSCCON
;	.line	127; "main.c"	ANSELA = 0;
	BANKSEL	_ANSELA
	CLRF	_ANSELA
;	.line	128; "main.c"	ANSELB = 0;
	CLRF	_ANSELB
;	.line	129; "main.c"	ANSELC = 0;
	CLRF	_ANSELC
;	.line	130; "main.c"	ADCON0 = 0;
	BANKSEL	_ADCON0
	CLRF	_ADCON0
;	.line	131; "main.c"	ADCON1 = 0;
	CLRF	_ADCON1
;	.line	132; "main.c"	ADCON2 = 0;
	CLRF	_ADCON2
;	.line	133; "main.c"	CM1CON0 = 0;
	BANKSEL	_CM1CON0
	CLRF	_CM1CON0
;	.line	134; "main.c"	CM1CON1 = 0;
	CLRF	_CM1CON1
;	.line	135; "main.c"	CM2CON0 = 0;
	CLRF	_CM2CON0
;	.line	136; "main.c"	CM2CON1 = 0;
	CLRF	_CM2CON1
;	.line	139; "main.c"	TRISA0 = 0;
	BANKSEL	_TRISAbits
	BCF	_TRISAbits,0
;	.line	141; "main.c"	TRISA4 = 1;
	BSF	_TRISAbits,4
;	.line	142; "main.c"	TRISA5 = 1;
	BSF	_TRISAbits,5
;	.line	143; "main.c"	TRISC0 = 0;
	BCF	_TRISCbits,0
;	.line	145; "main.c"	TRISC1 = 1;
	BSF	_TRISCbits,1
;	.line	146; "main.c"	TRISC2 = 0;
	BCF	_TRISCbits,2
;	.line	148; "main.c"	TRISA2 = 1;
	BSF	_TRISAbits,2
;	.line	149; "main.c"	TRISB5 = 0;
	BCF	_TRISBbits,5
;	.line	150; "main.c"	TRISB6 = 0;
	BCF	_TRISBbits,6
;	.line	151; "main.c"	TRISC0 = 1;
	BSF	_TRISCbits,0
;	.line	152; "main.c"	TRISC3 = 0;	/***/	/* Rail Data */
	BCF	_TRISCbits,3
;	.line	153; "main.c"	TRISB4 = 0;		/* Enable */
	BCF	_TRISBbits,4
;	.line	154; "main.c"	TRISC5 = 0;		/* LED */
	BCF	_TRISCbits,5
;	.line	157; "main.c"	SSP1IF = 0;
	BANKSEL	_PIR1bits
	BCF	_PIR1bits,3
;	.line	158; "main.c"	TMR1IF = 0;
	BCF	_PIR1bits,0
;	.line	159; "main.c"	CCP1IF = 0;
	BCF	_PIR1bits,2
;	.line	161; "main.c"	CCP1IE = 1;			// disable interrupt on CCP1 will be check by polling as of today
	BANKSEL	_PIE1bits
	BSF	_PIE1bits,2
;	.line	162; "main.c"	PEIE = 1;
	BANKSEL	_INTCONbits
	BSF	_INTCONbits,6
;	.line	163; "main.c"	GIE = 1;
	BSF	_INTCONbits,7
;	.line	164; "main.c"	}
	RETURN	
; exit point of _system_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;; Starting pCode block
S_main__pps_init	code
_pps_init:
; 2 exit points
;	.line	96; "main.c"	PPSLOCK = 0x55;
	MOVLW	0x55
	BANKSEL	_PPSLOCK
	MOVWF	_PPSLOCK
;	.line	97; "main.c"	PPSLOCK = 0xaa;
	MOVLW	0xaa
	MOVWF	_PPSLOCK
;	.line	98; "main.c"	PPSLOCK = 0;		// unlock PPS
	CLRF	_PPSLOCK
;	.line	100; "main.c"	SSPCLKPPS = 0x05;
	MOVLW	0x05
	MOVWF	_SSPCLKPPS
;	.line	101; "main.c"	RA5PPS = 0b10000;		// RA5 output SCL
	MOVLW	0x10
	BANKSEL	_RA5PPS
	MOVWF	_RA5PPS
;	.line	102; "main.c"	SSPDATPPS = 0x04;
	MOVLW	0x04
	BANKSEL	_SSPDATPPS
	MOVWF	_SSPDATPPS
;	.line	103; "main.c"	RA4PPS = 0b10001;		// RA4 output SDA
	MOVLW	0x11
	BANKSEL	_RA4PPS
	MOVWF	_RA4PPS
;	.line	105; "main.c"	RXPPS = 0b10001;		// input  EUSART RX -> RC1
	MOVLW	0x11
	BANKSEL	_RXPPS
	MOVWF	_RXPPS
;	.line	106; "main.c"	RC2PPS = 0b10100;		// RC2 output TX/CK
	MOVLW	0x14
	BANKSEL	_RC2PPS
	MOVWF	_RC2PPS
;	.line	108; "main.c"	RC3PPS = 0b01100;		// CCP1
	MOVLW	0x0c
	MOVWF	_RC3PPS
;	.line	112; "main.c"	COGINPPS = 0b10011;		// RC3 PULSE
	MOVLW	0x13
	BANKSEL	_COGINPPS
	MOVWF	_COGINPPS
;	.line	113; "main.c"	RB5PPS = 0b01001;		// COG1B -> RPWM
	MOVLW	0x09
	BANKSEL	_RB5PPS
	MOVWF	_RB5PPS
;	.line	114; "main.c"	RB6PPS = 0b01000;		// COG1A -> LPWM
	MOVLW	0x08
	MOVWF	_RB6PPS
;	.line	116; "main.c"	PPSLOCK = 0x55;
	MOVLW	0x55
	BANKSEL	_PPSLOCK
	MOVWF	_PPSLOCK
;	.line	117; "main.c"	PPSLOCK = 0xaa;
	MOVLW	0xaa
	MOVWF	_PPSLOCK
;	.line	118; "main.c"	PPSLOCK = 1;		// lock PPS
	MOVLW	0x01
	MOVWF	_PPSLOCK
;	.line	119; "main.c"	}
	RETURN	
; exit point of _pps_init


;	code size estimation:
;	  433+  137 =   570 instructions ( 1414 byte)

	end
