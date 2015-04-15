
        LIST P=18F4550
        NOLIST

; MPLAB-Cxx  MCP2515 Register Definition Module using PIC18F4550
; Works in conjunction with Structs in REGS2515.H


MCP2515_REGS:       UDATA

CANSTAT_byte               
CANSTATbits           RES 1
CANCTRL_byte              
CANCTRLbits           RES 1
CNF1_byte              
CNF1bits              RES 1
CNF2_byte              
CNF2bits              RES 1
CNF3_byte              
CNF3bits              RES 1
CANINTE_byte              
CANINTEbits           RES 1
CANINTF_byte              
CANINTFbits           RES 1
EFLG_byte              
EFLGbits              RES 1
BFPCTRL_byte              
BFPCTRLbits           RES 1
TXRTSCTRL_byte          
TXRTSCTRLbits         RES 1
TXB0CTRL_byte          
TXB0CTRLbits          RES 1
TXB1CTRL_byte          
TXB1CTRLbits          RES 1
TXB2CTRL_byte          
TXB2CTRLbits          RES 1
RXB0CTRL_byte          
RXB0CTRLbits          RES 1
RXB1CTRL_byte          
RXB1CTRLbits          RES 1


        GLOBAL CANSTAT_byte
        GLOBAL CANSTATbits
        GLOBAL CANCTRL_byte  
        GLOBAL CANCTRLbits   
        GLOBAL CNF1_byte     
        GLOBAL CNF1bits      
        GLOBAL CNF2_byte     
        GLOBAL CNF2bits      
        GLOBAL CNF3_byte     
        GLOBAL CNF3bits      
        GLOBAL CANINTE_byte  
        GLOBAL CANINTEbits   
        GLOBAL CANINTF_byte  
        GLOBAL CANINTFbits   
        GLOBAL EFLG_byte     
        GLOBAL EFLGbits      
        GLOBAL BFPCTRL_byte  
        GLOBAL BFPCTRLbits   
        GLOBAL TXRTSCTRL_byte
        GLOBAL TXRTSCTRLbits 
        GLOBAL TXB0CTRL_byte 
        GLOBAL TXB0CTRLbits  
        GLOBAL TXB1CTRL_byte 
        GLOBAL TXB1CTRLbits  
        GLOBAL TXB2CTRL_byte 
        GLOBAL TXB2CTRLbits  
        GLOBAL RXB0CTRL_byte 
        GLOBAL RXB0CTRLbits  
        GLOBAL RXB1CTRL_byte 
        GLOBAL RXB1CTRLbits  
  
  END