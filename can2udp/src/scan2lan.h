/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* Thanks to Stefan Krauss and the SocketCAN team
 */
#ifndef _CAN2LAN_H_
#define _CAN2LAN_H_
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define MAXSTRING	64		/* max string length     */
#define MAXLINE		1024		/* max line length       */
#define MAX_TCP_CONN	16		/* max TCP clients       */
#define MAXDG   	4096		/* maximum datagram size */
#define MAXMTU   	1400		/* maximum MTU           */
#define MAX_PACKETS	40		/* maximum number of CAN frames per TCP packet */
#define MAXUDP  	16		/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))

#define	CRC		0x01
#define COMPRESSED	0x02
#define TERM_SPEED	B500000

#if 0
struct cs2_config {
    int canid;
    int can_socket;
    int net_socket;
    int udp_socket;
    int tcp_socket;
    int config_flags;
    int simple_can;
    char *config_dir;
    char *filename;
    char **page_name;
    char *gleisbild_sr2;
};
#endif

/* special address description flags for the CAN_ID */
#define CAN_EFF_FLAG 0x80000000U /* EFF/SFF is set in the MSB */
#define CAN_RTR_FLAG 0x40000000U /* remote transmission request */
#define CAN_ERR_FLAG 0x20000000U /* error frame */

/* valid bits in CAN ID for frame formats */
#define CAN_SFF_MASK 0x000007FFU /* standard frame format (SFF) */
#define CAN_EFF_MASK 0x1FFFFFFFU /* extended frame format (EFF) */
#define CAN_ERR_MASK 0x1FFFFFFFU /* omit EFF, RTR, ERR flags */

struct can_frame {
    uint32_t can_id;  /* 32  CAN_ID + EFF/RTR/ERR flags */
    uint8_t  can_dlc; /* data length code: 0 .. 8 */
    uint8_t  data[8] __attribute__((aligned(8)));
};

static const int MAXPENDING = 16;	/* max outstanding tcp connections */
unsigned char netframe[MAXDG];
unsigned char ec_frame[13];

uint8_t * read_config_file(char *filename, char *config_dir, uint32_t *nbytes);
int time_stamp(char *timestamp);
char **read_track_file(char *filename, char **page_name);
int send_tcp_config_data(char *filename, char *config_dir,  uint32_t canid, int tcp_socket, int flags);
void print_can_frame(char *format_string, unsigned char *netframe);
int net_to_net(int net_socket, struct sockaddr *net_addr, unsigned char *netframe, int length);
int frame_to_can(int can_socket, unsigned char *netframe);
int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame);
uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);
#endif /* _CAN2LAN_H_ */

