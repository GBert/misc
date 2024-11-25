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
; generic MACROs
;------------------------------------------------------------------------------
VERSION_TO_POSTINC0 MACRO
                ; Version 1.17
                MOVLW   1
                MOVWF   POSTINC0
                MOVLW   17
                MOVWF   POSTINC0
                MOVLW   0
                MOVWF   POSTINC0
                ; this is a back detector (german Rueckmelder)
                MOVLW   0x40
                MOVWF   POSTINC0
                ENDM
;------------------------------------------------------------------------------
; generic routines
;------------------------------------------------------------------------------

CHECK_BROADCAST
                MOVF    CANFRAME+5,W
                ADDWF   CANFRAME+6,W
                ADDWF   CANFRAME+7,W
                ADDWF   CANFRAME+8,W
                ;STAUS Z=1 if broadcast
                RETURN

CHECK_UID
                MOVF    CANFRAME+5,W
                XORLW   'C'
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+6,W
                XORLW   'B'
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+7,W
                XORWF   UID_H,W
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+8,W
                XORWF   UID_L,W
                ; return now anyway
                RETURN

UID_TO_POSTINC0
                MOVLW   'C'
                MOVWF   POSTINC0
                MOVLW   'B'
                MOVWF   POSTINC0
                MOVFF   UID_H,POSTINC0
                MOVFF   UID_L,POSTINC0
                RETURN

CREATE_RESPONSE_HEADER_SYSCMD
                MOVLW   0x01
CREATE_RESPONSE_HEADER
                ; response command in WREG
                IORLW   0x01	;mark as respose
                MOVFF   EVENTPP,FSR0H
                MOVFF   EVENTPP+1,FSR0L
                ; mark with 0x01 as System CMD Response
                CLRF    POSTINC0
                MOVWF   POSTINC0
                MOVFF   CANHASH_HIGH,POSTINC0
                MOVFF   CANHASH_LOW,POSTINC0
                RETURN

; finish frame, be sure the pointer is in the appropiate range
EVENT_RING_NORMALIZE
                MOVLW   15
                ADDWF   FSR0L,F
                BNC     NORM_POINTER1
                INCF    FSR0H,F
NORM_POINTER1
                MOVLW   0xF0
                ANDWF   FSR0L,F

                MOVLW   0x04            ; make sure that FSR is in the range 0x400 - 0x7ff
                IORWF   FSR0H,F
                MOVLW   0x07
                ANDWF   FSR0H,F
                MOVFF   FSR0H,EVENTPP
                MOVFF   FSR0L,EVENTPP+1

                INCF    EVENTCNT,F
                RETURN

;------------------------------------------------------------------------------
; inspector
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; ANALYZE COMMANDS
; check broadcast and addressed packets
;------------------------------------------------------------------------------

CANINSPECT
                BCF     CANFRAME,6	; mark as processed
                ; we are looking only for extended frames here
                BTFSS   CANFRAME,7	; extended CAN frame ?
                RETURN

                ; Analyse Command Byte
                ; switch(CANFRAME+1)
                MOVF    CANFRAME+1,W
                ; System CMD 0x00 ?
                BZ      SYSYTEM_CMD
                ; System CMD Response 0x01 check for Stop/Go
                XORLW   0x01
                BTFSC   STATUS,Z
                BRA     CHECK_STOP_GO
                ; S88 Event Status 0x22 ?
                XORLW   0x22 ^ 0x01
                BTFSC   STATUS,Z
                BRA     S88_EVENT
                ; CAN PING 0x30 ?
                XORLW   0x30 ^ 0x22
                BTFSC   STATUS,Z
                BRA     CAN_PING
                ; CAN PING 0x31 (CS2 PC) ?
                XORLW   0x31 ^ 0x30
                BTFSC   STATUS,Z
                BRA     CAN_PING2
                ; CAN BOOTLOADER Service 0x36 ?
                XORLW   0x36 ^ 0x31
                BTFSC   STATUS,Z
                BRA     CAN_BOOTLOADER
                ; CAN CONFIG Service 0x3A ?
                XORLW   0x3A ^ 0x36
                BTFSC   STATUS,Z
                BRA     SEND_CONFIGURATION
                ; Switch Accessoir 0x16 (Zubehoer schalten)?
                XORLW   0x16 ^ 0x3A
                BTFSC   STATUS,Z
                BRA     CAN_SWITCH_ACC


;------------------------------------------------------------------------------
; ANALIZE Commands 0x00 = SYSTEM COMMANDS
; check broadcast and addressed packets
;------------------------------------------------------------------------------

SYSYTEM_CMD
                RCALL   CHECK_BROADCAST
                BZ      SYSYTEM_CMD_BROADCAST
                RCALL   CHECK_UID
                BZ      SYSYTEM_CMD_ADDRESSED
                RETURN

SYSYTEM_CMD_BROADCAST
; command addressed to 0x00000000 BROADCAST
; check for STOP/GO
CHECK_STOP_GO  ;we come to CHECK_STOP_GO by response analyze CMD=0x00
                ;analyze SUB CMD
                ;switch(CANFRAME+9)
                ;check GO/STOP
                MOVF    CANFRAME+9,W
                BZ      SYSTEM_CMD_SUB_STOP
                XORLW   0x01
                BZ      SYSTEM_CMD_SUB_GO
                RETURN  ;System Command not handled

SYSYTEM_CMD_ADDRESSED
; command addressed to my UID
                ;analyze SUB CMD
                ;switch(CANFRAME+9)
                MOVF    CANFRAME+9,W
                XORLW   0x0B
                BZ      SYSTEM_CMD_SUB_CONFIG
                XORLW   0x0C ^ 0x0B
                BZ      SYSTEM_CMD_SUB_DEVICEID
                XORLW   0x80   ^ 0x0C
                BZ      SYSTEM_CMD_SUB_RESET
                RETURN  ;System Command not handled

;------------------------------------------------------------------------------
; System Command (0x00) SUB Command 0x00 STOP
;------------------------------------------------------------------------------
SYSTEM_CMD_SUB_STOP
                BCF     SYSTEM_FLAGS,SYSTEM_FLAG_STOP_GO
                RETURN
;------------------------------------------------------------------------------
; System Command (0x00) SUB Command 0x01 GO
;------------------------------------------------------------------------------
SYSTEM_CMD_SUB_GO
                BSF     SYSTEM_FLAGS, SYSTEM_FLAG_STOP_GO
                BTFSC   OPTION_FLAGS, OPTION_FLAG_S88ALLONFIRSTGO
                BSF     SYSTEM_FLAGS, SYSTEMFLAG_S88SENDALL_ONFIRST
                RETURN

;------------------------------------------------------------------------------
; System Command (0x00) SUB Command 0x0B Status: Configuration set/get
;------------------------------------------------------------------------------
SYSTEM_CMD_SUB_CONFIG ;ConfigChannel Get/Set
                ; DLC = 6 Status
                MOVF    CANFRAME+4,W
                XORLW   6
                BZ     CHANNEL_GET ;DLC = 6 Get Channel value
                XORLW   8 ^ 6
                BZ     CHANNEL_SET ;DLC = 8 Set Channel value
                RETURN

;------------------------------------------------------------------------------
; System Command (0x00) SUB Command 0x0C DeviceID set/get
;------------------------------------------------------------------------------

SYSTEM_CMD_SUB_DEVICEID ; Get / Set DeviceID by command
                MOVF    CANFRAME+4,W
                XORLW   0x05
                BTFSC   STATUS,Z
                GOTO    DEVICEID_GET
                XORLW   0x07 ^ 0x05
                BTFSC   STATUS,Z
                GOTO    DEVICEID_SET
                RETURN	; unknown

;------------------------------------------------------------------------------
; System Command (0x00) SUB Command 0x80 Reset
;------------------------------------------------------------------------------
SYSTEM_CMD_SUB_RESET  ; Reset
                ; Reboot ?
                ; DLC = 6
                MOVF    CANFRAME+4,W
                XORLW   6
                BTFSC   STATUS,Z
                RESET   ; better use RESET instruction can be detected (RCON)
                ; reboot only, answer will be sent after reset
                ; (RCON: RI = 0 after reset)
                RETURN

CAN_SEND_RESET
                RCALL   CREATE_RESPONSE_HEADER_SYSCMD
                MOVLW   6  ; DLC
                MOVWF   POSTINC0
                RCALL   UID_TO_POSTINC0
                MOVLW   0x80
                MOVWF   POSTINC0
                ;RCON nRI = 0 RESET INSTRUCTION, nRI =1 other Reset
                BTFSS   RCON,RI
                CLRF    POSTINC0       ; RI = 0: by instruction:reason is 0 (by CAN command)
                BTFSC   RCON,RI
                MOVFF   RCON, POSTINC0 ; RI = 1: not by instr. set reason to RCON for debug
                ;mark RCON: RI as handled
                BSF     RCON,RI     ;mark RESET instruction as handled
                BRA     EVENT_RING_NORMALIZE


                ; for testing
SYSYTEM_CMD_OTHER
                RCALL   CREATE_RESPONSE_HEADER_SYSCMD
                MOVF    CANFRAME+4,W ;use DLC received DLC
                MOVWF   POSTINC0
                MOVLW   0
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; System CMD 0x0B -> channel value
;------------------------------------------------------------------------------
; ConfigChannel1 = DeviceID
; ConfigChannel2 = PIO ContactID Offset
; ConfigChannel3 = S88 Length (number 16bit modules)
; ConfigChannel4 = S88 ContactID Offset
; ConfigChannel5 = Scan Interval
; ConfigChannel6 = UID (2 lowest bytes)
; ConfigChannel7 = OPTIONS FLAGS
CHANNEL_GET
                RCALL   CREATE_RESPONSE_HEADER_SYSCMD
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
                MOVF    CANFRAME+10,W
                XORLW   1
                BZ      CHANNEL_GET_CH1
                XORLW   2 ^ 1
                BZ      CHANNEL_GET_CH2
                XORLW   3 ^ 2
                BZ      CHANNEL_GET_CH3
                XORLW   4 ^ 3
                BZ      CHANNEL_GET_CH4
                XORLW   5 ^ 4
                BZ      CHANNEL_GET_CH5
                XORLW   6 ^ 5
                BZ      CHANNEL_GET_CH6
                XORLW   7 ^ 6
                BZ      CHANNEL_GET_CH7
                XORLW   8 ^ 7
                BZ      CHANNEL_GET_CH8
                ; unknown
                ; RETURN	;no valid channel
                MOVLW   0xFF
                MOVWF   POSTINC0
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH1 MOVFF   ID_H, POSTINC0
                MOVFF   ID_L, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH2 MOVFF   PIOC_H, POSTINC0
                MOVFF   PIOC_L, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH3 ; S88 bus length (nr 16 bit modules)
                CLRF    POSTINC0
                MOVFF   S88LENGTH, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH4 ; S88 contactID offset
                MOVFF   S88C_H, POSTINC0
                MOVFF   S88C_L, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH5 ; scan interval
                CLRF    POSTINC0
                MOVFF   SCAN_INTERVAL, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH6 ; UID (2 lower bytes)
                MOVFF   UID_H, POSTINC0
                MOVFF   UID_L, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH7 ; OPTION FLAGS
                CLRF POSTINC0
                MOVFF   OPTION_FLAGS, POSTINC0
                BRA     EVENT_RING_NORMALIZE

CHANNEL_GET_CH8 ; S88 forced event delay
                CLRF    POSTINC0
                MOVFF   S88_FEVENTDELAY_SOLL, POSTINC0
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; System CMD 0x0B -> save value
;------------------------------------------------------------------------------
; ConfigChannel1 = DeviceID
; ConfigChannel2 = PIO ContactID Offset
; ConfigChannel3 = S88 Length (number 16bit modules)
; ConfigChannel4 = S88 ContactID Offset
; ConfigChannel5 = Scan Interval
; ConfigChannel6 = UID (2 lowest bytes)
; ConfigChannel7 = OPTIONS FLAGS

;ToDo write selective if use eeprom
CHANNEL_SET
SETUP_VALUE

                ; switch(setup channel)
                MOVF    CANFRAME+10,W
                XORLW   1
                BZ      CHANNEL_SET_CH1
                XORLW   2 ^ 1
                BZ      CHANNEL_SET_CH2
                XORLW   3 ^ 2
                BZ      CHANNEL_SET_CH3
                XORLW   4 ^ 3
                BZ      CHANNEL_SET_CH4
                XORLW   5 ^ 4
                BZ      CHANNEL_SET_CH5
                XORLW   6 ^ 5
                BZ      CHANNEL_SET_CH6
                XORLW   7 ^ 6
                BZ      CHANNEL_SET_CH7
                XORLW   8 ^ 7
                BZ      CHANNEL_SET_CH8
                RETURN	;channel not supported

CHANNEL_SET_CH1 ; Device ID
                MOVFF   CANFRAME+11,ID_H
                MOVFF   CANFRAME+12,ID_L
                CALL    EEP_WRITE_DEVICEID
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH2 ; PIO contact ID offset
                MOVFF   CANFRAME+11,PIOC_H
                MOVFF   CANFRAME+12,PIOC_L
                CALL    EEP_WRITE_PIOC
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH3 ; S88 length (nr S88 16bit modules)
                MOVF    CANFRAME+12, W
                CALL    S88_SET_LENGTH_CHECKED
                CALL    EEP_WRITE_S88LENGTH
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH4 ; S88 contact ID offset
                MOVFF   CANFRAME+11,S88C_H
                MOVFF   CANFRAME+12,S88C_L
                CALL    EEP_WRITE_S88C
                BRA     SETUP_VALUE_RESPONSE


CHANNEL_SET_CH5 ; scan interval
                MOVFF   CANFRAME+12,SCAN_INTERVAL
                CALL    EEP_WRITE_SCANINTERVAL
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH6 ; UID low bytes
                MOVFF   CANFRAME+11,UID_H
                MOVFF   CANFRAME+12,UID_L
                CALL    EEP_WRITE_UID
                CALL    READCANHASH ;new hash when UID changes
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH7 ; OPTIONS
                MOVFF   CANFRAME+12,OPTION_FLAGS
                CALL    EEP_WRITE_OPTIONFLAGS
                ; in case at least one S88 send all flag is set,
                ; set SYSTEM_FLAGS, SYSTEMFLAG_S88SENDALL_ONFIRST
                ; next S88Event SingleIO will send all S88 contacts
                MOVF    OPTION_FLAGS,W
                ANDLW   0x03
                BTFSS   STATUS,Z
                BSF     SYSTEM_FLAGS, SYSTEMFLAG_S88SENDALL_ONFIRST
                BRA     SETUP_VALUE_RESPONSE

CHANNEL_SET_CH8 ; S88 forced event delay in ms
                MOVFF   CANFRAME+12,S88_FEVENTDELAY_SOLL
                CALL    EEP_WRITE_S88_FEVENTDELAY
                BRA     SETUP_VALUE_RESPONSE

SETUP_VALUE_RESPONSE
                RCALL   CREATE_RESPONSE_HEADER_SYSCMD
                MOVLW   7  ;DLC
                MOVWF   POSTINC0
                ; use bytes from received command, copy
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

;------------------------------------------------------------------------------
; HANDLING System CMD 0x0C ->get/set Device ID value
;------------------------------------------------------------------------------
DEVICEID_GET
                RCALL	CREATE_RESPONSE_HEADER_SYSCMD
                MOVLW   7   ;DLC
                MOVWF   POSTINC0
                RCALL   UID_TO_POSTINC0
                MOVLW   0x0C
                MOVWF   POSTINC0
                MOVFF   ID_H,POSTINC0
                MOVFF   ID_L,POSTINC0
                BRA     EVENT_RING_NORMALIZE

DEVICEID_SET
                MOVFF   CANFRAME+10,ID_H
                MOVFF   CANFRAME+11,ID_L
                ; write non volatile
                CALL    EEP_WRITE_DEVICEID ;write to EEPROM

                BRA     DEVICEID_GET ; answer to id set is same as id get

;------------------------------------------------------------------------------
; Switch Accessois (0x1B)
;------------------------------------------------------------------------------

CAN_SWITCH_ACC
                ; if DLC != 8 -> RETURN
                MOVF    CANFRAME+4,W
                XORLW   8
                BZ      CAN_SWITCH_ACC_NEXT1
                RETURN

CAN_SWITCH_ACC_NEXT1
                RCALL   CHECK_UID
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
                MOVLW   0x17
                RCALL   CREATE_RESPONSE_HEADER
                MOVLW   7    ;DLC
                MOVWF   POSTINC0

                MOVFF   CANFRAME+5,POSTINC0
                MOVFF   CANFRAME+6,POSTINC0
                MOVFF   CANFRAME+7,POSTINC0
                MOVFF   CANFRAME+8,POSTINC0

                MOVFF   CANFRAME+11,PLUSW0	; ?
                MOVFF   CANFRAME+12,PLUSW0      ; ?

                ; 01 -> success
                MOVLW   0x01
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

CAN_SWITCH_ACC_FAULT
                RETURN

;------------------------------------------------------------------------------
; answer CAN Ping (0x30)
;------------------------------------------------------------------------------

CAN_PING
                ; if DLC != 0 -> return
                MOVF    CANFRAME+4,F
                BZ      CAN_PING_SEND
                RETURN

CAN_PING2
                ; if DLC != 8 -> return
                MOVF    CANFRAME+4,W
                XORLW   8
                BTFSS   STATUS,Z
                RETURN
                ; check for CS2 PC ping with response bit
                MOVF    CANFRAME+11,W
                XORLW   0xEE
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+12,W
                XORLW   0xEE
                BTFSS   STATUS,Z
                RETURN
CAN_PING_SEND
                MOVLW   0x31
                RCALL   CREATE_RESPONSE_HEADER
                MOVLW   0x8  ;DLC
                MOVWF   POSTINC0
                RCALL   UID_TO_POSTINC0
                VERSION_TO_POSTINC0
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; CAN Bootloader Service (0x36)
;------------------------------------------------------------------------------

CAN_BOOTLOADER
                ; cold state ?
                BTFSC   SYSTEM_FLAGS, SYSTEM_FLAG_WARM_STATE
                RETURN ; return if != 0 (warm state)

                ; if DLC == 0
                MOVF    CANFRAME+4,F
                BZ      CAN_BOOTLOADER_PING
                ; if DLC == 5
                MOVF    CANFRAME+4,W
                XORLW   0x05
                BZ      CAN_BOOTLOADER_SYSTEM
                RETURN

CAN_BOOTLOADER_SYSTEM
                RCALL   CHECK_UID
                BNZ     CAN_BOOTLOADER_EXIT
                ; check for CAN-Bootloader Go 0x11
                MOVF    CANFRAME+9,W
                XORLW   0x11
                BZ      CAN_BOOTLOADER_GO
CAN_BOOTLOADER_EXIT
                RETURN

CAN_BOOTLOADER_PING
                MOVLW   0x37 ; mark with 0x37 as CAN Bootloader response
                RCALL   CREATE_RESPONSE_HEADER
                MOVLW   0x08  ;DLC
                MOVWF   POSTINC0
                RCALL   UID_TO_POSTINC0
                VERSION_TO_POSTINC0
                BRA     EVENT_RING_NORMALIZE

CAN_BOOTLOADER_GO
                ; we are now in warm state
                BSF     SYSTEM_FLAGS, SYSTEM_FLAG_WARM_STATE
                ; respond to CAN Bootloader Go with System Reset CAN Frame
                ; w/o doing something
                RCALL   CREATE_RESPONSE_HEADER_SYSCMD
                MOVLW   0x06 ;DLC
                MOVWF   POSTINC0
                RCALL   UID_TO_POSTINC0
                MOVLW   0x80
                MOVWF   POSTINC0
                MOVLW   0
                MOVWF   POSTINC0
                BRA     EVENT_RING_NORMALIZE

;------------------------------------------------------------------------------
; Setup Command (0x3A)
;------------------------------------------------------------------------------
; ConfigChannel1 = DeviceID
; ConfigChannel2 = PIO ContactID Offset
; ConfigChannel3 = S88 Length (number 16bit modules)
; ConfigChannel4 = S88 ContactID Offset
; ConfigChannel5 = Scan Interval
SEND_CONFIGURATION
                ; if DLC != 5 -> return
                MOVF    CANFRAME+4,W
                XORLW   0x05
                BTFSS   STATUS,Z
                RETURN
                RCALL   CHECK_UID
                BTFSS   STATUS,Z
                RETURN
                ; switch(channel index)
                MOVF    CANFRAME+9,W
                MOVWF   TEMP ; used in SEND_INDEX
                BZ      SEND_CONFIGURATION_IND0
                XORLW   1
                BZ      SEND_CONFIGURATION_IND1
                XORLW   2 ^ 1
                BZ      SEND_CONFIGURATION_IND2
                XORLW   3 ^ 2
                BZ      SEND_CONFIGURATION_IND3
                XORLW   4 ^ 3
                BZ      SEND_CONFIGURATION_IND4
                XORLW   5 ^ 4
                BZ      SEND_CONFIGURATION_IND5
                RETURN  ; index unknown

SEND_CONFIGURATION_IND0 ; device description
                CLRF    TEMP16   ;for safety
                CLRF    TEMP16+1   ;for safety
                LOAD_INDEX_P INDEX0
                GOTO    SEND_INDEX

SEND_CONFIGURATION_IND1 ; DeviceID
                MOVFF   ID_H, TEMP16
                MOVFF   ID_L, TEMP16+1
                LOAD_INDEX_P INDEX1
                GOTO    SEND_INDEX

SEND_CONFIGURATION_IND2 ; PIO contact ID offset
                MOVFF   PIOC_H, TEMP16
                MOVFF   PIOC_L, TEMP16+1
                LOAD_INDEX_P INDEX2
                GOTO    SEND_INDEX

SEND_CONFIGURATION_IND3 ; S88 LENGTH
                CLRF    TEMP16
                MOVFF   S88LENGTH, TEMP16+1
                LOAD_INDEX_P INDEX3
                GOTO    SEND_INDEX

SEND_CONFIGURATION_IND4 ; S88 contact ID offset
                MOVFF   S88C_H, TEMP16
                MOVFF   S88C_L, TEMP16+1
                LOAD_INDEX_P INDEX4
                GOTO    SEND_INDEX

SEND_CONFIGURATION_IND5 ; SCAN INTERVAL
#IFDEF	INDEX5
                CLRF    TEMP16
                MOVFF   SCAN_INTERVAL, TEMP16+1
                LOAD_INDEX_P INDEX5
                GOTO    SEND_INDEX
#ELSE
                RETURN
#ENDIF

;------------------------------------------------------------------------------
; S88 Event query (0x22) = Feedback event (query)
;------------------------------------------------------------------------------

S88_EVENT
                ; test S88 bus length and return if 0
                MOVF    S88LENGTH,W     ;affects Z
                BTFSC   STATUS,Z
                RETURN  ;S88LENGTH = 0

                ; Device ID check
                MOVF    CANFRAME+5,W
                XORWF   ID_H,W	;DEVID_H,W
                BTFSS   STATUS,Z
                RETURN
                MOVF    CANFRAME+6,W
                XORWF   ID_L,W	;DEVID_L,W
                BTFSS   STATUS,Z
                RETURN
                ; Check DLC
                MOVF    CANFRAME+4,W
                XORLW   4
                BTFSC   STATUS,Z
                BRA     S88_SINGLE_IO   ;DLC = 4 S88 Event: Read Status Single contact
                ; if DLC != 7 -> return
                XORLW   7 ^ 4
                BTFSS   STATUS,Z
                RETURN  ;DLC!=4 and DLC!=7
                ; DLC = 7
                ; S88 BlockEvent
                ; check start and end contactID in range
                ; check start contactID in range
                MOVFF   CANFRAME+7, TEMP16
                MOVFF   CANFRAME+8, TEMP16+1
S88_EVENT_RECALC
                CALL    CALC_S88CONTACTID_INRANGE
                BTFSC   STATUS,C
                BRA     S88_EVENT_CALC_NR_BITS ;start contactID in range
                ; start out of range, check end contactID in range
                MOVFF   CANFRAME+9, TEMP16
                MOVFF   CANFRAME+10, TEMP16+1
                CALL    CALC_S88CONTACTID_INRANGE
                BTFSS   STATUS,C
                RETURN	;START AND END OOUT OF RANGE
                ; end is in range, set S88C +1 as start
                MOVFF   S88C_H, TEMP16
                MOVFF   S88C_L, TEMP16+1
                INCF    TEMP16+1,F
                BTFSC   STATUS,Z
                INCF    TEMP16,F
                BRA     S88_EVENT_RECALC
S88_EVENT_CALC_NR_BITS
                ; start is in range
                MOVFF   PRODH, TEMP    ;nr of bits to BufferEnd H
                MOVFF   PRODL, TEMP +1 ;nr of bits to BufferEnd L
                CALL    CALC_S88CONTACTID_BUFFER_PTR ;get buffer pointer
                ; calculate nr of bits (temp:temp+1)
                ; end to temp
                MOVFF   CANFRAME+9,TEMP16 ;endindexH
                MOVFF   CANFRAME+10, TEMP16+1 ;endindexL
                CALL    CALC_S88CONTACTID_INRANGE
                BTFSS   STATUS,C
                BRA     S88_EVENT_INC_NR_OF_BITS ;EndIndex out of range, send until buffer end
                ; end index in range, subsrtact nr of bits to BufferEnd from TEMP:TEMP+1
                MOVF    PRODL,W
                SUBWF   TEMP+1,F
                MOVF    PRODH,W
                BTFSS   STATUS,C
                INCF    PRODH,W
                SUBWF   TEMP,F
S88_EVENT_INC_NR_OF_BITS
                ; inrement mr of bits by 1 because start amd end index are included
                INCF    TEMP+1,F
                BTFSC   STATUS,Z
                INCF    TEMP,F
                ; bit count in temp:temp+1 (H:L)
                ; TODO   CHECK LENGTH
                BRA     S88_EVENT_SETFLAGS

S88_EVENT_CHK_END;start contactID is out of range, check end in range
                MOVFF   CANFRAME+9, TEMP16
                MOVFF   CANFRAME+10, TEMP16+1
                CALL    CALC_S88CONTACTID_INRANGE
                BTFSS   STATUS,C
                RETURN  ; start and end out of range
                ; end is in range, calculate valid start
                MOVFF   S88C_H, TEMP16
                MOVFF   S88C_L, TEMP16+1
                INCF    TEMP16+1,F
                BTFSS   STATUS,C
                INCF    TEMP16,F

S88_EVENT_SETFLAGS
                ; adjust pointer to force flags (one byte before state flags)
                DECF    FSR0L,F ;TODO
                ; set change send flags
S88_EVENT_LOOP
                MOVF    TEMP+1,W
                BNZ     S88_EVENT_LOOP_CHECKBIT0
                MOVF    TEMP
                BTFSC   STATUS,Z,W
                RETURN
S88_EVENT_LOOP_CHECKBIT0
                MOVF    MASK,W
                XORLW   0x01	;mask masks first bit?
                BNZ     S88_EVENT_LOOP_BITSET
                ; yes. check if remaining bits >=8
                MOVLW   0xF8
                ANDWF   TEMP+1,W
                BNZ     S88_EVENT_LOOP_BYTESET            ;lowbyte >8
                MOVF    TEMP,W
                BZ      S88_EVENT_LOOP_BITSET             ;highbyte = 0
                ; remaining bits >=8
S88_EVENT_LOOP_BYTESET
                SETF    INDF0	;set all bits
                MOVLW   5
                ADDWF   FSR0L,F
                ; decrement Bitcount by 8
                MOVLW   8
                SUBWF   TEMP+1,F
                MOVLW   0
                BTFSS   STATUS,C
                MOVLW   1
                SUBWF   TEMP,F
                BRA     S88_EVENT_LOOP

S88_EVENT_LOOP_BITSET
                MOVF    MASK,W
                IORWF   INDF0,F
                BCF     STATUS,C
                RLCF    MASK,F
                BNZ     S88_EVENT_LOOP_BITSET1
                ; mask is 0, next byte
                RLCF    MASK,F  ;set to 1 again
                MOVLW   5
                ADDWF   FSR0L,F
S88_EVENT_LOOP_BITSET1
                ; decrement Bitcount
                DECF    TEMP+1;
                BTFSS   STATUS,C
                DECF    TEMP,F
                BRA     S88_EVENT_LOOP

S88_SINGLE_IO   ; S88 Event with DLC4 (single io query)
                ; CS2 S88Event with contactID 0 or contactID=Offset forces all Events
                MOVF    CANFRAME+7,W
                ADDWF   CANFRAME+8,W
                BTFSC   STATUS,Z
                BRA     S88_FORCE_ALL                ; contactID=0, send all
                MOVF    CANFRAME+7,W
                SUBWF   S88C_H,W
                BTFSS   STATUS,Z
                BRA     S88_SINGLE_IO_1
                MOVF    CANFRAME+8,W
                SUBWF   S88C_L,W
                BTFSC   STATUS,Z
                BRA     S88_FORCE_ALL                ; contactID=S88C offset, send all
S88_SINGLE_IO_1
                ; contact ID not 0 and not S88C_H:S88C:L
                ; check in my range
                MOVFF   CANFRAME+7,TEMP16            ; contactID high
                MOVFF   CANFRAME+8,TEMP16+1          ; contactID low
                ; (TEMP16:TEMP16+1) = contactID
                CALL    CALC_S88CONTACTID_INRANGE
                BTFSS   STATUS,C
                RETURN                               ; contactID out of range
                ; contact ID is in my range
                ; check option send all on first
                BTFSC   SYSTEM_FLAGS, SYSTEMFLAG_S88SENDALL_ONFIRST
                BRA     S88_FORCE_ALL                ; send all on first s88event is requested
                ; calculate contact buffer pointer
                CALL    CALC_S88CONTACTID_BUFFER_PTR
                MOVF    INDF0,W                      ; get current state byte
                MOVWF   ACTUAL                       ; new value
                GOTO    PUTEVENTEQUAL                ; send event new=old

S88_FORCE_ALL
                BCF     SYSTEM_FLAGS, SYSTEMFLAG_S88SENDALL_ONFIRST
                ; set all force flags
                LFSR    FSR0,S88_DATA_P+2            ; ForceFlags
                BCF     STATUS,C
                RLCF    S88LENGTH,W                  ; S88LENGTH * 2
                MOVWF   TEMP
S88_FORCE_ALL_LOOP
                SETF    INDF0
                MOVLW   5
                ADDWF   FSR0L,F
                DECFSZ  TEMP,F
                BRA     S88_FORCE_ALL_LOOP
                RETURN

; vim: set shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
