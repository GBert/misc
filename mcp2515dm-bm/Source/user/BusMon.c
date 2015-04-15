/*********************************************************************
 *
 *                Microchip USB C18 Firmware - MCP2515 CAN Bus
 *                Monitor Demo
 *
 *********************************************************************
 * FileName:        BusMon.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.10
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
 * Pat Richards       06/12/08    Original.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include "spi.h"

#include "system\typedefs.h"
#include "system\usb\usb.h"

#include "io_cfg.h"             // I/O pin mapping
#include "BusMon.h"
#include "REGS2515.h"
#include "spi.h"

const char firmware_version[] = "2.10";
/** P R I V A T E  P R O T O T Y P E S ***************************************/
void CheckLoadButton(void);
unsigned char ReadSPI(void);
void SetBitTiming(char data_rate);
void CANLoadTX(void);

/** D E C L A R A T I O N S **************************************************/
#define CAN_1000kbps  0       //BRP setting in CNF1
#define CAN_500kbps   1       //
#define CAN_250kbps   3       //
#define CAN_125kbps   7       //

/** S T R U C T S ********************************************************/
struct {
    unsigned char INTF : 1; //bit 0
    unsigned char MCP_RXBn : 2;
    unsigned char SOF : 1;
    unsigned char USBsend : 1;
    unsigned char USBQueue : 1;
    unsigned char CANLoading : 1;
    unsigned char Flag8 : 1;
} UserFlag;

struct {
    unsigned char BusLoad : 2; //bit 0
    unsigned char TX_buff : 2; //OUT message to instruct TX buff to send
    unsigned char CAN_USB_PTR : 2;
    unsigned char nu2 : 1;
    unsigned char nu3 : 1;
} usbcan_STATUS;


/** V A R I A B L E S ********************************************************/
#pragma udata
//byte old_sw2,old_sw3;

BOOL emulate_mode;
rom signed char dir_table[] = {-4, -4, -4, 0, 4, 4, 4, 0};
byte movement_length;
byte vector = 0;

byte AquireData = 1; //0 = STOP; 1 = Aquire (Not Used)
char buffer[3];
char inbuffer[BUF_SIZE]; // 64 byte input to USB device buffer
char outbuffer[BUF_SIZE]; // 64 byte output to USB device buffer

byte TimerCounter = 0xF0;
static unsigned char gTimeout = 0;

unsigned int CANLoadTimer = 0x00;
unsigned int LoadPrescaler = 0x8000;

static byte LOAD_FLAG = PERCENT_0;

static char USB_ptr = 0xFF; //Points to the USB location of the start of the
//four possible CAN messages      
static char old_CANCTRL; //Used to track CANCTRL changes from host


char DataArray[32];
char ReadArray[32];

extern unsigned char isUsbPowered;


#pragma code

/******************************************************************************
 * Function:        void ProcessIO
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void) {
    unsigned char n, b, c, y;
    int a;
    // User Application USB tasks

    if (!(isUsbPowered)) //Only generate traffic if NOT connected to USB
    {
        CheckLoadButton();
        CANLoadTX();
        return;
    }

    if ((usb_device_state < CONFIGURED_STATE) || (UCONbits.SUSPND == 1)) return;

    //----- Read USB buffer if it has data from the host -----
    if (HIDRxReport(inbuffer, 64)) // USB receive buffer has data
    {
        LED_RX_ON(); //Turn LED on
        T0CONbits.TMR0ON = 1; //Start timer for TX LED on time
        gTimeout = 0; //Reset timout

        //---- CANmsgs: Check if host has requested CAN messages to be transmited
        switch (inbuffer[u_CANmsgs]) // interpret command
        {
            case 0x00: // No messages are available
                break;

            case 0x01: // Message 1 is available
                GetUSBData(m1_SIDH, 13, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, 13, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }
                break;

            case 0x02: // Message 1 and 2 are available
                //Message 1
                GetUSBData(m1_SIDH, m1_DLC + 5, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, m1_DLC + 5, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }

                //Message 2
                GetUSBData(m2_SIDH, m2_DLC + 5, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, m2_DLC + 5, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }
                break;

            case 0x03: // Message 1, 2, and 3 are available
                //Message 1
                GetUSBData(m1_SIDH, m1_DLC + 5, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, m1_DLC + 5, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }

                //Message 2
                GetUSBData(m2_SIDH, m2_DLC + 5, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, m2_DLC + 5, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }

                //Message 3
                GetUSBData(m3_SIDH, m3_DLC + 5, DataArray);
                n = SPILoadTX(CAN_LOAD_TX, m3_DLC + 5, DataArray);

                if (n == CAN_LD_TXB0_ID) //if TX0 is loaded
                {
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                } else if (n == CAN_LD_TXB1_ID) //if TX1 is loaded
                {
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                } else if (n == CAN_LD_TXB2_ID) //if TX2 is loaded
                {
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                }
                break;

            case 0x04: //--FUTURE-- Message 1, 2, 3, and 4 are available
                break;

            default: // unrecognized or null command
                ;
        }// END switch: inbuffer[u_CANmsgs]

        //---- CANCTRL: Write to the CANCTRL register if changed
        if (inbuffer[u_CANCTRL] != old_CANCTRL) //If host sent new CANCTRL value
        {
            SPIByteWrite(CANCTRL, inbuffer[u_CANCTRL]); //Write to CANCTRL
            EEPROMBYTEWrite(CANCTRL, inbuffer[u_CANCTRL]);
            EEPROMCRCWrite(0, 128);
            old_CANCTRL = inbuffer[u_CANCTRL]; //
            outbuffer[u_CANSTAT] = SPIByteRead(CANSTAT);
            while ((outbuffer[u_CANSTAT] & 0xE0) != (inbuffer[u_CANCTRL] & 0xE0))//if didn't change modes yet
            {
                outbuffer[u_CANSTAT] = SPIByteRead(CANSTAT);
            }
            UserFlag.USBsend = 1; //Set flag so will send USB message
        }

        //---- SPI: SPI command from host
        if (inbuffer[u_SPI]) //If host sent SPI command (non-zero)
        {
            switch (inbuffer[u_SPI]) {
                case CAN_RESET: //
                    SPIReset();
                    CANInit();

                    break;

                case CAN_READ: //
                    if (!UserFlag.USBQueue) // If previous message is queued
                    {
                        outbuffer[u_SPI] = CAN_READ; //Send back to host
                        outbuffer[u_REG] = inbuffer[u_REG]; //Send back to host
                        outbuffer[u_DATA] = SPIByteRead(inbuffer[u_REG]); //Send back to host
                    }
                    UserFlag.USBsend = 1; //Set flag so will send USB message
                    UserFlag.USBQueue = 1; //Indicates msg is queued, but not sent
                    break;

                case CAN_WRITE: //
                    //outbuffer[u_SPI] = 0;        //Send back to host //JM
                    SPIByteWrite(inbuffer[u_REG], inbuffer[u_DATA]);
                    EEPROMBYTEWrite(inbuffer[u_REG], inbuffer[u_DATA]);
                    EEPROMCRCWrite(0, 128);
                    break;

                case CAN_RTS: //
                    SPI_RTS(inbuffer[u_DATA]);
                    break;

                case CAN_RD_STATUS: //
                    outbuffer[u_DATA] = SPIReadStatus();
                    UserFlag.USBsend = 1; //Set flag so will send USB message
                    break;
                case FIRMWARE_VER_RD:
                    memmove(&outbuffer[u_STATUS], firmware_version, sizeof (firmware_version));
                    outbuffer[u_STATUS + sizeof (firmware_version)] = 0;
                    UserFlag.USBsend = 1; //Set flag so will send USB message
                    break;

                default: // unrecognized or null command
                    ;
            }// END switch: inbuffer[u_SPI]
        }
    }//END if (HIDRxReport(inbuffer, 1)

    //---- Check RXnBF pins and service messages as needed ---
    switch (CheckCANRX()) // Check if CAN message received
    {
        case 0x01: // Message in RXB0 (Msgs in this buffer are Standard)
            SPIReadRX(CAN_RD_START_RXB0SIDH, 13, ReadArray);
            LoadUSBString(ReadArray);
            break;

        case 0x02: // Message in RXB1 (Msgs in this buffer are Extended)
            SPIReadRX(CAN_RD_START_RXB1SIDH, 13, ReadArray);
            LoadUSBString(ReadArray);
            break;

        case 0x03: // Message in RXB0 and RXB1
            SPIReadRX(CAN_RD_START_RXB0SIDH, 13, ReadArray);
            LoadUSBString(ReadArray);
            SPIReadRX(CAN_RD_START_RXB1SIDH, 13, ReadArray);
            LoadUSBString(ReadArray);
            break;

        default: // unrecognized or null command
            ;
    }// END switch: CheckCANRX()

    //---- The following turns off the TX and RX USB indicator LEDs after some time
    //Inst. cycle = 200 ns; TMR0IF sets every 51 us
    if (INTCONbits.TMR0IF) {
        TimerCounter++;
        if (!TimerCounter) //if rolled over, set flag. User code will handle the rest.
        {
            LED_TX_OFF(); //Turn LED off
            LED_RX_OFF(); //Turn LED off
            T0CONbits.TMR0ON = 0; //Start timer for TX LED on time
            TimerCounter = 0xFE;
            gTimeout = 1; //Reset timout
        }
        INTCONbits.TMR0IF = 0;
    }

    //------ Load USB Data to be transmitted to the host --------
    if (UserFlag.MCP_RXBn | UserFlag.USBsend) {
        if (!mHIDTxIsBusy()) {
            HIDTxReport(outbuffer, 64);

            outbuffer[0] = 0x00; //PKR$$$ Need this??

            UserFlag.MCP_RXBn = 0; //clear flag
            UserFlag.USBsend = 0; //clear flag
            UserFlag.USBQueue = 0; //Clear message queue

            LED_TX_ON(); //Turn LED on
            T0CONbits.TMR0ON = 1; //Start timer for TX LED on time
            gTimeout = 0; //Reset timout

            outbuffer[u_SPI] = 0x00; //clear back to 00h so host doesn't detect "SPI response"
            USB_ptr = 0xFF; //Point to location 0xFF
            outbuffer[u_CANmsgs] = 0x00; //Clear message status
        }
    }
}//end ProcessIO

/******************************************************************************
 * Function:        char CheckCANRX(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          Which RX buffer have message
 *                  0 = None; 1 = RXB0; 2 = RXB1; 3 = Both buffers
 *
 * Side Effects:    None
 *
 * Overview:        
 *
 * Note:            
 *****************************************************************************/
char CheckCANRX(void) {
    //First, set the correct flag. ORs with flag because this might not be the
    //1st time through

    UserFlag.MCP_RXBn = 0;
    if (!RXB0_2515) {
        UserFlag.MCP_RXBn = UserFlag.MCP_RXBn | 0x01; //RXB0 is active low
    }
    if (!RXB1_2515) {
        UserFlag.MCP_RXBn = UserFlag.MCP_RXBn | 0x02; //RXB1 is active low
    }

    return UserFlag.MCP_RXBn;
}

/******************************************************************************
 * Function:        void LoadUSBString//(char *data)
 *
 * PreCondition:    Set the pointer to the USB array "USB_ptr"
 *
 * Input:           Pointer to array that contains the data to write to USB
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Loads data read from the MCP2515 RX buffer. Note, the
 *                  function already knows the *data is 13 bytes
 *
 * Note:            
 *****************************************************************************/
void LoadUSBString(char *data) {
    unsigned char n, rtr, ide, dlc, num_bytes, CAN_ptr;

    CAN_ptr = 0;

    if (USB_ptr == 0xFF) //If 0xFF, then this is the 1st time through
        USB_ptr = 0;

    //-----Calculate how big the CAN msg is... how many USB bytes to use---
    if (data[mx_SIDL] & 0x08) { //If extended ID
        ide = 0x20; //Maps to 'INFO' byte in USB
        num_bytes = 0x04;
        //All four ID registers (extended ID)
    } else {
        ide = 0x00; //Maps to 'INFO' byte in USB
        num_bytes = 0x02; //Just the standard ID registers
    }

    if (ide) //If extended ID, then RTR is in DLC; else in SIDL
    {
        if (data[mx_DLC] & 0x40) { //RTR bit in DLC
            rtr = 0x10; //Maps to 'INFO' byte in USB
        } else {
            rtr = 0x00; //Maps to 'INFO' byte in USB
        }
    } else {
        if (data[mx_SIDL] & 0x10) { //RTR bit in SIDL
            rtr = 0x10; //Maps to 'INFO' byte in USB
        } else {
            rtr = 0x00; //Maps to 'INFO' byte in USB
        }
    }
    dlc = data[mx_DLC] & 0x0F; //Determine the data length code
    //END--Calculate how big the CAN msg is... how many USB bytes to use---END

    //Calculate if there is enough room to add the CAN message to USB:
    //52 is the location of the 1st control message. Subract the pointer location
    //If this is larger than the current CAN message (includes the "INFO" byte)
    //then can load 'outbuffer[]'. Otherwise, will need to switch interfaces
    //(assuming currently on the 1st interface).

    //If enough room, add the bytes
    if (52 - USB_ptr > num_bytes) {
        //Now load the 'INFO' byte
        outbuffer[USB_ptr] = 0x80 + ide + rtr + dlc;
        USB_ptr++;

        //Load the identifier registers
        for (n = 0; n < num_bytes; n++) {
            outbuffer[USB_ptr] = data[CAN_ptr];
            USB_ptr++;
            CAN_ptr++;
        }

        if (!ide) CAN_ptr = CAN_ptr + 2; //skip over EXIDE regs
        CAN_ptr++; //skip over the DLC register

        if (!rtr) //if a data frame
        {
            //Load the data registers
            for (n = 0; n < dlc; n++) //
            {
                outbuffer[USB_ptr] = data[CAN_ptr];
                USB_ptr++;
                CAN_ptr++;
            }
        }

        //Now clear next byte (STATUS). Cleared to 0 in case this is the last time through.
        //PC looks at MSb and, if clear, no message follows
        outbuffer[USB_ptr] = 0x00;

        //Next, need to load the other status bytes from byte 53 - 59
        //[CANINTF, EFLG, TEC, REC, CANSTAT, CANCTRL]

        //Need to read TEC, REC, and CANSTAT to report to host
        SPISeqRead(TEC, 3, ReadArray);
        outbuffer[u_TEC] = ReadArray[0];
        outbuffer[u_REC] = ReadArray[1];
        outbuffer[u_CANSTAT] = ReadArray[2];
    }
}

/******************************************************************************
 * Function:        void GetUSBData//(char start_ptr, char num_bytes, char *data)
 *
 * PreCondition:    None
 *
 * Input:           Pointer to location in USB string
 *                  Number of bytes to get
 *                  Pointer to array that contains the data to write to USB
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Gets data from the USB string.
 *
 * Note:            
 *****************************************************************************/
void GetUSBData(char start_ptr, char num_bytes, char *data) {
    unsigned char n;

    for (n = start_ptr; n < start_ptr + num_bytes; n++)
        data[n - start_ptr] = inbuffer[n];
}

/******************************************************************************
 * Function:        void UserInit
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function initializes the PIC.
 *                  
 *
 * Note:            None
 *****************************************************************************/

void UserInit(void) {
    byte i;

#if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See io_cfg.h
#endif

#if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;
#endif


    CS_2515_HIGH(); //Drive high
    tris_CS = 0; //Output
    OpenSPI(SPI_FOSC_16, MODE_00, SMPMID);

    TRISBbits.TRISB0 = 1; //SDI
    TRISBbits.TRISB1 = 0; //SCK

    //-------------------------
    // initialize variables
    //-------------------------
    for (i = 0; i < BUF_SIZE; i++) // initialize input and output buffer to 0
    {
        inbuffer[i] = 0;
        outbuffer[i] = 0;
    }

    //Timer 0
    TMR0H = 0; //clear timer
    TMR0L = 0; //clear timer
    T0CONbits.PSA = 0; //Assign prescaler to Timer 0
    T0CONbits.T0PS2 = 1; //Setup prescaler
    T0CONbits.T0PS1 = 1; //Will time out every 51 us based on
    T0CONbits.T0PS0 = 1; //20 MHz Fosc
    T0CONbits.T0CS = 0; //Increment on instuction cycle

    INTCON2 = 0xFF; //INT2 on rising edge
    INTCON3bits.INT2IP = 0; //Low priority
    INTCON3bits.INT2IF = 0; //Clear flag
    INTCON3bits.INT2IE = 1; //Enable INT2 interrupt

    //Outputs for the LEDs
    ADCON1 = 0x0F; //Digital pins
    CMCON = 0x07; //Digital pins

    LED_25PCT_OFF();
    LED_50PCT_OFF();
    LED_75PCT_OFF();
    LED_100PCT_OFF();

    TRISBbits.TRISB6 = INPUT_PIN;
    TRISBbits.TRISB7 = INPUT_PIN;

    tris_LED_25PCT = OUTPUT_PIN;
    tris_LED_50PCT = OUTPUT_PIN;
    tris_LED_75PCT = OUTPUT_PIN;
    tris_LED_100PCT = OUTPUT_PIN;

    UserFlag.CANLoading = OFF;

    LED_RX_OFF();
    LED_TX_OFF();

    tris_LED_TX = OUTPUT_PIN;
    tris_LED_RX = OUTPUT_PIN;

    tris_SW_LOAD = INPUT_PIN;

    //RTS Pin Outputs
    RTS0_2515_HIGH();
    tris_RTS0_pin = OUTPUT_PIN;

    RTS1_2515_HIGH();
    tris_RTS1_pin = OUTPUT_PIN;

    RTS2_2515_HIGH();
    tris_RTS2_pin = OUTPUT_PIN;

    tris_CAN_RES = OUTPUT_PIN;
    CAN_RES_ON(); //JM
    //  CAN_RES_OFF();  //Disconnect the termination resistor by default

    UserFlag.MCP_RXBn = 0; //clear flag
    UserFlag.USBsend = 0; //clear flag
    UserFlag.USBQueue = 0; //Clear message queue

    //Need to set up MCP2515 before enabling interrupts
    CANInit(); // See BusMon.c & .h

    RCONbits.IPEN = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}//end UserInit

/******************************************************************************
 * Function:        void CANInit//(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function initializes the MCP2515.
 *                  
 *
 * Note:            None
 *****************************************************************************/
void CANInit() {
    unsigned char n, EEPROM_ver;
    SPIReset(); //Wait state is in SPIReset() function
    EEPROM_ver = EEPROMCRCCheck(0, 128);
    //EEPROM_ver=0;
    for (n = 0; n < 250; n++); //Wait anyway

    //Set Config Mode and verify
    //CANCTRL Register

    if (EEPROM_ver) {
        CANCTRL_byte = EEPROMBYTERead(CANCTRL);
    }
    /*
    else
    {
                    EEPROMBYTEWrite(CANCTRL,CANCTRL_byte);
    }
     */
    SET_CONFIG; //Set Configuration mode
    CLEAR_OSM; //Not One-Shot-Mode
    CLEAR_ABORT; //Clear ABAT bit
    CLKOUT_ENABLED; //Enabled CLKOUT pin (will configure as SOF pin in CNF3)
    if (!EEPROM_ver) {
        EEPROMBYTEWrite(CANCTRL, CANCTRL_byte);
    }

    SPIByteWrite(CANCTRL, CANCTRL_byte); //Write to CANCTRL
    //Verify Mode change
    if ((SPIByteRead(CANSTAT)& 0xE0) != OPMODE_CONFIG)
        SPIByteWrite(CANCTRL, CANCTRL_byte); //Try again

    if (isUsbPowered) //Only if connected to USB. Traffic gen board doesn't process RX msgs
    {
        //Configure INTERRUPTS

        if (EEPROM_ver) {
            CANINTE_byte = EEPROMBYTERead(CANINTE);
        }
        /*
        else
        {
                EEPROMBYTEWrite(CANINTE,CANINTE_byte);
        }
         */
        G_RXB_INT_ENABLED;
        G_TXB_INT_ENABLED;
        if (!EEPROM_ver) {
            EEPROMBYTEWrite(CANINTE, CANINTE_byte);
        }

        SPIByteWrite(CANINTE, CANINTE_byte);
    }

    //Use RXnBF pins as interrupts on RX //(BFPCTRL register)
    RXB0BF_PIN_INT;
    RXB1BF_PIN_INT;

    SPIByteWrite(BFPCTRL, BFPCTRL_byte);

    //Configure TXnRTS pins as RTS pins
    TXRTSCTRL_byte = 0x07;
    SPIByteWrite(TXRTSCTRL, TXRTSCTRL_byte);


    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXB0CTRL);
        SPIByteWrite(RXB0CTRL, DataArray[0]);
        DataArray[0] = EEPROMBYTERead(RXB1CTRL);
        SPIByteWrite(RXB1CTRL, DataArray[0]);
    } else {
        DataArray[0] = 0;
        SPIByteWrite(RXB0CTRL, DataArray[0]);
        SPIByteWrite(RXB1CTRL, DataArray[0]);
        EEPROMBYTEWrite(RXB0CTRL, DataArray[0]);
        EEPROMBYTEWrite(RXB1CTRL, DataArray[0]);
    }

    //Configure MASKS
    DataArray[0] = 0x00;
    DataArray[1] = 0x00;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXM0SIDH);
        DataArray[1] = EEPROMBYTERead(RXM0SIDL);
        DataArray[2] = EEPROMBYTERead(RXM0EID8);
        DataArray[3] = EEPROMBYTERead(RXM0EID0);
    } else {
        EEPROMDataWrite(RXM0SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXM0SIDH, 4, DataArray); //RX Mask 0

    DataArray[0] = 0x00;
    DataArray[1] = 0x00;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXM1SIDH);
        DataArray[1] = EEPROMBYTERead(RXM1SIDL);
        DataArray[2] = EEPROMBYTERead(RXM1EID8);
        DataArray[3] = EEPROMBYTERead(RXM1EID0);
    } else {
        EEPROMDataWrite(RXM1SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXM1SIDH, 4, DataArray); //RX Mask 1
    //SPISeqWrite(RXB0CTRL, 1, 0);  //RXB0 Control JM
    //SPISeqWrite(RXB1CTRL, 1, 0);  //RXB1 Control JM

    //Configure FILTERS
    //Will RX standard messages into RXB0 and extended into RXB1


    DataArray[0] = 0x00;
    DataArray[1] = 0x00;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF0SIDH);
        DataArray[1] = EEPROMBYTERead(RXF0SIDL);
        DataArray[2] = EEPROMBYTERead(RXF0EID8);
        DataArray[3] = EEPROMBYTERead(RXF0EID0);
    } else {
        EEPROMDataWrite(RXF0SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF0SIDH, 4, DataArray);
    //SPIByteWrite(RXF0SIDL, 0x00); //Clearing EXIDE... the rest is "don't care"

    DataArray[0] = 0x00;
    DataArray[1] = 0x00;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF1SIDH);
        DataArray[1] = EEPROMBYTERead(RXF1SIDL);
        DataArray[2] = EEPROMBYTERead(RXF1EID8);
        DataArray[3] = EEPROMBYTERead(RXF1EID0);
    } else {
        EEPROMDataWrite(RXF1SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF1SIDH, 4, DataArray);

    //SPIByteWrite(RXF1SIDL, 0x00); //Clearing EXIDE... the rest is "don't care" //JM

    DataArray[0] = 0x00;
    DataArray[1] = 0x08;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF2SIDH);
        DataArray[1] = EEPROMBYTERead(RXF2SIDL);
        DataArray[2] = EEPROMBYTERead(RXF2EID8);
        DataArray[3] = EEPROMBYTERead(RXF2EID0);
    } else {
        EEPROMDataWrite(RXF2SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF2SIDH, 4, DataArray);

    //SPIByteWrite(RXF2SIDL, 0x08); //Setting EXIDE... the rest is "don't care"

    DataArray[0] = 0x00;
    DataArray[1] = 0x08;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF3SIDH);
        DataArray[1] = EEPROMBYTERead(RXF3SIDL);
        DataArray[2] = EEPROMBYTERead(RXF3EID8);
        DataArray[3] = EEPROMBYTERead(RXF3EID0);
    } else {
        EEPROMDataWrite(RXF3SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF3SIDH, 4, DataArray);


    //SPIByteWrite(RXF3SIDL, 0x08); //Setting EXIDE... the rest is "don't care" //JM

    DataArray[0] = 0x00;
    DataArray[1] = 0x08;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF4SIDH);
        DataArray[1] = EEPROMBYTERead(RXF4SIDL);
        DataArray[2] = EEPROMBYTERead(RXF4EID8);
        DataArray[3] = EEPROMBYTERead(RXF4EID0);
    } else {
        EEPROMDataWrite(RXF4SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF4SIDH, 4, DataArray);

    //SPIByteWrite(RXF4SIDL, 0x08); //Setting EXIDE... the rest is "don't care" //JM

    DataArray[0] = 0x00;
    DataArray[1] = 0x08;
    DataArray[2] = 0x00;
    DataArray[3] = 0x00;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(RXF5SIDH);
        DataArray[1] = EEPROMBYTERead(RXF5SIDL);
        DataArray[2] = EEPROMBYTERead(RXF5EID8);
        DataArray[3] = EEPROMBYTERead(RXF5EID0);
    } else {
        EEPROMDataWrite(RXF5SIDH, &DataArray[0], 4);
    }

    SPISeqWrite(RXF5SIDH, 4, DataArray);

    //SPIByteWrite(RXF5SIDL, 0x08); //Setting EXIDE... the rest is "don't care" //JM

    //Configure TX BUFFER 0
    DataArray[0] = 0xA3;
    DataArray[1] = 0x09;
    DataArray[2] = 0x12;
    DataArray[3] = 0x34;
    DataArray[4] = 0x04;
    DataArray[5] = 0x00;
    DataArray[6] = 0x01;
    DataArray[7] = 0x02;
    DataArray[8] = 0x03;
    DataArray[9] = 0x04;
    DataArray[10] = 0x05;
    DataArray[11] = 0x06;
    DataArray[12] = 0x07;


    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(TXB0SIDH);
        DataArray[1] = EEPROMBYTERead(TXB0SIDL);
        DataArray[2] = EEPROMBYTERead(TXB0EID8);
        DataArray[3] = EEPROMBYTERead(TXB0EID0);
        DataArray[4] = EEPROMBYTERead(TXB0DLC);
        DataArray[5] = EEPROMBYTERead(TXB0D0);
        DataArray[6] = EEPROMBYTERead(TXB0D1);
        DataArray[7] = EEPROMBYTERead(TXB0D2);
        DataArray[8] = EEPROMBYTERead(TXB0D3);
        DataArray[9] = EEPROMBYTERead(TXB0D4);
        DataArray[10] = EEPROMBYTERead(TXB0D5);
        DataArray[11] = EEPROMBYTERead(TXB0D6);
        DataArray[12] = EEPROMBYTERead(TXB0D7);

    } else {
        EEPROMDataWrite(TXB0SIDH, &DataArray, 13);
    }

    SPILoadTX(CAN_LD_TXB0_ID, 13, DataArray);

    //Configure TX BUFFER 1
    DataArray[0] = 0x22;
    DataArray[1] = 0x00;
    DataArray[2] = 0x64;
    DataArray[3] = 0x68;
    DataArray[4] = 0x02;
    DataArray[5] = 0x00;
    DataArray[6] = 0x11;
    DataArray[7] = 0x22;
    DataArray[8] = 0x33;
    DataArray[9] = 0x44;
    DataArray[10] = 0x55;
    DataArray[11] = 0x66;
    DataArray[12] = 0x77;

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(TXB1SIDH);
        DataArray[1] = EEPROMBYTERead(TXB1SIDL);
        DataArray[2] = EEPROMBYTERead(TXB1EID8);
        DataArray[3] = EEPROMBYTERead(TXB1EID0);
        DataArray[4] = EEPROMBYTERead(TXB1DLC);
        DataArray[5] = EEPROMBYTERead(TXB1D0);
        DataArray[6] = EEPROMBYTERead(TXB1D1);
        DataArray[7] = EEPROMBYTERead(TXB1D2);
        DataArray[8] = EEPROMBYTERead(TXB1D3);
        DataArray[9] = EEPROMBYTERead(TXB1D4);
        DataArray[10] = EEPROMBYTERead(TXB1D5);
        DataArray[11] = EEPROMBYTERead(TXB1D6);
        DataArray[12] = EEPROMBYTERead(TXB1D7);

    } else {
        EEPROMDataWrite(TXB1SIDH, &DataArray, 13);
    }

    SPILoadTX(CAN_LD_TXB1_ID, 13, DataArray);

    //Configure TX BUFFER 2
    DataArray[0] = 0xAA;
    DataArray[1] = 0x03;
    DataArray[2] = 0x33;
    DataArray[3] = 0x33;
    DataArray[4] = 0x08;
    DataArray[5] = 0xAA;
    DataArray[6] = 0xBB;
    DataArray[7] = 0xCC;
    DataArray[8] = 0xDD;
    DataArray[9] = 0xEE;
    DataArray[10] = 0xFF;
    DataArray[11] = 0x0A;
    DataArray[12] = 0x0B;
    SPILoadTX(CAN_LD_TXB2_ID, 13, DataArray);

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(TXB2SIDH);
        DataArray[1] = EEPROMBYTERead(TXB2SIDL);
        DataArray[2] = EEPROMBYTERead(TXB2EID8);
        DataArray[3] = EEPROMBYTERead(TXB2EID0);
        DataArray[4] = EEPROMBYTERead(TXB2DLC);
        DataArray[5] = EEPROMBYTERead(TXB2D0);
        DataArray[6] = EEPROMBYTERead(TXB2D1);
        DataArray[7] = EEPROMBYTERead(TXB2D2);
        DataArray[8] = EEPROMBYTERead(TXB2D3);
        DataArray[9] = EEPROMBYTERead(TXB2D4);
        DataArray[10] = EEPROMBYTERead(TXB2D5);
        DataArray[11] = EEPROMBYTERead(TXB2D6);
        DataArray[12] = EEPROMBYTERead(TXB2D7);

    } else {
        EEPROMDataWrite(TXB2SIDH, &DataArray, 13);
    }


    SPISeqRead(TXB1SIDH, 13, ReadArray);

    //Configure BIT TIMING
    SOF_ENABLED; //Start-of-Frame signal enabled. Will set when "SetBitTiming()" is called

    if (EEPROM_ver) {
        DataArray[0] = EEPROMBYTERead(CNF3); //Load array
        DataArray[1] = EEPROMBYTERead(CNF2); //
        DataArray[2] = EEPROMBYTERead(CNF1); //
        SPISeqWrite(CNF3, 3, DataArray); //Write registers

    } else {
        SetBitTiming(CAN_125kbps); //Note, this function puts device in Normal Mode

    }

    //Set Normal Mode and verify
    SET_NORMAL;
    SPIByteWrite(CANCTRL, CANCTRL_byte); //Write to CANCTRL
    old_CANCTRL = 0x00;
    //EEPROMCRCWrite(0,128);

    //Verify Mode change
    if ((SPIByteRead(CANSTAT)& 0xE0) != OPMODE_NORMAL)
        SPIByteWrite(CANCTRL, CANCTRL_byte); //Try again
}

/******************************************************************************
 * Function:        void SetBitTiming//(char data_rate)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the bit timing.
 *                  
 *
 * Note:            None
 *****************************************************************************/
void SetBitTiming(char data_rate) {
    unsigned char a, b, c, dummy;
    //Set Config Mode and verify
    SET_CONFIG; //Set Configuration mode
    SPIByteWrite(CANCTRL, CANCTRL_byte); //Write to CANCTRL
    //Verify Mode change
    if ((SPIByteRead(CANSTAT)& 0xE0) != OPMODE_CONFIG)
        SPIByteWrite(CANCTRL, CANCTRL_byte); //Try again

    //Need 10TQ based on 20 MHz osc
    //Will only need to modify BRP to acheive the 4 bit rates
    //First clear CNFn registers
    CNF1_byte = 0;
    CNF2_byte = 0;
    CNF3_byte = 0;
    //Now set individual bits
    SJW_1TQ; //Sync Jump Width
    PRSEG_3TQ; //Prop Segment
    PHSEG1_3TQ; //Phase Seg 1
    PHSEG2_3TQ; //Phase Seg 2
    BTLMODE_CNF3; //Use CNF3 to set Phase Seg 2

    SOF_ENABLED; //Start-of-Frame signal enabled. Will set when "SetBitTiming()" is called

    //Configure Baud Rate Prescaler
    switch (data_rate) // interpret command
    {
        case CAN_125kbps: // Set to 125 kbps
            BRP7; //
            break;

        case CAN_250kbps: // Set to 250 kbps
            BRP3; //
            break;

        case CAN_500kbps: // Set to 500 kbps
            BRP1; //
            break;

        case CAN_1000kbps: // Set to 1000 kbps
            BRP0; //
            break;

        default: // unrecognized or null command
            ;
    }// END switch: data_rate

    //Now write the CNFn registers
    DataArray[0] = CNF3_byte; //Load array
    DataArray[1] = CNF2_byte; //
    DataArray[2] = CNF1_byte; //
    SPISeqWrite(CNF3, 3, DataArray); //Write registers

    //Set Normal Mode and verify
    SET_NORMAL;
    SPIByteWrite(CANCTRL, CANCTRL_byte); //Write to CANCTRL
    //Verify Mode change
    if ((SPIByteRead(CANSTAT)& 0xE0) != OPMODE_CONFIG)
        SPIByteWrite(CANCTRL, CANCTRL_byte); //Try again
    EEPROMDataWrite(CNF3, &DataArray, 3);
}

/******************************************************************************
 * Function:        void CheckLoadButton
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function checks if the bus load button is being pressed
 *                  and increments the bus load LEDs and LOAD_FLAG as needed
 *
 * Note:            
 *****************************************************************************/
void CheckLoadButton(void) {
    static byte flag_enable = 0x01, button_flag = 0;

    //---- Software latch and debounce
    if (SW_LOAD) //if button is released
    {
        button_flag = 0x00;
        flag_enable = 0x01; //Open gate so it will pass "bd_flag"
    } else if (!SW_LOAD) // else button is pressed
    {
        button_flag = 0x01;
    }

    //---- If button press was latched
    if (button_flag & flag_enable) //if button is pressed (gated by pass_gate flag)
    {
        flag_enable = 0x00; //Disable gate so code will not return here until
        //button is released first
        //"switch" calculation is done so "case" match increments, then loops
        //after reaching last case.
        //... but first, increment LOAD_FLAG and reset if it is > 4h
        LOAD_FLAG++;
        if (LOAD_FLAG > 0x04) LOAD_FLAG = 0x00;

        switch (LOAD_FLAG) // interpret command
        {
            case PERCENT_0: // 0% Bus Load
                LED_25PCT_OFF();
                LED_50PCT_OFF();
                LED_75PCT_OFF();
                LED_100PCT_OFF();
                UserFlag.CANLoading = OFF;
                LOAD_FLAG = PERCENT_0; //set to jump to next 'case' next time through
                break;

            case PERCENT_25: // 25% Bus Load
                LED_25PCT_ON();
                LED_50PCT_OFF();
                LED_75PCT_OFF();
                LED_100PCT_OFF();

                UserFlag.CANLoading = ON;
                LoadPrescaler = 0x8000;
                LOAD_FLAG = PERCENT_25; //set to jump to next 'case' next time through
                break;

            case PERCENT_50: // 50% Bus Load
                LED_25PCT_ON();
                LED_50PCT_ON();
                LED_75PCT_OFF();
                LED_100PCT_OFF();

                UserFlag.CANLoading = ON;
                LoadPrescaler = 0xC000;
                LOAD_FLAG = PERCENT_50; //set to jump to next 'case' next time through
                break;

            case PERCENT_75: // 75% Bus Load
                LED_25PCT_ON();
                LED_50PCT_ON();
                LED_75PCT_ON();
                LED_100PCT_OFF();

                UserFlag.CANLoading = ON;
                LoadPrescaler = 0xF000;
                LOAD_FLAG = PERCENT_75; //set to jump to next 'case' next time through
                break;

            case PERCENT_100: // 100% Bus Load
                LED_25PCT_ON();
                LED_50PCT_ON();
                LED_75PCT_ON();
                LED_100PCT_ON();

                UserFlag.CANLoading = ON;
                LoadPrescaler = 0xFA00;
                LOAD_FLAG = PERCENT_100;
                break;

            default: // unrecognized or null command
                ;
        }// END switch: load_flag
    }
}

/******************************************************************************
 * Function:        void CANLoadTX(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function transmits messages on the CAN bus depending
 *                  on the Load Button setting
 *
 * Note:            
 *****************************************************************************/
void CANLoadTX(void) {
    static char buffer_q;

    if (UserFlag.CANLoading) //if bus loading is turned on
    {

        CANLoadTimer++; //Increment the counter
        if (!CANLoadTimer) //if rolled over, set flag. User code will handle the rest.
        {
            CANLoadTimer = LoadPrescaler;
            switch (buffer_q) // interpret command
            {
                case 0: // Send buffer 0
                    RTS0_2515_LOW();
                    RTS0_2515_HIGH();
                    buffer_q = 1; //For next time around
                    break;
                case 1: // Send buffer 1
                    RTS1_2515_LOW();
                    RTS1_2515_HIGH();
                    buffer_q = 2; //For next time around
                    break;
                case 2: // Send buffer 2
                    RTS2_2515_LOW();
                    RTS2_2515_HIGH();
                    buffer_q = 0; //For next time around
                    break;
                default: // unrecognized or null command
                    buffer_q = 0;
            }// END switch:
        }//END: if (!CANLoadTimer)
    }//END: if(UserFlag.CANLoading)
}

/******************************************************************************
 * Function:        void EEPROMBYTEWrite(char addr,char data)
 *
 * PreCondition:    None
 *
 * Input:           data and address
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/
void EEPROMBYTEWrite(char addr, char data) {
    char buf_con = INTCON;
    EEADR = addr;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xaa;
    EECON1bits.WR = 1;
    while (EECON1bits.WR == 1);
    EECON1bits.WREN = 0;
    //INTCONbits.GIE=1;
    INTCON = buf_con;


}

/******************************************************************************
 * Function:        void EEPROMDataWrite(char addr,char* data,char len)
 *
 * PreCondition:    void EEPROMBYTEWrite(char addr,char data)
 *
 * Input:           begin address,data address,data length
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/
void EEPROMDataWrite(char addr, char* data, char len) {
    unsigned char i;
    for (i = 0; i < len; i++) {
        EEPROMBYTEWrite(addr + i, data[i]);
    }
}

/******************************************************************************
 * Function:        void EEPROMBYTERead(char addr,char data)
 *
 * PreCondition:    None
 *
 * Input:           address
 *
 * Output:          data
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/

char EEPROMBYTERead(char addr) {
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    return EEDATA;

}

/******************************************************************************
 * Function:        void EEPROMDataRead(char addr,char data)
 *
 * PreCondition:    None
 *
 * Input:           address in EEPROM,buffer address,data length
 *
 * Output:          data
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/
void EEPROMDataRead(char addr, char* data, char len) {
    unsigned char i;
    for (i = 0; i < len; i++) {
        data[i] = EEPROMBYTERead(addr + i);
    }
}

/******************************************************************************
 * Function:        void EEPROMCRCWrite(char addr,char len)
 *
 * PreCondition:    None
 *
 * Input:           address in EEPROM,data length
 *
 * Output:          data
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/

void EEPROMCRCWrite(char addr, char len) {
    unsigned char buf, i;
    buf = 0;
    for (i = 0; i < len; i++) {
        buf += EEPROMBYTERead(i + addr);
    }
    EEPROMBYTEWrite(128, buf);

}

/******************************************************************************
 * Function:        bool EEPROMCRCCheck(char addr,char len)
 *
 * PreCondition:    None
 *
 * Input:           address in EEPROM,data length
 *
 * Output:          true for checksum equal
 *
 * Side Effects:    None
 *
 * Overview:        This function writes a byte to EEPROM in addr
 *
 * Note:            
 *****************************************************************************/

BOOL EEPROMCRCCheck(char addr, char len) {
    unsigned char buf, i;
    buf = 0;
    for (i = 0; i < len; i++) {
        buf += EEPROMBYTERead(i + addr);
    }

    if (buf == EEPROMBYTERead(128)) {
        return TRUE;
    } else {
        return FALSE;
    }

}

/** EOF BusMon.c********************************************************/
