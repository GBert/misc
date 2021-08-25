/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <linux/can.h>

#define FRAME_SIZE	13
#define MAXSIZE		16384

unsigned char GET_GB[]	= { 0x00, 0x30, 0x47, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char GET_GB1[] = { 0x00, 0x3A, 0x47, 0x11, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

char *TCP_FORMAT_STRG	= "->TCP    CANID 0x%06X   [%d]";
char *TCP_FORMATS_STRG	= "->TCP*   CANID 0x%06X   [%d]";
char *S_TCP_FORMAT_STRG = "  TCP<-  CANID 0x%06X   [%d]";

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void print_net_frame(char *format_string, unsigned char *netframe) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s   ", timestamp);
    printf(format_string, ntohl(canid) & CAN_EFF_MASK, netframe[4]);
    for (i = 5; i < 5 + dlc; i++) {
	printf(" %02x", netframe[i]);
    }
    if (dlc < 8) {
	printf("(%02x", netframe[i]);
	for (i = 6 + dlc; i < 13; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("  ");
    for (i = 5; i < 13; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    if (send(net_socket, netframe, length, 0) != length) {
	fprintf(stderr, "couldn't send on TCP socket: %s\n", strerror(errno));
	return -1;
    }
    print_net_frame(S_TCP_FORMAT_STRG, netframe);

    return 0;
}

int check_data(int tcp_socket, unsigned char *netframe) {
    uint32_t canid;
    unsigned char netframe_config[FRAME_SIZE];

    memcpy(&canid, netframe, 4);
    canid = ntohl(canid);
    switch (canid & 0x00FF0000UL) {
    case (0x00310000UL):
	memcpy(netframe_config, GET_GB1, 13);
	/* copy Gleisbox ID */
	memcpy(&netframe_config[5], &netframe[5], 4);
	netframe_to_net(tcp_socket, netframe_config, 13);
	break;
    default:
	break;
    }
    return 0;
}

int main(int argc, char **argv) {
    int sockfd, tcp_packet_nr, n = 1;
    int i;
    fd_set rset;
    struct sockaddr_in servaddr;
    unsigned char netframe[FRAME_SIZE];
    unsigned char recvline[MAXSIZE];
    char timestamp[16];

    if (argc != 2) {
	fprintf(stderr, "usage:  %s <IP address>\n", argv[0]);
	exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (inet_aton((argv[1]), (struct in_addr *)&servaddr.sin_addr.s_addr) == 0) {
	fprintf(stderr, "invalid address : %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    servaddr.sin_port = htons(15731);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
	fprintf(stderr, "can't connect to TCP socket : %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memcpy(netframe, GET_GB, FRAME_SIZE);

    netframe_to_net(sockfd, netframe, FRAME_SIZE);
    FD_ZERO(&rset);
    tcp_packet_nr = 0;
    for (;;) {
	FD_SET(sockfd, &rset);

	if (select(sockfd + 1, &rset, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	tcp_packet_nr++;
	if (FD_ISSET(sockfd, &rset)) {
	    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) > 0) {
		/* check the whole TCP packet, if there are more than one CAN frame included */
		/* TCP packets with size modulo 13 !=0 are ignored though */
		if (n % 13) {
		    time_stamp(timestamp);
		    fprintf(stderr, "%s received packet %% 13 : length %d\n", timestamp, n);
		} else {
		    for (i = 0; i < n; i += 13) {
			if (i >= 13)
			    print_net_frame(TCP_FORMATS_STRG, &recvline[i]);
			else
			    print_net_frame(TCP_FORMAT_STRG, &recvline[i]);
			check_data(sockfd, &recvline[i]);
		    }
		}
	    }
	}
    }
    return 0;
}
