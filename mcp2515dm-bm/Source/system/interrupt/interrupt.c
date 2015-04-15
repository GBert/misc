/******************************************************************************
* PIC USB
* interrupt.c
******************************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\interrupt\interrupt.h"
#include "user\BusMon.h"

/** V A R I A B L E S ********************************************************/

/** I N T E R R U P T  V E C T O R S *****************************************/

#pragma code high_vector=0x08
void interrupt_at_high_vector(void)
{
//  INTCONbits.GIE = 1;
//    _asm goto InterruptHandler _endasm
}
#pragma code

#pragma code low_vector=0x18
void interrupt_at_low_vector(void)
{
//    _asm goto InterruptHandler _endasm
}

/** D E C L A R A T I O N S **************************************************/


#pragma code
/******************************************************************************
 * Function:        void high_isr(void)
 * PreCondition:    None
 * Input:
 * Output:
 * Side Effects:
 * Overview:
 *****************************************************************************/
#pragma interrupt InterruptHandler
void InterruptHandler(void)
{
 INTCON3bits.INT2F = 0;
// LATEbits.LATE1 = !LATEbits.LATE1;
//  INTCONbits.RBIF = 0; //PKR$$$ if this is not included, then it enables when running the code and board will not enumerate.
  INTCONbits.GIE = 1;


//  //Inst. cycle = 200 ns
//  //Interrupts every 51 us
//  if(INTCONbits.TMR0IF)	
//  {
//  	TimerCounter++;
//  	if (!TimerCounter) //if rolled over, set flag. User code will handle the rest.
//  	{
//  	  TimerCounter = 0xFA41;
//  	  gTimeout = 1;
//  	}
//  	INTCONbits.TMR0IF = 0;
//  	
//  }
}

/******************************************************************************
 * Function:        void low_isr(void)
 * PreCondition:    None
 * Input:
 * Output:
 * Side Effects:
 * Overview:
 *****************************************************************************/
#pragma interruptlow low_isr
void low_isr(void)
{
}
#pragma code

/** EOF interrupt.c **********************************************************/
