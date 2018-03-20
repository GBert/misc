/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include "main.h"

extern struct serial_buffer rx_fifo;

char nibble_to_hex(uint8_t c) {
    char nibble;
    nibble = (c & 0x0f) + '0';
    if (nibble >= 0x3a)
        nibble += 7;
    return (nibble);
}

uint8_t hex_to_nibble(uint8_t c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return c - 'a' + 10;
}

uint8_t hex_to_byte(struct serial_buffer *fifo) {
    uint8_t n;
    char c;

    if (c = getchar_fifo(&rx_fifo))
        n = hex_to_nibble(c);
    n = n << 4;
    if (c = getchar_fifo(&rx_fifo))
        n += hex_to_nibble(c);
    return n;
}

