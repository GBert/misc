/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 * Thx to FPerry for his input and testing
 *
 * purpose: wake up M*rklin LinkS88
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <endian.h>

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

#define SLEEPING	10000
#define MAXDG   	4096	/* maximum datagram size */
#define MAXUDP  	16	/* maximum datagram size */
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define TCYC_MAX	1000	/* max cycle time in ms */

static char *F_CAN_FORMAT_STRG	= "      CAN->   0x%08X   [%d]";
static char *T_CAN_FORMAT_STRG	= "      CAN<-   0x%08X   [%d]";
static char delimiters[] = " .,;:!-";

static unsigned char M_CAN_BOOTLOADER[]		= { 0x00, 0x36, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char M_CAN_PING[]		= { 0x00, 0x30, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char M_LINKS88_ID[]		= { 0x00, 0x31, 0x03, 0x00, 0x08, 0x53, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x10 };
static unsigned char M_LINKS88_WAKE_I[]		= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0xE4, 0x00, 0x00, 0x00 };
static unsigned char M_LINKS88_WAKE_II[]	= { 0x00, 0x36, 0x03, 0x00, 0x05, 0x53, 0x38, 0x38, 0x00, 0x11, 0x00, 0x00, 0x00 };
static unsigned char M_LINKS88_WAKE_III[]	= { 0x00, 0x01, 0x03, 0x00, 0x07, 0x53, 0x38, 0x38, 0x00, 0x0C, 0x00, 0x00, 0x00 };

static unsigned char M_LINKS88_SETUP[]		= { 0x00, 0x00, 0x03, 0x00, 0x08, 0x53, 0x38, 0x38, 0x00, 0x0b, 0x00, 0x00, 0x00 };

unsigned char netframe[MAXDG];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 1.21\n\n");
    fprintf(stderr, "         -c <config_string>  config string like \"B1=1,T1=10,B2=3\"\n");
    fprintf(stderr, "                             means: B1=1  -> bus 1 length one module\n");
    fprintf(stderr, "                                    T1=10 -> bus 1 cycle time 10ms\n");
    fprintf(stderr, "                                    B2=3  -> bus 2 length three modules\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -d                  daemonize\n");
    fprintf(stderr, "         -e #no_of_links88   exit after no of LinkS88 responded - default 1\n\n");
}

struct s88_bus_t {
    int length;
    int interval;
    int tcyc;
};

struct node {
    int id;
    struct node *next;
};

struct node *insert_right(struct node *list, int id) {
    struct node *new_node = (struct node *)calloc(1, sizeof(struct node));
    new_node->id = id;
    new_node->next = list->next;
    list->next = new_node;
    return new_node;
}

void free_list(struct node *list) {
    struct node *p, *next_node;
    for (p = list->next; p != NULL; p = next_node) {
	next_node = p->next;
	free(p);
    }
    list->next = NULL;
}

struct node *search_node(struct node *list, int id) {
    while (list != NULL) {
	if (list->id == id)
	    return list;
	list = list->next;
    }
    return NULL;
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

int send_can_frame(int can_socket, struct can_frame *frame, int verbose) {
    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
	fprintf(stderr, "error writing CAN frame: %s\n", strerror(errno));
	return -1;
    }
    if (verbose)
	print_can_frame(T_CAN_FORMAT_STRG, frame);
    return 0;
}

int send_defined_can_frame(int can_socket, unsigned char *data, int verbose) {
    struct can_frame frame;
    uint32_t can_id;
    memset(&frame, 0, sizeof(frame));
    memcpy(&can_id, &data[0], 4);
    frame.can_id = htonl(can_id);
    frame.can_dlc = data[4];
    memcpy(&frame.data, &data[5], 8);
    send_can_frame(can_socket, &frame, verbose);
    return 0;
}

int main(int argc, char **argv) {
    int i, links88_id, sc, max_fds, opt;
    struct can_frame frame;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    char *config_string;
    char *token;
    fd_set read_fds;
    struct s88_bus_t s88_bus[3];

    int background = 0;
    int verbose = 1;
    int exit_on_wake_up = 1;
    int known_links88_ids = 0;
    unsigned char links88_id_h = 0;
    unsigned char links88_id_l = 0;
    unsigned char raw_frame[13];
    struct node *links88_head, *links88_list;
    struct timespec to_wait;

    config_string = NULL;
    memset(&s88_bus[0], 0, sizeof(s88_bus[0]));
    memset(&s88_bus[1], 0, sizeof(s88_bus[1]));
    memset(&s88_bus[2], 0, sizeof(s88_bus[2]));
    to_wait.tv_sec = 0;
    to_wait.tv_nsec = SLEEPING * 1000;

    links88_head = (struct node *)calloc(1, sizeof(struct node));
    if (links88_head == NULL) {
	fprintf(stderr, "can't malloc LinkS88 node\n");
	exit(EXIT_FAILURE);
    }

    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "c:i:de:h?")) != -1) {
	switch (opt) {
	case 'c':
	    config_string = strdup(optarg);
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'd':
	    verbose = 0;
	    background = 1;
	    break;
	case 'e':
	    exit_on_wake_up = atoi(optarg);
	    if (!exit_on_wake_up) {
		fprintf(stderr, "wrong or missing number of LinkS88: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
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

    /* mini config string parser */
    if (config_string != NULL) {
	while ((token = strsep(&config_string, delimiters))) {
	    if (*token == 'B') {
		token++;
		i = *token - '0';
		if ((i > 0) && (i < 4)) {
		    token++;
		    s88_bus[i - 1].length = (int)strtoul(++token, (char **)NULL, 10);
		    printf("bus %d length %d\n", i, s88_bus[i - 1].length);
		}
	    } else if (*token == 'T') {
		token++;
		i = *token - '0';
		if ((i > 0) && (i < 4)) {
		    token++;
		    s88_bus[i - 1].tcyc = (int)strtoul(++token, (char **)NULL, 10);
		    if (s88_bus[i - 1].tcyc > TCYC_MAX) {
			fprintf(stderr, "Cycle time %d ms greater than TCYC_MAX of %d ms\n", s88_bus[i - 1].tcyc, TCYC_MAX);
			exit(EXIT_FAILURE);
		    }
		    printf("bus %d Tcyc %d ms\n", i, s88_bus[i - 1].tcyc);
		}
	    }
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

    /* daemonize the process if requested */
    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    FD_ZERO(&read_fds);
    FD_SET(sc, &read_fds);
    max_fds = sc;

    /* initiate a CAN Ping */
    nanosleep(&to_wait, NULL);
    memcpy(raw_frame, M_CAN_BOOTLOADER, 13);
    send_defined_can_frame(sc, raw_frame, verbose);

    memcpy(raw_frame, M_CAN_PING, 13);
    nanosleep(&to_wait, NULL);
    send_defined_can_frame(sc, raw_frame, verbose);

    while (1) {
	if (select(max_fds + 1, &read_fds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		if (verbose) {
		    print_can_frame(F_CAN_FORMAT_STRG, &frame);
		}

		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x31:
		    /* looking for already known LinkS88 */
		    if ((memcmp(&frame.data[0], &M_LINKS88_ID[5], 2) == 0) && (frame.can_dlc == 8)) {
			links88_id_h = frame.data[2];
			links88_id_l = frame.data[3];
			links88_id = frame.data[3] + ((frame.data[2] - 0x38) << 8);
			links88_list = links88_head;
			if (search_node(links88_list, links88_id) == NULL) {
			    printf("inserting awoken LinkS88 ID %d (0x%02x%02x -> 0x%04x) into list\n", links88_id, links88_id_h, links88_id_l, links88_id);
			    insert_right(links88_list, links88_id);
			    known_links88_ids++;
			    /* now send the setup */
			    /* the bus length first */
			    printf("configure LinkS88 ID %d (0x%04x) \n", links88_id, links88_id);
			    for (i = 2; i < 5; i++) {
				if (s88_bus[i - 2].length) {
				    memcpy(raw_frame, M_LINKS88_SETUP, 13);
				    raw_frame[7] = links88_id_h;
				    raw_frame[8] = links88_id_l;
				    raw_frame[10] = i;
				    raw_frame[12] = s88_bus[i - 2].length & 0xff;
				    nanosleep(&to_wait, NULL);
				    send_defined_can_frame(sc, raw_frame, verbose);
				}
			    }
			    /* the cycle time next */
			    for (i = 5; i < 8; i++) {
				if (s88_bus[i - 5].tcyc) {
				    memcpy(raw_frame, M_LINKS88_SETUP, 13);
				    raw_frame[7] = links88_id_h;
				    raw_frame[8] = links88_id_l;
				    raw_frame[10] = i;
				    raw_frame[11] = (s88_bus[i - 5].tcyc >> 8) & 0x3;
				    raw_frame[12] =  s88_bus[i - 5].tcyc & 0xff;
				    nanosleep(&to_wait, NULL);
				    send_defined_can_frame(sc, raw_frame, verbose);
				}
			    }
			}
			if (known_links88_ids >= exit_on_wake_up) {
			    free_list(links88_head);
			    close(sc);
			    exit(EXIT_SUCCESS);
			} else {
			    printf("already know LinkS88 IDs: %d - exit on number of %d IDs\n", known_links88_ids, exit_on_wake_up);
			}
		    }
		    break;
		case 0x37:
		    if (frame.can_dlc == 8) {
			/* check if there is a response from a LinkS88
			   and it's unknown (didn't responded to a CAN ping ) */
			links88_id_h = frame.data[2];
			links88_id_l = frame.data[3];
			links88_id = links88_id_l + ((links88_id_h - 0x38) << 8);
			links88_list = links88_head;
			if ((memcmp(&frame.data[0], &M_LINKS88_ID[5], 2) == 0) &&
			    (search_node(links88_list, links88_id) == NULL)) {

			    if (verbose) {
				printf("Found LinkS88 ID: %d (0x%02x%02x -> 0x%04x)\n", links88_id, links88_id_h, links88_id_l, links88_id);
				printf("   sending wake-up sequence\n");
			    }

			    memcpy(raw_frame, M_LINKS88_WAKE_I, 13);
			    raw_frame[7] = links88_id_h;
			    raw_frame[8] = links88_id_l;
			    send_defined_can_frame(sc, raw_frame, verbose);
			    nanosleep(&to_wait, NULL);
			    memcpy(raw_frame, M_LINKS88_WAKE_II, 13);
			    raw_frame[7] = links88_id_h;
			    raw_frame[8] = links88_id_l;
			    send_defined_can_frame(sc, raw_frame, verbose);
			    nanosleep(&to_wait, NULL);
			    memcpy(raw_frame, M_LINKS88_WAKE_III, 13);
			    raw_frame[7] = links88_id_h;
			    raw_frame[8] = links88_id_l;
			    raw_frame[11] = links88_id_l;
			    nanosleep(&to_wait, NULL);
			    send_defined_can_frame(sc, raw_frame, verbose);
			    /* no send CAN Ping to trigger LinkS88 setup */
			    printf("   sending CAN ping\n");
			    memcpy(raw_frame, M_CAN_PING, 13);
			    nanosleep(&to_wait, NULL);
			    send_defined_can_frame(sc, raw_frame, verbose);
			}
		    }
		    break;
		default:
		    break;
		}
	    }
	}
    }
    return 0;
}
