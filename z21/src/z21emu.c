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
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <linux/can.h>

#include "utils.h"
#include "z21.h"
#include "read-cs2-config.h"

struct sockaddr_in *bsa;
pthread_mutex_t lock;

struct z21_data_t z21_data;

static char *UDP_SRC_STRG	= "->UDP    len 0x%04x ID 0x%04x";
static char *UDP_DST_STRG	= "  UDP->  len 0x%04x ID 0x%04x";
static char *TCP_FORMAT_STRG	= "->TCP    CANID 0x%06X   [%d]";
static char *TCP_FORMATS_STRG	= "->TCP*   CANID 0x%06X   [%d]";

#define MAXLINE	256
char cs2addr[32] = "127.0.0.1";
char config_dir[MAXLINE] = "/www/config/";

static unsigned char MS_POWER_ON[]	= { 0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00 };
static unsigned char MS_POWER_OFF[]	= { 0x00, 0x00, 0x03, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char XPN_SERIAL_NUMBER_RESPONSE[] = { 0x08, 0x00, 0x10, 0x00, 0x4D, 0xC1, 0x02, 0x00 };
static unsigned char XPN_X_STATUS_CHANGED[]       = { 0x08, 0x00, 0x40, 0x00, 0x62, 0x22, 0x00, 0x40 };
static unsigned char XPN_X_BC_TRACK_POWER_OFF[]   = { 0x07, 0x00, 0x40, 0x00, 0x61, 0x00, 0x61 };
static unsigned char XPN_X_BC_TRACK_POWER_ON[]    = { 0x07, 0x00, 0x40, 0x00, 0x61, 0x01, 0x60 };
static unsigned char XPN_X_LOCO_INFO[]            = { 0x0E, 0x00, 0x40, 0x00, 0xEF, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char Z21_VERSION[]                = { 0x09, 0x00, 0x40, 0x00, 0xF3, 0x0A, 0x01, 0x23, 0xDB };

/*
static unsigned char XPN_X_BC_STOPPED[]           = { 0x07, 0x00, 0x40, 0x00, 0x81, 0x00, 0x81 };
*/

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -c config_dir -p <port> -s <port>\n", prg);
    fprintf(stderr, "   Version 0.2\n\n");
    fprintf(stderr, "         -c <config_dir>     set the config directory - default %s\n", config_dir);
    fprintf(stderr, "         -p <port>           primary UDP port for the server - default %d\n", PRIMARY_UDP_PORT);
    fprintf(stderr, "         -s <port>           secondary UDP port for the server - default %d\n", SECONDARY_UDP_PORT);
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface\n");
    fprintf(stderr, "         -i <CAN interface>  CAN interface\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

int send_broadcast(unsigned char *udpframe, char *format, int verbose) {
    int s;
    uint16_t length;

    length = le16(&udpframe[0]);
    s = sendto(z21_data.sb, udpframe, length, 0, (struct sockaddr *)&z21_data.sbaddr, sizeof(z21_data.sbaddr));

    if (s < 0) {
	fprintf(stderr, "UDP write error: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }
    if (s != length) {
    } else {
	print_udp_frame(format, length, udpframe);
    }
    return (EXIT_SUCCESS);
}

int send_xpn(unsigned char *data, int verbose) {
    unsigned char udpxpn[64];
    char *format;
    int length;

    length = le16(data);
    if (length > 64) {
	fprintf(stderr, "Xpn length to long: %d\n", length);
	return (EXIT_FAILURE);
    }
    memcpy(udpxpn, data, length);
    format = UDP_DST_STRG;
    send_broadcast(udpxpn, format, verbose);
    return (EXIT_SUCCESS);
}

int send_tcp_frame(unsigned char *frame, char *format, int verbose) {
    int on, s;
    uint16_t length = 13;

    s = sendto(z21_data.st, frame, length, 0, (struct sockaddr *)&z21_data.staddr, sizeof(z21_data.staddr));
    if (s < 0) {
	fprintf(stderr, "TCP write error: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }

    /* disable Nagle - force PUSH */
    if (setsockopt(z21_data.st, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }

    if (s != 13) {
    } else {
	print_udp_frame(format, length, frame);
    }
    return (EXIT_SUCCESS);
}

int send_can(unsigned char *data, int verbose) {
    unsigned char frame[16];

    memcpy(frame, data, 13);
    send_tcp_frame(frame, TCP_FORMAT_STRG, verbose);
    return (EXIT_SUCCESS);
}

int send_xpn_loco_info(uint16_t loco_id, int verbose) {
    unsigned char xpnframe[32];

    memcpy(xpnframe, XPN_X_LOCO_INFO, sizeof(XPN_X_LOCO_INFO));
    xpnframe[5] = loco_id >> 8;
    xpnframe[6] = loco_id & 0xff;
    /* TODO */

    xpnframe[13] = xor(&xpnframe[4], 8);
    send_xpn(xpnframe, verbose);

    return (EXIT_SUCCESS);
}

int send_can_loco_function(uint16_t loco_id, uint8_t function, uint8_t switchtype) {
    return (EXIT_SUCCESS);
}

int send_can_loco_drive(uint16_t loco_id, uint8_t direction, uint8_t step, uint8_t speed) {
    return (EXIT_SUCCESS);
}

int check_data_xpn(struct z21_data_t *z21_data, int udplength, int verbose) {
    uint16_t length, header, loco_id;
    uint8_t db0, xheader;
    unsigned char xpnframe[32];

    length = le16(&z21_data->udpframe[0]);
    header = le16(&z21_data->udpframe[2]);

    if (verbose)
	print_udp_frame(UDP_SRC_STRG, udplength, z21_data->udpframe);

    switch (header) {
    case LAN_GET_SERIAL_NUMBER:
	if (length == 4)
	    send_xpn(XPN_SERIAL_NUMBER_RESPONSE, verbose);
	break;
    case LAN_GET_CODE:
	break;
    case LAN_GET_HWINFO:
	break;
    case LAN_LOGOFF:
	break;
    case LAN_X_HEADER:
	xheader = z21_data->udpframe[4];
	switch (xheader) {
	case 0x21:
	    db0 = z21_data->udpframe[5];
	    switch (db0) {
	    case LAN_X_GET_STATUS:
		memcpy(xpnframe, XPN_X_STATUS_CHANGED, sizeof(XPN_X_STATUS_CHANGED));
		xpnframe[6] = z21_data->power ? 0x00 : 0x02;
		xpnframe[7] = xor(&xpnframe[4], 3);
		send_xpn(xpnframe, verbose);
		break;
	    case LAN_X_GET_FIRMWARE_VERSION:
		send_can(Z21_VERSION, verbose);
		break;
	    case LAN_X_SET_TRACK_POWER_ON:
		z21_data->power = 1;
		send_can(MS_POWER_ON, verbose);
		break;
	    case LAN_X_SET_TRACK_POWER_OFF:
		z21_data->power = 0;
		send_can(MS_POWER_OFF, verbose);
		break;
	    default:
		break;
	    }
	case LAN_X_GET_LOCO_INFO:
	    if (length == 9) {
		loco_id = be16(&z21_data->udpframe[6]);
		send_xpn_loco_info(loco_id, verbose);
	    }
	    break;
	case LAN_X_SET_LOCO:
	    if (length == 0x0A ) {
		loco_id = be16(&z21_data->udpframe[6]) & 0x3FFF;
	    	if (xpnframe[5] == LAN_X_SET_LOCO_FUNCTION) {
		    uint8_t switchtype = (xpnframe[5] >> 6) & 0x03;
		    uint8_t function = xpnframe[5] & 0x3F;
		    send_can_loco_function(loco_id, function, switchtype);
		} else if ((xpnframe[5] & 0xF0 ) == 0x10) {
		/* LAN_X_SET_LOCO_DRIVE */
		    uint8_t step = xpnframe[5] & 0x03;
		    uint8_t direction = xpnframe[8] >> 7;
		    uint8_t speed = xpnframe[8] & 0x7F;
		    send_can_loco_drive(loco_id, direction, step, speed);
		}
	    }
	    /* LAN_X_SET_LOCO */
	    break;
	}
	/* LAN_X_HEADER */
	break;
    default:
	break;
    }
    return (EXIT_SUCCESS);
}

int check_data_can(struct z21_data_t *z21_data, uint8_t *data, int verbose) {
    uint32_t uid;

    switch ((be32(data) & 0x00FF0000UL) >> 16)
    case 0x01:
	{
	    uid = be32(&data[5]);
	    switch (data[9]) {
	    case 0x00:
		uid ? printf("System: UID 0x%08X ", uid) : printf("System: alle ");
		printf("Stopp\n");
		send_xpn(XPN_X_BC_TRACK_POWER_OFF, verbose);
		break;
	    case 0x01:
		uid ? printf("System: UID 0x%08X ", uid) : printf("System: alle ");
		printf("Go\n");
		send_xpn(XPN_X_BC_TRACK_POWER_ON, verbose);
		break;
	    }
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

    while ((opt = getopt(argc, argv, "c:p:s:b:i:hf?")) != -1) {
	switch (opt) {
	case 'c':
            if (strnlen(optarg, MAXLINE) < MAXLINE) {
                strncpy(config_dir, optarg, sizeof(config_dir) - 1);
            } else {
                fprintf(stderr, "config file dir to long\n");
                exit(EXIT_FAILURE);
            }
            break;
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
	    printf("FD_ISSET sp, ret %d\n", ret);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else if (ret) {
		check_data_xpn(&z21_data, ret, z21_data.foreground);
		//print_udp_frame(UDP_SRC_STRG, z21_data.udpframe);
	    }
	    /* send_broadcast(z21_data.udpframe, UDP_DST_STRG, z21_data.foreground); */
	}
	/* received a UDP packet on secondary */
	if (FD_ISSET(z21_data.ss, &readfds)) {
	    ret = read(z21_data.ss, z21_data.udpframe, MAXDG);
	    printf("FD_ISSET ss, ret %d\n", ret);
	    if (ret < 0) {
		fprintf(stderr, "UDP read error: %s\n", strerror(errno));
		break;
	    } else {
		print_udp_frame(UDP_SRC_STRG, ret, z21_data.udpframe);
	    }
	    /* TODO */
	    send_broadcast(z21_data.udpframe, UDP_DST_STRG, z21_data.foreground);
	}

	if (FD_ISSET(z21_data.st, &readfds)) {
	    int i, n;
	    n = recv(z21_data.st, recvline, MAXSIZE, 0);
	    printf("FD_ISSET st, n %d\n", n);
	    if (n > 0) {
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
			check_data_can(&z21_data, &recvline[i], z21_data.foreground);
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
