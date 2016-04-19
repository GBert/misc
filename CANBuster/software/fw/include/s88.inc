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
                ENDC

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

BTFS88_INPUT    MACRO
#IFDEF WR841_SLCAN
                BTFSS   TRISB,4
#ELSE
                BTFSS   TRISC,S88_DATA
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
                CLRF    S88TEMP
                CLRF    S88MASK
                MOVLW   0
                MOVWF   S88STATE
                CLRF    S88COUNT
                RETURN

;------------------------------------------------------------------------------
; Finite State Machine
;------------------------------------------------------------------------------

S88_FSM
                MOVLW   HIGH $
                MOVWF   PCLATH
                MOVF    S88STATE,W
                ANDLW   0x0F
                MOVWF   S88STATE
                RLNCF   S88STATE,W
                ADDWF   PCL,F
                BRA     S88_STATE_1
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
; S88 subs
;------------------------------------------------------------------------------

S88_STATE_1
                MOVLW   1
                MOVWF   S88MASK
                MOVLW   47
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
                ; read 1st bit
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
                RLCF    S88MASK,F
                S88_CLOCK_LOW
                ; read n-bit
                DECF    S88COUNT,F
                BNZ     S88_MORE_BITS
                MOVLW   0
                MOVWF   S88STATE
                RETURN

S88_MORE_BITS   DECF    S88STATE,F      ; go back -> read next
                RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
