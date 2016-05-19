/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"
#include "can.h"

uint16_t m_Can_ECanTXRXMsgBuf[NUM_OF_ECAN_BUFFERS][8] __attribute__ ((aligned(NUM_OF_ECAN_BUFFERS * 16)));

void init_can(void)
{
    /* wait getting into config mode */
    C1CTRL1bits.REQOP = CAN_OPMODE_CONFIG;
    while (C1CTRL1bits.OPMODE != CAN_OPMODE_CONFIG) ;

    C1CTRL1bits.CANCKS = 0;
    C1CTRL1bits.CSIDL = 0;
    C1CTRL1bits.CANCAP = 0;

  // can-calc-bittiming
    C1CFG1=CB_C1CFG1;
    C1CFG2=CB_C1CFG2;

    C1FCTRL = 0xC01F;

    /* DMA Config Channel 0 for TX IRQ = 70 */
    DMA0CON = 0x2020;
    // DMA0REQ = 70;
    DMA0CNT = 7;
    DMA0PAD = (volatile unsigned int)&C1TXD;
    DMA0REQ = 0x0046;               //C1TX - ECAN1 Transmit Data Request
    DMA0STAH = 0;
    DMA0STAL = (unsigned int)&m_Can_ECanTXRXMsgBuf;
    DMA0CONbits.CHEN = 0x1;

    DMA2CON = 0x0000;
    DMA2REQ = 34;
    DMA2CNT = 7;
    DMA2PAD = (volatile unsigned int)&C1RXD;
    DMA2STAL = (unsigned int)&m_Can_ECanTXRXMsgBuf;
    DMA2STAH = 0;
    DMA2CONbits.CHEN = 1;

    //IEC0bits.DMA1IE = 1;

    /* Set SFR Window to Buffer register in RAM */
    C1CTRL1bits.WIN = 1;

    /* RX Filter */
    /* Select Acceptance Filter Mask 0 for Acceptance Filter 0 */
    C1FMSKSEL1bits.F0MSK = 0x0;
    /* Configure Acceptance Filter Mask 0 register to mask SID<2:0>
     * Mask Bits (11-bits) : 0b111 1111 1000 */
    C1RXM0SIDbits.SID = 0x7F8;
    // C1RXM0SIDbits.SID = 0x000;

    /* Configure Acceptance Filter 0 to match standard identifier
       Filter Bits (11-bits): 0b011 1010 xxx with the mask setting, message with SID
       range 0x1D0-0x1D7 will be accepted by the ECAN module. */
    C1RXF0SIDbits.SID = 0x01D0;
    /* Acceptance Filter 0 to check for Standard Identifier */
    C1RXM0SIDbits.MIDE = 0x1;
    //    C1RXM0SIDbits.MIDE = 0x0;
    C1RXF0SIDbits.EXIDE = 0x0;
    /* Acceptance Filter 0 to use Message Buffer 10 to store message */
    C1BUFPNT1bits.F0BP = 0xA;
    /* Filter 0 enabled for Identifier match with incoming message */
    C1FEN1 = 0x0;
    C1FEN1bits.FLTEN0 = 0x1;

    /* Set SFR Window to Control register in RAM */
    C1CTRL1bits.WIN = 0;

    // Configure Message Buffer 0 for Transmission and assign priority
    C1TR01CONbits.TXEN0 = 0x1;
    C1TR01CONbits.TX0PRI = 0x3;

    //    C1TR01CONbits.TXEN1 = 0x0;
    //    C1TR01CONbits.TX1PRI = 0x3;

    // start Can
    C1CTRL1bits.REQOP = CAN_OPMODE_NORMAL;
    while (C1CTRL1bits.OPMODE != CAN_OPMODE_NORMAL);
}

void can_test_send(void) {

    m_Can_ECanTXRXMsgBuf[0][0] = 0x01D3 << 2;   // IDE, SRR = 0 SID = 1D2
    m_Can_ECanTXRXMsgBuf[0][1] = 0x0000;        // EID = 0
    m_Can_ECanTXRXMsgBuf[0][2] = 0x0008;        // RTR = 0, DLC = Data Length counter = 8 = 8 Wör sollen üagen werden
    m_Can_ECanTXRXMsgBuf[0][3] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][4] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][5] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][6] = 0xabcd;

    // Empfangspuffer leer setzen
    C1RXFUL1 = 0;

    C1TR01CONbits.TXREQ0 = 0x1;
    while(C1TR01CONbits.TXREQ0 == 1);

}

bool can_test_receive(void) {
    if (C1RXFUL1bits.RXFUL10)
	return 1;
    return 0;
}
