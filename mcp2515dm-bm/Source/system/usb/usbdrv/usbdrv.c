/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbdrv.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
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

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "io_cfg.h"             // Required for USBCheckBusStatus()

/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void USBModuleEnable(void);
void USBModuleDisable(void);

void USBSuspend(void);
void USBWakeFromSuspend(void);

void USBProtocolResetHandler(void);
void USB_SOF_Handler(void);
void USBStallHandler(void);
void USBErrorHandler(void);

/** D E C L A R A T I O N S **************************************************/
#pragma code
/******************************************************************************
 * Function:        void USBCheckBusStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine enables/disables the USB module by monitoring
 *                  the USB power signal.
 *
 * Note:            None
 *****************************************************************************/
void USBCheckBusStatus(void)
{
    /**************************************************************************
     * Bus Attachment & Detachment Detection
     * usb_bus_sense is an i/o pin defined in io_cfg.h
     *************************************************************************/
    #define USB_BUS_ATTACHED    1
    #define USB_BUS_DETACHED    0

    if(usb_bus_sense == USB_BUS_ATTACHED)       // Is USB bus attached?
    {
        if(UCONbits.USBEN == 0)                 // Is the module off?
            USBModuleEnable();                  // Is off, enable it
    }
    else
    {
        if(UCONbits.USBEN == 1)                 // Is the module on?
            USBModuleDisable();                 // Is on, disable it
    }//end if(usb_bus_sense...)
    
    /*
     * After enabling the USB module, it takes some time for the voltage
     * on the D+ or D- line to rise high enough to get out of the SE0 condition.
     * The USB Reset interrupt should not be unmasked until the SE0 condition is
     * cleared. This helps preventing the firmware from misinterpreting this
     * unique event as a USB bus reset from the USB host.
     */
    if(usb_device_state == ATTACHED_STATE)
    {
        if(!UCONbits.SE0)
        {
            UIR = 0;                        // Clear all USB interrupts
            UIE = 0;                        // Mask all USB interrupts
            UIEbits.URSTIE = 1;             // Unmask RESET interrupt
            UIEbits.IDLEIE = 1;             // Unmask IDLE interrupt
            usb_device_state = POWERED_STATE;
        }//end if                           // else wait until SE0 is cleared
        
    }//end if(usb_device_state == ATTACHED_STATE)

}//end USBCheckBusStatus

/******************************************************************************
 * Function:        void USBModuleEnable(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine enables the USB module.
 *                  An end designer should never have to call this routine
 *                  manually. This routine should only be called from
 *                  USBCheckBusStatus().
 *
 * Note:            See USBCheckBusStatus() for more information.
 *****************************************************************************/
void USBModuleEnable(void)
{
    UCON = 0;
    UIE = 0;                                // Mask all USB interrupts
    UCONbits.USBEN = 1;                     // Enable module & attach to bus
    usb_device_state = ATTACHED_STATE;      // Defined in usbmmap.c & .h
}//end USBModuleEnable

/******************************************************************************
 * Function:        void USBModuleDisable(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine disables the USB module.
 *                  An end designer should never have to call this routine
 *                  manually. This routine should only be called from
 *                  USBCheckBusStatus().
 *
 * Note:            See USBCheckBusStatus() for more information.
 *****************************************************************************/
void USBModuleDisable(void)
{
    UCON = 0;                               // Disable module & detach from bus
    UIE = 0;                                // Mask all USB interrupts
    usb_device_state = DETACHED_STATE;      // Defined in usbmmap.c & .h
}//end USBModuleDisable

/******************************************************************************
 * Function:        void USBSoftDetach(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    The device will have to be re-enumerated to function again.
 *
 * Overview:        USBSoftDetach electrically disconnects the device from
 *                  the bus. This is done by stop supplying Vusb voltage to
 *                  pull-up resistor. The pull-down resistors on the host
 *                  side will pull both differential signal lines low and
 *                  the host registers the event as a disconnect.
 *
 *                  Since the USB cable is not physically disconnected, the
 *                  power supply through the cable can still be sensed by
 *                  the device. The next time USBCheckBusStatus() function
 *                  is called, it will reconnect the device back to the bus.
 *
 * Note:            None
 *****************************************************************************/
void USBSoftDetach(void)
{
    USBModuleDisable();
}//end USBSoftDetach

/******************************************************************************
 * Function:        void USBDriverService(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is the heart of this firmware. It manages
 *                  all USB interrupts.
 *
 * Note:            Device state transitions through the following stages:
 *                  DETACHED -> ATTACHED -> POWERED -> DEFAULT ->
 *                  ADDRESS_PENDING -> ADDRESSED -> CONFIGURED -> READY
 *****************************************************************************/
void USBDriverService(void)
{   
    /*
     * Pointless to continue servicing if USB cable is not even attached.
     */
    if(usb_device_state == DETACHED_STATE) return;
    
    /*
     * Task A: Service USB Activity Interrupt
     */

    if(UIRbits.ACTVIF && UIEbits.ACTVIE)    USBWakeFromSuspend();

    /*
     * Pointless to continue servicing if the device is in suspend mode.
     */    
    if(UCONbits.SUSPND==1) return;
            
    /*
     * Task B: Service USB Bus Reset Interrupt.
     * When bus reset is received during suspend, ACTVIF will be set first,
     * once the UCONbits.SUSPND is clear, then the URSTIF bit will be asserted.
     * This is why URSTIF is checked after ACTVIF.
     *
     * The USB reset flag is masked when the USB state is in
     * DETACHED_STATE or ATTACHED_STATE, and therefore cannot
     * cause a USB reset event during these two states.
     */
    if(UIRbits.URSTIF && UIEbits.URSTIE)    USBProtocolResetHandler();
    
    /*
     * Task C: Service other USB interrupts
     */
    if(UIRbits.IDLEIF && UIEbits.IDLEIE)    USBSuspend();
    if(UIRbits.SOFIF && UIEbits.SOFIE)      USB_SOF_Handler();
    if(UIRbits.STALLIF && UIEbits.STALLIE)  USBStallHandler();
    if(UIRbits.UERRIF && UIEbits.UERRIE)    USBErrorHandler();

    /*
     * Pointless to continue servicing if the host has not sent a bus reset.
     * Once bus reset is received, the device transitions into the DEFAULT
     * state and is ready for communication.
     */
    if(usb_device_state < DEFAULT_STATE) return;

    /*
     * Task D: Servicing USB Transaction Complete Interrupt
     */
    if(UIRbits.TRNIF && UIEbits.TRNIE)
    {
        /*
         * USBCtrlEPService only services transactions over EP0.
         * It ignores all other EP transactions.
         */
        USBCtrlEPService();
        
        /*
         * Other EP can be serviced later by responsible device class firmware.
         * Each device driver knows when an OUT or IN transaction is ready by
         * checking the buffer ownership bit.
         * An OUT EP should always be owned by SIE until the data is ready.
         * An IN EP should always be owned by CPU until the data is ready.
         *
         * Because of this logic, it is not necessary to save the USTAT value
         * of non-EP0 transactions.
         */
        UIRbits.TRNIF = 0;
    }//end if(UIRbits.TRNIF && UIEbits.TRNIE)
    
}//end USBDriverService

/******************************************************************************
 * Function:        void USBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
void USBSuspend(void)
{
    /*
     * NOTE: Do not clear UIRbits.ACTVIF here!
     * Reason:
     * ACTVIF is only generated once an IDLEIF has been generated.
     * This is a 1:1 ratio interrupt generation.
     * For every IDLEIF, there will be only one ACTVIF regardless of
     * the number of subsequent bus transitions.
     *
     * If the ACTIF is cleared here, a problem could occur when:
     * [       IDLE       ][bus activity ->
     * <--- 3 ms ----->     ^
     *                ^     ACTVIF=1
     *                IDLEIF=1
     *  #           #           #           #   (#=Program polling flags)
     *                          ^
     *                          This polling loop will see both
     *                          IDLEIF=1 and ACTVIF=1.
     *                          However, the program services IDLEIF first
     *                          because ACTIVIE=0.
     *                          If this routine clears the only ACTIVIF,
     *                          then it can never get out of the suspend
     *                          mode.             
     */
    UIEbits.ACTVIE = 1;                     // Enable bus activity interrupt
    UIRbits.IDLEIF = 0;
    UCONbits.SUSPND = 1;                    // Put USB module in power conserve
                                            // mode, SIE clock inactive
    /*
     * At this point the PIC can go into sleep,idle, or
     * switch to a slower clock, etc.
     */
    
    /* Modifiable Section */
    PIR2bits.USBIF = 0;
    INTCONbits.RBIF = 0;
    PIE2bits.USBIE = 1;                     // Set USB wakeup source
    INTCONbits.RBIE = 1;                    // Set sw2,3 wakeup source
    Sleep();                                // Goto sleep
    
    if(INTCONbits.RBIF == 1)                // Check if external stimulus
    {
        USBRemoteWakeup();                  // If yes, attempt RWU
    }
    PIE2bits.USBIE = 0;
    INTCONbits.RBIE = 0;
    /* End Modifiable Section */

}//end USBSuspend

/******************************************************************************
 * Function:        void USBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            None
 *****************************************************************************/
void USBWakeFromSuspend(void)
{
    /* 
     * If using clock switching, this is the place to restore the
     * original clock frequency.
     */
    UCONbits.SUSPND = 0;
    UIEbits.ACTVIE = 0;
    UIRbits.ACTVIF = 0;
}//end USBWakeFromSuspend

/******************************************************************************
 * Function:        void USBRemoteWakeup(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function should be called by user when the device
 *                  is waken up by an external stimulus other than ACTIVIF.
 *                  Please read the note below to understand the limitations.
 *
 * Note:            The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *****************************************************************************/
void USBRemoteWakeup(void)
{
    static word delay_count;
    
    if(usb_stat.RemoteWakeup == 1)          // Check if RemoteWakeup function
    {                                       // has been enabled by the host.
        USBWakeFromSuspend();               // Unsuspend USB modue
        UCONbits.RESUME = 1;                // Start RESUME signaling

        /* Modifiable Section */
        
        delay_count = 1800U;                // Set RESUME line for 1-13 ms
        do
        {
            delay_count--;
        }while(delay_count);        
        
        /* End Modifiable Section */
        
        UCONbits.RESUME = 0;
    }//endif 
}//end USBRemoteWakeup

/******************************************************************************
 * Function:        void USB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed devices
 *                  every 1 ms. This interrupt may be useful for isochronous
 *                  pipes. End designers should implement callback routine
 *                  as necessary.
 *
 * Note:            None
 *****************************************************************************/
void USB_SOF_Handler(void)
{
    /* Callback routine here */
    
    UIRbits.SOFIF = 0;
}//end USB_SOF_Handler

/******************************************************************************
 * Function:        void USBStallHandler(void)
 *
 * PreCondition:    A STALL packet is sent to the host by the SIE.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The STALLIF is set anytime the SIE sends out a STALL
 *                  packet regardless of which endpoint causes it.
 *                  A Setup transaction overrides the STALL function. A stalled
 *                  endpoint stops stalling once it receives a setup packet.
 *                  In this case, the SIE will accepts the Setup packet and
 *                  set the TRNIF flag to notify the firmware. STALL function
 *                  for that particular endpoint pipe will be automatically
 *                  disabled (direction specific).
 *
 *                  There are a few reasons for an endpoint to be stalled.
 *                  1. When a non-supported USB request is received.
 *                     Example: GET_DESCRIPTOR(DEVICE_QUALIFIER)
 *                  2. When an endpoint is currently halted.
 *                  3. When the device class specifies that an endpoint must
 *                     stall in response to a specific event.
 *                     Example: Mass Storage Device Class
 *                              If the CBW is not valid, the device shall
 *                              STALL the Bulk-In pipe.
 *                              See USB Mass Storage Class Bulk-only Transport
 *                              Specification for more details.
 *
 * Note:            UEPn.EPSTALL can be scanned to see which endpoint causes
 *                  the stall event.
 *                  If
 *****************************************************************************/
void USBStallHandler(void)
{
    /*
     * Does not really have to do anything here,
     * even for the control endpoint.
     * All BDs of Endpoint 0 are owned by SIE right now,
     * but once a Setup Transaction is received, the ownership
     * for EP0_OUT will be returned to CPU.
     * When the Setup Transaction is serviced, the ownership
     * for EP0_IN will then be forced back to CPU by firmware.
     */
    if(UEP0bits.EPSTALL == 1)
    {
        USBPrepareForNextSetupTrf();        // Firmware work-around
        UEP0bits.EPSTALL = 0;
    }
    UIRbits.STALLIF = 0;
}//end USBStallHandler

/******************************************************************************
 * Function:        void USBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this interrupt is mainly for debugging
 *                  during development. Check UEIR to see which error causes
 *                  the interrupt.
 *
 * Note:            None
 *****************************************************************************/
void USBErrorHandler(void)
{
    UIRbits.UERRIF = 0;
}//end USBErrorHandler

/******************************************************************************
 * Function:        void USBProtocolResetHandler(void)
 *
 * PreCondition:    A USB bus reset is received from the host.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Currently, this routine flushes any pending USB
 *                  transactions. It empties out the USTAT FIFO. This action
 *                  might not be desirable in some applications.
 *
 * Overview:        Once a USB bus reset is received from the host, this
 *                  routine should be called. It resets the device address to
 *                  zero, disables all non-EP0 endpoints, initializes EP0 to
 *                  be ready for default communication, clears all USB
 *                  interrupt flags, unmasks applicable USB interrupts, and
 *                  reinitializes internal state-machine variables.
 *
 * Note:            None
 *****************************************************************************/
void USBProtocolResetHandler(void)
{
    UEIR = 0;                       // Clear all USB error flags
    UIR = 0;                        // Clears all USB interrupts
    UEIE = 0b10011111;              // Unmask all USB error interrupts
    UIE = 0b01111011;               // Enable all interrupts except ACTVIE
    
    UADDR = 0x00;                   // Reset to default address
    mDisableEP1to15();              // Reset all non-EP0 UEPn registers
    UEP0 = EP_CTRL|HSHK_EN;         // Init EP0 as a Ctrl EP, see usbdrv.h

    while(UIRbits.TRNIF == 1)       // Flush any pending transactions
        UIRbits.TRNIF = 0;

    UCONbits.PKTDIS = 0;            // Make sure packet processing is enabled
    USBPrepareForNextSetupTrf();    // Declared in usbctrltrf.c
    
    usb_stat.RemoteWakeup = 0;      // Default status flag to disable
    usb_active_cfg = 0;             // Clear active configuration
    usb_device_state = DEFAULT_STATE;
}//end USBProtocolResetHandler


/* Auxiliary Function */
void ClearArray(byte* startAdr,byte count)
{
    *startAdr;
    while(count)
    {
        _asm
        clrf POSTINC0,0
        _endasm
        count--;
    }//end while
}//end ClearArray

/** EOF usbdrv.c *************************************************************/
