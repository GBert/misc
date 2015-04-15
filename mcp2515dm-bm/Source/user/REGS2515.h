//#pragma nolist
/********************************************************************
 *   CAN MCP2515 C definitions file                                  
 ********************************************************************
 *   Filename:       2515REGS.h                                      
 *   Date:           05/28/07                                        
 *   File Version:   1.00                                     
 ********************************************************************
 *   Functions:                                                      
 *               Header file                                         
 ********************************************************************
 *   Revision History:                                               
 *     
 ********************************************************************/

#ifndef REGS2515_H
#define REGS2515_H

/*******************************************************************
 *                   Register Mapping                          
 *******************************************************************/

/* Configuration Registers */
#define CANSTAT         0x0E
#define CANCTRL         0x0F
#define BFPCTRL         0x0C
#define TEC             0x1C
#define REC             0x1D
#define CNF3            0x28
#define CNF2            0x29
#define CNF1            0x2A
#define CANINTE         0x2B
#define CANINTF         0x2C
#define EFLG            0x2D
#define TXRTSCTRL       0x0D

/*  Recieve Filters */
#define RXF0SIDH        0x00
#define RXF0SIDL        0x01
#define RXF0EID8        0x02
#define RXF0EID0        0x03
#define RXF1SIDH        0x04
#define RXF1SIDL        0x05
#define RXF1EID8        0x06
#define RXF1EID0        0x07
#define RXF2SIDH        0x08
#define RXF2SIDL        0x09
#define RXF2EID8        0x0A
#define RXF2EID0        0x0B
#define RXF3SIDH        0x10
#define RXF3SIDL        0x11
#define RXF3EID8        0x12
#define RXF3EID0        0x13
#define RXF4SIDH        0x14
#define RXF4SIDL        0x15
#define RXF4EID8        0x16
#define RXF4EID0        0x17
#define RXF5SIDH        0x18
#define RXF5SIDL        0x19
#define RXF5EID8        0x1A
#define RXF5EID0        0x1B

/* Receive Masks */
#define RXM0SIDH        0x20
#define RXM0SIDL        0x21
#define RXM0EID8        0x22
#define RXM0EID0        0x23
#define RXM1SIDH        0x24
#define RXM1SIDL        0x25
#define RXM1EID8        0x26
#define RXM1EID0        0x27

/* Tx Buffer 0 */
#define TXB0CTRL        0x30
#define TXB0SIDH        0x31
#define TXB0SIDL        0x32
#define TXB0EID8        0x33
#define TXB0EID0        0x34
#define TXB0DLC         0x35
#define TXB0D0          0x36
#define TXB0D1          0x37
#define TXB0D2          0x38
#define TXB0D3          0x39
#define TXB0D4          0x3A
#define TXB0D5          0x3B
#define TXB0D6          0x3C
#define TXB0D7          0x3D
                         
/* Tx Buffer 1 */
#define TXB1CTRL        0x40
#define TXB1SIDH        0x41
#define TXB1SIDL        0x42
#define TXB1EID8        0x43
#define TXB1EID0        0x44
#define TXB1DLC         0x45
#define TXB1D0          0x46
#define TXB1D1          0x47
#define TXB1D2          0x48
#define TXB1D3          0x49
#define TXB1D4          0x4A
#define TXB1D5          0x4B
#define TXB1D6          0x4C
#define TXB1D7          0x4D

/* Tx Buffer 2 */
#define TXB2CTRL        0x50
#define TXB2SIDH        0x51
#define TXB2SIDL        0x52
#define TXB2EID8        0x53
#define TXB2EID0        0x54
#define TXB2DLC         0x55
#define TXB2D0          0x56
#define TXB2D1          0x57
#define TXB2D2          0x58
#define TXB2D3          0x59
#define TXB2D4          0x5A
#define TXB2D5          0x5B
#define TXB2D6          0x5C
#define TXB2D7          0x5D
                         
/* Rx Buffer 0 */
#define RXB0CTRL        0x60
#define RXB0SIDH        0x61
#define RXB0SIDL        0x62
#define RXB0EID8        0x63
#define RXB0EID0        0x64
#define RXB0DLC         0x65
#define RXB0D0          0x66
#define RXB0D1          0x67
#define RXB0D2          0x68
#define RXB0D3          0x69
#define RXB0D4          0x6A
#define RXB0D5          0x6B
#define RXB0D6          0x6C
#define RXB0D7          0x6D
                         
/* Rx Buffer 1 */
#define RXB1CTRL        0x70
#define RXB1SIDH        0x71
#define RXB1SIDL        0x72
#define RXB1EID8        0x73
#define RXB1EID0        0x74
#define RXB1DLC         0x75
#define RXB1D0          0x76
#define RXB1D1          0x77
#define RXB1D2          0x78
#define RXB1D3          0x79
#define RXB1D4          0x7A
#define RXB1D5          0x7B
#define RXB1D6          0x7C
#define RXB1D7          0x7D


                         
/*******************************************************************
 *                   Register Bit Mapping                          
 *******************************************************************/

//---CANSTAT-----------------
extern volatile unsigned char CANSTAT_byte;
extern volatile struct{
        unsigned :1;
        unsigned ICOD0:1;
        unsigned ICOD1:1;
        unsigned ICOD2:1;
        unsigned :1;
        unsigned OPMOD0:1;
        unsigned OPMOD1:1;
        unsigned OPMOD2:1;
}CANSTATbits;

//---CANCTRL-----------------
extern volatile unsigned char CANCTRL_byte;
extern volatile struct{
        unsigned CLKPRE0:1;
        unsigned CLKPRE1:1;
        unsigned CLKEN:1;
        unsigned OSM:1;
        unsigned ABAT:1;
        unsigned REQOP0:1;
        unsigned REQOP1:1;
        unsigned REQOP2:1;
} CANCTRLbits;

//---CNF1-----------------
extern volatile unsigned char CNF1_byte;
extern volatile struct{
        unsigned BRP0:1;
        unsigned BRP1:1;
        unsigned BRP2:1;
        unsigned BRP3:1;
        unsigned BRP4:1;
        unsigned BRP5:1;
        unsigned SJW0:1;
        unsigned SJW1:1;
} CNF1bits;

//---CNF2-----------------
extern volatile unsigned char CNF2_byte;
extern volatile struct{
        unsigned PRSEG0:1;
        unsigned PRSEG1:1;
        unsigned PRSEG2:1;
        unsigned PHSEG1_0:1;
        unsigned PHSEG1_1:1;
        unsigned PHSEG1_2:1;
        unsigned SAM:1;
        unsigned BTLMODE:1;
} CNF2bits;

//---CNF3-----------------
extern volatile unsigned char CNF3_byte;
extern volatile struct{
        unsigned PHSEG2_0:1;
        unsigned PHSEG2_1:1;
        unsigned PHSEG2_2:1;
        unsigned :1;
        unsigned :1;
        unsigned :1;
        unsigned WAKFIL:1;
        unsigned SOF:1;
} CNF3bits;

//---CANINTE-----------------
extern volatile unsigned char CANINTE_byte;
extern volatile struct{
        unsigned RX0IE:1;
        unsigned RX1IE:1;
        unsigned TX0IE:1;
        unsigned TX1IE:1;
        unsigned TX2IE:1;
        unsigned ERRIE:1;
        unsigned WAKIE:1;
        unsigned MERRE:1;
} CANINTEbits;

//---CANINTF-----------------
extern volatile unsigned char CANINTF_byte;
extern volatile struct{
        unsigned RX0IF:1;
        unsigned RX1IF:1;
        unsigned TX0IF:1;
        unsigned TX1IF:1;
        unsigned TX2IF:1;
        unsigned ERRIF:1;
        unsigned WAKIF:1;
        unsigned MERRF:1;
} CANINTFbits;

//---EFLG-----------------
extern volatile unsigned char EFLG_byte;
extern volatile struct{
        unsigned EWARN:1;
        unsigned RXWAR:1;
        unsigned TXWAR:1;
        unsigned RXEP:1;
        unsigned TXEP:1;
        unsigned TXB0:1;
        unsigned RX0OVR:1;
        unsigned RX1OVR:1;
} EFLGbits;

//---BFPCTRL-----------------
extern volatile unsigned char BFPCTRL_byte;
extern volatile struct{
        unsigned B0BFM:1;
        unsigned B1BFM:1;
        unsigned B0BFE:1;
        unsigned B1BFE:1;
        unsigned B0BFS:1;
        unsigned B1BFS:1;
        unsigned :1;
        unsigned :1;
} BFPCTRLbits;

//---TXRTSCTRL-----------------
extern volatile unsigned char TXRTSCTRL_byte;
extern volatile struct{
        unsigned B0RTSM:1;
        unsigned B1RTSM:1;
        unsigned B2RTSM:1;
        unsigned B0RTS:1;
        unsigned B1RTS:1;
        unsigned B2RTS:1;
        unsigned :1;
        unsigned :1;
} TXRTSCTRLbits;

//---TXB0CTRL-----------------
extern volatile unsigned char TXB0CTRL_byte;
extern volatile struct{
        unsigned TXP0:1;
        unsigned TXP1:1;
        unsigned :1;
        unsigned TXREQ:1;
        unsigned TXERR:1;
        unsigned MLOA:1;
        unsigned ABTF:1;
        unsigned :1;
} TXB0CTRLbits;

//---TXB1CTRL-----------------
extern volatile unsigned char TXB1CTRL_byte;
extern volatile struct{
        unsigned TXP0:1;
        unsigned TXP1:1;
        unsigned :1;
        unsigned TXREQ:1;
        unsigned TXERR:1;
        unsigned MLOA:1;
        unsigned ABTF:1;
        unsigned :1;
} TXB1CTRLbits;

//---TXB2CTRL-----------------
extern volatile unsigned char TXB2CTRL_byte;
extern volatile struct{
        unsigned TXP0:1;
        unsigned TXP1:1;
        unsigned :1;
        unsigned TXREQ:1;
        unsigned TXERR:1;
        unsigned MLOA:1;
        unsigned ABTF:1;
        unsigned :1;
} TXB2CTRLbits;

//---RXB0CTRL-----------------
extern volatile unsigned char RXB0CTRL_byte;
extern volatile struct{
        unsigned FILHIT:1;
        unsigned BUKT1:1;
        unsigned BUKT:1;
        unsigned RXRTR:1;
        unsigned :1;
        unsigned RXM0:1;
        unsigned RXM1:1;
        unsigned :1;
} RXB0CTRLbits;

//---RXB1CTRL-----------------
extern volatile unsigned char RXB1CTRL_byte;
extern volatile struct{
        unsigned FILHIT:3;
        unsigned RXRTR:1;
        unsigned :1;
        unsigned RXM0:1;
        unsigned RXM1:1;
        unsigned :1;
} RXB1CTRLbits;


///*******************************************************************
// *                    Bit Timing Configuration                     *
// *******************************************************************/
 
/* CNF1 */
#define SJW_1TQ           CNF1bits.SJW1 = 0; CNF1bits.SJW0 = 0;
#define SJW_2TQ           CNF1bits.SJW1 = 0; CNF1bits.SJW0 = 1;
#define SJW_3TQ           CNF1bits.SJW1 = 1; CNF1bits.SJW0 = 0;
#define SJW_4TQ           CNF1bits.SJW1 = 1; CNF1bits.SJW0 = 1;

#define BRP0    CNF1_byte = (CNF1_byte & 0xC0) + 0x00 ;
#define BRP1    CNF1_byte = (CNF1_byte & 0xC0) + 0x01 ;
#define BRP2    CNF1_byte = (CNF1_byte & 0xC0) + 0x02 ;
#define BRP3    CNF1_byte = (CNF1_byte & 0xC0) + 0x03 ;
#define BRP4    CNF1_byte = (CNF1_byte & 0xC0) + 0x04 ;
#define BRP5    CNF1_byte = (CNF1_byte & 0xC0) + 0x05 ;
#define BRP6    CNF1_byte = (CNF1_byte & 0xC0) + 0x06 ;
#define BRP7    CNF1_byte = (CNF1_byte & 0xC0) + 0x07 ;
#define BRP8    CNF1_byte = (CNF1_byte & 0xC0) + 0x08 ;
#define BRP9    CNF1_byte = (CNF1_byte & 0xC0) + 0x09 ;
#define BRP10   CNF1_byte = (CNF1_byte & 0xC0) + 0x10;
#define BRP11   CNF1_byte = (CNF1_byte & 0xC0) + 0x11;
#define BRP12   CNF1_byte = (CNF1_byte & 0xC0) + 0x12;
#define BRP13   CNF1_byte = (CNF1_byte & 0xC0) + 0x13;
#define BRP14   CNF1_byte = (CNF1_byte & 0xC0) + 0x14;
#define BRP15   CNF1_byte = (CNF1_byte & 0xC0) + 0x15;
#define BRP16   CNF1_byte = (CNF1_byte & 0xC0) + 0x16;
#define BRP17   CNF1_byte = (CNF1_byte & 0xC0) + 0x17;
#define BRP18   CNF1_byte = (CNF1_byte & 0xC0) + 0x18;
#define BRP19   CNF1_byte = (CNF1_byte & 0xC0) + 0x19;
#define BRP20   CNF1_byte = (CNF1_byte & 0xC0) + 0x20;
#define BRP21   CNF1_byte = (CNF1_byte & 0xC0) + 0x21;
#define BRP22   CNF1_byte = (CNF1_byte & 0xC0) + 0x22;
#define BRP23   CNF1_byte = (CNF1_byte & 0xC0) + 0x23;
#define BRP24   CNF1_byte = (CNF1_byte & 0xC0) + 0x24;
#define BRP25   CNF1_byte = (CNF1_byte & 0xC0) + 0x25;
#define BRP26   CNF1_byte = (CNF1_byte & 0xC0) + 0x26;
#define BRP27   CNF1_byte = (CNF1_byte & 0xC0) + 0x27;
#define BRP28   CNF1_byte = (CNF1_byte & 0xC0) + 0x28;
#define BRP29   CNF1_byte = (CNF1_byte & 0xC0) + 0x29;
#define BRP30   CNF1_byte = (CNF1_byte & 0xC0) + 0x30;
#define BRP31   CNF1_byte = (CNF1_byte & 0xC0) + 0x31;

/* CNF2 */
#define BTLMODE_CNF3      CNF2bits.BTLMODE = 1;
#define BTLMODE_PH1_IPT   CNF2bits.BTLMODE = 0;
                          
#define SMPL_3X           CNF2bits.SAM = 1;
#define SMPL_1X           CNF2bits.SAM = 0;

#define PHSEG1_1TQ  CNF2bits.PHSEG1_2 = 0; CNF2bits.PHSEG1_1 = 0; CNF2bits.PHSEG1_0 = 0;
#define PHSEG1_2TQ  CNF2bits.PHSEG1_2 = 0; CNF2bits.PHSEG1_1 = 0; CNF2bits.PHSEG1_0 = 1;
#define PHSEG1_3TQ  CNF2bits.PHSEG1_2 = 0; CNF2bits.PHSEG1_1 = 1; CNF2bits.PHSEG1_0 = 0;
#define PHSEG1_4TQ  CNF2bits.PHSEG1_2 = 0; CNF2bits.PHSEG1_1 = 1; CNF2bits.PHSEG1_0 = 1;
#define PHSEG1_5TQ  CNF2bits.PHSEG1_2 = 1; CNF2bits.PHSEG1_1 = 0; CNF2bits.PHSEG1_0 = 0;
#define PHSEG1_6TQ  CNF2bits.PHSEG1_2 = 1; CNF2bits.PHSEG1_1 = 0; CNF2bits.PHSEG1_0 = 1;
#define PHSEG1_7TQ  CNF2bits.PHSEG1_2 = 1; CNF2bits.PHSEG1_1 = 1; CNF2bits.PHSEG1_0 = 0;
#define PHSEG1_8TQ  CNF2bits.PHSEG1_2 = 1; CNF2bits.PHSEG1_1 = 1; CNF2bits.PHSEG1_0 = 1;

#define PRSEG_1TQ  CNF2bits.PRSEG2 = 0; CNF2bits.PRSEG1 = 0; CNF2bits.PRSEG0 = 0;
#define PRSEG_2TQ  CNF2bits.PRSEG2 = 0; CNF2bits.PRSEG1 = 0; CNF2bits.PRSEG0 = 1;
#define PRSEG_3TQ  CNF2bits.PRSEG2 = 0; CNF2bits.PRSEG1 = 1; CNF2bits.PRSEG0 = 0;
#define PRSEG_4TQ  CNF2bits.PRSEG2 = 0; CNF2bits.PRSEG1 = 1; CNF2bits.PRSEG0 = 1;
#define PRSEG_5TQ  CNF2bits.PRSEG2 = 1; CNF2bits.PRSEG1 = 0; CNF2bits.PRSEG0 = 0;
#define PRSEG_6TQ  CNF2bits.PRSEG2 = 1; CNF2bits.PRSEG1 = 0; CNF2bits.PRSEG0 = 1;
#define PRSEG_7TQ  CNF2bits.PRSEG2 = 1; CNF2bits.PRSEG1 = 1; CNF2bits.PRSEG0 = 0;
#define PRSEG_8TQ  CNF2bits.PRSEG2 = 1; CNF2bits.PRSEG1 = 1; CNF2bits.PRSEG0 = 1;

/* CNF3 */
#define PHSEG2_1TQ  CNF3bits.PHSEG2_2 = 0; CNF3bits.PHSEG2_1 = 0; CNF3bits.PHSEG2_0 = 0;
#define PHSEG2_2TQ  CNF3bits.PHSEG2_2 = 0; CNF3bits.PHSEG2_1 = 0; CNF3bits.PHSEG2_0 = 1;
#define PHSEG2_3TQ  CNF3bits.PHSEG2_2 = 0; CNF3bits.PHSEG2_1 = 1; CNF3bits.PHSEG2_0 = 0;
#define PHSEG2_4TQ  CNF3bits.PHSEG2_2 = 0; CNF3bits.PHSEG2_1 = 1; CNF3bits.PHSEG2_0 = 1;
#define PHSEG2_5TQ  CNF3bits.PHSEG2_2 = 1; CNF3bits.PHSEG2_1 = 0; CNF3bits.PHSEG2_0 = 0;
#define PHSEG2_6TQ  CNF3bits.PHSEG2_2 = 1; CNF3bits.PHSEG2_1 = 0; CNF3bits.PHSEG2_0 = 1;
#define PHSEG2_7TQ  CNF3bits.PHSEG2_2 = 1; CNF3bits.PHSEG2_1 = 1; CNF3bits.PHSEG2_0 = 0;
#define PHSEG2_8TQ  CNF3bits.PHSEG2_2 = 1; CNF3bits.PHSEG2_1 = 1; CNF3bits.PHSEG2_0 = 1;

#define SOF_ENABLED     CNF3bits.SOF = 1;
#define SOF_DISABLED    CNF3bits.SOF = 0;
#define WAKFIL_ENABLED  CNF3bits.WAKFIL = 1;
#define WAKFIL_DISABLED CNF3bits.WAKFIL = 0;    


/*******************************************************************
 *                  Control/Configuration Registers                *
 *******************************************************************/
/* CANINTE */
#define RXB0_INT_ENABLE     CANINTEbits.RX0IE = 1;
#define RXB0_INT_DISABLED   CANINTEbits.RX0IE = 0;
#define RXB1_INT_ENABLED    CANINTEbits.RX1IE = 1;
#define RXB1_INT_DISABLED   CANINTEbits.RX1IE = 0;
#define G_RXB_INT_ENABLED   CANINTEbits.RX0IE = 1; CANINTEbits.RX1IE = 1;
#define G_RXB_INT_DISABLED  CANINTEbits.RX0IE = 0; CANINTEbits.RX1IE = 0;

#define TXB0_INT_ENABLED    CANINTEbits.TX0IE = 1;                       
#define TXB0_INT_DISABLED   CANINTEbits.TX0IE = 0;                       
#define TXB1_INT_ENABLED    CANINTEbits.TX1IE = 1;                       
#define TXB1_INT_DISABLED   CANINTEbits.TX1IE = 0;                       
#define TXB2_INT_ENABLED    CANINTEbits.TX2IE = 1;
#define TXB2_INT_DISABLED   CANINTEbits.TX2IE = 0;
#define G_TXB_INT_ENABLED   CANINTEbits.TX0IE = 1; CANINTEbits.TX1IE = 1; CANINTEbits.TX2IE = 1;
#define G_TXB_INT_DISABLED  CANINTEbits.TX0IE = 0; CANINTEbits.TX1IE = 0; CANINTEbits.TX2IE = 0;

#define ERRIE_ENABLED       CANINTEbits.ERRIE = 1;
#define ERRIE_DISABLED      CANINTEbits.ERRIE = 0;
#define WAKIE_ENABLED       CANINTEbits.WAKIE = 1;
#define WAKIE_DISABLED      CANINTEbits.WAKIE = 0;
#define MERRE_ENABLED       CANINTEbits.MERRE = 1;
#define IVRE_DISABLED       CANINTEbits.MERRE = 0;

/* CANINTF */
#define RXB0_INT_STATUS     CANINTFbits.RX0IF;
#define RXB0_INT_SET        CANINTFbits.RX0IF = 1;
#define RXB0_INT_RESET      CANINTFbits.RX0IF = 0;

#define RXB1_INT_STATUS     CANINTFbits.RX1IF;
#define RXB1_INT_SET        CANINTFbits.RX1IF = 1;
#define RXB1_INT_RESET      CANINTFbits.RX1IF = 0;
                            
#define TXB0_INT_STATUS     CANINTFbits.TX0IF;
#define TXB0_INT_SET        CANINTFbits.TX0IF = 1;
#define TXB0_INT_RESET      CANINTFbits.TX0IF = 0;

#define TXB1_INT_STATUS     CANINTFbits.TX1IF;
#define TXB1_INT_SET        CANINTFbits.TX1IF = 1;
#define TXB1_INT_RESET      CANINTFbits.TX1IF = 0;

#define TXB2_INT_STATUS     CANINTFbits.TX2IF;
#define TXB2_INT_SET        CANINTFbits.TX2IF = 1;
#define TXB2_INT_RESET      CANINTFbits.TX2IF = 0;

#define ERRIF_STATUS        CANINTFbits.ERRIF;
#define ERRIF_SET           CANINTFbits.ERRIF = 1;
#define ERRIF_RESET         CANINTFbits.ERRIF = 0;

#define WAKIF_STATUS        CANINTFbits.WAKIF;
#define WAKIF_SET           CANINTFbits.WAKIF = 1;
#define WAKIF_RESET         CANINTFbits.WAKIF = 0;

#define MERRF_STATUS        CANINTFbits.MERRF;
#define MERRF_SET           CANINTFbits.MERRF = 1;
#define MERRF_RESET         CANINTFbits.MERRF = 0;

/* CANCTRL */ 
#define SET_CONFIG    CANCTRLbits.REQOP2 = 1; CANCTRLbits.REQOP1 = 0; CANCTRLbits.REQOP0 = 0;
#define SET_LISTEN    CANCTRLbits.REQOP2 = 0; CANCTRLbits.REQOP1 = 1; CANCTRLbits.REQOP0 = 1;
#define SET_LOOPBACK  CANCTRLbits.REQOP2 = 0; CANCTRLbits.REQOP1 = 1; CANCTRLbits.REQOP0 = 0;
#define SET_SLEEP     CANCTRLbits.REQOP2 = 0; CANCTRLbits.REQOP1 = 0; CANCTRLbits.REQOP0 = 1;     
#define SET_NORMAL    CANCTRLbits.REQOP2 = 0; CANCTRLbits.REQOP1 = 0; CANCTRLbits.REQOP0 = 0;

#define SET_ABORT     CANCTRLbits.ABAT = 1;
#define CLEAR_ABORT   CANCTRLbits.ABAT = 0;

#define SET_OSM       CANCTRLbits.OSM = 1;
#define CLEAR_OSM     CANCTRLbits.OSM = 0;

#define CLKOUT_ENABLED  CANCTRLbits.CLKEN = 1;
#define CLKOUT_DISABLED CANCTRLbits.CLKEN = 0;
#define CLKOUT_PRE_8    CANCTRLbits.CLKPRE1 = 1; CANCTRLbits.CLKPRE0 = 1;
#define CLKOUT_PRE_4    CANCTRLbits.CLKPRE1 = 1; CANCTRLbits.CLKPRE0 = 0;
#define CLKOUT_PRE_2    CANCTRLbits.CLKPRE1 = 0; CANCTRLbits.CLKPRE0 = 1;
#define CLKOUT_PRE_1    CANCTRLbits.CLKPRE1 = 0; CANCTRLbits.CLKPRE0 = 0;

/* CANSTAT */
#define OPMODE_CONFIG   0x80
#define OPMODE_LISTEN   0x60
#define OPMODE_LOOPBACK 0x40
#define OPMODE_SLEEP    0x20
#define OPMODE_NORMAL   0x00

/* BFPCTRL */
#define RXB0BF_PIN_DIS  BFPCTRLbits.B0BFE = 0;
#define RXB1BF_PIN_DIS  BFPCTRLbits.B1BFE = 0;
#define RXB0BF_PIN_EN   BFPCTRLbits.B0BFE = 1;
#define RXB1BF_PIN_EN   BFPCTRLbits.B1BFE = 1;
#define RXB0BF_PIN_INT  BFPCTRLbits.B0BFE = 1; BFPCTRLbits.B0BFM = 1;
#define RXB1BF_PIN_INT  BFPCTRLbits.B1BFE = 1; BFPCTRLbits.B1BFM = 1;
#define RXB0BF_PIN_GPO  BFPCTRLbits.B0BFE = 1; BFPCTRLbits.B0BFM = 0;
#define RXB1BF_PIN_GPO  BFPCTRLbits.B1BFE = 1; BFPCTRLbits.B1BFM = 0;


///* RXBnCTRL */
//#define RXM_RCV_ALL     0x60
//#define RXM_VALID_EXT   0x40
//#define RXM_VALID_STD   0x20
//#define RXM_VALID_ALL   0x00
//
//#define RXRTR_REMOTE    0x08
//#define RXRTR_NO_REMOTE 0x00
//
//#define BUKT_ROLLOVER    0x04
//#define BUKT_NO_ROLLOVER 0x00
//
//#define FILHIT0_FLTR_1  0x01
//#define FILHIT0_FLTR_0  0x00
//
//#define FILHIT1_FLTR_5  0x05
//#define FILHIT1_FLTR_4  0x04
//#define FILHIT1_FLTR_3  0x03
//#define FILHIT1_FLTR_2  0x02
//#define FILHIT1_FLTR_1  0x01
//#define FILHIT1_FLTR_0  0x00
//
//
///* TXBnCTRL */
//#define TXREQ_SET       0x08
//#define TXREQ_CLEAR     0x00
//
//#define TXP_HIGHEST     0x03
//#define TXP_INTER_HIGH  0x02
//#define TXP_INTER_LOW   0x01
//#define TXP_LOWEST      0x00
//    

/*******************************************************************
*                  Register Bit Masks                             *
*******************************************************************/

#define DLC_0          0x00
#define DLC_1          0x01
#define DLC_2          0x02
#define DLC_3          0x03
#define DLC_4          0x04
#define DLC_5          0x05
#define DLC_6          0x06
#define DLC_7          0x07    
#define DLC_8          0x08


//---- Copied to "SPI.H"
///*******************************************************************
// *                  CAN SPI commands                               *
// *******************************************************************/
//
//#define CAN_RESET       0xC0
//#define CAN_READ        0x03
//#define CAN_WRITE       0x02
//#define CAN_RTS         0x80
//#define CAN_RTS_TXB0    0x81
//#define CAN_RTS_TXB1    0x82
//#define CAN_RTS_TXB2    0x84
//#define CAN_RD_STATUS   0xA0
//#define CAN_BIT_MODIFY  0x05  
//#define CAN_RX_STATUS   0xB0
//#define CAN_RD_RX_BUFF  0x90
//#define CAN_LOAD_TX     0X40  

//END- Copied to "SPI.H"

/*******************************************************************
 *                  Miscellaneous                                  *
 *******************************************************************/

#define DUMMY_BYTE      0x00
#define TXB0            0x31
#define TXB1            0x41
#define TXB2            0x51
#define RXB0            0x61
#define RXB1            0x71
#define EXIDE_SET       0x08
#define EXIDE_RESET     0x00

#endif //REGS2515_H
