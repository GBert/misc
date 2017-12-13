/* --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice
 * you can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * --------------------------------------------------------------------------------
 *
 * CAN bounce test
 *
 * compile:
 * gcc -Wall -o bounce bounce.c
 *
 * # test setup:
 * modprobe can
 * modprobe can-raw
 * modprobe can-gw
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/time.h>

#include <linux/can.h>

/*
 * CFL_WORSTCASE corresponds to Ken Tindells *worst* case calculation
 * for stuff-bits (see "Guaranteeing Message Latencies on Controller
 * Area Network" 1st ICC'94) the needed bits on the wire can be
 * calculated as:
 *
 * (34 + 8n)/5 + 47 + 8n for SFF frames (11 bit CAN-ID) => (269 + 48n)/5
 * (54 + 8n)/5 + 67 + 8n for EFF frames (29 bit CAN-ID) => (389 + 48n)/5
 */

#define NUM_OF_FRAMES        10000
#define CAN_NORMAL_FRAME_BITS(a)    ((269 + 48*a)/5)
#define CAN_EXTENDED_FRAME_BITS(a)  ((389 + 48*a)/5)

uint32_t be32(uint8_t *u) {
    return (u[0]<<24) | (u[1]<<16) | (u[2]<<8) | u[3];
}

int init_can(char *can_interface) {
    int socket_can;
    struct ifreq ifr;
    struct sockaddr_can caddr;

    socklen_t caddrlen = sizeof(caddr);
    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, can_interface, sizeof(ifr.ifr_name) - 1);

    if ((socket_can = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	perror("Can't connect to can socket \n");
	exit(1);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(socket_can, SIOCGIFINDEX, &ifr) < 0) {
	perror("SIOCGIFINDEX");
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_can, (struct sockaddr *)&caddr, caddrlen) < 0) {
	perror("bind error");
	exit(1);
    }
    printf("CAN Socket %s opened\n", can_interface);
    return socket_can;
}

void print_can_frame(struct can_frame *can_frame) {
    uint8_t i;
    printf("CAN 0x%08X", can_frame->can_id);
    printf(" [%d]", can_frame->can_dlc);
    for (i = 0; i < can_frame->can_dlc; i++) {
	printf(" %02x", can_frame->data[i]);
    }
    printf("\n");
}

void bounce_can_frame(int socket_can, struct can_frame *can_frame) {
    uint32_t counter;
    uint32_t be_counter;
    counter = be32(can_frame->data);
    counter++;
    be_counter = htonl(counter);
    memcpy(&can_frame->data[0], &be_counter, 4);
    if ((write(socket_can, can_frame, sizeof(*can_frame))) != sizeof(*can_frame)) {
	perror("CAN write error");
    }
}

int main(void) {
    int socket_can1, socket_can2, ret;
    fd_set readfds;
    long count;
    struct can_frame *pcan_frame;
    struct timeval tv1, tv2;
    float elapsed_time, bandwidth, packets_per_second;

    ret = 0;

    socket_can1 = init_can("can0");
    socket_can2 = init_can("can1");
    pcan_frame = (struct can_frame *)malloc(sizeof(struct can_frame));
    if (!pcan_frame)
	return -1;
    memset(pcan_frame, 0, sizeof(struct can_frame));
    pcan_frame->can_id = 123;
    pcan_frame->can_dlc = 4;

    count = NUM_OF_FRAMES;

    gettimeofday(&tv1, NULL);
    bounce_can_frame(socket_can2, pcan_frame);

    while (count >= 0) {
	FD_ZERO(&readfds);
	FD_SET(socket_can1, &readfds);
	FD_SET(socket_can2, &readfds);

	ret = select((socket_can1 > socket_can2) ? socket_can1 + 1 : socket_can2 + 1, &readfds, NULL, NULL, NULL);

	/* received a CAN frame */
	if (FD_ISSET(socket_can1, &readfds)) {
	    if ((read(socket_can1, pcan_frame, sizeof(struct can_frame))) < 0) {
		perror("read error on CAN\n");
		free(pcan_frame);
		return -1;
	    }
	    bounce_can_frame(socket_can1, pcan_frame);
	}
	if (FD_ISSET(socket_can2, &readfds)) {
	    if ((read(socket_can2, pcan_frame, sizeof(struct can_frame))) < 0) {
		perror("read error on CAN\n");
		free(pcan_frame);
		return -1;
	    }
	    bounce_can_frame(socket_can2, pcan_frame);
	}
	count--;
    }
    gettimeofday(&tv2, NULL);
    elapsed_time = (float)((tv2.tv_sec - tv1.tv_sec) * 1E6 + (tv2.tv_usec - tv1.tv_usec)) / 1E6;
    bandwidth = NUM_OF_FRAMES * CAN_NORMAL_FRAME_BITS(4) / elapsed_time / 1E6;
    packets_per_second = NUM_OF_FRAMES / elapsed_time / 1000;
    printf("Send %d in %.3f sec (%.3f tpackets/sec %.3f Mbit)\n", NUM_OF_FRAMES, elapsed_time, packets_per_second, bandwidth);

    free(pcan_frame);
    return ret;
}
