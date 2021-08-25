/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>

#define XNTCPPORT	61235
#define FRAME_SIZE	32
#define MAXSIZE		16384


unsigned char GET_LM2[]		= { 0x41 };
unsigned char RESP_LM2_0[]	= { 0x21, 0x24, 0x05 };
unsigned char RESP_LM2_1[]	= { 0x21, 0x21, 0x00 };
unsigned char RESP_LM2_2[] 	= { 0xE1, 0x62, 0x22, 0x00, 0x40 };
unsigned char RESP_LM2_3[] 	= { 0xE1, 0x63, 0x21, 0x30, 0x00, 0x72 };
unsigned char LOKINFO_0[]	= { 0xE1, 0xE4, 0x04, 0x00, 0x00, 0x00, 0x00 };

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

void add_crc(uint8_t *data, int length) {
    int i;
    uint8_t xor = 0;

    for (i = 0; i < (length - 2); i++) {
        xor = xor ^ *data;
        data++;
    }
    *data = xor;
}

uint8_t add_parity(uint8_t data) {
    uint8_t temp, parity = 0;
    data &= 0x7f;
    temp = data;

    while (temp) {
        parity = !parity;
        temp &= (temp - 1);
    }
    if (parity)
        data |= 0x80;
    return data;
}

void print_net_frame(unsigned char *frame, int length) {
    int i;
    char timestamp[16];

    time_stamp(timestamp);
    printf("%s   ", timestamp);
    for (i = 0; i < length; i++)
	printf(" %02x", frame[i]);
    printf("\n");
}

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    int on = 1;

    printf("length %d\n", length);
    if (send(net_socket, netframe, length, 0) != length) {
	fprintf(stderr, "couldn't send on TCP socket: %s\n", strerror(errno));
	return -1;
    }

    /* disable Nagle algorithm - force PUSH to send immediatly */
    if (setsockopt(net_socket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
	return -1;
    }

    print_net_frame(netframe, length);

    return 0;
}

int check_data(int tcp_socket, unsigned char *frame) {

    if (memcmp(&frame[0], RESP_LM2_0, 3) == 0) {
    	// memcpy(frame, GET_LM2, sizeof(GET_LM2));
    	memcpy(frame, RESP_LM2_2, sizeof(RESP_LM2_2));
	netframe_to_net(tcp_socket, frame, sizeof(RESP_LM2_2));
	memcpy(frame, GET_LM2, sizeof(GET_LM2));
	netframe_to_net(tcp_socket, frame, sizeof(GET_LM2));
    }
    if (memcmp(&frame[0], RESP_LM2_1, 3) == 0) {
    	// memcpy(frame, GET_LM2, sizeof(GET_LM2));
    	memcpy(frame, RESP_LM2_3, sizeof(RESP_LM2_3));
	netframe_to_net(tcp_socket, frame, sizeof(RESP_LM2_3));
	/* usec_sleep(900000);
	memcpy(frame, GET_LM2, sizeof(GET_LM2));
	netframe_to_net(tcp_socket, frame, sizeof(GET_LM2));
	*/
    }
    return 0;
}

int main(int argc, char **argv) {
    int ret, sockfd, tcp_packet_nr, n = 1;
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

    servaddr.sin_port = htons(XNTCPPORT);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
	fprintf(stderr, "can't connect to TCP socket : %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memcpy(netframe, GET_LM2, 1);
    netframe_to_net(sockfd, netframe, sizeof(GET_LM2));

    FD_ZERO(&rset);
    tcp_packet_nr = 0;
    for (;;) {
	FD_SET(sockfd, &rset);

        ret = select(sockfd + 1, &rset, NULL, NULL, NULL);
	if (ret < 0) {
	    fprintf(stderr, "socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	tcp_packet_nr++;
	if (FD_ISSET(sockfd, &rset)) {
	    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) > 0) {
		print_net_frame(recvline, n);
		check_data(sockfd, recvline);
	    } else if (n == 0) {
		fprintf(stderr, "%s server closed connection\n", timestamp);
		close(sockfd);
		exit(EXIT_FAILURE);
	    } else {
		time_stamp(timestamp);
		fprintf(stderr, "%s received packet with error: %d %s\n", timestamp, n, strerror(errno));
		exit(EXIT_FAILURE);
	    }
	}
    }
    return 0;
}
