        processor p16f873
        radix dec

        include p16f873.inc

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

        __idlocs 0x1630

;===============================================================================
; DATA address definitions

Common_RAM      equ     0x0070                              ; size: 16 bytes

;===============================================================================
; CODE area

        ; code

        org     __CODE_START                                ; address: 0x0000

vector_reset:                                               ; address: 0x0000

        nop
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     STATUS, RP1                                 ; reg: 0x003, bit: 6
        clrf    PORTA                                       ; reg: 0x005

vector_int:                                                 ; address: 0x0004

        movlw   0x04
        movwf   PORTB                                       ; reg: 0x006
        movlw   0x40
        movwf   PORTC                                       ; reg: 0x007
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x3a
        movwf   PORTA                                       ; reg: 0x005
        movlw   0xca
        movwf   PORTB                                       ; reg: 0x006
        movlw   0x80
        movwf   PORTC                                       ; reg: 0x007
        movlw   0x06
        movwf   ADCON0                                      ; reg: 0x01f
        clrwdt
        movlw   0xc4
        movwf   TMR0                                        ; reg: 0x001
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     T1CON, TMR1CS                               ; reg: 0x010, bit: 1
        bsf     T1CON, T1CKPS0                              ; reg: 0x010, bit: 4
        bsf     T1CON, T1CKPS1                              ; reg: 0x010, bit: 5
        clrf    0x20                                        ; reg: 0x020
        bsf     0x20, 0x3                                   ; reg: 0x020
        bsf     0x20, 0x2                                   ; reg: 0x020
        call    function_019
        call    function_016

label_002:                                                  ; address: 0x001d

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_005
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_006
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_002
        bcf     PORTA, RA2                                  ; reg: 0x005, bit: 2

label_003:                                                  ; address: 0x002d

        call    function_018
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        btfsc   (Common_RAM + 15), 0x7                      ; reg: 0x07f
        call    function_019
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        call    function_012
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        bsf     0x20, 0x1                                   ; reg: 0x020
        call    function_017
        call    function_005
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_004
        call    function_007
        goto    label_002

label_004:                                                  ; address: 0x0040

        call    function_000
        call    function_022
        goto    label_003

function_000:                                               ; address: 0x0043

        bsf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        call    function_001
        bcf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_005
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_007
        return

label_005:                                                  ; address: 0x004d

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
        goto    label_006
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        return

label_006:                                                  ; address: 0x005f

        incf    0x21, F                                     ; reg: 0x021
        return

label_007:                                                  ; address: 0x0061

        bcf     0x20, 0x1                                   ; reg: 0x020

label_008:                                                  ; address: 0x0062

        call    function_005
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        goto    label_009
        call    function_018
        btfss   0x20, 0x3                                   ; reg: 0x020
        goto    label_008
        return

label_009:                                                  ; address: 0x0069

        call    function_023
        clrf    (Common_RAM + 15)                           ; reg: 0x07f
        return

function_001:                                               ; address: 0x006c

        movlw   0x06
        call    function_020
        bcf     0x20, 0x1                                   ; reg: 0x020
        clrf    0x2a                                        ; reg: 0x02a
        movlw   0x08
        movwf   0x2f                                        ; reg: 0x02f
        bcf     0x20, 0x7                                   ; reg: 0x020
        bcf     0x20, 0x0                                   ; reg: 0x020
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_011
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_010:                                                  ; address: 0x0085

        incf    (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x0e
        xorwf   (Common_RAM + 15), W                        ; reg: 0x07f
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x02
        btfsc   0x25, 0x0                                   ; reg: 0x025
        retlw   0x01
        nop
        goto    label_012

label_011:                                                  ; address: 0x0091

        movlw   0x01
        call    function_021
        nop
        goto    label_010

label_012:                                                  ; address: 0x0095

        movlw   0x14
        call    function_020
        movlw   0x13
        call    function_020
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_014
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_013:                                                  ; address: 0x00aa

        rrf     0x25, W                                     ; reg: 0x025
        rrf     0x2a, F                                     ; reg: 0x02a
        incf    (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x0e
        xorwf   (Common_RAM + 15), W                        ; reg: 0x07f
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_012
        goto    label_015

label_014:                                                  ; address: 0x00b7

        movlw   0x01
        call    function_021
        nop
        goto    label_013

label_015:                                                  ; address: 0x00bb

        movlw   0x13
        call    function_020
        movlw   0x13
        call    function_020
        nop
        nop
        clrf    0x25                                        ; reg: 0x025
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        bsf     0x25, 0x0                                   ; reg: 0x025
        movlw   0x02
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        xorwf   0x25, W                                     ; reg: 0x025
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_018
        movlw   0x01
        call    function_021
        movlw   0x00
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        movlw   0x01
        movwf   0x25                                        ; reg: 0x025

label_016:                                                  ; address: 0x00d2

        incf    (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x80
        btfsc   0x25, 0x0                                   ; reg: 0x025
        andwf   (Common_RAM + 15), F                        ; reg: 0x07f
        movlw   0x0e
        xorwf   (Common_RAM + 15), W                        ; reg: 0x07f
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_017
        btfsc   0x25, 0x0                                   ; reg: 0x025
        retlw   0x00
        retlw   0x01

label_017:                                                  ; address: 0x00dd

        retlw   0x02

label_018:                                                  ; address: 0x00de

        movlw   0x01
        call    function_021
        nop
        goto    label_016

function_002:                                               ; address: 0x00e2

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
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        bcf     0x20, 0x7                                   ; reg: 0x020
        bsf     PORTA, RA2                                  ; reg: 0x005, bit: 2

label_019:                                                  ; address: 0x00f2

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x2b                                        ; reg: 0x02b
        call    function_004
        xorlw   0x00
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_027
        nop
        movf    0x22, W                                     ; reg: 0x022
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_020
        nop
        incf    0x22, F                                     ; reg: 0x022
        goto    label_021

label_020:                                                  ; address: 0x0102

        movlw   0xa0
        movwf   0x22                                        ; reg: 0x022
        nop

label_021:                                                  ; address: 0x0105

        decfsz  0x28, F                                     ; reg: 0x028
        goto    label_019
        bcf     0x20, 0x5                                   ; reg: 0x020

label_022:                                                  ; address: 0x0108

        movf    0x25, W                                     ; reg: 0x025
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x28                                        ; reg: 0x028
        movf    0x21, W                                     ; reg: 0x021
        movwf   FSR                                         ; reg: 0x004
        movf    0x28, W                                     ; reg: 0x028
        btfss   PORTA, RA5                                  ; reg: 0x005, bit: 5
        goto    label_024
        movwf   INDF                                        ; reg: 0x000
        movf    0x21, W                                     ; reg: 0x021
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_023
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        goto    label_024

label_023:                                                  ; address: 0x0119

        incf    0x21, F                                     ; reg: 0x021

label_024:                                                  ; address: 0x011a

        movlw   0x01
        subwf   0x2d, F                                     ; reg: 0x02d
        movf    0x25, W                                     ; reg: 0x025
        movwf   FSR                                         ; reg: 0x004
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        decfsz  0x26, F                                     ; reg: 0x026
        goto    label_025
        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        return
        movlw   0x48
        subwf   0x2d, W                                     ; reg: 0x02d
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        btfsc   PORTB, RB0                                  ; reg: 0x006, bit: 0
        goto    label_029
        return

label_025:                                                  ; address: 0x012b

        movf    0x25, W                                     ; reg: 0x025
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_026
        incf    0x25, F                                     ; reg: 0x025
        goto    label_022

label_026:                                                  ; address: 0x0131

        movlw   0xa0
        movwf   0x25                                        ; reg: 0x025
        goto    label_022

label_027:                                                  ; address: 0x0134

        bsf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        clrf    TMR0                                        ; reg: 0x001
        movf    0x25, W                                     ; reg: 0x025
        movwf   0x22                                        ; reg: 0x022
        movf    0x26, W                                     ; reg: 0x026
        movwf   0x28                                        ; reg: 0x028

label_028:                                                  ; address: 0x013a

        call    function_005
        movlw   0x8a
        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_028
        bcf     0x20, 0x0                                   ; reg: 0x020
        bcf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        call    function_023
        return

label_029:                                                  ; address: 0x0143

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
        btfsc   (Common_RAM + 9), 0x7                       ; reg: 0x079
        iorlw   0x02
        btfsc   (Common_RAM + 10), 0x7                      ; reg: 0x07a
        iorlw   0x04
        btfsc   (Common_RAM + 11), 0x7                      ; reg: 0x07b
        iorlw   0x08
        call    function_003
        movlw   0x16
        call    function_003
        movf    (Common_RAM + 9), W                         ; reg: 0x079
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 10), W                        ; reg: 0x07a
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 11), W                        ; reg: 0x07b
        andlw   0x7f
        call    function_003
        movlw   0x00
        btfsc   (Common_RAM + 12), 0x7                      ; reg: 0x07c
        iorlw   0x02
        btfsc   (Common_RAM + 13), 0x7                      ; reg: 0x07d
        iorlw   0x04
        btfsc   (Common_RAM + 14), 0x7                      ; reg: 0x07e
        iorlw   0x08
        call    function_003
        movlw   0x30
        call    function_003
        movf    (Common_RAM + 12), W                        ; reg: 0x07c
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 13), W                        ; reg: 0x07d
        andlw   0x7f
        call    function_003
        movf    (Common_RAM + 14), W                        ; reg: 0x07e
        andlw   0x7f
        call    function_003
        comf    0x2b, W                                     ; reg: 0x02b
        call    function_003
        bcf     PORTB, RB0                                  ; reg: 0x006, bit: 0
        return

function_003:                                               ; address: 0x0178

        movwf   0x25                                        ; reg: 0x025
        xorwf   0x2b, F                                     ; reg: 0x02b
        movf    0x21, W                                     ; reg: 0x021
        movwf   FSR                                         ; reg: 0x004
        movf    0x25, W                                     ; reg: 0x025
        movwf   INDF                                        ; reg: 0x000
        movf    0x21, W                                     ; reg: 0x021
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_030
        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        return

label_030:                                                  ; address: 0x0185

        incf    0x21, F                                     ; reg: 0x021
        return

function_004:                                               ; address: 0x0187

        bsf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        movlw   0x1a
        call    function_020
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x19
        call    function_020
        nop
        movlw   0x08
        movwf   0x2f                                        ; reg: 0x02f

label_031:                                                  ; address: 0x0191

        btfsc   0x2b, 0x0                                   ; reg: 0x02b
        goto    label_032
        nop
        bsf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        movlw   0x19
        call    function_020
        btfsc   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x19
        call    function_020
        nop
        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        rrf     0x2b, F                                     ; reg: 0x02b
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_031
        goto    label_033

label_032:                                                  ; address: 0x01a1

        bcf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        movlw   0x19
        call    function_020
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x01
        movlw   0x19
        call    function_020
        nop
        bcf     STATUS, C                                   ; reg: 0x003, bit: 0
        rrf     0x2b, F                                     ; reg: 0x02b
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_031
        goto    label_033

label_033:                                                  ; address: 0x01ae

        nop
        nop
        bcf     PORTA, RA0                                  ; reg: 0x005, bit: 0
        movlw   0x17
        call    function_020
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x01
        nop
        movlw   0x16
        call    function_020
        retlw   0x00

function_005:                                               ; address: 0x01b9

        movlw   0x60
        subwf   0x2d, W                                     ; reg: 0x02d
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_035
        btfss   PIR1, RCIF                                  ; reg: 0x00c, bit: 5
        goto    label_034
        btfsc   RCSTA, OERR                                 ; reg: 0x018, bit: 1
        goto    label_037
        btfsc   RCSTA, FERR                                 ; reg: 0x018, bit: 2
        goto    label_038
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
        goto    label_036
        movlw   0xa0
        movwf   0x23                                        ; reg: 0x023
        return

label_034:                                                  ; address: 0x01d3

        nop
        movlw   0x04
        call    function_021
        return

label_035:                                                  ; address: 0x01d7

        movlw   0x05
        call    function_021
        return

label_036:                                                  ; address: 0x01da

        incf    0x23, F                                     ; reg: 0x023
        return

label_037:                                                  ; address: 0x01dc

        bcf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bsf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        movlw   0x03
        call    function_021
        return

label_038:                                                  ; address: 0x01e1

        movf    RCREG, W                                    ; reg: 0x01a
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        bsf     (Common_RAM + 15), 0x7                      ; reg: 0x07f
        movlw   0x01
        call    function_021
        nop
        goto    label_039

label_039:                                                  ; address: 0x01e8

        return

function_006:                                               ; address: 0x01e9

        btfss   PORTB, RB1                                  ; reg: 0x006, bit: 1
        goto    label_040
        goto    label_045

label_040:                                                  ; address: 0x01ec

        nop
        btfss   0x20, 0x4                                   ; reg: 0x020
        goto    label_042
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        btfss   RCSTA, OERR                                 ; reg: 0x018, bit: 1
        goto    label_044
        nop
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   TXREG                                       ; reg: 0x019
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        decfsz  0x29, F                                     ; reg: 0x029
        goto    label_041
        bcf     0x20, 0x4                                   ; reg: 0x020

label_041:                                                  ; address: 0x01fd

        movf    0x24, W                                     ; reg: 0x024
        sublw   0x77
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_043
        movlw   0x30
        movwf   0x24                                        ; reg: 0x024
        return

label_042:                                                  ; address: 0x0204

        movlw   0x04
        call    function_021
        nop
        nop
        return

label_043:                                                  ; address: 0x0209

        incf    0x24, F                                     ; reg: 0x024
        return

label_044:                                                  ; address: 0x020b

        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x03
        call    function_021
        nop
        return

label_045:                                                  ; address: 0x0210

        nop
        goto    label_042

function_007:                                               ; address: 0x0212

        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        return
        btfsc   0x20, 0x5                                   ; reg: 0x020
        return
        movf    0x22, W                                     ; reg: 0x022
        subwf   0x23, W                                     ; reg: 0x023
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x60
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_050
        movwf   0x27                                        ; reg: 0x027
        movwf   0x2d                                        ; reg: 0x02d
        btfsc   PORTB, RB1                                  ; reg: 0x006, bit: 1
        goto    label_046
        movlw   0x48
        subwf   0x2d, W                                     ; reg: 0x02d
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2

label_046:                                                  ; address: 0x0224

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004

label_047:                                                  ; address: 0x0226

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        btfsc   INDF, 0x7                                   ; reg: 0x000
        goto    label_051
        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_048
        incf    FSR, F                                      ; reg: 0x004
        goto    label_049

label_048:                                                  ; address: 0x0231

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004

label_049:                                                  ; address: 0x0233

        decfsz  0x27, F                                     ; reg: 0x027
        goto    label_047
        movf    0x23, W                                     ; reg: 0x023
        movwf   0x22                                        ; reg: 0x022

label_050:                                                  ; address: 0x0237

        clrf    0x2d                                        ; reg: 0x02d
        btfss   PORTB, RB1                                  ; reg: 0x006, bit: 1
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        return

label_051:                                                  ; address: 0x023b

        movf    FSR, W                                      ; reg: 0x004
        movwf   0x22                                        ; reg: 0x022
        btfsc   INDF, 0x6                                   ; reg: 0x000
        goto    label_053
        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_052
        movlw   0x02
        movwf   0x28                                        ; reg: 0x028
        goto    label_056

label_052:                                                  ; address: 0x0244

        movlw   0x04
        movwf   0x28                                        ; reg: 0x028
        goto    label_056

label_053:                                                  ; address: 0x0247

        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_054
        movlw   0x06
        movwf   0x28                                        ; reg: 0x028
        goto    label_056

label_054:                                                  ; address: 0x024c

        movf    0x27, W                                     ; reg: 0x027
        sublw   0x01
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_060
        incf    FSR, F                                      ; reg: 0x004

label_055:                                                  ; address: 0x0255

        movf    INDF, W                                     ; reg: 0x000
        movwf   0x28                                        ; reg: 0x028
        sublw   0x02
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_057
        movf    0x28, W                                     ; reg: 0x028
        sublw   0x20
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_057

label_056:                                                  ; address: 0x025e

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        movf    0x28, W                                     ; reg: 0x028
        movwf   0x2f                                        ; reg: 0x02f
        call    function_009
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_057
        movf    0x28, W                                     ; reg: 0x028
        movwf   0x2f                                        ; reg: 0x02f
        decf    0x2f, F                                     ; reg: 0x02f
        call    function_008
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_058
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return

label_057:                                                  ; address: 0x0273

        call    function_010
        return

label_058:                                                  ; address: 0x0275

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        call    function_011
        btfss   PORTA, RA5                                  ; reg: 0x005, bit: 5
        goto    label_059
        movf    0x24, W                                     ; reg: 0x024
        subwf   0x21, W                                     ; reg: 0x021
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        addlw   0x48
        addwf   0x28, W                                     ; reg: 0x028
        sublw   0x28
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return

label_059:                                                  ; address: 0x0282

        bsf     0x20, 0x5                                   ; reg: 0x020
        clrf    0x2c                                        ; reg: 0x02c
        return

label_060:                                                  ; address: 0x0285

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_055

function_008:                                               ; address: 0x0288

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        movwf   0x2b                                        ; reg: 0x02b
        nop

label_061:                                                  ; address: 0x028d

        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_063
        incf    FSR, F                                      ; reg: 0x004

label_062:                                                  ; address: 0x0292

        movf    INDF, W                                     ; reg: 0x000
        xorwf   0x2b, F                                     ; reg: 0x02b
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_061
        movf    0x2b, W                                     ; reg: 0x02b
        xorlw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        retlw   0x01

label_063:                                                  ; address: 0x029d

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_062

function_009:                                               ; address: 0x02a0

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        nop
        goto    label_064

label_064:                                                  ; address: 0x02a4

        movf    0x23, W                                     ; reg: 0x023
        subwf   FSR, W                                      ; reg: 0x004
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_066
        incf    FSR, F                                      ; reg: 0x004

label_065:                                                  ; address: 0x02ad

        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_067
        retlw   0x01

label_066:                                                  ; address: 0x02b0

        movlw   0xa0
        movwf   FSR                                         ; reg: 0x004
        goto    label_065

label_067:                                                  ; address: 0x02b3

        btfsc   INDF, 0x7                                   ; reg: 0x000
        retlw   0x02
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x00
        goto    label_064

function_010:                                               ; address: 0x02b8

        movlw   0x00
        movwf   0x28                                        ; reg: 0x028
        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movlw   0x00
        movwf   INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_068
        incf    0x22, F                                     ; reg: 0x022
        return

label_068:                                                  ; address: 0x02c4

        movlw   0xa0
        movwf   0x22                                        ; reg: 0x022
        return

function_011:                                               ; address: 0x02c7

        movf    0x22, W                                     ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        xorlw   0x81
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        bsf     PORTB, RB0                                  ; reg: 0x006, bit: 0
        return

function_012:                                               ; address: 0x02cf

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

label_069:                                                  ; address: 0x02da

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        btfsc   INDF, 0x7                                   ; reg: 0x000
        goto    label_072
        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_070
        incf    FSR, F                                      ; reg: 0x004
        goto    label_071

label_070:                                                  ; address: 0x02e5

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004

label_071:                                                  ; address: 0x02e7

        decfsz  0x27, F                                     ; reg: 0x027
        goto    label_069
        movf    0x21, W                                     ; reg: 0x021
        movwf   0x24                                        ; reg: 0x024
        return

label_072:                                                  ; address: 0x02ec

        movf    FSR, W                                      ; reg: 0x004
        movwf   0x24                                        ; reg: 0x024
        btfsc   INDF, 0x6                                   ; reg: 0x000
        goto    label_074
        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_073
        movlw   0x02
        movwf   0x29                                        ; reg: 0x029
        goto    label_077

label_073:                                                  ; address: 0x02f5

        movlw   0x04
        movwf   0x29                                        ; reg: 0x029
        goto    label_077

label_074:                                                  ; address: 0x02f8

        btfsc   INDF, 0x5                                   ; reg: 0x000
        goto    label_075
        movlw   0x06
        movwf   0x29                                        ; reg: 0x029
        goto    label_077

label_075:                                                  ; address: 0x02fd

        movf    0x27, W                                     ; reg: 0x027
        sublw   0x01
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_080
        incf    FSR, F                                      ; reg: 0x004

label_076:                                                  ; address: 0x0306

        movf    INDF, W                                     ; reg: 0x000
        movwf   0x29                                        ; reg: 0x029
        sublw   0x02
        btfsc   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_078
        movf    0x29, W                                     ; reg: 0x029
        sublw   0x20
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        goto    label_078

label_077:                                                  ; address: 0x030f

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        movf    0x29, W                                     ; reg: 0x029
        movwf   0x2f                                        ; reg: 0x02f
        call    function_014
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_078
        movf    0x29, W                                     ; reg: 0x029
        movwf   0x2f                                        ; reg: 0x02f
        decf    0x2f, F                                     ; reg: 0x02f
        call    function_013
        xorlw   0x00
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_079
        xorlw   0x02
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return

label_078:                                                  ; address: 0x0324

        call    function_015
        return

label_079:                                                  ; address: 0x0326

        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        bsf     0x20, 0x4                                   ; reg: 0x020
        return

label_080:                                                  ; address: 0x032a

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_076

function_013:                                               ; address: 0x032d

        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        movf    INDF, W                                     ; reg: 0x000
        nop
        movwf   0x25                                        ; reg: 0x025

label_081:                                                  ; address: 0x0332

        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_083
        incf    FSR, F                                      ; reg: 0x004

label_082:                                                  ; address: 0x0337

        movf    INDF, W                                     ; reg: 0x000
        xorwf   0x25, F                                     ; reg: 0x025
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x02
        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_081
        movf    0x25, W                                     ; reg: 0x025
        xorlw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        retlw   0x01

label_083:                                                  ; address: 0x0342

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_082

function_014:                                               ; address: 0x0345

        movf    0x24, W                                     ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004
        nop
        goto    label_084

label_084:                                                  ; address: 0x0349

        movf    0x21, W                                     ; reg: 0x021
        subwf   FSR, W                                      ; reg: 0x004
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        retlw   0x00
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_086
        incf    FSR, F                                      ; reg: 0x004

label_085:                                                  ; address: 0x0352

        decfsz  0x2f, F                                     ; reg: 0x02f
        goto    label_087
        retlw   0x01

label_086:                                                  ; address: 0x0355

        movlw   0x30
        movwf   FSR                                         ; reg: 0x004
        goto    label_085

label_087:                                                  ; address: 0x0358

        btfsc   INDF, 0x7                                   ; reg: 0x000
        retlw   0x02
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        retlw   0x00
        goto    label_084

function_015:                                               ; address: 0x035d

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
        goto    label_088
        incf    0x24, F                                     ; reg: 0x024
        return

label_088:                                                  ; address: 0x036a

        movlw   0x30
        movwf   0x24                                        ; reg: 0x024
        return

function_016:                                               ; address: 0x036d

        bsf     RCSTA, SPEN                                 ; reg: 0x018, bit: 7
        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bcf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bsf     RCSTA, SREN                                 ; reg: 0x018, bit: 5
        bsf     RCSTA, FERR                                 ; reg: 0x018, bit: 2
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movlw   0x4b
        btfss   PORTA, RA4                                  ; reg: 0x005, bit: 4
        goto    label_089
        movlw   0x40
        btfsc   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_089
        movlw   0x14

label_089:                                                  ; address: 0x037a

        bsf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        movwf   TXREG                                       ; reg: 0x019
        bcf     STATUS, RP0                                 ; reg: 0x003, bit: 5
        bsf     RCSTA, CREN                                 ; reg: 0x018, bit: 4
        bcf     PORTB, RB2                                  ; reg: 0x006, bit: 2
        return

function_017:                                               ; address: 0x0380

        btfsc   0x20, 0x7                                   ; reg: 0x020
        goto    label_091
        btfss   0x20, 0x0                                   ; reg: 0x020
        goto    label_090
        movlw   0xba
        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        bcf     0x20, 0x0                                   ; reg: 0x020
        bsf     0x20, 0x7                                   ; reg: 0x020
        return

label_090:                                                  ; address: 0x038b

        clrf    TMR0                                        ; reg: 0x001
        bcf     0x20, 0x6                                   ; reg: 0x020
        bsf     0x20, 0x0                                   ; reg: 0x020
        return

label_091:                                                  ; address: 0x038f

        btfsc   0x20, 0x6                                   ; reg: 0x020
        return
        btfss   0x20, 0x0                                   ; reg: 0x020
        goto    label_093
        movlw   0x03
        movwf   PCLATH                                      ; reg: 0x00a
        movf    0x2c, W                                     ; reg: 0x02c
        addwf   0x2c, W                                     ; reg: 0x02c
        addwf   PCL, F                                      ; reg: 0x002
        nop
        nop
        movlw   0xba
        goto    label_092
        movlw   0xa7
        goto    label_092
        movlw   0x94
        goto    label_092
        movlw   0x81
        goto    label_092
        movlw   0x6e
        goto    label_092
        movlw   0x5b
        goto    label_092
        movlw   0x49
        goto    label_092
        movlw   0x36
        goto    label_092

label_092:                                                  ; address: 0x03aa

        subwf   TMR0, W                                     ; reg: 0x001
        btfss   STATUS, C                                   ; reg: 0x003, bit: 0
        return
        bcf     0x20, 0x0                                   ; reg: 0x020
        bsf     0x20, 0x6                                   ; reg: 0x020
        return

label_093:                                                  ; address: 0x03b0

        btfss   0x20, 0x5                                   ; reg: 0x020
        clrf    0x2c                                        ; reg: 0x02c
        movf    0x2c, W                                     ; reg: 0x02c
        xorlw   0x08
        btfss   STATUS, Z                                   ; reg: 0x003, bit: 2
        incf    0x2c, F                                     ; reg: 0x02c
        clrf    TMR0                                        ; reg: 0x001
        bsf     0x20, 0x0                                   ; reg: 0x020
        return

function_018:                                               ; address: 0x03b9

        btfsc   0x20, 0x3                                   ; reg: 0x020
        goto    label_095
        btfsc   0x20, 0x1                                   ; reg: 0x020
        bcf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        btfss   T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        goto    label_094
        btfss   PIR1, TMR1IF                                ; reg: 0x00c, bit: 0
        return
        bcf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        bsf     0x20, 0x3                                   ; reg: 0x020
        bsf     0x20, 0x2                                   ; reg: 0x020
        return

label_094:                                                  ; address: 0x03c5

        clrf    TMR1L                                       ; reg: 0x00e
        clrf    TMR1H                                       ; reg: 0x00f
        bcf     PIR1, TMR1IF                                ; reg: 0x00c, bit: 0
        bsf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        return

label_095:                                                  ; address: 0x03ca

        btfss   0x20, 0x2                                   ; reg: 0x020
        goto    label_096
        btfss   0x20, 0x1                                   ; reg: 0x020
        return

label_096:                                                  ; address: 0x03ce

        btfsc   0x20, 0x2                                   ; reg: 0x020
        goto    label_097
        btfss   PIR1, TMR1IF                                ; reg: 0x00c, bit: 0
        return
        decfsz  0x26, F                                     ; reg: 0x026
        goto    label_098
        bcf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0
        bsf     T1CON, T1CKPS0                              ; reg: 0x010, bit: 4
        bsf     T1CON, T1CKPS1                              ; reg: 0x010, bit: 5
        bcf     0x20, 0x3                                   ; reg: 0x020
        return

label_097:                                                  ; address: 0x03d9

        bcf     0x20, 0x2                                   ; reg: 0x020
        movlw   0x05
        movwf   0x26                                        ; reg: 0x026
        call    function_019
        bcf     T1CON, T1CKPS0                              ; reg: 0x010, bit: 4
        bsf     T1CON, T1CKPS1                              ; reg: 0x010, bit: 5
        clrf    TMR1L                                       ; reg: 0x00e
        clrf    TMR1H                                       ; reg: 0x00f
        bsf     T1CON, TMR1ON                               ; reg: 0x010, bit: 0

label_098:                                                  ; address: 0x03e2

        bcf     PIR1, TMR1IF                                ; reg: 0x00c, bit: 0
        return

function_019:                                               ; address: 0x03e4

        clrf    (Common_RAM + 9)                            ; reg: 0x079
        clrf    (Common_RAM + 10)                           ; reg: 0x07a
        clrf    (Common_RAM + 11)                           ; reg: 0x07b
        clrf    (Common_RAM + 12)                           ; reg: 0x07c
        clrf    (Common_RAM + 13)                           ; reg: 0x07d
        clrf    (Common_RAM + 14)                           ; reg: 0x07e
        clrf    (Common_RAM + 15)                           ; reg: 0x07f
        clrf    0x2d                                        ; reg: 0x02d
        bcf     0x20, 0x4                                   ; reg: 0x020
        bcf     0x20, 0x5                                   ; reg: 0x020
        bcf     PORTB, RB0                                  ; reg: 0x006, bit: 0
        clrf    0x2c                                        ; reg: 0x02c
        movlw   0xa0
        movwf   0x23                                        ; reg: 0x023
        movwf   0x22                                        ; reg: 0x022
        movwf   FSR                                         ; reg: 0x004

label_099:                                                  ; address: 0x03f4

        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0xff
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        goto    label_100
        incf    FSR, F                                      ; reg: 0x004
        goto    label_099

label_100:                                                  ; address: 0x03fb

        movlw   0x30
        movwf   0x21                                        ; reg: 0x021
        movwf   0x24                                        ; reg: 0x024
        movwf   FSR                                         ; reg: 0x004

label_101:                                                  ; address: 0x03ff

        clrf    INDF                                        ; reg: 0x000
        movf    FSR, W                                      ; reg: 0x004
        sublw   0x77
        btfsc   STATUS, Z                                   ; reg: 0x003, bit: 2
        return
        incf    FSR, F                                      ; reg: 0x004
        goto    label_101

function_020:                                               ; address: 0x0406

        call    function_021
        nop
        call    function_005
        call    function_006
        return

function_021:                                               ; address: 0x040b

        movwf   0x2e                                        ; reg: 0x02e

label_102:                                                  ; address: 0x040c

        decfsz  0x2e, F                                     ; reg: 0x02e
        goto    label_102
        return

label_103:                                                  ; address: 0x040f

        btfss   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_103
        movlw   0x05
        call    function_021

label_104:                                                  ; address: 0x0413

        btfsc   PORTA, RA3                                  ; reg: 0x005, bit: 3
        goto    label_104
        movlw   0x05
        call    function_021
        bcf     PORTA, RA2                                  ; reg: 0x005, bit: 2
        return
        addlw   0xff
        addlw   0xff

function_022:                                               ; address: 0x041b

        movlw   0x12
        movwf   0x25                                        ; reg: 0x025
        nop

label_105:                                                  ; address: 0x041e

        movlw   0x05
        call    function_021
        btfss   PORTA, RA1                                  ; reg: 0x005, bit: 1
        return
        decfsz  0x25, F                                     ; reg: 0x025
        goto    label_105
        return

function_023:                                               ; address: 0x0425

        incfsz  (Common_RAM + 14), F                        ; reg: 0x07e
        return
        incfsz  (Common_RAM + 13), F                        ; reg: 0x07d
        return
        incf    (Common_RAM + 12), F                        ; reg: 0x07c
        return

function_024:                                               ; address: 0x042b

        incfsz  (Common_RAM + 11), F                        ; reg: 0x07b
        return
        incfsz  (Common_RAM + 10), F                        ; reg: 0x07a
        return
        incf    (Common_RAM + 9), F                         ; reg: 0x079
        return

        end
