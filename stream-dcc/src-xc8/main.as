opt subtitle "Microchip Technology Omniscient Code Generator (Lite mode) build 59893"

opt pagewidth 120

	opt lm

	processor	16F1704
clrc	macro
	bcf	3,0
	endm
clrz	macro
	bcf	3,2
	endm
setc	macro
	bsf	3,0
	endm
setz	macro
	bsf	3,2
	endm
skipc	macro
	btfss	3,0
	endm
skipz	macro
	btfss	3,2
	endm
skipnc	macro
	btfsc	3,0
	endm
skipnz	macro
	btfsc	3,2
	endm
pushw	macro
	movwi fsr1++
	endm
popw	macro
	moviw --fsr1
	endm
indf	equ	0
indf0	equ	0
indf1	equ	1
pc	equ	2
pcl	equ	2
status	equ	3
fsr0l	equ	4
fsr0h	equ	5
fsr1l	equ	6
fsr1h	equ	7
bsr	equ	8
wreg	equ	9
intcon	equ	11
c	equ	1
z	equ	0
pclath	equ	10
# 49 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INDF0 equ 00h ;# 
# 68 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INDF1 equ 01h ;# 
# 87 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PCL equ 02h ;# 
# 106 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
STATUS equ 03h ;# 
# 169 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR0L equ 04h ;# 
# 188 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR0H equ 05h ;# 
# 210 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR1L equ 06h ;# 
# 229 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR1H equ 07h ;# 
# 248 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BSR equ 08h ;# 
# 299 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
WREG equ 09h ;# 
# 318 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PCLATH equ 0Ah ;# 
# 337 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INTCON equ 0Bh ;# 
# 414 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PORTA equ 0Ch ;# 
# 463 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PORTC equ 0Eh ;# 
# 512 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIR1 equ 011h ;# 
# 582 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIR2 equ 012h ;# 
# 638 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIR3 equ 013h ;# 
# 682 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR0 equ 015h ;# 
# 701 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR1 equ 016h ;# 
# 707 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR1L equ 016h ;# 
# 726 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR1H equ 017h ;# 
# 745 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T1CON equ 018h ;# 
# 816 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T1GCON equ 019h ;# 
# 885 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR2 equ 01Ah ;# 
# 904 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PR2 equ 01Bh ;# 
# 923 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T2CON equ 01Ch ;# 
# 993 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TRISA equ 08Ch ;# 
# 1037 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TRISC equ 08Eh ;# 
# 1086 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIE1 equ 091h ;# 
# 1156 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIE2 equ 092h ;# 
# 1212 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PIE3 equ 093h ;# 
# 1256 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OPTION_REG equ 095h ;# 
# 1338 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PCON equ 096h ;# 
# 1394 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
WDTCON equ 097h ;# 
# 1452 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OSCTUNE equ 098h ;# 
# 1509 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OSCCON equ 099h ;# 
# 1580 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OSCSTAT equ 09Ah ;# 
# 1641 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADRES equ 09Bh ;# 
# 1647 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADRESL equ 09Bh ;# 
# 1666 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADRESH equ 09Ch ;# 
# 1685 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADCON0 equ 09Dh ;# 
# 1764 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADCON1 equ 09Eh ;# 
# 1810 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ADCON2 equ 09Fh ;# 
# 1857 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
LATA equ 010Ch ;# 
# 1901 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
LATC equ 010Eh ;# 
# 1950 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CM1CON0 equ 0111h ;# 
# 2006 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CM1CON1 equ 0112h ;# 
# 2081 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CM2CON0 equ 0113h ;# 
# 2137 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CM2CON1 equ 0114h ;# 
# 2212 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CMOUT equ 0115h ;# 
# 2237 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BORCON equ 0116h ;# 
# 2269 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FVRCON equ 0117h ;# 
# 2344 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
DAC1CON0 equ 0118h ;# 
# 2453 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
DAC1CON1 equ 0119h ;# 
# 2572 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ZCD1CON equ 011Ch ;# 
# 2617 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ANSELA equ 018Ch ;# 
# 2661 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ANSELC equ 018Eh ;# 
# 2710 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMADR equ 0191h ;# 
# 2716 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMADRL equ 0191h ;# 
# 2735 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMADRH equ 0192h ;# 
# 2754 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMDAT equ 0193h ;# 
# 2760 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMDATL equ 0193h ;# 
# 2779 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMDATH equ 0194h ;# 
# 2798 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMCON1 equ 0195h ;# 
# 2853 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PMCON2 equ 0196h ;# 
# 2872 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
VREGCON equ 0197h ;# 
# 2892 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC1REG equ 0199h ;# 
# 2897 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RCREG equ 0199h ;# 
# 2901 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RCREG1 equ 0199h ;# 
# 2945 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TX1REG equ 019Ah ;# 
# 2950 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TXREG1 equ 019Ah ;# 
# 2954 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TXREG equ 019Ah ;# 
# 2998 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SP1BRG equ 019Bh ;# 
# 3004 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SP1BRGL equ 019Bh ;# 
# 3009 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SPBRG equ 019Bh ;# 
# 3013 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SPBRG1 equ 019Bh ;# 
# 3017 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SPBRGL equ 019Bh ;# 
# 3073 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SP1BRGH equ 019Ch ;# 
# 3078 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SPBRGH equ 019Ch ;# 
# 3082 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SPBRGH1 equ 019Ch ;# 
# 3126 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC1STA equ 019Dh ;# 
# 3131 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RCSTA1 equ 019Dh ;# 
# 3135 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RCSTA equ 019Dh ;# 
# 3305 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TX1STA equ 019Eh ;# 
# 3310 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TXSTA1 equ 019Eh ;# 
# 3314 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TXSTA equ 019Eh ;# 
# 3484 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BAUD1CON equ 019Fh ;# 
# 3489 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BAUDCON1 equ 019Fh ;# 
# 3493 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BAUDCTL1 equ 019Fh ;# 
# 3497 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BAUDCON equ 019Fh ;# 
# 3501 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BAUDCTL equ 019Fh ;# 
# 3729 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
WPUA equ 020Ch ;# 
# 3778 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
WPUC equ 020Eh ;# 
# 3827 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1BUF equ 0211h ;# 
# 3832 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPBUF equ 0211h ;# 
# 4080 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1ADD equ 0212h ;# 
# 4085 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPADD equ 0212h ;# 
# 4333 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1MSK equ 0213h ;# 
# 4338 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPMSK equ 0213h ;# 
# 4586 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1STAT equ 0214h ;# 
# 4591 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPSTAT equ 0214h ;# 
# 4707 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1CON1 equ 0215h ;# 
# 4712 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPCON equ 0215h ;# 
# 4716 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPCON1 equ 0215h ;# 
# 4720 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1CON equ 0215h ;# 
# 4976 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1CON2 equ 0216h ;# 
# 4981 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPCON2 equ 0216h ;# 
# 5097 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSP1CON3 equ 0217h ;# 
# 5102 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPCON3 equ 0217h ;# 
# 5218 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ODCONA equ 028Ch ;# 
# 5262 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ODCONC equ 028Eh ;# 
# 5311 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR1 equ 0291h ;# 
# 5317 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR1L equ 0291h ;# 
# 5336 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR1H equ 0292h ;# 
# 5355 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCP1CON equ 0293h ;# 
# 5360 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ECCP1CON equ 0293h ;# 
# 5510 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR2 equ 0298h ;# 
# 5516 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR2L equ 0298h ;# 
# 5535 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPR2H equ 0299h ;# 
# 5554 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCP2CON equ 029Ah ;# 
# 5559 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
ECCP2CON equ 029Ah ;# 
# 5709 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCPTMRS equ 029Eh ;# 
# 5796 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SLRCONA equ 030Ch ;# 
# 5840 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SLRCONC equ 030Eh ;# 
# 5889 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INLVLA equ 038Ch ;# 
# 5938 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INLVLC equ 038Eh ;# 
# 5987 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCAP equ 0391h ;# 
# 6036 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCAN equ 0392h ;# 
# 6085 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCAF equ 0393h ;# 
# 6134 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCCP equ 0397h ;# 
# 6183 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCCN equ 0398h ;# 
# 6232 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
IOCCF equ 0399h ;# 
# 6281 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR4 equ 0415h ;# 
# 6300 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PR4 equ 0416h ;# 
# 6319 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T4CON equ 0417h ;# 
# 6389 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TMR6 equ 041Ch ;# 
# 6408 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PR6 equ 041Dh ;# 
# 6427 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T6CON equ 041Eh ;# 
# 6497 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OPA1CON equ 0511h ;# 
# 6550 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
OPA2CON equ 0515h ;# 
# 6603 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM3DCL equ 0617h ;# 
# 6638 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM3DCH equ 0618h ;# 
# 6707 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM3CON equ 0619h ;# 
# 6712 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM3CON0 equ 0619h ;# 
# 6772 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM4DCL equ 061Ah ;# 
# 6807 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM4DCH equ 061Bh ;# 
# 6876 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM4CON equ 061Ch ;# 
# 6881 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PWM4CON0 equ 061Ch ;# 
# 6941 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1PHR equ 0691h ;# 
# 6998 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1PHF equ 0692h ;# 
# 7055 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1BLKR equ 0693h ;# 
# 7112 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1BLKF equ 0694h ;# 
# 7169 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1DBR equ 0695h ;# 
# 7226 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1DBF equ 0696h ;# 
# 7283 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1CON0 equ 0697h ;# 
# 7353 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1CON1 equ 0698h ;# 
# 7403 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1RIS equ 0699h ;# 
# 7458 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1RSIM equ 069Ah ;# 
# 7513 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1FIS equ 069Bh ;# 
# 7568 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1FSIM equ 069Ch ;# 
# 7623 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1ASD0 equ 069Dh ;# 
# 7688 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1ASD1 equ 069Eh ;# 
# 7725 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COG1STR equ 069Fh ;# 
# 7786 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PPSLOCK equ 0E0Fh ;# 
# 7805 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
INTPPS equ 0E10h ;# 
# 7824 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T0CKIPPS equ 0E11h ;# 
# 7843 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T1CKIPPS equ 0E12h ;# 
# 7862 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
T1GPPS equ 0E13h ;# 
# 7881 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCP1PPS equ 0E14h ;# 
# 7900 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CCP2PPS equ 0E15h ;# 
# 7919 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
COGINPPS equ 0E17h ;# 
# 7938 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPCLKPPS equ 0E20h ;# 
# 7957 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPDATPPS equ 0E21h ;# 
# 7976 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
SSPSSPPS equ 0E22h ;# 
# 7995 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RXPPS equ 0E24h ;# 
# 8014 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CKPPS equ 0E25h ;# 
# 8033 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLCIN0PPS equ 0E28h ;# 
# 8052 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLCIN1PPS equ 0E29h ;# 
# 8071 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLCIN2PPS equ 0E2Ah ;# 
# 8090 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLCIN3PPS equ 0E2Bh ;# 
# 8109 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RA0PPS equ 0E90h ;# 
# 8128 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RA1PPS equ 0E91h ;# 
# 8147 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RA2PPS equ 0E92h ;# 
# 8166 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RA4PPS equ 0E94h ;# 
# 8185 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RA5PPS equ 0E95h ;# 
# 8204 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC0PPS equ 0EA0h ;# 
# 8223 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC1PPS equ 0EA1h ;# 
# 8242 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC2PPS equ 0EA2h ;# 
# 8261 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC3PPS equ 0EA3h ;# 
# 8280 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC4PPS equ 0EA4h ;# 
# 8299 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
RC5PPS equ 0EA5h ;# 
# 8318 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLCDATA equ 0F0Fh ;# 
# 8349 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1CON equ 0F10h ;# 
# 8466 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1POL equ 0F11h ;# 
# 8543 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1SEL0 equ 0F12h ;# 
# 8634 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1SEL1 equ 0F13h ;# 
# 8725 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1SEL2 equ 0F14h ;# 
# 8816 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1SEL3 equ 0F15h ;# 
# 8907 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1GLS0 equ 0F16h ;# 
# 9018 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1GLS1 equ 0F17h ;# 
# 9129 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1GLS2 equ 0F18h ;# 
# 9240 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC1GLS3 equ 0F19h ;# 
# 9351 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2CON equ 0F1Ah ;# 
# 9468 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2POL equ 0F1Bh ;# 
# 9545 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2SEL0 equ 0F1Ch ;# 
# 9636 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2SEL1 equ 0F1Dh ;# 
# 9727 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2SEL2 equ 0F1Eh ;# 
# 9818 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2SEL3 equ 0F1Fh ;# 
# 9909 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2GLS0 equ 0F20h ;# 
# 10020 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2GLS1 equ 0F21h ;# 
# 10131 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2GLS2 equ 0F22h ;# 
# 10242 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC2GLS3 equ 0F23h ;# 
# 10353 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3CON equ 0F24h ;# 
# 10470 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3POL equ 0F25h ;# 
# 10547 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3SEL0 equ 0F26h ;# 
# 10638 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3SEL1 equ 0F27h ;# 
# 10729 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3SEL2 equ 0F28h ;# 
# 10820 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3SEL3 equ 0F29h ;# 
# 10911 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3GLS0 equ 0F2Ah ;# 
# 11022 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3GLS1 equ 0F2Bh ;# 
# 11133 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3GLS2 equ 0F2Ch ;# 
# 11244 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
CLC3GLS3 equ 0F2Dh ;# 
# 11355 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
STATUS_SHAD equ 0FE4h ;# 
# 11386 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
WREG_SHAD equ 0FE5h ;# 
# 11405 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
BSR_SHAD equ 0FE6h ;# 
# 11424 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
PCLATH_SHAD equ 0FE7h ;# 
# 11443 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR0L_SHAD equ 0FE8h ;# 
# 11462 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR0H_SHAD equ 0FE9h ;# 
# 11481 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR1L_SHAD equ 0FEAh ;# 
# 11500 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
FSR1H_SHAD equ 0FEBh ;# 
# 11519 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
STKPTR equ 0FEDh ;# 
# 11538 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TOSL equ 0FEEh ;# 
# 11557 "/opt/microchip/xc8/v1.33/include/pic16f1704.h"
TOSH equ 0FEFh ;# 
	FNCALL	_main,_system_init
	FNCALL	_main,_timer_init
	FNROOT	_main
	global	_INTCONbits
_INTCONbits	set	0xB
	global	_PIR1bits
_PIR1bits	set	0x11
	global	_T1CON
_T1CON	set	0x18
	global	_T1GCONbits
_T1GCONbits	set	0x19
	global	_TMR1H
_TMR1H	set	0x17
	global	_TMR1L
_TMR1L	set	0x16
	global	_ADCON0
_ADCON0	set	0x9D
	global	_ADCON1
_ADCON1	set	0x9E
	global	_ADCON2
_ADCON2	set	0x9F
	global	_OSCCON
_OSCCON	set	0x99
	global	_PIE1bits
_PIE1bits	set	0x91
	global	_TRISA5
_TRISA5	set	0x465
	global	_TRISC5
_TRISC5	set	0x475
	global	_CM1CON0
_CM1CON0	set	0x111
	global	_CM1CON1
_CM1CON1	set	0x112
	global	_CM2CON0
_CM2CON0	set	0x113
	global	_CM2CON1
_CM2CON1	set	0x114
	global	_LATA5
_LATA5	set	0x865
	global	_ANSELA
_ANSELA	set	0x18C
	global	_ANSELC
_ANSELC	set	0x18E
	global	_CCP1CON
_CCP1CON	set	0x293
	global	_CCPR1H
_CCPR1H	set	0x292
	global	_CCPR1L
_CCPR1L	set	0x291
psect	text0,local,class=CODE,delta=2,merge=1
; #config settings
	file	"/opt/microchip/xc8/v1.33/include/pic16f1704.h"
	line	5315
global __ptext0
__ptext0:	;psect for function _system_init
global __CFG_CLKOUTEN$OFF
__CFG_CLKOUTEN$OFF equ 0x0
global __CFG_WDTE$OFF
__CFG_WDTE$OFF equ 0x0
global __CFG_MCLRE$ON
__CFG_MCLRE$ON equ 0x0
global __CFG_FOSC$INTOSC
__CFG_FOSC$INTOSC equ 0x0
global __CFG_LVP$ON
__CFG_LVP$ON equ 0x0
global __CFG_PLLEN$OFF
__CFG_PLLEN$OFF equ 0x0
	file	"main.as"
	line	#
psect cinit,class=CODE,delta=2
global start_initialization
start_initialization:

global __initialization
__initialization:
psect cinit,class=CODE,delta=2,merge=1
global end_of_initialization,__end_of__initialization

;End of C runtime variable initialization code

end_of_initialization:
__end_of__initialization:	bcf	pic14e$flags,0 ;clear compiler interrupt flag
movlb 0
ljmp _main	;jump to C main() function
psect	cstackCOMMON,class=COMMON,space=1,noexec
global __pcstackCOMMON
__pcstackCOMMON:
?_system_init:	; 0 bytes @ 0x0
??_system_init:	; 0 bytes @ 0x0
?_timer_init:	; 0 bytes @ 0x0
??_timer_init:	; 0 bytes @ 0x0
?_main:	; 0 bytes @ 0x0
??_main:	; 0 bytes @ 0x0
;!
;!Data Sizes:
;!    Strings     0
;!    Constant    0
;!    Data        0
;!    BSS         0
;!    Persistent  0
;!    Stack       0
;!
;!Auto Spaces:
;!    Space          Size  Autos    Used
;!    COMMON           14      0       0
;!    BANK0            80      0       0
;!    BANK1            80      0       0
;!    BANK2            80      0       0
;!    BANK3            80      0       0
;!    BANK4            80      0       0
;!    BANK5            80      0       0
;!    BANK6            16      0       0

;!
;!Pointer List with Targets:
;!
;!    None.


;!
;!Critical Paths under _main in COMMON
;!
;!    None.
;!
;!Critical Paths under _main in BANK0
;!
;!    None.
;!
;!Critical Paths under _main in BANK1
;!
;!    None.
;!
;!Critical Paths under _main in BANK2
;!
;!    None.
;!
;!Critical Paths under _main in BANK3
;!
;!    None.
;!
;!Critical Paths under _main in BANK4
;!
;!    None.
;!
;!Critical Paths under _main in BANK5
;!
;!    None.
;!
;!Critical Paths under _main in BANK6
;!
;!    None.

;;
;;Main: autosize = 0, tempsize = 0, incstack = 0, save=0
;;

;!
;!Call Graph Tables:
;!
;! ---------------------------------------------------------------------------------
;! (Depth) Function   	        Calls       Base Space   Used Autos Params    Refs
;! ---------------------------------------------------------------------------------
;! (0) _main                                                 0     0      0       0
;!                        _system_init
;!                         _timer_init
;! ---------------------------------------------------------------------------------
;! (1) _timer_init                                           0     0      0       0
;! ---------------------------------------------------------------------------------
;! (1) _system_init                                          0     0      0       0
;! ---------------------------------------------------------------------------------
;! Estimated maximum stack depth 1
;! ---------------------------------------------------------------------------------
;!
;! Call Graph Graphs:
;!
;! _main (ROOT)
;!   _system_init
;!   _timer_init
;!

;! Address spaces:

;!Name               Size   Autos  Total    Cost      Usage
;!BIGRAM             1F0      0       0       0        0.0%
;!NULL                 0      0       0       0        0.0%
;!CODE                 0      0       0       0        0.0%
;!BITCOMMON            E      0       0       1        0.0%
;!BITSFR0              0      0       0       1        0.0%
;!SFR0                 0      0       0       1        0.0%
;!COMMON               E      0       0       2        0.0%
;!BITSFR1              0      0       0       2        0.0%
;!SFR1                 0      0       0       2        0.0%
;!BITSFR2              0      0       0       3        0.0%
;!SFR2                 0      0       0       3        0.0%
;!STACK                0      0       0       3        0.0%
;!BITSFR3              0      0       0       4        0.0%
;!SFR3                 0      0       0       4        0.0%
;!ABS                  0      0       0       4        0.0%
;!BITBANK0            50      0       0       5        0.0%
;!BITSFR4              0      0       0       5        0.0%
;!SFR4                 0      0       0       5        0.0%
;!BANK0               50      0       0       6        0.0%
;!BITSFR5              0      0       0       6        0.0%
;!SFR5                 0      0       0       6        0.0%
;!BITBANK1            50      0       0       7        0.0%
;!BITSFR6              0      0       0       7        0.0%
;!SFR6                 0      0       0       7        0.0%
;!BANK1               50      0       0       8        0.0%
;!BITSFR7              0      0       0       8        0.0%
;!SFR7                 0      0       0       8        0.0%
;!BITBANK2            50      0       0       9        0.0%
;!BITSFR8              0      0       0       9        0.0%
;!SFR8                 0      0       0       9        0.0%
;!BANK2               50      0       0      10        0.0%
;!BITSFR9              0      0       0      10        0.0%
;!SFR9                 0      0       0      10        0.0%
;!BITBANK3            50      0       0      11        0.0%
;!BITSFR10             0      0       0      11        0.0%
;!SFR10                0      0       0      11        0.0%
;!BANK3               50      0       0      12        0.0%
;!BITSFR11             0      0       0      12        0.0%
;!SFR11                0      0       0      12        0.0%
;!BITBANK4            50      0       0      13        0.0%
;!BITSFR12             0      0       0      13        0.0%
;!SFR12                0      0       0      13        0.0%
;!BANK4               50      0       0      14        0.0%
;!BITSFR13             0      0       0      14        0.0%
;!SFR13                0      0       0      14        0.0%
;!BITBANK5            50      0       0      15        0.0%
;!BITSFR14             0      0       0      15        0.0%
;!SFR14                0      0       0      15        0.0%
;!BANK5               50      0       0      16        0.0%
;!BITSFR15             0      0       0      16        0.0%
;!SFR15                0      0       0      16        0.0%
;!BITBANK6            10      0       0      17        0.0%
;!BITSFR16             0      0       0      17        0.0%
;!SFR16                0      0       0      17        0.0%
;!BANK6               10      0       0      18        0.0%
;!BITSFR17             0      0       0      18        0.0%
;!SFR17                0      0       0      18        0.0%
;!BITSFR18             0      0       0      19        0.0%
;!SFR18                0      0       0      19        0.0%
;!DATA                 0      0       0      19        0.0%
;!BITSFR19             0      0       0      20        0.0%
;!SFR19                0      0       0      20        0.0%
;!BITSFR20             0      0       0      21        0.0%
;!SFR20                0      0       0      21        0.0%
;!BITSFR21             0      0       0      22        0.0%
;!SFR21                0      0       0      22        0.0%
;!BITSFR22             0      0       0      23        0.0%
;!SFR22                0      0       0      23        0.0%
;!BITSFR23             0      0       0      24        0.0%
;!SFR23                0      0       0      24        0.0%
;!BITSFR24             0      0       0      25        0.0%
;!SFR24                0      0       0      25        0.0%
;!BITSFR25             0      0       0      26        0.0%
;!SFR25                0      0       0      26        0.0%
;!BITSFR26             0      0       0      27        0.0%
;!SFR26                0      0       0      27        0.0%
;!BITSFR27             0      0       0      28        0.0%
;!SFR27                0      0       0      28        0.0%
;!BITSFR28             0      0       0      29        0.0%
;!SFR28                0      0       0      29        0.0%
;!BITSFR29             0      0       0      30        0.0%
;!SFR29                0      0       0      30        0.0%
;!BITSFR30             0      0       0      31        0.0%
;!SFR30                0      0       0      31        0.0%
;!BITSFR31             0      0       0      32        0.0%
;!SFR31                0      0       0      32        0.0%

	global	_main

;; *************** function _main *****************
;; Defined at:
;;		line 51 in file "main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2, status,0, pclath, cstack
;; Tracked objects:
;;		On entry : 17F/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6
;;      Params:         0       0       0       0       0       0       0       0
;;      Locals:         0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0
;;      Totals:         0       0       0       0       0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels required when called:    1
;; This function calls:
;;		_system_init
;;		_timer_init
;; This function is called by:
;;		Startup code after reset
;; This function uses a non-reentrant model
;;
psect	maintext,global,class=CODE,delta=2,merge=1,split=1
	file	"main.c"
	line	51
global __pmaintext
__pmaintext:	;psect for function _main
psect	maintext
	file	"main.c"
	line	51
	global	__size_of_main
	__size_of_main	equ	__end_of_main-_main
	
_main:	
;incstack = 0
	opt	stack 15
; Regs used in _main: [wreg+status,2+status,0+pclath+cstack]
	line	52
	
l477:	
;main.c: 52: system_init();
	fcall	_system_init
	line	53
;main.c: 53: timer_init();
	fcall	_timer_init
	goto	l479
	line	55
;main.c: 55: while(1) {
	
l55:	
	line	56
	
l479:	
;main.c: 56: LATA5 = 1;
	movlb 2	; select bank2
	bsf	(2149/8)^0100h,(2149)&7	;volatile
	line	57
	
l481:	
;main.c: 57: LATA5 = 1;
	bsf	(2149/8)^0100h,(2149)&7	;volatile
	line	58
	
l483:	
;main.c: 58: LATA5 = 1;
	bsf	(2149/8)^0100h,(2149)&7	;volatile
	line	59
	
l485:	
;main.c: 59: LATA5 = 1;
	bsf	(2149/8)^0100h,(2149)&7	;volatile
	line	60
	
l487:	
;main.c: 60: LATA5 = 0;
	bcf	(2149/8)^0100h,(2149)&7	;volatile
	goto	l479
	line	61
	
l56:	
	line	55
	goto	l479
	
l57:	
	line	62
	
l58:	
	global	start
	ljmp	start
	opt stack 0
GLOBAL	__end_of_main
	__end_of_main:
	signat	_main,88
	global	_timer_init

;; *************** function _timer_init *****************
;; Defined at:
;;		line 37 in file "main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6
;;      Params:         0       0       0       0       0       0       0       0
;;      Locals:         0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0
;;      Totals:         0       0       0       0       0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text1,local,class=CODE,delta=2,merge=1
	line	37
global __ptext1
__ptext1:	;psect for function _timer_init
psect	text1
	file	"main.c"
	line	37
	global	__size_of_timer_init
	__size_of_timer_init	equ	__end_of_timer_init-_timer_init
	
_timer_init:	
;incstack = 0
	opt	stack 15
; Regs used in _timer_init: [wreg+status,2]
	line	38
	
l469:	
;main.c: 38: T1CON = 0b00110000;
	movlw	(030h)
	movlb 0	; select bank0
	movwf	(24)	;volatile
	line	41
	
l471:	
;main.c: 41: T1GCONbits.TMR1GE = 0;
	bcf	(25),7	;volatile
	line	42
	
l473:	
;main.c: 42: TMR1H = 0;
	clrf	(23)	;volatile
	line	43
	
l475:	
;main.c: 43: TMR1L = 0;
	clrf	(22)	;volatile
	line	44
;main.c: 44: CCP1CON = 0b00000010;
	movlw	(02h)
	movlb 5	; select bank5
	movwf	(659)^0280h	;volatile
	line	47
;main.c: 47: CCPR1H = 0xFF;
	movlw	(0FFh)
	movwf	(658)^0280h	;volatile
	line	48
;main.c: 48: CCPR1L = 0xFF;
	movlw	(0FFh)
	movwf	(657)^0280h	;volatile
	line	49
	
l52:	
	return
	opt stack 0
GLOBAL	__end_of_timer_init
	__end_of_timer_init:
	signat	_timer_init,88
	global	_system_init

;; *************** function _system_init *****************
;; Defined at:
;;		line 8 in file "main.c"
;; Parameters:    Size  Location     Type
;;		None
;; Auto vars:     Size  Location     Type
;;		None
;; Return value:  Size  Location     Type
;;		None               void
;; Registers used:
;;		wreg, status,2
;; Tracked objects:
;;		On entry : 0/0
;;		On exit  : 0/0
;;		Unchanged: 0/0
;; Data sizes:     COMMON   BANK0   BANK1   BANK2   BANK3   BANK4   BANK5   BANK6
;;      Params:         0       0       0       0       0       0       0       0
;;      Locals:         0       0       0       0       0       0       0       0
;;      Temps:          0       0       0       0       0       0       0       0
;;      Totals:         0       0       0       0       0       0       0       0
;;Total ram usage:        0 bytes
;; Hardware stack levels used:    1
;; This function calls:
;;		Nothing
;; This function is called by:
;;		_main
;; This function uses a non-reentrant model
;;
psect	text2,local,class=CODE,delta=2,merge=1
	line	8
global __ptext2
__ptext2:	;psect for function _system_init
psect	text2
	file	"main.c"
	line	8
	global	__size_of_system_init
	__size_of_system_init	equ	__end_of_system_init-_system_init
	
_system_init:	
;incstack = 0
	opt	stack 15
; Regs used in _system_init: [wreg+status,2]
	line	10
	
l431:	
;main.c: 10: OSCCON = 0b11111000;
	movlw	(0F8h)
	movlb 1	; select bank1
	movwf	(153)^080h	;volatile
	line	14
	
l433:	
;main.c: 14: ANSELA = 0;
	movlb 3	; select bank3
	clrf	(396)^0180h	;volatile
	line	15
	
l435:	
;main.c: 15: ANSELC = 0;
	clrf	(398)^0180h	;volatile
	line	16
	
l437:	
;main.c: 16: ADCON0 = 0;
	movlb 1	; select bank1
	clrf	(157)^080h	;volatile
	line	17
	
l439:	
;main.c: 17: ADCON1 = 0;
	clrf	(158)^080h	;volatile
	line	18
	
l441:	
;main.c: 18: ADCON2 = 0;
	clrf	(159)^080h	;volatile
	line	19
	
l443:	
;main.c: 19: CM1CON0 = 0;
	movlb 2	; select bank2
	clrf	(273)^0100h	;volatile
	line	20
	
l445:	
;main.c: 20: CM1CON1 = 0;
	clrf	(274)^0100h	;volatile
	line	21
	
l447:	
;main.c: 21: CM2CON0 = 0;
	clrf	(275)^0100h	;volatile
	line	22
	
l449:	
;main.c: 22: CM2CON1 = 0;
	clrf	(276)^0100h	;volatile
	line	23
	
l451:	
;main.c: 23: TRISA5 = 0;
	movlb 1	; select bank1
	bcf	(1125/8)^080h,(1125)&7	;volatile
	line	24
	
l453:	
;main.c: 24: TRISC5 = 0;
	bcf	(1141/8)^080h,(1141)&7	;volatile
	line	27
	
l455:	
;main.c: 27: PIR1bits.SSP1IF = 0;
	movlb 0	; select bank0
	bcf	(17),3	;volatile
	line	28
	
l457:	
;main.c: 28: PIR1bits.TMR1IF = 0;
	bcf	(17),0	;volatile
	line	29
	
l459:	
;main.c: 29: PIR1bits.CCP1IF = 0;
	bcf	(17),2	;volatile
	line	31
	
l461:	
;main.c: 31: PIE1bits.CCP1IE = 0;
	movlb 1	; select bank1
	bcf	(145)^080h,2	;volatile
	line	32
	
l463:	
;main.c: 32: PIE1bits.SSP1IE = 1;
	bsf	(145)^080h,3	;volatile
	line	33
	
l465:	
;main.c: 33: INTCONbits.PEIE = 1;
	bsf	(11),6	;volatile
	line	34
	
l467:	
;main.c: 34: INTCONbits.GIE = 1;
	bsf	(11),7	;volatile
	line	35
	
l49:	
	return
	opt stack 0
GLOBAL	__end_of_system_init
	__end_of_system_init:
	signat	_system_init,88
global	___latbits
___latbits	equ	1
	global	btemp
	btemp set 07Eh

	DABS	1,126,2	;btemp
	global	pic14e$flags
	pic14e$flags	set btemp
	global	wtemp
	global	wtemp0
	wtemp set btemp
	wtemp0 set btemp
	global	wtemp1
	wtemp1 set btemp+2
	global	wtemp2
	wtemp2 set btemp+4
	global	wtemp3
	wtemp3 set btemp+6
	global	wtemp4
	wtemp4 set btemp+8
	global	wtemp5
	wtemp5 set btemp+10
	global	wtemp6
	wtemp6 set btemp+1
	global	ttemp
	global	ttemp0
	ttemp set btemp
	ttemp0 set btemp
	global	ttemp1
	ttemp1 set btemp+3
	global	ttemp2
	ttemp2 set btemp+6
	global	ttemp3
	ttemp3 set btemp+9
	global	ttemp4
	ttemp4 set btemp+1
	global	ltemp
	global	ltemp0
	ltemp set btemp
	ltemp0 set btemp
	global	ltemp1
	ltemp1 set btemp+4
	global	ltemp2
	ltemp2 set btemp+8
	global	ltemp3
	ltemp3 set btemp+2
	end
