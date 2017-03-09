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

#include <xc.h>
#include "main.h"

#ifndef BAUDRATE
#define BAUDRATE	500000
#endif
#define USE_BRG16	0
#define USE_BRGH	1

/* USART calculating Baud Rate Generator
 * if BRGH = 0 => FOSC/[64 (n + 1)]
 * if BRGH = 1 => FOSC/[16 (n + 1)]
 * avoid rounding errors
 */

#if USE_BRGH == 0
#define	SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 32) - 1) / 2 )
#else
#define	SBRG_VAL	( (((_XTAL_FREQ / BAUDRATE) / 8) - 1) / 2 )
#endif


/* circular buffer */

#endif		/* _USART_H_ */
