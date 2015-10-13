/******************************************************************************
 * @file interrupt.h
 *
 * @brief This module handles any interrupt specifics.
 *
 * @author Alex
 *****************************************************************************/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>
#include <stm32f10x.h>
#include <core_cm3.h>

/******************************************************************************
 * Type definitions
 *****************************************************************************/

/** Type for holding interrupt status */
typedef uint32_t InterruptStatus_t;

/******************************************************************************
 * Function definitions
 *****************************************************************************/

/** Globally enables interrupts if they were enabled */
static inline void Interrupt_restore(InterruptStatus_t status)
{
    __set_PRIMASK(status & 0x01);
}

/** Globally disables interrupts if they are not already disabled */
static inline InterruptStatus_t Interrupt_saveAndDisable(void)
{
	InterruptStatus_t status;

    status = __get_PRIMASK();
    __set_PRIMASK(status | 0x01);

    return status;
}

#endif /* INTERRUPT_H_ */
