/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 *
 * this code emulates the M*rklin Gleisbox to some extend . Only for testing
 *  the M*rklinApp and gateway (can2lan) code
 *
 *  Usage: eg. verbose, running in foreground and listening to default vcan0:
 *    ./virtual_gfp -v -f
 *
 */

/* Thanks to Stefan Krauss and the SocketCAN team
 */

/*
 *  Test setup:

  sudo modprobe can
  sudo modprobe vcan
  sudo modprobe can-raw
  sudo modprobe can-gw
  sudo ip link add dev vcan0 type vcan
  sudo ip link add dev vcan1 type vcan
  sudo ifconfig vcan0 up
  sudo ifconfig vcan1 up
  sudo cangw -A -s vcan0 -d vcan1 -e
  sudo cangw -A -s vcan1 -d vcan0 -e
  ./virtual_gfp -f -i vcan1
  ./can2lan -f -i vcan0

 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <linux/can.h>

#define MAXDG   	4096		/* maximum datagram size */
#define MAXUDP  	16		/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define debug_print(...) \
            do { if (DEBUG) fprintf(stderr, ##__VA_ARGS__); } while (0)

char *F_CAN_FORMAT_STRG    ="      CAN->  CANID 0x%08X R [%d]";
char *F_S_CAN_FORMAT_STRG  ="short CAN->  CANID 0x%08X R [%d]";
char *T_CAN_FORMAT_STRG    ="      CAN<-  CANID 0x%08X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE [] = {0x00,0x36,0x03,0x01,0x05,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00};

unsigned char M_ALL_ID []            = {0x00,0x00,0x00,0x00};
unsigned char M_GLEISBOX_ID []       = {0x00,0x31,0x9B,0x32,0x08,0x47,0x42,0xF5,0xDA,0x00,0x00,0x00,0x10};

unsigned char M_GLEISBOX_STATUS1  [] = {0x00,0x3B,0x03,0x01,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0xC3,0xDA};
unsigned char M_GLEISBOX_STATUS2  [] = {0x00,0x3B,0x03,0x02,0x08,0x36,0x30,0x31,0x31,0x33,0x00,0x00,0x00};
unsigned char M_GLEISBOX_STATUS3  [] = {0x00,0x3B,0x03,0x03,0x08,0x47,0x6C,0x65,0x69,0x73,0x62,0x6F,0x78};
unsigned char M_GLEISBOX_STATUS4  [] = {0x00,0x3B,0x03,0x04,0x08,0x20,0x32,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char M_GLEISBOX_STATUS5  [] = {0x00,0x3B,0x03,0x05,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char M_GLEISBOX_STATUS6  [] = {0x00,0x3B,0x9B,0x32,0x05,0x47,0x42,0xF5,0xDA,0x00,0x00,0x00,0x10};

unsigned char M_GLEISBOX_INDEX1_1 [] = {0x00,0x3B,0x03,0x01,0x08,0x01,0xFD,0x30,0xF0,0xE0,0xC0,0x00,0x0F};
unsigned char M_GLEISBOX_INDEX1_2 [] = {0x00,0x3B,0x03,0x02,0x08,0x06,0x70,0x06,0xC2,0x07,0x67,0x08,0x0C};
unsigned char M_GLEISBOX_INDEX1_3 [] = {0x00,0x3B,0x03,0x03,0x08,0x54,0x52,0x41,0x43,0x4B,0x00,0x30,0x2E};
unsigned char M_GLEISBOX_INDEX1_4 [] = {0x00,0x3B,0x03,0x04,0x08,0x30,0x30,0x00,0x32,0x2E,0x35,0x30,0x00};
unsigned char M_GLEISBOX_INDEX1_5 [] = {0x00,0x3B,0x03,0x05,0x08,0x41,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char M_GLEISBOX_INDEX1_6 [] = {0x00,0x3B,0x9B,0x32,0x05,0x47,0x42,0xF5,0xDA,0x01,0x00,0x00,0x00};

unsigned char M_GLEISBOX_INDEX2_1 [] = {0x00,0x3B,0x03,0x01,0x08,0x03,0xFD,0xC0,0x0C,0x30,0xC0,0x00,0x00};
unsigned char M_GLEISBOX_INDEX2_2 [] = {0x00,0x3B,0x03,0x02,0x08,0x03,0x9D,0x04,0xB2,0x0A,0x1E,0x0C,0x49};
unsigned char M_GLEISBOX_INDEX2_3 [] = {0x00,0x3B,0x03,0x03,0x08,0x56,0x4F,0x4C,0x54,0x00,0x31,0x30,0x2E};
unsigned char M_GLEISBOX_INDEX2_4 [] = {0x00,0x3B,0x03,0x04,0x08,0x30,0x30,0x00,0x32,0x37,0x2E,0x30,0x30};
unsigned char M_GLEISBOX_INDEX2_5 [] = {0x00,0x3B,0x03,0x05,0x08,0x00,0x56,0x6F,0x6C,0x74,0x00,0x00,0x00};
unsigned char M_GLEISBOX_INDEX2_6 [] = {0x00,0x3B,0x9B,0x32,0x05,0x47,0x42,0xF5,0xDA,0x02,0x00,0x00,0x00};

unsigned char M_GLEISBOX_INDEX3_1 [] = {0x00,0x3B,0x03,0x01,0x08,0x04,0x00,0x0C,0x08,0xF0,0xC0,0x00,0x00};
unsigned char M_GLEISBOX_INDEX3_2 [] = {0x00,0x3B,0x03,0x02,0x08,0x00,0x6B,0x00,0xA4,0x00,0xCD,0x00,0xDB};
unsigned char M_GLEISBOX_INDEX3_3 [] = {0x00,0x3B,0x03,0x03,0x08,0x54,0x45,0x4D,0x50,0x00,0x30,0x2E,0x30};
unsigned char M_GLEISBOX_INDEX3_4 [] = {0x00,0x3B,0x03,0x04,0x08,0x00,0x38,0x30,0x2E,0x30,0x00,0x43,0x00};
unsigned char M_GLEISBOX_INDEX3_5 [] = {0x00,0x3B,0x9B,0x32,0x05,0x47,0x42,0xF5,0xDA,0x03,0x00,0x00,0x00};

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -i <can int>        can interface - default vcan0\n");
    fprintf(stderr, "         -v                  running verbose\n");
    fprintf(stderr, "         -f                  running in foreground\n");
    fprintf(stderr, "\n");
}

int time_stamp(char *timestamp){
    struct timeval  tv;
    struct tm      *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp,"%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int) tv.tv_usec/1000);
    return 0;
}

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    char timestamp[16];
    time_stamp(timestamp);
    printf("%s ",timestamp);
    printf(format_string, frame->can_id & CAN_EFF_MASK, frame->can_dlc);
    for (i = 0; i < frame->can_dlc; i++) {
	printf(" %02x", frame->data[i]);
    }
    if (frame->can_dlc < 8) {
	for (i = frame->can_dlc ; i < 8 ; i++) {
	    printf("   ");
        }
    }
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if(isprint(frame->data[i]))
	    printf("%c",frame->data[i]);
        else
	    putchar(46);
    }
    printf("\n");
}

int send_can_frame(int can_socket, struct can_frame *frame, int verbose) {
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
        perror("error writing CAN frame\n");
        return -1;
    }
    if (verbose)
        print_can_frame(T_CAN_FORMAT_STRG, frame);
    return 0;
}

int send_defined_can_frame(int can_socket, unsigned char *data, int verbose) {

    struct can_frame frame;
    uint32_t can_id;
    bzero(&frame, sizeof(frame));
    memcpy(&can_id, &data[0], 4);
    frame.can_id = htonl(can_id);
    frame.can_dlc=data[4];
    memcpy(&frame.data, &data[5], 8);
    send_can_frame(can_socket, &frame, verbose);
    return 0;
}

int main(int argc, char **argv) {
    pid_t pid;
    extern int optind, opterr, optopt;
    int max_fds, opt;
    struct can_frame frame;

    int sc;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);

    fd_set read_fds;

    int verbose = 1;
    int background = 0;
    strcpy(ifr.ifr_name, "vcan0");

    while ((opt = getopt(argc, argv, "u:t:d:b:i:vf?")) != -1) {
	switch (opt) {
	case 'i':
	    strcpy(ifr.ifr_name, optarg);
	    break;
	case 'v':
	    verbose = 1;
	    break;
	case 'f':
	    background = 0;
	    break;
	case '?':
	    print_usage(basename(argv[0]));
	    exit(0);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(1);
	    break;
	}
    }

    bzero(&caddr, sizeof(caddr));

    /* prepare CAN socket */
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	perror("error creating CAN socket\n");
	exit(1);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	perror("SIOCGIFINDEX error\n");
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(sc, (struct sockaddr *) &caddr, caddrlen) < 0) {
	perror("error binding CAN socket\n");
	exit(1);
    }

    /* daemonize the process if requested */
    if (background) {
	/* fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* if we got a good PID, then we can exit the parent process */
	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    while (1) {
	if (select(max_fds + 1 , &read_fds, NULL, NULL, NULL) < 0)
	    perror("select error");
	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
            if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		perror("error reading CAN frame\n");
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
                if (verbose) {
		    print_can_frame(F_CAN_FORMAT_STRG, &frame);
		}

		switch ((frame.can_id & 0x00FF0000UL ) >> 16) {
                case 0x00:
                    if ((memcmp(&frame.data[0], &M_GLEISBOX_ID[5], 4) == 0 ) ||
                        (memcmp(&frame.data[0], M_ALL_ID, 4) == 0 ) ) {
                        /* frame.can_id &= 0xFFFF0000UL; */
                        frame.can_id |= 0x00010000UL;
                        send_can_frame(sc, &frame, verbose);
                    }
                    break;
		case 0x08:	/* loco speed		*/
		case 0x0A:	/* loco direction	*/
		case 0x0C:	/* loco function	*/
		case 0x16:	/* extension	 	*/
		    /* frame.can_id &= 0xFFFF0000UL; */
		    frame.can_id |= 0x00010000UL; 
		    send_can_frame(sc, &frame, verbose);
                    break;
		case 0x30:	/* ping / ID /software 	*/
                    send_defined_can_frame(sc, M_GLEISBOX_ID, verbose);
                    break;
		case 0x3A:	/* status	 	*/
                    if (memcmp(&frame.data[0], &M_GLEISBOX_ID[5], 4) == 0 ) {
                        switch(frame.data[4]) {
                        case 0x00:
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS1, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS2, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS3, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS4, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS5, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_STATUS6, verbose);
                            break;
                        case 0x01:
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_1, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_2, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_3, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_4, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_5, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX1_6, verbose);
                            break;
                        case 0x02:
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_1, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_2, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_3, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_4, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_5, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX2_6, verbose);
                            break;
                        case 0x03:
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX3_1, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX3_2, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX3_3, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX3_4, verbose);
                            send_defined_can_frame(sc, M_GLEISBOX_INDEX3_5, verbose);
                            break;
                        default:
                            break;
                        }
                    }
		default:
		    break;
		}

            } else {
		print_can_frame(F_S_CAN_FORMAT_STRG, &frame);
	    }
	}
    }
    close(sc);
    return 0;
}
