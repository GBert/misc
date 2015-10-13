/******************************************************************************
 * @file lin.h
 *
 * @brief This module handles any LIN specifics. For details refer to the
 *        modules implementation.
 *
 * @author Alex
 *****************************************************************************/

#ifndef LIN_H_
#define LIN_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>

/******************************************************************************
 * Enumeration declarations
 *****************************************************************************/

/** LIN bus states */
typedef enum
{
    LinState_Init = 0,
    LinState_Awake,
    LintState_Sleep
} LinState_t;

/** LIN bus errors */
typedef enum
{
    LinError_Framing = 1,
    LinError_Noise,
    LinError_Parity
} LinError_t;

/******************************************************************************
 * Function pointer (call-back) declarations
 *****************************************************************************/

typedef void (*LinCbState_t)  (LinState_t state);
typedef void (*LinCbError_t)  (LinError_t error);
typedef void (*LinCbRxByte_t) (uint8_t rxByte);
typedef void (*LinCbTxByte_t) (uint8_t txByte);

/******************************************************************************
 * Function declarations
 *****************************************************************************/

uint8_t Lin_initialize(uint32_t baudRate);
void Lin_deinitialize(void);
uint8_t Lin_removeTransmitTask(uint32_t taskId);
void Lin_doBackground(void);

#endif /* LIN_H_ */
