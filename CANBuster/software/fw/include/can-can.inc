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
                TEMP    : 2
                TEMP16  : 2
                B_SAVE  : 1                 ; ISR BANK SELECT REGISTER
                S_SAVE  : 1                 ; ISR STATUS REGISTER
                W_SAVE  : 1                 ; ISR WORKING REGISTER
                STATE   : 1
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

                ; after boot we are in cold state
                MOVLW	0
                MOVWF   STATE	
                ; switch off A/Ds, D/As and comaprators
                CLRF    ADCON0          ;D
                CLRF    ADCON1          ;D
                CLRF    ADCON2          ;D
                CLRF    ANCON0          ;N
                CLRF    ANCON1          ;N
                CLRF    CM1CON          ;C
                CLRF    CM2CON          ;C

#IFNDEF WR841_SLCAN
                CLRF    LATD
#ENDIF
                CLRF    LATC
                BCF     TRISC, 2
                CLRF    GPIO                ; Init. LED
                RTSCLR                      ; Init. RTS
                CLRF    DDR                 ; All O/P

                CALL    INITUART            ; Init. UART
                CALL    INITRING            ; Init. Rx/Tx Ring Buffer
                CALL    INITCAN             ; Init. CAN Bus
                CALL    TIMER0INIT          ; Init. TIMER0
                CALL    TIMER3INIT          ; Init. TIMER3
                CALL    I2CINIT             ; Init. I2C
#IFNDEF WR841_SLCAN
                CALL    PIOINIT             ; Init. PIO
#ENDIF
                CALL    INITEVENTRING       ; Init. Event Ring Buffer
#IFDEF PWM_FW
                CALL    PWMINIT
                MOVLW   0
                MOVFF   WREG,S88LENGTH
#ENDIF

                CALL    S88INIT
                CALL    READ_UID

                BSF     RCON,IPEN           ; Enable priority interrupts
                BSF     INTCON,PEIE         ; Init. ISR
                BSF     INTCON,GIE
               
;------------------------------------------------------------------------------
; Main
;------------------------------------------------------------------------------
MAIN
                CALL    RESETUART           ; UART RESET
                CALL    RXCAN               ; RECEIVE CAN
                CALL    CANINSPECT
                CALL    TXCAN               ; SEND CAN
                CALL    CANINSPECT
#IFDEF PWM_FW
                CALL    PWM_TEST
#ENDIF

                BTFSC   INTCON, TMR0IF
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
#INCLUDE        "eeprom.inc"                ; EEPROM
#INCLUDE        "id.inc"                    ; UID
#INCLUDE        "pio.inc"                   ; PIO
#IFDEF PWM_FW
  #INCLUDE      "pwm.inc"                   ; PWM
#ENDIF
#INCLUDE      "s88.inc"                     ; S88
#INCLUDE        "event-ring.inc"            ; Event ring
#INCLUDE        "can-inspect.inc"           ; CAN Inspector

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
