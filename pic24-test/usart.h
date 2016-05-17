/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _USART_H_
#define _USART_H_

#include "main.h"

#ifndef BAUDRATE
#define BAUDRATE	115200
#endif

#define	SBRG_VAL	(((FCY / BAUDRATE) - 8) / 16 )

/* circular buffer */

#endif /* _USART_H_ */
