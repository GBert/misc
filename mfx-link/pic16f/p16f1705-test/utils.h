/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _UTILS_H_
#define _UTILS_H_

char nibble_to_hex(uint8_t c);
uint8_t hex_to_nibble(uint8_t c);
uint8_t hex_to_byte(struct serial_buffer *fifo);

#endif	/* _UTILS_H_ */
