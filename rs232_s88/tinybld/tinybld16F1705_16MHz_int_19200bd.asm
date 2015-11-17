	radix 	DEC
        
	; change these lines accordingly to your application	
#include "p16f1705.inc"
	PROCESSOR   P16F1705
IdTypePIC = 0x29		; , Please refer to the table below, must exists in "piccodes.ini"	
#define max_flash  0x2000	; in WORDS, not bytes!!! (= 'max flash memory' from "piccodes.ini" divided by 2), Please refer to the table below

xtal 	EQU 	16000000	; you may also want to change: _HS_OSC _XT_OSC
baud 	EQU 	19200		; standard TinyBld baud rates: 115200 or 19200

PDIP		EQU	14
change_RX	EQU	1	; RX Pin 0:default 1:use rxPPS
rxPPS		EQU	21	; Serial RX Pin select, Please refer to the table below
txPPS		EQU	20	; Serial TX Pin select, Please refer to the table below

;   The above 9 lines can be changed and buid a bootloader for the desired frequency (and PIC type)

; +---------+--------+------------+--------+--------+-----------+--------+------+
; |IdTypePIC| Device | Erase_Page | TX bit | RX bit | max_flash | EEPROM | PDIP |
; +---------+--------+------------+--------+--------+-----------+--------+------+
; |   0x27  |16F1704 |  32 words  |  PPS   |  C5(5) |  0x1000   |    0   |  14  |
; |   0x29  |16F1705 |  32 words  |  PPS   |  C5(5) |  0x2000   |    0   |  14  |
; |   0x27  |16F1708 |  32 words  |  PPS   |  B5(12)|  0x1000   |    0   |  20  |
; |   0x29  |16F1709 |  32 words  |  PPS   |  B5(12)|  0x2000   |    0   |  20  |
; +---------+--------+------------+--------+--------+-----------+--------+------+

; +----------+------+----------+------+
; | register | BANK | register | BANK |
; +----------+------+----------+------+
; | PMCON1/2 |  3   |PMADRL/DAT|  3   |
; +----------+------+----------+------+

;<rxPPS> RXPPS:0x0E24 ,<txPPS> RxxPPS:0x0E90~
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|  RA0   |  0  |  RA1   |  1  |  RA2   |  2  |  RA3   |  3  |  RA4   |  4  |  RA5   |  5  |
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|  RC0   | 16  |  RC1   | 17  |  RC2   | 18  |  RC3   | 19  |  RC4   | 20  |  RC5   | 21  |
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;20pin only
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|PORT/BIT|value|
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+
;|  RB4   | 12  |  RB5   | 13  |  RB6   | 14  |  RB7   | 15  |  RC6   | 22  |  RC7   | 23  |
;+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+--------+-----+


        ;********************************************************************
	;	Tiny Bootloader		16F170x series		Size=100words
	;	(2014.07.30 Revision 2)
	;
        ;       claudiu.chiculita@ugal.ro
        ;       http://www.etc.ugal.ro/cchiculita/software/picbootloader.htm
	;
	;	This program is only available in Tiny AVR/PIC Bootloader +.
	;
	;	Tiny AVR/PIC Bootloader +
	;	https://sourceforge.net/projects/tinypicbootload/
	;
        ;********************************************************************

	#include "spbrgselect.inc"

	#define first_address max_flash-100 ; 100 word in size

    __CONFIG _CONFIG1, _FOSC_INTOSC & _WDTE_OFF & _PWRTE_ON & _MCLRE_ON & _CP_OFF & _BOREN_OFF & _CLKOUTEN_OFF & _IESO_OFF & _FCMEN_OFF
    __CONFIG _CONFIG2, _WRT_OFF & _PPS1WAY_OFF & _ZCDDIS_OFF & _PLLEN_OFF & _STVREN_OFF & _BORV_LO & _LPBOR_OFF & _LVP_ON


	errorlevel 1, -305		; suppress warning msg that takes f as default
	
	cblock 0x7B
	crc
	i
	cnt1
	cnt2
	cnt3
	endc
	

;0000000000000000000000000 RESET 00000000000000000000000000

	org	0x0000
;	pagesel	IntrareBootloader
;	goto	IntrareBootloader
	DW	0x339F		;bra $-0x60

;view with TabSize=4
;&&&&&&&&&&&&&&&&&&&&&&&   START     &&&&&&&&&&&&&&&&&
;----------------------  Bootloader  ----------------------
;               
					;PC_flash:    C1h          AddrH  AddrL  nr  ...(DataLo DataHi)...  crc
					;PIC_response:   id   K                                                 K

	org 	first_address
;	nop
;	nop
;	nop
;	nop
	org 	first_address+4
IntrareBootloader:
					;init int clock & serial port
	movlp	(max_flash>>8)-1
	movlw	PIR1			;FSR1=PIR1
	movwf	FSR1L

	movlb	.28			;BANK28
	movlw 	0x55			;unlock pps
	movwf 	PPSLOCK
	movlw 	0xAA
	movwf 	PPSLOCK
	bcf 	PPSLOCK,PPSLOCKED
 #if (change_RX==1)
	movlw	rxPPS			;set RX
	movwf	RXPPS
 #endif
	movlb	.29			;BANK29
	movlw	B'00010100'		;set TX
	movwf	RA0PPS+txPPS
	movlb	.28			;BANK28
	movlw 	0x55			;lock pps
	movwf 	PPSLOCK
	movlw 	0xAA
	movwf 	PPSLOCK
	bsf 	PPSLOCK,PPSLOCKED

	movlb	0x01			;BANK1
	bsf	OSCCON,6		;internal clock 16MHz

	movlb	0x03			;BANK3

 #if PDIP==14
	clrf	ANSELA+2		;ANSELC
 #else
	clrf	ANSELA+1		;ANSELB
 #endif

 #if ((txPPS/8==0) | (rxPPS/8==0))
	clrf	ANSELA			;ANSELA
 #endif
 #if ((txPPS/8==1) | (rxPPS/8==1))
	clrf	ANSELA+1		;ANSELB
 #endif
 #if ((txPPS/8==2) | (rxPPS/8==2))
	clrf	ANSELA+2		;ANSELC
 #endif

	movlw	b'00100100'
	movwf	TXSTA
	movlw	spbrg_value
	movwf	SPBRGL
	movlw	b'10010000'
	movwf	RCSTA
					;wait for computer
	call	Receive			
	sublw	0xC1			;Expect C1
	skpz
	bra	way_to_exit
	movlw	IdTypePIC		;PIC type
	movwf	TXREG
;	SendL	IdSoftVer		;firmware ver x

MainLoop:
	movlw 	'B'
mainl:
	movwf	TXREG
	clrf	crc
	call	Receive			;H
	movwf	PMADRH
	call	Receive			;L
	movwf	PMADRL
	call	Receive			;count
	movwf	i

rcvoct:
	call	Receive			;data L
	movwf	PMDATL
	decf	i,f
	call	Receive			;data H
	movwf	PMDATH
	movlw 	((1<<LWLO) | (1<<WREN))
	call	wr_w			;write Latches
	incf	PMADRL,f
	decfsz	i,f
	bra	rcvoct

	call	Receive			;crc
ziieroare:
	movlw 	'N'
	skpz
	bra	mainl

	decf	PMADRL,f		;PMADRL=PMADRL-1
	call	wr_e 			;erase operation
	call	wr_w			;write operation
	bra	MainLoop


Receive:
	movlw	xtal/2000000+2		;for 20MHz => 11 => 1second
	movwf	cnt1
rpt2:
	clrf	cnt2
rpt3:
	clrf	cnt3
rptc:
	btfss 	INDF1,RCIF		;test RX
	bra 	$+4			;not recive
	movf 	RCREG,w			;return in w
	addwf 	crc,f			;compute checksum
	return

	decfsz	cnt3,f
	bra	rptc
	decfsz	cnt2,f
	bra	rpt3
	decfsz	cnt1,f
	bra	rpt2
					;timeout:
way_to_exit:				;exit in all other cases; must be BANK3
	clrf	PMCON1
	bcf	RCSTA,SPEN		;deactivate UART
	movlb	0x00			;BANK0
	clrf	PCLATH
	bra	first_address		; PCLATH=0, Please do not change the GOTO instruction.

wr_e:
	movlw 	((1<<FREE) | (1<<WREN))
wr_w:
	movwf	PMCON1
	movlw	0x55
	movwf	PMCON2
	movlw	0xaa
	movwf	PMCON2
	bsf	PMCON1,WR
	nop
	nop
	retlw	((1<<WREN))

;*************************************************************
; After reset
; Do not expect the memory to be zero,
; Do not expect registers to be initialised like in catalog.

	end
