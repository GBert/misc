;   External Oscillator Selection:
;	HS (crystal oscillator) above 8 MHz; PFM set to high power
		CONFIG	FEXTOSC = HS

;   Reset Oscillator Selection: 
;	EXT_SC with 4x PLL, with EXTOSC operating per FEXTOSC bits
		CONFIG	RSTOSC = EXTOSC_4PLL

;   Clock out Enable bit:
		CONFIG	CLKOUTEN = OFF
;
;   PRLOCKED One-Way Set Enable bit:
		CONFIG	PR1WAY = OFF

;   Clock Switch Enable bit:
		CONFIG	CSWEN = ON           Writing to NOSC and NDIV is allowed

;   Fail-Safe Clock Monitor Enable bit:
		CONFIG	FCMEN = OFF

;   MCLR Enable bit:
;	 If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR
		CONFIG	MCLRE = EXTMCLR

;   Power-up timer selection bits:
		CONFIG	PWRTS = PWRT_OFF
;
;   Multi-vector enable bit:
;	Interrupt contoller does not use vector table to prioritze interrupts
		CONFIG	MVECEN = OFF

;
;   IVTLOCK bit One-way set enable bit:
;     IVT1WAY = OFF        IVTLOCK bit can be cleared and set repeatedly
;     IVT1WAY = ON         IVTLOCK bit can be cleared and set only once
;
;   Low Power BOR Enable bit:
		CONFIG	LPBOREN = OFF
;
;   Brown-out Reset Enable bits:
		CONFIG	BOREN = OFF
;
;   Brown-out Reset Voltage Selection bits:
;     BORV = VBOR_2P85     Brown-out Reset Voltage (VBOR) set to 2.8V
;     BORV = VBOR_2P7      Brown-out Reset Voltage (VBOR) set to 2.7V
;     BORV = VBOR_245      Brown-out Reset Voltage (VBOR) set to 2.45V
;     BORV = VBOR_2P45     Brown-out Reset Voltage (VBOR) set to 2.45V
;
;   ZCD Disable bit:
;	ZCD disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON
		CONFIG	ZCD = OFF

;   PPSLOCK bit One-Way Set Enable bit:
;	PPSLOCK bit can be set and cleared repeatedly (subject to the unlock sequence)
		CONFIG	PPS1WAY = OFF

;   Stack Full/Underflow Reset Enable bit:
;	Stack full/underflow will cause Reset
		CONFIG	STVREN = ON
;
;   Debugger Enable bit:
		CONFIG	DEBUG = OFF
;
;   Extended Instruction Set Enable bit:
		CONFIG	XINST = OFF
;
;   WDT Period selection bits:
;     WDTCPS = WDTCPS_0    Divider ratio 1:32
;     WDTCPS = WDTCPS_1    Divider ratio 1:64
;     WDTCPS = WDTCPS_2    Divider ratio 1:128
;     WDTCPS = WDTCPS_3    Divider ratio 1:256
;     WDTCPS = WDTCPS_4    Divider ratio 1:512
;	...

;   WDT operating mode:
		CONFIG	WDTE = OFF

;   WDT Window Select bits:
;     WDTCWS = WDTCWS_0    window delay = 87.5; no software control; keyed access required
;     WDTCWS = WDTCWS_1    window delay = 75 percent of time; no software control; keyed access required
;     WDTCWS = WDTCWS_2    window delay = 62.5 percent of time; no software control; keyed access required
;     WDTCWS = WDTCWS_3    window delay = 50 percent of time; no software control; keyed access required
;     WDTCWS = WDTCWS_4    window delay = 37.5 percent of time; no software control; keyed access required
;     WDTCWS = WDTCWS_5    window delay = 25 percent of time; no software control; keyed access required
;     WDTCWS = WDTCWS_6    window always open (100%); no software control; keyed access required
;     WDTCWS = WDTCWS_7    window always open (100%); software control; keyed access not required

;   WDT input clock selector:
;     WDTCCS = LFINTOSC    WDT reference clock is the 31.0 kHz LFINTOSC
;     WDTCCS = MFINTOSC    WDT reference clock is the 32kHz MFINTOSC output
;     WDTCCS = SOSC        WDT reference clock is SOSC
;     WDTCCS = SC          Software Control

;   Boot Block Size selection bits:
;     BBSIZE = BBSIZE_8192 Boot Block size is 8192 words
;     BBSIZE = BBSIZE_4096 Boot Block size is 4096 words
;     BBSIZE = BBSIZE_2048 Boot Block size is 2048 words
;     BBSIZE = BBSIZE_1024 Boot Block size is 1024 words
;     BBSIZE = BBSIZE_512  Boot Block size is 512 words

;   Boot Block enable bit:
		CONFIG	BBEN = OFF

;   Storage Area Flash enable bit:
		CONFIG	SAFEN = OFF
;
;   Application Block write protection bit:
		CONFIG	WRTAPP = OFF         Application Block not write protected

;   Configuration Register Write Protection bit:
		CONFIG	WRTB = OFF

;   Boot Block Write Protection bit:
		CONFIG	WRTC = OFF

;   Data EEPROM Write Protection bit:
		CONFIG	WRTD = OFF

;   SAF Write protection bit:
		CONFIG	WRTSAF = OFF

;   Low Voltage Programming Enable bit:
		CONFIG	LVP = ON

;   PFM and Data EEPROM Code Protection bit:
		CONFIG	CP = OFF

