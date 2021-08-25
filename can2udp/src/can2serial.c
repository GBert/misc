/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*   CC-Schnitte Emulation
 */

#include "can2serial.h"

static char *CAN_FORMAT_STRG     = "<-SER<CAN    CANID 0x%06X   [%d]";
static char *TCP_FORMAT_STRG     = "->TCP>SER    CANID 0x%06X   [%d]";
static char *TCP_FORMATS_STRG    = "->TCP>SER*   CANID 0x%06X   [%d]";
static char *SER_CAN_FORMAT_STRG = "->SER>CAN    CANID 0x%06X   [%d]";
static char *SER_TCP_FORMAT_STRG = "->SER>TCP    CANID 0x%06X   [%d]";

struct timeval last_sent_can;
struct timeval last_sent_serial;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf -t <tcp_port> -a <IP addr> -i <can interface> -s <serial interface>\n", prg);
    fprintf(stderr, "   Version 0.9\n\n");
    fprintf(stderr, "         -t <port>           TCP port for the server - default 15731\n");
    fprintf(stderr, "         -a <IP addr>        IP address\n");
    fprintf(stderr, "         -i <can int>        CAN interface - default can0\n");
    fprintf(stderr, "         -w                  hardware flow-control CTS/RTS\n");
    fprintf(stderr, "         -s <serial int>     serial interface - default /dev/ttyGS0\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
    fprintf(stderr, "         -v                  verbose output (in foreground)\n\n");
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

void print_can_frame(char *format_string, unsigned char *netframe, int verbose) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    if (!verbose)
	return;

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
	for (i = 6 + dlc; i < CAN_ENCAP_SIZE; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("  ");
    for (i = 5; i < CAN_ENCAP_SIZE; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar('.');
    }

    printf("\n");
}

int frame_to_serial(int fd, unsigned char *netframe) {
    struct timespec to_wait;
    struct timeval actual_time;
    long usec;

    /* we calculate the difference between the actual time and the time the last command was sent */
    /* probably we don't need to wait anymore before putting next CAN frame on the serial interface */
    gettimeofday(&actual_time, NULL);
    usec = (actual_time.tv_sec - last_sent_serial.tv_sec) * 1000000;
    usec += (actual_time.tv_usec - last_sent_serial.tv_usec);
    if (usec < TIME_WAIT_SERIAL_US) {
        to_wait.tv_sec = 0;
        to_wait.tv_nsec = (TIME_WAIT_SERIAL_US - usec) * 1000;
        nanosleep(&to_wait, NULL);
    }

    if (write(fd, netframe, 13) != 13) {
	fprint_syslog_wc(stderr, LOG_ERR, "error sennding serial frame:", strerror(errno));
	return -1;
    }
    gettimeofday(&last_sent_serial, NULL);
    return 0;
}

int frame_to_can(int can_socket, unsigned char *netframe) {
    uint32_t canid;
    struct can_frame frame;
    struct timespec to_wait;
    struct timeval actual_time;
    long usec;

    /* Maerklin TCP/UDP Format: always 13 (CAN_ENCAP_SIZE) bytes
     *   byte 0 - 3  CAN ID
     *   byte 4      DLC
     *   byte 5 - 12 CAN data
     */
    memset(&frame, 0, sizeof(frame));
    memcpy(&canid, netframe, 4);
    /* CAN uses (network) big endian format */
    frame.can_id = ntohl(canid);
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = netframe[4];
    memcpy(&frame.data, &netframe[5], 8);

    /* we calculate the difference between the actual time and the time the last command was sent */
    /* probably we don't need to wait anymore before putting next CAN frame on the wire */
    gettimeofday(&actual_time, NULL);
    usec = (actual_time.tv_sec - last_sent_can.tv_sec) * 1000000;
    usec += (actual_time.tv_usec - last_sent_can.tv_usec);
    if (usec < TIME_WAIT_CAN_US) {
	to_wait.tv_sec = 0;
	to_wait.tv_nsec = (TIME_WAIT_CAN_US - usec) * 1000;
	nanosleep(&to_wait, NULL);
    }

    /* send CAN frame */
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprint_syslog_wc(stderr, LOG_ERR, "error CAN frame:", strerror(errno));
	return -1;
    }

    gettimeofday(&last_sent_can, NULL);
    return 0;
}

int rawframe_to_net(int net_socket, struct sockaddr *net_addr, unsigned char *netframe, int length) {
    int s;
    s = sendto(net_socket, netframe, length, 0, net_addr, sizeof(*net_addr));
    if (s != length) {
	fprintf(stderr, "%s: error sending TCP/UDP data; %s\n", __func__, strerror(errno));
	return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int n, i, max_fds, hw_flow, opt, nready;
    int background, verbose, ec_index = 0;
    char timestamp[16];
    struct termios term_attr;
    int sc, se, st;		/* CAN socket, TCP socket */
    struct sockaddr_in tcp_addr;
    struct sockaddr_can caddr;
    socklen_t caddrlen = sizeof(caddr);
    char if_name[MAXSTRING];
    char tcp_dst_address[16];
    struct ifreq ifr;
    /* socklen_t tcp_client_length = sizeof(tcp_addr); */
    fd_set all_fds, read_fds;
    uint32_t canid;
    int eci, ret, tcp_port;
    char buffer[64];
    unsigned char rawframe[64];
    struct can_frame frame;

    verbose = 0;
    background = 1;
    hw_flow = 0;
    memset(tcp_dst_address, 0, sizeof(tcp_dst_address));
    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    memset(&last_sent_can, 0, sizeof(last_sent_can));
    memset(&last_sent_serial, 0, sizeof(last_sent_serial));

    strcpy(ifr.ifr_name, "can0");
    tcp_port = 15731;
    strcpy(if_name, "/dev/ttyGS0");

    while ((opt = getopt(argc, argv, "a:t:s:i:vhf?")) != -1) {
	switch (opt) {
	case 'a':
	    strncpy(tcp_dst_address, optarg, sizeof(tcp_dst_address) - 1);
	    break;
	case 't':
	    tcp_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 's':
	    strncpy(if_name, optarg, sizeof(if_name) - 1);
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 'w':
	    hw_flow = 1;
	    break;
	case 'v':
	    verbose = 1;
	    break;
	case 'f':
	    background = 0;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    /* prepare simple serial CAN interface aka Schnitte */
    if ((se = open(if_name, O_RDWR | O_TRUNC | O_NONBLOCK | O_NOCTTY)) < 0) {
	fprintf(stderr, "opening serial interface >%s< error: %s\n", if_name, strerror(errno));
	exit(EXIT_FAILURE);
    } else {
	memset(&term_attr, 0, sizeof(term_attr));
	if (tcgetattr(se, &term_attr) < 0) {
	    fprintf(stderr, "can't get terminal settings error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (hw_flow)
	    term_attr.c_cflag = CS8 | CRTSCTS | CLOCAL | CREAD;
	else
	    term_attr.c_cflag = CS8 | CLOCAL | CREAD;
	term_attr.c_iflag = 0;
	term_attr.c_oflag = 0;
	term_attr.c_lflag = NOFLSH;
	if (cfsetospeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "serial interface ospeed error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (cfsetispeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "serial interface ispeed error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	if (tcsetattr(se, TCSANOW, &term_attr) < 0) {
	    fprintf(stderr, "serial interface set error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    if (tcp_dst_address[0]) {
	/* prepare TCP client socket */
	if ((st = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	    fprintf(stderr, "creating TCP socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	tcp_addr.sin_family = AF_INET;
	tcp_addr.sin_addr.s_addr = inet_addr(tcp_dst_address);
	tcp_addr.sin_port = htons(tcp_port);
	if (connect(st, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
	    fprintf(stderr, "connecting to TCP socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* don't use CAN directly */
	sc = 0;
    } else {
	/* prepare CAN socket */
	memset(&caddr, 0, sizeof(caddr));
	sc = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (sc < 0) {
	    fprintf(stderr, "creating CAN socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	caddr.can_family = AF_CAN;
	if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	    fprintf(stderr, "setup CAN error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	caddr.can_ifindex = ifr.ifr_ifindex;

	if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	    fprintf(stderr, "binding CAN socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* don't use TCP */
	st = 0;
    }

    /* daemonize the process if requested */
    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    FD_ZERO(&all_fds);
    FD_SET(se, &all_fds);
    if (st) {
	FD_SET(st, &all_fds);
	max_fds = MAX(se, st);
    } else {
	FD_SET(sc, &all_fds);
	max_fds = MAX(se, sc);
    }

    while (1) {
	read_fds = all_fds;
	if ((nready = select(max_fds + 1, &read_fds, NULL, NULL, NULL)) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	}

	/* serial interface */
	if (FD_ISSET(se, &read_fds)) {
	    while ((ret = read(se, buffer, sizeof(buffer))) > 0) {
		for (eci = 0; eci < ret; eci++) {
		    ec_frame[ec_index++] = (unsigned char)buffer[eci];
		    if (ec_index == 13) {
			/* we got a complete CAN frame */
			ec_index = 0;
			/* send frame to TCP socket if open */
			if (st) {
			    rawframe_to_net(st, (struct sockaddr *)&tcp_addr, ec_frame, 13);
			    if (!background)
				print_can_frame(SER_TCP_FORMAT_STRG, ec_frame, verbose);
			/* otherwise send frame to CAN socket */
			} else {
			    frame_to_can(sc, ec_frame);
			    if (!background)
				print_can_frame(SER_CAN_FORMAT_STRG, ec_frame, verbose);
			}
		    }
		}
	    }
	}

	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "CAN read error: %s\n", strerror(errno));
		break;
	    } else {
		/* prepare packet */
		memset(rawframe, 0, 13);
		canid = htonl(frame.can_id & 0x1FFFFFFF);
		memcpy(rawframe, &canid, 4);
		rawframe[4] = frame.can_dlc;
		memcpy(&rawframe[5], &frame.data, frame.can_dlc);

		/* send serial packet */
		if (write(se, rawframe, 13) != 13) {
		    fprintf(stderr, "serial write error: %s\n", strerror(errno));
		    break;
		}
		print_can_frame(CAN_FORMAT_STRG, rawframe, verbose);
	    }
	}

	/* received a TCP packet */
	if (FD_ISSET(st, &read_fds)) {
	    if (verbose && !background) {
		time_stamp(timestamp);
		printf("%s packet from: %s\n", timestamp,
		       inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
	    }
	    if ((n = read(st, netframe, MAXDG)) == 0) {
		/* TCP connection closed by server */
		if (verbose && !background) {
		    time_stamp(timestamp);
		    printf("%s server %s closed connection\n", timestamp,
			   inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		}
		close(st);
		FD_CLR(st, &all_fds);
		break;
	    } else {
		/* check the whole TCP packet, if there are more than one CAN frame included */
		/* TCP packets with size modulo 13 !=0 are ignored though */
		if (n % 13) {
		    time_stamp(timestamp);
		    fprintf(stderr, "%s received packet %% 13 : length %d\n", timestamp, n);
		} else {
		    for (i = 0; i < n; i += 13) {
			ret = frame_to_serial(se, &netframe[i]);
			if ((ret == 0) && (verbose && !background)) {
			    if (i > 0)
				print_can_frame(TCP_FORMATS_STRG, &netframe[i], verbose);
			    else
				print_can_frame(TCP_FORMAT_STRG, &netframe[i], verbose);
			}
		    }
		}
	    }
	}
    }
    close(sc);
    close(se);
    close(st);
    return 0;
}
