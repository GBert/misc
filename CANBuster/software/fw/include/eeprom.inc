;------------------------------------------------------------------------------
;
; CANBuster EEPROM routines
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

		; DATA EEPROM adresses
EEA_S88LENGTH   EQU     0x000
EEA_PIOC_H      EQU     0x001
EEA_PIOC_L      EQU     0x002
EEA_S88C_H      EQU     0x003
EEA_S88C_L      EQU     0x004

;------------------------------------------------------------------------------
; Generic Flash routines
;------------------------------------------------------------------------------

EEPROM_READ
                ; MOVLW  DATA_EE_ADDRH
                ; MOVWF  EEADRH             ; Upper bits of Data Memory Address to read
                ; MOVLW  DATA_EE_ADDR
                ; MOVWF  EEADR              ; Lower bits of Data Memory Address to read
                BCF     EECON1, EEPGD       ; Point to DATA memory
                BCF     EECON1, CFGS        ; Access EEPROM
                BSF     EECON1, RD          ; EEPROM Read
                NOP
                MOVF    EEDATA, W           ; W = EEDATA
		RETURN

EEPROM_WRITE
		; MOVLW   DATA_EE_ADDRH
                ; MOVWF   EEADRH	; Upper bits of Data Memory Address to write
                ; MOVLW   DATA_EE_ADDR
                ; MOVWF   EEADR		; Lower bits of Data Memory Address to write
                ; MOVLW   DATA_EE_DATA
                ; MOVWF   EEDATA	; Data Memory Value to write
                BCF     EECON1, EEPGD	; Point to DATA memory
                BCF     EECON1, CFGS	; Access EEPROM
                BSF     EECON1, WREN	; Enable writes
                BCF     INTCON, GIE	; Disable Interrupts
                MOVLW   55h
                MOVWF   EECON2		; Write 55h
                MOVLW   0AAh
                MOVWF   EECON2		; Write 0AAh
                BSF     EECON1, WR	; Set WR bit to begin write
                BTFSC   EECON1, WR	; Wait for write to complete GOTO $-2
                BSF     INTCON, GIE	; Enable Interrupts
					; User code execution
                BCF     EECON1, WREN	; Disable writes on write complete (EEIF set)
		RETURN

EEPROM_REFRESH
                CLRF    EEADR		; Start at address 0
                CLRF    EEADRH
                BCF     EECON1, CFGS	; Set for memory
                BCF     EECON1, EEPGD	; Set for Data EEPROM
                BCF     INTCON, GIE	; Disable interrupts
                BSF     EECON1, WREN	; Enable writes

EEPROM_REFRESH_LOOP			; Loop to refresh array
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

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
