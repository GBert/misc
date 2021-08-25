/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 * Contributions by Rainer Müller
 *
 *	NOTE: RC5 protocol should be activated before using this program by entering
 *			 echo rc-5 > /sys/class/rc/rc0/protocols
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <linux/can.h>
#include <linux/input.h>

#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum UDP datagram size */
#define DEFAULT_LOCO	5
#define DEFAULT_STEP	16
#define MAXSPEED	1000
#define BLINDTIME	250000	/* ignore codes for a number of us */

static unsigned char LOCO_SPEED[]     = { 0x00, 0x08, 0x03, 0x00, 0x06, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char LOCO_DIRECTION[] = { 0x00, 0x0A, 0x03, 0x00, 0x05, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char LOCO_FUNCTION[]  = { 0x00, 0x0C, 0x03, 0x00, 0x06, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char START_STOP[]     = { 0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface> -r <infrared-interface> -s <step>\n", prg);
    fprintf(stderr, "   Version 0.5\n\n");
    fprintf(stderr, "         -l <loco id>        loco id - default %d\n", DEFAULT_LOCO);
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -r <ir int>         infrared event interface - default /dev/input/event1\n");
    fprintf(stderr, "         -s <step>           speed setp - default %d\n", DEFAULT_STEP);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    char timestamp[16];
    time_stamp(timestamp);
    printf("%s ", timestamp);
    printf(format_string, frame->can_id & CAN_EFF_MASK, frame->can_dlc);
    for (i = 0; i < frame->can_dlc; i++) {
	printf(" %02x", frame->data[i]);
    }
    if (frame->can_dlc < 8) {
	for (i = frame->can_dlc; i < 8; i++) {
	    printf("   ");
	}
    }
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

int send_can_frame(int can_socket, struct can_frame *frame) {
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
	fprintf(stderr, "error writing CAN frame: %s\n", strerror(errno));
	return -1;
    }
    return 0;
}

int send_defined_can_frame(int can_socket, unsigned char *data) {
    struct can_frame frame;
    uint32_t can_id;
    memset(&frame, 0, sizeof(frame));
    memcpy(&can_id, &data[0], 4);
    frame.can_id = htonl(can_id);
    frame.can_dlc = data[4];
    memcpy(&frame.data, &data[5], 8);
    send_can_frame(can_socket, &frame);
    return 0;
}

int main(int argc, char **argv) {
    int step, loco, speed, n, opt, sc, ret, fd, status;
    struct can_frame frame;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    struct input_event ev[64];
    char ir_int[255];
    unsigned char data[13];
    unsigned char loco_function[32];
    unsigned char direction;
    socklen_t caddrlen = sizeof(caddr);
    time_t last = 0;

    status = 0;
    loco = DEFAULT_LOCO;
    speed = 0;
    step = DEFAULT_STEP;
    direction = 0;
    memset(loco_function, 0, sizeof(loco_function));

    strcpy(ifr.ifr_name, "can0");
    strcpy(ir_int, "/dev/input/event1");

    while ((opt = getopt(argc, argv, "s:l:i:r:h?")) != -1) {
	switch (opt) {
	case 'l':
	    loco = atoi(optarg);
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'r':
	    strncpy(ir_int, optarg, sizeof(ir_int) - 1);
	    break;
	case 's':
	    step = atoi(optarg);
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    memset(&caddr, 0, sizeof(caddr));

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if ((fd = open(ir_int, O_RDONLY)) < 0) {
	fprintf(stderr, "error opening IR device: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    while (1) {
	ret = read(fd, ev, sizeof(struct input_event) * 64);
	if (ret < (int)sizeof(struct input_event)) {
	    fprintf(stderr, "error reading IR device: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	for (n = 0; n < (int)(ret / sizeof(struct input_event)); n++) {
	    /* printf("Event: time %ld.%06ld, ", ev[n].time.tv_sec, ev[n].time.tv_usec); */
	    /* printf(" type :0x%02x, code 0x%02x, value 0x%04x\n", ev[n].type, ev[n].code, ev[n].value); */
	    if (ev[n].type == EV_MSC && (ev[n].code == MSC_RAW || ev[n].code == MSC_SCAN)) {
		time_t timestamp = ev[n].time.tv_sec * 1000000 + ev[n].time.tv_usec;
		if ((timestamp - last) < BLINDTIME) continue;
		last = timestamp;
		int keycode = ev[n].value & 0xff;
		memcpy(&frame.data, &data[5], 8);
		switch (keycode) {
		case 0x00:
		case 0x01:
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
		    printf("function %d\n", keycode);
		    loco_function[keycode] ^= 1;
		    memcpy(data, LOCO_FUNCTION, sizeof(data));
		    data[8] = loco & 0xff;
		    data[9] = keycode;
		    data[10] = loco_function[keycode] & 0xff;
		    send_defined_can_frame(sc, data);
		    break;
		case 0x0c:
		    printf("Start/Stop\n");
		    memcpy(data, START_STOP, sizeof(data));
		    status ^= 1;
		    data[9] = status & 0x01;
		    send_defined_can_frame(sc, data);
		    break;
		case 0x0d:
		    printf("Direction\n");
		    memcpy(data, LOCO_DIRECTION, sizeof(data));
		    data[8] = loco & 0xff;
		    direction ^= 1;
		    data[9] = (direction & 0x01) + 1;
		    send_defined_can_frame(sc, data);
		    break;
		case 0x10:
		    memcpy(data, LOCO_SPEED, sizeof(data));
		    data[8] = loco & 0xff;
		    speed += step;
		    if (speed > MAXSPEED)
			speed = MAXSPEED;
		    printf("loco %d speed + %d\n", loco, speed);
		    data[9] = (speed >> 8) & 0x03;
		    data[10] = speed & 0xff;
		    send_defined_can_frame(sc, data);
		    break;
		case 0x11:
		    memcpy(data, LOCO_SPEED, sizeof(data));
		    data[8] = loco & 0xff;
		    if (speed > 0)
			speed -= step;
		    if (speed < 0)
			speed = 0;
		    printf("loco %d speed - %d \n", loco, speed);
		    data[9] = (speed >> 8) & 0x03;
		    data[10] = speed & 0xff;
		    send_defined_can_frame(sc, data);
		    break;
		default:
		    printf("unhandled value 0x%02x\n", ev[n].value);
		    break;
		}
	    }
	}
    }
    return 0;
}
