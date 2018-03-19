;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #10289 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"i2c_lcd.c"
	list	p=16f1709
	radix dec
	include "p16f1709.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_delay_ms
	extern	_putchar
	extern	_putchar_wait
	extern	_puts_rom
	extern	_init_usart
	extern	_fifo_putchar
	extern	_print_rom_fifo
	extern	_print_debug_value
	extern	_print_debug_fifo
	extern	_i2c_start
	extern	_i2c_restart
	extern	_i2c_stop
	extern	_i2c_sendACK
	extern	_i2c_write
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
	extern	__gptrget1

	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_LCD_puts
	global	_LCD_goto
	global	_LCD_init
	global	_LCD_putch
	global	_LCD_putcmd
	global	_I2C_PCF8574_Write

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_i2c_lcd_0	udata
r0x1000	res	1
r0x1001	res	1
r0x1002	res	1
r0x100E	res	1
r0x100F	res	1
r0x1010	res	1
r0x1011	res	1
r0x1012	res	1
r0x1013	res	1
r0x1003	res	1
r0x1004	res	1
r0x1005	res	1
r0x1006	res	1
r0x1007	res	1
r0x1018	res	1
r0x1015	res	1
r0x1016	res	1
r0x1017	res	1
r0x1009	res	1
r0x100C	res	1
r0x100B	res	1
r0x100A	res	1
r0x100D	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_i2c_lcd	code
;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __gptrget1
;   _LCD_putch
;   __gptrget1
;   _LCD_putch
;8 compiler assigned registers:
;   r0x1009
;   STK00
;   r0x100A
;   STK01
;   r0x100B
;   STK02
;   r0x100C
;   r0x100D
;; Starting pCode block
S_i2c_lcd__LCD_puts	code
_LCD_puts:
; 2 exit points
;	.line	110; "i2c_lcd.c"	void LCD_puts(uint8_t addr, const char *s) {
	BANKSEL	r0x1009
	MOVWF	r0x1009
	MOVF	STK00,W
	MOVWF	r0x100A
	MOVF	STK01,W
	MOVWF	r0x100B
	MOVF	STK02,W
	MOVWF	r0x100C
_00148_DS_:
;	.line	112; "i2c_lcd.c"	while (*s != 0)
	BANKSEL	r0x100C
	MOVF	r0x100C,W
	MOVWF	STK01
	MOVF	r0x100B,W
	MOVWF	STK00
	MOVF	r0x100A,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x100D
	MOVWF	r0x100D
	MOVF	r0x100D,W
	BTFSC	STATUS,2
	GOTO	_00151_DS_
;	.line	113; "i2c_lcd.c"	LCD_putch(addr, *s++);
	INCF	r0x100C,F
	BTFSC	STATUS,2
	INCF	r0x100B,F
	BTFSC	STATUS,2
	INCF	r0x100A,F
	MOVF	r0x100D,W
	MOVWF	STK00
	MOVF	r0x1009,W
	PAGESEL	_LCD_putch
	CALL	_LCD_putch
	PAGESEL	$
	GOTO	_00148_DS_
_00151_DS_:
;	.line	114; "i2c_lcd.c"	}
	RETURN	
; exit point of _LCD_puts

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;5 compiler assigned registers:
;   r0x1015
;   STK00
;   r0x1016
;   STK01
;   r0x1017
;; Starting pCode block
S_i2c_lcd__LCD_goto	code
_LCD_goto:
; 2 exit points
;	.line	88; "i2c_lcd.c"	void LCD_goto(uint8_t addr, uint8_t row, uint8_t column) {
	BANKSEL	r0x1015
	MOVWF	r0x1015
	MOVF	STK00,W
	MOVWF	r0x1016
	MOVF	STK01,W
	MOVWF	r0x1017
;;unsigned compare: left < lit(0x1=1), size=1
;	.line	89; "i2c_lcd.c"	switch (row) {
	MOVLW	0x01
	SUBWF	r0x1016,W
	BTFSS	STATUS,0
	GOTO	_00129_DS_
;;genSkipc:3257: created from rifx:0x7ffe400e9200
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x5=5), size=1
	MOVLW	0x05
	SUBWF	r0x1016,W
	BTFSC	STATUS,0
	GOTO	_00129_DS_
;;genSkipc:3257: created from rifx:0x7ffe400e9200
	DECF	r0x1016,F
	MOVLW	HIGH(_00143_DS_)
	MOVWF	PCLATH
	MOVLW	_00143_DS_
	ADDWF	r0x1016,W
	BTFSC	STATUS,0
	INCF	PCLATH,F
	MOVWF	PCL
_00143_DS_:
	GOTO	_00125_DS_
	GOTO	_00126_DS_
	GOTO	_00127_DS_
	GOTO	_00128_DS_
_00125_DS_:
;	.line	91; "i2c_lcd.c"	LCD_putcmd(addr, LCD_LINE1 + (column - 1), 1);
	BANKSEL	r0x1017
	MOVF	r0x1017,W
	MOVWF	r0x1016
	MOVLW	0x7f
	ADDWF	r0x1016,F
	MOVLW	0x01
	MOVWF	STK01
	MOVF	r0x1016,W
	MOVWF	STK00
	MOVF	r0x1015,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	92; "i2c_lcd.c"	break;
	GOTO	_00131_DS_
_00126_DS_:
;	.line	94; "i2c_lcd.c"	LCD_putcmd(addr, LCD_LINE2 + (column - 1), 1);
	BANKSEL	r0x1017
	MOVF	r0x1017,W
	MOVWF	r0x1016
	MOVLW	0xbf
	ADDWF	r0x1016,F
	MOVLW	0x01
	MOVWF	STK01
	MOVF	r0x1016,W
	MOVWF	STK00
	MOVF	r0x1015,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	95; "i2c_lcd.c"	break;
	GOTO	_00131_DS_
_00127_DS_:
;	.line	97; "i2c_lcd.c"	LCD_putcmd(addr, LCD_LINE3 + (column - 1), 1);
	BANKSEL	r0x1017
	MOVF	r0x1017,W
	MOVWF	r0x1016
	MOVLW	0x93
	ADDWF	r0x1016,F
	MOVLW	0x01
	MOVWF	STK01
	MOVF	r0x1016,W
	MOVWF	STK00
	MOVF	r0x1015,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	98; "i2c_lcd.c"	break;
	GOTO	_00131_DS_
_00128_DS_:
;	.line	100; "i2c_lcd.c"	LCD_putcmd(addr, LCD_LINE4 + (column - 1), 1);
	BANKSEL	r0x1017
	MOVF	r0x1017,W
	MOVWF	r0x1016
	MOVLW	0xd3
	ADDWF	r0x1016,F
	MOVLW	0x01
	MOVWF	STK01
	MOVF	r0x1016,W
	MOVWF	STK00
	MOVF	r0x1015,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	101; "i2c_lcd.c"	break;
	GOTO	_00131_DS_
_00129_DS_:
;	.line	103; "i2c_lcd.c"	LCD_putcmd(addr, LCD_LINE1 + (column - 1), 1);
	BANKSEL	r0x1017
	MOVF	r0x1017,W
	MOVWF	r0x1016
	MOVLW	0x7f
	ADDWF	r0x1016,W
	MOVWF	r0x1017
	MOVLW	0x01
	MOVWF	STK01
	MOVF	r0x1017,W
	MOVWF	STK00
	MOVF	r0x1015,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
_00131_DS_:
;	.line	106; "i2c_lcd.c"	}
	RETURN	
; exit point of _LCD_goto

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _delay_ms
;   _LCD_putcmd
;   _delay_ms
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _delay_ms
;   _LCD_putcmd
;   _delay_ms
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;   _LCD_putcmd
;3 compiler assigned registers:
;   r0x1018
;   STK00
;   STK01
;; Starting pCode block
S_i2c_lcd__LCD_init	code
_LCD_init:
; 2 exit points
;	.line	72; "i2c_lcd.c"	void LCD_init(uint8_t addr) {
	BANKSEL	r0x1018
	MOVWF	r0x1018
;	.line	73; "i2c_lcd.c"	delay_ms(20);		// Wait > 15 ms after power ON
	MOVLW	0x14
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay_ms
	CALL	_delay_ms
	PAGESEL	$
;	.line	75; "i2c_lcd.c"	LCD_putcmd(addr, LCD_INIT_BYTE, 0);
	MOVLW	0x00
	MOVWF	STK01
	MOVLW	0x30
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	76; "i2c_lcd.c"	delay_ms(5);		// Wait > 4.1 ms
	MOVLW	0x05
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay_ms
	CALL	_delay_ms
	PAGESEL	$
;	.line	77; "i2c_lcd.c"	LCD_putcmd(addr, LCD_INIT_BYTE, 0);
	MOVLW	0x00
	MOVWF	STK01
	MOVLW	0x30
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	78; "i2c_lcd.c"	LCD_putcmd(addr, LCD_INIT_BYTE, 0);
	MOVLW	0x00
	MOVWF	STK01
	MOVLW	0x30
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	79; "i2c_lcd.c"	LCD_putcmd(addr, LCD_BUS_WIDTH_4Bit, 0);
	MOVLW	0x00
	MOVWF	STK01
	MOVLW	0x20
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	80; "i2c_lcd.c"	LCD_putcmd(addr, LCD_4BITS_2LINES_5x8FONT, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x28
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	81; "i2c_lcd.c"	LCD_putcmd(addr, LCD_DISPLAY_OFF_CURSOR_OFF_BLINK_OFF, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x08
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	82; "i2c_lcd.c"	LCD_putcmd(addr, LCD_CLEAR, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x01
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	83; "i2c_lcd.c"	LCD_putcmd(addr, LCD_INCREMENT_NO_SHIFT, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x06
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	84; "i2c_lcd.c"	LCD_putcmd(addr, LCD_DISPLAY_ON_CURSOR_OFF, 1);
	MOVLW	0x01
	MOVWF	STK01
	MOVLW	0x0c
	MOVWF	STK00
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_LCD_putcmd
	CALL	_LCD_putcmd
	PAGESEL	$
;	.line	85; "i2c_lcd.c"	}
	RETURN	
; exit point of _LCD_init

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;7 compiler assigned registers:
;   r0x1003
;   STK00
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;   r0x1008
;; Starting pCode block
S_i2c_lcd__LCD_putch	code
_LCD_putch:
; 2 exit points
;	.line	61; "i2c_lcd.c"	void LCD_putch(uint8_t addr, uint8_t data) {
	BANKSEL	r0x1003
	MOVWF	r0x1003
	MOVF	STK00,W
;	.line	63; "i2c_lcd.c"	lcddata = HI_NIBBLE(data) | LCD_BL | LCD_RS;	// Get high nibble
	MOVWF	r0x1004
	MOVWF	r0x1005
	CLRF	r0x1006
	MOVLW	0xf0
	ANDWF	r0x1005,F
	CLRF	r0x1006
	MOVLW	0x09
	IORWF	r0x1005,F
;	.line	64; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata | LCD_EN);	// Send it!
	MOVF	r0x1005,W
	MOVWF	r0x1007
	MOVWF	r0x1005
	CLRF	r0x1006
	BSF	r0x1005,2
	MOVF	r0x1005,W
;;1	MOVWF	r0x1008
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	65; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
	BANKSEL	r0x1007
	BCF	r0x1007,2
	MOVF	r0x1007,W
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	66; "i2c_lcd.c"	lcddata = LO_NIBBLE(data) | LCD_BL | LCD_RS;	// Get low nibble
	BANKSEL	r0x1004
	MOVF	r0x1004,W
	MOVWF	r0x1005
	SWAPF	r0x1005,W
	ANDLW	0xf0
	MOVWF	r0x1004
	MOVWF	r0x1005
	CLRF	r0x1006
	BTFSC	r0x1005,7
	DECF	r0x1006,F
	MOVLW	0xf0
	ANDWF	r0x1005,W
	MOVWF	r0x1004
	CLRF	r0x1007
	MOVLW	0x09
	IORWF	r0x1004,F
;	.line	67; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata | LCD_EN);	// Send it!
	MOVF	r0x1004,W
	MOVWF	r0x1005
	MOVWF	r0x1004
	CLRF	r0x1007
	BSF	r0x1004,2
	MOVF	r0x1004,W
	MOVWF	r0x1006
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	68; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
	BANKSEL	r0x1005
	BCF	r0x1005,2
	MOVF	r0x1005,W
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	69; "i2c_lcd.c"	}
	RETURN	
; exit point of _LCD_putch

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _delay_ms
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _I2C_PCF8574_Write
;   _delay_ms
;9 compiler assigned registers:
;   r0x100E
;   STK00
;   r0x100F
;   STK01
;   r0x1010
;   r0x1011
;   r0x1012
;   r0x1013
;   r0x1014
;; Starting pCode block
S_i2c_lcd__LCD_putcmd	code
_LCD_putcmd:
; 2 exit points
;	.line	42; "i2c_lcd.c"	void LCD_putcmd(uint8_t addr, uint8_t data, uint8_t cmdtype) {
	BANKSEL	r0x100E
	MOVWF	r0x100E
	MOVF	STK00,W
	MOVWF	r0x1011
	MOVWF	r0x100F
	MOVF	STK01,W
	MOVWF	r0x1010
;;99	MOVF	r0x100F,W
;	.line	46; "i2c_lcd.c"	lcddata = HI_NIBBLE(data) | LCD_BL;
	CLRF	r0x1012
	MOVLW	0xf0
	ANDWF	r0x1011,F
	CLRF	r0x1012
	BSF	r0x1011,3
;	.line	47; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata | LCD_EN);
	MOVF	r0x1011,W
	MOVWF	r0x1013
	MOVWF	r0x1011
	CLRF	r0x1012
	BSF	r0x1011,2
	MOVF	r0x1011,W
;;1	MOVWF	r0x1014
	MOVWF	STK00
	MOVF	r0x100E,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	48; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
	BANKSEL	r0x1013
	BCF	r0x1013,2
	MOVF	r0x1013,W
	MOVWF	STK00
	MOVF	r0x100E,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	51; "i2c_lcd.c"	if (cmdtype) {
	BANKSEL	r0x1010
	MOVF	r0x1010,W
	BTFSC	STATUS,2
	GOTO	_00112_DS_
;	.line	53; "i2c_lcd.c"	lcddata = LO_NIBBLE(data) | LCD_BL;
	MOVF	r0x100F,W
	MOVWF	r0x1010
	SWAPF	r0x1010,W
	ANDLW	0xf0
	MOVWF	r0x100F
	MOVWF	r0x1010
	CLRF	r0x1011
	BTFSC	r0x1010,7
	DECF	r0x1011,F
	MOVLW	0xf0
	ANDWF	r0x1010,W
	MOVWF	r0x100F
	CLRF	r0x1012
	BSF	r0x100F,3
;	.line	54; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata | LCD_EN);
	MOVF	r0x100F,W
	MOVWF	r0x1010
	MOVWF	r0x100F
	CLRF	r0x1012
	BSF	r0x100F,2
	MOVF	r0x100F,W
	MOVWF	r0x1011
	MOVWF	STK00
	MOVF	r0x100E,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
;	.line	55; "i2c_lcd.c"	I2C_PCF8574_Write(addr, lcddata & ~LCD_EN);	// Reset LCD bus
	BANKSEL	r0x1010
	BCF	r0x1010,2
	MOVF	r0x1010,W
	MOVWF	STK00
	MOVF	r0x100E,W
	PAGESEL	_I2C_PCF8574_Write
	CALL	_I2C_PCF8574_Write
	PAGESEL	$
_00112_DS_:
;	.line	57; "i2c_lcd.c"	delay_ms(2);		// For most command, Wait > 100 us is ok.
	MOVLW	0x02
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	_delay_ms
	CALL	_delay_ms
	PAGESEL	$
;	.line	58; "i2c_lcd.c"	}
	RETURN	
; exit point of _LCD_putcmd

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_start
;   _i2c_write
;   _i2c_write
;   _i2c_stop
;   _i2c_start
;   _i2c_write
;   _i2c_write
;   _i2c_stop
;4 compiler assigned registers:
;   r0x1000
;   STK00
;   r0x1001
;   r0x1002
;; Starting pCode block
S_i2c_lcd__I2C_PCF8574_Write	code
_I2C_PCF8574_Write:
; 2 exit points
;	.line	29; "i2c_lcd.c"	int8_t I2C_PCF8574_Write(uint8_t addr, uint8_t value) {
	BANKSEL	r0x1000
	MOVWF	r0x1000
	MOVF	STK00,W
	MOVWF	r0x1001
;	.line	31; "i2c_lcd.c"	i2c_start();
	PAGESEL	_i2c_start
	CALL	_i2c_start
	PAGESEL	$
;	.line	32; "i2c_lcd.c"	s = i2c_write(addr << 1);
	BANKSEL	r0x1000
	MOVF	r0x1000,W
	MOVWF	r0x1002
	LSLF	r0x1002,W
	MOVWF	r0x1000
	PAGESEL	_i2c_write
	CALL	_i2c_write
	PAGESEL	$
	BANKSEL	r0x1000
	MOVWF	r0x1000
;;signed compare: left < lit(0x0=0), size=1, mask=ff
;	.line	33; "i2c_lcd.c"	if (s < 0) {		//bus collision ?
	BSF	STATUS,0
	BTFSS	r0x1000,7
;	.line	35; "i2c_lcd.c"	WCOL = 0;	// clear collision status bit
	BCF	STATUS,0
;	.line	37; "i2c_lcd.c"	s = i2c_write(value);
	BTFSS	STATUS,0
	GOTO	_00001_DS_
	BANKSEL	_SSP1CONbits
	BCF	_SSP1CONbits,7
_00001_DS_:
	BANKSEL	r0x1001
	MOVF	r0x1001,W
	PAGESEL	_i2c_write
	CALL	_i2c_write
	PAGESEL	$
	BANKSEL	r0x1000
	MOVWF	r0x1000
;	.line	38; "i2c_lcd.c"	i2c_stop();
	PAGESEL	_i2c_stop
	CALL	_i2c_stop
	PAGESEL	$
;	.line	39; "i2c_lcd.c"	return s;
	BANKSEL	r0x1000
	MOVF	r0x1000,W
;	.line	40; "i2c_lcd.c"	}
	RETURN	
; exit point of _I2C_PCF8574_Write


;	code size estimation:
;	  312+   96 =   408 instructions ( 1008 byte)

	end
