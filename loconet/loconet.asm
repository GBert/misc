;	20Mhz crystal
;
;	LocoNet 16.66 kBaud / 8N1 - 60us Timing
;	CSMA/CD

        processor p16f873
        radix dec

	ERRORLEVEL  -302,-1302

        include p16f873.inc
;



; The recognition of labels and registers is not always good, therefore
; be treated cautiously the results.

        CONFIG  FOSC  = HS
        CONFIG  WDTE  = OFF
        CONFIG  PWRTE = ON
        CONFIG  BOREN = ON
        CONFIG  LVP   = ON
        CONFIG  CPD   = OFF
        CONFIG  WRT   = ON
        CONFIG  DEBUG = OFF
        CONFIG  CP    = OFF

        __idlocs 0x1632

#DEFINE LN_TX   RA0
#DEFINE LN_RX   RA1
#DEFINE LED     RA2

;===============================================================================
; DATA address definitions

Common_RAM      equ     0x0070                              ; size: 16 bytes

;===============================================================================
; CODE area

        ; code

        org     __CODE_START                                ; address: 0x0000

vector_reset:                                               ; address: 0x0000

        nop
        goto    START
        nop
        nop

vector_int:                                                 ; address: 0x0004

        goto    vector_reset

START: 	                                                    ; address: 0x0005
        ; BANK0
        bcf     STATUS, RP0
        bcf     STATUS, RP1
        clrf    PORTA
        movlw   0x04
        movwf   PORTB
        movlw   0x40
        movwf   PORTC                                       ; reg: 0x007
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x3a
        movwf   TRISA                                       ; reg: 0x085
        movlw   0xcb
        movwf   TRISB                                       ; reg: 0x086
        movlw   0x80
        movwf   TRISC                                       ; reg: 0x087
        ; only digital I/O
        movlw   0x06
        movwf   ADCON1                                      ; reg: 0x09f
        clrwdt
        movlw   0xc4
        movwf   OPTION_REG                                  ; reg: 0x081
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     T1CON, TMR1CS                               ; internal clock Fosc/4 => 5MHz
        bsf     T1CON, T1CKPS0                              ; 1:8 Prescale Value
        bsf     T1CON, T1CKPS1                              ;
        clrf    0x20                                        ; reg: 0x020
        bsf     0x20, 0x3                                   ; reg: 0x020
        bsf     0x20, 0x2                                   ; reg: 0x020
        call    function_019
        call    function_016

label_003:                                                  ; address: 0x0021

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_005
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_006
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_002
        bcf     PORTA, LED                                  ; reg: 0x005, bit: 2

label_004:                                                  ; address: 0x0031

        call    function_018
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        btfsc   (Common_RAM + 14), 0x7                      ; reg: 0x07e
        call    function_019
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        call    function_012
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_005
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_005
        call    function_007
        goto    label_003

label_005:                                                  ; address: 0x0044

        call    function_000
        call    function_022
        goto    label_004

function_000:                                               ; address: 0x0047

        bsf     PORTA, LED                                  ; reg: 0x005, bit: 2
        call    function_001
        bcf     PORTA, LED                                  ; reg: 0x005, bit: 2
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_006
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_008
        return

label_006:                                                  ; address: 0x0051

        movf    0x24, W                                     ; reg: 0x024
        subwf   0x21, W                                     ; reg: 0x021
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x48
        sublw   0x46
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        movf    0x21, W                                     ; reg: 0x021
        movwf   FSR                                         ; reg: 0x004
        movf    0x2a, W                                     ; reg: 0x02a
        movwf   INDF                                        ; reg: 0x000
        movf    0x21, W                                     ; reg: 0x021
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_007
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        return

label_007:                                                  ; address: 0x0063

        incf    0x21, F                                     ; reg: 0x021
        return

label_008:                                                  ; address: 0x0065

        bcf     0x20, 0x1                                   ; reg: 0x020

label_009:                                                  ; address: 0x0066

        call    function_005
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        goto    label_010
        call    function_018
        btfss   0x20, 0x3                                   ; reg: 0x020
        goto    label_009
        return

label_010:                                                  ; address: 0x006d

        call    function_023
        clrf    (Common_RAM + 14)                           ; reg: 0x07e
        return

function_001:                                               ; address: 0x0070

        movlw   0x06
        call    function_020
        bcf     0x20, 0x1                                   ; reg: 0x020
        clrf    0x2a                                        ; reg: 0x02a
        movlw   0x08
        movwf   0x2f                                        ; reg: 0x02f
        bcf     0x20, 0x7                                   ; reg: 0x020
        bcf     0x20, 0x0                                   ; reg: 0x020
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_012
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_011:                                                  ; address: 0x0089

        incf    (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x0e
        xorwf   (Common_RAM + 14), W                        ; reg: 0x07e
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x02
        btfsc   0x25, 0x0                                   ; reg: 0x025
        retlw   0x01
        nop
        goto    label_013

label_012:                                                  ; address: 0x0095

        movlw   0x01
        call    function_021
        nop
        goto    label_011

label_013:                                                  ; address: 0x0099

        movlw   0x14
        call    function_020
        movlw   0x13
        call    function_020
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_015
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_014:                                                  ; address: 0x00ae

        rrf     0x25, W                                     ; reg: 0x025
        rrf     0x2a, F                                     ; reg: 0x02a
        incf    (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x0e
        xorwf   (Common_RAM + 14), W                        ; reg: 0x07e
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_013
        goto    label_016

label_015:                                                  ; address: 0x00bb

        movlw   0x01
        call    function_021
        nop
        goto    label_014

label_016:                                                  ; address: 0x00bf

        movlw   0x13
        call    function_020
        movlw   0x13
        call    function_020
        nop
        nop
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_019
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_017:                                                  ; address: 0x00d6

        incf    (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 14), F                        ; reg: 0x07e
        movlw   0x0e
        xorwf   (Common_RAM + 14), W                        ; reg: 0x07e
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_018
        btfsc   0x25, 0x0                                   ; reg: 0x025
        retlw   0x00
        retlw   0x01

label_018:                                                  ; address: 0x00e1

        retlw   0x02

label_019:                                                  ; address: 0x00e2

        movlw   0x01
        call    function_021
        nop
        goto    label_017

function_002:                                               ; address: 0x00e6

        btfss   0x20, 0x5                                   ; reg: 0x020
        return
        btfsc   0x20, 0x3                                   ; reg: 0x020
        return
        btfss   0x20, 0x7                                   ; reg: 0x020
        return
        btfss   0x20, 0x6                                   ; reg: 0x020
        return
        movf    0x22, W                                     ; reg: 0x022
        movwf   0x25                                        ; reg: 0x025
        movf    0x28, W                                     ; reg: 0x028
        movwf   0x26                                        ; reg: 0x026
        btfss   PORTA, LN_RX                                  ; reg: 0x005, bit: 1
        return

label_020:                                                  ; address: 0x00f4

        bsf     PORTA, LN_TX                                  ; reg: 0x005, bit: 0
        bcf     0x20, 0x7                                   ; reg: 0x020
        bsf     PORTA, LED                                  ; reg: 0x005, bit: 2
        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x2b                                        ; reg: 0x02b
        call    function_004
        xorlw   0x00
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_028
        nop
        movf    0x22, W                                     ; reg: 0x022
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_021
        nop
        incf    0x22, F                                     ; reg: 0x022
        goto    label_022

label_021:                                                  ; address: 0x0107

        movlw   0xa0
        movwf   0x22                                        ; reg: 0x022
        nop

label_022:                                                  ; address: 0x010a

        decfsz  0x28, F                                     ; reg: 0x028
        goto    label_020
        bcf     0x20, 0x5                                   ; reg: 0x020

label_023:                                                  ; address: 0x010d

        movf    0x25, W                                     ; reg: 0x025
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x28                                        ; reg: 0x028
        movf    0x21, W                                     ; reg: 0x021
        movwf   FSR                                         ; reg: 0x004
        movf    0x28, W                                     ; reg: 0x028
        btfss   PORTA, RA5                                  ; reg: 0x005, bit: 5
        goto    label_025
        movwf   INDF                                        ; reg: 0x000
        movf    0x21, W                                     ; reg: 0x021
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_024
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        goto    label_025

label_024:                                                  ; address: 0x011e

        incf    0x21, F                                     ; reg: 0x021

label_025:                                                  ; address: 0x011f

        movlw   0x01
        subwf   0x2d, F                                     ; reg: 0x02d
        movf    0x25, W                                     ; reg: 0x025
        movwf   FSR                                         ; reg: 0x004
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        decfsz  0x26, F                                     ; reg: 0x026
        goto    label_026
        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        return
        movlw   0x48
        subwf   0x2d, W                                     ; reg: 0x02d
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        btfsc   (Common_RAM + 15), 0x0                      ; reg: 0x07f
        goto    label_030
        return

label_026:                                                  ; address: 0x0130

        movf    0x25, W                                     ; reg: 0x025
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_027
        incf    0x25, F                                     ; reg: 0x025
        goto    label_023

label_027:                                                  ; address: 0x0136

        movlw   0xa0
        movwf   0x25                                        ; reg: 0x025
        goto    label_023

label_028:                                                  ; address: 0x0139

        bsf     PORTA, LN_TX                                  ; reg: 0x005, bit: 0
        clrf    TMR0                                        ; reg: 0x001
        movf    0x25, W                                     ; reg: 0x025
        movwf   0x22                                        ; reg: 0x022
        movf    0x26, W                                     ; reg: 0x026
        movwf   0x28                                        ; reg: 0x028

label_029:                                                  ; address: 0x013f

        call    function_005
        movlw   0x8a
        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_029
        bcf     0x20, 0x0                                   ; reg: 0x020
        bcf     PORTA, LN_TX                                  ; reg: 0x005, bit: 0
        call    function_023
        return

label_030:                                                  ; address: 0x0148

        clrf    0x2b                                        ; reg: 0x02b
        movlw   0xe5
        call    function_003
        movlw   0x10
        call    function_003
        movlw   0x50
        call    function_003
        movlw   0x50
        call    function_003
        movlw   0x01
        call    function_003
        movlw   0x00
        btfsc   (Common_RAM + 8), 0x7                       ; reg: 0x078
        iorlw   0x02
        btfsc   (Common_RAM + 9), 0x7                       ; reg: 0x079
        iorlw   0x04
        btfsc   (Common_RAM + 10), 0x7                      ; reg: 0x07a
        iorlw   0x08
        call    function_003
        movlw   0x16
        call    function_003
        movf    (Common_RAM + 8), W                         ; reg: 0x078
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 9), W                         ; reg: 0x079
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 10), W                        ; reg: 0x07a
        andlw   0x7f
        call    function_003
        movlw   0x00
        btfsc   (Common_RAM + 11), 0x7                      ; reg: 0x07b
        iorlw   0x02
        btfsc   (Common_RAM + 12), 0x7                      ; reg: 0x07c
        iorlw   0x04
        btfsc   (Common_RAM + 13), 0x7                      ; reg: 0x07d
        iorlw   0x08
        call    function_003
        movlw   0x32
        call    function_003
        movf    (Common_RAM + 11), W                        ; reg: 0x07b
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 12), W                        ; reg: 0x07c
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 13), W                        ; reg: 0x07d
        andlw   0x7f
        call    function_003
        comf    0x2b, W                                     ; reg: 0x02b
        call    function_003
        bcf     (Common_RAM + 15), 0x0                      ; reg: 0x07f
        return

function_003:                                               ; address: 0x017d

        movwf   0x25                                        ; reg: 0x025
        xorwf   0x2b, F                                     ; reg: 0x02b
        movf    0x21, W                                     ; reg: 0x021
        movwf   FSR                                         ; reg: 0x004
        movf    0x25, W                                     ; reg: 0x025
        movwf   INDF                                        ; reg: 0x000
        movf    0x21, W                                     ; reg: 0x021
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_031
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        return

label_031:                                                  ; address: 0x018a

        incf    0x21, F                                     ; reg: 0x021
        return

function_004:                                               ; address: 0x018c

        nop
        movlw   0x17
        call    function_020
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x19
        call    function_020
        nop
        movlw   0x08
        movwf   0x2f                                        ; reg: 0x02f

label_032:                                                  ; address: 0x0196

        btfsc   0x2b, 0x0                                   ; reg: 0x02b
        goto    label_033
        nop
        bsf     PORTA, LN_TX                                ; reg: 0x005, bit: 0
        movlw   0x1a
        call    function_020
        btfsc   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x18
        call    function_020
        nop
        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        rrf     0x2b, F                                     ; reg: 0x02b
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_032
        goto    label_034

label_033:                                                  ; address: 0x01a6

        bcf     PORTA, LN_TX                                ; reg: 0x005, bit: 0
        movlw   0x1A
        call    function_020
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x18
        call    function_020
        nop
        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        rrf     0x2b, F                                     ; reg: 0x02b
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_032
        goto    label_034

label_034:                                                  ; address: 0x01b3

        nop
        nop
        bcf     PORTA, LN_TX                                ; reg: 0x005, bit: 0
        movlw   0x19
        call    function_020
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x01
        nop
        movlw   0x16
        call    function_020
        retlw   0x00

function_005:                                               ; address: 0x01be

        movlw   0x60
        subwf   0x2d, W                                     ; reg: 0x02d
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_036
        btfss   PIR1, RCIF                                  ; reg: 0x00c, bit: 5
        goto    label_035
        btfsc   RCSTA, OERR                                 ; reg: 0x018, bit: 1
        goto    label_038
        btfsc   RCSTA, FERR                                 ; reg: 0x018, bit: 2
        goto    label_039
        incf    0x2d, F                                     ; reg: 0x02d
        movlw   0x4c
        subwf   0x2d, W                                     ; reg: 0x02d
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        bsf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        movf    0x23, W                                     ; reg: 0x023
        movwf   FSR                                         ; reg: 0x004
        movf    RCREG, W                                    ; reg: 0x01a
        movwf   INDF                                        ; reg: 0x000
        movf    0x23, W                                     ; reg: 0x023
        sublw   0xff
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_037
        movlw   0xa0
        movwf   0x23                                        ; reg: 0x023
        return

label_035:                                                  ; address: 0x01d8

        nop
        movlw   0x04
        call    function_021
        return

label_036:                                                  ; address: 0x01dc

        movlw   0x05
        call    function_021
        return

label_037:                                                  ; address: 0x01df

        incf    0x23, F                                     ; reg: 0x023
        return

label_038:                                                  ; address: 0x01e1

        bcf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bsf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        movlw   0x03
        call    function_021
        return

label_039:                                                  ; address: 0x01e6

        movf    RCREG, W                                    ; reg: 0x01a
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        bsf     (Common_RAM + 14), 0x7                      ; reg: 0x07e
        movlw   0x01
        call    function_021
        nop
        goto    label_040

label_040:                                                  ; address: 0x01ed

        return

function_006:                                               ; address: 0x01ee

        btfss   PORTB, RB1                                  ; reg: 0x006, bit: 1
        goto    label_041
        goto    label_046

label_041:                                                  ; address: 0x01f1

        nop
        btfss   0x20, 0x4                                   ; reg: 0x020
        goto    label_043
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        btfss   RCSTA, OERR                                 ; reg: 0x018, bit: 1
        goto    label_045
        nop
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   TXREG                                       ; reg: 0x019
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        decfsz  0x29, F                                     ; reg: 0x029
        goto    label_042
        bcf     0x20, 0x4                                   ; reg: 0x020

label_042:                                                  ; address: 0x0202

        movf    0x24, W                                     ; reg: 0x024
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_044
        movlw   0x30
        movwf   0x24                                        ; reg: 0x024
        return

label_043:                                                  ; address: 0x0209

        movlw   0x04
        call    function_021
        nop
        nop
        return

label_044:                                                  ; address: 0x020e

        incf    0x24, F                                     ; reg: 0x024
        return

label_045:                                                  ; address: 0x0210

        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x03
        call    function_021
        nop
        return

label_046:                                                  ; address: 0x0215

        nop
        goto    label_043

function_007:                                               ; address: 0x0217

        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        return
        btfsc   0x20, 0x5                                   ; reg: 0x020
        return
        movf    0x22, W                                     ; reg: 0x022
        subwf   0x23, W                                     ; reg: 0x023
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x60
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_051
        movwf   0x27                                        ; reg: 0x027
        movwf   0x2d                                        ; reg: 0x02d
        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        goto    label_047
        movlw   0x48
        subwf   0x2d, W                                     ; reg: 0x02d
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2

label_047:                                                  ; address: 0x0229

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004

label_048:                                                  ; address: 0x022b

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        btfsc   INDF, 0x7                                   ; reg: 0x000
        goto    label_052
        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_049
        incf    FSR, F                                      ; reg: 0x004
        goto    label_050

label_049:                                                  ; address: 0x0236

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004

label_050:                                                  ; address: 0x0238

        decfsz  0x27, F                                     ; reg: 0x027
        goto    label_048
        movf    0x23, W                                     ; reg: 0x023
        movwf   0x22                                        ; reg: 0x022

label_051:                                                  ; address: 0x023c

        clrf    0x2d                                        ; reg: 0x02d
        btfss   PORTB, RB1                                  ; reg: 0x006, bit: 1
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        return

label_052:                                                  ; address: 0x0240

        movf    FSR, W                                      ; reg: 0x004
        movwf   0x22                                        ; reg: 0x022
        btfsc   INDF, 0x6                                   ; reg: 0x000
        goto    label_054
        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_053
        movlw   0x02
        movwf   0x28                                        ; reg: 0x028
        goto    label_057

label_053:                                                  ; address: 0x0249

        movlw   0x04
        movwf   0x28                                        ; reg: 0x028
        goto    label_057

label_054:                                                  ; address: 0x024c

        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_055
        movlw   0x06
        movwf   0x28                                        ; reg: 0x028
        goto    label_057

label_055:                                                  ; address: 0x0251

        movf    0x27, W                                     ; reg: 0x027
        sublw   0x01
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_061
        incf    FSR, F                                      ; reg: 0x004

label_056:                                                  ; address: 0x025a

        movf    INDF, W                                     ; reg: 0x000
        movwf   0x28                                        ; reg: 0x028
        sublw   0x02
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_058
        movf    0x28, W                                     ; reg: 0x028
        sublw   0x20
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_058

label_057:                                                  ; address: 0x0263

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        movf    0x28, W                                     ; reg: 0x028
        movwf   0x2f                                        ; reg: 0x02f
        call    function_009
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_058
        movf    0x28, W                                     ; reg: 0x028
        movwf   0x2f                                        ; reg: 0x02f
        decf    0x2f, F                                     ; reg: 0x02f
        call    function_008
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_059
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return

label_058:                                                  ; address: 0x0278

        call    function_010
        return

label_059:                                                  ; address: 0x027a

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        call    function_011
        btfss   PORTA, RA5                                  ; reg: 0x005, bit: 5
        goto    label_060
        movf    0x24, W                                     ; reg: 0x024
        subwf   0x21, W                                     ; reg: 0x021
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x48
        addwf   0x28, W                                     ; reg: 0x028
        sublw   0x28
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return

label_060:                                                  ; address: 0x0287

        bsf     0x20, 0x5                                   ; reg: 0x020
        clrf    0x2c                                        ; reg: 0x02c
        return

label_061:                                                  ; address: 0x028a

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_056

function_008:                                               ; address: 0x028d

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x2b                                        ; reg: 0x02b
        nop

label_062:                                                  ; address: 0x0292

        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_064
        incf    FSR, F                                      ; reg: 0x004

label_063:                                                  ; address: 0x0297

        movf    INDF, W                                     ; reg: 0x000
        xorwf   0x2b, F                                     ; reg: 0x02b
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_062
        movf    0x2b, W                                     ; reg: 0x02b
        xorlw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        retlw   0x01

label_064:                                                  ; address: 0x02a2

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_063

function_009:                                               ; address: 0x02a5

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        nop
        goto    label_065

label_065:                                                  ; address: 0x02a9

        movf    0x23, W                                     ; reg: 0x023
        subwf   FSR, W                                      ; reg: 0x004
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_067
        incf    FSR, F                                      ; reg: 0x004

label_066:                                                  ; address: 0x02b2

        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_068
        retlw   0x01

label_067:                                                  ; address: 0x02b5

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_066

label_068:                                                  ; address: 0x02b8

        btfsc   INDF, 0x7                                   ; reg: 0x000
        retlw   0x02
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x00
        goto    label_065

function_010:                                               ; address: 0x02bd

        movlw   0x00
        movwf   0x28                                        ; reg: 0x028
        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_069
        incf    0x22, F                                     ; reg: 0x022
        return

label_069:                                                  ; address: 0x02c9

        movlw   0xa0
        movwf   0x22                                        ; reg: 0x022
        return

function_011:                                               ; address: 0x02cc

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        xorlw   0x81
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        bsf     (Common_RAM + 15), 0x0                      ; reg: 0x07f
        return

function_012:                                               ; address: 0x02d4

        btfsc   0x20, 0x4                                   ; reg: 0x020
        return
        movf    0x24, W                                     ; reg: 0x024
        subwf   0x21, W                                     ; reg: 0x021
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x48
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        movwf   0x27                                        ; reg: 0x027
        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004

label_070:                                                  ; address: 0x02df

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        btfsc   INDF, 0x7                                   ; reg: 0x000
        goto    label_073
        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_071
        incf    FSR, F                                      ; reg: 0x004
        goto    label_072

label_071:                                                  ; address: 0x02ea

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004

label_072:                                                  ; address: 0x02ec

        decfsz  0x27, F                                     ; reg: 0x027
        goto    label_070
        movf    0x21, W                                     ; reg: 0x021
        movwf   0x24                                        ; reg: 0x024
        return

label_073:                                                  ; address: 0x02f1

        movf    FSR, W                                      ; reg: 0x004
        movwf   0x24                                        ; reg: 0x024
        btfsc   INDF, 0x6                                   ; reg: 0x000
        goto    label_075
        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_074
        movlw   0x02
        movwf   0x29                                        ; reg: 0x029
        goto    label_078

label_074:                                                  ; address: 0x02fa

        movlw   0x04
        movwf   0x29                                        ; reg: 0x029
        goto    label_078

label_075:                                                  ; address: 0x02fd

        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_076
        movlw   0x06
        movwf   0x29                                        ; reg: 0x029
        goto    label_078

label_076:                                                  ; address: 0x0302

        movf    0x27, W                                     ; reg: 0x027
        sublw   0x01
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_081
        incf    FSR, F                                      ; reg: 0x004

label_077:                                                  ; address: 0x030b

        movf    INDF, W                                     ; reg: 0x000
        movwf   0x29                                        ; reg: 0x029
        sublw   0x02
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_079
        movf    0x29, W                                     ; reg: 0x029
        sublw   0x20
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_079

label_078:                                                  ; address: 0x0314

        btfss   PORTA, LN_RX                                  ; reg: 0x005, bit: 1
        return
        movf    0x29, W                                     ; reg: 0x029
        movwf   0x2f                                        ; reg: 0x02f
        call    function_014
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_079
        movf    0x29, W                                     ; reg: 0x029
        movwf   0x2f                                        ; reg: 0x02f
        decf    0x2f, F                                     ; reg: 0x02f
        call    function_013
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_080
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return

label_079:                                                  ; address: 0x0329

        call    function_015
        return

label_080:                                                  ; address: 0x032b

        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        bsf     0x20, 0x4                                   ; reg: 0x020
        return

label_081:                                                  ; address: 0x032f

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_077

function_013:                                               ; address: 0x0332

        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        nop
        movwf   0x25                                        ; reg: 0x025

label_082:                                                  ; address: 0x0337

        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_084
        incf    FSR, F                                      ; reg: 0x004

label_083:                                                  ; address: 0x033c

        movf    INDF, W                                     ; reg: 0x000
        xorwf   0x25, F                                     ; reg: 0x025
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_082
        movf    0x25, W                                     ; reg: 0x025
        xorlw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        retlw   0x01

label_084:                                                  ; address: 0x0347

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_083

function_014:                                               ; address: 0x034a

        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        nop
        goto    label_085

label_085:                                                  ; address: 0x034e

        movf    0x21, W                                     ; reg: 0x021
        subwf   FSR, W                                      ; reg: 0x004
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_087
        incf    FSR, F                                      ; reg: 0x004

label_086:                                                  ; address: 0x0357

        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_088
        retlw   0x01

label_087:                                                  ; address: 0x035a

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_086

label_088:                                                  ; address: 0x035d

        btfsc   INDF, 0x7                                   ; reg: 0x000
        retlw   0x02
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        retlw   0x00
        goto    label_085

function_015:                                               ; address: 0x0362

        movlw   0x00
        movwf   0x29                                        ; reg: 0x029
        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        call    function_024
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_089
        incf    0x24, F                                     ; reg: 0x024
        return

label_089:                                                  ; address: 0x036f

        movlw   0x30
        movwf   0x24                                        ; reg: 0x024
        return

function_016:                                               ; address: 0x0372

        bsf     RCSTA, SPEN                                 ; reg: 0x018, bit: 7
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bsf     RCSTA, SREN                                 ; reg: 0x018, bit: 5
        bsf     RCSTA, FERR                                 ; reg: 0x018, bit: 2
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x4b
        btfss   PORTA, RA4                                  ; reg: 0x005, bit: 4
        goto    label_090
        movlw   0x40
        btfsc   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_090
        movlw   0x14

label_090:                                                  ; address: 0x037f

        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movwf   TXREG                                       ; reg: 0x019
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bsf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        return

function_017:                                               ; address: 0x0385

        btfsc   0x20, 0x7                                   ; reg: 0x020
        goto    label_092
        btfss   0x20, 0x0                                   ; reg: 0x020
        goto    label_091
        movlw   0xba
        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        bcf     0x20, 0x0                                   ; reg: 0x020
        bsf     0x20, 0x7                                   ; reg: 0x020
        return

label_091:                                                  ; address: 0x0390

        clrf    TMR0                                        ; reg: 0x001
        bcf     0x20, 0x6                                   ; reg: 0x020
        bsf     0x20, 0x0                                   ; reg: 0x020
        return

label_092:                                                  ; address: 0x0394

        btfsc   0x20, 0x6                                   ; reg: 0x020
        return
        btfss   0x20, 0x0                                   ; reg: 0x020
        goto    label_094
        movlw   0x03
        movwf   PCLATH                                      ; reg: 0x00a
        movf    0x2c, W                                     ; reg: 0x02c
        addwf   0x2c, W                                     ; reg: 0x02c
        addwf   PCL, F                                      ; reg: 0x002
        nop
        nop
        movlw   0xba
        goto    label_093
        movlw   0xa7
        goto    label_093
        movlw   0x94
        goto    label_093
        movlw   0x81
        goto    label_093
        movlw   0x6e
        goto    label_093
        movlw   0x5b
        goto    label_093
        movlw   0x49
        goto    label_093
        movlw   0x36
        goto    label_093

label_093:                                                  ; address: 0x03af

        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        bcf     0x20, 0x0                                   ; reg: 0x020
        bsf     0x20, 0x6                                   ; reg: 0x020
        return

label_094:                                                  ; address: 0x03b5

        btfss   0x20, 0x5                                   ; reg: 0x020
        clrf    0x2c                                        ; reg: 0x02c
        movf    0x2c, W                                     ; reg: 0x02c
        xorlw   0x08
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        incf    0x2c, F                                     ; reg: 0x02c
        clrf    TMR0                                        ; reg: 0x001
        bsf     0x20, 0x0                                   ; reg: 0x020
        return

function_018:                                               ; address: 0x03be

        btfsc   0x20, 0x3                                   ; reg: 0x020
        goto    label_096
        btfsc   0x20, 0x1                                   ; reg: 0x020
        bcf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        btfss   T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        goto    RESTART_TIMER1
        btfss   PIR1, TMR1IF                                ; skip if Timer1 overflow
        return
        bcf     T1CON, TMR1ON                               ; stop Timer1
        bsf     0x20, 0x3                                   ; reg: 0x020
        bsf     0x20, 0x2                                   ; reg: 0x020
        return

RESTART_TIMER1:                                             ; address: 0x03ca

        clrf    TMR1L                                       ; clear Timer1
        clrf    TMR1H                                       ;
        bcf     PIR1, TMR1IF                                ; clear Timer1 overflow flag
        bsf     T1CON, TMR1ON                               ; start Timer1
        return

label_096:                                                  ; address: 0x03cf

        btfss   0x20, 0x2                                   ; reg: 0x020
        goto    label_097
        btfss   0x20, 0x1                                   ; reg: 0x020
        return

label_097:                                                  ; address: 0x03d3

        btfsc   0x20, 0x2                                   ; reg: 0x020
        goto    label_098
        btfss   PIR1, TMR1IF                                ; skip Return if Timer1 overflow is set
        return
        decfsz  0x26, F                                     ; reg: 0x026
        goto    label_099
        bcf     T1CON, TMR1ON                               ; Stop Timer 1
        bsf     T1CON, T1CKPS0                              ; Prescaler 1:8
        bsf     T1CON, T1CKPS1                              ;
        bcf     0x20, 0x3                                   ; reg: 0x020
        return

label_098:                                                  ; address: 0x03de

        bcf     0x20, 0x2                                   ; reg: 0x020
        movlw   0x05
        movwf   0x26                                        ; reg: 0x026
        call    function_019
        bcf     T1CON, T1CKPS0                              ; Prescaler 1:4
        bsf     T1CON, T1CKPS1                              ;
        clrf    TMR1L                                       ; clear Timer1 value
        clrf    TMR1H                                       ;
        bsf     T1CON, TMR1ON                               ; Start Timer1

label_099:                                                  ; address: 0x03e7

        bcf     PIR1, TMR1IF                                ; reg: 0x00c, bit: 0
        return

function_019:                                               ; address: 0x03e9

        clrf    (Common_RAM + 8)                            ; reg: 0x078
        clrf    (Common_RAM + 9)                            ; reg: 0x079
        clrf    (Common_RAM + 10)                           ; reg: 0x07a
        clrf    (Common_RAM + 11)                           ; reg: 0x07b
        clrf    (Common_RAM + 12)                           ; reg: 0x07c
        clrf    (Common_RAM + 13)                           ; reg: 0x07d
        clrf    (Common_RAM + 14)                           ; reg: 0x07e
        clrf    0x2d                                        ; reg: 0x02d
        bcf     0x20, 0x4                                   ; reg: 0x020
        bcf     0x20, 0x5                                   ; reg: 0x020
        bcf     (Common_RAM + 15), 0x0                      ; reg: 0x07f
        clrf    0x2c                                        ; reg: 0x02c
        movlw   0xa0
        movwf   0x23                                        ; reg: 0x023
        movwf   0x22                                        ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004

label_100:                                                  ; address: 0x03f9

        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_101
        incf    FSR, F                                      ; reg: 0x004
        goto    label_100

label_101:                                                  ; address: 0x0400

        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        movwf   0x24                                        ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004

label_102:                                                  ; address: 0x0404

        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        incf    FSR, F                                      ; reg: 0x004
        goto    label_102

function_020:                                               ; address: 0x040b

        call    function_021
        nop
        call    function_005
        call    function_006
        return

function_021:                                               ; address: 0x0410

        movwf   0x2e                                        ; reg: 0x02e

label_103:                                                  ; address: 0x0411

        decfsz  0x2e, F                                     ; reg: 0x02e
        goto    label_103
        return

label_104:                                                  ; address: 0x0414

        btfss   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_104
        movlw   0x05
        call    function_021

label_105:                                                  ; address: 0x0418

        btfsc   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_105
        movlw   0x05
        call    function_021
        bcf     PORTA, LED                                  ; reg: 0x005, bit: 2
        return
        addlw   0xff
        addlw   0xff

function_022:                                               ; address: 0x0420

        movlw   0x12
        movwf   0x25                                        ; reg: 0x025
        nop

label_106:                                                  ; address: 0x0423

        movlw   0x05
        call    function_021
        btfss   PORTA, LN_RX                                ; reg: 0x005, bit: 1
        return
        decfsz  0x25, F                                     ; reg: 0x025
        goto    label_106
        return

function_023:                                               ; address: 0x042a

        incfsz  (Common_RAM + 13), F                        ; reg: 0x07d
        return
        incfsz  (Common_RAM + 12), F                        ; reg: 0x07c
        return
        incf    (Common_RAM + 11), F                        ; reg: 0x07b
        return

function_024:                                               ; address: 0x0430

        incfsz  (Common_RAM + 10), F                        ; reg: 0x07a
        return
        incfsz  (Common_RAM + 9), F                         ; reg: 0x079
        return
        incf    (Common_RAM + 8), F                         ; reg: 0x078
        return

        end
