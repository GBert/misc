;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #10289 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"i2c.c"
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

	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_i2c_wait
	global	_i2c_start
	global	_i2c_restart
	global	_i2c_stop
	global	_i2c_sendACK
	global	_i2c_sendNACK
	global	_i2c_write

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_i2c_0	udata
r0x1001	res	1
r0x1002	res	1
_i2c_write_data_1_22	res	1
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
code_i2c	code
;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_write	code
_i2c_write:
; 2 exit points
;	.line	46; "i2c.c"	int8_t i2c_write(volatile uint8_t data) {
	BANKSEL	_i2c_write_data_1_22
	MOVWF	_i2c_write_data_1_22
;	.line	47; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	48; "i2c.c"	SSPBUF = data;
	BANKSEL	_i2c_write_data_1_22
	MOVF	_i2c_write_data_1_22,W
	BANKSEL	_SSPBUF
	MOVWF	_SSPBUF
;	.line	49; "i2c.c"	if (WCOL)
	BTFSS	_SSP1CONbits,7
	GOTO	_00134_DS_
;	.line	50; "i2c.c"	return -1;
	MOVLW	0xff
	GOTO	_00135_DS_
_00134_DS_:
;	.line	51; "i2c.c"	return 0;
	MOVLW	0x00
_00135_DS_:
;	.line	52; "i2c.c"	}
	RETURN	
; exit point of _i2c_write

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_sendNACK	code
_i2c_sendNACK:
; 2 exit points
;	.line	41; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	42; "i2c.c"	ACKDT = 1;
	BANKSEL	_SSP1CON2bits
	BSF	_SSP1CON2bits,5
;	.line	43; "i2c.c"	ACKEN = 1;
	BSF	_SSP1CON2bits,4
;	.line	44; "i2c.c"	}
	RETURN	
; exit point of _i2c_sendNACK

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_sendACK	code
_i2c_sendACK:
; 2 exit points
;	.line	35; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	36; "i2c.c"	ACKDT = 0;
	BANKSEL	_SSP1CON2bits
	BCF	_SSP1CON2bits,5
;	.line	37; "i2c.c"	ACKEN = 1;
	BSF	_SSP1CON2bits,4
;	.line	38; "i2c.c"	}
	RETURN	
; exit point of _i2c_sendACK

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_stop	code
_i2c_stop:
; 2 exit points
;	.line	30; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	31; "i2c.c"	PEN = 1;
	BANKSEL	_SSP1CON2bits
	BSF	_SSP1CON2bits,2
;	.line	32; "i2c.c"	}
	RETURN	
; exit point of _i2c_stop

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_restart	code
_i2c_restart:
; 2 exit points
;	.line	25; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	26; "i2c.c"	RSEN = 1;
	BANKSEL	_SSP1CON2bits
	BSF	_SSP1CON2bits,1
;	.line	27; "i2c.c"	}
	RETURN	
; exit point of _i2c_restart

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _i2c_wait
;   _i2c_wait
;; Starting pCode block
S_i2c__i2c_start	code
_i2c_start:
; 2 exit points
;	.line	20; "i2c.c"	i2c_wait();
	PAGESEL	_i2c_wait
	CALL	_i2c_wait
	PAGESEL	$
;	.line	21; "i2c.c"	SEN = 1;
	BANKSEL	_SSP1CON2bits
	BSF	_SSP1CON2bits,0
;	.line	22; "i2c.c"	}
	RETURN	
; exit point of _i2c_start

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;2 compiler assigned registers:
;   r0x1001
;   r0x1002
;; Starting pCode block
S_i2c__i2c_wait	code
_i2c_wait:
; 2 exit points
_00105_DS_:
;	.line	14; "i2c.c"	while (SSP1STAT & 0x05) ;
	BANKSEL	_SSP1STAT
	MOVF	_SSP1STAT,W
	BANKSEL	r0x1001
	MOVWF	r0x1001
	CLRF	r0x1002
	MOVF	r0x1001,W
	ANDLW	0x05
	BTFSS	STATUS,2
	GOTO	_00105_DS_
_00108_DS_:
;	.line	16; "i2c.c"	while (SSP1CON2 & 0x1F) ;
	BANKSEL	_SSP1CON2
	MOVF	_SSP1CON2,W
	BANKSEL	r0x1001
	MOVWF	r0x1001
	CLRF	r0x1002
	MOVF	r0x1001,W
	ANDLW	0x1f
	BTFSS	STATUS,2
	GOTO	_00108_DS_
;	.line	17; "i2c.c"	}
	RETURN	
; exit point of _i2c_wait


;	code size estimation:
;	   42+   24 =    66 instructions (  180 byte)

	end
