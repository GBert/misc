;------------------------------------------------------------------------------
;
; CAN S88
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
; port defines
;------------------------------------------------------------------------------

#IF PCBV > 1
  #DEFINE      S88_DATA   1
#ELSE
  #DEFINE      S88_DATA   2
#ENDIF

#DEFINE        S88_CLOCK  6         
#DEFINE        S88_LOAD   7         
#DEFINE        S88_REST   5         

;------------------------------------------------------------------------------
; Init. S88
;------------------------------------------------------------------------------
S88INIT

#IF PCBV > 1
                BSF     TRISC,2         ; Input S88 Data
#ELSE
                BSF     TRISC,1         ; Input S88 Data
#ENDIF
                BCF     TRISC,6         ; Output S88 Clock
                BCF     TRISC,7         ; Output S88 Load
                BCF     TRISB,5         ; Output S88 Reset

                ; 
		BCF     LATC,6          ; clear pins
                BCF     LATC,7          ; 
                BCF     LATB,5          ; 

                RETURN

;
; vim: shiftwidth=4 tabstop=4 softtabstop=4 expandtab
;
