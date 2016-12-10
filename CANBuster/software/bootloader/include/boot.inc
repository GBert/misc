;------------------------------------------------------------------------------
;
;   The Wellington Boot Loader for PIC18
;
;   The Boot Loader
;
;   Copyright (c) 2014 - 2015 Darron M Broad
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;
;   Wellington Boot Loader uses methods derived from Chiculita Claudiu's
;
;   Tiny PIC Bootloader
;
;   Copyright (c) 2003-2008 Claudiu Chiculita
;
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
;   This file is part of The Wellington Boot Loader.
;
;   The Wellington Boot Loader is free software: you can redistribute it and/or
;   modify it under the terms of the GNU General Public License as published
;   by the Free Software Foundation.
;
;   The Wellington Boot Loader is distributed in the hope that it will be
;   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;   GNU General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with The Wellington Boot Loader. If not, see http://www.gnu.org/licenses/
;------------------------------------------------------------------------------

                LIST
                RADIX       DEC

;------------------------------------------------------------------------------
; Boot Loader Origin
;------------------------------------------------------------------------------

#IF BOOTSIZE % ERASESIZE
ERASEROWS       EQU         (BOOTSIZE / ERASESIZE) + 1
#ELSE
ERASEROWS       EQU         (BOOTSIZE / ERASESIZE)
#ENDIF

; Boot Loader Origin is N Erase Rows from The End Of Flash
STARTADDR       EQU         MAX_FLASH - (ERASEROWS * ERASESIZE)

;------------------------------------------------------------------------------
; Boot Loader Protocol
;------------------------------------------------------------------------------

; COMMANDS
HELLO           EQU     0x80
FLASH_ERASE     EQU     0x81
FLASH_WRITE     EQU     0x82
FLASH_READ      EQU     0x83
EE_WRITE        EQU     0x84
EE_READ         EQU     0x85
RUN             EQU     0x86

; RESPONSES
OKAY            EQU     'O'
ERRCMD          EQU     'E'
ERRSUM          EQU     'S'

;------------------------------------------------------------------------------
; Timeout macros
;------------------------------------------------------------------------------

#INCLUDE        "timeout.inc"

;------------------------------------------------------------------------------
; Variables
;------------------------------------------------------------------------------

blen            EQU     0x000       ; Buffer Length
buffer          EQU     0x100       ; Buffer + Checksum

csum            EQU     0x001       ; Checksum
cmd             EQU     0x002       ; Command
cnt             EQU     0x003       ; Counter
wtemp           EQU     0x004       ; Temporary

timer1          EQU     0x005       ; Timer
timer2          EQU     0x006       ;   "
timer3          EQU     0x007       ;   "
timer4          EQU     0x008       ;   "

wait            EQU     0x009

;------------------------------------------------------------------------------
; GOTO Boot Loader
;------------------------------------------------------------------------------

                ORG     0
                GOTO    STARTADDR

;------------------------------------------------------------------------------
; GOTO App.
;------------------------------------------------------------------------------

                ORG     STARTADDR - 4
GOTOAPP
                NOP
                NOP

;------------------------------------------------------------------------------
; Boot Loader
;------------------------------------------------------------------------------

                ORG     STARTADDR
                INIT
                RCALL   InitComms

;------------------------------------------------------------------------------
; Begin
;------------------------------------------------------------------------------
#IFDEF SWITCH
                BSF     DDR,SWITCH      ; BOOT SWITCH I/P
                BTFSS   INPUT,SWITCH
                BRA     GOTOAPP
#ENDIF
                BSF     OUTPUT,LED      ; BOOT LED RESET
                BCF     DDR,LED         ; BOOT LED O/P

;------------------------------------------------------------------------------
; Receive Command
;------------------------------------------------------------------------------
OKCOMPUTER
                RCALL   Receive
                MOVWF   cmd
 
                MOVLW   HELLO           ; < HELLO?
                SUBWF   cmd,W
                BNC     OKCOMPUTER      ; IGNORE

                MOVLW   (RUN + 1)       ; > RUN?
                SUBWF   cmd,W
                BC      OKCOMPUTER      ; IGNORE
       
;------------------------------------------------------------------------------
; Perform command
;------------------------------------------------------------------------------

                MOVF    cmd,W
                XORLW   RUN
                BNZ     CONTINUE

                SETF    wait            ; wait a little bit before
WAIT_LOOP
                DECFSZ  wait,f
                BRA     WAIT_LOOP

                BRA     GOTOAPP         ; RUN...

CONTINUE
                MOVF    cmd,W
                XORLW   HELLO
                BNZ     FLASHEEPROM     ; FLASH...

; FOLLOW THROUGH
;------------------------------------------------------------------------------
; Peform HELLO Operation
;------------------------------------------------------------------------------
HELLOWORLD
                MOVLW   ROWSIZE
                RCALL   Send

                MOVLW   HIGH  ERASESIZE
                RCALL   Send
                MOVLW   LOW   ERASESIZE
                RCALL   Send

                MOVLW   UPPER STARTADDR
                RCALL   Send
                MOVLW   HIGH  STARTADDR
                RCALL   Send
                MOVLW   LOW   STARTADDR
                RCALL   Send
#IFDEF MAX_EE
                MOVLW   HIGH  MAX_EE
                RCALL   Send
                MOVLW   LOW   MAX_EE
                RCALL   Send
#ELSE
                CLRW
                RCALL   Send
                CLRW
                RCALL   Send
#ENDIF
; FOLLOW THROUGH
;------------------------------------------------------------------------------
; Send Command Response Code
;------------------------------------------------------------------------------
COMPUTERSAYSYES
                MOVLW   OKAY
COMPUTERSAYSNO
                RCALL   Send
                BRA     OKCOMPUTER

;------------------------------------------------------------------------------
; Perform FLASH/EEPROM Command
;------------------------------------------------------------------------------
FLASHEEPROM
                MOVFF   cmd,csum

;------------------------------------------------------------------------------
; Receive FLASH/EEPROM Address
;------------------------------------------------------------------------------

                RCALL   Receive         ; Upper Byte
                MOVWF   TBLPTRU

                RCALL   Receive         ; High Byte
                MOVWF   TBLPTRH
#IFDEF EEADRH 
                MOVWF   EEADRH
#ENDIF
                RCALL   Receive         ; Low Byte
                MOVWF   TBLPTRL
#IFDEF EEADR
                MOVWF   EEADR
#ENDIF
;------------------------------------------------------------------------------
; Receive FLASH/EEPROM Data Byte Count
;------------------------------------------------------------------------------

                RCALL   Receive
                MOVWF   blen

;------------------------------------------------------------------------------
; Receive FLASH/EEPROM Data Bytes
;------------------------------------------------------------------------------

                LFSR    FSR0,buffer
                MOVWF   cnt
ReceiveLoop
                RCALL   Receive
                MOVWF   POSTINC0
                DECF    cnt,F
                BNZ     ReceiveLoop

;------------------------------------------------------------------------------
; Validate FLASH/EEPROM Checksum
;------------------------------------------------------------------------------

                MOVF    csum,F
                BZ      FlashCommand

                MOVLW   ERRSUM
                BRA     COMPUTERSAYSNO

;------------------------------------------------------------------------------
; Peform FLASH/EEPROM Operation
;------------------------------------------------------------------------------
FlashCommand
                LFSR    FSR0,buffer

                MOVF    cmd,W
                XORLW   FLASH_ERASE
                BZ      EraseFlash

                MOVF    cmd,W
                XORLW   FLASH_WRITE
                BZ      WriteFlash

                MOVF    cmd,W
                XORLW   FLASH_READ
                BZ      ReadFlash

                MOVF    cmd,W
                XORLW   EE_WRITE
                BZ      WriteEEPROM

                BRA     ReadEEPROM

;------------------------------------------------------------------------------
; Erase Flash Row, Then Write Flash Row?
;
;  DS39761B-page 100
;------------------------------------------------------------------------------
EraseFlash
                MOVLW   ERASE_FLASH
                RCALL   Write

;------------------------------------------------------------------------------
; Write Flash Row
;
;  DS39761B-page 102
;------------------------------------------------------------------------------
WriteFlash
                DECF    blen,F          ; Skip Checksum
                BZ      COMPUTERSAYSYES ; Nothing to write...
WriteFlashLoop
                MOVFF   POSTINC0,TABLAT
                TBLWT   *+
                DECF    blen,F
                BNZ     WriteFlashLoop

                TBLRD   *-
                MOVLW   WRITE_FLASH
                RCALL   Write

                BRA     COMPUTERSAYSYES

;------------------------------------------------------------------------------
; Read Flash Row
;
;  DS39761B-page 100
;------------------------------------------------------------------------------
ReadFlash       
                MOVLW   ROWSIZE
                MOVWF   cnt
ReadFlashLoop   
                TBLRD   *+
                MOVF    TABLAT,W
                RCALL   Send
                DECF    cnt,F
                BNZ     ReadFlashLoop

                BRA     COMPUTERSAYSYES

;------------------------------------------------------------------------------
; Write EEPROM Byte
;
;  DS39761B-page 107
;------------------------------------------------------------------------------
WriteEEPROM     
                MOVFF   buffer,EEDATA
                MOVLW   (1 << WREN)     ; EEPROM WRITE
                RCALL   Write

                BRA     COMPUTERSAYSYES

;------------------------------------------------------------------------------
; Read EEPROM Byte
;
;  DS39761B-page 107
;------------------------------------------------------------------------------
ReadEEPROM
                MOVLW   (1 << RD)       ; EEPROM READ
                MOVWF   EECON1
                NOP
                MOVF    EEDATA,W        ; W = EEDATA
                RCALL   Send

                BRA     COMPUTERSAYSYES

;------------------------------------------------------------------------------
; Write W = EECON1
;
;  DS39761B-page 103
;------------------------------------------------------------------------------
Write
                MOVWF   EECON1          ; Enable Write To Memory

                ; Unlock Sequence
                MOVLW   0x55
                MOVWF   EECON2
                MOVLW   0xAA
                MOVWF   EECON2

                BSF     EECON1,WR       ; Start Erase/Write (CPU stall)
WriteWait       
                BTFSC   EECON1,WR
                BRA     WriteWait

                BCF     EECON1,WREN     ; Disable Write To Memory
                RETURN

;------------------------------------------------------------------------------
; Communications
;------------------------------------------------------------------------------

#IFDEF CAN
    #INCLUDE    "can.inc"
#ELSE
    #INCLUDE    "uart.inc"
#ENDIF

;------------------------------------------------------------------------------
;
; vim: ft=asm shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
