/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        io_cfg.h
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
 * Pat Richards         06/11/08    Modified to meet this application
 ********************************************************************/

#ifndef IO_CFG_H
#define IO_CFG_H

/** I N C L U D E S *************************************************/
#include "autofiles\usbcfg.h"

/** T R I S *********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/** U S B ***********************************************************/
#define tris_usb_bus_sense  TRISAbits.TRISA1    // Input

#if defined(USE_USB_BUS_SENSE_IO)
#define usb_bus_sense       PORTAbits.RA1
#else
#define usb_bus_sense       1
#endif

#define tris_self_power     TRISAbits.TRISA2    // Input

#if defined(USE_SELF_POWER_SENSE_IO)
#define self_power          PORTAbits.RA2
#else
#define self_power          1
#endif

#define mask_power_sense 0x02

//2515 board check if power is comming from USB or external (not CAN bus)
#define usb_power_check   self_power | usb_bus_sense //1 = USB or ext power; 0 = CANbus power

// External Transceiver Interface
#define tris_usb_vpo        TRISBbits.TRISB3    // Output
#define tris_usb_vmo        TRISBbits.TRISB2    // Output
#define tris_usb_rcv        TRISAbits.TRISA4    // Input
#define tris_usb_vp         TRISCbits.TRISC5    // Input
#define tris_usb_vm         TRISCbits.TRISC4    // Input
#define tris_usb_oe         TRISCbits.TRISC1    // Output

#define tris_usb_suspnd     TRISAbits.TRISA3    // Output

/** S P I : Chip Select and Serial Lines *****************************/
#define tris_CS             TRISBbits.TRISB4    // Output
#define CS_2515             LATBbits.LATB4
#define CS_2515_LOW()       CS_2515 = 0;
#define CS_2515_HIGH()      CS_2515 = 1;

#define SDIpin              PORTBbits.RB0

/**** Other MCP2515 Pins *****************************/
#define tris_INT_pin        TRISBbits.TRISB5    // Input
#define INT_2515()          PORTBbits.RB5

#define tris_SOF_pin        TRISBbits.TRISB2    // Input
#define SOF_2515            PORTBbits.RB2

#define tris_RXB1_pin       TRISBbits.TRISB6    // Input
#define RXB1_2515           PORTBbits.RB6

#define tris_RXB0_pin       TRISBbits.TRISB7    // Input
#define RXB0_2515           PORTBbits.RB7

#define tris_RTS0_pin       TRISDbits.TRISD7    // Output
#define RTS0_2515           LATDbits.LATD7
#define RTS0_2515_LOW()     RTS0_2515 = 0;
#define RTS0_2515_HIGH()    RTS0_2515 = 1;

#define tris_RTS1_pin       TRISDbits.TRISD6    // Output
#define RTS1_2515           LATDbits.LATD6
#define RTS1_2515_LOW()     RTS1_2515 = 0;
#define RTS1_2515_HIGH()    RTS1_2515 = 1;

#define tris_RTS2_pin       TRISDbits.TRISD5    // Output
#define RTS2_2515           LATDbits.LATD5
#define RTS2_2515_LOW()     RTS2_2515 = 0;
#define RTS2_2515_HIGH()    RTS2_2515 = 1;

/**** LEDs *****************************/
#define tris_LED_25PCT      TRISAbits.TRISA5    // Output
#define LED_25PCT_ON()      LATAbits.LATA5 = 0;
#define LED_25PCT_OFF()     LATAbits.LATA5 = 1;

#define tris_LED_50PCT      TRISEbits.TRISE0    // Output
#define LED_50PCT_ON()      LATEbits.LATE0 = 0;
#define LED_50PCT_OFF()     LATEbits.LATE0 = 1;

#define tris_LED_75PCT      TRISEbits.TRISE1    // Output
#define LED_75PCT_ON()      LATEbits.LATE1 = 0;
#define LED_75PCT_OFF()     LATEbits.LATE1 = 1;

#define tris_LED_100PCT     TRISEbits.TRISE2    // Output
#define LED_100PCT_ON()     LATEbits.LATE2 = 0;
#define LED_100PCT_OFF()    LATEbits.LATE2 = 1;

#define tris_LED_TX         TRISDbits.TRISD0    // Output
#define LED_TX_ON()         LATDbits.LATD0 = 0;
#define LED_TX_OFF()        LATDbits.LATD0 = 1;

#define tris_LED_RX         TRISDbits.TRISD1    // Output
#define LED_RX_ON()         LATDbits.LATD1 = 0;
#define LED_RX_OFF()        LATDbits.LATD1 = 1;

/**** Misc *****************************/
#define tris_CAN_RES        TRISCbits.TRISC0    // Output
#define CAN_RES_ON()        LATCbits.LATC0 = 0;
#define CAN_RES_OFF()       LATCbits.LATC0 = 1;

#define tris_SW_LOAD        TRISAbits.TRISA4    // Input
#define SW_LOAD             PORTAbits.RA4


#endif //IO_CFG_H
