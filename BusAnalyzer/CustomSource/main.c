/******************************************************************************
 * @file main.c
 *
 * @brief Main module of bus analyzer application for STM32F103
 *
 * @description
 *
 *   This application has been written for the STM32-H103 header board
 *   developed by Olimex (http://www.olimex.com/dev/stm32-h103.html).
 *   It uses the integrated CAN interface as well as one of the UARTs to
 *   realize a LIN bus interface. LIN and CAN may run concurrently. The
 *   application has to be controlled by a host PC via another UART (interfaced
 *   via USB, see also http://ftdichip.com/Products/FT232R.htm).
 *
 *   (De-)activation as well as configuration of each interface (baud rate, ...)
 *   has to be done by the host PC via a dedicated set of serial commands. If
 *   a bus event occurs (byte/frame received) a packet is assembled and put
 *   into a circular buffer (interrupt context!). A background task is
 *   responsible to manage the packet transmission to the host PC as well as
 *   processing any host PC commands.
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>
#include <stm32f10x.h>
#include <core_cm3.h>
#include "time.h"
#include "communication.h"

/******************************************************************************
 * Function definitions
 *****************************************************************************/

int main (void)
{
    /* Basic RCC and PLL setup */
    SystemInit();

    /* Use 0 bits for preemption priority and 4 bits for sub-priority,
     * this shall disable interrupt nesting */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    Time_initialize();
	Communication_initialize();

	for(;;)
	{
		/* Do host communication (process commands, provide packets) */
		Communication_doBackground();
	}
}
