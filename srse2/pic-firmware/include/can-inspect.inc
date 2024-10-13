;------------------------------------------------------------------------------
;
; CANBuster CAN inspect
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
                MOVF    CANFRAME+1,F
                BTFSC   STATUS,Z
                BRA     SYSYTEM_CMD

                ; System CMD 0x01 (ID ?)
                MOVF    CANFRAME+1,W
                XORLW   1
                BTFSC   STATUS,Z
                BRA     SYSTEM_CMD_R

                ; System CMD reponse 0x01 ? (looking for on/off state)
                MOVF    CANFRAME+1,W
                XORLW   0x01
                BTFSC   STATUS,Z
                BRA     SYSTEM_CMD_RESPONSE

                ; Switch Accessoir 0x16 (Zubehoer schalten)?
                MOVF    CANFRAME+1,W
                XORLW   0x16
                BTFSC   STATUS,Z
                BRA     CAN_SWITCH_ACC

                ; S88 Event Status 0x22 ?
                MOVF    CANFRAME+1,W
                XORLW   0x22
                BTFSC   STATUS,Z
                BRA     S88_EVENT

                ; CAN PING 0x30 ?
                MOVF    CANFRAME+1,W
                XORLW   0x30
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

                ; send CONFIG data ?
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
; System Sub CMD Reset 0x80 -> reboot - e.g. for updates
;------------------------------------------------------------------------------

CAN_REBOOT
                GOTO	0

;------------------------------------------------------------------------------
; answer System Command (0x00)
;------------------------------------------------------------------------------

SYSYTEM_CMD
                RCALL   CHECK_ID
                BNZ     SYSYTEM_CMD_EXIT

                ; System Command & Sub Command 0x0B
                MOVF    CANFRAME+9,W
                XORLW   0x0B
                BNZ     SYSYTEM_CMD_NEXT

                ; DLC = 6 Status
                MOVF    CANFRAME+4,W
                XORLW   6
                BNZ     SYSYTEM_CMD_NEXT1
                ; TODO
                BRA     CHANNEL_GET

SYSYTEM_CMD_NEXT1
                ; DLC = 8 Configuration
                MOVF    CANFRAME+4,W
                XORLW   8
                BZ      SETUP_VALUE
                RETURN

SYSYTEM_CMD_NEXT
                ; Reboot ?
                ; DLC = 6
                MOVF    CANFRAME+4,W
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
; System CMD response 0x01 / status on/off
;------------------------------------------------------------------------------

SYSTEM_CMD_RESPONSE
                MOVF    CANFRAME+4,W
                XORLW   5
                BTFSS   STATUS,Z
                RETURN
                MOVFF   CANFRAME+9,POWER_ONOFF
                ; mark as new state
                MOVLW   0x80
                XORWF   POWER_ONOFF
                RETURN

;------------------------------------------------------------------------------
; System CMD 0x0B -> channel value
;------------------------------------------------------------------------------

CHANNEL_GET
                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L
                CLRF    POSTINC0
                MOVLW   0x01
                MOVWF   POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW   8
                MOVWF   POSTINC0
                MOVFF   CANFRAME+5,POSTINC0
                MOVFF   CANFRAME+6,POSTINC0
                MOVFF   CANFRAME+7,POSTINC0
                MOVFF   CANFRAME+8,POSTINC0
		; system sub command 0x0B
                MOVFF   CANFRAME+9,POSTINC0
                ; number of channel
                MOVFF   CANFRAME+10,POSTINC0
                MOVFF   CANFRAME+10,WREG
                XORLW   2
                BNZ     CHANNEL_CHECK_3
                MOVFF   S88C_H,POSTINC0
                MOVFF   S88C_L,POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_CHECK_3
                MOVFF   CANFRAME+10,WREG
                XORLW   3
                BNZ     CHANNEL_CHECK_4
                MOVLW	0
                MOVWF   POSTINC0
                MOVFF   S88LENGTH,POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_CHECK_4
                MOVFF   CANFRAME+10,WREG
                XORLW   4
                BNZ     CHANNEL_CHECK_UNKNOWN
                MOVLW	0
                MOVWF   POSTINC0
                MOVFF   SCAN_INTERVAL,POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_CHECK_UNKNOWN
                MOVLW	0xFF
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; System CMD 0x0B -> save value
;------------------------------------------------------------------------------

SETUP_VALUE
                ; setup ID request ?
                MOVLW   0x01
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT1
                MOVFF   CANFRAME+11,ID_H
                MOVFF   CANFRAME+12,ID_L
                BRA     SETUP_VALUE_WRITE
SETUP_VALUE_CONT1
                MOVLW   0x02
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT2
                MOVFF   CANFRAME+11,S88C_H
                MOVFF   CANFRAME+12,S88C_L
                BRA     SETUP_VALUE_WRITE
SETUP_VALUE_CONT2
                MOVLW   0x03
                XORWF   CANFRAME+10,W
                BNZ     SETUP_VALUE_CONT3
                MOVFF   CANFRAME+12,S88LENGTH
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
                CLRF    POSTINC0
                MOVLW   0x01
                MOVWF   POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW   7
                MOVWF   POSTINC0
                MOVFF   CANFRAME+5,POSTINC0
                MOVFF   CANFRAME+6,POSTINC0
                MOVFF   CANFRAME+7,POSTINC0
                MOVFF   CANFRAME+8,POSTINC0
                MOVFF   CANFRAME+9,POSTINC0
                MOVFF   CANFRAME+10,POSTINC0
                ; 1 -> success
                MOVLW   1
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

                RETURN

;------------------------------------------------------------------------------
; System CMD 0x0C -> save Device ID value
;------------------------------------------------------------------------------

SYSTEM_CMD_R
                RCALL   CHECK_ID
                BNZ     SYSTEM_CMD_R_EXIT

                ; System Command & Sub Command 0x0C
                MOVF    CANFRAME+9,W
                XORLW   0x0C
                BNZ     SYSTEM_CMD_R_NEXT

                MOVFF   CANFRAME+10,DEVID_H
                MOVFF   CANFRAME+11,DEVID_L
SYSTEM_CMD_R_NEXT
SYSTEM_CMD_R_EXIT
                RETURN

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
; S88 Event
;------------------------------------------------------------------------------

S88_EVENT
                ; test S88 bus length and return if 0
                MOVFF	S88LENGTH,WREG
                IORLW	0
                ; return if 0
                BTFSC	STATUS,Z
                RETURN

                ; Devide ID check
                MOVF    CANFRAME+5,W
                XORWF   DEVID_H,W
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+6,W
                XORWF   DEVID_L,W
                BTFSS   STATUS,Z
                RETURN

                MOVF	CANFRAME+4,W
                XORLW	4
                BTFSC	STATUS,Z
                BRA     S88_SINGLE_IO
                ; if DLC != 7 -> return
                MOVF	CANFRAME+4,W
                XORLW	7
                BTFSS	STATUS,Z
                RETURN

                ; if not S88 Counter Start -> return
                MOVF	CANFRAME+7,W
                XORWF   S88C_H,W
                BTFSS	STATUS,Z
                RETURN
                MOVF	CANFRAME+8,W
                XORWF   S88C_L,W
                BTFSS	STATUS,Z
                RETURN
                ; skip if no read command
                MOVF	CANFRAME+11,W
                XORLW	1
                BTFSS	STATUS,Z
                RETURN

                ; reset S88 as we need fresh infos
                GOTO	S88INIT

S88_SINGLE_IO
                MOVFF	CANFRAME+7,TEMP16
                MOVFF	CANFRAME+8,TEMP16+1
                MOVF	S88C_L,W
                SUBWF	TEMP16+1
                MOVF	S88C_H,W
                BTFSS	STATUS,C
                INCFSZ	S88C_H,W
                SUBWF	TEMP16,F

                ; skip if IO > 256
                MOVF    TEMP16,W
                XORLW   0
                BTFSS	STATUS,Z
                RETURN
                ; skip if IO > module number * 16
                MOVFF	S88LENGTH,WREG
                SWAPF	WREG
                ADDLW	1
                BCF	STATUS,C
                SUBFWB	TEMP16+1,W
                BTFSS	STATUS,C
                RETURN

                MOVF	TEMP16+1,W
                CALL	S88_N_GENEVENT
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
                ; Version 1.16
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	16
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
                ; cold state ?
                MOVF    STATE,W
                IORLW   0
                ; return if != 0 (warm state)
                BTFSS   STATUS,Z
                RETURN

                ; if DLC == 0
                MOVF	CANFRAME+4,F
                BZ	CAN_BOOTLOADER_PING
                ; if DLC == 5
                MOVF	CANFRAME+4,W
                XORLW   5
                BZ	CAN_BOOTLOADER_SYSTEM
                RETURN

CAN_BOOTLOADER_SYSTEM
                RCALL	CHECK_ID
                BNZ	CAN_BOOTLOADER_EXIT
                ; check for CAN-Bootloader Go 0x11
                MOVF	CANFRAME+9,W
                XORLW   0x11
                BZ	CAN_BOOTLOADER_GO
CAN_BOOTLOADER_EXIT
                RETURN

CAN_BOOTLOADER_PING
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
                ; Version 1.16
                MOVLW	1
                MOVWF	POSTINC0
                MOVLW	16
                MOVWF	POSTINC0
                MOVLW	0
                MOVWF	POSTINC0
                ; this is a back detector (german Rueckmelder)
                MOVLW	0x40
                MOVWF	POSTINC0
                BRA	EVENT_RING_NORMALIZE

CAN_BOOTLOADER_GO
                ; we are now in warm state
                MOVLW   1
                MOVWF   STATE
                ; respond to CAN Bootloader Go with System Reset CAN Frame
                ; w/o doing something
                MOVFF	EVENTPP,FSR0H
                MOVFF	EVENTPP+1,FSR0L
                CLRF	POSTINC0
                MOVLW	1
                MOVWF	POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                MOVLW	6
                MOVWF	POSTINC0
                MOVLW	'C'
                MOVWF	POSTINC0
                MOVLW	'B'
                MOVWF	POSTINC0
                MOVFF	ID_H,POSTINC0
                MOVFF	ID_L,POSTINC0
                MOVLW	0x80
                MOVWF	POSTINC0
                MOVLW	0
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
