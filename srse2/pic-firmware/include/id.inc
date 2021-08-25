;------------------------------------------------------------------------------
;
; CANBuster ID routines
;
; Copyright (c) 2016 Gerhard Bertelsmann
;
;------------------------------------------------------------------------------
;
; This file is part of the CANBuster project.
;
; CANBuster is licensed under the CC BY-NC-SA 4.0.
;
; See file /LICENSE for details.
; 
;------------------------------------------------------------------------------

                LIST
                RADIX   DEC

                CBLOCK
                INDEX : 1
                INDEX_APOS : 1
                INDEX_VL : 1
                INDEX_VH : 1
                INDEX_N : 1
                INDEX_L : 1
                INDEX_C : 1
                ENDC

;------------------------------------------------------------------------------
; Generic Flash routines
;------------------------------------------------------------------------------

EEPROM_REFRESH
                CLRF    EEADR		; Start at address 0
                CLRF    EEADRH
                BCF     EECON1, CFGS	; Set for memory
                BCF     EECON1, EEPGD	; Set for Data EEPROM
                BCF     INTCON, GIE	; Disable interrupts
                BSF     EECON1, WREN	; Enable writes

EEPROM_REFRESH_LOOP	; Loop to refresh array
                BSF     EECON1, RD	; Read current address
                MOVLW   55h
                MOVWF   EECON2
                MOVLW   0AAh
                MOVWF   EECON2
                BSF     EECON1, WR	; Set WR bit to begin write
                BTFSC   EECON1, WR	; Wait for write to complete
                BRA $-2
                INCFSZ  EEADR, F	; Increment address
                BRA     EEPROM_REFRESH_LOOP	; Not zero, do it again
                INCFSZ  EEADRH, F	; Increment the high address
                BRA     EEPROM_REFRESH_LOOP	; Not zero, do it again
                BCF     EECON1, WREN	; Disable writes
                BSF     INTCON, GIE	; Enable interrupts
                RETURN


ERASE_DATA
                BSF     EECON1, EEPGD	; point to Flash program memory
                BCF     EECON1, CFGS	; access Flash program memory
                BSF     EECON1, WREN	; enable write to memory
                BSF     EECON1, FREE

                BCF     INTCON, GIE	; disable interrupts
                MOVLW   0x55
                MOVWF   EECON2
                MOVLW   0xAA
                MOVWF   EECON2
                BSF     EECON1, WR	; start program (CPU stall)
                NOP

                BSF     INTCON, GIE	; enable interrupts
                BCF     EECON1, WREN	; Disable writes on write complete (EEIF set)
                RETURN

FIN_WRITE_DATA
                BSF     EECON1, EEPGD	; point to Flash program memory
                BCF     EECON1, CFGS	; access Flash program memory
                BSF     EECON1, WREN	; enable write to memory
                BCF     EECON1, FREE

                BCF     INTCON, GIE	; disable interrupts
                MOVLW   0x55
                MOVWF   EECON2
                MOVLW   0xAA
                MOVWF   EECON2
                BSF     EECON1, WR	; start program (CPU stall)

                BSF     INTCON, GIE	; enable interrupts
                BCF     EECON1, WREN	; Disable writes on write complete (EEIF set)
                RETURN

;------------------------------------------------------------------------------
; Location ID routines
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; Location ID
; 0x200000	UID High
; 0x200001	UID Low
; 0x200002	Start counting at X-High for event
; 0x200003	Start counting at X-Low for event
; 0x200004	Start counting at X-High for event - S88
; 0x200005	Start counting at X-Low for event - S88
; 0x200006	Scan Interval (in 512us steps)
; 0x200007	S88 Bus length
;------------------------------------------------------------------------------

SETLOCIDP
                MOVWF   TBLPTRL
                CLRF    TBLPTRH
                MOVLW   20h
                MOVWF   TBLPTRU
                RETURN

READ_UID
                ; read ID and counting numbers
                MOVLW   0
                RCALL   SETLOCIDP
                TBLRD*+
                MOVFF   TABLAT,ID_H
                TBLRD*+
                MOVFF   TABLAT,ID_L
                TBLRD*+
                MOVFF   TABLAT,PIOC_H
                TBLRD*+
                MOVFF   TABLAT,PIOC_L
                TBLRD*+
                MOVFF   TABLAT,S88C_H
                TBLRD*+
                MOVFF   TABLAT,S88C_L
                TBLRD*+
                MOVFF   TABLAT,SCAN_INTERVAL
                TBLRD*+
                MOVFF   TABLAT,S88LENGTH
                RETURN

SET_UID
                ; set ID and counting numbers
                MOVLW   0
                RCALL   SETLOCIDP
                RCALL   ERASE_DATA

                MOVFF   ID_H,TABLAT
                TBLWT*+
                MOVFF   ID_L,TABLAT
                TBLWT*+
                MOVFF   PIOC_H,TABLAT
                TBLWT*+
                MOVFF   PIOC_L,TABLAT
                TBLWT*+
                MOVFF   S88C_H,TABLAT
                TBLWT*+
                MOVFF   S88C_L,TABLAT
                TBLWT*+
                MOVFF   SCAN_INTERVAL,TABLAT
                TBLWT*+
                MOVFF	S88LENGTH,TABLAT
                TBLWT*+

                MOVLW   0
                RCALL   SETLOCIDP
                RCALL   FIN_WRITE_DATA

                RETURN

;------------------------------------------------------------------------------
; Set CAN Hash according to Maerklin documentation
;------------------------------------------------------------------------------
;
;  CS2 CAN hash generation
;
;  hash' = highword ^ lowword
;
;  xxxxxxx11 0xxxxxxxx
;  ^^^^^^^    ^^^^^^^^
;   \\\\\\\   ||||||||
;    \\\\\\-- ||||||||
;     \\\\\\ \||||||||
;  000xxxxxx xxxxxxxxx


READCANHASH
                RCALL   READ_UID
                MOVLW   'C'
                XORWF   ID_H,W
                MOVWF   CANHASH_HIGH
                ; shift left 3
                SWAPF   CANHASH_HIGH,F
                RRNCF   CANHASH_HIGH,F
                ; set two lower bits
                BSF     CANHASH_HIGH,0
                BSF     CANHASH_HIGH,1

                MOVLW   'B'
                XORWF   ID_L,W
                MOVWF   CANHASH_LOW
                BSF     CANHASH_HIGH,2
                BTFSS   ID_L,7
                BCF     CANHASH_HIGH,2
                BCF     CANHASH_LOW,7
                RETURN

;------------------------------------------------------------------------------
; send variable definitions
;------------------------------------------------------------------------------

LOAD_INDEX_P    MACRO   INDEX
                MOVLW   LOW INDEX
                MOVWF   TBLPTRL
                MOVLW   HIGH INDEX
                MOVWF   TBLPTRH
                MOVLW   UPPER INDEX
                MOVWF   TBLPTRU
                ENDM

SEND_INDEX
                TBLRD*+
                MOVFF   TABLAT,INDEX_N
                CLRF    INDEX_C
                ; align
                TBLRD*+
                MOVFF   TABLAT,INDEX_APOS

                ; load Event Queue pointer
                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L
SEND_INDEX_LOOP1
                ; CAN ID  0x003B030n  n -> number
                CLRF    POSTINC0
                MOVLW   0x3B
                MOVWF   POSTINC0
                MOVLW   0x03
                MOVWF   POSTINC0
                INCF    INDEX_C,F
                MOVFF   INDEX_C,POSTINC0
                ; CAN DLC
                MOVLW   8
                MOVWF   POSTINC0
                MOVWF   INDEX_L
SEND_INDEX_LOOP2
                TBLRD*+
                MOVFF   TABLAT,POSTINC0
             DECFSZ  INDEX_APOS
                BRA     SEND_INDEX_LOOP3
                ; TODO
SEND_INDEX_LOOP3
                DECFSZ  INDEX_L,F
                BRA     SEND_INDEX_LOOP2
                CALL    EVENT_RING_NORMALIZE

                DECFSZ  INDEX_N,F
                BRA     SEND_INDEX_LOOP1

                ; send last entry with DLC = 6
                CLRF    POSTINC0
                MOVLW   0x3B
                MOVWF   POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW   6
                MOVWF   POSTINC0
                MOVLW   'C'
                MOVWF   POSTINC0
                MOVLW   'B'
                MOVWF   POSTINC0
                MOVFF   ID_H,POSTINC0
                MOVFF   ID_L,POSTINC0
                MOVFF   INDEX,POSTINC0
                MOVFF   INDEX_C,POSTINC0

                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; variable definition for cs2.exe
;------------------------------------------------------------------------------
INDEX0
#IFDEF PWM
                DB        4 ,  0
                DB        0 ,  5 ,  0 ,  0 ,  1 ,  2,   0 ,  1
                DB       'P', 'W', 'M',  0 ,  0 ,  0,   0 ,  0
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                DB        4 ,  0
                DB        0 ,  4 ,  0 ,  0 ,  1 ,  2 ,  0 ,  1
                DB       'S', '8', '8',  0 ,  0 ,  0 ,  0 ,  0
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

INDEX1
                ;  5 rows - actual value at positon 28
                DB        5 , 28
                DB        1 ,  2 ,  0 ,  0 ,  0 ,0xFF,  0 ,  0
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r', ' ', 'I', 'D',  0 , '0',  0 , '2'
                DB       '5', '5',  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0

INDEX2
                ;  5 rows - actual value at positon 26
                DB        5 , 26
                DB        2 ,  2 ,  0 ,  0 ,0x27,0x10,0x03,0xe8
                DB       'S', 't', 'a', 'r', 't', ' ', 'N', 'r'
                DB        0 , '0',  0 , '1', '0', '0', '0', '0'
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0

INDEX3
#IFDEF PWM
                ;  5 rows - actual value at positon 33
                DB        5 , 33
                DB        3 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '1',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                ;  5 rows - actual value at positon 29
                DB        5 , 29
                DB        3 ,  2 ,  0 ,  0 ,  0 ,0x0F,  0 ,  1
                DB       'L',0xc3,0xa4, 'n', 'g', 'e', ' ', 'S'
                DB       '8', '8', '-', 'B', 'u', 's',  0 , '0'
                DB        0 , '1', '5',  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

INDEX4
#IFDEF PWM
                ;  5 rows - actual value at positon 32
                DB        5 , 32
                DB        4 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '2',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                ;  5 rows - actual value at positon 35
                DB        5 , 35
                DB        4 ,  2 ,  0 ,  0 ,0x27,0x10,0x03,0xe8
                DB       'S', '8', '8', ' ', 'S', 't', 'a', 'r'
                DB       't',  0 , '0',  0 , '1', '0', '0', '0'
                DB       '0',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

#IFDEF PWM
INDEX5
                DB        5 ,  33
                DB        5 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '3',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
