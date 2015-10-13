/******************************************************************************
 * @file can.h
 *
 * @brief This module handles any CAN specifics. For details refer to the
 *        modules implementation.
 *
 * @author Alex
 *****************************************************************************/

#ifndef CAN_H_
#define CAN_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>
#include <stm32f10x_can.h>

/******************************************************************************
 * Enumeration declarations
 *****************************************************************************/

/** CAN bus states */
typedef enum
{
    CanState_Init = 0,
    CanState_Active,
    CanState_Warning,
    CanState_Passive,
    CanState_BusOff,
    CanState_WakeUp,
    CanState_Sleep
} CanState_t;

/** CAN bus errors */
typedef enum
{
    CanError_Stuff = 1,
    CanError_Form,
    CanError_Ack,
    CanError_Bit1,
    CanError_Bit0,
    CanError_Crc
} CanError_t;

/******************************************************************************
 * Function pointer (call-back) declarations
 *****************************************************************************/

typedef void (*CanCbState_t) (CanState_t state);
typedef void (*CanCbError_t) (CanError_t error);
typedef void (*CanCbRxMsg_t) (const CanRxMsg* pMsg);
typedef void (*CanCbTxMsg_t) (const CanTxMsg* pMsg, uint8_t isQueued);

/******************************************************************************
 * Function declarations
 *****************************************************************************/

uint8_t Can_initialize(uint32_t baudRate, uint8_t silentMode,
                       uint8_t busOffRecoveryMode, uint8_t singleShotMode);
void Can_deinitialize(void);
void Can_connectCbState(CanCbState_t pCb);
void Can_connectCbError(CanCbError_t pCb);
void Can_connectCbRxMsg(CanCbRxMsg_t pCb);
void Can_connectCbTxMsg(CanCbTxMsg_t pCb);
uint8_t Can_transmitQueued(CanTxMsg* pTxMsg, uint64_t deltaTime);
uint8_t Can_addTransmitTask(CanTxMsg* pTxMsg, uint64_t cycleTime, uint32_t taskId);
uint8_t Can_removeTransmitTask(uint32_t taskId);
void Can_doBackground(void);

#endif /* CAN_H_ */
