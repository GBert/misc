;------------------------------------------------------------------------------
;
; CANBuster CAN inspect
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
; inspector
;------------------------------------------------------------------------------

CANINSPECT
                BTFSS	CANFRAME,6	; a new CAN frame ?
                RETURN

                BCF	CANFRAME,6	; mark as processed

                ; we are looking only for extended frames here
                BTFSS	CANFRAME,7	; extended CAN frame ?
                RETURN

                ; System CMD 0x00 ?
                MOVF	CANFRAME+1,F
                BTFSC   STATUS,Z
                BRA     SYSYTEM_CMD

                ; Switch Accessoir 0x16 (Zubehoer schalten)?
                MOVF	CANFRAME+1,W
                XORLW	0x16
                BTFSC   STATUS,Z
                BRA     CAN_SWITCH_ACC

                ; CAN PING 0x30 ?
                MOVF	CANFRAME+1,W
                XORLW	0x30
                BTFSC   STATUS,Z
                BRA     CAN_PING

                ; CAN PING 0x31 (CS2.exe) ?
                MOVF    CANFRAME+1,W
                XORLW   0x31
                BTFSC   STATUS,Z
                BRA     CAN_PING2

                ; CAN BOOTLOADER Service 0x36 ?
                MOVF    CANFRAME+1,W
                XORLW   0x36
                BTFSC   STATUS,Z
                BRA     CAN_BOOTLOADER

                ; send CONFIG data
                MOVF    CANFRAME+1,W
                XORLW   0x3A
                BTFSC   STATUS,Z
                BRA     SEND_CONFIGURATION

                RETURN

;------------------------------------------------------------------------------
; generic routines
;------------------------------------------------------------------------------

CHECK_ID
                MOVF    CANFRAME+5,W
                XORLW   'C'
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+6,W
                XORLW   'B'
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+7,W
                XORWF   ID_H,W
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+8,W
                XORWF   ID_L,W
                ; return now anyway
                RETURN

;------------------------------------------------------------------------------
; be sure the pointer is in the appropiate range
;------------------------------------------------------------------------------

EVENT_RING_NORMALIZE
                MOVLW   15
                ADDWF   FSR0L,F
                BNC     NORM_POINTER1
                INCF    FSR0H,F
NORM_POINTER1
                MOVLW   0xF0
                ANDWF   FSR0L,F

                MOVLW	0x04            ; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF	FSR0H,F
                MOVLW	0x07
                ANDWF	FSR0H,F
                MOVFF	FSR0H,EVENTPP
                MOVFF	FSR0L,EVENTPP+1

                INCF	EVENTCNT,F
                RETURN

;------------------------------------------------------------------------------
; answer System Command (0x00)
;------------------------------------------------------------------------------

SYSYTEM_CMD
                RCALL   CHECK_ID
                BNZ	SYSYTEM_CMD_EXIT
                ; Configuration DLC = 8 & Sub Command 0x0b
                ; DLC = 8
                MOVF	CANFRAME+4,W
                XORLW	8
                BNZ	SYSYTEM_CMD_NEXT1
                MOVF    CANFRAME+9,W
                XORLW	0x0B
                BZ	SETUP_VALUE
SYSYTEM_CMD_NEXT1
                ; Reboot ?
                ; DLC = 6
                MOVF	CANFRAME+4,W
                XORLW	6
                BNZ	SYSYTEM_CMD_NEXT2
                MOVF    CANFRAME+9,W
                XORLW	0x80
                BZ	CAN_REBOOT

SYSYTEM_CMD_NEXT2
SYSYTEM_CMD_EXIT
                RETURN

                ; for testing
SYSYTEM_CMD_OTHER
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x01 as System CMD Response
                MOVLW	0x00
                MOVWF	POSTINC0
                MOVLW	0x01
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVF	CANFRAME+4,W
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                BRA	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; System Sub CMD Reset 0x0B -> save value
;------------------------------------------------------------------------------

SETUP_VALUE
                ; setup ID request ?
                MOVLW   0x01
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT1
                MOVFF   CANFRAME+11,ID_H
                MOVLW   'U'
                ADDWF   ID_H
                MOVFF   CANFRAME+12,ID_L
                MOVLW   'S'
                ADDWF   ID_L
                BRA     SETUP_VALUE_WRITE
SETUP_VALUE_CONT1
                MOVLW   0x02
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT2
                MOVFF   CANFRAME+11,PIOC_H
                MOVFF   CANFRAME+12,PIOC_L
                BRA     SETUP_VALUE_WRITE
SETUP_VALUE_CONT2
                MOVLW   0x03
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT3
                MOVFF   CANFRAME+11,S88C_H
                MOVFF   CANFRAME+12,S88C_L
                BRA     SETUP_VALUE_WRITE
SETUP_VALUE_CONT3
                MOVLW   0x04
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT4
                MOVFF   CANFRAME+12,SCAN_INTERVAL
                BRA     SETUP_VALUE_WRITE

SETUP_VALUE_CONT4
                ; exit
                RETURN

SETUP_VALUE_WRITE
                CALL    SET_UID
                CALL    READCANHASH

SETUP_VALUE_RESPONSE
                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L
                CLRF	POSTINC0
                MOVLW	0x01
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW	7
                MOVWF	POSTINC0
                MOVFF   CANFRAME+5,POSTINC0
                MOVFF   CANFRAME+6,POSTINC0
                MOVFF   CANFRAME+7,POSTINC0
                MOVFF   CANFRAME+8,POSTINC0
                MOVFF   CANFRAME+9,POSTINC0
                MOVFF   CANFRAME+10,POSTINC0
                ; 1 -> success
                MOVLW   1
                MOVWF   POSTINC0
                BRA	EVENT_RING_NORMALIZE

                RETURN

;------------------------------------------------------------------------------
; System Sub CMD Reset 0x80 -> reboot - e.g. for updates
;------------------------------------------------------------------------------

CAN_REBOOT
                GOTO	0

;------------------------------------------------------------------------------
; Switch Accessois (0x1B)
;------------------------------------------------------------------------------

CAN_SWITCH_ACC
                ; if DLC != 8 -> RETURN
                MOVF	CANFRAME+4,W
                XORLW	8
                BZ      CAN_SWITCH_ACC_NEXT1
                RETURN

CAN_SWITCH_ACC_NEXT1
                RCALL   CHECK_ID
                BTFSS   STATUS,Z
                RETURN

                ; fix servo number if needed
                MOVFF   CANFRAME+10,TEMP
                MOVLW   3
                ANDWF   TEMP,F
                RLNCF   TEMP,F

                ; store new PWM value
                ; TODO: needs range verification
#IFDEF PWM_FW
                LFSR    FSR0,PWM_VALUE1
                MOVF    TEMP,W
                MOVFF   CANFRAME+11,PLUSW0
                INCF    WREG,F
                MOVFF   CANFRAME+12,PLUSW0
#ENDIF

                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L
                CLRF	POSTINC0
                MOVLW	0x17
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW	7
                MOVWF	POSTINC0

                MOVFF   CANFRAME+5,POSTINC0
                MOVFF   CANFRAME+6,POSTINC0
                MOVFF   CANFRAME+7,POSTINC0
                MOVFF   CANFRAME+8,POSTINC0

                MOVFF   CANFRAME+11,PLUSW0
                MOVFF   CANFRAME+12,PLUSW0

                ; 01 -> success
                MOVLW   0x01
                MOVWF   POSTINC0
                BRA	EVENT_RING_NORMALIZE

CAN_SWITCH_ACC_FAULT
                RETURN

;------------------------------------------------------------------------------
; answer CAN Ping (0x30)
;------------------------------------------------------------------------------

CAN_PING
                ; if DLC != 0 -> return
                MOVF	CANFRAME+4,F
                BZ	CAN_PING_CONT
                RETURN

CAN_PING2
                ; if DLC != 8 -> return
                MOVF	CANFRAME+4,W
                XORLW	8
                BTFSS	STATUS,Z
                RETURN
                ; check for CS2.exe ping with response bit
                ; cs2.exe ping looks like 00310000#xxxxxxxxxxxxEEEE
                MOVF    CANFRAME+11,W
                XORLW   0xEE
                BTFSS	STATUS,Z
                RETURN
                MOVF    CANFRAME+12,W
                XORLW   0xEE
                BTFSS	STATUS,Z
                RETURN

CAN_PING_CONT
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x31 as CAN Ping response
                CLRF	POSTINC0
                MOVLW	0x31
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW	0x8
                MOVWF	POSTINC0
                MOVLW	'C'
                MOVWF	POSTINC0
                MOVLW	'B'
                MOVWF	POSTINC0
                MOVFF	ID_H,POSTINC0
                MOVFF	ID_L,POSTINC0
                ; Version 1.1
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                ; this is a back detector (german Rueckmelder)
                MOVLW	0x40
                MOVWF	POSTINC0
                BRA	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; CAN Bootloader Service (0x36)
;------------------------------------------------------------------------------

CAN_BOOTLOADER
                ; if DLC != 0 -> return
                MOVF	CANFRAME+4,F
                BZ	CAN_BOOTLOADER_CONT
                RETURN

CAN_BOOTLOADER_CONT
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                ; mark with 0x37 as CAN Bootloader response
                CLRF	POSTINC0
                MOVLW	0x37
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW	0x8
                MOVWF	POSTINC0
                MOVLW	'C'
                MOVWF	POSTINC0
                MOVLW	'B'
                MOVWF	POSTINC0
                MOVFF	ID_H,POSTINC0
                MOVFF	ID_L,POSTINC0
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                MOVWF	POSTINC0
                ; this is a back detector (german Rueckmelder)
                MOVLW	0x40
                MOVWF	POSTINC0
                BRA	EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; Setup Command (0x3A)
;------------------------------------------------------------------------------

SEND_CONFIGURATION
                ; if DLC != 5 -> return
                MOVF	CANFRAME+4,W
                XORLW	5
                BTFSS   STATUS,Z
                RETURN
                RCALL   CHECK_ID
                BTFSS   STATUS,Z
                RETURN

SEND_CONF_CONT
                MOVF    CANFRAME+9,W
                MOVWF   INDEX
                XORLW   0
                BNZ     SEND_CONF_CONT1
                LOAD_INDEX_P INDEX0
                GOTO    SEND_INDEX

SEND_CONF_CONT1
                MOVF    CANFRAME+9,W
                XORLW   1
                BNZ     SEND_CONF_CONT2
                LOAD_INDEX_P INDEX1
                GOTO    SEND_INDEX

SEND_CONF_CONT2
                MOVF    CANFRAME+9,W
                XORLW   2
                BNZ     SEND_CONF_CONT3
                LOAD_INDEX_P INDEX2
                GOTO    SEND_INDEX

SEND_CONF_CONT3
                MOVF    CANFRAME+9,W
                XORLW   3
                BNZ     SEND_CONF_CONT4
                LOAD_INDEX_P INDEX3
                GOTO    SEND_INDEX

SEND_CONF_CONT4
                MOVF    CANFRAME+9,W
                XORLW   4
                BNZ     SEND_CONF_CONT5
                LOAD_INDEX_P INDEX4
                GOTO    SEND_INDEX

SEND_CONF_CONT5

#IFDEF PWM
                MOVF    CANFRAME+9,W
                XORLW   5
                BNZ     SEND_CONF_CONT6
                LOAD_INDEX_P INDEX5
                GOTO    SEND_INDEX
#ENDIF
SEND_CONF_CONT6
                RETURN
;
; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
