/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB½ C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author          	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Vinaya Skanda 	10/18/06  First release of source file
* Vinaya Skanda		07/25/07  Updates from Joe Supinsky and Jatinder Gharoo incorporated
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This code is tested on Explorer-16 board with ECAN PICTail Card.
* The device used is dsPIC33FJ256GP710 controller 
*************************************************************************************************/

#if defined(__dsPIC33F__)
#include "p33fxxxx.h"
#elif defined(__PIC24H__)
#include "p24hxxxx.h"
#endif

#include<ECAN2Config.h>

/* Dma Initialization for ECAN2 Transmission */

void dma1init(void)
{

    DMA1CON = 0x2020;
    DMA1PAD = (int)&C2TXD;	/* ECAN 2 (C2TXD) */
    DMA1CNT = 0x0007;
    DMA1REQ = 0x0047;		/* ECAN 2 Transmit */
    DMA1STA = __builtin_dmaoffset(ecan2msgBuf);
    DMA1CONbits.CHEN = 1;

}

/* Dma Initialization for ECAN2 Reception */
void dma3init(void)
{

    DMACS0 = 0;
    DMA3CON = 0x0020;
    DMA3PAD = (int)&C2RXD;	/* ECAN 2 (C2RXD) */
    DMA3CNT = 0x0007;
    DMA3REQ = 0x0037;		/* ECAN 2 Receive */
    DMA3STA = __builtin_dmaoffset(ecan2msgBuf);
    DMA3CONbits.CHEN = 1;

}

void ecan2ClkInit(void)
{

/* FCAN is selected to be FCY
/* FCAN = FCY = 40MHz */
    C2CTRL1bits.CANCKS = 0x1;

/*
Bit Time = (Sync Segment + Propagation Delay + Phase Segment 1 + Phase Segment 2)=20*TQ
Phase Segment 1 = 8TQ
Phase Segment 2 = 6Tq
Propagation Delay = 5Tq
Sync Segment = 1TQ
CiCFG1<BRP> =(FCAN /(2 ×N×FBAUD))– 1
*/

    /* Synchronization Jump Width set to 4 TQ */
    C2CFG1bits.SJW = 0x3;
    /* Baud Rate Prescaler */
    C2CFG1bits.BRP = BRP_VAL;

    /* Phase Segment 1 time is 8 TQ */
    C2CFG2bits.SEG1PH = 0x7;
    /* Phase Segment 2 time is set to be programmable */
    C2CFG2bits.SEG2PHTS = 0x1;
    /* Phase Segment 2 time is 6 TQ */
    C2CFG2bits.SEG2PH = 0x5;
    /* Propagation Segment time is 5 TQ */
    C2CFG2bits.PRSEG = 0x4;
    /* Bus line is sampled three times at the sample point */
    C2CFG2bits.SAM = 0x1;

}

void ecan2Init(void)
{

/* Request Configuration Mode */

    C2CTRL1bits.REQOP = 4;
    while (C2CTRL1bits.OPMODE != 4) ;

    ecan2ClkInit();

    C2FCTRLbits.FSA = 0 b01000;	/* FIFO Starts at Message Buffer 16 */
    C2FCTRLbits.DMABS = 0 b110;	/* 32 CAN Message Buffers in DMA RAM */

/*	Filter Configuration

	ecan2WriteRxAcptFilter(int n, long identifier, unsigned int exide,unsigned int bufPnt,unsigned int maskSel)

	n = 0 to 15 -> Filter number

	identifier -> SID <10:0> : EID <17:0> 

	exide = 0 -> Match messages with standard identifier addresses 
	exide = 1 -> Match messages with extended identifier addresses 

	bufPnt = 0 to 14  -> RX Buffer 0 to 14
	bufPnt = 15 -> RX FIFO Buffer

	maskSel = 0	->	Acceptance Mask 0 register contains mask
	maskSel = 1	->	Acceptance Mask 1 register contains mask
	maskSel = 2	->	Acceptance Mask 2 register contains mask
	maskSel = 3	->	No Mask Selection
	
*/

    ecan2WriteRxAcptFilter(1, 0x1FFEFFFF, 1, 15, 0);

/*	Mask Configuration

	ecan2WriteRxAcptMask(int m, long identifierMask, unsigned int mide, unsigned int exide)

	m = 0 to 2 -> Mask Number

	identifier -> SID <10:0> : EID <17:0> 

	mide = 0 -> Match either standard or extended address message if filters match 
	mide = 1 -> Match only message types that correpond to 'exide' bit in filter

	exide = 0 -> Match messages with standard identifier addresses 
	exide = 1 -> Match messages with extended identifier addresses 

	
*/

    ecan2WriteRxAcptMask(1, 0x1FFFFFFF, 1, 1);

/* Enter Normal Mode */

    C2CTRL1bits.REQOP = 0;
    while (C2CTRL1bits.OPMODE != 0) ;

/* ECAN transmit/receive message control */

    C2RXFUL1 = C2RXFUL2 = C2RXOVF1 = C2RXOVF2 = 0x0000;
    C2TR01CONbits.TXEN0 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR01CONbits.TXEN1 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR23CONbits.TXEN2 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR23CONbits.TXEN3 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR45CONbits.TXEN4 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */
    C2TR45CONbits.TXEN5 = 1;	/* ECAN2, Buffer 0 is a Transmit Buffer */

    C2TR01CONbits.TX0PRI = 0 b11;	/* Message Buffer 0 Priority Level */
    C2TR01CONbits.TX1PRI = 0 b11;	/* Message Buffer 1 Priority Level */
    C2TR23CONbits.TX2PRI = 0 b11;	/* Message Buffer 0 Priority Level */
    C2TR23CONbits.TX3PRI = 0 b11;	/* Message Buffer 1 Priority Level */
    C2TR45CONbits.TX4PRI = 0 b11;	/* Message Buffer 0 Priority Level */
    C2TR45CONbits.TX5PRI = 0 b11;	/* Message Buffer 1 Priority Level */

}
