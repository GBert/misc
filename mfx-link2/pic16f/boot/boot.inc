;-------------------------------------------------------------------------------
;
;   The Cowboy Boot Loader for Enhanced PIC12/PIC16
;
;   The Boot Loader
;
;   Copyright (c) 2017 Darron M Broad
;
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;   This file is part of The Cowboy Boot Loader.
;
;   The Cowboy Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Cowboy Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Cowboy Boot Loader. If not, see http://www.gnu.org/licenses/
;-------------------------------------------------------------------------------

                LIST
                RADIX   DEC

;-------------------------------------------------------------------------------
; Boot Loader Origin
;-------------------------------------------------------------------------------

STARTADDR       EQU     MAX_FLASH - (BOOTSIZE * ERASESIZE)

;-------------------------------------------------------------------------------
; Boot Loader Protocol
;-------------------------------------------------------------------------------

; VERSION
PROTOCOL        EQU     4

; ARCHITECTURE
WORDSIZE        EQU     14

; COMMANDS
VERSION         EQU     0x90
HELLO           EQU     0x91
FLASH_ERASE     EQU     0x92
FLASH_WRITE     EQU     0x93
FLASH_READ      EQU     0x94
EE_ERASE        EQU     0x95
EE_WRITE        EQU     0x96
EE_READ         EQU     0x97
RUN             EQU     0x98

; RESPONSES
OKAY            EQU     'O'
ERRCMD          EQU     'E'
ERRSUM          EQU     'S'

; OPTIONS
OPTIONS         EQU     0

;-------------------------------------------------------------------------------
; Macros
;-------------------------------------------------------------------------------

; Decrement REG, goto LOC if result not ZERO
DJNZ            MACRO   REG,LOC
                DECFSZ  REG,F
                GOTO    LOC
                ENDM
#INCLUDE        "led.inc"
#INCLUDE        "timeout.inc"

;-------------------------------------------------------------------------------
; Absolute Variables
;-------------------------------------------------------------------------------

CBLOCK          0x20
    buffer      :       (ROWSIZE * 2)       ; Buffer
    bufsum      :       1                   ; Buffer Checksum
ENDC

CBLOCK          0x70
    buflen      :       1                   ; Buffer Length
    checksum    :       1                   ; Checksum Accumulator
    command     :       1                   ; Command
    count       :       1                   ; Counter
    timer1      :       1                   ; Timer
    timer2      :       1                   ;   "
    timer3      :       1                   ;   "
ENDC

;-------------------------------------------------------------------------------
; GOTO Boot Loader
;-------------------------------------------------------------------------------

                ORG     0
                MOVLB   0                   ; 0
                MOVLP   STARTADDR >> 8      ; 1
                GOTO    STARTADDR           ; 2
                NOP                         ; 3

                ORG     4
                RETFIE

;-------------------------------------------------------------------------------
; GOTO App.
;-------------------------------------------------------------------------------

                ORG     STARTADDR - 6
GOTOAPP
                NOP                         ; MOVLB 0
                NOP                         ; MOVLP 0
                NOP                         ; 0
                NOP                         ; 1
                NOP                         ; 2
                NOP                         ; 3

;-------------------------------------------------------------------------------
; Begin Boot Loader
;-------------------------------------------------------------------------------

                ORG     STARTADDR

                INIT
#IFDEF SWITCH
                BANKSEL DDR                 ; BANK 1 | 0
                BSF     DDR,SWITCH          ; BOOT SWITCH I/P
                BANKSEL INPUT               ; BANK 0
                BTFSC   INPUT,SWITCH        ; BOOT SWITCH TEST
                BRA     GOTOAPP             ; GOTO APP IF HIGH
#ENDIF
                LEDON                       ; BOOT LED ON
                BANKSEL DDR                 ; BANK 1 | 0
                BCF     DDR,LED             ; BOOT LED O/P
INITAGAIN
                CALL    InitComms

;-------------------------------------------------------------------------------
; Receive and Perform Command
;-------------------------------------------------------------------------------
OKCOMPUTER
                CALL    Receive
                MOVWF   command             ; Save command
                MOVWF   checksum            ; Initialise checksum

                MOVLW   VERSION             ; VERSION?
                SUBWF   command,W
                BZ      DOVERSION           ; VERSION...

                BNC     OKCOMPUTER          ; IGNORE <

                MOVLW   HELLO               ; HELLO?
                SUBWF   command,W
                BZ      HELLOWORLD          ; HELLO...

                MOVLW   RUN                 ; RUN?
                SUBWF   command,W
                BZ      RUNAPP              ; RUN...

                BC      OKCOMPUTER          ; IGNORE >

                BRA     FLASHEEPROM         ; FLASH...

;-------------------------------------------------------------------------------
; Perform RUN Command
;-------------------------------------------------------------------------------
RUNAPP
                LEDOFF                      ; BOOT LED OFF
                BRA     GOTOAPP

;-------------------------------------------------------------------------------
; Perform VERSION Command
;-------------------------------------------------------------------------------
DOVERSION
                MOVLW   PROTOCOL
                CALL    Send

                BRA     COMPUTERSAYSYES

;-------------------------------------------------------------------------------
; Perform HELLO Command
;-------------------------------------------------------------------------------
HELLOWORLD
                MOVLW   WORDSIZE
                CALL    Send

                MOVLW   ROWSIZE
                CALL    Send

                MOVLW   HIGH  ERASESIZE
                CALL    Send
                MOVLW   LOW   ERASESIZE
                CALL    Send

                MOVLW   UPPER STARTADDR     ; = 0
                CALL    Send
                MOVLW   HIGH  STARTADDR
                CALL    Send
                MOVLW   LOW   STARTADDR
                CALL    Send
#IFDEF MAX_EE
                MOVLW   MAX_EE              ; X 64
                CALL    Send
#ELSE
                CLRW
                CALL    Send
#ENDIF
                MOVLW   OPTIONS
                CALL    Send

; FOLLOW THROUGH
;-------------------------------------------------------------------------------
; Send Command Response Code
;-------------------------------------------------------------------------------
COMPUTERSAYSYES
                MOVLW   OKAY
COMPUTERSAYSNO
                CALL    Send
                BRA     OKCOMPUTER

;-------------------------------------------------------------------------------
; Perform FLASH/EEPROM Command
;-------------------------------------------------------------------------------
FLASHEEPROM

;-------------------------------------------------------------------------------
; Receive FLASH/EEPROM Address
;-------------------------------------------------------------------------------

                CALL    Receive             ; Upper Byte :- DISCARD

                CALL    Receive             ; High Byte
                BANKSEL MEMADRH             ; BANK 3  EEADRL | PMADRH
                MOVWF   MEMADRH             ; BANK 16 NMVADRH

                CALL    Receive             ; Low Byte
                BANKSEL MEMADRL             ; BANK 3  EEADRH | PMADRL
                MOVWF   MEMADRL             ; BANK 16 NMVADRL

;-------------------------------------------------------------------------------
; Receive FLASH/EEPROM Data Word Count (includes checksum)
;-------------------------------------------------------------------------------

                CALL    Receive
                MOVWF   buflen

;-------------------------------------------------------------------------------
; Receive FLASH/EEPROM Data Words (includes checksum)
;-------------------------------------------------------------------------------

                MOVWF   count
                ADDWF   count,F
                DECF    count,F             ; BYTES = WORDS x 2 + 1

                CLRF    FSR0H
                MOVLW   buffer
                MOVWF   FSR0L
ReceiveLoop
                CALL    Receive
                MOVWI   FSR0++

                DJNZ    count,ReceiveLoop

;-------------------------------------------------------------------------------
; Validate FLASH/EEPROM Checksum
;-------------------------------------------------------------------------------

                MOVF    checksum,F
                BZ      FlashCommand

                MOVLW   ERRSUM
                BRA     COMPUTERSAYSNO

;-------------------------------------------------------------------------------
; Perform FLASH/EEPROM Command
;-------------------------------------------------------------------------------
FlashCommand
                BANKSEL MEMCON1             ; BANK 3  EECON1 | PMCON1
                                            ; BANK 16 NVMCON1
                MOVLW   buffer
                MOVWF   FSR0L

                MOVF    command,W
                XORLW   FLASH_ERASE
                BZ      EraseFlash

                MOVF    command,W
                XORLW   FLASH_WRITE
                BZ      WriteFlash

                MOVF    command,W
                XORLW   FLASH_READ
                BZ      ReadFlash
#IFDEF MAX_EE
                MOVF    command,W
                XORLW   EE_ERASE
                BZ      EraseEEPROM

                MOVF    command,W
                XORLW   EE_WRITE
                BZ      WriteEEPROM

                MOVF    command,W
                XORLW   EE_READ
                BZ      ReadEEPROM
#ENDIF
                MOVLW   ERRCMD
                BRA     COMPUTERSAYSNO

;-------------------------------------------------------------------------------
; Erase Flash Row, Then Write Flash Row?
;
;  DS40001853B-page 156
;-------------------------------------------------------------------------------
EraseFlash
                MOVLW   ERASE_FLASH
                CALL    Write

;-------------------------------------------------------------------------------
; Write Flash Row
;
;  DS40001853B-page 158
;-------------------------------------------------------------------------------
WriteFlash
                DECF    buflen,F            ; Skip Checksum
                BZ      COMPUTERSAYSYES     ; Nothing to write...
WriteFlashLoop
                MOVIW   FSR0++
                MOVWF   MEMDATH

                MOVIW   FSR0++
                MOVWF   MEMDATL

                MOVLW   1
                XORWF   buflen,W
                BZ      WriteFlashLast

                MOVLW   WRITE_LATCH
                CALL    Write

                INCF    MEMADRL,F

                DECF    buflen,F
                BRA     WriteFlashLoop
WriteFlashLast
                MOVLW   WRITE_FLASH
                CALL    Write

                BRA     COMPUTERSAYSYES

;-------------------------------------------------------------------------------
; Read Flash Row
;
;  DS40001853B-page 153
;-------------------------------------------------------------------------------
ReadFlash
                MOVLW   ROWSIZE
                MOVWF   count
ReadFlashLoop   
                MOVLW   READ_FLASH
                MOVWF   MEMCON1
                NOP
                NOP

                MOVF    MEMDATL,W
                CALL    Send

                BANKSEL MEMDATH             ; BANK 3  EEDATH | PMDATH
                MOVF    MEMDATH,W           ; BANK 16 NVMDATH
                CALL    Send

                BANKSEL MEMADRL             ; BANK 3  EEADRL | PMADRL
                INCF    MEMADRL,F           ; BANK 16 NVMADRL

                DJNZ    count,ReadFlashLoop

                BRA     COMPUTERSAYSYES

;-------------------------------------------------------------------------------
; Erase EEPROM Byte
;-------------------------------------------------------------------------------
#IFDEF MAX_EE
EraseEEPROM
                MOVLW   0xFF
                MOVWF   MEMDATL
                MOVLW   (1 << WREN)         ; EEPROM WRITE
                CALL    Write

                BRA     COMPUTERSAYSYES

;-------------------------------------------------------------------------------
; Write EEPROM Byte
;-------------------------------------------------------------------------------
WriteEEPROM
                MOVIW   FSR0
                MOVWF   MEMDATL
                MOVLW   (1 << WREN)         ; EEPROM WRITE
                CALL    Write

                BRA     COMPUTERSAYSYES

;-------------------------------------------------------------------------------
; Read EEPROM Byte
;-------------------------------------------------------------------------------
ReadEEPROM
                MOVLW   (1 << RD)           ; EEPROM READ
                MOVWF   MEMCON1
                NOP
                MOVF    MEMDATL,W
                CALL    Send

                BRA     COMPUTERSAYSYES
#ENDIF 
;-------------------------------------------------------------------------------
; Write (BANK 3)
;
;  DS40001853B-page 155
;-------------------------------------------------------------------------------
Write
                MOVWF   MEMCON1             ; Enable Write To Memory

                ; Unlock Sequence
                MOVLW   0x55
                MOVWF   MEMCON2
                MOVLW   0xAA
                MOVWF   MEMCON2

                BSF     MEMCON1,WR          ; Start Erase/Write (CPU stall)
                NOP
                NOP
WriteWait
                BTFSC   MEMCON1,WR
                BRA     WriteWait

                BCF     MEMCON1,WREN        ; Disable Write To Memory
                RETURN

;-------------------------------------------------------------------------------
; Communications
;-------------------------------------------------------------------------------
#IFDEF UART
    #INCLUDE    "uart.inc"
#ENDIF
#IFDEF USOFT
    #INCLUDE    "usoft.inc"
#ENDIF
;-------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
