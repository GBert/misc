/******************************************************************************
 * @file board.c
 *
 * @brief This module handles any CAN specifics.
 *
 * @details Functions are provided to setup a CAN channel with application
 *          specific parameters (baud rate, ...). All event handling is done
 *          using interrupts. A single call-back function per event can be used
 *          by other modules to get notified (observer pattern). All message
 *          types are received, transmission can be done using transmit tasks
 *          or a transmit queue.
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "can.h"
#include "time.h"
#include "interrupt.h"

/******************************************************************************
 * Defines and Macros
 *****************************************************************************/

/** Number of transmit mailboxes */
#define NUMBER_OF_TX_MAILBOXES              3

/** Maximum number of transmit tasks (must be a power of 2: 2^N) */
#define MAXIMUM_NUMBER_OF_TX_TASKS          64

/** Maximum number of queued transmit messages (must be a power of 2: 2^N) */
#define MAXIMUM_NUMBER_OF_QUEUED_MESSAGES   64

/******************************************************************************
 * Structure declarations
 *****************************************************************************/

/** Transmit task structure */
typedef struct {
    CanTxMsg TxMsg;
    uint64_t CycleTimeUs;
    uint64_t LastTriggerTimeUs;
    uint32_t TaskId;
    uint8_t IsUsed;
} TxTask_t;

/** Queued transmit message structure */
typedef struct {
    CanTxMsg TxMsg;
    uint64_t DeltaTime;
} QueuedTxMsg_t;

/** Transmit queue structure */
typedef struct {
    uint64_t LastTriggerTimeUs;
    uint32_t ReadIndex;
    uint32_t WriteIndex;
    QueuedTxMsg_t QueuedMessages[MAXIMUM_NUMBER_OF_QUEUED_MESSAGES];
} TransmitQueue_t;

/******************************************************************************
 * Function declarations
 *****************************************************************************/

static void DefaultCanCbState(CanState_t state);
static void DefaultCanCbError(CanError_t error);
static void DefaultCanCbRxMsg(const CanRxMsg * pMsg);
static void DefaultCanCbTxMsg(const CanTxMsg * pMsg, uint8_t isQueued);

/******************************************************************************
 * Global variables
 *****************************************************************************/

/** Call-back handle for CAN status events */
static CanCbState_t g_pCbStateCan = DefaultCanCbState;

/** Call-back handle for CAN error events */
static CanCbError_t g_pCbErrorCan = DefaultCanCbError;

/** Call-back handle for CAN receive events */
static CanCbRxMsg_t g_pCbRxMsgCan = DefaultCanCbRxMsg;

/** Call-back handle for CAN transmit events */
static CanCbTxMsg_t g_pCbTxMsgCan = DefaultCanCbTxMsg;

/** Buffer for pending transmit messages (used for loop-back emulation) */
static CanTxMsg g_BufferedCanTxMsgs[NUMBER_OF_TX_MAILBOXES];

/** Buffer for pending transmit messages (used for loop-back emulation) */
static uint8_t g_BufferedCanTxMsgIsQueued[NUMBER_OF_TX_MAILBOXES];

/** CAN state */
static CanState_t g_CanState;

/** Preallocated CAN transmit tasks */
static TxTask_t g_TransmitTasks[MAXIMUM_NUMBER_OF_TX_TASKS];

/** Holds last processed transmit task */
static uint32_t g_LastProcessedTransmitTask;

static TransmitQueue_t g_TransmitQueue;

/******************************************************************************
 * Function definitions
 *****************************************************************************/

/******************************************************************************
 * @brief Default CAN state changed call-back function.
 *****************************************************************************/
static void DefaultCanCbState(CanState_t state)
{
    (void)state;
}

/******************************************************************************
 * @brief Default CAN error detected call-back function.
 *****************************************************************************/
static void DefaultCanCbError(CanError_t error)
{
    (void)error;
}

/******************************************************************************
 * @brief Default CAN receive message event call-back function.
 *****************************************************************************/
static void DefaultCanCbRxMsg(const CanRxMsg * pMsg)
{
    (void)pMsg;
}

/******************************************************************************
 * @brief Default CAN transmit message event call-back function.
 *****************************************************************************/
static void DefaultCanCbTxMsg(const CanTxMsg * pMsg, uint8_t isQueued)
{
    (void)pMsg;
    (void)isQueued;
}

/******************************************************************************
 * @brief Calculates the CAN bit timing parameters for a given baud rate
 *        (this will be time consuming).
 *
 * @param baudRate: The desired baud rate.
 * @param pCanInitParams: The returned bit timing parameters.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
static uint8_t CalculateBitTiming(uint32_t baudRate, CAN_InitTypeDef * pCanInitParams)
{
    const uint32_t tseg1Max = 16;
    const uint32_t tseg2Min = 1;
    const uint32_t tseg2Max = 8;
    const uint32_t sjwMax = 4;
    const uint32_t brpMin = 1;
    const uint32_t brpMax = 512;
    const uint32_t baudRateDeviation = baudRate >> 7;	/* BaudRate / 128 => around 1 percent deviation */
    const uint32_t samplePointDesired = 192;	/* 0.75 * 256 (scale to allow usage of fixed point arithmetic) */
    const uint32_t samplePointDeviation = 38;	/* 0.15 * 256 (scale to allow usage of fixed point arithmetic) */
    uint8_t solutionFound = 0;
    uint32_t lastBaudRateError = 0;
    uint32_t lastSamplePointError = 0;
    uint32_t brp;
    uint32_t tseg1;
    uint32_t tseg2;

    for (tseg2 = tseg2Min; tseg2 <= tseg2Max; tseg2++) {
	/* TSEG1 must be at least as large as TSEG2 in order to reach a sample point > 0.5 */
	for (tseg1 = tseg2; tseg1 <= tseg1Max; tseg1++) {
	    uint32_t numTq = tseg1 + tseg2 + 1;	/* Number of time quanta per CAN bit */
	    uint32_t brpEstimate = SystemFrequency_APB1Clk / (numTq * baudRate);
	    uint32_t brpLowerLimit = (brpEstimate > brpMin) ? (brpEstimate - 1) : brpMin;
	    uint32_t brpUpperLimit = (brpEstimate < brpMax) ? (brpEstimate + 1) : brpMax;

	    /* Only useful BRPs within a certain range are tested */
	    for (brp = brpLowerLimit; brp <= brpUpperLimit; brp++) {
		uint32_t calcBaudRate = SystemFrequency_APB1Clk / (numTq * brp);
		uint32_t baudRateError = (calcBaudRate > baudRate) ? (calcBaudRate - baudRate)
		    : (baudRate - calcBaudRate);

		if (baudRateError <= baudRateDeviation) {
		    uint32_t samplePoint = ((tseg1 + 1) << 8) / numTq;	/* SP * 256 (scale to allow usage of fixed point arithmetic) */
		    uint32_t samplePointError = (samplePoint > samplePointDesired) ? (samplePoint - samplePointDesired)
			: samplePointDesired - samplePoint;

		    if (samplePointError <= samplePointDeviation) {
			if ((0 != solutionFound)
			    && ((samplePointError > lastSamplePointError) || (baudRateError > lastBaudRateError))) {
			    /* New solution is not better than old one */
			    continue;
			}

			solutionFound = 1;

			/* New or better solution found */
			pCanInitParams->CAN_Prescaler = (uint16_t) brp;
			pCanInitParams->CAN_BS1 = (uint8_t) (tseg1 - 1);
			pCanInitParams->CAN_BS2 = (uint8_t) (tseg2 - 1);
			pCanInitParams->CAN_SJW = (tseg2 <= sjwMax) ? (tseg2 - 1) : (sjwMax - 1);
			lastBaudRateError = baudRateError;
			lastSamplePointError = samplePointError;
		    }
		}
	    }
	}
    }

    return (0 != solutionFound) ? 0 : 1;
}

/******************************************************************************
 * @brief Initializes CAN hardware peripheral with the given baud rate.
 *
 * @param baudRate: The baud rate to be configured.
 * @param autoBusOffRecovery: The bus-off recovery policy to be used.
 * @param silentMode: Indicates if silent mode is to be used.
 * @param singleShotMode: Indicates if single-shot mode is to be used.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
uint8_t Can_initialize(uint32_t baudRate, uint8_t silentMode, uint8_t singleShotMode, uint8_t busOffRecoveryMode)
{
    GPIO_InitTypeDef ioInitParams;
    CAN_InitTypeDef canInitParams;
    NVIC_InitTypeDef intInitParams;
    CAN_FilterInitTypeDef canFilterInitParams;
    InterruptStatus_t status;
    uint32_t i;

    /* Enable clock for CAN peripheral */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* Enable clock for GPIO used by CAN */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure CANRX (PA.11) */
    ioInitParams.GPIO_Pin = GPIO_Pin_11;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &ioInitParams);

    /* Configure CANTX (PA.12) */
    ioInitParams.GPIO_Pin = GPIO_Pin_12;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &ioInitParams);

    CAN_StructInit(&canInitParams);
    canInitParams.CAN_Mode = (0 != silentMode) ? CAN_Mode_Silent : CAN_Mode_Normal;
    canInitParams.CAN_NART = (0 != singleShotMode) ? ENABLE : DISABLE;
    canInitParams.CAN_ABOM = (0 != busOffRecoveryMode) ? ENABLE : DISABLE;

    if (0 != CalculateBitTiming(baudRate, &canInitParams)) {
	return 1;
    }

    /* Begin of critical section, no CAN interrupts shall fire unless the
     * initialization has been completed. */
    status = Interrupt_saveAndDisable();

    CAN_Init(CAN1, &canInitParams);

    /* Enable CAN transmit mailbox empty interrupt */
    intInitParams.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    intInitParams.NVIC_IRQChannelCmd = ENABLE;
    intInitParams.NVIC_IRQChannelPreemptionPriority = 0;
    intInitParams.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&intInitParams);
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);

    /* Enable CAN receive FIFO0 interrupt */
    intInitParams.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    intInitParams.NVIC_IRQChannelCmd = ENABLE;
    intInitParams.NVIC_IRQChannelPreemptionPriority = 0;
    intInitParams.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&intInitParams);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

    /* Enable CAN status change interrupts */
    intInitParams.NVIC_IRQChannel = CAN1_SCE_IRQn;
    intInitParams.NVIC_IRQChannelCmd = ENABLE;
    intInitParams.NVIC_IRQChannelPreemptionPriority = 0;
    intInitParams.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&intInitParams);
    CAN_ITConfig(CAN1, CAN_IT_EWG | CAN_IT_EPV | CAN_IT_LEC | CAN_IT_ERR, ENABLE);

    /* Configure a single filter that lets any CAN frame pass and assign this
     * filter to FIFO0. FIFO1 is not used. */
    canFilterInitParams.CAN_FilterIdHigh = 0;
    canFilterInitParams.CAN_FilterIdLow = 0;
    canFilterInitParams.CAN_FilterMaskIdHigh = 0;
    canFilterInitParams.CAN_FilterMaskIdLow = 0;
    canFilterInitParams.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
    canFilterInitParams.CAN_FilterNumber = 0;
    canFilterInitParams.CAN_FilterMode = CAN_FilterMode_IdMask;
    canFilterInitParams.CAN_FilterScale = CAN_FilterScale_32bit;
    canFilterInitParams.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&canFilterInitParams);

    g_CanState = CanState_Init;

    for (i = 0; i < MAXIMUM_NUMBER_OF_TX_TASKS; i++) {
	g_TransmitTasks[i].IsUsed = 0;
    }

    g_TransmitQueue.LastTriggerTimeUs = 0;
    g_TransmitQueue.ReadIndex = 0;
    g_TransmitQueue.WriteIndex = 0;

    g_LastProcessedTransmitTask = 0;

    /* End of critical section */
    Interrupt_restore(status);

    return 0;
}

/******************************************************************************
 * @brief Disables the CAN interface.
 *****************************************************************************/
void Can_deinitialize(void)
{
    CAN_DeInit(CAN1);
}

/******************************************************************************
 * @brief Connects call-back to CAN status change event.
 *
 * @param pCb: The call-back function to be connected to the event.
 *****************************************************************************/
void Can_connectCbState(CanCbState_t pCb)
{
    g_pCbStateCan = pCb;
}

/******************************************************************************
 * @brief Connects call-back to CAN error event.
 *
 * @param pCb: The call-back function to be connected to the event.
 *****************************************************************************/
void Can_connectCbError(CanCbError_t pCb)
{
    g_pCbErrorCan = pCb;
}

/******************************************************************************
 * @brief Connects call-back to CAN receive event.
 *
 * @param pCb: The call-back function to be connected to the event.
 *****************************************************************************/
void Can_connectCbRxMsg(CanCbRxMsg_t pCb)
{
    g_pCbRxMsgCan = pCb;
}

/******************************************************************************
 * @brief Connects call-back to CAN transmit event.
 *
 * @param pCb: The call-back function to be connected to the event.
 *****************************************************************************/
void Can_connectCbTxMsg(CanCbTxMsg_t pCb)
{
    g_pCbTxMsgCan = pCb;
}

/******************************************************************************
 * @brief Gets a message from the transmit queue. This function evaluations
 *        the delta time attached to a message. Messages are only dequeued if
 *        the delta time has elapsed.
 *
 * @param pTxMsg: The returned message to be transmitted.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t TransmitMessageDequeue(void)
{
    const uint64_t currentTimeUs = Time_getUs();;
    QueuedTxMsg_t *pQueuedTxMsg;
    uint8_t mailbox;

    if (g_TransmitQueue.WriteIndex == g_TransmitQueue.ReadIndex) {
	/* Nothing to be transmitted */
	return 1;
    }

    pQueuedTxMsg = &g_TransmitQueue.QueuedMessages[g_TransmitQueue.ReadIndex];

    if ((0 != pQueuedTxMsg->DeltaTime)
	&& (pQueuedTxMsg->DeltaTime > (currentTimeUs - g_TransmitQueue.LastTriggerTimeUs))) {
	return 1;
    }

    mailbox = CAN_Transmit(CAN1, &pQueuedTxMsg->TxMsg);

    if (mailbox == CAN_NO_MB) {
	/* No empty mailboxes available */
	return 1;
    }

    g_TransmitQueue.LastTriggerTimeUs = currentTimeUs;
    g_TransmitQueue.ReadIndex = (g_TransmitQueue.ReadIndex + 1) & (MAXIMUM_NUMBER_OF_QUEUED_MESSAGES - 1);

    g_BufferedCanTxMsgs[mailbox] = pQueuedTxMsg->TxMsg;
    g_BufferedCanTxMsgIsQueued[mailbox] = 1;

    return 0;
}

/******************************************************************************
 * @brief Transmit a message using a queue.
 *
 * @param pTxMsg: The message to be transmitted.
 * @param deltaTime: The delta time with reference to any previous queued
 *                   transmit message.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
uint8_t Can_transmitQueued(CanTxMsg * pTxMsg, uint64_t deltaTime)
{
    const uint32_t space = ((g_TransmitQueue.ReadIndex - g_TransmitQueue.WriteIndex - 1)
			    & (MAXIMUM_NUMBER_OF_QUEUED_MESSAGES - 1));

    if (0 == space) {
	return 1;
    }

    g_TransmitQueue.QueuedMessages[g_TransmitQueue.WriteIndex].TxMsg = *pTxMsg;
    g_TransmitQueue.QueuedMessages[g_TransmitQueue.WriteIndex].DeltaTime = deltaTime;
    g_TransmitQueue.WriteIndex = (g_TransmitQueue.WriteIndex + 1) & (MAXIMUM_NUMBER_OF_QUEUED_MESSAGES - 1);

    return 0;
}

/******************************************************************************
 * @brief Adds a transmit task to the CAN schedule.
 *
 * @param pTxMsg: The message to be transmitted.
 * @param cycleTime: The message cycle time in us.
 * @param taskId: An unique transmit task identifier.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
uint8_t Can_addTransmitTask(CanTxMsg * pTxMsg, uint64_t cycleTime, uint32_t taskId)
{
    uint32_t i;
    const uint64_t currentTimeUs = Time_getUs();

    /* Check if task already exists, if, alter its parameters */
    for (i = 0; i < MAXIMUM_NUMBER_OF_TX_TASKS; i++) {
	if ((taskId != g_TransmitTasks[i].TaskId)
	    || (0 == g_TransmitTasks[i].IsUsed)) {
	    continue;
	}

	g_TransmitTasks[i].TxMsg = *pTxMsg;
	g_TransmitTasks[i].CycleTimeUs = cycleTime;
	g_TransmitTasks[i].LastTriggerTimeUs = currentTimeUs;
	return 0;
    }

    /* Add new task to schedule as long as there are empty tasks */
    for (i = 0; i < MAXIMUM_NUMBER_OF_TX_TASKS; i++) {
	if (0 == g_TransmitTasks[i].IsUsed) {
	    break;
	}
    }

    if (MAXIMUM_NUMBER_OF_TX_TASKS == i) {
	return 1;
    }

    g_TransmitTasks[i].TxMsg = *pTxMsg;
    g_TransmitTasks[i].CycleTimeUs = cycleTime;
    g_TransmitTasks[i].LastTriggerTimeUs = currentTimeUs;
    g_TransmitTasks[i].IsUsed = 1;

    return 0;
}

/******************************************************************************
 * @brief Removes a transmit task from the CAN schedule.
 *
 * @param taskId: Task identifier for the task to be removed.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
uint8_t Can_removeTransmitTask(uint32_t taskId)
{
    uint32_t i;

    for (i = 0; i < MAXIMUM_NUMBER_OF_TX_TASKS; i++) {
	if ((0 != g_TransmitTasks[i].IsUsed)
	    && (taskId == g_TransmitTasks[i].TaskId)) {
	    break;
	}
    }

    if (MAXIMUM_NUMBER_OF_TX_TASKS == i) {
	return 1;
    }

    g_TransmitTasks[i].IsUsed = 0;

    return 0;
}

/******************************************************************************
 * @brief Processes the CAN transmit task schedule (triggers messages).
 *****************************************************************************/
static void ProcessTransmitTaskSchedule(void)
{
    const uint64_t currentTimeUs = Time_getUs();
    uint64_t elapsedTimeUs;
    uint32_t index;
    uint32_t i;
    uint8_t mailbox;

    for (i = 0; i < MAXIMUM_NUMBER_OF_TX_TASKS; i++) {
	index = (g_LastProcessedTransmitTask + i) & (MAXIMUM_NUMBER_OF_TX_TASKS - 1);

	if (0 == g_TransmitTasks[index].IsUsed) {
	    continue;
	}

	elapsedTimeUs = currentTimeUs - g_TransmitTasks[index].LastTriggerTimeUs;

	if (elapsedTimeUs < g_TransmitTasks[index].CycleTimeUs) {
	    continue;
	}

	mailbox = CAN_Transmit(CAN1, &g_TransmitTasks[index].TxMsg);

	if (mailbox == CAN_NO_MB) {
	    /* No empty mailboxes available */
	    break;
	}

	g_BufferedCanTxMsgs[mailbox] = g_TransmitTasks[index].TxMsg;
	g_BufferedCanTxMsgIsQueued[mailbox] = 0;

	if (0 == g_TransmitTasks[index].CycleTimeUs) {
	    g_TransmitTasks[index].IsUsed = 0;
	} else {
	    g_TransmitTasks[index].LastTriggerTimeUs += g_TransmitTasks[index].CycleTimeUs;
	}
    }

    g_LastProcessedTransmitTask = index;
}

/******************************************************************************
 * @brief Processes the CAN transmit queue.
 *****************************************************************************/
static void ProcessTransmitQueue(void)
{
    while (0 == TransmitMessageDequeue()) ;
}

/******************************************************************************
 * @brief Updates the bus state (intended to be called from background). The
 *        CAN controller does not provide interrupts for all state transitions
 *        (e.g. init => active, passive => active, bus-off => active).
 *****************************************************************************/
static void CheckBusState(void)
{
    CanState_t state;
    uint32_t errorStatusRegister;
    InterruptStatus_t status;

    /* Begin of critical section */
    status = Interrupt_saveAndDisable();

    /* Check whether CAN state has changed */
    errorStatusRegister = CAN1->ESR;

    if (errorStatusRegister & CAN_FLAG_BOF) {
	state = CanState_BusOff;
    } else if (errorStatusRegister & CAN_FLAG_EPV) {
	state = CanState_Passive;
    } else if (errorStatusRegister & CAN_FLAG_EWG) {
	state = CanState_Warning;
    } else {
	state = CanState_Active;
    }

    if (state != g_CanState) {
	g_CanState = state;

	/* Notify status event listener */
	g_pCbStateCan(state);
    }

    /* End of critical section */
    Interrupt_restore(status);
}

/******************************************************************************
 * @brief Background worker for CAN
 *
 * @note ISR-safety must be ensured by the caller.
 *****************************************************************************/
void Can_doBackground(void)
{
    CheckBusState();

    ProcessTransmitTaskSchedule();

    ProcessTransmitQueue();
}

/******************************************************************************
 * @brief Interrupt service routine for CAN transmit mailbox empty events.
 *****************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
    const uint32_t tsrRqcp0 = 0x00000001UL;	/* Transmit status register: Request completed mailbox0 */
    const uint32_t tsrRqcp1 = 0x00000100UL;	/* Transmit status register: Request completed mailbox1 */
    const uint32_t tsrRqcp2 = 0x00010000UL;	/* Transmit status register: Request completed mailbox2 */
    const uint32_t tsrTxOk0 = 0x00000002UL;	/* Transmit status register: Transmission OK of mailbox0 */
    const uint32_t tsrTxOk1 = 0x00000200UL;	/* Transmit status register: Transmission OK of mailbox1 */
    const uint32_t tsrTxOk2 = 0x00020000UL;	/* Transmit status register: Transmission OK of mailbox2 */
    uint8_t mailbox = 0;
    uint8_t txOk = 0;

    /* Only one transmit interrupt at a time is handled. If multiple transmit
     * interrupts are pending the ISR will be called multiple times.
     * Discarded messages (single shot mode) are not provided to any event listener. */
    if (RESET != (CAN1->TSR & tsrRqcp0)) {
	txOk = (RESET == (CAN1->TSR & tsrTxOk0)) ? 0 : 1;
	CAN1->TSR = tsrRqcp0;
	mailbox = 0;
    } else if (RESET != (CAN1->TSR & tsrRqcp1)) {
	txOk = (RESET == (CAN1->TSR & tsrTxOk1)) ? 0 : 1;
	CAN1->TSR = tsrRqcp1;
	mailbox = 1;
    } else if (RESET != (CAN1->TSR & tsrRqcp2)) {
	txOk = (RESET == (CAN1->TSR & tsrTxOk2)) ? 0 : 1;
	CAN1->TSR = tsrRqcp2;
	mailbox = 2;
    }

    if (0 != txOk) {
	/* Notify transmit event listener */
	g_pCbTxMsgCan(&g_BufferedCanTxMsgs[mailbox], g_BufferedCanTxMsgIsQueued[mailbox]);
    }
}

/******************************************************************************
 * @brief Interrupt service routine for CAN status changed and error events.
 *****************************************************************************/
void CAN1_SCE_IRQHandler(void)
{
    /* Master status register: Error interrupt */
    const uint32_t msrErri = 0x00000004;
    const uint32_t errorBitMask = 0x07;
    const uint32_t errorBitsPosition = 4;
    CanError_t error;

    CheckBusState();

    if (RESET != (CAN1->MSR & msrErri)) {
	error = (CanError_t) ((CAN1->ESR >> errorBitsPosition) & errorBitMask);

	/* Notify error event listener */
	g_pCbErrorCan(error);

	/* Clear pending interrupt (w1c) */
	CAN1->MSR = msrErri;
    }
}

/******************************************************************************
 * @brief Interrupt service routine for CAN receive FIFO0 events.
 *****************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMsg;

    CAN_Receive(CAN1, CAN_FIFO0, &RxMsg);
    CAN_FIFORelease(CAN1, CAN_FIFO0);

    /* Notify receive event listener */
    g_pCbRxMsgCan(&RxMsg);
}
