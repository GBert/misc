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
; 0x200007	free
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

                MOVLW   0
                RCALL   SETLOCIDP
                RCALL   FIN_WRITE_DATA

                RETURN

;------------------------------------------------------------------------------
; Set CAN Hash according to Maerklin documentation
;------------------------------------------------------------------------------
READCANHASH
                RCALL   READ_UID
                MOVLW   'C'
                XORWF   ID_H,W
                MOVWF   CANHASH_HIGH
                ; shift left 3
                SWAPF	CANHASH_HIGH,F
                RRNCF	CANHASH_HIGH,F
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

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
