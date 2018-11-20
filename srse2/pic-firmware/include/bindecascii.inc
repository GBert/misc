;Takes hex number in NumH:NumL  Returns decimal in ;TenK:Thou:Hund:Tens:Ones
;written by John Payson

;input
;=A3*163 + A2*162 + A1*161 + A0*160
;=A3*4096 + A2*256 + A1*16 + A0
		CBLOCK
                NumH: 1         ;A3*16+A2
                NumL: 1         ;A1*16+A0
;share variables
;=B4*104 + B3*103 + B2*102 + B1*101 + B0*100
;=B4*10000 + B3*1000 + B2*100 + B1*10 + B0
                TenK: 1         ;B4
                Thou: 1	        ;B3
                Hund: 1         ;B2
                Tens: 1         ;B1
                Ones: 1         ;B0
                ENDC

                SWAPF   NumH,w  ;w  = A2*16+A3
                ANDLW   0x0F    ;w  = A3	*** PERSONALLY, I'D REPLACE THESE 2
                ADDLW   0xF0	;w  = A3-16	*** LINES WITH "IORLW b'11110000B' " -AW
                MOVWF   Thou	;B3 = A3-16
                ADDWF   Thou,f	;B3 = 2*(A3-16) = 2A3 - 32
                ADDLW   226	;w  = A3-16 - 30 = A3-46
                MOVWF   Hund	;B2 = A3-46
                ADDLW   50	;w  = A3-46 + 50 = A3+4
                MOVWF   Ones	;B0 = A3+4

                MOVF    NumH,w	;w  = A3*16+A2
                ANDLW   0x0F	;w  = A2
                ADDWF   Hund,f	;B2 = A3-46 + A2 = A3+A2-46
                ADDWF   Hund,f	;B2 = A3+A2-46  + A2 = A3+2A2-46
                ADDWF   Ones,f	;B0 = A3+4 + A2 = A3+A2+4
                ADDLW   233	;w  = A2 - 23
                MOVWF   Tens	;B1 = A2-23
                ADDWF   Tens,f	;B1 = 2*(A2-23)
                ADDWF   Tens,f	;B1 = 3*(A2-23) = 3A2-69 (Doh! thanks NG)

                SWAPF   NumL,w	;w  = A0*16+A1
                ANDLW   0x0F	;w  = A1
                ADDWF   Tens,f	;B1 = 3A2-69 + A1 = 3A2+A1-69 range -69...-9
                ADDWF   Ones,f	;B0 = A3+A2+4 + A1 = A3+A2+A1+4 and Carry = 0 (thanks NG)

                RLCF    Tens,f	;B1 = 2*(3A2+A1-69) + C = 6A2+2A1-138 and Carry is now 1 as tens register had to be negitive
                RLCF    Ones,f	;B0 = 2*(A3+A2+A1+4) + C = 2A3+2A2+2A1+9 (+9 not +8 due to the carry from prev line, Thanks NG)
                COMF    Ones,f	;B0 = ~(2A3+2A2+2A1+9) = -2A3-2A2-2A1-10 (ones complement plus 1 is twos complement. Thanks SD)
;;Nikolai Golovchenko [golovchenko at MAIL.RU] says: comf can be regarded like:
;;      comf Ones, f
;;      incf Ones, f
;;      decf Ones, f
;;First two instructions make up negation. So,
;;Ones  = -1 * Ones - 1 
;;      = - 2 * (A3 + A2 + A1) - 9 - 1 
;;      = - 2 * (A3 + A2 + A1) - 10
                 RLCF    Ones,f	;B0 = 2*(-2A3-2A2-2A1-10) = -4A3-4A2-4A1-20

                 MOVF    NumL,w	;w  = A1*16+A0
                 ANDLW   0x0F	;w  = A0
                 ADDWF   Ones,f	;B0 = -4A3-4A2-4A1-20 + A0 = A0-4(A3+A2+A1)-20 range -215...-5 Carry=0
                 RLCF    Thou,f	;B3 = 2*(2A3 - 32) = 4A3 - 64

                 MOVLW   0x07	;w  = 7
                 MOVWF   TenK	;B4 = 7

;B0 = A0-4(A3+A2+A1)-20	;-5...-200
;B1 = 6A2+2A1-138	;-18...-138
;B2 = A3+2A2-46		;-1...-46
;B3 = 4A3-64		;-4...-64
;B4 = 7			;7
; At this point, the original number is
; equal to TenK*10000+Thou*1000+Hund*100+Tens*10+Ones 
; if those entities are regarded as two's compliment 
; binary.  To be precise, all of them are negative 
; except TenK.  Now the number needs to be normal- 
; ized, but this can all be done with simple byte 
; arithmetic.

                MOVLW   10       ;w  = 10
Lb1:		; do
                ADDWF   Ones,f	; B0 += 10
                DECF    Tens,f	; B1 -= 1
                BTFSS   STATUS,C
                ;skip no carry
                GOTO    Lb1	; while B0 < 0
                ; jmp carry
Lb2:		; do
                ADDWF   Tens,f	; B1 += 10
                DECF    Hund,f	; B2 -= 1
                BTFSS   STATUS,C
                GOTO    Lb2	; while B1 < 0
Lb3:			;do
                ADDWF   Hund,f	; B2 += 10
                DECF    Thou,f	; B3 -= 1
                BTFSS   STATUS,C
                GOTO    Lb3	; while B2 < 0
Lb4:			;do
                ADDWF   Thou,f	; B3 += 10
                DECF    TenK,f	; B4 -= 1
                BTFSS   STATUS,C
                GOTO    Lb4	; while B3 < 0

                RETLW   0

