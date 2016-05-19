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
*
* The Processor starts with the External Crystal without PLL enabled and then the Clock is switched to PLL Mode.
*************************************************************************************************/

#if defined(__dsPIC33F__)
#include "p33fxxxx.h"
#elif defined(__PIC24H__)
#include "p24hxxxx.h"
#endif

// #include <ECAN1Config.h>
// #include <ECAN2Config.h>

//  Macros for Configuration Fuse Registers 
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
								// Clock Switching is enabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: XT Crystanl

_FWDT(FWDTEN_OFF);		// Watchdog Timer Enabled/disabled by user software
								// (LPRC can be disabled by clearing SWDTEN bit in RCON register

// Define ECAN Message Buffers
ECAN1MSGBUF ecan1msgBuf __attribute__ ((space(dma), aligned(ECAN1_MSG_BUF_LENGTH * 16)));
ECAN2MSGBUF ecan2msgBuf __attribute__ ((space(dma), aligned(ECAN2_MSG_BUF_LENGTH * 16)));

// Prototype Declaration
void oscConfig(void);
void clearIntrflags(void);
void ecan1WriteMessage(void);
void ecan2WriteMessage(void);

int main(void)
{

/* Configure Oscillator Clock Source 	*/
    oscConfig();

/* Clear Interrupt Flags 				*/
    clearIntrflags();

/* ECAN1 Initialisation 		
   Configure DMA Channel 0 for ECAN1 Transmit
   Configure DMA Channel 2 for ECAN1 Receive */
    ecan1Init();
    dma0init();
    dma2init();

/* Enable ECAN1 Interrupt 	*/

    IEC2bits.C1IE = 1;
    C1INTEbits.TBIE = 1;
    C1INTEbits.RBIE = 1;

/* ECAN2 Initialisation 		
   Configure DMA Channel 1 for ECAN2 Transmit
   Configure DMA Channel 3 for ECAN2 Receive */
    ecan2Init();
    dma1init();
    dma3init();

/* Enable ECAN2 Interrupt 	*/
    IEC3bits.C2IE = 1;
    C2INTEbits.TBIE = 1;
    C2INTEbits.RBIE = 1;

/* Write Message to ECAN1 Message Buffer 0 to 5 	
   Request Message Transmission			*/
    ecan1WriteMessage();
    C1TR01CONbits.TXREQ0 = 1;
    C1TR01CONbits.TXREQ1 = 1;
    C1TR23CONbits.TXREQ2 = 1;
    C1TR23CONbits.TXREQ3 = 1;
    C1TR45CONbits.TXREQ4 = 1;
    C1TR45CONbits.TXREQ5 = 1;

/* Write Message to ECAN2 Message Buffer 0 to 5 
   Request Message Transmission			*/
    ecan2WriteMessage();
    C2TR01CONbits.TXREQ0 = 1;
    C2TR01CONbits.TXREQ1 = 1;
    C2TR23CONbits.TXREQ2 = 1;
    C2TR23CONbits.TXREQ3 = 1;
    C2TR45CONbits.TXREQ4 = 1;
    C2TR45CONbits.TXREQ5 = 1;

/* Loop infinitely */

    while (1) ;

}

/* ECAN1 buffer loaded with Identifiers and Data */
void ecan1WriteMessage(void)
{

/* Writing the message for Transmission

ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

    ecan1WriteTxMsgBufId(0, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(0, 8, 0x1111, 0x1111, 0x1111, 0x1111);

    ecan1WriteTxMsgBufId(1, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(1, 8, 0x2222, 0x2222, 0x2222, 0x2222);

    ecan1WriteTxMsgBufId(2, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(2, 8, 0x3333, 0x3333, 0x3333, 0x3333);

    ecan1WriteTxMsgBufId(3, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(3, 8, 0x4444, 0x4444, 0x4444, 0x4444);

    ecan1WriteTxMsgBufId(4, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(4, 8, 0x5555, 0x5555, 0x5555, 0x5555);

    ecan1WriteTxMsgBufId(5, 0x1FFEFFFF, 1, 0);
    ecan1WriteTxMsgBufData(5, 8, 0x6666, 0x6666, 0x6666, 0x6666);

}

/* ECAN2 buffer loaded with Identifiers and Data */

void ecan2WriteMessage(void)
{

/* Writing the message for Transmission

ecan2WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan2WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer Number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

    ecan2WriteTxMsgBufId(0, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(0, 8, 0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA);

    ecan2WriteTxMsgBufId(1, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(1, 8, 0xBBBB, 0xBBBB, 0xBBBB, 0xBBBB);

    ecan2WriteTxMsgBufId(2, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(2, 8, 0xCCCC, 0xCCCC, 0xCCCC, 0xCCCC);

    ecan2WriteTxMsgBufId(3, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(3, 8, 0xDDDD, 0xDDDD, 0xDDDD, 0xDDDD);

    ecan2WriteTxMsgBufId(4, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(4, 8, 0xEEEE, 0xEEEE, 0xEEEE, 0xEEEE);

    ecan2WriteTxMsgBufId(5, 0x1FFEFFFF, 1, 0);
    ecan2WriteTxMsgBufData(5, 8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF);

}

void clearIntrflags(void)
{
/* Clear Interrupt Flags */

    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;
}

void oscConfig(void)
{

/*  Configure Oscillator to operate the device at 40Mhz
 	Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
 	Fosc= 8M*40/(2*2)=80Mhz for 8M input clock */

    PLLFBD = 38;		/* M=40 */
    CLKDIVbits.PLLPOST = 0;	/* N1=2 */
    CLKDIVbits.PLLPRE = 0;	/* N2=2 */
    OSCTUN = 0;			/* Tune FRC oscillator, if FRC is used */

/* Disable Watch Dog Timer */

    RCONbits.SWDTEN = 0;

/* Clock switch to incorporate PLL*/
    __builtin_write_OSCCONH(0x03);	// Initiate Clock Switch to Primary
    // Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONL(0x01);	// Start clock switching
    while (OSCCONbits.COSC != 0 b011) ;	// Wait for Clock switch to occur       

/* Wait for PLL to lock */

    while (OSCCONbits.LOCK != 1) {
    };
}

void __attribute__ ((interrupt, no_auto_psv)) _C1Interrupt(void)
{
    IFS2bits.C1IF = 0;		// clear interrupt flag
    if (C1INTFbits.TBIF) {
	C1INTFbits.TBIF = 0;
    }

    if (C1INTFbits.RBIF) {
	C1INTFbits.RBIF = 0;
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _C2Interrupt(void)
{
    IFS3bits.C2IF = 0;		// clear interrupt flag
    if (C2INTFbits.TBIF) {
	C2INTFbits.TBIF = 0;
    }

    if (C2INTFbits.RBIF) {
	C2INTFbits.RBIF = 0;
    }
}

//------------------------------------------------------------------------------
//    DMA interrupt handlers
//------------------------------------------------------------------------------

void __attribute__ ((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0;	// Clear the DMA0 Interrupt Flag;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
    IFS0bits.DMA1IF = 0;	// Clear the DMA1 Interrupt Flag;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
    IFS1bits.DMA2IF = 0;	// Clear the DMA2 Interrupt Flag;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
    IFS2bits.DMA3IF = 0;	// Clear the DMA3 Interrupt Flag;
}
