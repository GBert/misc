/******************************************************************************
 * @file usart.h
 *
 * @brief This module handles any USART specifics. For details refer to the
 *        modules implementation.
 *
 * @author Alex
 *****************************************************************************/

#ifndef USART_H_
#define USART_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <inttypes.h>

/******************************************************************************
 * Function pointer (call-back) declarations
 *****************************************************************************/

typedef void (*UsartCbRxData) (const uint8_t* pData, uint32_t length);

/******************************************************************************
 * Function declarations
 *****************************************************************************/

void Usart_initialize(void);
void Usart_connectCbRxData(UsartCbRxData pCb);
uint32_t Usart_transmit(uint8_t* pData, uint32_t length);
void Usart_doBackground(void);

#endif /* USART_H_ */
