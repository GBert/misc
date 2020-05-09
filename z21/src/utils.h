/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _UTILS_H_
#define _UTILS_H_

struct node {
    int id;
    struct node *next;
};

void usec_sleep(int usec);
uint8_t xor(unsigned char *data, int length);
void print_udp_frame(char *format, int length, unsigned char *udpframe);
void print_net_frame(char *format, unsigned char *udpframe, int verbose);
int time_stamp(char *timestamp);

uint16_t le16(uint8_t * u);
uint16_t be16(uint8_t * u);
uint32_t be32(uint8_t * u);
uint32_t le32(uint8_t * u);

#endif /* _UTILS_H_ */
