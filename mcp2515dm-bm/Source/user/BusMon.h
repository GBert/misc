/*********************************************************************
 *
 *                Microchip USB C18 Firmware - Mouse Demo
 *
 *********************************************************************
 * FileName:        MCP3551.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.10
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Pat Richards       06/12/08   Original.
 ********************************************************************/

#ifndef BusMon_H
#define BusMon_H


#define BUF_SIZE        64

// ASCII commands from sent by GUI
#define START_ACQUISITION   'S'    // Start aquisition of data
#define STOP_ACQUISITION    'T'    // Stop aquisition of data
#define CHANNEL_SELECTION   'C'    // A/D Channel SelectionCANLoading

#define OFF   0
#define ON    1

#define PERCENT_0   0x00
#define PERCENT_25  0x01
#define PERCENT_50  0x02
#define PERCENT_75  0x03
#define PERCENT_100 0x04

//USB Message Parse: Defines locations in 64 byte USB message
//CAN Message X
#define mx_SIDH   0
#define mx_SIDL   1
#define mx_EID8   2
#define mx_EID0   3
#define mx_DLC    4
#define mx_D0     5
#define mx_D1     6
#define mx_D2     7
#define mx_D3     8
#define mx_D4     9
#define mx_D5     10
#define mx_D6     11
#define mx_D7     12


//CAN Message 1
#define m1_SIDH   0
#define m1_SIDL   1
#define m1_EID8   2
#define m1_EID0   3
#define m1_DLC    4
#define m1_D0     5
#define m1_D1     6
#define m1_D2     7
#define m1_D3     8
#define m1_D4     9
#define m1_D5     10
#define m1_D6     11
#define m1_D7     12

//CAN Message 2
#define m2_SIDH   13
#define m2_SIDL   14
#define m2_EID8   15
#define m2_EID0   16
#define m2_DLC    17
#define m2_D0     18
#define m2_D1     19
#define m2_D2     20
#define m2_D3     21
#define m2_D4     22
#define m2_D5     23
#define m2_D6     24
#define m2_D7     25

//CAN Message 3
#define m3_SIDH   26
#define m3_SIDL   27
#define m3_EID8   28
#define m3_EID0   29
#define m3_DLC    30
#define m3_D0     31
#define m3_D1     32
#define m3_D2     33
#define m3_D3     34
#define m3_D4     35
#define m3_D5     36
#define m3_D6     37
#define m3_D7     38

//CAN Message 4
#define m4_SIDH   39
#define m4_SIDL   40
#define m4_EID8   41
#define m4_EID0   42
#define m4_DLC    43
#define m4_D0     44
#define m4_D1     45
#define m4_D2     46
#define m4_D3     47
#define m4_D4     48
#define m4_D5     49
#define m4_D6     50
#define m4_D7     51

//Registers, Status, Control
#define u_CANmsgs 52
#define u_CANINTF 53
#define u_EFLG    54
#define u_TEC     55
#define u_REC     56
#define u_CANSTAT 57
#define u_CANCTRL 58
#define u_STATUS  59
#define u_SPI     60
#define u_REG     61
#define u_DATA    62
#define u_res     63

/** V A R I A B L E S ********************************************************/
static unsigned char gCANRXFlag;  //0 = None; 1 = RXB0; 2 = RXB1; 3 = Both

/** S T R U C T S ********************************************************/
extern struct {
  unsigned char   INTF:1;   //bit 0 
	unsigned char   MCP_RXBn:2; 
  unsigned char   SOF:1;
  unsigned char   USBsend:1;
  unsigned char   USBQueue:1;
  unsigned char   CANLoading:1;
  unsigned char   Flag8:1;
} UserFlag;


extern struct {
  unsigned char   BusLoad:2;    //bit 0 
	unsigned char   TX_buff:2;    //OUT message to instruct TX buff to send
  unsigned char   CAN_USB_PTR:2;
  unsigned char   nu2:1;
  unsigned char   nu3:1;
} usbcan_STATUS;


/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);
void CANInit(void);
void InterruptHandler(void);
char CheckCANRX(void);
void LoadUSBString(char *data);
void GetUSBData(char start_ptr, char num_bytes, char *data);

//Used in SPI2515.C
void SPIReset(void);
void SPIByteWrite(unsigned char addr, unsigned char value );
void SPISeqWrite(unsigned char startaddr, unsigned char numbytes, char *data);
unsigned char SPIByteRead(unsigned char addr);
void SPISeqRead(unsigned char startaddr, unsigned char numbytes, char *data);
void SPIReadRX(unsigned char opcode, unsigned char numbytes, char *data);
char SPILoadTX(unsigned char opcode, unsigned char numbytes, char *data);
void SPI_RTS(unsigned char buffer);
char SPIReadStatus(void);
void EEPROMBYTEWrite(char addr,char data);
void EEPROMDataWrite(char addr,char* data,char len);
char EEPROMBYTERead(char addr);
void EEPROMDataRead(char addr,char* data,char len);
void EEPROMCRCWrite(char addr,char len);
BOOL EEPROMCRCCheck(char addr,char len);
#endif //BusMon_H
