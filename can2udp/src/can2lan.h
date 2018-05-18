/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
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

#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <linux/can.h>
#include <ifaddrs.h>

#include <syslog.h>

#define MAXLINE			1024		/* max string length     */
#define MAX_TCP_CONN		16		/* max TCP clients       */
#define CAN_ENCAP_SIZE 		13		/* maximum datagram size */
#define MAXDG   		(CAN_ENCAP_SIZE*512)	/* maximum datagram size */
#define MAXMTU   		1400		/* maximum MTU           */
#define MAX_PACKETS		10		/* maximum number of CAN frames per TCP packet */
#define MAXUDP  		16		/* maximum datagram size */
#define MAXIPLEN  		40		/* maximum IP string length */
#define MAX_UDP_BCAST_RETRY  	10		/* maximum retries getting UDP socket */
#define MAX_TRACK_PAGE		64		/* maximum number track pages */
#define MAX(a,b)		((a) > (b) ? (a) : (b))

#define	CRC			0x01
#define COMPRESSED		0x02
#define TERM_SPEED		B500000

#define fprint_syslog(pipe, spipe, text) \
	    do { fprintf( pipe, "%s: " text "\n", __func__); \
		 syslog( spipe, "%s: " text "\n", __func__); } while (0)

#define fprint_syslog_wc(pipe, spipe, text, var) \
	    do { fprintf( pipe, "%s: " text " %s\n", __func__, var); \
		 syslog( spipe, "%s: " text " %s\n", __func__, var); } while (0)

#if 0
struct cs2_config {
    int canid;
    int can_socket;
    int net_socket;
    int udp_socket;
    int tcp_socket;
    int config_flags;
    char *config_dir;
    char *filename;
    char **page_name;
    char *gleisbild_sr2;
};
#endif

enum cs2_copy_state {
    CS2_STATE_INACTIVE,
    CS2_STATE_NORMAL_CONFIG,
    CS2_STATE_TRACK_SUM,
    CS2_STATE_GET_TRACKS,
    CS2_STATE_BROADCAST_UPDATE
};

struct id_node {
    uint32_t id;
    uint8_t slave_node;
    struct id_node *next;
};

struct cs2_config_data_t {
    int deflated_stream_size;
    int deflated_size;
    int deflated_size_counter;
    int inflated_size;
    uint16_t crc;
    uint16_t guihash;
    char *dir;
    char *name;
    int next;
    int verbose;
    int track_index;
    unsigned int state;
    int start;
    int stream;
    int cs2_tcp_socket;
    int cs2_config_copy;
    unsigned int ddi;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
    char **page_name;
};

#define MS1_BUFFER_SIZE 8
#define MS1_BUFFER_MASK (MS1_BUFFER_SIZE-1)

struct MS1_Node_Buffer {
    uint8_t data[MS1_BUFFER_SIZE];
    uint8_t read;
    uint8_t write;
};

static const int MAXPENDING = 16;	/* max outstanding tcp connections */
unsigned char netframe[MAXDG];
unsigned char ec_frame[13];

/* prototypes */
uint8_t * read_config_file(char *filename, char *config_dir, uint32_t *nbytes);
int time_stamp(char *timestamp);
uint16_t generateHash(uint32_t uid);
char **read_track_file(char *filename, char **page_name);
int send_tcp_config_data(char *filename, char *config_dir,  uint32_t canid, int tcp_socket, int flags);
void print_can_frame(char *format_string, unsigned char *netframe, int verbose);
int net_to_net(int net_socket, struct sockaddr *net_addr, unsigned char *netframe, int length);
int frame_to_can(int can_socket, unsigned char *netframe);
int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame);
void ms1_node_buffer_init(void);
int ms1_node_buffer_in(uint8_t node);
int ms1_node_buffer_out(uint8_t *node);
int ms1_print_handles(struct id_node *node);
struct id_node *ms1_search_for_id(struct id_node *node, uint32_t id);
struct id_node *ms1_search_for_slave(struct id_node *node, uint8_t slave_node);
int ms1_add_id(struct id_node *root_node, uint32_t id, uint8_t slave_node);
int reassemble_data(struct cs2_config_data_t *config_data, unsigned char *netframe);
uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);
extern int write_pid_file(const char *pid_fn);
void free_track_file(char **page_name);
#endif /* _CAN2LAN_H_ */

