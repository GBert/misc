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
char *T_CAN_FORMAT_STRG    ="    ->CAN    CANID 0x%08X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE [] = {0x00,0x36,0x03,0x01,0x05,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00};

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

char *time_stamp(){
    char *timestamp = (char *)malloc(sizeof(char) * 16);
    struct timeval  tv;
    struct timezone tz;
    struct tm      *tm;

    gettimeofday(&tv, &tz);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp,"%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int) tv.tv_usec/1000);
    return timestamp;
}

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    printf("%s ",time_stamp()); 
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
    int nbytes;
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if ((nbytes = write(can_socket, frame, sizeof(*frame))) != sizeof(*frame)) {
        perror("error writing CAN frame\n");
        return -1;
    }
    if (verbose)
        print_can_frame(T_CAN_FORMAT_STRG, frame);
    return 0;
}

    
int main(int argc, char **argv) {
    pid_t pid;
    extern int optind, opterr, optopt;
    int max_fds, opt;
    int nbytes, ret;
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
	ret = select(max_fds + 1 , &read_fds, NULL, NULL, NULL);
	if (ret<0)
	    perror("select error\n");

	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
            if ((nbytes = read(sc, &frame, sizeof(struct can_frame))) < 0) {
		perror("error reading CAN frame\n");
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
                if (verbose) {
		    print_can_frame(F_CAN_FORMAT_STRG, &frame);
		}

		switch ((frame.can_id & 0x00FF0000UL ) >> 17) {
		case 0x04:	/* loco speed		*/
		case 0x05:	/* loco direction	*/
		case 0x06:	/* loco function	*/
		case 0x0B:	/* extension	 	*/
		    frame.can_id &= 0xFFFF0000UL; 
		    frame.can_id |= 0x00010000UL; 
		    send_can_frame(sc, &frame, verbose);
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
