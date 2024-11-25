;------------------------------------------------------------------------------
;
; CANBuster ID routines
;
; Copyright (c) 2024 Gerhard Bertelsmann
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
                INDEX_APOS : 1
                INDEX_N : 1
                INDEX_L : 1
                INDEX_C : 1
                ENDC

LOAD_INDEX_P    MACRO   INDEX
                MOVLW   LOW INDEX
                MOVWF   TBLPTRL
                MOVLW   HIGH INDEX
                MOVWF   TBLPTRH
                MOVLW   UPPER INDEX
                MOVWF   TBLPTRU
                ENDM



; Config in DataEEPROM
; EEPROM DEFAULT DATA is defined in idlocs.inc

READ_UID    RETURN

; EEPROM ADDRESSES
; 0x00: UNUSED
; 0x01: 'C'  Valid Marking (if != 'C' EEPROM will be initialized )
; 0x02: 'B'  Valid Marking (if != 'B' EEPROM will be initialized )
; 0x03: ID_H Device ID (Kennung) HIGH
; 0x04: ID_L Device ID (Kennung) LOW
; 0x05: IO Start Adress (Offset) HIGH
; 0x06: IO Start Adress (Offset) LOW
; 0x07: S88 Start Adress (Offset) HIGH
; 0x08: S88 Start Adress (Offset) LOW
; 0x09: Scan Interval
; 0x0A: S88 Length (Nr of 16Bit modules)
; 0x0B: UID 3rd byte
; 0x0C: UID 4th byte
#DEFINE EEP_ADDR_ID_H           0x03
#DEFINE EEP_ADDR_ID_L           0x04
#DEFINE EEP_ADDR_PIOC_H         0x05
#DEFINE EEP_ADDR_PIOC_L         0x06
#DEFINE EEP_ADDR_S88C_H         0x07
#DEFINE EEP_ADDR_S88C_L         0x08
#DEFINE EEP_ADDR_SCAN_INTERV    0x09
#DEFINE EEP_ADDR_S88LENGTH      0x0A
#DEFINE EEP_ADDR_UID_H          0x0B
#DEFINE EEP_ADDR_UID_L          0x0C
#DEFINE EEP_ADDR_OPTIONFLAGS    0x0D
#DEFINE EEP_ADDR_S88FEVENTDELAY 0x0E

EEP_SET_UNUSED_ADDR
; for safety keep EEADR in unused range
                MOVLW	0xFF
                MOVWF	EEADR
                RETURN

EEPROM_READ_LOWPAGE	;Adress in WREG 0...255
                MOVWF	EEADR
                CLRF    EEADRH
                BCF     EECON1, EEPGD ; Point to DATA memory
                BCF     EECON1, CFGS ; Access EEPROM
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVF    EEDATA, W ; W = EEDATA
                RETURN


EEPROM_WRITE_LOWPAGE
; ADDRESS in WREG, DATA in TEMP
; value is checked before write cycle (if new = old skips write cycle)
                MOVWF	EEADR
                CLRF    EEADRH
                BCF     EECON1, EEPGD ; Point to DATA memory
                BCF     EECON1, CFGS ; Access EEPROM
EEPROM_WRITE_LOWPAGE_CONT
                ; read before write
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVF    EEDATA, W
                SUBWF	TEMP,W
                BZ      EEPROM_WRITE_END ;old = new, skip write
                BTFSC	OPTION_FLAGS, OPTION_LED_ON_EEP_WRITE
                LED_EEPWRITE_ON
                BTFSC	OPTION_FLAGS, OPTION_LED_ON_EEP_WRITE
                CLRF	HEARTBEAT
                MOVFF	TEMP,EEDATA
                BCF     INTCON, GIE	; Disable interrupts
                BSF     EECON1, WREN	; Enable writes
                MOVLW   55h
                MOVWF   EECON2
                MOVLW   0AAh
                MOVWF   EECON2
                BSF     EECON1, WR	; Set WR bit to begin write
                BTFSC   EECON1, WR	; Wait for write to complete
                BRA $-2
                BCF     EECON1, WREN	; Disable writes
                BSF     INTCON, GIE	; Enable interrupts
EEPROM_WRITE_END
                RETURN


EEP_INIT        ; write default data to EEPROM (if 'C''B' check not valid)
                LOAD_INDEX_P EEPROM_DEFAULT_DATA
                TBLRD*+
                MOVFF   TABLAT,TEMP
                MOVLW 1 ;Start at EEP Address 1
                MOVWF   EEADR
                CLRF    EEADRH
                BCF     EECON1, EEPGD ; Point to DATA memory
                BCF     EECON1, CFGS ; Access EEPROM
                BCF     INTCON, GIE	; Disable interrupts
                BSF     EECON1, WREN	; Enable writes
EEP_INIT_LOOP
                TBLRD*+
                MOVFF   TABLAT,EEDATA
                MOVLW   0x55
                MOVWF   EECON2
                MOVLW   0x0AA
                MOVWF   EECON2
                BSF     EECON1, WR	; Set WR bit to begin write
                BTFSC   EECON1, WR	; Wait for write to complete
                BRA $-2
                INCFSZ  EEADR, F	; Increment address
                DECFSZ	TEMP,F
                BRA     EEP_INIT_LOOP

                BCF     EECON1, WREN	; Disable writes
                BCF     INTCON, GIE	; Enable interrupts
                BRA     EEP_SET_UNUSED_ADDR


EEP_CHECK_INIT  ;check for 'C','B' at EEPROM address 0x01, 0x02
                ;init EEPROM if not found
                MOVLW   0x01
                CALL	EEPROM_READ_LOWPAGE
                SUBLW  'C'
                BNZ	EEP_INIT
                MOVLW   0x02
                CALL	EEPROM_READ_LOWPAGE
                SUBLW  'B'
                BNZ	EEP_INIT
                BRA     EEP_SET_UNUSED_ADDR

EEP_READ_CONFIGS ; no call structure because speed
                MOVLW   EEP_ADDR_ID_H
                MOVWF   EEADR         ;3
                CLRF    EEADRH
                BCF     EECON1, EEPGD ; Point to DATA memory
                BCF     EECON1, CFGS ; Access EEPROM
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,ID_H
                INCF    EEADR,F       ;4
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,ID_L
                INCF    EEADR,F       ;5
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,PIOC_H
                INCF    EEADR,F       ;6
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,PIOC_L
                INCF    EEADR,F       ;7
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,S88C_H
                INCF    EEADR,F       ;8
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,S88C_L
                INCF    EEADR,F       ;9
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,SCAN_INTERVAL
                INCF    EEADR,F      ;10
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVF    EEDATA,W
                CALL    S88_SET_LENGTH_CHECKED
                INCF    EEADR,F      ;11
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,UID_H
                INCF    EEADR,F      ;12
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,UID_L
                INCF    EEADR,F      ;13
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,OPTION_FLAGS
                INCF    EEADR,F      ;14
                BSF     EECON1, RD ; EEPROM Read
                NOP
                MOVFF   EEDATA,S88_FEVENTDELAY_SOLL
                ;for safety keep EEADR in unused range
                BRA     EEP_SET_UNUSED_ADDR

EEP_WRITE_DEVICEID
                MOVLW   EEP_ADDR_ID_H
                MOVFF   ID_H, TEMP
                RCALL	EEPROM_WRITE_LOWPAGE
                MOVLW   EEP_ADDR_ID_L
                MOVFF   ID_L, TEMP
EEP_WRANDSAVE   RCALL	EEPROM_WRITE_LOWPAGE
                ;for safety keep EEADR in unused range
                BRA     EEP_SET_UNUSED_ADDR

EEP_WRITE_OPTIONFLAGS
                MOVLW   EEP_ADDR_OPTIONFLAGS
                MOVFF   OPTION_FLAGS, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_UID
                MOVLW   EEP_ADDR_UID_H
                MOVFF   UID_H, TEMP
                RCALL	EEPROM_WRITE_LOWPAGE
                MOVLW   EEP_ADDR_UID_L
                MOVFF   UID_L, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_PIOC
                MOVLW   EEP_ADDR_PIOC_H
                MOVFF   PIOC_H, TEMP
                RCALL	EEPROM_WRITE_LOWPAGE
                MOVLW   EEP_ADDR_PIOC_L
                MOVFF   PIOC_L, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_S88C
                MOVLW   EEP_ADDR_S88C_H
                MOVFF   S88C_H, TEMP
                RCALL	EEPROM_WRITE_LOWPAGE
                MOVLW   EEP_ADDR_S88C_L
                MOVFF   S88C_L, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_S88LENGTH
                MOVLW   EEP_ADDR_S88LENGTH
                MOVFF   S88LENGTH, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_SCANINTERVAL
                MOVLW   EEP_ADDR_SCAN_INTERV
                MOVFF   SCAN_INTERVAL, TEMP
                BRA     EEP_WRANDSAVE

EEP_WRITE_S88_FEVENTDELAY
                MOVLW   EEP_ADDR_S88FEVENTDELAY
                MOVFF   S88_FEVENTDELAY_SOLL, TEMP
                BRA     EEP_WRANDSAVE

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
                MOVLW   'C'
                XORWF   UID_H,W
                MOVWF   CANHASH_HIGH
                ; shift left 3
                SWAPF   CANHASH_HIGH,F
                RRNCF   CANHASH_HIGH,F
                ; set two lower bits
                BSF     CANHASH_HIGH,0
                BSF     CANHASH_HIGH,1
                MOVLW   'B'
                XORWF   UID_L,W
                MOVWF   CANHASH_LOW
                BSF     CANHASH_HIGH,2
                BTFSS   UID_L,7
                BCF     CANHASH_HIGH,2
                BCF     CANHASH_LOW,7
                RETURN

;------------------------------------------------------------------------------
; Send Configuration Index Description
;------------------------------------------------------------------------------

; Send Configuration Index Description (Multi CAN frame answer)
; IMPORTANT BEFORE CALL of this function
; current value of dependent config channel in (TEMP16):(TEMP16+1)
; INDEX 0...x in TEMP
; Tablepointer points to right index description (DB section) below
SEND_INDEX
; Current value in TEMP16:TEMP16+1 (H:L)
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
                ; check for current value position
                MOVF    INDEX_APOS
                BZ      SEND_INDEX_LOOP3 ;has no current value or already sent
                DECFSZ  INDEX_APOS
                BRA     SEND_INDEX_LOOP3
                ; Send current value
                MOVFF  TEMP16,POSTINC0
                MOVFF  TEMP16+1,POSTINC0
SEND_INDEX_LOOP3
                DECFSZ  INDEX_L,F
                BRA     SEND_INDEX_LOOP2
                CALL    EVENT_RING_NORMALIZE

                DECFSZ  INDEX_N,F
                BRA     SEND_INDEX_LOOP1

                ; send last entry with DLC = 5
                CLRF    POSTINC0
                MOVLW   0x3B
                MOVWF   POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW   5  ;DLC
                MOVWF   POSTINC0
                CALL    UID_TO_POSTINC0
                MOVFF   TEMP,POSTINC0   ;requested INDEX
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; variable definition for CS2
;------------------------------------------------------------------------------
; !!!!  IMPORTANT !!!!
; for index > 0
; the configchannel_nr MUST correspond TO CANNEL_GET/CHANNEL_SET number
; DO NOT SEND INDEX if channel nr does not exist

; CS2 input types for configuration
; 1 = DROPDOWN LIST see protocol definition for definition
; 2 = SLIDER    WE USE SLIDERS HERE !!!!!!!!!!!!!!!!!!!!!!

; Format eines Datenblocks mit der Moeglichkeit einen Wert einzustelle:
; Typ         Bedeutung                    Beispiel
;
; Char        Konfigirationskanalnummer    0x05: Setzen unter Kanal 5
; Char        Kenner Slider                Wert 2
; Word        Unterer Wert                 0
; Word        Oberer Wert                  2500
; Word        Aktuelle Einstellung         500
; String      Auswahlbezeichnung           Variable Strombegrenzung\0
; String      Bezeichnung Start            0.000\0
; String      Bezeichnung Ende             2500.000\0
; String      Bezeichnung Einheit          mA\0

; Actual Value is at Index 6

; ConfigChannel1 = DeviceID
; ConfigChannel2 = PIO ContactID Offset
; ConfigChannel3 = S88 Length (number 16bit modules)
; ConfigChannel4 = S88 ContactID Offset
; ConfigChannel5 = Scan Interval


;---------------------------------------------
; INDEX 0 is device descriptions
;---------------------------------------------

INDEX0
#IFDEF PWM
                DB        5 ,  0
                DB        0 ,  5 ,  0 ,  0 ,  1 ,  2,   0 ,  1
                DB       'P', 'W', 'M',  0 ,  0 ,  0,   0 ,  0
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                DB        5 ,  0
; nr measvalues, nr configchannels, not used, not used, 4 byte serial nr
                DB        0 ,  4 ,  0 ,  0 ,  1 ,  2 ,  0 ,  1
; article number 8 byte
                DB       'S', '8', '8',  0 ,  0 ,  0 ,  0 ,  0
; device description /0 terminate
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF
;---------------------------------------------
; INDEX 1...n  are config channel descriptions
;---------------------------------------------
INDEX1
                ;  5 rows - actual value at positon 28
                DB        5 , 6 ;28
;configchannel_nr, 2=slider, min_h, min_l, max_h, max_l current_h, current_
                DB        1 ,  2 ,  0 ,  0 ,  0 ,0xFF,  0 ,  0
                DB       'C', 'A', 'N', 'B', 'u', 's', 't', 'e'
                DB       'r', ' ', 'I', 'D',  0 , '0',  0 , '2'
                DB       '5', '5',  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0

INDEX2
                ;  5 rows - actual value at positon 26
                DB        5 , 6 ;26
                DB        2 ,  2 ,  0 ,  0 ,0x27,0x10,0x03,0xe8
                DB       'S', 't', 'a', 'r', 't', ' ', 'N', 'r'
                DB        0 , '0',  0 , '1', '0', '0', '0', '0'
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0

INDEX3
#IFDEF PWM
                ;  5 rows - actual value at positon 33
                DB        5 , 6 ;33
                DB        3 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '1',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                ;  5 rows - actual value at positon 29
                DB        5 , 6 ;29
                DB        3 ,  2 ,  0x00 , 0x01 , 0x00 ,0x10,  0 ,  1
                DB       'L',0xc3,0xa4, 'n', 'g', 'e', ' ', 'S'
                DB       '8', '8', '-', 'B', 'u', 's',  0 , '1'
                DB        0 , '1', '6',  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

INDEX4
#IFDEF PWM
                ;  5 rows - actual value at positon 32
                DB        5 , 6 ;32
                DB        4 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '2',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ELSE
                ;  5 rows - actual value at positon 35
                DB        5 , 6
                DB        4 ,  2 ,  0 ,  0 ,0x27,0x10,0x03,0xe8
                DB       'S', '8', '8', ' ', 'S', 't', 'a', 'r'
                DB       't',  0 , '0',  0 , '1', '0', '0', '0'
                DB       '0',  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

#IFDEF PWM
INDEX5
                DB        5 ,  6
                DB        5 ,  2 ,0x07,0xD0,0x0F,0xa0,0x0B,0xB8
                DB       'S', 'e', 'r', 'v', 'o', ' ', '3',  0
                DB       '2', '0', '0', '0',  0 , '4', '0', '0'
                DB       '0',  0 , 't', 'i', 'c', 'k', 's',  0
                DB        0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
#ENDIF

;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
