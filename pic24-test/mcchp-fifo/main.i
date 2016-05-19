# 1 "main.c"
# 1 "/home/eh2be5j/projekte/github-misc/pic24-test/mcchp-fifo//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
# 62 "main.c"
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);




_FWDT(FWDTEN_OFF);



ECAN1MSGBUF ecan1msgBuf __attribute__ ((space(dma), aligned(ECAN1_MSG_BUF_LENGTH * 16)));
ECAN2MSGBUF ecan2msgBuf __attribute__ ((space(dma), aligned(ECAN2_MSG_BUF_LENGTH * 16)));


void oscConfig(void);
void clearIntrflags(void);
void ecan1WriteMessage(void);
void ecan2WriteMessage(void);

int main(void)
{


    oscConfig();


    clearIntrflags();




    ecan1Init();
    dma0init();
    dma2init();



    IEC2bits.C1IE = 1;
    C1INTEbits.TBIE = 1;
    C1INTEbits.RBIE = 1;




    ecan2Init();
    dma1init();
    dma3init();


    IEC3bits.C2IE = 1;
    C2INTEbits.TBIE = 1;
    C2INTEbits.RBIE = 1;



    ecan1WriteMessage();
    C1TR01CONbits.TXREQ0 = 1;
    C1TR01CONbits.TXREQ1 = 1;
    C1TR23CONbits.TXREQ2 = 1;
    C1TR23CONbits.TXREQ3 = 1;
    C1TR45CONbits.TXREQ4 = 1;
    C1TR45CONbits.TXREQ5 = 1;



    ecan2WriteMessage();
    C2TR01CONbits.TXREQ0 = 1;
    C2TR01CONbits.TXREQ1 = 1;
    C2TR23CONbits.TXREQ2 = 1;
    C2TR23CONbits.TXREQ3 = 1;
    C2TR45CONbits.TXREQ4 = 1;
    C2TR45CONbits.TXREQ5 = 1;



    while (1) ;

}


void ecan1WriteMessage(void)
{
# 167 "main.c"
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



void ecan2WriteMessage(void)
{
# 214 "main.c"
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


    IFS0 = 0;
    IFS1 = 0;
    IFS2 = 0;
    IFS3 = 0;
    IFS4 = 0;
}

void oscConfig(void)
{





    PLLFBD = 38;
    CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 0;
    OSCTUN = 0;



    RCONbits.SWDTEN = 0;


    __builtin_write_OSCCONH(0x03);

    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0 b011) ;



    while (OSCCONbits.LOCK != 1) {
    };
}

void __attribute__ ((interrupt, no_auto_psv)) _C1Interrupt(void)
{
    IFS2bits.C1IF = 0;
    if (C1INTFbits.TBIF) {
 C1INTFbits.TBIF = 0;
    }

    if (C1INTFbits.RBIF) {
 C1INTFbits.RBIF = 0;
    }
}

void __attribute__ ((interrupt, no_auto_psv)) _C2Interrupt(void)
{
    IFS3bits.C2IF = 0;
    if (C2INTFbits.TBIF) {
 C2INTFbits.TBIF = 0;
    }

    if (C2INTFbits.RBIF) {
 C2INTFbits.RBIF = 0;
    }
}





void __attribute__ ((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA1Interrupt(void)
{
    IFS0bits.DMA1IF = 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
    IFS1bits.DMA2IF = 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _DMA3Interrupt(void)
{
    IFS2bits.DMA3IF = 0;
}
