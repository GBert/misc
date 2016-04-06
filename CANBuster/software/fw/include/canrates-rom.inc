;------------------------------------------------------------------------------
;
; CAN rates
;
; Copyright (c) 2016 Gerhard Bertelsmann
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

; RATE, BRGCON1, BRGCON3, BRGCON3
;CLOCK == 16000000
DB 0x08, 0x00, 0x84, 0x00 ; 1000
DB 0x07, 0x00, 0x86, 0x00 ; 1000
DB 0x06, 0x00, 0xBC, 0x01 ; 500
DB 0x05, 0x01, 0xBC, 0x01 ; 250
DB 0x04, 0x03, 0xBC, 0x01 ; 125
DB 0x03, 0x04, 0xBC, 0x01 ; 100
DB 0x02, 0x09, 0xBC, 0x01 ; 50
DB 0x01, 0x18, 0xBC, 0x01 ; 20
DB 0x00, 0x31, 0xBC, 0x01 ; 10



#IF CLOCK == 40000000
    #IF CANRATE == 500
        #DEFINE BRG1 0x04
        #DEFINE BRG2 0x84
        #DEFINE BRG3 0x00
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x04
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x09
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x18
        #DEFINE BRG2 0x84
        #DEFINE BRG3 0x00
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x18
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
#ENDIF

#IF CLOCK == 48000000
    #IF CANRATE == 1000
        #DEFINE BRG1 0x00
        #DEFINE BRG2 0xBF
        #DEFINE BRG3 0x06
    #ENDIF
    #IF CANRATE == 500
        #DEFINE BRG1 0x02
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x05
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x0B
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x0E
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x1D
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 20
        #DEFINE BRG1 0x3B
        #DEFINE BRG2 0xBF
        #DEFINE BRG3 0x02
    #ENDIF
#ENDIF

#IF CLOCK == 60000000
    #IF CANRATE == 1000
        #DEFINE BRG1 0x00
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 500
        #DEFINE BRG1 0x01
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x03
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x07
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x09
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x13
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 20
        #DEFINE BRG1 0x31
        #DEFINE BRG2 0xBB
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 10
        #DEFINE BRG1 0x3B
        #DEFINE BRG2 0xBF
        #DEFINE BRG3 0x07
    #ENDIF
#ENDIF

#IF CLOCK == 64000000
    #IF CANRATE == 1000
        #DEFINE BRG1 0x01
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 500
        #DEFINE BRG1 0x03
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 250
        #DEFINE BRG1 0x07
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 125
        #DEFINE BRG1 0x0F
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 100
        #DEFINE BRG1 0x13
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
    #IF CANRATE == 50
        #DEFINE BRG1 0x27
        #DEFINE BRG2 0xBC
        #DEFINE BRG3 0x01
    #ENDIF
#ENDIF

;------------------------------------------------------------------------------
;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
