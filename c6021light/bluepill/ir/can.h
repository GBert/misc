/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _CAN_H_
#define _CAN_H_

#include <stdint.h>

void can_setup(void);
void send_can_debug(uint32_t val);
void send_can_data(uint32_t command, uint8_t dlc, uint8_t *data);

#endif  /* _CAN_H_ */

