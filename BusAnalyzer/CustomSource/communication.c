/******************************************************************************
 * @file communication.c
 *
 * @brief This module handles any host communication related issues, i.e.
 *        parsing and executing commands received via serial interface and
 *        forwarding of hardware events.
 *
 * @author Alex
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "communication.h"
#include "time.h"
#include "interrupt.h"
#include "usart.h"
#include "can.h"
#include "lin.h"
#include <string.h>

/******************************************************************************
 * Defines and Macros
 *****************************************************************************/

/** Communication buffer size (must be a power of 2, i.e. 2^N), the same size
 *  is used for transmission and reception. */
#define COMMUNICATON_BUFFER_SIZE            512

/******************************************************************************
 * Structure declarations
 *****************************************************************************/

/** Communication circular buffer structure */
typedef struct
{
	uint32_t ReadIndex;
	uint32_t WriteIndex;
	uint8_t Data[COMMUNICATON_BUFFER_SIZE];
	uint8_t Overrun;
} CommunicationBuffer_t;

/******************************************************************************
 * Enumeration declarations
 *****************************************************************************/

/** Host command types (5 bit = 32 different commands) */
typedef enum
{
	Configure,              /**< Set bus type, baud rate, ... */
	TransmitQueued,         /**< Transmit packet via queue */
	AddTransmitTask,        /**< Transmit packet (periodically, ...) */
	RemoveTransmitTask      /**< Stop transmission of packet */
} Command_t;

/** Hardware response types (5 bit = 32 different responses) */
typedef enum
{
	Acknowledge,            /**< Positive response for any command except TransmitQueued */
	NotAcknowledge,         /**< Negative response for any command except TransmitQueued */
	ReceivePacket,          /**< Bus packet received or transmitted */
	ReceivePacketQueued,    /**< Bus packet via queue transmitted */
	ReceiveState,           /**< Bus state changed */
	ReceiveError,           /**< Bus error detected */
	HeartBeat,              /**< Heart beat signal */
	StatusMessage           /**< Status message */
} Response_t;

/** Supported bus types (3 bit = 8 different bus types) */
typedef enum
{
	None,
	Can,
	Lin
} Bus_t;

/** Direction */
typedef enum
{
	Transmit,
	Receive
} Direction_t;

/******************************************************************************
 * Global variables
 *****************************************************************************/

/** Look-up table for CRC calculation */
static const uint8_t g_CrcTable[] = {
	0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
	0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
	0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
	0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
	0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
	0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
	0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
	0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
	0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
	0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
	0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
	0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
	0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
	0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
	0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
	0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
	0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
	0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
	0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
	0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
	0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
	0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
	0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
	0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
	0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
	0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
	0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
	0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
	0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
	0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
	0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
	0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

/** Preamble bit pattern used for communication */
static const uint8_t g_DataStreamPreamble = 0xA0;

/** Bit offset of packet type (command, response) */
static const uint32_t g_PacketTypeByteBitOffset = 3;

/** Communication receive buffer */
static CommunicationBuffer_t g_RxBuffer;

/** Communication transmit buffer */
static CommunicationBuffer_t g_TxBuffer;

/** Current bus type */
static Bus_t g_BusType = None;

/******************************************************************************
 * Function definitions
 *****************************************************************************/

/******************************************************************************
 * @brief Calculates the number of free bytes inside a buffer.
 *
 * @param pBuffer: Pointer to the buffer.
 *
 * @retval The number of free bytes inside the given buffer.
 *****************************************************************************/
static inline uint32_t CalculateBufferSpace(CommunicationBuffer_t* pBuffer)
{
	return ((pBuffer->ReadIndex - pBuffer->WriteIndex - 1) & (COMMUNICATON_BUFFER_SIZE - 1));
}

/******************************************************************************
 * @brief Calculates a communication buffer index (including wrap-around).
 *
 * @param index: The index to be modified.
 * @param offset: Offset to be used.
 *
 * @retval The modified index.
 *****************************************************************************/
static inline uint32_t CalculateIndex(uint32_t index, uint32_t offset)
{
	return ((index + offset) & (COMMUNICATON_BUFFER_SIZE - 1));
}

/******************************************************************************
 * @brief Adds the given number of bytes to a circular buffer.
 *
 * @param pBuffer: The buffer to be used.
 * @param pData: Pointer to data bytes to be buffered.
 * @param length: Number of data bytes to be buffered.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t AddDataToBuffer(CommunicationBuffer_t* pBuffer, const uint8_t* pData, uint32_t length)
{
    const uint32_t space = CalculateBufferSpace(pBuffer);
    uint32_t i;

    if ((0 != pBuffer->Overrun) || (space < length))
    {
        /* Not enough room in buffer or a not handled overrun exists */
    	pBuffer->Overrun = 1;
        return 1;
    }

	/* Use slow copy algorithm */
	for (i = 0; i < length; i++)
	{
		pBuffer->Data[pBuffer->WriteIndex] = pData[i];
		pBuffer->WriteIndex = CalculateIndex(pBuffer->WriteIndex, 1);
	}

    return 0;
}

/******************************************************************************
 * @brief Calculates a CRC (of type CCITT8)
 *
 * @param pData: Pointer to data bytes to be used for calculation.
 * @param length: Number of data bytes to be used.
 *
 * @retval The calculated CRC.
 *****************************************************************************/
static uint8_t CalculateCrc(uint8_t* pData, uint32_t length)
{
	uint8_t crc = 0;
	uint32_t i;

	for (i = 0; i < length; i++)
	{
		crc = g_CrcTable[(uint8_t)(crc ^ pData[i])];
	}

	return crc;
}

/******************************************************************************
 * @brief Serializes the current time into 5 bytes and stores them at the
 *        right position inside a response packet
 *
 * @param pData: Pointer to storage for serialized time.
 *****************************************************************************/
static void SerializeTime(uint8_t* pData)
{
	const uint32_t timeBytesOffset = 2;
	const uint64_t time = Time_getUs();

	pData[timeBytesOffset + 0] = (uint8_t)(time >> 0);
	pData[timeBytesOffset + 1] = (uint8_t)(time >> 8);
	pData[timeBytesOffset + 2] = (uint8_t)(time >> 16);
	pData[timeBytesOffset + 3] = (uint8_t)(time >> 24);
	pData[timeBytesOffset + 4] = (uint8_t)(time >> 32);
}

/******************************************************************************
 * @brief Serial data available call-back function. Gets invoked whenever new
 *        serial data has been received.
 *
 * @param pData: Pointer to received serial data.
 * @param length: Number of received bytes.
 *
 * @note No need to ensure ISR-safety, call-backs are always invoked from
 *       within ISR-safe context (in ISR or with disabled interrupts).
 *****************************************************************************/
static void SerialReceiveDataAvailableCallback(const uint8_t* pData, uint32_t length)
{
	AddDataToBuffer(&g_RxBuffer, pData, length);
}

/******************************************************************************
 * @brief CAN state changed call-back function. Gets invoked whenever the CAN
 *        interface is in use and its state changes.
 *
 * @param state: The new CAN state.
 *
 * @note No need to ensure ISR-safety, call-backs are always invoked from
 *       within ISR-safe context (in ISR or with disabled interrupts).
 *****************************************************************************/
static void CanStateChangedCallback(CanState_t state)
{
	const uint32_t canStateResponseLength = 9;
	uint8_t data[canStateResponseLength];

	data[0] = (uint8_t)(g_DataStreamPreamble | canStateResponseLength);
	data[1] = (uint8_t)(ReceiveState << g_PacketTypeByteBitOffset | Can);
	SerializeTime(data);
	data[7] = state;
	data[8] = CalculateCrc(data, canStateResponseLength - 1);

	AddDataToBuffer(&g_TxBuffer, data, canStateResponseLength);
}

/******************************************************************************
 * @brief CAN error detected call-back function. Gets invoked whenever the CAN
 *        interface is in use and a bus error is detected.
 *
 * @param error: The detected bus error type.
 *
 * @note No need to ensure ISR-safety, call-backs are always invoked from
 *       within ISR-safe context (in ISR or with disabled interrupts).
 *****************************************************************************/
static void CanErrorCallback(CanError_t error)
{
	const uint32_t canErrorResponseLength = 9;
	uint8_t data[canErrorResponseLength];

	data[0] = (uint8_t)(g_DataStreamPreamble | canErrorResponseLength);
	data[1] = (uint8_t)(ReceiveError << g_PacketTypeByteBitOffset | Can);
	SerializeTime(data);
	data[7] = error;
	data[8] = CalculateCrc(data, canErrorResponseLength - 1);

	AddDataToBuffer(&g_TxBuffer, data, canErrorResponseLength);
}

/******************************************************************************
 * @brief Helper function that handles all message events (used by call-backs).
 *
 * @param pMsg: The message that triggered the event.
 * @param dir: The message direction (transmit, receive).
 * @param type: The response type to be used.
 *****************************************************************************/
static void HandleCanMessage(const CanRxMsg* pMsg, Direction_t dir, Response_t type)
{
	const uint32_t canMsgResponseBaseLength = 10;
	const uint32_t canMsgExtIdAdditionalBytes = 2;
	const uint32_t identifierBytesOffset = 7;
	const uint32_t canMsgResponseLength = canMsgResponseBaseLength + pMsg->DLC +
										  ((CAN_ID_EXT == pMsg->IDE) ? canMsgExtIdAdditionalBytes : 0);
	uint8_t data[canMsgResponseLength];
	uint32_t identifier;
	uint32_t i;
	uint8_t* pDataBytes;

	data[0] = (uint8_t)(g_DataStreamPreamble | canMsgResponseLength);
	data[1] = (uint8_t)(type << g_PacketTypeByteBitOffset | Can);
	SerializeTime(data);

	if (CAN_ID_STD == pMsg->IDE)
	{
		identifier = pMsg->StdId;
		identifier |= (CAN_RTR_REMOTE == pMsg->RTR) ? (1 << 14) : 0;
		identifier |= (Transmit == dir) ? (1 << 15) : 0;
		data[identifierBytesOffset + 0] = (uint8_t)(identifier >> 8);
		data[identifierBytesOffset + 1] = (uint8_t)(identifier >> 0);
		pDataBytes = &data[identifierBytesOffset + 2];
	}
	else
	{
		identifier = pMsg->ExtId;
		identifier |= (1 << 29);
		identifier |= (CAN_RTR_REMOTE == pMsg->RTR) ? (1 << 30) : 0;
		identifier |= (Transmit == dir) ? (1 << 31) : 0;
		data[identifierBytesOffset + 0] = (uint8_t)(identifier >> 24);
		data[identifierBytesOffset + 1] = (uint8_t)(identifier >> 16);
		data[identifierBytesOffset + 2] = (uint8_t)(identifier >> 8);
		data[identifierBytesOffset + 3] = (uint8_t)(identifier >> 0);
		pDataBytes = &data[identifierBytesOffset + 4];
	}

	for (i = 0; i < pMsg->DLC; i++)
	{
		*pDataBytes = pMsg->Data[i];
		pDataBytes++;
	}

	data[canMsgResponseLength - 1] = CalculateCrc(data, canMsgResponseLength - 1);

	AddDataToBuffer(&g_TxBuffer, data, canMsgResponseLength);
}

/******************************************************************************
 * @brief CAN receive message event call-back function. Gets invoked whenever
 *        the CAN interface is in use and a message is been received.
 *
 * @param pRxMsg: The received message.
 *
 * @note No need to ensure ISR-safety, call-backs are always invoked from
 *       within ISR-safe context (in ISR or with disabled interrupts).
 *****************************************************************************/
static void CanReceiveMessageCallback(const CanRxMsg* pRxMsg)
{
	HandleCanMessage(pRxMsg, Receive, ReceivePacket);
}

/******************************************************************************
 * @brief CAN transmit message event call-back function. Gets invoked whenever
 *        the CAN interface is in use and a message has been successfully
 *        transmitted.
 *
 * @param pTxMsg: The transmitted message.

 * @note No need to ensure ISR-safety, call-backs are always invoked from
 *       within ISR-safe context (in ISR or with disabled interrupts).
 *****************************************************************************/
static void CanTransmitMessageCallback(const CanTxMsg* pTxMsg, uint8_t isQueued)
{
	const Response_t responseType = (0 == isQueued) ? ReceivePacket : ReceivePacketQueued;

	/* Dirty HACK: The fact that CanRxMsg and CanTxMsg have essentially
	 *             the same layout is used to make a valid type-cast. */
	HandleCanMessage((CanRxMsg*)pTxMsg, Transmit, responseType);
}

/******************************************************************************
 * @brief Adds a heart-beat packet to the transmit buffer.
 *****************************************************************************/
void Communication_transmitStatusMessage(StatusMessage_t type, const char* pMessage)
{
	const uint32_t statusMessageResponseBaseLength = 4;
	const uint32_t maxPacketSize = 0x1F;
	const uint32_t maxMessageLength = maxPacketSize - statusMessageResponseBaseLength;
	const uint32_t messageLength = strnlen(pMessage, maxMessageLength);
	const uint32_t statusMessageResponseLength =statusMessageResponseBaseLength + messageLength;
	InterruptStatus_t status;
	uint8_t data[statusMessageResponseLength];

	data[0] = g_DataStreamPreamble | statusMessageResponseLength;
	data[1] = StatusMessage << g_PacketTypeByteBitOffset;
	data[2] = (uint8_t)type;

	strncpy((char*)&data[3], pMessage, messageLength);

	data[statusMessageResponseLength - 1] = CalculateCrc(data, statusMessageResponseLength - 1);

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	AddDataToBuffer(&g_TxBuffer, data, statusMessageResponseLength);

	/* End of critical section */
	Interrupt_restore(status);
}

/******************************************************************************
 * @brief Adds an acknowledge packet to the transmit buffer.
 *
 * @param checksum: The checksum of the command to be acknowledged.
 *****************************************************************************/
static void TransmitAcknowledge(uint8_t checksum)
{
	const uint32_t ackResponseLength = 4;
	InterruptStatus_t status;
	uint8_t data[ackResponseLength];

	data[0] = g_DataStreamPreamble | ackResponseLength;
	data[1] = Acknowledge << g_PacketTypeByteBitOffset;
	data[2] = checksum;
	data[3] = CalculateCrc(data, ackResponseLength - 1);

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	AddDataToBuffer(&g_TxBuffer, data, ackResponseLength);

	/* End of critical section */
	Interrupt_restore(status);
}

/******************************************************************************
 * @brief Adds a negative acknowledge packet to the transmit buffer.
 *
 * @param checksum: The checksum of the command to be not acknowledged.
 *****************************************************************************/
static void TransmitNotAcknowledge(uint8_t checksum)
{
	const uint32_t nackResponseLength = 4;
	InterruptStatus_t status;
	uint8_t data[nackResponseLength];

	data[0] = g_DataStreamPreamble | nackResponseLength;
	data[1] = NotAcknowledge << g_PacketTypeByteBitOffset;
	data[2] = checksum;
	data[3] = CalculateCrc(data, nackResponseLength - 1);

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	AddDataToBuffer(&g_TxBuffer, data, nackResponseLength);

	/* End of critical section */
	Interrupt_restore(status);
}

/******************************************************************************
 * @brief Adds a heart-beat packet to the transmit buffer.
 *****************************************************************************/
static void TransmitHeartBeat(void)
{
	const uint32_t heartBeatResponseLength = 5;
	const uint8_t majorVersion = 0x01;
	const uint8_t minorVersion = 0x00;
	InterruptStatus_t status;
	uint8_t data[heartBeatResponseLength];

	data[0] = g_DataStreamPreamble | heartBeatResponseLength;
	data[1] = HeartBeat << g_PacketTypeByteBitOffset;
	data[2] = majorVersion;
	data[3] = minorVersion;
	data[4] = CalculateCrc(data, heartBeatResponseLength - 1);

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	AddDataToBuffer(&g_TxBuffer, data, heartBeatResponseLength);

	/* End of critical section */
	Interrupt_restore(status);
}

/******************************************************************************
 * @brief Parses the command type of a given packet.
 *
 * @param pData: Pointer to data the packet.
 *
 * @retval The parsed CommandType.
 *****************************************************************************/
static Command_t ParseCommandType(uint8_t* pData)
{
	const uint32_t cmdBytePosition = 1;
	const uint32_t cmdByteMask = 0xF8;

	return ((pData[cmdBytePosition] & cmdByteMask) >> g_PacketTypeByteBitOffset);
}

/******************************************************************************
 * @brief Parses the bus tpye of a given configuration command.
 *
 * @param pData: Pointer to data the packet.
 *
 * @retval The parsed bus type.
 *****************************************************************************/
static Bus_t ParseBusType(uint8_t* pData)
{
	const uint32_t busTypeBytePosition = 1;
	const uint32_t busTypeByteMask = 0x07;

	return (pData[busTypeBytePosition] & busTypeByteMask);
}

/******************************************************************************
 * @brief Parses the baud rate of a given configuration command (CAN, LIN).
 *
 * @param pData: Pointer to data the packet.
 *
 * @retval The parsed baud rate.
 *****************************************************************************/
static uint32_t ParseBaudRate(uint8_t *pData)
{
	const uint32_t baudRateBytesPos = 2;
	uint32_t baudRate;

	baudRate = ((uint32_t)pData[baudRateBytesPos + 0]) << 0;
	baudRate |= ((uint32_t)pData[baudRateBytesPos + 1]) << 8;
	baudRate |= ((uint32_t)pData[baudRateBytesPos + 2]) << 16;
	baudRate |= ((uint32_t)pData[baudRateBytesPos + 3]) << 24;

	return baudRate;
}

/******************************************************************************
 * @brief Handles the NONE configuration command (disables current bus mode).
 *
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t HandleNoneConfigurationCommand(uint32_t length)
{
	const uint32_t noneCmdLength = 3;

	if (length != noneCmdLength)
	{
		return 1;
	}

	Can_deinitialize();
	Lin_deinitialize();

	return 0;
}

/******************************************************************************
 * @brief Handles the CAN configuration command.
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t HandleCanConfigurationCommand(uint8_t *pData, uint32_t length)
{
	const uint32_t canCmdLength = 8;
	const uint32_t modeBytePosition = 6;
	const uint32_t ModeMask = 0x01;
	const uint32_t silentModeBitPos = 0;
	const uint32_t singleShotModeBitPos = 1;
	const uint32_t busOffRecoveryModeBitPos = 2;
	const uint32_t crcIndex = length - 1;
	uint32_t baudRate;
	uint8_t silentMode;
	uint8_t singleShotMode;
	uint8_t busOffRecoveryMode;
	uint8_t retVal;

	if (length != canCmdLength)
	{
		return 1;
	}

	baudRate = ParseBaudRate(pData);
	silentMode = (pData[modeBytePosition] >> silentModeBitPos) & ModeMask;
	singleShotMode = (pData[modeBytePosition] >> singleShotModeBitPos) & ModeMask;
	busOffRecoveryMode = (pData[modeBytePosition] >> busOffRecoveryModeBitPos) & ModeMask;

	retVal = Can_initialize(baudRate, silentMode, singleShotMode, busOffRecoveryMode);

	if (0 != retVal)
	{
		/* Most likely failed to configure baud rate */
		TransmitNotAcknowledge(pData[crcIndex]);
	}

	return retVal;
}

/******************************************************************************
 * @brief Handles the LIN configuration command.
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t HandleLinConfigurationCommand(uint8_t *pData, uint32_t length)
{
	const uint32_t linCmdLength = 7;
	uint32_t baudRate;

	if (length != linCmdLength)
	{
		return 1;
	}

	baudRate = ParseBaudRate(pData);

	return Lin_initialize(baudRate);
}

/******************************************************************************
 * @brief Handles the configuration command.
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t HandleConfigurationCommand(uint8_t *pData, uint32_t length)
{
	const uint32_t crcIndex = length - 1;
	const Bus_t busType = ParseBusType(pData);
	uint8_t retVal;

	Time_reset();

	switch (busType)
	{
		case None:
		{
			retVal = HandleNoneConfigurationCommand(length);
			break;
		}
		case Can:
		{
			retVal = HandleCanConfigurationCommand(pData, length);
			break;
		}
		case Lin:
		{
			retVal = HandleLinConfigurationCommand(pData, length);
			break;
		}
		default:
		{
			return 1;
		}
	}

	if (0 == retVal)
	{
		g_BusType = busType;
		TransmitAcknowledge(pData[crcIndex]);
	}

	return retVal;
}

/******************************************************************************
 * @brief Parses the time (5 bytes) of a given command into a 64 bit value.
 *
 * @param pData: Pointer to data the packet.
 *
 * @retval The parsed 64 bit timed.
 *****************************************************************************/
static uint64_t ParseTime(uint8_t* pData)
{
	const uint32_t timeBytesPos = 2;
	uint64_t time;

	time = ((uint32_t)pData[timeBytesPos + 0]) << 0;
	time |= ((uint32_t)pData[timeBytesPos + 1]) << 8;
	time |= ((uint32_t)pData[timeBytesPos + 2]) << 16;
	time |= ((uint32_t)pData[timeBytesPos + 3]) << 24;
	time |= ((uint64_t)pData[timeBytesPos + 4]) << 32;

	return time;
}

/******************************************************************************
 * @brief Parses the CAN message of an add transmit task or transmit queued
 *        command (CAN, LIN).
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 * @param pTxMsg: The returned parsed CAN message.
 *
 * @retval The number of bytes parsed.
 *****************************************************************************/
static uint32_t ParseCanTransmitMessage(uint8_t* pData, uint32_t length, uint32_t cmdBaseLength, CanTxMsg* pTxMsg)
{
	const uint32_t identifierBytesPos = 7;
	const uint32_t canMsgExtIdAdditionalBytes = 2;
	const uint32_t canMaxMsgLength = 8;
	const uint32_t stdIdentifierMask = 0x07FF;
	const uint32_t extIdentifierMask = 0x1FFFFFFF;
	const uint32_t ideBitPosition = 5;
	const uint32_t rtrBitPosition = 6;
	uint8_t* pDataBytes;
	uint8_t i;

	if (0 == (pData[identifierBytesPos] & (1 << ideBitPosition)))
	{
		pTxMsg->StdId = ((uint32_t)pData[identifierBytesPos + 1] << 0);
		pTxMsg->StdId |= ((uint32_t)pData[identifierBytesPos] << 8);
		pTxMsg->StdId &= stdIdentifierMask;
		pTxMsg->IDE = CAN_ID_STD;
		pTxMsg->RTR = (0 == (pData[identifierBytesPos] & (1 << rtrBitPosition))) ? CAN_RTR_DATA : CAN_RTR_REMOTE;
		pTxMsg->DLC = length - cmdBaseLength;
		pDataBytes = &pData[identifierBytesPos + 2];
	}
	else
	{
		pTxMsg->ExtId = ((uint32_t)pData[identifierBytesPos + 3] << 0);
		pTxMsg->ExtId |= ((uint32_t)pData[identifierBytesPos + 2] << 8);
		pTxMsg->ExtId |= ((uint32_t)pData[identifierBytesPos + 1] << 16);
		pTxMsg->ExtId |= ((uint32_t)pData[identifierBytesPos + 0] << 24);
		pTxMsg->ExtId &= extIdentifierMask;
		pTxMsg->IDE = CAN_ID_EXT;
		pTxMsg->RTR = (0 == (pData[identifierBytesPos] & (1 << rtrBitPosition))) ? CAN_RTR_DATA : CAN_RTR_REMOTE;
		pTxMsg->DLC = length - cmdBaseLength - canMsgExtIdAdditionalBytes;
		pDataBytes = &pData[identifierBytesPos + 4];
	}

	if (pTxMsg->DLC > canMaxMsgLength)
	{
		return 0;
	}

	for (i = 0; i < pTxMsg->DLC; i++)
	{
		pTxMsg->Data[i] = pDataBytes[i];
	}

	return (pDataBytes - pData);
}

/******************************************************************************
 * @brief Handles the transmit queued command.
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint32_t HandleTransmitQueuedCommand(uint8_t* pData, uint32_t length)
{
	const uint32_t transmitQueuedCmdBaseLength = 10;
	uint64_t deltaTime;
	CanTxMsg txMsg;
	uint32_t parsedBytes;

	deltaTime = ParseTime(pData);

	parsedBytes = ParseCanTransmitMessage(pData, length, transmitQueuedCmdBaseLength, &txMsg);

	if (0 == parsedBytes)
	{
		return 1;
	}

	if (0 != Can_transmitQueued(&txMsg, deltaTime))
	{
		static uint64_t lastTimeUs = 0;
		const uint64_t currentTimeUs = Time_getUs();
		const int64_t elapsedTimeUs = lastTimeUs - currentTimeUs;
		const int64_t maxMessageRateUs = 1000000UL;

		/* The message shall be emitted with a maximum rate of 1 Hz and not on
		 * each occurrence of a transmit queue overflow (which might be quiet
		 * frequent). */
		if (   (elapsedTimeUs < 0)
			|| (elapsedTimeUs > maxMessageRateUs))
		{
			lastTimeUs = currentTimeUs;
			Communication_transmitStatusMessage(StatusMessage_Warning, "HW: Transmit data lost");
		}
	}

	return 0;
}

/******************************************************************************
 * @brief Parses the TaskId of an add transmit task command (CAN, LIN)
 *
 * @param pData: Pointer to data the packet.
 *
 * @retval The parsed TaskId.
 *****************************************************************************/
static uint32_t ParseTaskId(uint8_t *pData)
{
	uint32_t taskId;

	taskId = ((uint32_t)pData[0]) << 0;
	taskId |= ((uint32_t)pData[1]) << 8;
	taskId |= ((uint32_t)pData[2]) << 16;
	taskId |= ((uint32_t)pData[3]) << 24;

	return taskId;
}

/******************************************************************************
 * @brief Handles the add CAN transmit task command
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
static uint8_t HandleAddCanTransmitTaskCommand(uint8_t* pData, uint32_t length)
{
	const uint32_t crcIndex = length - 1;
	const uint32_t addTxTaskCmdBaseLength = 14;
	uint64_t cycleTime;
	CanTxMsg txMsg;
	uint32_t taskId;
	uint32_t parsedBytes;

	cycleTime = ParseTime(pData);

	parsedBytes = ParseCanTransmitMessage(pData, length, addTxTaskCmdBaseLength, &txMsg);

	if (0 == parsedBytes)
	{
		return 1;
	}

	taskId = ParseTaskId(&pData[parsedBytes] + txMsg.DLC);

	if (0 == Can_addTransmitTask(&txMsg, cycleTime, taskId))
	{
		TransmitAcknowledge(pData[crcIndex]);
	}
	else
	{
		TransmitNotAcknowledge(pData[crcIndex]);
	}

	return 0;
}

/******************************************************************************
 * @brief Handles the add LIN transmit task command
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
static uint8_t HandleAddLinTransmitTaskCommand(uint8_t* pData, uint32_t length)
{
	/* TODO */
	(void)pData;
	(void)length;
	return 1;
}

/******************************************************************************
 * @brief Handles the add transmit task command
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
static uint8_t HandleAddTransmitTaskCommand(uint8_t* pData, uint32_t length)
{
	const Bus_t busType = ParseBusType(pData);
	const uint32_t addTxTaskCmdBaseLength = 14;
	uint8_t retVal;

	if (length < addTxTaskCmdBaseLength)
	{
		return 1;
	}

	switch (busType)
	{
		case Can: /* Intentional fall-through */
		{
			retVal = HandleAddCanTransmitTaskCommand(pData, length);
			break;
		}
		case Lin:
		{
			retVal = HandleAddLinTransmitTaskCommand(pData, length);
			break;
		}
		default:
		{
			retVal = 1;
			break;
		}
	}

	return retVal;
}

/******************************************************************************
 * @brief Handles the remove transmit task command
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed
 *****************************************************************************/
static uint8_t HandleRemoveTransmitTaskCommand(uint8_t* pData, uint32_t length)
{
	const uint32_t crcIndex = length - 1;
	const uint32_t removeTxTaskCmdLength = 7;
	const uint32_t taskIdBytesPos = 2;
	const Bus_t busType = ParseBusType(pData);
	uint32_t taskId;
	uint8_t retVal;

	if (length != removeTxTaskCmdLength)
	{
		return 1;
	}

	taskId = ParseTaskId(&pData[taskIdBytesPos]);

	switch (busType)
	{
		case Can: /* Intentional fall-through */
		{
			retVal = Can_removeTransmitTask(taskId);
			break;
		}
		case Lin:
		{
			retVal = Lin_removeTransmitTask(taskId);
			break;
		}
		default:
		{
			retVal = 1;
			break;
		}
	}

	if (0 == retVal)
	{
		TransmitAcknowledge(pData[crcIndex]);
	}
	else
	{
		TransmitNotAcknowledge(pData[crcIndex]);
	}

	return 0;
}

/******************************************************************************
 * @brief Handles all received commands (parsing and execution).
 *
 * @param pData: Pointer to data the packet.
 * @param length: Number of bytes belonging to the command.
 *
 * @retval 0 if succeeded, 1 if failed.
 *****************************************************************************/
static uint8_t HandleCommand(uint8_t* pData, uint32_t length)
{
	uint8_t retVal;

	switch (ParseCommandType(pData))
	{
		case Configure:
		{
			retVal = HandleConfigurationCommand(pData, length);
			break;
		}
		case TransmitQueued:
		{
			retVal = HandleTransmitQueuedCommand(pData, length);
			break;
		}
		case AddTransmitTask:
		{
			retVal = HandleAddTransmitTaskCommand(pData, length);
			break;
		}
		case RemoveTransmitTask:
		{
			retVal = HandleRemoveTransmitTaskCommand(pData, length);
			break;
		}
		default:
		{
			retVal = 0;
			break;
		}
	}

	return retVal;
}

/******************************************************************************
 * @brief Parses the received data for commands.
 *
 * @note This function is called from background only.
 *****************************************************************************/
static void ParseReceiveData(void)
{
	const uint8_t dataLengthMask = 0x1F;
	const uint32_t maxPacketSize = dataLengthMask;
	InterruptStatus_t status;
	uint8_t data[maxPacketSize];
	uint32_t i, j;
	uint32_t length;
	uint32_t space;
	uint32_t readIndex;
	uint32_t writeIndex;
	uint8_t* pData;

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	readIndex = g_RxBuffer.ReadIndex;
	writeIndex = g_RxBuffer.WriteIndex;
	space = COMMUNICATON_BUFFER_SIZE - CalculateBufferSpace(&g_RxBuffer) - 1;

	/* End of critical section */
	Interrupt_restore(status);

	for (i = readIndex; i != writeIndex; i = CalculateIndex(i, 1))
	{
		/* Check for valid preamble */
		if ((g_RxBuffer.Data[i] & g_DataStreamPreamble) != g_DataStreamPreamble)
		{
			/* Continue searching for valid PDUs inside received data */
			continue;
		}

		length = g_RxBuffer.Data[i] & dataLengthMask;

		/* Check if enough bytes available */
		if ((0 == length) || (space < length))
		{
			/* Continue searching for valid PDUs inside received data */
			continue;
		}

		/* Check if command passes buffer end (wrap-around) */
		if ((readIndex + length) < readIndex)
		{
			for (j = 0; j < length; j++)
			{
				data[j] = g_RxBuffer.Data[CalculateIndex(i, j)];
			}

			pData = data;
		}
		else /* No wrap-around */
		{
			pData = &g_RxBuffer.Data[i];
		}

		/* At first control checksum before parsing content */
		if (0 != CalculateCrc(pData, length))
		{
			/* Continue searching for valid PDUs inside received data */
			continue;
		}

		if (0 == HandleCommand(pData, length))
		{
			/* Adjust read pointer by number of bytes just parsed */
			i = CalculateIndex(i, length - 1);

			/* Begin of critical section */
			status = Interrupt_saveAndDisable();

			g_RxBuffer.ReadIndex = CalculateIndex(i, 1);

			/* End of critical section */
			Interrupt_restore(status);
		}
	}
}

/******************************************************************************
 * @brief Triggers the transmission of any data to be transmitted to the host.
 *
 * @note This function is called from background only.
 *****************************************************************************/
static void TriggerTransmitData(void)
{
	InterruptStatus_t status;
	uint32_t length;
	uint32_t bytesTransmitted;
	uint32_t readIndex;
	uint32_t writeIndex;

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	readIndex = g_TxBuffer.ReadIndex;
	writeIndex = g_TxBuffer.WriteIndex;

	/* End of critical section */
	Interrupt_restore(status);

	/* Check if data is available */
	if (readIndex != writeIndex)
	{
		/* If a wrap-around happened transmit only the bytes up to the end of
		 * the buffer. The next call of this function will then transmit the
		 * remaining bytes */
		length = (writeIndex > readIndex) ?
				 (writeIndex - readIndex) :
				 (COMMUNICATON_BUFFER_SIZE - readIndex);

		bytesTransmitted = Usart_transmit(&g_TxBuffer.Data[readIndex], length);

		/* Begin of critical section */
		status = Interrupt_saveAndDisable();

		g_TxBuffer.ReadIndex = CalculateIndex(readIndex, bytesTransmitted);

		/* End of critical section */
		Interrupt_restore(status);
	}
}

/******************************************************************************
 * @brief Triggers the heart-beat signal each second.
 *
 * @note This function is called from background only.
 *****************************************************************************/
static void TriggerHeartBeatSignal(void)
{
	static uint64_t lastTriggerTimeUs = 0;
	const int64_t heartBeatSignalIntervalUs = 1000000;
	uint64_t currentTimeUs = Time_getUs();
	int64_t elapsedTimeUs = (int64_t)currentTimeUs - (int64_t)lastTriggerTimeUs;

	/* Either the interval has elapsed or the time might have been reset */
	if (   (elapsedTimeUs >= heartBeatSignalIntervalUs)
		|| (elapsedTimeUs < 0))
	{
		TransmitHeartBeat();
		lastTriggerTimeUs = currentTimeUs;
	}
}

/******************************************************************************
 * @brief Checks for a communication buffer overrun. Resets the overrun flag
 *        after the buffer has become empty and a status message has been
 *        transmitted.
 *
 * @param pBuffer: The buffer to be used.
 *
 * @note This function is called from background only.
 *****************************************************************************/
static void CheckForBufferOverrun(CommunicationBuffer_t* pBuffer)
{
	InterruptStatus_t status;

	/* Begin of critical section */
	status = Interrupt_saveAndDisable();

	if (   (0 != pBuffer->Overrun)
		&& (pBuffer->ReadIndex == pBuffer->WriteIndex))
	{
		pBuffer->Overrun = 0;
		Communication_transmitStatusMessage(StatusMessage_Warning, "HW: Communication Buffer Overrun");
	}

	/* End of critical section */
	Interrupt_restore(status);
}

/******************************************************************************
 * @brief Initializes the board and communication.
 *****************************************************************************/
void Communication_initialize(void)
{
	g_RxBuffer.ReadIndex = 0;
	g_RxBuffer.WriteIndex = 0;

	g_TxBuffer.ReadIndex = 0;
	g_TxBuffer.WriteIndex = 0;

	g_BusType = None;

    Usart_initialize();

    Usart_connectCbRxData(SerialReceiveDataAvailableCallback);

    Can_connectCbState(CanStateChangedCallback);
    Can_connectCbError(CanErrorCallback);
    Can_connectCbRxMsg(CanReceiveMessageCallback);
    Can_connectCbTxMsg(CanTransmitMessageCallback);
}

/******************************************************************************
 * @brief Polls for commands to be executed and triggers the transmission of
 *        buffered data.
 *****************************************************************************/
void Communication_doBackground(void)
{
	switch (g_BusType)
	{
		case Can: Can_doBackground(); break;
		case Lin: Lin_doBackground(); break;
		default: break;
	}

    Usart_doBackground();
    ParseReceiveData();
    TriggerTransmitData();
    TriggerHeartBeatSignal();
    CheckForBufferOverrun(&g_TxBuffer);
    CheckForBufferOverrun(&g_RxBuffer);
}

