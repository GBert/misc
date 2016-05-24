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

void init_can(void) {
    /* set config register visible in SFRs */
    C1CTRL1bits.WIN = 0;
    /* wait getting into config mode */
    C1CTRL1bits.REQOP = CAN_OPMODE_CONFIG;
    while (C1CTRL1bits.OPMODE != CAN_OPMODE_CONFIG) ;

    C1CTRL1bits.CANCKS = 0;
    C1CTRL1bits.CSIDL = 0;
    C1CTRL1bits.CANCAP = 0;

    /* can-calc-bittiming */
    C1CFG1 = CB_C1CFG1;
    C1CFG2 = CB_C1CFG2;

    // C1FCTRL = 0xC008;
    C1FCTRLbits.DMABS = 4;	/* 16 CAN Message Buffers in DMA RAM */
    C1FCTRLbits.FSA = 8;	/* FIFO Starts at Message Buffer 8 */

    /* DMA Config Channel 0 for TX IRQ = 70 */
    DMA0CON = 0x2020;
    DMA0CNT = 7;
    DMA0PAD = (volatile unsigned int)&C1TXD;
    DMA0REQ = 0x0046;		/* C1TX - ECAN1 Transmit Data Request */
    DMA0STAH = 0;
    DMA0STAL = (unsigned int)&m_Can_ECanTXRXMsgBuf;
    DMA0CONbits.CHEN = 1;

    /* DMA Config Channel 2 for RX IRQ = 34 */
    DMA2CON = 0x0020;
    DMA2CNT = 7;
    DMA2PAD = (volatile unsigned int)&C1RXD;
    DMA2REQ = 0x0022;		/* C1TX - ECAN1 Receive Data Request */
    DMA2STAH = 0;
    DMA2STAL = (unsigned int)&m_Can_ECanTXRXMsgBuf;
    DMA2CONbits.CHEN = 1;

    //IEC0bits.DMA1IE = 1;

    /* Set SFR Window to Buffer register in RAM */
    C1CTRL1bits.WIN = 1;

    /* Select Acceptance Filter Mask 0 for Acceptance Filter 0 */
    C1FMSKSEL1bits.F0MSK = 0x0;
    /* Configure Acceptance Filter Mask 0 register to mask SID<2:0>
       Mask Bits (11-bits) : 0b000 0000 0111 */
    C1RXM0SIDbits.SID = 0x0000;
    /* Configure Acceptance Filter 0 to match standard identifier
       Filter Bits (11-bits): 0b011 1010 xxx */
    C1RXF0SIDbits.SID = 0x0000;
    /* Acceptance Filter 0 to check for Standard Identifier */
    C1RXM0SIDbits.MIDE = 0x0;
    // C1RXF0SIDbits.EXIDE= 0x0;
    /* Acceptance Filter 0 to use Message Buffer 10 to store message */
    C1BUFPNT1bits.F0BP = 0xf;
    /* Filter 0 enabled for Identifier match with incoming message */
    C1FEN1bits.FLTEN0 = 0x1;

    /* delete all filters */
    //C1FEN1 = 0x0;

    /* Set SFR Window to Control register in RAM */
    C1CTRL1bits.WIN = 0;

    /* ECAN transmit/receive message control */

    C1RXFUL1 = C1RXFUL2 = C1RXOVF1 = C1RXOVF2 = 0x0000;
#if 0
    C1TR01CONbits.TXEN0 = 1;	/* ECAN1, Buffer 0 is a Transmit Buffer */
    C1TR01CONbits.TXEN1 = 1;	/* ECAN1, Buffer 1 is a Transmit Buffer */
    C1TR23CONbits.TXEN2 = 1;	/* ECAN1, Buffer 2 is a Transmit Buffer */
    C1TR23CONbits.TXEN3 = 1;	/* ECAN1, Buffer 3 is a Transmit Buffer */
    C1TR45CONbits.TXEN4 = 1;	/* ECAN1, Buffer 4 is a Transmit Buffer */
    C1TR45CONbits.TXEN5 = 1;	/* ECAN1, Buffer 5 is a Transmit Buffer */
    C1TR67CONbits.TXEN6 = 1;	/* ECAN1, Buffer 6 is a Transmit Buffer */
    C1TR67CONbits.TXEN7 = 1;	/* ECAN1, Buffer 7 is a Transmit Buffer */

    C1TR01CONbits.TX0PRI = 0;	/* Message Buffer 0 Priority Level */
    C1TR01CONbits.TX1PRI = 0;	/* Message Buffer 1 Priority Level */
    C1TR23CONbits.TX2PRI = 0;	/* Message Buffer 2 Priority Level */
    C1TR23CONbits.TX3PRI = 0;	/* Message Buffer 3 Priority Level */
    C1TR45CONbits.TX4PRI = 0;	/* Message Buffer 4 Priority Level */
    C1TR45CONbits.TX5PRI = 0;	/* Message Buffer 5 Priority Level */
    C1TR67CONbits.TX6PRI = 0;	/* Message Buffer 6 Priority Level */
    C1TR67CONbits.TX7PRI = 0;	/* Message Buffer 7 Priority Level */
#else
    C1TR01CON = 0x8080;		/* ECAN1, Buffer 0&1 Transmit Buffer - Prio 0 */
    C1TR23CON = 0x8080;		/* ECAN1, Buffer 2&3 Transmit Buffer - Prio 0 */
    C1TR45CON = 0x8080;		/* ECAN1, Buffer 4&5 Transmit Buffer - Prio 0 */
    C1TR67CON = 0x8080;		/* ECAN1, Buffer 6&7 Transmit Buffer - Prio 0 */
#endif

    // start Can
    //C1CTRL1bits.REQOP = CAN_OPMODE_LOOPBACK;
    //while (C1CTRL1bits.OPMODE != CAN_OPMODE_LOOPBACK);
    C1CTRL1bits.REQOP = CAN_OPMODE_NORMAL;
    while (C1CTRL1bits.OPMODE != CAN_OPMODE_NORMAL) ;
}

void can_test_send(void) {
    m_Can_ECanTXRXMsgBuf[0][0] = 0x01D3 << 2;	// IDE, SRR = 0 SID = 1D2
    m_Can_ECanTXRXMsgBuf[0][1] = 0x0000;	// EID = 0
    m_Can_ECanTXRXMsgBuf[0][2] = 0x0008;	// RTR = 0, DLC = Data Length counter = 8
    m_Can_ECanTXRXMsgBuf[0][3] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][4] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][5] = 0xabcd;
    m_Can_ECanTXRXMsgBuf[0][6] = 0xabcd;

    while (C1TR01CONbits.TXREQ0 == 1) ;
    C1TR01CONbits.TXREQ0 = 0x1;
}

int can_test_receive(void) {
    unsigned int fnrb;
    unsigned int fnrbmask;
    fnrb = C1FIFObits.FNRB;

    fnrbmask = 1 << fnrb;
    if (C1RXFUL1 && fnrbmask) {
	C1RXFUL1 &= ~fnrbmask;
	return fnrb;
    }
    return fnrb;
}
