/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
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
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF 
#pragma config PWRT     = ON
#pragma config BOR      = ON
#pragma config BORV     = 2
#pragma config VREGEN   = ON      //USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
#pragma config ICPRT    = ON       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required
#include <delays.h>

#include "system\usb\usb_compile_time_validation.h" // Optional
#include "user\BusMon.h"                        // Modifiable


/** V A R I A B L E S ********************************************************/
#pragma udata
unsigned int i;
unsigned char isUsbPowered;
unsigned char newPortA, lastPortA, debCounter;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void USBTasks(void);

/** V E C T O R  R E M A P P I N G *******************************************/

/*
extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x002000
void _reset (void)
{
    _asm goto _startup _endasm
}
 */

//#pragma code _HIGH_INTERRUPT_VECTOR = 0x002008
//void _high_ISR (void)
//{
//    ;
//}
//
//#pragma code LOW_INTERRUPT_VECTOR = 0x002018
//void _low_ISR (void)
//{
//  _asm
//    goto InterruptHandler
//  _endasm
//}

#pragma code                                                                                                                

/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *****************************************************************************/
void main(void) {
    ADCON1 |= 0x0F; // Default all pins to digital

    isUsbPowered = 0;

    UserInit(); // See BusMon.c & .h

    // Debounce Port A:
    newPortA = 0;
    lastPortA = 0;
    debCounter = 0;

    for (i = 0; i < 100; i++) {
        newPortA = PORTA & mask_power_sense;
        if (newPortA == lastPortA) debCounter++;
        if (debCounter > 10) break;
        lastPortA = newPortA;
        Delay10KTCYx(50);
        LED_TX_ON();
        Delay10KTCYx(50);
        LED_TX_OFF();
    }

    if (lastPortA & mask_power_sense) {
        isUsbPowered = 1;
        LED_TX_ON();
    }

    InitializeSystem();

    while (1) {
        if (isUsbPowered) //Only do USB USB if not powered by CAN connector
            USBTasks(); // USB Tasks
        //  INTCONbits.GIE = 1;
        ProcessIO(); // See user\BusMon.c & .h
    }
}//end main

/******************************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization routine.
 *                  All required USB initialization routines are called from
 *                  here.
 *
 *                  User application initialization routine should also be
 *                  called from here.                  
 *
 * Note:            None
 *****************************************************************************/
static void InitializeSystem(void) {

#if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See io_cfg.h
#endif

#if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;
#endif

    if (isUsbPowered) { //Only initialize USB if not powered by CAN connector
        mInitializeUSBDriver(); // See usbdrv.h
    }
}//end InitializeSystem

/******************************************************************************
 * Function:        void USBTasks(void)
 *
 * PreCondition:    InitializeSystem has been called.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Service loop for USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void USBTasks(void) {
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus(); // Must use polling method
    if (UCFGbits.UTEYE != 1)
        USBDriverService(); // Interrupt or polling method

}// end USBTasks


////*****************************************************************************
//// High priority interrupt vector
//#pragma code InterruptVectorHigh = 0x08
//void InterruptVectorHigh(void)
//{
//_asm
//bra InterruptHandler // jump to interrupt routine
//_endasm
//}
//#pragma code InterruptVectorHigh = 0x08
//void InterruptVectorLow(void)
//{
//_asm
//bra InterruptHandler // jump to interrupt routine
//_endasm
//}
//
//
//#pragma code
//#pragma interrupt InterruptHandler
//void InterruptHandler(void)
//{
//}

/** EOF main.c ***************************************************************/
