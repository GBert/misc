//-----------------------------------------------------------------------------
// F50x_CAN0_Transmit.c
//-----------------------------------------------------------------------------
// Copyright 2008 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// This program transmits data to another CAN node using a configurable number
// of message objects and datasize.  The message identifier is set based on the 
// message object number.  The corresponding receive firmware is 
// F50x_CAN0_Receive.c.  The following two #defines should be set to the
// same values in both the transmit and receive code:
//
// #define MESSAGE_OBJECTS          
// #define MESSAGE_SIZE    
//
// #define TX_COMPLETE_MASK should also be set.  See definition for details
//
// The purpose of this example is to show how to initialize a message object
// for transfer and how to handle certain error conditions.
//
// How To Test:
//
// 1) Verify the LED and switch pin jumpers are populated 
//    (J19 for device A and J11 for device B).
//
// 2) Make sure the CAN jumpers in J17 (for A side) and J26 (for B side)
//    are connected.
//
// 3) Download the code to a F50x-TB (either device A or device B) that is
//    connected as above to another device running the F50x_CAN0_Receive 
//    code.
//
// 4) Run the code on the Receiver first, then run the code on the Transmitter.
//
// 5) After all the transmissions are complete, the LED will be lit if
//    the transfers were sucessful.
//
// Target:         C8051F500 (Side A of a C8051F500-TB)
// Tool chain:     Keil C51 8.0 / Keil EVAL C51
// Command Line:   None
//
// Release 1.4 / 01 APR 2009 (GP)
//    - Fixed polling for CAN Busy to wait for 0b
//
// Release 1.3 / 22 DEC 2008 (GP)
//    - Updated CAN initalization loop for unused objects
//
// Release 1.2 / 18 JUL 2008 (ADT)
//    - Removed Oscillator calibration
//
// Release 1.1 / 11 JUN 2008 (ADT)
//    - Edited Formatting
//
// Release 1.0 / 11 MAR 2008 (GP)
//    - Initial Revision
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "compiler_defs.h"
#include "C8051F500_defs.h"            // SFR declarations

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------

void OSCILLATOR_Init (void);
void PORT_Init (void);
void CAN0_Init (void);

void CAN0_TransferMO (U8 obj_num);

INTERRUPT_PROTO (CAN0_ISR, INTERRUPT_CAN0);

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define SYSCLK       24000000          // System clock speed in Hz

#define MESSAGE_OBJECTS    32          // Number of message objects to use
                                       // Range is 1-32
#define MESSAGE_SIZE        8          // Size in bytes of each CAN message
                                       // Range is 1-8

#define TX_COMPLETE_MASK  0xFFFFFFFF   // Set this to (2^MESSAGE_OBJECTS - 1)


//-----------------------------------------------------------------------------
// Bit Definition Masks
//-----------------------------------------------------------------------------

// CAN0STAT
#define BOff  0x80                     // Busoff Status
#define EWarn 0x40                     // Warning Status
#define EPass 0x20                     // Error Passive
#define RxOk  0x10                     // Receive Message Successfully
#define TxOk  0x08                     // Transmitted Message Successfully
#define LEC   0x07                     // Last Error Code

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT (LED, SFR_P1, 3);                 // LED = 1 turns on the LED

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

bit CAN_ERROR = 0;                     // 0 = No Errors during transmission
                                       // 1 = Some error(s) occurred

UU32 CAN_TX_COMPLETE;                  // Bit status register that is updated
                                       // when a TX complete is received for
                                       // a specific message object.  Should be
                                       // equal to TX_COMPLETE_MASK when done

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void main (void)
{
   U16 iter;                           // Loop counter

   SFRPAGE = ACTIVE_PAGE;              // Set for PCA0MD

   PCA0MD &= ~0x40;                    // Disable Watchdog Timer

   OSCILLATOR_Init ();                 // Initialize oscillator
   PORT_Init ();                       // Initialize crossbar and GPIO
   CAN0_Init ();                       // Start CAN peripheral

   CAN_TX_COMPLETE.U32 = 0x0000;       // Initialize as no messages transmitted

   EIE2 |= 0x02;                       // Enable CAN interupts
   EA = 1;                             // Enable global interrupts

   // CAN0_TransferMO will automatically generate data and send it using the
   // specified Message Object.  The receive code knows what data to expect
   // for each Message Object and performs the data verification.

   LED = 0;

   for (iter = 0; iter < MESSAGE_OBJECTS; iter++) 
   {
      CAN0_TransferMO (iter);
   }

   // Check Transmission Requests registers to wait for completion
   SFRPAGE = CAN0_PAGE;
   while (CAN0TR1 | CAN0TR2) 
   {
      LED = !LED;
      for (iter = 0; iter < 65000; iter++);   
   }

   // Check global variable that all transfers completed without error
   if (CAN_TX_COMPLETE.U32 != TX_COMPLETE_MASK) 
   {
      CAN_ERROR = 1; 
   }

   // Once all transmissions are complete, turn off the LED or leave it on 
   // based on the error status

   if (CAN_ERROR) 
   {                                   // If any errors occurred
      LED = 0;                         // Turn off LED
   }                       
   else 
   {
      LED = 1;                         // No error, so keep LED on
   }                       

   while (1);
}

//-----------------------------------------------------------------------------
// Initialization Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// OSCILLATOR_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Initialize the internal oscillator to 24 MHz
//
//-----------------------------------------------------------------------------
void OSCILLATOR_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE = CONFIG_PAGE;

   OSCICN = 0x87;                      // Set internal oscillator divider to 1

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P0.6   digital  push-pull        CAN TX
// P0.7   digital  open-drain       CAN RX
//
// P1.3   digital  push-pull        LED
//
//-----------------------------------------------------------------------------

void PORT_Init (void)
{
   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CONFIG_PAGE;             // Port SFR's on Configuration page

   P0MDOUT  |= 0x40;                   // P0.6 (CAN0 TX) is push-pull
   P1MDOUT  |= 0x08;                   // P1.3 (LED) is push-pull

   XBR0     = 0x02;                    // Enable CAN0 on Crossbar
   XBR2     = 0x40;                    // Enable Crossbar and weak pull-ups

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// CAN0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the CAN peripheral and all of the message objects
//
// The following settings are chosen for the peripheral and message objects:
//
// CAN Bit Clock : 1 Mbps
// Auto Retransmit : Automatic Retransmission is enabled
// MsgVal        : Set to Valid based on the #define MESSAGE_OBJECTS
// Filtering     : Enabled for all valid message objects
// Message Identifier : 11-bit standard; Each message object is only used by
//                      one message ID and the ID is equal to the object number
// Direction     : All valid message objects are configured for transmit
// End of Buffer : All message objects are treated as separate buffers
//
// The following interrupts are enabled and are handled by CAN0_ISR
//
// Error Interrupts
// Status Change Interrupt
// Transmit Interrupt
//
//-----------------------------------------------------------------------------

void CAN0_Init (void)
{
   U8 iter;

   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CAN0_PAGE;               // All CAN register are on page 0x0C

   CAN0CN |= 0x01;                     // Start Intialization mode

   //---------Initialize general CAN peripheral settings

   CAN0CN |= 0x4E;                     // Enable Status, Error, 
                                       // Module Interrupts
                                       // Enable access to bit timing register

   // See the CAN Bit Timing Spreadsheet for how to calculate this value
   CAN0BT = 0x1402;                    // Based on 24 Mhz CAN clock, set the
                                       // CAN bit rate to 1 Mbps

   //---------Initialize settings common to all message objects

   // Command Mask Register
   CAN0IF1CM = 0x00F0;                 // Write Operation
                                       // Transfer ID Mask, MDir, MXtd
                                       // Transfer ID, Dir, Xtd, MsgVal
                                       // Transfer Control Bits
                                       // Don't set TxRqst or transfer data

   // Mask Registers
   CAN0IF1M1 = 0x0000;                 // Mask Bits 15-0 not used for filtering
   CAN0IF1M2 = 0x5FFC;                 // Ignore Extended Identifier for 
                                       // filtering
                                       // Used Direction bit for filtering
                                       // Use ID bits 28-18 for filtering

   // Arbitration Registers
   CAN0IF1A1 = 0x0000;                 // 11-bit ID, so lower 16-bits not used

   // Message Control Registers
   CAN0IF1MC = 0x0880 | MESSAGE_SIZE;  // Enable Transmit Interrupt
                                       // Message Object is a Single Message
                                       // Message Size set by #define

   //---------Initialize unique settings for each valid message object

   for (iter = 0; iter < MESSAGE_OBJECTS; iter++)
   {
      // For example purposes, set 11-bit identifier based on the message 
      // object that is used to send it.

      // Arbitration Registers
      CAN0IF1A2 = 0xA000 | (iter << 2);  // Set MsgVal to valid
                                         // Set Direction to write
                                         // Set 11-bit Identifier to iter

      CAN0IF1CR = iter;                // Start command request
	  
      while (CAN0IF1CRH & 0x80) {}       // Poll on Busy bit
   }

   //---------Initialize settings for unused message objects

   for (iter = MESSAGE_OBJECTS; iter < 32; iter++)
   {
      // Set remaining message objects to be Ignored
      CAN0IF1A2 = 0x0000;              // Set MsgVal to 0 to Ignore
      CAN0IF1CR = iter;                // Start command request
	  
      while (CAN0IF1CRH & 0x80) {}     // Poll on Busy bit
   }

   CAN0CN &= ~0x41;                    // Return to Normal Mode and disable
                                       // access to bit timing register

   SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// Supporting Subroutines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CAN0_TransferMO
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : U8 obj_num - message object number to send data
//                             range is 0x01 - 0x20
//
// Send data using the message object passed as the parameter. The data
// is generated using the message object number as the seed.
//
//-----------------------------------------------------------------------------

void CAN0_TransferMO (U8 obj_num)
{
   // This function assumes that the message object is fully initialized
   // in CAN0_Init and so all it has to do is fill the data registers and 
   // initiate transmission

   U8 SFRPAGE_save = SFRPAGE;
   SFRPAGE  = CAN0_PAGE;               // All CAN register are on page 0x0C

   // Initialize all 8 data bytes even though they might not be sent
   // The number to send was configured earlier by setting Message Control

   CAN0IF1DA1H = obj_num;              // Initialize data registers based
   CAN0IF1DA1L = obj_num + 1;          // on message object used
   CAN0IF1DA2H = obj_num + 2;
   CAN0IF1DA2L = obj_num + 3;           
   CAN0IF1DB1H = obj_num + 4;
   CAN0IF1DB1L = obj_num + 5;
   CAN0IF1DB2H = obj_num + 6;
   CAN0IF1DB2L = obj_num + 7;

   CAN0IF1CM = 0x0087;                 // Set Direction to Write
                                       // Write TxRqst, all 8 data bytes

   CAN0IF1CR = obj_num;                // Start command request
	  
   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit
}


//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CAN0_ISR
//-----------------------------------------------------------------------------
//
// The ISR is triggered upon any CAN errors or upon a complete transmission
//
// If an error occurs, a global counter is updated
//
//-----------------------------------------------------------------------------

INTERRUPT (CAN0_ISR, INTERRUPT_CAN0)
{
   // SFRPAGE is set to CAN0_Page automatically when ISR starts

   U8 status = CAN0STAT;               // Read status, which clears the Status
                                       // Interrupt bit pending in CAN0IID

   U8 Interrupt_ID = CAN0IID;          // Read which message object caused
                                       // the interrupt

   CAN0IF1CM = 0x0008;                 // Set Command Mask to clear pending 
                                       // interrupt for the message object


   CAN0IF1CR = Interrupt_ID;           // Start command request to actually 
                                       // clear the interrupt
	  
   while (CAN0IF1CRH & 0x80) {}        // Poll on Busy bit

   if (status & TxOk)                  // If transmit completed successfully
   {
      // Set variable to indicate this message object's transfer completed
 
      // Message Object 0 reports as 0x20 in the CAN0IID register, so 
      // convert it to 0x00
      if (Interrupt_ID == 0x20) 
      {
         Interrupt_ID = 0x00; 
      }

      // Bit-shifting doesn't work with an operator greater than 15, so
      // account for it 
      if (Interrupt_ID <= 15) 
      {
	      CAN_TX_COMPLETE.U32 |= (U16) (0x01 << Interrupt_ID); 
      }
      else if (Interrupt_ID <= 0x1F)
      {
         CAN_TX_COMPLETE.U16[MSB] |= (U16) (0x01 << (Interrupt_ID - 16)); 
      }
   }

   // If an error occured, simply update the global variable and continue
   if (status & LEC)                 
   {
       // The LEC bits identify the type of error, but those are grouped here
      if ((status & LEC) != 0x07) 
      {
          CAN_ERROR = 1; 
      }
   }

   if (status & BOff)
   {
      CAN_ERROR = 1;
   }

   if (status & EWarn)
   {
      CAN_ERROR = 1;
   }

   // Old SFRPAGE is popped off stack when ISR exits
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------

