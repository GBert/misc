        processor p16f628
        radix dec

	ERRORLEVEL  -302,-1302

        include p16f628.inc

; The recognition of labels and registers is not always good, therefore
; be treated cautiously the results.

        CONFIG  WDTE  = OFF
        CONFIG  PWRTE = ON
        CONFIG  FOSC  = HS
        CONFIG  MCLRE = OFF
        CONFIG  BOREN = ON
        CONFIG  LVP   = OFF
        CONFIG  CPD   = OFF
        CONFIG  CP    = OFF


;===============================================================================

#DEFINE	RXINPUT	RA5

;===============================================================================
; DATA address definitions

Common_RAM      equ     0x0070                              ; size: 16 bytes
w_temp		equ	0x70
status_temp	equ	0x71
pclath_temp	equ	0x72
fsr_temp	equ	0x73

;===============================================================================
; CODE area

        ; code

        org     __CODE_START                                ; address: 0x0000

vector_reset:                                               ; address: 0x0000

        clrf    STATUS                                      ; reg: 0x003
        clrf    PCLATH                                      ; reg: 0x00a
        clrf    INTCON                                      ; reg: 0x00b
        goto    label_022

vector_int:                                                 ; address: 0x0004

        movwf   w_temp
        swapf   STATUS, W
        clrf    STATUS
        movwf   status_temp
        movlw   0x5e			;  PS 1:128 
        movwf   TMR0                                        ; reg: 0x001
        bcf     INTCON, T0IF                                ; reg: 0x00b, bit: 2
        movf    FSR, W                                      ; reg: 0x004
        movwf   fsr_temp
        movf    0x37, W                                     ; reg: 0x037
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_002
        movf    0x3d, W                                     ; reg: 0x03d
        subwf   0x3e, W                                     ; reg: 0x03e
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_006
        movlw   0x01
        movwf   0x37                                        ; reg: 0x037
        goto    label_006

label_002:                                                  ; address: 0x0017

        decf    0x36, F                                     ; reg: 0x036
        movf    0x36, W                                     ; reg: 0x036
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_006
        movlw   0x03
        movwf   0x36                                        ; reg: 0x036
        movf    0x37, W                                     ; reg: 0x037
        sublw   0x01
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_003
        bcf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        incf    0x37, F                                     ; reg: 0x037
        goto    label_006

label_003:                                                  ; address: 0x0024

        movf    0x37, W                                     ; reg: 0x037
        sublw   0x09
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_004
        movf    0x3d, W                                     ; reg: 0x03d
        movwf   FSR                                         ; reg: 0x004
        btfsc   INDF, 0x0                                   ; reg: 0x000
        bsf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        btfss   INDF, 0x0                                   ; reg: 0x000
        bcf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        rrf     INDF, F                                     ; reg: 0x000
        incf    0x37, F                                     ; reg: 0x037
        goto    label_006

label_004:                                                  ; address: 0x0031

        movf    0x37, W                                     ; reg: 0x037
        sublw   0x0a
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_005
        bsf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        incf    0x37, F                                     ; reg: 0x037
        goto    label_006

label_005:                                                  ; address: 0x0038

        clrf    0x37                                        ; reg: 0x037
        incf    0x3d, F                                     ; reg: 0x03d
        movf    0x3d, W                                     ; reg: 0x03d
        sublw   0xd0
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_006
        movlw   0xa0
        movwf   0x3d                                        ; reg: 0x03d

label_006:                                                  ; address: 0x0040

        movf    0x39, W                                     ; reg: 0x039
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_007
        btfsc   PORTA, RXINPUT
        goto    label_010
        movlw   0x04
        movwf   0x38                                        ; reg: 0x038
        movlw   0x01
        movwf   0x39                                        ; reg: 0x039
        clrf    0x3a                                        ; reg: 0x03a
        goto    label_010

label_007:                                                  ; address: 0x004b

        decf    0x38, F                                     ; reg: 0x038
        movf    0x38, W                                     ; reg: 0x038
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_010
        movlw   0x03
        movwf   0x38                                        ; reg: 0x038
        movf    0x39, W                                     ; reg: 0x039
        sublw   0x09
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_009
        clrf    0x39                                        ; reg: 0x039
        btfss   PORTA, RXINPUT
        goto    label_010
        movf    0x3c, W                                     ; reg: 0x03c
        movwf   FSR                                         ; reg: 0x004
        movf    0x3a, W                                     ; reg: 0x03a
        movwf   INDF                                        ; reg: 0x000
        incf    0x3c, F                                     ; reg: 0x03c
        movf    0x3c, W                                     ; reg: 0x03c
        sublw   0xde
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_008
        movlw   0xd0
        movwf   0x3c                                        ; reg: 0x03c

label_008:                                                  ; address: 0x0063

        movf    0x3c, W                                     ; reg: 0x03c
        subwf   0x3b, W                                     ; reg: 0x03b
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_010
        bsf     0x4a, 0x1                                   ; reg: 0x04a
        bsf     PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_010

label_009:                                                  ; address: 0x006a

        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        btfsc   PORTA, RXINPUT
        bsf     STATUS, C                                   ; reg: 0x003, bit: 0
        rrf     0x3a, F                                     ; reg: 0x03a
        incf    0x39, F                                     ; reg: 0x039

label_010:                                                  ; address: 0x006f

        btfss   PIR1, RCIF	; check if data received
        goto    label_017	; no data received
        btfss   RCSTA, OERR	; overrrun error ?
        btfsc   RCSTA, FERR	; framing error
        goto    label_016
        btfss   RCSTA, RX9D	; 9th bit set ?
        goto    label_012
        bcf     0x46, 0x2                                   ; reg: 0x046
        clrf    0x45                                        ; reg: 0x045
        movf    RCREG, W                                    ; reg: 0x01a
        movwf   0x40                                        ; reg: 0x040
        btfsc   0x46, 0x3                                   ; reg: 0x046
        goto    label_020
        xorlw   0x60
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_011
        xorlw   0xc0
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_011
        movf    0x40, W                                     ; reg: 0x040
        xorwf   0x41, W                                     ; reg: 0x041
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_015
        xorlw   0xa0
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_011
        xorlw   0x60
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_013
        goto    label_020

label_011:                                                  ; address: 0x008d

        clrf    0x44                                        ; reg: 0x044
        bsf     0x46, 0x2                                   ; reg: 0x046
        goto    label_020

label_012:                                                  ; address: 0x0090

        movlw   0x20
        addwf   0x45, W                                     ; reg: 0x045
        movwf   FSR                                         ; reg: 0x004
        movf    RCREG, W                                    ; reg: 0x01a
        btfss   0x46, 0x2                                   ; reg: 0x046
        goto    label_020
        movwf   INDF                                        ; reg: 0x000
        xorwf   0x44, F                                     ; reg: 0x044
        incf    0x45, F                                     ; reg: 0x045
        movlw   0x20
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        andlw   0x0f
        addlw   0x02
        xorwf   0x45, W                                     ; reg: 0x045
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_020
        bcf     0x46, 0x2                                   ; reg: 0x046
        bcf     0x46, 0x4                                   ; reg: 0x046
        movf    0x44, W                                     ; reg: 0x044
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        bsf     0x46, 0x3                                   ; reg: 0x046
        goto    label_020

label_013:                                                  ; address: 0x00a7

        bsf     PORTB, RB0
        movlw   0x20		; send space
        movwf   TXREG                                       ; reg: 0x019

label_014:                                                  ; address: 0x00aa

        btfss   PIR1, TXIF                                  ; reg: 0x00c, bit: 4
        goto    label_014
        movlw   0x20
        movwf   TXREG                                       ; reg: 0x019
        bcf     0x46, 0x1                                   ; reg: 0x046
        goto    label_020

label_015:                                                  ; address: 0x00b0

        btfsc   0x46, 0x4                                   ; reg: 0x046
        bsf     0x46, 0x5                                   ; reg: 0x046
        bcf     0x46, 0x4                                   ; reg: 0x046
        btfss   0x46, 0x0                                   ; reg: 0x046
        goto    label_020
        bsf     PORTB, RB0                                  ; reg: 0x006, bit: 0
        clrf    0x42                                        ; reg: 0x042
        bsf     0x46, 0x1                                   ; reg: 0x046
        goto    label_018

label_016:                                                  ; address: 0x00b9

        movf    RCREG, W                                    ; reg: 0x01a
        bcf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bsf     RCSTA, CREN                                 ; reg: 0x018, bit: 4

label_017:                                                  ; address: 0x00bc

        btfss   PIR1, TXIF                                  ; reg: 0x00c, bit: 4
        goto    label_020
        btfss   0x46, 0x1                                   ; reg: 0x046
        goto    label_019

label_018:                                                  ; address: 0x00c0

        movlw   0xde
        addwf   0x42, W                                     ; reg: 0x042
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   TXREG                                       ; reg: 0x019
        incf    0x42, F                                     ; reg: 0x042
        decfsz  0x43, F                                     ; reg: 0x043
        goto    label_020
        bcf     0x46, 0x0                                   ; reg: 0x046
        bcf     0x46, 0x1                                   ; reg: 0x046
        bsf     0x46, 0x4                                   ; reg: 0x046
        goto    label_020

label_019:                                                  ; address: 0x00cc

        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        btfss   RCSTA, OERR                                 ; reg: 0x018, bit: 1
        goto    label_020
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     PORTB, RB0                                  ; reg: 0x006, bit: 0

label_020:                                                  ; address: 0x00d1

        movf    (Common_RAM + 3), W                         ; reg: 0x073
        movwf   FSR                                         ; reg: 0x004
        swapf   status_temp, W
        movwf   STATUS
        swapf   w_temp, F
        swapf   w_temp, W
        retfie

function_000:                                               ; address: 0x00d8

        clrf    PCLATH                                      ; reg: 0x00a
        movf    0x50, W                                     ; reg: 0x050
        addwf   PCL, F                                      ; reg: 0x002
        goto    label_030
        goto    label_031
        goto    label_021
        goto    label_040

label_021:                                                  ; address: 0x00df

        clrf    PCLATH                                      ; reg: 0x00a
        clrf    0x50                                        ; reg: 0x050
        movf    0x32, W                                     ; reg: 0x032
        xorwf   0x52, W                                     ; reg: 0x052
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_035
        swapf   0x53, W                                     ; reg: 0x053
        andlw   0x0f
        addwf   PCL, F                                      ; reg: 0x002
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_041
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_032
        goto    label_036

label_022:                                                  ; address: 0x00f8

        movlw   0x07		; switch off comaparatos
        movwf   CMCON
        clrf    RCSTA		; stop UART
        movlw   0x06
        movwf   PORTA		; Initialize Port A - A1&A2 high
        movlw   0x00
        movwf   PORTB		; Initialize Port B
        bsf     STATUS, RP0	; Bank 1
        bcf     STATUS, RP1	;
        movlw   0x00
        movwf   PIE1		; disbale all interrupts
        movlw   0xfe
        movwf   TRISB           ; B0 out
        movlw   0xf9
        movwf   TRISA		; A1/A2 out
        movlw   0x88
        movwf   OPTION_REG                                  ; reg: 0x081
        movlw   0x0b
        movwf   PCON                                        ; reg: 0x08e
        bcf     STATUS, RP0	; Bank 0
        bcf     STATUS, RP1
        clrf    TMR0		; clear Timer0
        clrf    CCP1CON		; CCP off
        movlw   0x31		; Prescale 1:8 Stopp	
        movwf   T1CON
        clrf    T2CON		; no Timer2
        movlw   0x20
        movwf   FSR

label_023:                                                  ; address: 0x0114

        clrf    INDF                                        ; reg: 0x000
        incf    FSR, F                                      ; reg: 0x004
        btfss   FSR, 0x7                                    ; reg: 0x004
        goto    label_023
        movlw   0x00
        call    eeprom_read
        movwf   0x4c                                        ; reg: 0x04c
        call    function_007
        call    uart_init
        bcf     PORTB, RB0                                  ; reg: 0x006, bit: 0
        movlw   0xa0
        movwf   INTCON                                      ; reg: 0x00b
        bcf     PORTA, RA1                                  ; reg: 0x005, bit: 1
        clrf    PCLATH                                      ; reg: 0x00a
        bsf     PORTB, RB3                                  ; reg: 0x006, bit: 3
        bsf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        bsf     PORTA, RA3                                  ; reg: 0x005, bit: 3

label_024:                                                  ; address: 0x0125

        btfsc   0x46, 0x3                                   ; reg: 0x046
        call    function_003
        btfsc   0x46, 0x0                                   ; reg: 0x046
        goto    label_025
        call    function_002
        btfss   0x4a, 0x2                                   ; reg: 0x04a
        call    function_000

label_025:                                                  ; address: 0x012c

        btfss   0x46, 0x5                                   ; reg: 0x046
        goto    label_024
        bcf     0x46, 0x5                                   ; reg: 0x046
        movlw   0x01
        call    function_001
        movlw   0x04
        call    function_001
        movlw   0x05
        call    function_001
        goto    label_024

function_001:                                               ; address: 0x0136

        movwf   0x34                                        ; reg: 0x034
        movf    0x3e, W                                     ; reg: 0x03e
        movwf   FSR                                         ; reg: 0x004
        movf    0x34, W                                     ; reg: 0x034
        movwf   INDF                                        ; reg: 0x000
        incf    0x3e, W                                     ; reg: 0x03e
        movwf   0x34                                        ; reg: 0x034
        sublw   0xd0
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_026
        movlw   0xa0
        movwf   0x34                                        ; reg: 0x034

label_026:                                                  ; address: 0x0142

        movf    0x34, W                                     ; reg: 0x034
        subwf   0x3d, W                                     ; reg: 0x03d
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_026
        movf    0x34, W                                     ; reg: 0x034
        movwf   0x3e                                        ; reg: 0x03e
        return

function_002:                                               ; address: 0x0149

        btfsc   0x4a, 0x1                                   ; reg: 0x04a
        goto    label_027
        bsf     0x4a, 0x2                                   ; reg: 0x04a
        movf    0x3b, W                                     ; reg: 0x03b
        subwf   0x3c, W                                     ; reg: 0x03c
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return

label_027:                                                  ; address: 0x0150

        movf    0x3b, W                                     ; reg: 0x03b
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x32                                        ; reg: 0x032
        incf    0x3b, F                                     ; reg: 0x03b
        movf    0x3b, W                                     ; reg: 0x03b
        sublw   0xde
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_028
        movlw   0xd0
        movwf   0x3b                                        ; reg: 0x03b

label_028:                                                  ; address: 0x015b

        bcf     0x4a, 0x1                                   ; reg: 0x04a
        bcf     PORTA, RA1                                  ; reg: 0x005, bit: 1
        bcf     0x4a, 0x2                                   ; reg: 0x04a
        return

function_003:                                               ; address: 0x015f

        movf    0x20, W                                     ; reg: 0x020
        andlw   0x0f
        addlw   0x02
        movwf   0x35                                        ; reg: 0x035
        movlw   0x20
        movwf   0x33                                        ; reg: 0x033

label_029:                                                  ; address: 0x0165

        movf    0x33, W                                     ; reg: 0x033
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        call    function_001
        incf    0x33, F                                     ; reg: 0x033
        decfsz  0x35, F                                     ; reg: 0x035
        goto    label_029
        bcf     0x46, 0x3                                   ; reg: 0x046
        return

label_030:                                                  ; address: 0x016e

        incf    0x50, F                                     ; reg: 0x050
        movf    0x32, W                                     ; reg: 0x032
        movwf   0x53                                        ; reg: 0x053
        movwf   0x52                                        ; reg: 0x052
        andlw   0x0f
        movwf   0x51                                        ; reg: 0x051
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        incf    0x50, F                                     ; reg: 0x050
        return

label_031:                                                  ; address: 0x0177

        movf    0x51, W                                     ; reg: 0x051
        subwf   0x53, W                                     ; reg: 0x053
        andlw   0x07
        addlw   0x54
        movwf   FSR                                         ; reg: 0x004
        movf    0x32, W                                     ; reg: 0x032
        movwf   INDF                                        ; reg: 0x000
        xorwf   0x52, F                                     ; reg: 0x052
        decfsz  0x51, F                                     ; reg: 0x051
        return
        incf    0x50, F                                     ; reg: 0x050
        return

label_032:                                                  ; address: 0x0183

        movf    0x53, W                                     ; reg: 0x053
        call    function_004
        movf    0x53, W                                     ; reg: 0x053
        andlw   0x07
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_034
        movwf   0x51                                        ; reg: 0x051
        movlw   0x54
        movwf   0x52                                        ; reg: 0x052

label_033:                                                  ; address: 0x018c

        movf    0x52, W                                     ; reg: 0x052
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        call    function_005
        incf    0x52, F                                     ; reg: 0x052
        decfsz  0x51, F                                     ; reg: 0x051
        goto    label_033

label_034:                                                  ; address: 0x0193

        call    function_006
        return

label_035:                                                  ; address: 0x0195

        movlw   0x01
        call    function_001
        movlw   0x03
        call    function_001
        movlw   0x02
        goto    function_001

label_036:                                                  ; address: 0x019b

        movlw   0x02
        call    function_001
        movlw   0x30
        call    function_001
        movlw   0x01
        call    function_001
        movlw   0x33
        goto    function_001

function_004:                                               ; address: 0x01a3

        clrf    0x42                                        ; reg: 0x042
        clrf    0x43                                        ; reg: 0x043

function_005:                                               ; address: 0x01a5

        movwf   0x34                                        ; reg: 0x034
        movlw   0xde
        addwf   0x43, W                                     ; reg: 0x043
        movwf   FSR                                         ; reg: 0x004
        movf    0x34, W                                     ; reg: 0x034
        movwf   INDF                                        ; reg: 0x000
        xorwf   0x42, F                                     ; reg: 0x042
        incf    0x43, F                                     ; reg: 0x043
        return

function_006:                                               ; address: 0x01ae

        movf    0x42, W                                     ; reg: 0x042
        call    function_005
        bsf     0x46, 0x0                                   ; reg: 0x046
        return

function_007:                                               ; address: 0x01b2

        movlw   0x01
        call    eeprom_read
        andlw   0x1f
        iorlw   0x40
        movwf   0x41                                        ; reg: 0x041
        movwf   0x34                                        ; reg: 0x034
        swapf   0x34, W                                     ; reg: 0x034
        xorwf   0x34, F                                     ; reg: 0x034
        rrf     0x34, W                                     ; reg: 0x034
        xorwf   0x34, F                                     ; reg: 0x034
        btfsc   0x34, 0x2                                   ; reg: 0x034
        incf    0x34, F                                     ; reg: 0x034
        rrf     0x34, W                                     ; reg: 0x034
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        bsf     0x41, 0x7                                   ; reg: 0x041
        return

eeprom_read:

        bsf     STATUS, RP0
        bcf     STATUS, RP1
        movwf   EEADR                                       ; reg: 0x09b
        bsf     EECON1, RD                                  ; reg: 0x09c, bit: 0
        movf    EEDATA, W                                   ; reg: 0x09a
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        return

function_009:                                               ; address: 0x01c9

        movwf   (Common_RAM + 5)                            ; reg: 0x075
        call    eeprom_read
        xorwf   (Common_RAM + 4), W                         ; reg: 0x074
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     STATUS, RP1                                 ; reg: 0x003, bit: 6
        movf    0x75, W                                     ; reg: 0x0f5
        movwf   EEADR                                       ; reg: 0x09b
        movf    0x74, W                                     ; reg: 0x0f4
        movwf   EEDATA                                      ; reg: 0x09a
        bsf     EECON1, WREN                                ; reg: 0x09c, bit: 2

label_037:                                                  ; address: 0x01d5

        bcf     INTCON, GIE                                 ; reg: 0x00b, bit: 7
        btfsc   INTCON, GIE                                 ; reg: 0x00b, bit: 7
        goto    label_037
        movlw   0x55
        movwf   EECON2                                      ; reg: 0x01d
        movlw   0xaa
        movwf   EECON2                                      ; reg: 0x01d
        bsf     EECON1, 0x1                                 ; reg: 0x01c
        bsf     INTCON, GIE                                 ; reg: 0x00b, bit: 7

label_038:                                                  ; address: 0x01de

        btfsc   EECON1, 0x1                                 ; reg: 0x01c
        goto    label_038
        bcf     EECON1, 0x2                                 ; reg: 0x01c
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        return

uart_init:                                               ; address: 0x01e3

        bcf     PORTB, RB0
        bsf     PORTA, RA1
        clrf    0x46                                        ; reg: 0x046
        clrf    0x45                                        ; reg: 0x045
        bsf     STATUS, RP0	; Bank 1
        bcf     STATUS, RP1
        movlw   0x13		; Baudrate 62500 Hz = 20MHz / 16 / ( 19 +1 )
        movwf   SPBRG
        movlw   0x64		; BRGH = 1  TXEN = 1  TX9 = 1
        movwf   TXSTA
        bcf     STATUS, RP0	; Bank 0
        bcf     STATUS, RP1
        movlw   0xd0		; SPEN = 1  RX9 = 1  SREN = 1 
        movwf   RCSTA
        clrw

label_039:                                                  ; address: 0x01f2

        addlw   0xff
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_039
        movf    RCREG, W                                    ; reg: 0x01a
        movf    RCREG, W                                    ; reg: 0x01a
        movf    RCREG, W                                    ; reg: 0x01a
        bcf     0x4a, 0x1                                   ; reg: 0x04a
        movlw   0xa0
        movwf   0x3d                                        ; reg: 0x03d
        movwf   0x3e                                        ; reg: 0x03e
        movlw   0xd0
        movwf   0x3b                                        ; reg: 0x03b
        movwf   0x3c                                        ; reg: 0x03c
        clrf    0x37                                        ; reg: 0x037
        clrf    0x39                                        ; reg: 0x039
        movlw   0x01
        movwf   0x36                                        ; reg: 0x036
        bsf     PORTA, RA2                                  ; reg: 0x005, bit: 2

label_040:                                                  ; address: 0x0204

        clrf    0x50                                        ; reg: 0x050
        return

label_041:                                                  ; address: 0x0206

        movf    0x53, W                                     ; reg: 0x053
        xorlw   0x64
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_032
        movf    0x54, W                                     ; reg: 0x054
        xorlw   0x63
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_032
        movf    0x55, W                                     ; reg: 0x055
        xorlw   0x63
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_032
        movf    0x56, W                                     ; reg: 0x056
        xorlw   0x64
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_032
        movf    0x57, W                                     ; reg: 0x057
        xorlw   0x63
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_032

label_042:                                                  ; address: 0x021a

        call    function_011

label_043:                                                  ; address: 0x021b

        call    function_002
        btfsc   0x4a, 0x2                                   ; reg: 0x04a
        goto    label_043
        movf    0x32, W                                     ; reg: 0x032
        movwf   0x5a                                        ; reg: 0x05a
        call    function_001
        movf    0x5a, W                                     ; reg: 0x05a
        xorlw   0x61
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_045
        xorlw   0x03
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_044
        movlw   0x01
        call    function_017
        movlw   0x04
        call    function_017
        goto    label_043

label_044:                                                  ; address: 0x022d

        movlw   0x01
        call    function_017
        movlw   0x05
        call    function_017
        movlw   0x01
        call    function_017
        movlw   0x01
        call    function_017
        return

label_045:                                                  ; address: 0x0236

        movlw   0x01
        call    function_017
        movlw   0x02
        call    function_017
        clrf    0x58                                        ; reg: 0x058
        clrf    0x59                                        ; reg: 0x059

label_046:                                                  ; address: 0x023c

        call    function_002
        btfsc   0x4a, 0x2                                   ; reg: 0x04a
        goto    label_046
        movf    0x32, W                                     ; reg: 0x032
        movwf   0x5a                                        ; reg: 0x05a
        call    function_001
        movf    0x5a, W                                     ; reg: 0x05a
        xorlw   0x0d
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_047
        movf    0x59, W                                     ; reg: 0x059
        movwf   0x58                                        ; reg: 0x058
        movf    0x5a, W                                     ; reg: 0x05a
        movwf   0x59                                        ; reg: 0x059
        goto    label_046

label_047:                                                  ; address: 0x024b

        call    function_015
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_042
        movwf   (Common_RAM + 4)                            ; reg: 0x074
        andlw   0xe0
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_042
        movlw   0x01
        call    function_009
        goto    label_042

function_011:                                               ; address: 0x0255

        movlw   0x01
        call    function_017
        movlw   0x00
        call    function_017
        movlw   0x01
        call    function_017
        movlw   0x02
        call    function_017
        movlw   0x01
        call    function_012
        movlw   0x01
        call    function_017
        movlw   0x03
        call    function_017
        movlw   0x01
        call    function_017
        movlw   0x01
        call    function_017
        movlw   0x04
        call    function_017
        return

function_012:                                               ; address: 0x026a

        call    eeprom_read
        movwf   0x62                                        ; reg: 0x062
        call    function_013
        movf    0x61, W                                     ; reg: 0x061
        swapf   0x61, W                                     ; reg: 0x061
        andlw   0x0f
        iorlw   0x30
        call    function_001
        movf    0x61, W                                     ; reg: 0x061
        andlw   0x0f
        iorlw   0x30
        call    function_001
        return

function_013:                                               ; address: 0x0277

        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        movlw   0x08
        movwf   0x60                                        ; reg: 0x060
        clrf    0x61                                        ; reg: 0x061

label_048:                                                  ; address: 0x027b

        rlf     0x62, F                                     ; reg: 0x062
        rlf     0x61, F                                     ; reg: 0x061
        decfsz  0x60, F                                     ; reg: 0x060
        goto    label_049
        return

label_049:                                                  ; address: 0x0280

        movlw   0x61
        call    function_014
        goto    label_048

function_014:                                               ; address: 0x0283

        movwf   FSR                                         ; reg: 0x004
        movlw   0x03
        addwf   INDF, W                                     ; reg: 0x000
        movwf   0x34                                        ; reg: 0x034
        btfsc   0x34, 0x3                                   ; reg: 0x034
        movwf   INDF                                        ; reg: 0x000
        movlw   0x30
        addwf   INDF, W                                     ; reg: 0x000
        movwf   0x34                                        ; reg: 0x034
        btfsc   0x34, 0x7                                   ; reg: 0x034
        movwf   INDF                                        ; reg: 0x000
        return

function_015:                                               ; address: 0x028f

        movlw   0x0f
        andwf   0x59, F                                     ; reg: 0x059
        andwf   0x58, F                                     ; reg: 0x058
        movf    0x58, W                                     ; reg: 0x058
        addwf   0x58, W                                     ; reg: 0x058
        movwf   0x5a                                        ; reg: 0x05a
        addwf   0x5a, W                                     ; reg: 0x05a
        movwf   0x58                                        ; reg: 0x058
        addwf   0x58, W                                     ; reg: 0x058
        addwf   0x5a, W                                     ; reg: 0x05a
        addwf   0x59, W                                     ; reg: 0x059
        return

        ; code

        org     0x0600

function_016:                                               ; address: 0x0600

        addwf   PCL, F                                      ; reg: 0x002
        retlw   0x06
        retlw   0x26
        retlw   0x06
        retlw   0x45
        retlw   0x06
        retlw   0x48
        retlw   0x06
        retlw   0x5c
        retlw   0x06
        retlw   0x64
        retlw   0x06
        retlw   0x74

function_017:                                               ; address: 0x060d

        movwf   0x62                                        ; reg: 0x062
        movlw   0x06
        movwf   PCLATH                                      ; reg: 0x00a
        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        rlf     0x62, F                                     ; reg: 0x062
        movf    0x62, W                                     ; reg: 0x062
        call    function_016
        movwf   0x63                                        ; reg: 0x063
        incf    0x62, W                                     ; reg: 0x062
        call    function_016
        movwf   0x62                                        ; reg: 0x062

label_050:                                                  ; address: 0x0618

        call    function_018
        xorlw   0x80
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        xorlw   0x80
        call    function_001
        incf    0x62, F                                     ; reg: 0x062
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        incf    0x63, F                                     ; reg: 0x063
        goto    label_050

function_018:                                               ; address: 0x0622

        movf    0x63, W                                     ; reg: 0x063
        movwf   PCLATH                                      ; reg: 0x00a
        movf    0x62, W                                     ; reg: 0x062
        movwf   PCL                                         ; reg: 0x002
        retlw   0x47
        retlw   0x65
        retlw   0x6e
        retlw   0x4c
        retlw   0x49
        retlw   0x20
        retlw   0x76
        retlw   0x2e
        retlw   0x32
        retlw   0x41
        retlw   0x20
        retlw   0x30
        retlw   0x31
        retlw   0x2f
        retlw   0x30
        retlw   0x36
        retlw   0x2f
        retlw   0x30
        retlw   0x37
        retlw   0x20
        retlw   0x62
        retlw   0x79
        retlw   0x20
        retlw   0x46
        retlw   0x2e
        retlw   0x43
        retlw   0x61
        retlw   0xf1
        retlw   0x61
        retlw   0x64
        retlw   0x61
        retlw   0x0d
        retlw   0x0a
        retlw   0x80
        retlw   0x61
        retlw   0x29
        retlw   0x20
        retlw   0x58
        retlw   0x62
        retlw   0x75
        retlw   0x73
        retlw   0x20
        retlw   0x41
        retlw   0x64
        retlw   0x64
        retlw   0x72
        retlw   0x65
        retlw   0x73
        retlw   0x73
        retlw   0x3a
        retlw   0x20
        retlw   0x20
        retlw   0x20
        retlw   0x80
        retlw   0x62
        retlw   0x29
        retlw   0x20
        retlw   0x45
        retlw   0x78
        retlw   0x69
        retlw   0x74
        retlw   0x80
        retlw   0x53
        retlw   0x65
        retlw   0x6c
        retlw   0x65
        retlw   0x63
        retlw   0x74
        retlw   0x20
        retlw   0x6f
        retlw   0x70
        retlw   0x74
        retlw   0x69
        retlw   0x6f
        retlw   0x6e
        retlw   0x3a
        retlw   0x20
        retlw   0x80
        retlw   0x54
        retlw   0x68
        retlw   0x61
        retlw   0x6e
        retlw   0x6b
        retlw   0x73
        retlw   0x21
        retlw   0x20
        retlw   0x50
        retlw   0x6c
        retlw   0x65
        retlw   0x61
        retlw   0x73
        retlw   0x65
        retlw   0x20
        retlw   0x74
        retlw   0x75
        retlw   0x72
        retlw   0x6e
        retlw   0x20
        retlw   0x6f
        retlw   0x66
        retlw   0x66
        retlw   0x2e
        retlw   0x80

;===============================================================================
; EEDATA area

        ; eeprom

        org     __EEPROM_START                              ; address: 0x2100

        db      0x00
        db      0x00
        db      0x1d
        db      0x00

        ; eeprom

        org     0x2108

        db      0x47                                        ; 'G'
        db      0x34                                        ; '4'
        db      0x65                                        ; 'e'
        db      0x34                                        ; '4'
        db      0x6e                                        ; 'n'
        db      0x34                                        ; '4'
        db      0x4c                                        ; 'L'
        db      0x34                                        ; '4'
        db      0x49                                        ; 'I'
        db      0x34                                        ; '4'
        db      0x20                                        ; ' '
        db      0x34                                        ; '4'
        db      0x32                                        ; '2'
        db      0x34                                        ; '4'
        db      0x41                                        ; 'A'
        db      0x34                                        ; '4'
        db      0x46                                        ; 'F'
        db      0x34                                        ; '4'
        db      0x2e                                        ; '.'
        db      0x34                                        ; '4'
        db      0x43                                        ; 'C'
        db      0x34                                        ; '4'
        db      0x61                                        ; 'a'
        db      0x34                                        ; '4'
        db      0xf1
        db      0x34                                        ; '4'
        db      0x61                                        ; 'a'
        db      0x34                                        ; '4'
        db      0x64                                        ; 'd'
        db      0x34                                        ; '4'
        db      0x61                                        ; 'a'
        db      0x34                                        ; '4'
        db      0x30                                        ; '0'
        db      0x34                                        ; '4'
        db      0x36                                        ; '6'
        db      0x34                                        ; '4'
        db      0x2f                                        ; '/'
        db      0x34                                        ; '4'
        db      0x30                                        ; '0'
        db      0x34                                        ; '4'
        db      0x31                                        ; '1'
        db      0x34                                        ; '4'
        db      0x2f                                        ; '/'
        db      0x34                                        ; '4'
        db      0x30                                        ; '0'
        db      0x34                                        ; '4'
        db      0x37                                        ; '7'
        db      0x34                                        ; '4'
        db      0x39                                        ; '9'
        db      0x34                                        ; '4'
        db      0x36                                        ; '6'
        db      0x34                                        ; '4'
        db      0x30                                        ; '0'
        db      0x34                                        ; '4'
        db      0x30                                        ; '0'
        db      0x34                                        ; '4'
        db      0x2c                                        ; ','
        db      0x34                                        ; '4'
        db      0x38                                        ; '8'
        db      0x34                                        ; '4'
        db      0x4e                                        ; 'N'
        db      0x34                                        ; '4'
        db      0x31                                        ; '1'
        db      0x34                                        ; '4'

        end
