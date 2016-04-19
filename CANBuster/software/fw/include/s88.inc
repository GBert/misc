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
    #DEFINE      S88_DATA   1
  #ELSE
    #DEFINE      S88_DATA   2
  #ENDIF
#ENDIF


#DEFINE        S88_CLOCK  6         
#DEFINE        S88_LOAD   7         
#DEFINE        S88_REST   5         

;------------------------------------------------------------------------------
; S88 Macros
;------------------------------------------------------------------------------

S88_RESET_HIGH  MACRO
                BSF     LATC,7
                ENDM

S88_RESET_LOW   MACRO
                BCF     LATC,7
                ENDM

S88_LOAD_HIGH   MACRO
                BSF     LATB,5
                ENDM

S88_LOAD_LOW    MACRO
                BCF     LATB,5
                ENDM

S88_CLOCK_HIGH  MACRO
                BSF     LATC,6
                ENDM

S88_CLOCK_LOW   MACRO
                BCF     LATC,6
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
                BCF     TRISB,5         ; Output S88 Reset
                BCF     TRISC,6         ; Output S88 Clock
                BCF     TRISC,7         ; Output S88 Load

                BCF     LATB,5          ; clear pins
                BCF     LATC,6          ;
                BCF     LATC,7          ;

#ELSE
                BSF     TRISC,S88_DATA  ; Input S88 Data
                BCF     TRISC,6         ; Output S88 Clock
                BCF     TRISC,7         ; Output S88 Load
                BCF     TRISB,5         ; Output S88 Reset

                ; 
                BCF     LATC,6          ; clear pins
                BCF     LATC,7          ; 
                BCF     LATB,5          ; 
#ENDIF
                CLRF    S88TEMP
                CLRF    S88MASK
                CLRF    S88STATE
                CLRF    S88COUNT
                RETURN

;------------------------------------------------------------------------------
; Finite State Machine
;------------------------------------------------------------------------------

S88_FSM
                MOVF    S88STATE,W
                ANDLW   0x0F
                MOVWF   S88STATE
                ADDWF   PCL,F
                RCALL   S88_STATE_1
                RCALL   S88_STATE_2
                RCALL   S88_STATE_3
                RCALL   S88_STATE_4
                RCALL   S88_STATE_5
                RCALL   S88_STATE_6
                RCALL   S88_STATE_7
                RCALL   S88_STATE_8
                RCALL   S88_STATE_9
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
                MOVLW   16
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
                CLRF    S88STATE

S88_MORE_BITS   DECF    S88STATE,F      ; go back -> read next
                RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
