" Vim syntax file
" Language:     PIC16F84 Assembler (Microchip's microcontroller)
" Maintainer:   Aleksandar Veselinovic <aleksa@cs.cmu.com>
" Last Change:  2003 May 11
" URL:		http://galeb.etf.bg.ac.yu/~alexa/vim/syntax/pic.vim
" Revision:     1.01

" 20040419 : changed some things to suit pic18fxxx assembly
" Yves Kerléer yfig@lavache.com

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case match
syn keyword picTodo NOTE TODO XXX contained

syn case ignore

syn match picIdentifier "[a-z_$][a-z0-9_$]*"
syn match picLabel      "^[A-Z_$][A-Z0-9_$]*"
syn match picLabel      "^[A-Z_$][A-Z0-9_$]*:"me=e-1

syn match picASCII      "A\='.'"
syn match picBinary     "B'[0-1]\+'"
syn keyword picBinary	FAST W A ACCESS BANKED
syn match picDecimal    "D'\d\+'"
syn match picDecimal    "\d\+"
syn match picHexadecimal "0x\x\+"
syn match picHexadecimal "H'\x\+'"
syn match picHexadecimal "[0-9]\x*h"
syn keyword picHexadecimal	FSR0 FSR1 FSR2
syn match picOctal      "O'[0-7]\o*'"


syn match picComment    ";.*" contains=picTodo

syn region picString    start=+"+ end=+"+

syn keyword picRegister		PCL STATUS PORTA PORTB
syn keyword picRegister		EEDATA EEADR PCLATH
syn keyword picRegister		TRISA TRISB EECON1 EECON2
				"added SFRs:
syn keyword picRegister		TOSU TOSH TOSL STKPTR PCLATU
syn keyword picRegister		TBLPTRU TBLPTRH TBLPTRL TABLAT PRODH PRODL
syn keyword picRegister		INTCON INTCON1 INTCON2 INTCON3
syn keyword picRegister		INDF0 POSTINC0 POSTDEC0 PREINC0 PLUSW0
syn keyword picRegister		INDF1 POSTINC1 POSTDEC1 PREINC1 PLUSW1
syn keyword picRegister		INDF2 POSTINC2 POSTDEC2 PREINC2 PLUSW2
syn keyword picRegister		FSR0H FSR0L WREG FSR1H FSR1L
syn keyword picRegister		BSR FSR2H FSR2L TMR0H TMR0L T0CON
syn keyword picRegister		OSCCON LVDCON WDTCON RCON TMR1H TMR1L
syn keyword picRegister		T1CON TMR2 PR2 T2CON SSPBUF SSPADD
syn keyword picRegister		SSPSTAT SSPCON1 SSPCON2 ADRESH ADRESL
syn keyword picRegister		ADCON0 ADCON1 CCPR1H CCPR1L CCP1CON
syn keyword picRegister		CCPR2H CCPR2L CCP2CON TMR3H TMR3L T3CON
syn keyword picRegister		SPBRG RCREG TXREG TXSTA RCSTA IPR2 PIR2 PIE2
syn keyword picRegister		IPR1 PIR1 PIE1 TRISE TRISD TRISC
syn keyword picRegister		LATE LATD LATC LATB LATA PORTE PORTD PORTC

" Register --- bits

" STATUS
syn keyword picRegisterPart     N OV Z DC C

" PORTA
syn keyword picRegisterPart     T0CKI VREFM VREFP SS LVDIN OSC2 CLKO
syn match   picRegisterPart     "RA[0-6]"
syn match   picRegisterPart	"AN[0-4]"

" PORTB
syn keyword picRegisterPart     CCP2A
syn match   picRegisterPart     "RB[0-7]" 
syn match   picRegisterPart	"INT[0-2]"

" INTCON
syn keyword picRegisterPart     GIE GIEH PEIE GIEL TMR0IE INT0IE RBIE
syn keyword picRegisterPart	TMR0IF INT0IF RBIF T0IE INT0E T0IF INT0F

" EECON
syn keyword picRegisterPart     EEPGD CFGS FREE WRERR WREN WR RD

" TOSU
syn match   picRegisterPart	"TOS1[6-9]"
syn match   picRegisterPart	"TOS20"

" TOSH
syn match   picRegisterPart	"TOS[8-9]"
syn match   picRegisterPart	"TOS1[0-5]"

" TOSL
syn match   picRegisterPart	"TOS[0-7]"

" STKPTR
syn keyword picRegisterPart	STKFUL STKUNF

" PCLATU
syn match   picRegisterPart	"PC20"
syn match   picRegisterPart	"PC1[6-9]"

" PCLATH
syn match   picRegisterPart	"PC[8-9]"
syn match   picRegisterPart	"PC1[0-5]"

" PCL
syn match   picRegisterPart	"PC[0-7]"

" TBLPTRU
syn match   picRegisterPart	"TBLPTR20"
syn match   picRegisterPart	"TBLPTR1[6-9]"

" TBLPTRH
syn match   picRegisterPart	"TBLPTR[8-9]"
syn match   picRegisterPart	"TBLPTR1[0-5]"

" TBLPTRL
syn match   picRegisterPart	"TBLPTR[0-7]"

" INTCON2
syn keyword picRegisterPart	RBPU INTEDG0 INTEDG1 INTEDG2 TMR0IP RBIP
syn keyword picRegisterPart	NOT_RBPU T0IP

" INTCON3
syn keyword picRegisterPart	INT2IP INT1IP INT2IE INT1IE INT2IF INT1IF

" T0CON
syn keyword picRegisterPart	TMR0ON T08BIT T0CS T0SE PSA T0PS2 T0PS1 T0PS0

" OSCCON
syn keyword picRegisterPart	SCS

" LVDCON
syn keyword picRegisterPart	IRVST LVDEN LVDL3 LVDL2 LVDL1 LVDL0

" WDTCON
syn keyword picRegisterPart	SWDTE SWDTEN

" RCON
syn keyword picRegisterPart	IPEN NOT_RI RI NOT_TO TO NOT_PD PD
syn keyword picRegisterPart	NOT_POR POR NOT_BOR BOR

" T1CON
syn keyword picRegisterPart	RD16 T1CKPS1 T1CKPS0 T1OSCEN T1SYNC TMR1CS
syn keyword PICRegisterPart	TMR1ON NOT_T1SYNC T1INSYNC

" T2CON
syn keyword picRegisterPart	TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON
syn keyword picRegisterPart	T2CKPS1 T2CKPS0

" SSPSTAT
syn keyword picRegisterPart	SMP CKE D I2C_DAT NOT_A NOT_ADDRESS D_A
syn keyword picRegisterPart	DATA_ADDRESS P I2C_STOP S I2C_START R
syn keyword picRegisterPart	I2C_READ NOT_W NOT_WRITE R_W READ_WRITE UA BF

" SSPCON1
syn keyword picRegisterPart	WCOL SSPOV SSPEN CKP
syn match   picRegisterPart	"SSPM[0-3]"

" SSPCON2
syn keyword picRegisterPart	GCEN ACKSTAT ACKDT ACKEN RCEN PEN RSEN SEN

" ADCON0
syn keyword picRegisterPart	ADCS1 ADCS0 GO NOT_DONE DONE GO_DONE ADON
syn match   picRegisterPart	"CHS[0-2]"

" ADCON1
syn keyword picRegisterPart	ADFM ADCS2
syn match   picRegisterPart	"PCFG[0-3]"

" CCP1CON
syn keyword picRegisterPart	DC1B1 CCP1X DC1B0 CCP1Y
syn match   picRegisterPart	"CCP1M[0-3]"

" CCP2CON
syn keyword picRegisterPart	DC2B1 CCP2X DC2B0 CCP2Y
syn match   picRegisterPart	"CCP2M[0-3]"

" T3CON
syn keyword picRegisterPart	RD16 T3CCP2 T3CKPS1 T3CKPS0 T3CCP1
syn keyword picRegisterPart	NOT_T3SYNC T3SYNC T3INSYNC TMR3CS TMR3ON

" TXSTA
syn keyword picRegisterPart	CSRC TX9 NOT_TX8 TX8_9 TXEN SYNC BRGH TRMT
syn keyword picRegisterPart	TX9D TXD8

" RCSTA
syn keyword picRegisterPart	SPEN RX9 RC9 NOT_RC8 RC8_9 SREN CREN ADDEN
syn keyword picRegisterPart	FERR OERR RX9D RCD8

" IPR2
syn keyword picRegisterPart	EEIP BCLIP LVDIP TMR3IP CCP2IP

" PIR2
syn keyword picRegisterPart	EEIF BCLIF LVDIF TMR3IF CCP2IF

" PIE2
syn keyword picRegisterPart	EEIE BCLIE LVDIE TMR3IE CCP2IE

" IPR1
syn keyword picRegisterPart	PSPIP ADIP RCIP TXIP SSPIP CCP1IP TMR2IP
syn keyword picRegisterPart	TMR1IP

" PIR1
syn keyword picRegisterPart	PSPIF ADIF RCIF TXIF SSPIF CCP1IF TMR2IF
syn keyword picRegisterPart	TMR1IF

" PIE1
syn keyword picRegisterPart	PSPIE ADIE RCIE TXIE SSPIE CCP1IE TMR2IE
syn keyword picRegisterPart	TMR1IE

" TRISE
syn keyword picRegisterPart	IBF OBF IBOV PSPMODE TRISE2 TRISE1 TRISE0

" PORTC
syn match   picRegisterPart	"RC[0-7]"
syn keyword picRegisterPart	T1OSO T1CKI T1OSI CCP2 CCP1 SCK SCL SDI SDA
syn keyword picRegisterPart	SDO TX CK RX

" PORTD
syn match   picRegisterPart	"RD[0-7]"
syn match   picRegisterPart	"PSP[0-7]"

" PORTE
syn match   picRegisterPart	"RE[0-2]"
syn keyword picRegisterPart	RD AN5 WR AN6 CS AN7


" OpCodes...
syn keyword picOpcode  ADDWF ANDWF CLRF COMF DECF DECFSZ INCF INCFSZ
syn keyword picOpcode  IORWF MOVF MOVWF NOP SUBWF SWAPF XORWF
						"removed RLF RRF CLRW
syn keyword picOpcode  BCF BSF BTFSC BTFSS
syn keyword picOpcode  ADDLW ANDLW CALL CLRWDT GOTO IORLW MOVLW RETFIE
syn keyword picOpcode  RETLW RETURN SLEEP SUBLW XORLW
syn keyword picOpcode  GOTO
						"added some others:
syn keyword picOpcode  addwfc cpfseq cpfsgt cpfslt dcfsnz infsnz movff mulwf
syn keyword picOpcode  negf rlcf rlncf rrcf rrncf setf subwfb tstfsz
syn keyword picOpcode  btg
syn keyword picOpcode  bc bn bnc bnn bnov bnz bov bra bz
syn keyword picOpcode  daw pop push rcall reset
syn keyword picOpcode  lfsr movlb mullw
syn keyword picOpcode  tblrd* tblrd*+ tblrd*- tblrd+*
syn keyword picOpcode  tblwt* tblwt*+ tblwt*- tblwt+*


" Directives
syn keyword picDirective __BADRAM BANKISEL BANKSEL CBLOCK CODE __CONFIG
syn keyword picDirective CONSTANT DATA DB DE DT DW ELSE END ENDC
syn keyword picDirective ENDIF ENDM ENDW EQU ERROR ERRORLEVEL EXITM EXPAND
syn keyword picDirective EXTERN FILL GLOBAL IDATA __IDLOCS IF IFDEF IFNDEF
syn keyword picDirective INCLUDE LIST LOCAL MACRO __MAXRAM MESSG NOEXPAND
syn keyword picDirective NOLIST ORG PAGE PAGESEL PROCESSOR RADIX RES SET
syn keyword picDirective SPACE SUBTITLE TITLE UDATA UDATA_OVR UDATA_SHR
syn keyword picDirective VARIABLE WHILE INCLUDE
syn match picDirective   "#\=UNDEFINE"
syn match picDirective   "#\=INCLUDE"
syn match picDirective   "#\=DEFINE"


" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508 || !exists("did_pic16f84_syntax_inits")
  if version < 508
    let did_pic16f84_syntax_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink picTodo		Todo
  HiLink picComment		Comment
  HiLink picDirective		Statement
  HiLink picLabel		Label
  HiLink picString		String

 "HiLink picOpcode		Keyword
 "HiLink picRegister		Structure
 "HiLink picRegisterPart	Special

  HiLink picASCII		String
  HiLink picBinary		Number
  HiLink picDecimal		Number
  HiLink picHexadecimal		Number
  HiLink picOctal		Number

  HiLink picIdentifier		Identifier

  delcommand HiLink
endif

let b:current_syntax = "pic"

" vim: ts=8

