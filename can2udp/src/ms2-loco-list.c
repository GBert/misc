/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 */

#define _GNU_SOURCE
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
#include <ifaddrs.h>

#include "cs2-config.h"
#include "read-cs2-config.h"

#define MAX_LINE		1024
#define MAX_BUFFER		4096
#define CAN_ENCAP_SIZE		13	/* maximum datagram size */
#define MAXIPLEN		40	/* maximum IP string length */
#define MAX_UDP_BCAST_RETRY  	10
#define MAX(a,b)	((a) > (b) ? (a) : (b))

struct ms2_data_t {
    char *loco_file;
    char *config_data;
    char loco_name[32];
    int size;
    int sb;
    int sc;
    int loco_list_low;
    int loco_list_high;
    uint16_t hash;
    uint16_t crc;
    int verbose;
};

extern struct loco_data_t *loco_data;
uint16_t CRCCCITT(uint8_t * data, size_t length, uint16_t seed);

char loco_dir[MAX_LINE];

static char *F_CAN_FORMAT_STRG   = "      CAN->  CANID 0x%08X R [%d]";
static char *F_S_CAN_FORMAT_STRG = "short CAN->  CANID 0x%08X R [%d]";
static char *T_CAN_FORMAT_STRG   = "      CAN<-  CANID 0x%08X   [%d]";

static unsigned char M_GET_CONFIG[] = "loknamen";
static char M_GET_LOCO_INFO[]       = "lokinfo";

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.99\n\n");
    fprintf(stderr, "         -c <loco_dir>       set the locomotive file dir - default %s\n", loco_dir);
    fprintf(stderr, "         -i <can int>        can interface - default vcan1\n");
    fprintf(stderr, "         -d                  daemonize\n\n");
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void toc32(uint8_t *data, uint32_t x) {
    data[0] = (x & 0xFF000000) >> 24;
    data[1] = (x & 0x00FF0000) >> 16;
    data[2] = (x & 0x0000FF00) >> 8;
    data[3] =  x & 0x000000FF;
}

void toc16(uint8_t *data, uint16_t x) {
    data[0] = (x & 0xFF00) >> 8;
    data[1] =  x & 0x00FF;
}

unsigned int read_pipe(FILE *input, char *d, size_t n) {
    unsigned int i = 0;

    while (((*d++ = fgetc(input)) != EOF) && (i < n))
	i++;
    /* TODO */
    *--d = 0;

    memset(d, 0, 8);
    return i;
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

int send_frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame) {
    int s;
    uint8_t netframe[16];

    memset(netframe, 0, sizeof(netframe));
    frame->can_id &= CAN_EFF_MASK;
    toc32(netframe, frame->can_id);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);

    /* send TCP/UDP frame */
    s = sendto(net_socket, netframe, CAN_ENCAP_SIZE, 0, net_addr, sizeof(*net_addr));
    if (s != CAN_ENCAP_SIZE) {
	fprintf(stderr, "error sending TCP/UDP data: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int send_config_data(struct ms2_data_t *ms2_data) {
    int i;
    struct can_frame frame;

    frame.can_id = 0x00420300 | ms2_data->hash;
    frame.can_dlc = 6;
    toc32(&frame.data[0], ms2_data->size);
    toc16(&frame.data[4], ms2_data->crc);
    send_can_frame(ms2_data->sc, &frame, ms2_data->verbose);

    frame.can_dlc = 8;
    for (i = 0; i < ms2_data->size; i += 8) {
	memcpy(frame.data, &ms2_data->config_data[i], 8);
	usleep(2000);
	send_can_frame(ms2_data->sc, &frame, ms2_data->verbose);
    }
    return (EXIT_SUCCESS);
}

int get_loco_list(struct ms2_data_t *ms2_data) {
    int fdp[2], nframes;
    FILE *input_fd, *output_fd;

    if (read_loco_data(ms2_data->loco_file, CONFIG_FILE))
	fprintf(stderr, "can't read loco_file %s\n", ms2_data->loco_file);

    if ((pipe(fdp) == -1)) {
	fprintf(stderr, "setup pipe error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (ms2_data->verbose) {
	printf("Loco list size: %d\n", HASH_COUNT(loco_data));
    }

    input_fd = fdopen(fdp[1], "w");
    output_fd = fdopen(fdp[0], "r");

    show_loco_names(input_fd, ms2_data->loco_list_low, ms2_data->loco_list_high);
    fclose(input_fd);

    ms2_data->size = read_pipe(output_fd, ms2_data->config_data, MAX_BUFFER);

    nframes = (ms2_data->size + 7) / 8;
    ms2_data->crc = CRCCCITT((unsigned char *)ms2_data->config_data, nframes * 8, 0xffff);

    if (ms2_data->verbose)
	printf("Length %d CRC 0x%04X\n", ms2_data->size, ms2_data->crc);

    fclose(output_fd);
    return (EXIT_SUCCESS);
}

int get_loco_by_name(struct ms2_data_t *ms2_data) {
    int fdp[2], nframes;
    FILE *input_fd, *output_fd;

    if ((pipe(fdp) == -1)) {
	fprintf(stderr, "setup pipe error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    input_fd = fdopen(fdp[1], "w");
    output_fd = fdopen(fdp[0], "r");
    print_loco_by_name(input_fd, ms2_data->loco_name, MS2FKT);
    fclose(input_fd);

    ms2_data->size = read_pipe(output_fd, ms2_data->config_data, MAX_BUFFER);

    nframes = (ms2_data->size + 7) / 8;
    ms2_data->crc = CRCCCITT((unsigned char *)ms2_data->config_data, nframes * 8, 0xffff);

    if (ms2_data->verbose)
	printf("Length %d CRC 0x%04X\n", ms2_data->size, ms2_data->crc);
    fclose(output_fd);
    return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int max_fds, opt;
    uint16_t hash1, hash2;
    char *bcast_interface;
    char *udp_dst_address;
    struct can_frame frame;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    fd_set read_fds;
    struct ms2_data_t ms2_data;

    hash1 = hash2 = 0;
    int background = 0;
    strcpy(ifr.ifr_name, "can0");
    strcpy(loco_dir, "/www/config");

    memset(&ms2_data, 0, sizeof(ms2_data));

    ms2_data.config_data = calloc(MAX_BUFFER, 1);
    ms2_data.verbose = 1;

    udp_dst_address = (char *)calloc(MAXIPLEN, 1);
    if (!udp_dst_address) {
	fprintf(stderr, "can't alloc memory for udp_dst_address: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    };

    bcast_interface = (char *)calloc(MAXIPLEN, 1);
    if (!bcast_interface) {
	fprintf(stderr, "can't alloc memory for bcast_interface: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    };

    while ((opt = getopt(argc, argv, "c:i:dh?")) != -1) {
	switch (opt) {
	case 'c':
	    if (strnlen(optarg, MAX_LINE) < MAX_LINE) {
		strncpy(loco_dir, optarg, sizeof(loco_dir) - 1);
	    } else {
		fprintf(stderr, "config file dir to long\n");
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'd':
	    ms2_data.verbose = 0;
	    background = 1;
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

    /* prepare reading lokomotive.cs */
    if (asprintf(&ms2_data.loco_file, "%s/%s", loco_dir, "lokomotive.cs2") < 0) {
	fprintf(stderr, "can't alloc buffer for loco_name: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare CAN socket */
    memset(&caddr, 0, sizeof(caddr));
    if ((ms2_data.sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_family = AF_CAN;
    if (ioctl(ms2_data.sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(ms2_data.sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
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
    FD_SET(ms2_data.sc, &read_fds);
    max_fds = ms2_data.sc;

    while (1) {
	if (select(max_fds + 1, &read_fds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* received a CAN frame */
	if (FD_ISSET(ms2_data.sc, &read_fds)) {
	    if (read(ms2_data.sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		if (ms2_data.verbose) {
		    print_can_frame(F_CAN_FORMAT_STRG, &frame);
		}

		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x41:
		    if ((frame.can_dlc == 8) && (memcmp(&frame.data[0], M_GET_CONFIG, 8) == 0))
			hash1 = frame.can_id & 0XFFFF;
		    if (((frame.can_id & 0xFFFF) == hash1) && (frame.can_dlc == 4)) {
			sscanf((char *)frame.data, "%d %d", &ms2_data.loco_list_low, &ms2_data.loco_list_high);
			if (ms2_data.verbose)
			    printf("requesting locolist from %d to %d\n", ms2_data.loco_list_low, ms2_data.loco_list_high);
			ms2_data.hash = hash1;
			frame.can_id = 0x00410300;
			send_can_frame(ms2_data.sc, &frame, ms2_data.verbose);
			get_loco_list(&ms2_data);
			send_config_data(&ms2_data);
			hash1 = 0;	/* back to beginning */
		    }
		    if ((frame.can_dlc == strlen(M_GET_LOCO_INFO)) && (memcmp(&frame.data[0], M_GET_LOCO_INFO, strlen(M_GET_LOCO_INFO)) == 0)) {
			hash2 = frame.can_id & 0XFFFF;
			memset(ms2_data.loco_name, 0, sizeof(ms2_data.loco_name));
			ms2_data.hash = hash2;
		    } else if ((frame.can_id & 0xFFFF) == hash2) {
			if (ms2_data.loco_name[0]) {
			    memcpy(&ms2_data.loco_name[8], frame.data, frame.can_dlc);
			    if (ms2_data.verbose)
				printf("sending lokinfo >%s<\n", ms2_data.loco_name);
			    hash2 = 0;
			    frame.can_id = 0x00410300;
			    send_can_frame(ms2_data.sc, &frame, ms2_data.verbose);
			    get_loco_by_name(&ms2_data);
			    send_config_data(&ms2_data);
			} else {
			    memcpy(ms2_data.loco_name, frame.data, frame.can_dlc);
			}
		    }
		    break;
		default:
		    break;
		}
	    } else
		print_can_frame(F_S_CAN_FORMAT_STRG, &frame);
	}
    }
    close(ms2_data.sc);
    return 0;
}
