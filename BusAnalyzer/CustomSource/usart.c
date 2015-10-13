/******************************************************************************
 * @file board.c
 *
 * @brief This module handles any USART specifics.
 *
 * @details All USART communication is done using USART2 and DMA. DMA is
 *          required to keep the interrupt frequency low (caused by high
 *          baud rate).
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "usart.h"
#include "time.h"
#include "interrupt.h"

/******************************************************************************
 * Defines and Macros
 *****************************************************************************/

/** Baud rate used for host communication via USARTx */
#define USART_BAUD_RATE                         2250000UL

/** Size of transmit buffer for serial communication */
#define USART_BUFFER_SIZE                       64

/** DMA channel used for serial data transmission */
#define USART_TX_DMA_CHANNEL                    DMA1_Channel7

/** DMA channel used for serial data reception */
#define USART_RX_DMA_CHANNEL                    DMA1_Channel6

/******************************************************************************
 * Function declarations
 *****************************************************************************/

static void DefaultUsartCbRxData(const uint8_t* pData, uint32_t length);

/******************************************************************************
 * Global variables
 *****************************************************************************/

/** Call-back handle for serial data received events */
static UsartCbRxData g_pCbSerialData = DefaultUsartCbRxData;

/** Transmission pending flag */
static uint8_t g_SerialDataTxPending = 0;

/** Read index for receive buffer */
static uint32_t g_SerialDataRxIndex = 0;

/** Transmit buffer for serial data (DMA) */
static uint8_t g_SerialDataTxBuffer[USART_BUFFER_SIZE];

/** Receive buffer for serial data (DMA) */
static uint8_t g_SerialDataRxBuffer[USART_BUFFER_SIZE];

/******************************************************************************
 * Function definitions
 *****************************************************************************/

/******************************************************************************
 * @brief Default Serial data available call-back function.
 *****************************************************************************/
static void DefaultUsartCbRxData(const uint8_t* pData, uint32_t length)
{
    (void)pData;
    (void)length;
}

/******************************************************************************
 * @brief Initializes USART2 hardware peripheral.
 *****************************************************************************/
void Usart_initialize(void)
{
    GPIO_InitTypeDef ioInitParams;
    USART_InitTypeDef uartInitParams;
    NVIC_InitTypeDef intInitParams;
    DMA_InitTypeDef dmaInitParams;

    /* Enable clock for USART2 peripheral */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Enable clock for GPIO used by USART2 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /* Configure USART2_TX (PA.2) as alternate function push-pull */
    ioInitParams.GPIO_Pin = GPIO_Pin_2;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &ioInitParams);

    /* Configure USART2_RX (PA.3) as input floating */
    ioInitParams.GPIO_Pin = GPIO_Pin_3;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &ioInitParams);

    /* Configure USART2_CTS (PA.0) as input floating */
    ioInitParams.GPIO_Pin = GPIO_Pin_0;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &ioInitParams);

    /* Configure USART2_RTS (PA.1) as alternate function push-pull */
    ioInitParams.GPIO_Pin = GPIO_Pin_1;
    ioInitParams.GPIO_Speed = GPIO_Speed_50MHz;
    ioInitParams.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &ioInitParams);

    /* Initialize USART peripheral (8N1) */
    USART_StructInit(&uartInitParams);
    uartInitParams.USART_BaudRate = USART_BAUD_RATE;
    uartInitParams.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS | USART_HardwareFlowControl_CTS;
    uartInitParams.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uartInitParams.USART_Parity = USART_Parity_No;
    uartInitParams.USART_StopBits = USART_StopBits_1;
    uartInitParams.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &uartInitParams);
    USART_Cmd(USART2, ENABLE);

    /* Enable clock for DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Do basic (fixed) configuration of transmit DMA */
    dmaInitParams.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    dmaInitParams.DMA_MemoryBaseAddr = (uint32_t)g_SerialDataTxBuffer;
    dmaInitParams.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaInitParams.DMA_BufferSize = 0;
    dmaInitParams.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaInitParams.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaInitParams.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dmaInitParams.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaInitParams.DMA_Mode = DMA_Mode_Normal;
    dmaInitParams.DMA_Priority = DMA_Priority_Medium;
    dmaInitParams.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(USART_TX_DMA_CHANNEL, &dmaInitParams);

    /* Do basic (fixed) configuration of receive DMA */
    dmaInitParams.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
    dmaInitParams.DMA_MemoryBaseAddr = (uint32_t)g_SerialDataRxBuffer;
    dmaInitParams.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaInitParams.DMA_BufferSize = USART_BUFFER_SIZE;
    dmaInitParams.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaInitParams.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaInitParams.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dmaInitParams.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaInitParams.DMA_Mode = DMA_Mode_Circular;
    dmaInitParams.DMA_Priority = DMA_Priority_Medium;
    dmaInitParams.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(USART_RX_DMA_CHANNEL, &dmaInitParams);
    DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);

    /* Enable DMA interrupts */
    DMA_ITConfig(USART_TX_DMA_CHANNEL, DMA_IT_TC, ENABLE);
    DMA_ITConfig(USART_RX_DMA_CHANNEL, DMA_IT_TC | DMA_IT_HT, ENABLE);

    /* Advise USART2 to use DMA */
    USART_DMACmd(USART2, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

    /* Enable DMA1_Channel6 interrupt */
    intInitParams.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    intInitParams.NVIC_IRQChannelCmd = ENABLE;
    intInitParams.NVIC_IRQChannelPreemptionPriority = 0;
    intInitParams.NVIC_IRQChannelSubPriority = 14;
    NVIC_Init(&intInitParams);

    /* Enable DMA1_Channel7 interrupt */
    intInitParams.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    intInitParams.NVIC_IRQChannelCmd = ENABLE;
    intInitParams.NVIC_IRQChannelPreemptionPriority = 0;
    intInitParams.NVIC_IRQChannelSubPriority = 15;
    NVIC_Init(&intInitParams);
}

/******************************************************************************
 * @brief Connects call-back to serial data received event.
 *
 * @param pCb: The call-back function to be connected to the event.
 *****************************************************************************/
void Usart_connectCbRxData(UsartCbRxData pCb)
{
    g_pCbSerialData = pCb;
}

/******************************************************************************
 * @brief Background worker for serial communication, immediately processes
 *        any received data.
 *****************************************************************************/
void Usart_doBackground(void)
{
    uint32_t rxIndex;
    int32_t length;
    InterruptStatus_t status;

    /* Begin of critical section */
    status = Interrupt_saveAndDisable();

    rxIndex = USART_BUFFER_SIZE - USART_RX_DMA_CHANNEL->CNDTR;

    length = (int32_t)rxIndex - (int32_t)g_SerialDataRxIndex;

    if (0 < length)
    {
        /* Data available and no wrap around */
        g_pCbSerialData(&g_SerialDataRxBuffer[g_SerialDataRxIndex], (uint32_t)length);
        g_SerialDataRxIndex = rxIndex;
    }

    /* End of critical section */
    Interrupt_restore(status);
}

/******************************************************************************
 * @brief Transmits the given number of data bytes using the serial interface.
 *
 * @param pData: Pointer to data to be transmitted
 * @param length: Number of data bytes to be transmitted
 *
 * @retval The number of bytes transmitted.
 *****************************************************************************/
uint32_t Usart_transmit(uint8_t* pData, uint32_t length)
{
    uint32_t i;
    InterruptStatus_t status;

    /* Begin of critical section */
    status = Interrupt_saveAndDisable();

    if (0 != g_SerialDataTxPending)
    {
        /* End of critical section */
    	Interrupt_restore(status);
        return 0;
    }

    length = (length > USART_BUFFER_SIZE) ? USART_BUFFER_SIZE
                                                 : length;

    /* Copy data in dedicated transmit buffer */
    for (i = 0; i < length; i++)
    {
        g_SerialDataTxBuffer[i] = pData[i];
    }

    DMA_Cmd(USART_TX_DMA_CHANNEL, DISABLE);
    USART_TX_DMA_CHANNEL->CNDTR = length;
    DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE);

    g_SerialDataTxPending = 1;

    /* End of critical section */
    Interrupt_restore(status);

    return length;
}

/******************************************************************************
 * @brief Interrupt service routine for USART2 DMA TX.
 *****************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
    if (SET == DMA_GetITStatus(DMA1_FLAG_TC7))
    {
        /* Transfer complete interrupt */
        g_SerialDataTxPending = 0;
        DMA_ClearITPendingBit(DMA1_FLAG_TC7);
    }
}

/******************************************************************************
 * @brief Interrupt service routine for USART2 DMA RX.
 *****************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
    uint32_t rxIndex;
    int32_t length;

    if (SET == DMA_GetITStatus(DMA1_FLAG_HT6))
    {
        /* Half transfer interrupt */
        DMA_ClearITPendingBit(DMA1_FLAG_HT6);
    }

    if (SET == DMA_GetITStatus(DMA1_FLAG_TC6))
    {
        /* Transfer complete interrupt */
        DMA_ClearITPendingBit(DMA1_FLAG_TC6);
    }

    rxIndex = USART_BUFFER_SIZE - USART_RX_DMA_CHANNEL->CNDTR;

    length = (int32_t)rxIndex - (int32_t)g_SerialDataRxIndex;

    if (0 < length)
    {
        /* Data available and no wrap around */
        g_pCbSerialData(&g_SerialDataRxBuffer[g_SerialDataRxIndex], (uint32_t)length);
    }
    else
    {
        /* Data available and wrap around */
        g_pCbSerialData(&g_SerialDataRxBuffer[g_SerialDataRxIndex],
                        USART_BUFFER_SIZE - g_SerialDataRxIndex);
        g_pCbSerialData(&g_SerialDataRxBuffer[0], rxIndex);
    }

    g_SerialDataRxIndex = rxIndex;
}
