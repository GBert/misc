/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbdsc.c
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
 ********************************************************************/

/*********************************************************************
 * -usbdsc.c-
 * This file contains the USB descriptor information. It is used
 * in conjunction with the usbdsc.h file. When a descriptor is added
 * or removed from the main configuration descriptor, i.e. CFG01,
 * the user must also change the descriptor structure defined in
 * the usbdsc.h file. The structure is used to calculate the 
 * descriptor size, i.e. sizeof(CFG01).
 * 
 * A typical configuration descriptor consists of:
 * At least one configuration descriptor (USB_CFG_DSC)
 * One or more interface descriptors (USB_INTF_DSC)
 * One or more endpoint descriptors (USB_EP_DSC)
 *
 * Naming Convention:
 * To resolve ambiguity, the naming convention are as followed:
 * - USB_CFG_DSC type should be named cdxx, where xx is the
 *   configuration number. This number should match the actual
 *   index value of this configuration.
 * - USB_INTF_DSC type should be named i<yy>a<zz>, where yy is the
 *   interface number and zz is the alternate interface number.
 * - USB_EP_DSC type should be named ep<##><d>_i<yy>a<zz>, where
 *   ## is the endpoint number and d is the direction of transfer.
 *   The interface name should also be listed as a suffix to identify
 *   which interface does the endpoint belong to.
 *
 * Example:
 * If a device has one configuration, two interfaces; interface 0
 * has two endpoints (in and out), and interface 1 has one endpoint(in).
 * Then the CFG01 structure in the usbdsc.h should be:
 *
 * #define CFG01 rom struct                            \
 * {   USB_CFG_DSC             cd01;                   \
 *     USB_INTF_DSC            i00a00;                 \
 *     USB_EP_DSC              ep01o_i00a00;           \
 *     USB_EP_DSC              ep01i_i00a00;           \
 *     USB_INTF_DSC            i01a00;                 \
 *     USB_EP_DSC              ep02i_i01a00;           \
 * } cfg01
 * 
 * Note the hierarchy of the descriptors above, it follows the USB
 * specification requirement. All endpoints belonging to an interface
 * should be listed immediately after that interface.
 *
 * -------------------------------------------------------------------
 * Filling in the descriptor values in the usbdsc.c file:
 * -------------------------------------------------------------------
 * Most items should be self-explanatory, however, a few will be
 * explained for clarification.
 *
 * [Configuration Descriptor(USB_CFG_DSC)]
 * The configuration attribute must always have the _DEFAULT
 * definition at the minimum. Additional options can be ORed
 * to the _DEFAULT attribute. Available options are _SELF and _RWU.
 * These definitions are defined in the usbdefs_std_dsc.h file. The
 * _SELF tells the USB host that this device is self-powered. The
 * _RWU tells the USB host that this device supports Remote Wakeup.
 *
 * [Endpoint Descriptor(USB_EP_DSC)]
 * Assume the following example:
 * sizeof(USB_EP_DSC),DSC_EP,_EP01_OUT,_BULK,64,0x00
 *
 * The first two parameters are self-explanatory. They specify the
 * length of this endpoint descriptor (7) and the descriptor type.
 * The next parameter identifies the endpoint, the definitions are
 * defined in usbdefs_std_dsc.h and has the following naming
 * convention:
 * _EP<##>_<dir>
 * where ## is the endpoint number and dir is the direction of
 * transfer. The dir has the value of either 'OUT' or 'IN'.
 * The next parameter identifies the type of the endpoint. Available
 * options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
 * typically used because the default control transfer endpoint is
 * not defined in the USB descriptors. When _ISO option is used,
 * addition options can be ORed to _ISO. Example:
 * _ISO|_AD|_FE
 * This describes the endpoint as an isochronous pipe with adaptive
 * and feedback attributes. See usbdefs_std_dsc.h and the USB
 * specification for details. The next parameter defines the size of
 * the endpoint. The last parameter in the polling interval.
 *
 * -------------------------------------------------------------------
 * Adding a USB String
 * -------------------------------------------------------------------
 * A string descriptor array should have the following format:
 *
 * rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
 * sizeof(sdxxx),DSC_STR,<text>};
 *
 * The above structure provides a means for the C compiler to
 * calculate the length of string descriptor sdxxx, where xxx is the
 * index number. The first two bytes of the descriptor are descriptor
 * length and type. The rest <text> are string texts which must be
 * in the unicode format. The unicode format is achieved by declaring
 * each character as a word type. The whole text string is declared
 * as a word array with the number of characters equals to <size>.
 * <size> has to be manually counted and entered into the array
 * declaration. Let's study this through an example:
 * if the string is "USB" , then the string descriptor should be:
 * (Using index 02)
 * rom struct{byte bLength;byte bDscType;word string[3];}sd002={
 * sizeof(sd002),DSC_STR,'U','S','B'};
 *
 * A USB project may have multiple strings and the firmware supports
 * the management of multiple strings through a look-up table.
 * The look-up table is defined as:
 * rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};
 *
 * The above declaration has 3 strings, sd000, sd001, and sd002.
 * Strings can be removed or added. sd000 is a specialized string
 * descriptor. It defines the language code, usually this is
 * US English (0x0409). The index of the string must match the index
 * position of the USB_SD_Ptr array, &sd000 must be in position
 * USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
 * The look-up table USB_SD_Ptr is used by the get string handler
 * function in usb9.c.
 *
 * -------------------------------------------------------------------
 *
 * The look-up table scheme also applies to the configuration
 * descriptor. A USB device may have multiple configuration
 * descriptors, i.e. CFG01, CFG02, etc. To add a configuration
 * descriptor, user must implement a structure similar to CFG01.
 * The next step is to add the configuration descriptor name, i.e.
 * cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
 * is a dummy place holder since configuration 0 is the un-configured
 * state according to the definition in the USB specification.
 *
 ********************************************************************/
 
/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * system\usb\usbdefs\usbdefs_std_dsc.h
 *
 * Configuration information is defined in:
 * autofiles\usbcfg.h
 ********************************************************************/
 
/** I N C L U D E S *************************************************/
#include "system\typedefs.h"
#include "system\usb\usb.h"

/** C O N S T A N T S ************************************************/
#pragma romdata

/* Device Descriptor */
rom USB_DEV_DSC device_dsc=
{    
    sizeof(USB_DEV_DSC),    // Size of this descriptor in bytes
    DSC_DEV,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    EP0_BUFF_SIZE,          // Max packet size for EP0, see usbcfg.h
    0x04D8,                 // Vendor ID
    0x0070,                 // Product ID: MCP2515DM-BM Board
    0x0000,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
CFG01={
    /* Configuration Descriptor */
    sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    DSC_CFG,                // CONFIGURATION descriptor type
    sizeof(cfg01),          // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    3,                      // Configuration string index
    _DEFAULT,               // Attributes, see usbdefs_std_dsc.h
    250,                     // Max power consumption (2X mA)

    /* Interface Descriptor */
    sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    DSC_INTF,               // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    HID_INTF,               // Class code
    0,                      // Subclass code
    HID_PROTOCOL_NONE,      // Protocol code
    0,                      // Interface string index
    
    /* HID Class-Specific Descriptor */
    sizeof(USB_HID_DSC),    // Size of this descriptor in bytes
    DSC_HID,                // HID descriptor type
    0x0001,                 // HID Spec Release Number in BCD format
    0x00,                   // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,         // Number of class descriptors, see usbcfg.h
    DSC_RPT,                // Report descriptor type
    sizeof(hid_rpt01),      // Size of the report descriptor

    /* Endpoint Descriptor 1 IN */
    sizeof(USB_EP_DSC),DSC_EP,_EP01_IN,_INT,HID_INT_IN_EP_SIZE,0x1,
    /* Endpoint Descriptor 1 OUT */
    sizeof(USB_EP_DSC),DSC_EP,_EP01_OUT,_INT,HID_INT_OUT_EP_SIZE,0x10,
    //added, PKR
};
    
rom struct{byte bLength;byte bDscType;word string[1];}sd000={
sizeof(sd000),DSC_STR,0x0409};

rom struct{byte bLength;byte bDscType;word string[25];}sd001={
sizeof(sd001),DSC_STR,
'M','i','c','r','o','c','h','i','p',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'};

rom struct{byte bLength;byte bDscType;word string[19];}sd002={
sizeof(sd002),DSC_STR,
'M','C','P','2','5','1','5',' ','B','U',
'S',' ','M','O','N','I','T','O','R'};

rom struct{byte bLength;byte bDscType;word string[16];}sd003={
sizeof(sd003),DSC_STR,
'M','C','P','2','5','1','5','D','M','-','B','M',
' ','H','I','D'};

rom struct{byte report[29];}hid_rpt01={
    0x06, 0x00, 0xFF,       // Usage Page (Vendor Defined)
    0x09, 0x01,             // Usage (Vendor Usage)
    0xA1, 0x01,             // Collection (Application)
    0x19, 0x01,             //      Usage Minimum (Vendor Usage = 1)
//    0x29, 0x08,             //      Usage Maximum (Vendor Usage = 8)
    0x29, 0x40,             //      Usage Maximum (Vendor Usage = 64)
    0x15, 0x00,             //      Logical Minimum (Vendor Usage = 0)
    0x26, 0xFF, 0x00,       //      Logical Maximum (Vendor Usage = 255)
    0x75, 0x08,             //      Report Size (Vendor Usage = 8)
//    0x95, 0x08,             //      Report Count (Vendor Usage = 8)
    0x95, 0x40,             //      Report Count (Vendor Usage = 64)
    0x81, 0x02,             //      Input (Data, Var, Abs)
    0x19, 0x01,             //      Usage Minimum (Vendor Usage = 1)
//    0x29, 0x08,             //      Usage Maximum (Vendor Usage = 8)
    0x29, 0x40,             //      Usage Maximum (Vendor Usage = 64)
    0x91, 0x02,             //      Output (Data, Var, Ads)
    0xC0};                  // End Collection

rom const unsigned char *rom USB_CD_Ptr[]={&cfg01,&cfg01};
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

rom pFunc ClassReqHandler[1]=
{
    &USBCheckHIDRequest
};

#pragma code

/** EOF usbdsc.c ****************************************************/
