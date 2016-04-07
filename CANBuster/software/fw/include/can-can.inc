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
#IFDEF LEDHIGH
                BSF     GPIO,6              ; LED1 CAN Packet dropped
#ELSE
                BCF     GPIO,6
#ENDIF
                ENDM

CANRECV         MACRO
#IFDEF LEDHIGH
                BCF     GPIO,6              ; LED1 CAN Packet received
#ELSE
                BSF     GPIO,6
#ENDIF
                ENDM

UARTOVFL        MACRO
#IFDEF LEDHIGH
                BSF     GPIO,7              ; LED2 UART Packet dropped
#ELSE
                BCF     GPIO,7
#ENDIF
                ENDM

UARTRECV        MACRO
#IFDEF LEDHIGH
                BCF     GPIO,7              ; LED2 UART Packet received
#ELSE
                BCF     GPIO,7
#ENDIF
                ENDM

RTSCLR          MACRO
                BCF     GPIO,4              ; RTS/CTS CLEAR TO SEND
                ENDM

RTSSET          MACRO
                BSF     GPIO,4              ; RTS/CTS NOT CLEAR TO SEND
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
#IF UART == PORTB || UART == PORTD
                CLRF    PIE3
                CLRF    PIR3
#ELSE
                CLRF    PIE1
                CLRF    PIR1
#ENDIF

                ; switch off A/Ds, D/As and comaprators
                CLRF    ADCON0          ;D
                CLRF    ADCON1          ;D
                CLRF    ADCON2          ;D
                CLRF    ANCON0          ;N
                CLRF    ANCON1          ;N
                CLRF    CM1CON          ;C
                CLRF    CM2CON          ;C

                CLRF    LATD
                CLRF    LATC
                BCF     TRISC, 2
                CLRF    GPIO                ; Init. LED
                RTSCLR                      ; Init. RTS
                CLRF    DDR                 ; All O/P

                RCALL   INITUART            ; Init. UART
                RCALL   INITRING            ; Init. Rx/Tx Ring Buffer
                RCALL   INITCAN             ; Init. CAN Bus
                RCALL   TIMERINIT           ; Init. TIMER
                RCALL   I2CINIT             ; Init. I2C
                RCALL   PIOINIT             ; Init. PIO
                RCALL   INITEVENTRING       ; Init. Event Ring Buffer

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
#INCLUDE        "ascii.inc"                 ; ASCII I/O
#INCLUDE        "can.inc"                   ; CAN
#INCLUDE        "timer.inc"                 ; Timer
#INCLUDE        "i2c.inc"                   ; I2C
#INCLUDE        "pio.inc"                   ; PIO
#INCLUDE        "event-ring.inc"            ; Event ring

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
