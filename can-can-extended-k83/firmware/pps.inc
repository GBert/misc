;------------------------------------------------------------------------------
;
; Can-Can PPS K83
;
; Copyright (c) 2015 Gerhard Bertelsmann
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
; Init. I2C
;------------------------------------------------------------------------------
INITPPS
; PPSUnlock
                BANKSEL PPSLOCK
                MOVLW   0x55
                MOVWF   PPSLOCK
                MOVLW   0xAA
                MOVWF   PPSLOCK
                BCF     PPSLOCK,PPSLOCKED
; PPSInput
                ; UART2
                ;BANKSEL U2RXPPS
                MOVLW   0x0F                ;RB7
                MOVWF   U2RXPPS             ;RB7 = RX2

                ; CAN
                ;BANKSEL CANRXPPS
                MOVLW   0x0B                ;RB3
                MOVWF   CANRXPPS            ;RB3 = CANRX

                ;I2C1
                ;BANKSEL I2C1SCLPPS
                MOVLW   0x13                ;RC3
                MOVWF   I2C1SCLPPS          ;RC3 = I2C1 SCL

                ;BANKSEL I2C1SDAPPS
                MOVLW   0x14                ;RC4
                MOVWF   I2C1SDAPPS          ;RC4 = I2C1 SDA

                ;RS485
                ;BANKSEL U1RXPPS
                MOVLW   0x16                ;RC6
                MOVWF   U1RXPPS             ;RB7 = RX2
; PPSOutput
                ;UART
                ;BANKSEL RB6PPS
                MOVLW   0x16                ;UART2 TX
                MOVWF   RB6PPS              ;RB6 = UART2 TX

                ;CAN
                ;BANKSEL RB2PPS
                MOVLW   0x33                ;CAN0 TX
                MOVWF   RB2PPS              ;RB2 CANTX

                ;I2C1 needs to been set as output also (DS40001943B page 264)
                MOVLW   0x23                ;I2C1 SCL
                MOVWF   RC3PPS              ;RC3 SCL1

                MOVLW   0x22                ;I2C1 SDA
                MOVWF   RC4PPS              ;RC4 SDA1

                ;RS485
                ;BANKSEL RC7PPS
                MOVLW   0x13                ;UART1 TX
                MOVWF   RC7PPS              ;RC7 = UART2 TX
; PPSLock
                ;BANKSEL PPSLOCK
                MOVLW   0x55
                MOVWF   PPSLOCK
                MOVLW   0xAA
                MOVWF   PPSLOCK
                BSF     PPSLOCK,PPSLOCKED

                RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
