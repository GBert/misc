;------------------------------------------------------------------------------
;
; CAN S88
;
; Copyright (c) 2015 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; This file is part of the Can-Can CAN bus interface project.
;
; Can-Can is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------
;   Finite States
;
;   Phase  0   : NOP
;          1   : Load/PS High
;          2   : Clock High
;          3   : Clock Low
;          4   : Read 1st Data Bit
;          5   : Reset High
;          6   : Reset Low
;          7   : Load/PS Low
;          8   : Clock High
;          9   : Clock Low / Read Data Bit
;                Goto 8 until all bits are read
;         10   : NOP
;         11   : NOP
;         12   : NOP
;         13   : NOP
;         14   : NOP
;         15   : NOP
;
;
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------

                CBLOCK
                S88TEMP  : 1
                S88MASK  : 1
                S88STATE : 1
                S88COUNT : 1
                S88LENGTH : 1
                S88_BYTE_COUNT : 1
                S88_CT0_TMP : 1
                S88_CT1_TMP : 1
                S88_NEW_TMP : 1
                S88_STATE_TMP : 1
                S88_CHANGE_TMP : 1
                ENDC

;------------------------------------------------------------------------------
; don't change this !

#DEFINE S88_MAX_LENGTH    16

S88_DATA_P EQU  0x360
S88_CT0    EQU  0x360
S88_CT1    EQU  0x380
S88_NEW    EQU  0x3A0
S88_STATE  EQU  0x3C0
S88_CHANGE EQU  0x3E0

;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; port defines
;------------------------------------------------------------------------------

#IFDEF PCBV
  #IF PCBV > 1
    #DEFINE      S88_DATA   2
  #ELSE
    #DEFINE      S88_DATA   1
  #ENDIF
#ENDIF


#DEFINE        S88_CLOCK  6   ; RC6
#DEFINE        S88_LOAD   7   ; RC7
#DEFINE        S88_RESET  5   ; RB5

;------------------------------------------------------------------------------
; S88 Macros
;------------------------------------------------------------------------------

S88_LOAD_HIGH   MACRO
                BSF     LATC,S88_LOAD
                ENDM

S88_LOAD_LOW    MACRO
                BCF     LATC,S88_LOAD
                ENDM

S88_RESET_HIGH  MACRO
                BSF     LATB,S88_RESET
                ENDM

S88_RESET_LOW   MACRO
                BCF     LATB,S88_RESET
                ENDM

S88_CLOCK_HIGH  MACRO
                BSF     LATC,S88_CLOCK
                ENDM

S88_CLOCK_LOW   MACRO
                BCF     LATC,S88_CLOCK
                ENDM

;------------

BTFSS88_INPUT    MACRO
#IFDEF WR841_SLCAN
                BTFSS   PORTB,4
#ELSE
                BTFSS   PORTC,S88_DATA
#ENDIF
                ENDM

BTFCS88_INPUT    MACRO
#IFDEF WR841_SLCAN
                BTFSC   PORTB,4
#ELSE
                BTFSC   PORTC,S88_DATA
#ENDIF
                ENDM

;------------------------------------------------------------------------------
; Init. S88
;------------------------------------------------------------------------------

S88INIT
#IFDEF WR841_SLCAN
                BSF     TRISB,4         ; Input S88 Data
                BCF     TRISB,S88_RESET ; Output S88 Reset
                BCF     TRISC,S88_CLOCK ; Output S88 Clock
                BCF     TRISC,S88_LOAD  ; Output S88 Load

                BCF     LATB,S88_RESET  ; clear pins
                BCF     LATC,S88_CLOCK  ;
                BCF     LATC,S88_LOAD   ;

#ELSE
                BSF     TRISC,S88_DATA  ; Input S88 Data
                BCF     TRISC,S88_CLOCK ; Output S88 Clock
                BCF     TRISC,S88_LOAD  ; Output S88 Load
                BCF     TRISB,S88_RESET ; Output S88 Reset

                BCF     LATC,S88_CLOCK  ; clear pins
                BCF     LATC,S88_LOAD   ;
                BCF     LATB,S88_RESET  ;
#ENDIF

	; debug
	BCF  TRISC,5
                CLRF    S88TEMP
                CLRF    S88MASK
                CLRF    S88_BYTE_COUNT
                CLRF    S88STATE

S88_RESET_MEM
                ; TODO
                MOVLW   S88_MAX_LENGTH * 2
                MOVWF   S88COUNT
                LFSR    FSR0,S88_CT0
                ; waste some flash mem -> speed up setup
S88_FILL
                SETF    POSTINC0        ; fill with 255
                SETF    POSTINC0
                SETF    POSTINC0
                SETF    POSTINC0
                SETF    POSTINC0
                DECFSZ  S88COUNT,F
                BRA     S88_FILL
                RETURN

;------------------------------------------------------------------------------
; Finite State Machine
;------------------------------------------------------------------------------

S88_FSM
                MOVLW   HIGH S88_FSM_BRANCH
                MOVWF   PCLATH
                MOVF    S88STATE,W
                ANDLW   0x0F
                MOVWF   S88STATE
                RLNCF   S88STATE,W
                ADDWF   PCL,F
S88_FSM_BRANCH  BRA     S88_STATE_1
                BRA     S88_STATE_2
                BRA     S88_STATE_3
                BRA     S88_STATE_4
                BRA     S88_STATE_5
                BRA     S88_STATE_6
                BRA     S88_STATE_7
                BRA     S88_STATE_8
                BRA     S88_STATE_9
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                RETURN

;------------------------------------------------------------------------------
; load and store macros
;------------------------------------------------------------------------------

S88_PREP_POINT  MACRO   BYTE_COUNT
                LFSR    FSR0,S88_DATA_P
                ; absolut limit 16 x S88 -> 32 bytes
                MOVLW   (S88_MAX_LENGTH * 2) - 1
                ANDWF   S88_BYTE_COUNT,F
                MOVLW   5
                MULWF   S88_BYTE_COUNT
                ; we need to work on low bytes only
                MOVF    PRODL,W
                ADDWF   FSR0L,F
                ENDM

S88_LOAD_MEM    MACRO   BYTE_COUNT
                S88_PREP_POINT BYTE_COUNT
                MOVFF   POSTINC0,S88_CT0_TMP
                MOVFF   POSTINC0,S88_CT1_TMP
                MOVFF   POSTINC0,S88_NEW_TMP
                MOVFF   POSTINC0,S88_STATE_TMP
                MOVFF   POSTINC0,S88_CHANGE_TMP
                ENDM

S88_SAVE_MEM    MACRO   BYTE_COUNT
                S88_PREP_POINT BYTE_COUNT
                MOVFF   S88_CT0_TMP,POSTINC0
                MOVFF   S88_CT1_TMP,POSTINC0
                MOVFF   S88_NEW_TMP,POSTINC0
                MOVFF   S88_STATE_TMP,POSTINC0
                MOVFF   S88_CHANGE_TMP,POSTINC0
                ENDM

;------------------------------------------------------------------------------
; S88 subs
;------------------------------------------------------------------------------

S88_STATE_1
                CLRF    S88_BYTE_COUNT
                S88_LOAD_MEM  S88_BYTE_COUNT

                SWAPF   S88LENGTH,W
                ANDLW   0xF0
                ; return if length is zero
                BTFSC   STATUS,Z
                RETURN

                ADDLW   0xFF
                MOVWF   S88COUNT
                S88_LOAD_HIGH
                INCF    S88STATE,F
                RETURN
S88_STATE_2
                S88_CLOCK_HIGH
                INCF    S88STATE,F
                RETURN
S88_STATE_3
                S88_CLOCK_LOW
                INCF    S88STATE,F
                RETURN
S88_STATE_4
                MOVLW   2
                MOVWF   S88MASK
                ; read 1st bit
                BSF     S88_NEW_TMP,0
                BTFSS88_INPUT
                BCF     S88_NEW_TMP,0
                INCF    S88STATE,F
                RETURN
S88_STATE_5
                S88_RESET_HIGH
                INCF    S88STATE,F
                RETURN
S88_STATE_6
                S88_RESET_LOW
                INCF    S88STATE,F
                RETURN
S88_STATE_7
                S88_LOAD_LOW
                INCF    S88STATE,F
                RETURN
S88_STATE_8
                S88_CLOCK_HIGH
                INCF    S88STATE,F
                RETURN
S88_STATE_9
                S88_CLOCK_LOW
                ; read n-bit
                MOVF    S88MASK,W
                ; set bit - maybe cleared 2 cycles later
                IORWF   S88_NEW_TMP,F
                ; BTFCS88_INPUT
                BTFSS88_INPUT
                ; delete bit
                XORWF   S88_NEW_TMP,F

                BCF     STATUS,C
                RLCF    S88MASK,F
                BTFSC   STATUS,C
                RCALL   S88_STATE_9_E

                DECF    S88COUNT,F
                BNZ     S88_MORE_BITS
                CLRF    S88STATE
                RETURN

S88_MORE_BITS   DECF    S88STATE,F      ; go back -> read next
                RETURN

S88_STATE_9_E
	BTG  LATC,5
                ; load mask with 1 again
                MOVLW   1
                MOVWF   S88MASK
                ; BSF     S88MASK,1
                ; DEBOUNCE
                PIO_DEBOUNCE   S88_NEW_TMP, S88_STATE_TMP, S88_CHANGE_TMP, S88TEMP, S88_CT0_TMP, S88_CT1_TMP
                ; CREATE EVENT
                RCALL   S88_GENEVENT
                S88_SAVE_MEM  S88_BYTE_COUNT
                INCF    S88_BYTE_COUNT,F
                S88_LOAD_MEM  S88_BYTE_COUNT
                RETURN


;------------------------------------------------------------------------------
; event generation
;------------------------------------------------------------------------------

S88_GENEVENT
                CALL    READ_UID
                MOVFF   S88C_H,TEMP16
                MOVFF   S88C_L,TEMP16+1

                SWAPF   S88_BYTE_COUNT,W
                RRNCF   WREG,W
                ADDWF   TEMP16+1,F
                BTFSC   STATUS,C
                INCF    TEMP16,F

                MOVFF   S88_NEW_TMP,ACTUAL
                MOVLW   1
                MOVWF   MASK
                MOVFF   S88_CHANGE_TMP,TEMP

S88_GENEVENTLOOP
                INFSNZ  TEMP16+1,F
                INCF    TEMP16,F
                MOVF    TEMP,W
                ANDWF   MASK,W
                BTFSS   STATUS,Z
                CALL    PUTEVENT

                BCF     STATUS,C
                RLCF    MASK,F
                BNC     S88_GENEVENTLOOP

                RETURN

TABLE_SET_MASK
                MOVLW	HIGH TABLE_MASK
                MOVWF	PCLATH
                MOVF	TEMP,W
                ANDLW	0x7
                RLNCF	WREG,W
                ADDLW	LOW TABLE_MASK
                BTFSC	STATUS,C
                INCF	PCLATH,F
                MOVWF	PCL
TABLE_MASK      RETLW	b'00000001'
                RETLW	b'00000010'
                RETLW	b'00000100'
                RETLW	b'00001000'
                RETLW	b'00010000'
                RETLW	b'00100000'
                RETLW	b'01000000'
                RETLW	b'10000000'

S88_N_GENEVENT
                MOVWF	TEMP
                DECF	TEMP,F
                CALL	TABLE_SET_MASK
                MOVWF	MASK
                ; S88 storage address = x / 8 * 5 + 3
                ; just wipe out bits before schift (PIC can't do simple logical shift)
                MOVLW   0xf8
                ; ony the first 32 bytes are valid
                ANDWF	TEMP,F
                RRNCF	TEMP,F
                RRNCF	TEMP,F
                RRNCF	TEMP,F
                MOVLW	5
                MULWF	TEMP
                ; data stored in PRODL and PRODH
                LFSR	FSR0,S88_DATA_P+3
                MOVF	PRODL,W
                MOVFF	PLUSW0,ACTUAL
                MOVFF	FSR0L,TEMP
                ; saved value is inverted
                MOVLW	0xff
                XORWF	ACTUAL,F
                GOTO	PUTEVENT

;
; vim: set syntax=pic18f shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
