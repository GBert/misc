/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return.
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*
 * Z21 Emulation for Roco WiFi Mouse
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <net/if.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/can.h>

#include "utils.h"
#include "z21.h"

struct sockaddr_in *bsa;
pthread_mutex_t lock;

struct z21_data_t z21_data;

static char *UDP_SRC_STRG     = "->UDP   ";
static char *UDP_DST_STRG     = "  UDP-> ";
static char *TCP_FORMAT_STRG  = "->TCP    CANID 0x%06X   [%d]";
static char *TCP_FORMATS_STRG = "->TCP*   CANID 0x%06X   [%d]";

char cs2addr[32] = "127.0.0.1";

#if 0
static unsigned char Z21_VERSION[] = { 0x09, 0x00, 0x40, 0x00, 0xF3, 0x0A, 0x01, 0x23, 0xDB };
#endif

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -p <port> -s <port>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p <port>           primary UDP port for the server - default %d\n", PRIMARY_UDP_PORT);
    fprintf(stderr, "         -s <port>           secondary UDP port for the server - default %d\n", SECONDARY_UDP_PORT);
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface\n");
    fprintf(stderr, "         -i <CAN interface>  CAN interface\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

int send_broadcast(unsigned char *udpframe, char *format, int verbose) {
    int s;
    uint16_t length;

    length = udpframe[0] + (udpframe[1] << 8);
    s = sendto(z21_data.sb, udpframe, length, 0, (struct sockaddr *)&z21_data.sbaddr, sizeof(z21_data.sbaddr));

    if (s < 0) {
	fprintf(stderr, "UDP write error: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }
    if (s != length) {
    } else {
	print_udp_frame(format, udpframe);
    }
    return (EXIT_SUCCESS);
}

int check_data(struct z21_data_t *z21_data, int verbose) {
    uint16_t length, header;
    uint8_t xheader;

    length = be16(&z21_data->udpframe[0]);
    header = be16(&z21_data->udpframe[2]);

    if (verbose)
	printf("Z21 Data Header 0x%04x length %d\n", header, length);

    switch (header) {
    case LAN_X_HEADER:
	xheader = z21_data->udpframe[4];
	switch (xheader) {
	case LAN_X_GET_FIRMWARE_VERSION:
	    break;
	default:
	    break;
	}
	break;
    case LAN_GET_SERIAL_NUMBER:
	break;
    default:
	break;
    }
    return (EXIT_SUCCESS);
}

void *z21_periodic_tasks(void *ptr) {
    struct z21_data_t *z21_data = (struct z21_data_t *)ptr;

    while (1) {
	usec_sleep(1E6);
	if (z21_data->sb) {
	    /* printf("Z21 periodic wakeup\n"); */
	}
    }
}

int main(int argc, char **argv) {
    pid_t pid;
    pthread_t pth;
    int ret, opt, max_fds;
    /* primary UDP socket , secondary UDP socket, UDP broadcast socket */
    struct ifaddrs *ifap, *ifa;
    struct ifreq ifr;
    struct sockaddr_can caddr;
    struct sockaddr_in *bsa;
    fd_set readfds;
    int primary_port = PRIMARY_UDP_PORT;
    int secondary_port = SECONDARY_UDP_PORT;
    const int on = 1;
    char *udp_dst_address;
    char *bcast_interface;
    unsigned char recvline[MAXSIZE];
    char timestamp[16];

    socklen_t caddrlen = sizeof(caddr);
    memset(&ifr, 0, sizeof(ifr));
    memset(&z21_data, 0, sizeof(z21_data));
    z21_data.foreground = 1;

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

    while ((opt = getopt(argc, argv, "p:s:b:i:hf?")) != -1) {
	switch (opt) {
	case 'p':
	    primary_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 's':
	    secondary_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'b':
	    if (strnlen(optarg, MAXIPLEN) <= MAXIPLEN - 1) {
		/* broadcat IP begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
		    memset(udp_dst_address, 0, MAXIPLEN);
		    strncpy(udp_dst_address, optarg, MAXIPLEN - 1);
		} else {
		    memset(bcast_interface, 0, MAXIPLEN);
		    strncpy(bcast_interface, optarg, MAXIPLEN - 1);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'f':
	    z21_data.foreground = 1;
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

    /* prepare primary UDP socket */
    z21_data.sp = socket(PF_INET, SOCK_DGRAM, 0);
    if (z21_data.sp < 0) {
	fprintf(stderr, "primary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    z21_data.spaddr.sin_family = AF_INET;
    z21_data.spaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    z21_data.spaddr.sin_port = htons(primary_port);

    if (bind(z21_data.sp, (struct sockaddr *)&z21_data.spaddr, sizeof(z21_data.spaddr)) < 0) {
	fprintf(stderr, "primary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare secondary UDP socket */
    z21_data.ss = socket(PF_INET, SOCK_DGRAM, 0);
    if (z21_data.ss < 0) {
	fprintf(stderr, "secondary UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    z21_data.ssaddr.sin_family = AF_INET;
    z21_data.ssaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    z21_data.ssaddr.sin_port = htons(secondary_port);

    if (bind(z21_data.ss, (struct sockaddr *)&z21_data.ssaddr, sizeof(z21_data.ssaddr)) < 0) {
	fprintf(stderr, "scondary UDP bind error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare UDP sending socket */
    /* get the broadcast address */
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
	if (ifa->ifa_addr) {
	    if (ifa->ifa_addr->sa_family == AF_INET) {
		bsa = (struct sockaddr_in *)ifa->ifa_broadaddr;
		if (strncmp(ifa->ifa_name, bcast_interface, strlen(bcast_interface)) == 0)
		    udp_dst_address = inet_ntoa(bsa->sin_addr);
	    }
	}
    }
    freeifaddrs(ifap);

    z21_data.sbaddr.sin_family = AF_INET;
    z21_data.sbaddr.sin_port = htons(primary_port);

    ret = inet_pton(AF_INET, udp_dst_address, &z21_data.sbaddr.sin_addr);
    if (ret <= 0) {
	if (ret == 0)
	    fprintf(stderr, "UDP IP invalid\n");
	else
	    fprintf(stderr, "invalid address family\n");
	exit(EXIT_FAILURE);
    }

    z21_data.sb = socket(AF_INET, SOCK_DGRAM, 0);
    if (z21_data.sb < 0) {
	fprintf(stderr, "sending UDP socket error %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(z21_data.sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (strlen(ifr.ifr_name)) {
	/* prepare CAN socket */
	printf("ifr.ifr_name: >%s<\n", ifr.ifr_name);
	if ((z21_data.sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	    fprintf(stderr, "error creating CAN socket: %s on >%s<\n", strerror(errno), ifr.ifr_name);
	    exit(EXIT_FAILURE);
	}
	caddr.can_family = AF_CAN;
	if (ioctl(z21_data.sc, SIOCGIFINDEX, &ifr) < 0) {
	    fprintf(stderr, "setup CAN socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	caddr.can_ifindex = ifr.ifr_ifindex;

	if (bind(z21_data.sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	    fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    } else {
	/* prepare TCP client socket */
	if ((z21_data.st = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	    fprintf(stderr, "can't create TCP socket: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	memset(&z21_data.staddr, 0, sizeof(z21_data.staddr));
	z21_data.staddr.sin_family = AF_INET;

	if (inet_aton(cs2addr, (struct in_addr *)&z21_data.staddr.sin_addr.s_addr) == 0) {
	    fprintf(stderr, "invalid address : %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	z21_data.staddr.sin_port = htons(MAERKLIN_PORT);

	if (connect(z21_data.st, (struct sockaddr *)&z21_data.staddr, sizeof(z21_data.staddr))) {
	    fprintf(stderr, "can't connect to TCP socket : %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    if (pthread_mutex_init(&lock, NULL)) {
	fprintf(stderr, "can't init mutex %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (pthread_create(&pth, NULL, z21_periodic_tasks, &z21_data)) {
	fprintf(stderr, "can't create pthread %s\n", strerror(errno));
	exit(EXIT_FAILURE);

    }
    if (z21_data.foreground) {
	printf("created periodic z21 thread\n");
    }

    if (!z21_data.foreground) {
	/* Fork off the parent process */
	pid = fork();
	if (pid < 0)
	    exit(EXIT_FAILURE);
	/* If we got a good PID, then we can exit the parent process */

	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(EXIT_SUCCESS);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(z21_data.sp, &readfds);
	FD_SET(z21_data.ss, &readfds);
	if (z21_data.st) {
	    FD_SET(z21_data.st, &readfds);
	    max_fds = MAX(MAX(z21_data.sp, z21_data.ss), z21_data.st);
	} else {
	    FD_SET(z21_data.sc, &readfds);
	    max_fds = MAX(MAX(z21_data.sp, z21_data.ss), z21_data.sc);
	}

	if (select(max_fds + 1, &readfds, NULL, NULL, NULL) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	    break;
	}

	if (FD_ISSET(z21_data.sc, &readfds)) {
	}

	/* received a UDP packet on primary */
	if (FD_ISSET(z21_data.sp, &readfds)) {
	    ret = read(z21_data.sp, z21_data.udpframe, MAXDG);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		print_udp_frame(UDP_SRC_STRG, z21_data.udpframe);
	    }
	    /* send_broadcast(z21_data.udpframe, UDP_DST_STRG, z21_data.foreground); */
	}
	/* received a UDP packet on secondary */
	if (FD_ISSET(z21_data.ss, &readfds)) {
	    ret = read(z21_data.ss, z21_data.udpframe, MAXDG);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		print_udp_frame(UDP_SRC_STRG, z21_data.udpframe);
	    }
	    /* TODO */
	    send_broadcast(z21_data.udpframe, UDP_DST_STRG, z21_data.foreground);
	}

	if (FD_ISSET(z21_data.st, &readfds)) {
	    int i, n;
	    if ((n = recv(z21_data.st, recvline, MAXSIZE, 0)) > 0) {
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
		    }
		}
	    }
	}
    }
    close(z21_data.sp);
    close(z21_data.ss);
    close(z21_data.sb);
    pthread_join(pth, (void *)&z21_data);
    pthread_mutex_unlock(&lock);
    /* if we reach this point, there was an error */
    return EXIT_FAILURE;
}
