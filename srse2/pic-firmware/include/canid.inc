;------------------------------------------------------------------------------
;
; Can-Can CAN ID
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
; MCHP CAN ID
;------------------------------------------------------------------------------
;
; RXB0SIDH
; --------
; SID10 SID9  SID8  SID7  SID6  SID5  SID4  SID3
;
; EID28 EID27 EID26 EID25 EID24 EID23 EID22 EID21
;
; RXB0SIDL
; --------
; SID2  SID1  SID0  X    EXIDEN X     X     X
;
; EID20 EID19 EID18 X    EXIDEN X     EID17 EID16
;
; RXB0EIDH
; --------
; EID15 EID14 EID13 EID12 EID11 EID10 EID9  EID8
;
; RXB0EIDL
; --------
; EID7  EID6  EID5  EID4  EID3  EID2  EID1  EID0
;
; STANDARD ID 11-bit
; ------------------
; [XX XX XX XX XX 10 09 08]  [07 06 05 04 03 02 01 00]
;
; EXTENDED ID 29-bit
; ------------------
; [XX XX XX 28 27 26 25 24]  [23 22 21 20 19 18 17 16]
; [15 14 13 12 11 10 09 08]  [07 06 05 04 03 02 01 00]

;------------------------------------------------------------------------------
; MCHP STANDARD CAN ID macros
;------------------------------------------------------------------------------

; Take lower 3-bits in W and store in SID10..SID8, SID7..SID3 are RESET
TXSID_10_8      MACRO   
                ANDLW   b'00000111'
                SWAPF   WREG,F
                RLNCF   WREG,F
                MOVWF   RXB0SIDH
                ENDM

; Take upper 5-bits in W and store in SID7..SID3, SID10..SID8 are UNAFFECTED
TXSID_7_3       MACRO   
                ANDLW   b'11111000'
                SWAPF   WREG,F
                RLNCF   WREG,F
                IORWF   RXB0SIDH,F
                ENDM

; Take lower 3-bits in W and store in SID2..SID0, EXIDEN is RESET
TXSID_2_0       MACRO
                ANDLW   b'00000111'
                SWAPF   WREG,F
                RLNCF   WREG,F
                MOVWF   RXB0SIDL
                ENDM

;------------------------------------------------------------------------------

; Take SID10..SID8 and store in lower 3-bits of W
RXSID_10_8      MACRO
                SWAPF   RXB0SIDH,W
                RRNCF   WREG,F
                ANDLW   b'00000111'
                ENDM

; Take SID7..SID3 and store in upper 5-bits of CANID
RXSID_7_0       MACRO
                SWAPF   RXB0SIDH,W
                RRNCF   WREG,F
                ANDLW   b'11111000'
                MOVWF   CANID

; Take SID2..SID0 and store in upper 3-bits of W, take lower 5-bits from CANID
                SWAPF   RXB0SIDL,W
                RRNCF   WREG,F
                ANDLW   b'00000111'
                IORWF   CANID,W
                ENDM

;------------------------------------------------------------------------------
; MCHP EXTENDED CAN ID macros
;------------------------------------------------------------------------------

; Take lower 5-bits in W and store in EID28..EID24, EID23..EID21 are RESET
TXEXT_28_24     MACRO   
                ANDLW   b'00011111'
                SWAPF   WREG,F
                RRNCF   WREG,F
                MOVWF   RXB0SIDH
                ENDM

; Take upper 3-bits in W and store in EID23..EID21, EID28..EID24 are UNAFFECTED
TXEXT_23_21     MACRO   
                ANDLW   b'11100000'
                SWAPF   WREG,F
                RRNCF   WREG,F
                IORWF   RXB0SIDH,F
                ENDM

; Take 3-bits in W and store in EID20..EID18, EXIDEN & EID17..EID16 are RESET
TXEXT_20_18     MACRO   
                ANDLW   b'00011100'
                SWAPF   WREG,F
                RRNCF   WREG,F
                MOVWF   RXB0SIDL
                ENDM

; Take lower 2-bits in W and store in EXIDEN & EID17..EID16, EID20..EID18 are UNAFFECTED
TXEXT_17_16     MACRO   
                ANDLW   b'00000011'
                IORWF   RXB0SIDL,F
                ENDM

;------------------------------------------------------------------------------

; Take EID28..EID24 and store in lower 5-bits of W
RXEXT_28_24     MACRO
                SWAPF   RXB0SIDH,W
                RLNCF   WREG,F
                ANDLW   b'00011111'
                ENDM

; Take EID23..EID21 and store in upper 3-bits of CANID
RXEXT_23_16     MACRO
                SWAPF   RXB0SIDH,W
                RLNCF   WREG,F
                ANDLW   b'11100000'
                MOVWF   CANID

; Take EID20..EID18 and store in W, take upper 3-bits from CANID
                SWAPF   RXB0SIDL,W
                RLNCF   WREG,F
                ANDLW   b'00011100'
                IORWF   CANID,F

; Take EID17..EID16 and store in W, take upper 6-bits from CANID
                MOVF    RXB0SIDL,W
                ANDLW   b'00000011'
                IORWF   CANID,W
                ENDM

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
