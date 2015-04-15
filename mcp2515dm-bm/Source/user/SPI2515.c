/*********************************************************************
 *
 *                Microchip USB C18 Firmware - MCP2515 SPI Source
 *
 *********************************************************************
 * FileName:        SPI2515.c
 * Dependencies:    See INCLUDES section below
 * Processor:       
 * Compiler:        C18
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
 * Pat Richards       xx/xx/xx    Original.
 ********************************************************************/
#include <p18cxxx.h>
#include "spi.h"
#include "io_cfg.h"             // I/O pin mapping
#include "REGS2515.h"

/** V A R I A B L E S ********************************************************/
#pragma udata
unsigned char dummy;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
// See BusMon.h for Public Prototypes
void Delay_ms(unsigned char num_ms);
char SPIReadStatus(void);

/** D E C L A R A T I O N S **************************************************/

#define Mode00  0
#define Mode11  1

#define	ON	1
#define	OFF	0

#define HIGH  1
#define LOW   0


//#pragma code
//-----------------------------------------------------------------------------
//  SPIReset()
//-----------------------------------------------------------------------------
void SPIReset(void)
{
  CS_2515_LOW();
	WriteSPI(CAN_RESET);
  CS_2515_HIGH();
  for(dummy=0; dummy<255; dummy++);


}

//-------------------------------------------------------------------------
//  SPIByteWrite()
//-------------------------------------------------------------------------
void SPIByteWrite(unsigned char addr, unsigned char value )
{
  CS_2515_LOW();
  while(WriteSPI(CAN_WRITE));
  while(WriteSPI(addr));
  while(WriteSPI(value));
  CS_2515_HIGH();
}

//-------------------------------------------------------------------------
//     Function Name:    SPISegWrite                                
//     Return Value:     None                                        
//     Parameters:       Starting address, numbytes, and pointer
//                       to an array.               
//     Description:      This routine performs a sequential write.         
//-------------------------------------------------------------------------
void SPISeqWrite(unsigned char startaddr, unsigned char numbytes, char *data)
{
  unsigned char n;

  CS_2515_LOW();
  while( WriteSPI(CAN_WRITE) );
  while( WriteSPI(startaddr) );
  for(n=0; n<numbytes; n++)
    while( WriteSPI(data[n]) );
  CS_2515_HIGH();
}


//-----------------------------------------------------------------------------
//  SPIByteRead()
//-----------------------------------------------------------------------------
unsigned char SPIByteRead(unsigned char addr)
{
  unsigned char tempdata;
  CS_2515_LOW();
  WriteSPI(CAN_READ);
  WriteSPI(addr);
  tempdata = ReadSPI();
  CS_2515_HIGH();
  return tempdata;
}

//-----------------------------------------------------------------------------
//	SPISeqRead(unsigned char startaddr, unsigned char numbytes, *data)
//     Return Value:     None (puts read data in string)                                        
//     Parameters:       Starting address, numbytes, and pointer
//                       to an array.               
//     Description:      Sequential read from the MCP2515         
//-----------------------------------------------------------------------------
void SPISeqRead(unsigned char startaddr, unsigned char numbytes, char *data)
{
  unsigned char n;
  CS_2515_LOW();
  while( WriteSPI(CAN_READ) );
  while( WriteSPI(startaddr) );
  for(n=0; n<numbytes; n++)
  {
    data[n] = ReadSPI();
	}
  data[n] = ReadSPI();  //Last byte
  CS_2515_HIGH();
}

//-----------------------------------------------------------------------------
//  SPIReadRX(unsigned char opcode, unsigned char numbytes, unsigned char *data)
//	opcode = SPI opcode and pointer to RXBn register location// (see datasheet)
//  numbytes = number of bytes to read
//  *data = pointer to a string of data
//	
//-----------------------------------------------------------------------------
void SPIReadRX(unsigned char opcode, unsigned char numbytes, char *data)
{
  unsigned char n;
  CS_2515_LOW();
  while( WriteSPI(opcode));
  for(n=0; n<numbytes; n++)
  {
    data[n] = ReadSPI();
	}
  data[n] = ReadSPI();  //Last byte
  CS_2515_HIGH();
}

//-----------------------------------------------------------------------------
//  SPILoadTX(unsigned char opcode, unsigned char numbytes, unsigned char *data)
//	opcode = SPI opcode and pointer to TXBn register location// (see datasheet)
//  numbytes = number of bytes to write
//  *data = pointer to a string of data
//	
//-----------------------------------------------------------------------------
char SPILoadTX(unsigned char opcode, unsigned char numbytes, char *data)
{
  unsigned char n, buff;
  
  //Check if this function is suppossed to choose the TX buffer to load
  if(opcode == CAN_LOAD_TX)//This function chooses the buffer
  {
    n = SPIReadStatus();
    //Check for free buffer
    if(!(n & 0x04))             //TXB0 is available
      opcode = CAN_LD_TXB0_ID;  //Point to TXB0SIDH
    else if(!(n & 0x10))        //TXB1 is available
      opcode = CAN_LD_TXB1_ID;  //Point to TXB1SIDH
    else if(!(n & 0x40))        //TXB2 is available
      opcode = CAN_LD_TXB2_ID;  //Point to TXB2SIDH
    else if(n & 0x54)           //No buffers available
    {
      SPIByteWrite(TXB0CTRL, 0x00); //Clear TXREQ (and whole register)
      for(dummy=0; dummy<255; dummy++);
      opcode = CAN_LD_TXB0_ID;  //Point to TXB0SIDH
    }
  }

  CS_2515_LOW();
  while( WriteSPI(opcode) );
  for(n=0; n<numbytes; n++)
   while( WriteSPI(data[n]) );
  CS_2515_HIGH();
  
  return(opcode); //So calling function knows which buffer was loaded
}

//-----------------------------------------------------------------------------
//  RTS(buffer)
//	buffer = CAN_RTS_TXBn; where 'n' = 0, 1, 2
//  OR
//	buffer = CAN_RTS; followed by | 0 - 7 (e.g., "CAN_RTS | 7" sends TX0 and TX1)
//  OR
//	buffer = CAN_RTS_TXBn | CAN_RTS_TXBn | CAN_RTS_TXBn; where 'n' = 0, 1, 2
//-----------------------------------------------------------------------------
void SPI_RTS(unsigned char buffer)
{
  unsigned char tempdata;
	tempdata = SPIReadStatus();
	
	if(buffer & 0x01)	//Buffer 0	
		if(tempdata & 0x04)	//Check TXREQ first
		{
			Delay_ms(1);
			SPIByteWrite(TXB0CTRL, 0);			//Clear TXREQ (and everything else... not clean)
			while(SPIReadStatus() & 0x04); //Wait for TXREQ to clear
	  }
	if(buffer & 0x02)	//Buffer 1
		if(tempdata & 0x10)	//Check TXREQ first
		{
  		Delay_ms(1);
  		SPIByteWrite(TXB1CTRL, 0);			//Clear TXREQ (and everything else... not clean)
			while(SPIReadStatus() & 0x10); //Wait for TXREQ to clear
		}
	if(buffer & 0x04)	//Buffer 2
		if(tempdata & 0x40)	//Check TXREQ first
		{
  		Delay_ms(1);
  		SPIByteWrite(TXB2CTRL, 0);			//Clear TXREQ (and everything else... not clean)
			while(SPIReadStatus() & 0x40); //Wait for TXREQ to clear
		}
	
  CS_2515_LOW();
	WriteSPI(buffer);
  CS_2515_HIGH();
}

//-----------------------------------------------------------------------------
//  char SPIReadStatus//(void)
//    Performs Read Status command and returns result
//-----------------------------------------------------------------------------
char SPIReadStatus(void)
{
  unsigned char tempdata;
  CS_2515_LOW();
  WriteSPI(CAN_RD_STATUS);
  tempdata = ReadSPI();
  CS_2515_HIGH();
  return tempdata;
}

//-----------------------------------------------------------------------------
//  Delay_ms()
//  Tcy = 1 us
//  Tcy X 1000 = 1 ms
//  FFFFh - 3E8h (1000d) = FC17h
//  For(){} loop is in 1 ms increments
//-----------------------------------------------------------------------------
void Delay_ms(unsigned char num_ms)
{
  unsigned char n;
  
  TMR1H = 0xFC;
  TMR1L = 0x17;
  
  T1CONbits.TMR1ON = 1; //Start timer
  
  for(n = 0; n < num_ms; n++)
  {
    while(!PIR1bits.TMR1IF);  //Wait for timer flag
    PIR1bits.TMR1IF = 0;      //Clear flag
    TMR1H = 0xFC;
    TMR1L = 0x17;
  }

  T1CONbits.TMR1ON = 0; //Stop timer
}
