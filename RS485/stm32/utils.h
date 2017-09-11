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

void bin2hex(uint8_t *dst, unsigned char c);
void bin2triple(uint8_t * dst, uint16_t c);
uint8_t nibble2bin(uint8_t s);
uint8_t hex2bin(char *s);
uint16_t triple2bin(char *s);

#endif  /* _UTILS_H_ */


