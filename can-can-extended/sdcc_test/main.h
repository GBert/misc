/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <pic18fregs.h>
// #pragma config XINST=OFF

#define _XTAL_FREQ 64000000	// This is the speed your controller is running at
#define FCYC (_XTAL_FREQ/4L)	// target device instruction clock freqency

#define LED_TRIS        (TRISAbits.TRISA0)
#define LED             (LATAbits.LATA0)

#endif /* _MAIN_H_ */

