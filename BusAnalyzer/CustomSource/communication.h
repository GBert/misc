/******************************************************************************
 * @file communication.h
 *
 * @brief This module handles any host communication related issues, i.e.
 *        parsing and executing commands received via serial interface and
 *        forwarding of hardware events.
 *
 * @author Alex
 *****************************************************************************/

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

/******************************************************************************
 * Enum declarations
 *****************************************************************************/

typedef enum
{
	StatusMessage_Info,
	StatusMessage_Warning,
	StatusMessage_Error
} StatusMessage_t;

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void Communication_initialize(void);
void Communication_transmitStatusMessage(StatusMessage_t type, const char* pMessage);
void Communication_doBackground(void);

#endif /* COMMUNICATION_H_ */
