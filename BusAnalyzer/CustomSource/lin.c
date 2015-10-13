/******************************************************************************
 * @file lin.c
 *
 * @brief This module handles any LIN specifics.
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "lin.h"
#include "time.h"

/******************************************************************************
 * @brief Initializes LIN hardware peripheral with the given baud rate
 *
 * @param baudRate: The baud rate to be configured.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
uint8_t Lin_initialize(uint32_t baudRate)
{
    /* Just to temporarily omit compiler warning (unused parameter) */
    baudRate = 0;
    return 0;
}

/******************************************************************************
 * @brief Disables the LIN interface
 *****************************************************************************/
void Lin_deinitialize(void)
{
}

/******************************************************************************
 * @brief Removes a transmit task from the LIN schedule.
 *
 * @param taskId: Task identifier for the task to be removed.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
uint8_t Lin_removeTransmitTask(uint32_t taskId)
{
    (void)taskId;
    return 0;
}

/******************************************************************************
 * @brief Background worker for LIN
 *
 * @note ISR-safety must be ensured by the caller.
 *****************************************************************************/
void Lin_doBackground(void)
{
}
