;------------------------------------------------------------------------------
;
; Can-Can Firmware
;
; Copyright (c) 2014 Darron M Broad
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

                LIST
                RADIX   DEC

;------------------------------------------------------------------------------
; Macros
;------------------------------------------------------------------------------

; Decrement REG, goto LOC if result not ZERO
DJNZ            MACRO   REG,LOC
                DECFSZ  REG,F
                BRA     LOC
                ENDM

CANOVFL         MACRO
                BSF     GPIO,0              ; LED1 CAN Packet dropped
                ENDM

CANRECV         MACRO
                BCF     GPIO,0              ; LED1 CAN Packet received
                ENDM

UARTOVFL        MACRO
                BSF     GPIO,1              ; LED2 UART Packet dropped
                ENDM

UARTRECV        MACRO
                BCF     GPIO,1              ; LED2 UART Packet received
                ENDM

RTSCLR          MACRO
                BCF     GPIO,2              ; RTS/CTS CLEAR TO SEND
                ENDM

RTSSET          MACRO
                BSF     GPIO,2              ; RTS/CTS NOT CLEAR TO SEND
                ENDM

;------------------------------------------------------------------------------
; Variables
;------------------------------------------------------------------------------

                CBLOCK  0x0000              ; ACCESS RAM 0x00..0x5F
                B_SAVE  : 1                 ; ISR BANK SELECT REGISTER
                S_SAVE  : 1                 ; ISR STATUS REGISTER
                W_SAVE  : 1                 ; ISR WORKING REGISTER
                ENDC

;------------------------------------------------------------------------------
; Reset/ISR
;------------------------------------------------------------------------------

                ORG     0x0000
                GOTO    INIT
                ORG     0x0008
                GOTO    ISRHIGH             ; When IPEN = 0 or 1
                ORG     0x0018
                GOTO    ISRLOW              ; When IPEN = 1

;------------------------------------------------------------------------------
; Init.
;------------------------------------------------------------------------------
INIT
                MOVLB   0x0F                ; Default to Bank 15

                CLRF    INTCON              ; Reset ISR
                CLRF    PIE1
                CLRF    PIR1

                ; switch off A/Ds, D/As and comaprators
                CLRF    ADCON0          ;D
                CLRF    ADCON1          ;D
                CLRF    ADCON2          ;D
                CLRF    ANCON0          ;N
                CLRF    ANCON1          ;N
                CLRF    CM1CON          ;C
                CLRF    CM2CON          ;C

                CLRF    GPIO                ; Init. LED
                RTSCLR                      ; Init. RTS
                CLRF    DDR                 ; All O/P

                RCALL   INITUART            ; Init. UART
                RCALL   INITRING            ; Init. Rx/Tx Ring
                RCALL   INITCAN             ; Init. CAN Bus
                RCALL   TIMERINIT           ; Init. TIMER
                RCALL   I2CINIT             ; Init. I2C
                RCALL   PIOINIT             ; Init. PIO

                BSF     RCON,IPEN           ; Enable priority interrupts
                BSF     INTCON,PEIE         ; Init. ISR
                BSF     INTCON,GIE
               
;------------------------------------------------------------------------------
; Main
;------------------------------------------------------------------------------
MAIN
                RCALL   RESETUART           ; UART RESET
                RCALL   RXCAN               ; RECEIVE CAN
                RCALL   TXCAN               ; SEND CAN
		BTFSC	INTCON, TMR0IF
		RCALL   SCHEDULE
                BRA     MAIN

;------------------------------------------------------------------------------
; Includes
;------------------------------------------------------------------------------

#INCLUDE        "uart.inc"                  ; UART
#INCLUDE        "ring.inc"                  ; Rx/Tx Ring
#INCLUDE        "can.inc"                   ; CAN
#INCLUDE        "ascii.inc"                 ; ASCII I/O
#INCLUDE        "timer.inc"                 ; Timer
#INCLUDE        "i2c.inc"                   ; I2C
#INCLUDE        "pio.inc"                   ; I2C

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
