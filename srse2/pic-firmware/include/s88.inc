;------------------------------------------------------------------------------
;
; CAN S88
;
; Copyright (c) 2024 Gerhard Bertelsmann
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
                S88_FORCEEVENT_TMP : 1
                S88_FORCED_BYTE_COUNT : 1
                S88_FEVENTDELAY_COUNT : 1
                S88_FEVENTDELAY_SOLL  : 1
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

#IFDEF SRSE2_SLCAN
#DEFINE        S88_DATA   0   ; RB0
#DEFINE        S88_CLOCK  1   ; RB1
#DEFINE        S88_LOAD   4   ; RB4
#DEFINE        S88_RESET  5   ; RB5
#ENDIF

#IFDEF WR841_SLCAN
#DEFINE        S88_DATA   4   ; RB4
#DEFINE        S88_CLOCK  1   ; RB1
#DEFINE        S88_LOAD   6   ; RC6
#DEFINE        S88_RESET  7   ; RC7
#ENDIF


;------------------------------------------------------------------------------
; S88 Macros
;------------------------------------------------------------------------------

S88_LOAD_HIGH   MACRO
#IFDEF SRSE2_SLCAN
                BSF     LATB,S88_LOAD
#ELSE
                BSF     LATC,S88_LOAD
#ENDIF
                ENDM

S88_LOAD_LOW    MACRO
#IFDEF SRSE2_SLCAN
                BCF     LATB,S88_LOAD
#ELSE
                BCF     LATC,S88_LOAD
#ENDIF
                ENDM

S88_RESET_HIGH  MACRO
#IFDEF SRSE2_SLCAN
                BSF     LATB,S88_RESET
#ELSE
                BSF     LATC,S88_RESET
#ENDIF
                ENDM

S88_RESET_LOW   MACRO
#IFDEF SRSE2_SLCAN
                BCF     LATB,S88_RESET
#ELSE
                BCF     LATC,S88_RESET
#ENDIF
                ENDM

S88_CLOCK_HIGH  MACRO
                BSF     LATB,S88_CLOCK
                ENDM

S88_CLOCK_LOW   MACRO
                BCF     LATB,S88_CLOCK
                ENDM

;------------

BTFSS88_INPUT   MACRO
                BTFSS   PORTB,S88_DATA
                ENDM

BTFCS88_INPUT   MACRO
                BTFSC   PORTB,S88_DATA
#ENDIF
                ENDM

;------------------------------------------------------------------------------
; load and store macros
;------------------------------------------------------------------------------

S88_PREP_POINT  MACRO   BYTE_COUNT
                LFSR    FSR0,S88_DATA_P
                ; absolut limit 16 x S88 -> 32 bytes
                MOVLW   (S88_MAX_LENGTH * 2) - 1
                ANDWF   BYTE_COUNT,F
                MOVLW   5
                MULWF   BYTE_COUNT
                ; we need to work on low bytes only
                MOVF    PRODL,W
                ADDWF   FSR0L,F
                ENDM

S88_LOAD_MEM    MACRO   BYTE_COUNT
                S88_PREP_POINT BYTE_COUNT
                MOVFF   POSTINC0,S88_CT0_TMP
                MOVFF   POSTINC0,S88_CT1_TMP
                MOVFF   POSTINC0,S88_FORCEEVENT_TMP
                MOVFF   POSTINC0,S88_STATE_TMP
                MOVFF   POSTINC0,S88_CHANGE_TMP
                ENDM

S88_SAVE_MEM    MACRO   BYTE_COUNT
                S88_PREP_POINT BYTE_COUNT
                MOVFF   S88_CT0_TMP,POSTINC0
                MOVFF   S88_CT1_TMP,POSTINC0
                MOVFF   S88_FORCEEVENT_TMP,POSTINC0
                MOVFF   S88_STATE_TMP,POSTINC0
                MOVFF   S88_CHANGE_TMP,POSTINC0
                ENDM

S88_LOAD_FORCED MACRO   BYTE_COUNT
                S88_PREP_POINT BYTE_COUNT
                MOVLW   2
                ADDWF	FSR0L,F
                MOVFF	POSTINC0, S88_FORCEEVENT_TMP
                MOVFF	INDF0, ACTUAL
                DECF	FSR0L,F  ;adjust for save
                ENDM

S88_SAVE_FORCED MACRO   BYTE_COUNT
                MOVFF	S88_FORCEEVENT_TMP, INDF0
                ENDM

;------------------------------------------------------------------------------
; Init. S88
;------------------------------------------------------------------------------

S88INIT
#IFDEF WR841_SLCAN
                BSF     TRISB,S88_DATA  ; Input S88 Data
                BCF     TRISB,S88_RESET ; Output S88 Reset
                BCF     TRISC,S88_CLOCK ; Output S88 Clock
                BCF     TRISC,S88_LOAD  ; Output S88 Load

                BCF     LATB,S88_RESET  ; clear pins
                BCF     LATC,S88_CLOCK  ;
                BCF     LATC,S88_LOAD   ;

#ELSE
                BSF     TRISB,S88_DATA  ; Input S88 Data
                BCF     TRISB,S88_CLOCK ; Output S88 Clock
                BCF     TRISB,S88_LOAD  ; Output S88 Load
                BCF     TRISB,S88_RESET ; Output S88 Reset

                BCF     LATB,S88_CLOCK  ; clear pins
                BCF     LATB,S88_LOAD   ;
                BCF     LATB,S88_RESET  ;
#ENDIF

                CLRF    S88MASK
                CLRF    S88_BYTE_COUNT
                CLRF    S88STATE
                RETURN

S88_RESET_MEM
                MOVLW   S88_MAX_LENGTH * 2
                MOVWF   S88COUNT
                LFSR    FSR0,S88_CT0
S88_FILL
                SETF    POSTINC0  ; S88_CT0 fill with 255
                SETF    POSTINC0  ; S88_CT1 fill with 255
                CLRF    POSTINC0  ; S88_FORCEEVENT
                CLRF    POSTINC0  ; S88_STATE
                CLRF    POSTINC0  ; S88_CHANGE
                DECFSZ  S88COUNT,F
                BRA     S88_FILL
                RETURN

;------------------------------------------------------------------------------
; S88 helper functions
;------------------------------------------------------------------------------

S88_SET_LENGTH_CHECKED
; LEN in WREG
; checks for maxlen and writes S88LENGTH
                MOVWF	S88LENGTH
                SUBLW  S88_MAX_LENGTH
                BTFSC  STATUS,C
                BRA	S88_SET_LENGTH_CHEEND
                MOVLW	S88_MAX_LENGTH
                MOVWF	S88LENGTH
S88_SET_LENGTH_CHEEND
                MOVF	S88LENGTH,W
                RETURN ;corrected value in WREG

CALC_S88CONTACTID_INRANGE
; checks contact id in s88 contact range
; contactID in (TEMP16(Highbyte)):(TEMP16+1(Lowbyte))
; RETURN Carry=1 IN RANGE (WREG=1), Carry=0 OUT OF RANGE (WREG=0)
; RETURN PRODH:PRODL LastS88Index - ContacrIndex = Nr of bits until bufferend
; PRODH:PRODL only valid when in range

                ; if(ContactID <= (S88C_H:S88C_L)
                ; then out of range (ContactID to low)
                ; (S88C_H:S88C_L) + 1 = ContactID of first valid contact
                MOVF    TEMP16+1,W
                SUBWF   S88C_L,W
                MOVF    TEMP16,W
                SUBWFB  S88C_H,W
                BC      CALC_S88CONTACTID_RANGERR ;ContactID <= (S88C_H:S88C_L)
                ; ContactID > (S88C_H:S88C_L)
                ; calculate S88LENGTH * 16
                MOVF    S88LENGTH, W
                MULLW   16
                ; calculate (S88C_H:S88C_L) + (S88LENGTH * 16)
                MOVF	S88C_L,W
                ADDWF	PRODL, F
                BTFSC	STATUS,C
                INCF	PRODH,F
                MOVF	S88C_H,W
                ADDWF	PRODH,F
                ; PRODH:PRODL = (S88C_H:S88C_L) + (S88LENGTH * 16)
                ; PRODH:PRODL = contactID of last valid contact
                ; calc if(ContactID=Y <= PRODH:PRODL)
                ; return 1 (ContactID in range)
                MOVF	TEMP16+1,W
                SUBWF	PRODL,F
                MOVF	TEMP16,W
                SUBWFB  PRODH,F
                BNC	CALC_S88CONTACTID_RANGERR ;ContactID=Y > PRODH:PRODL
                ; IN RANGE
                BSF     STATUS,C
                RETLW   1
CALC_S88CONTACTID_RANGERR
                ; OUT OF RANGE
                BCF	STATUS,C
                RETLW	0

CALC_S88CONTACTID_BUFFER_PTR
; calculates buffer pointer to contact-state byte in buffer and bit mask
; from contactID in (TEM16:TEMP16+1)
; RETURN:
; FSR0 points to S88_state byte
; MASK is bit mask
; (TEM16:TEMP16+1) unchanged
                ;calculate: Contact ID - 1
                MOVFF	TEMP16,PRODH
                MOVFF	TEMP16+1,PRODL
                MOVF	PRODL,F
                BTFSC	STATUS,Z
                DECF	PRODH,F
                DECF	PRODL,F
                ; PRODH:PRODL= contactID-1
                ; calc PRODH:PRODL - S88C_H:S88C_L
                MOVF	S88C_L,W
                SUBWF	PRODL,F
                MOVF	S88C_H,W
                BTFSS	STATUS,C
                ADDLW	1
                SUBWF	PRODH,F
                ; get bit mask
                MOVF	PRODL,W
                ANDLW	0x07
                MOVWF	TEMP
                CALL	TABLE_SET_MASK   ; calculate bit mask
                MOVWF	MASK
                ; PRODH:PRODL = (contactID-1) - S88C_H:S88C_L
                ; devide by 8 to get byte offset
                BCF	STATUS,C
                RRCF	PRODH,F
                RRCF	PRODL,F   ; /2rotate offset
                RRCF	PRODH,F
                RRCF	PRODL,F   ; /2rotate offset
                RRCF	PRODH,F
                RRCF	PRODL,F   ; /2rotate offset
                ; multiply by entry size in buffer (*5)
                LFSR	FSR0,S88_DATA_P+3     ; points to first state
                MOVF	PRODL,W
                MULLW	5
                MOVF	PRODL,W
                ADDWF	FSR0L,F
                ; FSR points to state byte in buffer
                RETURN

;------------------------------------------------------------------------------
; Finite State Machine
;------------------------------------------------------------------------------

S88_FSM    ;S88 state machine called by SCHEDULE

                MOVF    S88STATE,W
                BZ      S88_STATE_1
                XORLW	7            ; mostly called check first (faster)
                BZ      S88_STATE_8
                XORLW	8 ^ 7        ; mostly called check second (faster)
                BZ      S88_STATE_9
                XORLW   1 ^ 8
                BZ      S88_STATE_2
                XORLW   2 ^ 1
                BZ      S88_STATE_3
                XORLW   3 ^ 2
                BZ      S88_STATE_4
                XORLW   4 ^ 3
                BZ      S88_STATE_5
                XORLW   5 ^ 4
                BZ      S88_STATE_6
                XORLW   6 ^ 5
                BZ      S88_STATE_7
                ; we should not come her
                CLRF	S88STATE
                RCALL	S88_RESET_MEM
                BRA	S88INIT


;------------------------------------------------------------------------------
; S88 subs
;------------------------------------------------------------------------------

S88_STATE_1
                CLRF    S88_BYTE_COUNT
                ; S88_LOAD_MEM  S88_BYTE_COUNT
                ; Moved to state 9 because force event
                ; racing condition

                MOVF	S88LENGTH,W
                ; return if length is zero
                BTFSC   STATUS,Z
                RETURN
                MULLW	16
                MOVFF	PRODL,S88COUNT	; S88COUNT = 0 256 Bytes ,else less

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

                ; load S88 input mask with 1 again
                MOVLW   1
                MOVWF   S88MASK
                ; load debounce environment
                S88_LOAD_MEM  S88_BYTE_COUNT ;Load debounce and force

                ; DEBOUNCE 4/2 equal S88_NEW_TMP bits
                ; S88_NEW_TMP is input data from S88 bus
#IFDEF S88_INPUT_HIGH_ACTIVE
                MOVF	S88_NEW_TMP,W     ;use COMF for high active
#ELSE
                COMF	S88_NEW_TMP,W     ;use COMF for low active
#ENDIF
                XORWF	S88_STATE_TMP,W   ;port changed ?
                MOVWF	PRODL
                ANDWF	S88_CT0_TMP,F     ;reset or count ct0
                COMF	S88_CT0_TMP,F

                btfsc	OPTION_FLAGS, OPTION_2EQUALSS88
                BRA	DEBOUNCE_2EQUALS

DEBOUNCE_4EQUALS
                 ; DEBOUNCE 4 Equal
                MOVF	S88_CT1_TMP,W     ; reset or count ct1
                ANDWF	PRODL,W
                XORWF	S88_CT0_TMP,W
                MOVWF	S88_CT1_TMP
                ANDWF	S88_CT0_TMP,W
                ANDWF	PRODL,W
                MOVWF	S88_CHANGE_TMP
                XORWF	S88_STATE_TMP,F
                BRA	DEBOUNCE_END

DEBOUNCE_2EQUALS
                ; DEBOUNCE 2 Equal
                MOVF	S88_CT0_TMP,W  ;MK
                ANDWF	PRODL,W
                MOVWF	S88_CHANGE_TMP
                XORWF	S88_STATE_TMP,F

DEBOUNCE_END
                ; CREATE CHANGE EVENTS, IF ANY
                MOVF	S88_CHANGE_TMP,W
                BTFSS	STATUS,Z
                RCALL   S88_GENEVENT
                ;save debounce environment
                S88_SAVE_MEM  S88_BYTE_COUNT
                INCF    S88_BYTE_COUNT,F

;                CLRF	S88_NEW_TMP  ;only used for debug (simulator)

                RETURN
;------------------------------------------------------------------------------
; Forced S88 event handling
;------------------------------------------------------------------------------

S88_CHECK_FORCED_EVENTS
                ; check if s88 query answers (forced events) to be sent
                ; checks 1 byte per call, sends max 1 event per call
                TSTFSZ	S88_FEVENTDELAY_COUNT
                DECF	S88_FEVENTDELAY_COUNT,F
                BTFSS	STATUS,Z
                RETURN	;delay != 0

                S88_LOAD_FORCED S88_FORCED_BYTE_COUNT
                MOVF    S88_FORCEEVENT_TMP,W
                BZ      S88_CHECK_FORCED_EVENTS_2 ;no input event forced in this byte
                ; forced events are to sent
                MOVFF   S88C_H,TEMP16
                MOVFF   S88C_L,TEMP16+1
                SWAPF   S88_FORCED_BYTE_COUNT,W
                RRNCF   WREG,W
                ADDWF   TEMP16+1,F
                BTFSC   STATUS,C
                INCF    TEMP16,F
                MOVLW   1
                MOVWF   MASK
                BCF     STATUS,W
S88_CHECK_FORCED_EVENTS_LP
                INFSNZ  TEMP16+1,F
                INCF    TEMP16,F
                MOVF    MASK,W
                ANDWF   S88_FORCEEVENT_TMP,W
                BTFSC   STATUS,Z
                BRA	S88_CHECK_FORCED_EVENTS_L1
                ; force bit = 1, clear bit and send event
                MOVFF	S88_FEVENTDELAY_SOLL, S88_FEVENTDELAY_COUNT ;wait time after send in ms
                COMF    MASK,W
                ANDWF   S88_FORCEEVENT_TMP,F
                S88_SAVE_FORCED S88_FORCED_BYTE_COUNT
                ; use goto if max 1 event per call, use call if max 8 events per call
                GOTO    PUTEVENTEQUAL
S88_CHECK_FORCED_EVENTS_L1
                BCF     STATUS,C
                RLCF    MASK,F
                BNC     S88_CHECK_FORCED_EVENTS_LP
S88_CHECK_FORCED_EVENTS_2
                ;increment S88_FORCED_BYTE_COUNT, check S88LENGTH reached
                BCF	STATUS,Z
                RLCF	S88LENGTH,W
                SUBWF	S88_FORCED_BYTE_COUNT,W
                MOVLW	0
                BTFSS	STATUS,Z
                INCF	S88_FORCED_BYTE_COUNT,W
                MOVWF	S88_FORCED_BYTE_COUNT
                S88_SAVE_FORCED S88_FORCED_BYTE_COUNT
                RETURN	;check next byte next call

;------------------------------------------------------------------------------
; event generation if S88 inputs change (old state = ~new state)
;------------------------------------------------------------------------------
S88_GENEVENT ;S88 Event on change with old=NOT new if change bit is set
                MOVFF   S88C_H,TEMP16
                MOVFF   S88C_L,TEMP16+1

                SWAPF   S88_BYTE_COUNT,W
                RRNCF   WREG,W
                ADDWF   TEMP16+1,F
                BTFSC   STATUS,C
                INCF    TEMP16,F

                MOVFF   S88_STATE_TMP,ACTUAL
                MOVLW   1
                MOVWF   MASK

S88_GENEVENTLOOP
                INFSNZ  TEMP16+1,F
                INCF    TEMP16,F
                MOVF    S88_CHANGE_TMP,W
                ANDWF   MASK,W
                BTFSS   STATUS,Z
                CALL    PUTEVENT
                ;we have sent event, clear change-flag and force-flag
                COMF	MASK,W ;invert mask
                ANDWF   S88_CHANGE_TMP, F
                ANDWF	S88_FORCEEVENT_TMP,F
                BCF     STATUS,C
                RLCF    MASK,F
                BNC     S88_GENEVENTLOOP
                RETURN

TABLE_SET_MASK
                MOVLW	7
                ANDWF	TEMP,F
                MOVF	TEMP,W
                MOVLW   1
                BTFSC	STATUS,Z
                RETURN
                BCF	STATUS,C
TABLE_SET_MASK_1
                RLCF	WREG,F
                DECFSZ	TEMP,F
                BRA	TABLE_SET_MASK_1
                RETURN

;
; vim: set syntax=pic18f shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
