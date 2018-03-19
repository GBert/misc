;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.7.1 #10289 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"usart.c"
	list	p=16f1709
	radix dec
	include "p16f1709.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	_delay_ms
	extern	_init_usart
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
	extern	__gptrput1

	extern STK04
	extern STK03
	extern STK02
	extern STK01
	extern STK00
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_print_rom_fifo
	global	_fifo_putchar
	global	_print_debug_fifo
	global	_print_debug_value
	global	_puts_rom
	global	_putchar_wait
	global	_putchar
	global	_print_hex_wait
	global	_sData

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_usart_0	udata
r0x1011	res	1
r0x1023	res	1
r0x1022	res	1
r0x1021	res	1
r0x1024	res	1
r0x1025	res	1
r0x1012	res	1
r0x1013	res	1
r0x1014	res	1
r0x1015	res	1
r0x1016	res	1
r0x1017	res	1
r0x1018	res	1
r0x101B	res	1
r0x101A	res	1
r0x1019	res	1
r0x101C	res	1
r0x101D	res	1
r0x101E	res	1
r0x101F	res	1
r0x1020	res	1
r0x1002	res	1
r0x1001	res	1
r0x1000	res	1
r0x1005	res	1
r0x1004	res	1
r0x1003	res	1
r0x1006	res	1
r0x1007	res	1
r0x1008	res	1
r0x1009	res	1
r0x100A	res	1
r0x100B	res	1
r0x100C	res	1
r0x100D	res	1
r0x100E	res	1
r0x100F	res	1
r0x1010	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

ID_usart_0	idata
_sData
	db	0x00, 0x00, 0x00


ID_usart_1	code
___str_0
	retlw 0x20 ; ' '
	retlw 0x44 ; 'D'
	retlw 0x61 ; 'a'
	retlw 0x74 ; 't'
	retlw 0x61 ; 'a'
	retlw 0x3a ; ':'
	retlw 0x20 ; ' '
	retlw 0x00 ; '.'
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_usart	code
;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __gptrget1
;   __gptrget1
;   __gptrget1
;   __gptrput1
;   __gptrput1
;   __gptrget1
;   __gptrget1
;   __gptrget1
;   __gptrput1
;   __gptrput1
;22 compiler assigned registers:
;   r0x1000
;   STK00
;   r0x1001
;   STK01
;   r0x1002
;   STK02
;   r0x1003
;   STK03
;   r0x1004
;   STK04
;   r0x1005
;   r0x1006
;   r0x1007
;   r0x1008
;   r0x1009
;   r0x100A
;   r0x100B
;   r0x100C
;   r0x100D
;   r0x100E
;   r0x100F
;   r0x1010
;; Starting pCode block
S_usart__print_rom_fifo	code
_print_rom_fifo:
; 2 exit points
;	.line	100; "usart.c"	char print_rom_fifo(const unsigned char *s, struct serial_buffer *fifo) {
	BANKSEL	r0x1000
	MOVWF	r0x1000
	MOVF	STK00,W
	MOVWF	r0x1001
	MOVF	STK01,W
	MOVWF	r0x1002
	MOVF	STK02,W
	MOVWF	r0x1003
	MOVF	STK03,W
	MOVWF	r0x1004
	MOVF	STK04,W
;	.line	101; "usart.c"	unsigned char head = fifo->head;
	MOVWF	r0x1005
	MOVWF	STK01
	MOVF	r0x1004,W
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x1006
	MOVWF	r0x1006
;	.line	103; "usart.c"	while ((c = *s++)) {
	MOVLW	0x01
	ADDWF	r0x1005,W
	MOVWF	r0x1007
	CLRF	r0x1008
	RLF	r0x1008,F
	MOVF	r0x1004,W
	ADDWF	r0x1008,F
	CLRF	r0x1009
	RLF	r0x1009,F
	MOVF	r0x1003,W
	ADDWF	r0x1009,F
	MOVLW	0x02
	ADDWF	r0x1005,W
	MOVWF	r0x100A
	CLRF	r0x100B
	RLF	r0x100B,F
	MOVF	r0x1004,W
	ADDWF	r0x100B,F
	CLRF	r0x100C
	RLF	r0x100C,F
	MOVF	r0x1003,W
	ADDWF	r0x100C,F
_00163_DS_:
	BANKSEL	r0x1002
	MOVF	r0x1002,W
	MOVWF	STK01
	MOVF	r0x1001,W
	MOVWF	STK00
	MOVF	r0x1000,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x100D
	MOVWF	r0x100D
	INCF	r0x1002,F
	BTFSC	STATUS,2
	INCF	r0x1001,F
	BTFSC	STATUS,2
	INCF	r0x1000,F
	MOVF	r0x100D,W
	MOVWF	r0x100E
	MOVF	r0x100D,W
	BTFSC	STATUS,2
	GOTO	_00165_DS_
;	.line	104; "usart.c"	head++;
	INCF	r0x1006,W
	MOVWF	r0x100D
;	.line	105; "usart.c"	head &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
	MOVLW	0x1f
	ANDWF	r0x100D,W
	MOVWF	r0x1006
;	.line	106; "usart.c"	if (head != fifo->tail) {		/* space left ? */
	MOVF	r0x1007,W
	MOVWF	STK01
	MOVF	r0x1008,W
	MOVWF	STK00
	MOVF	r0x1009,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x100D
	MOVWF	r0x100D
;	.line	107; "usart.c"	fifo->data[head] = c;
	XORWF	r0x1006,W
	BTFSC	STATUS,2
	GOTO	_00161_DS_
	MOVF	r0x1006,W
	ADDWF	r0x100A,W
	MOVWF	r0x100D
	MOVF	r0x100B,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x100F
	MOVF	r0x100C,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1010
	MOVF	r0x100E,W
	MOVWF	STK02
	MOVF	r0x100D,W
	MOVWF	STK01
	MOVF	r0x100F,W
	MOVWF	STK00
	MOVF	r0x1010,W
	PAGESEL	__gptrput1
	CALL	__gptrput1
	PAGESEL	$
	GOTO	_00163_DS_
_00161_DS_:
;	.line	109; "usart.c"	return -1;
	MOVLW	0xff
	GOTO	_00166_DS_
_00165_DS_:
;	.line	112; "usart.c"	fifo->head = head;		/* only store new pointer if all is OK */
	BANKSEL	r0x1006
	MOVF	r0x1006,W
	MOVWF	STK02
	MOVF	r0x1005,W
	MOVWF	STK01
	MOVF	r0x1004,W
	MOVWF	STK00
	MOVF	r0x1003,W
	PAGESEL	__gptrput1
	CALL	__gptrput1
	PAGESEL	$
;	.line	113; "usart.c"	return 1;
	MOVLW	0x01
_00166_DS_:
;	.line	114; "usart.c"	}
	RETURN	
; exit point of _print_rom_fifo

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __gptrget1
;   _print_debug_fifo
;   __gptrget1
;   _putchar_wait
;   __gptrget1
;   _print_debug_value
;   _putchar_wait
;   _print_debug_value
;   _putchar_wait
;   _putchar_wait
;   __gptrget1
;   _putchar
;   __gptrput1
;   __gptrget1
;   _print_debug_fifo
;   __gptrget1
;   _putchar_wait
;   __gptrget1
;   _print_debug_value
;   _putchar_wait
;   _print_debug_value
;   _putchar_wait
;   _putchar_wait
;   __gptrget1
;   _putchar
;   __gptrput1
;11 compiler assigned registers:
;   r0x1019
;   STK00
;   r0x101A
;   STK01
;   r0x101B
;   r0x101C
;   r0x101D
;   r0x101E
;   r0x101F
;   r0x1020
;   STK02
;; Starting pCode block
S_usart__fifo_putchar	code
_fifo_putchar:
; 2 exit points
;	.line	76; "usart.c"	char fifo_putchar(struct serial_buffer *fifo) {
	BANKSEL	r0x1019
	MOVWF	r0x1019
	MOVF	STK00,W
	MOVWF	r0x101A
	MOVF	STK01,W
	MOVWF	r0x101B
;	.line	78; "usart.c"	tail = fifo->tail;
	MOVLW	0x01
	ADDWF	r0x101B,W
	MOVWF	r0x101C
	CLRF	r0x101D
	RLF	r0x101D,F
	MOVF	r0x101A,W
	ADDWF	r0x101D,F
	CLRF	r0x101E
	RLF	r0x101E,F
	MOVF	r0x1019,W
	ADDWF	r0x101E,F
	MOVF	r0x101C,W
	MOVWF	STK01
	MOVF	r0x101D,W
	MOVWF	STK00
	MOVF	r0x101E,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x101F
	MOVWF	r0x101F
;	.line	79; "usart.c"	print_debug_fifo(fifo);
	MOVF	r0x101B,W
	MOVWF	STK01
	MOVF	r0x101A,W
	MOVWF	STK00
	MOVF	r0x1019,W
	PAGESEL	_print_debug_fifo
	CALL	_print_debug_fifo
	PAGESEL	$
;	.line	80; "usart.c"	if (fifo->head != tail) {
	BANKSEL	r0x101B
	MOVF	r0x101B,W
	MOVWF	STK01
	MOVF	r0x101A,W
	MOVWF	STK00
	MOVF	r0x1019,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x101C
	MOVWF	r0x101C
;	.line	81; "usart.c"	tail++;
	XORWF	r0x101F,W
	BTFSC	STATUS,2
	GOTO	_00144_DS_
	INCF	r0x101F,F
;	.line	82; "usart.c"	tail &= SERIAL_BUFFER_SIZE_MASK;	/* wrap around if neededd */
	MOVLW	0x1f
	ANDWF	r0x101F,W
	MOVWF	r0x101C
;	.line	84; "usart.c"	putchar_wait('f');
	MOVLW	0x66
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	85; "usart.c"	print_debug_value('T', fifo->tail);
	MOVLW	0x01
	BANKSEL	r0x101B
	ADDWF	r0x101B,W
	MOVWF	r0x101D
	CLRF	r0x101E
	RLF	r0x101E,F
	MOVF	r0x101A,W
	ADDWF	r0x101E,F
	CLRF	r0x101F
	RLF	r0x101F,F
	MOVF	r0x1019,W
	ADDWF	r0x101F,F
	MOVF	r0x101D,W
	MOVWF	STK01
	MOVF	r0x101E,W
	MOVWF	STK00
	MOVF	r0x101F,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x1020
	MOVWF	r0x1020
	MOVWF	STK00
	MOVLW	0x54
	PAGESEL	_print_debug_value
	CALL	_print_debug_value
	PAGESEL	$
;	.line	86; "usart.c"	putchar_wait(' ');
	MOVLW	0x20
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	87; "usart.c"	print_debug_value('T', tail);
	BANKSEL	r0x101C
	MOVF	r0x101C,W
	MOVWF	STK00
	MOVLW	0x54
	PAGESEL	_print_debug_value
	CALL	_print_debug_value
	PAGESEL	$
;	.line	88; "usart.c"	putchar_wait('\r');
	MOVLW	0x0d
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	89; "usart.c"	putchar_wait('\n');
	MOVLW	0x0a
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	91; "usart.c"	if (putchar(fifo->data[tail])) {
	MOVLW	0x02
	BANKSEL	r0x101B
	ADDWF	r0x101B,F
	BTFSC	STATUS,0
	INCF	r0x101A,F
	BTFSC	STATUS,2
	INCF	r0x1019,F
	MOVF	r0x101C,W
	ADDWF	r0x101B,F
	MOVF	r0x101A,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x101A
	MOVF	r0x1019,W
	BTFSC	STATUS,0
	ADDLW	0x01
	MOVWF	r0x1019
	MOVF	r0x101B,W
	MOVWF	STK01
	MOVF	r0x101A,W
	MOVWF	STK00
	MOVF	r0x1019,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x1020
	MOVWF	r0x1020
	PAGESEL	_putchar
	CALL	_putchar
	PAGESEL	$
	BANKSEL	r0x101B
	MOVWF	r0x101B
	MOVF	r0x101B,W
	BTFSC	STATUS,2
	GOTO	_00144_DS_
;	.line	92; "usart.c"	fifo->tail = tail;
	MOVF	r0x101C,W
	MOVWF	STK02
	MOVF	r0x101D,W
	MOVWF	STK01
	MOVF	r0x101E,W
	MOVWF	STK00
	MOVF	r0x101F,W
	PAGESEL	__gptrput1
	CALL	__gptrput1
	PAGESEL	$
;	.line	93; "usart.c"	return 1;
	MOVLW	0x01
	GOTO	_00145_DS_
_00144_DS_:
;	.line	96; "usart.c"	return 0;
	MOVLW	0x00
_00145_DS_:
;	.line	97; "usart.c"	}
	RETURN	
; exit point of _fifo_putchar

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _print_debug_value
;   _putchar_wait
;   _print_debug_value
;   _putchar_wait
;   _putchar_wait
;   _putchar_wait
;   _print_debug_value
;   _putchar_wait
;   _print_debug_value
;   _putchar_wait
;   _putchar_wait
;   _putchar_wait
;1 compiler assigned register :
;   STK00
;; Starting pCode block
S_usart__print_debug_fifo	code
_print_debug_fifo:
; 2 exit points
;	.line	58; "usart.c"	print_debug_value('S', SERIAL_BUFFER_SIZE);
	MOVLW	0x20
	MOVWF	STK00
	MOVLW	0x53
	PAGESEL	_print_debug_value
	CALL	_print_debug_value
	PAGESEL	$
;	.line	59; "usart.c"	putchar_wait(' ');
	MOVLW	0x20
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	60; "usart.c"	print_debug_value('M', SERIAL_BUFFER_SIZE_MASK);
	MOVLW	0x1f
	MOVWF	STK00
	MOVLW	0x4d
	PAGESEL	_print_debug_value
	CALL	_print_debug_value
	PAGESEL	$
;	.line	61; "usart.c"	putchar_wait(' ');
	MOVLW	0x20
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	71; "usart.c"	putchar_wait('\r');
	MOVLW	0x0d
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	72; "usart.c"	putchar_wait('\n');
	MOVLW	0x0a
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	73; "usart.c"	}
	RETURN	
; exit point of _print_debug_fifo

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _putchar_wait
;   _putchar_wait
;   _print_hex_wait
;   _putchar_wait
;   _putchar_wait
;   _print_hex_wait
;3 compiler assigned registers:
;   r0x1017
;   STK00
;   r0x1018
;; Starting pCode block
S_usart__print_debug_value	code
_print_debug_value:
; 2 exit points
;	.line	49; "usart.c"	void print_debug_value(char c, unsigned char value) {
	BANKSEL	r0x1017
	MOVWF	r0x1017
	MOVF	STK00,W
	MOVWF	r0x1018
;	.line	50; "usart.c"	putchar_wait(c);
	MOVF	r0x1017,W
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	51; "usart.c"	putchar_wait(':');
	MOVLW	0x3a
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	52; "usart.c"	print_hex_wait(value);
	BANKSEL	r0x1018
	MOVF	r0x1018,W
	PAGESEL	_print_hex_wait
	CALL	_print_hex_wait
	PAGESEL	$
;	.line	53; "usart.c"	}
	RETURN	
; exit point of _print_debug_value

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   _putchar_wait
;   _putchar_wait
;   _putchar_wait
;   _putchar_wait
;5 compiler assigned registers:
;   r0x1012
;   r0x1013
;   r0x1014
;   r0x1015
;   r0x1016
;; Starting pCode block
S_usart__print_hex_wait	code
_print_hex_wait:
; 2 exit points
;	.line	36; "usart.c"	void print_hex_wait(unsigned char c) {
	BANKSEL	r0x1012
	MOVWF	r0x1012
;	.line	38; "usart.c"	nibble = ((c & 0xf0) >> 4) + '0';
	MOVWF	r0x1013
	CLRF	r0x1014
	MOVLW	0xf0
	ANDWF	r0x1013,F
	CLRF	r0x1014
	SWAPF	r0x1013,W
	ANDLW	0x0f
	MOVWF	r0x1015
	SWAPF	r0x1014,W
	MOVWF	r0x1016
	ANDLW	0xf0
	IORWF	r0x1015,F
	XORWF	r0x1016,F
	MOVLW	0xf0
	BTFSC	r0x1016,3
	IORWF	r0x1016,F
	MOVF	r0x1015,W
	MOVWF	r0x1013
	MOVLW	0x30
	ADDWF	r0x1013,F
;;unsigned compare: left < lit(0x3A=58), size=1
;	.line	39; "usart.c"	if (nibble >= 0x3a)
	MOVLW	0x3a
	SUBWF	r0x1013,W
	BTFSS	STATUS,0
	GOTO	_00126_DS_
;;genSkipc:3257: created from rifx:0x7ffc0f2c50a0
;	.line	40; "usart.c"	nibble += 7;
	MOVF	r0x1013,W
	MOVWF	r0x1015
	MOVLW	0x07
	ADDWF	r0x1015,W
	MOVWF	r0x1013
_00126_DS_:
;	.line	41; "usart.c"	putchar_wait(nibble);
	BANKSEL	r0x1013
	MOVF	r0x1013,W
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	43; "usart.c"	nibble = (c & 0x0f) + '0';
	BANKSEL	r0x1012
	MOVF	r0x1012,W
	MOVWF	r0x1013
	MOVLW	0x0f
	ANDWF	r0x1013,W
	MOVWF	r0x1012
	MOVLW	0x30
	ADDWF	r0x1012,F
;;unsigned compare: left < lit(0x3A=58), size=1
;	.line	44; "usart.c"	if (nibble >= 0x3a)
	MOVLW	0x3a
	SUBWF	r0x1012,W
	BTFSS	STATUS,0
	GOTO	_00128_DS_
;;genSkipc:3257: created from rifx:0x7ffc0f2c50a0
;	.line	45; "usart.c"	nibble += 7;
	MOVF	r0x1012,W
	MOVWF	r0x1013
	MOVLW	0x07
	ADDWF	r0x1013,W
	MOVWF	r0x1012
_00128_DS_:
;	.line	46; "usart.c"	putchar_wait(nibble);
	BANKSEL	r0x1012
	MOVF	r0x1012,W
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
;	.line	47; "usart.c"	}
	RETURN	
; exit point of _print_hex_wait

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __gptrget1
;   _putchar_wait
;   __gptrget1
;   _putchar_wait
;7 compiler assigned registers:
;   r0x1021
;   STK00
;   r0x1022
;   STK01
;   r0x1023
;   r0x1024
;   r0x1025
;; Starting pCode block
S_usart__puts_rom	code
_puts_rom:
; 2 exit points
;	.line	29; "usart.c"	void puts_rom(const char *s) {
	BANKSEL	r0x1021
	MOVWF	r0x1021
	MOVF	STK00,W
	MOVWF	r0x1022
	MOVF	STK01,W
	MOVWF	r0x1023
_00117_DS_:
;	.line	31; "usart.c"	while ((c = *s++)) {
	BANKSEL	r0x1023
	MOVF	r0x1023,W
	MOVWF	STK01
	MOVF	r0x1022,W
	MOVWF	STK00
	MOVF	r0x1021,W
	PAGESEL	__gptrget1
	CALL	__gptrget1
	PAGESEL	$
	BANKSEL	r0x1024
	MOVWF	r0x1024
	INCF	r0x1023,F
	BTFSC	STATUS,2
	INCF	r0x1022,F
	BTFSC	STATUS,2
	INCF	r0x1021,F
	MOVF	r0x1024,W
	MOVWF	r0x1025
	MOVF	r0x1024,W
	BTFSC	STATUS,2
	GOTO	_00120_DS_
;	.line	32; "usart.c"	putchar_wait(c);
	MOVF	r0x1025,W
	PAGESEL	_putchar_wait
	CALL	_putchar_wait
	PAGESEL	$
	GOTO	_00117_DS_
_00120_DS_:
;	.line	34; "usart.c"	}
	RETURN	
; exit point of _puts_rom

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1011
;; Starting pCode block
S_usart__putchar_wait	code
_putchar_wait:
; 2 exit points
;	.line	24; "usart.c"	void putchar_wait(unsigned char c) {
	BANKSEL	r0x1011
	MOVWF	r0x1011
_00112_DS_:
;	.line	25; "usart.c"	while (!TRMT) ;
	BANKSEL	_TX1STAbits
	BTFSS	_TX1STAbits,1
	GOTO	_00112_DS_
;	.line	26; "usart.c"	TXREG1 = c;
	BANKSEL	r0x1011
	MOVF	r0x1011,W
	BANKSEL	_TXREG1
	MOVWF	_TXREG1
;	.line	27; "usart.c"	}
	RETURN	
; exit point of _putchar_wait

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;1 compiler assigned register :
;   r0x1011
;; Starting pCode block
S_usart__putchar	code
_putchar:
; 2 exit points
;	.line	15; "usart.c"	char putchar(unsigned char c) {
	BANKSEL	r0x1011
	MOVWF	r0x1011
;	.line	16; "usart.c"	if (TRMT) {
	BANKSEL	_TX1STAbits
	BTFSS	_TX1STAbits,1
	GOTO	_00106_DS_
;	.line	17; "usart.c"	TXREG1 = c;
	BANKSEL	r0x1011
	MOVF	r0x1011,W
	BANKSEL	_TXREG1
	MOVWF	_TXREG1
;	.line	18; "usart.c"	return 1;
	MOVLW	0x01
	GOTO	_00107_DS_
_00106_DS_:
;	.line	20; "usart.c"	return 0;
	MOVLW	0x00
_00107_DS_:
;	.line	21; "usart.c"	}
	RETURN	
; exit point of _putchar


;	code size estimation:
;	  338+   95 =   433 instructions ( 1056 byte)

	end
