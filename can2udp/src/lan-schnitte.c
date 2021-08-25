/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* virtual CC-Schnitte */

#include "can2lan.h"

#define TIME_WAIT_US	10 * 1000
#define DEF_TCP_PORT	7970

static char *TCP_FORMAT_STRG	 = "->TCP > CAN     0x%08X   [%d]";
static char *TCP_FORMATS_STRG	 = "->TCP*> CAN     0x%08X   [%d]";
static char *CAN_TCP_FORMAT_STRG = "->CAN > TCP     0x%08X   [%d]";

struct timeval last_sent;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -t <tcp_port> -i <can interface>\n",
	    prg);
    fprintf(stderr, "   Version 0.99\n\n");
    fprintf(stderr, "         -t <port>           listening TCP port for the server - default %d\n", DEF_TCP_PORT);
    fprintf(stderr, "         -i <can int>        CAN interface - default can0\n");
    fprintf(stderr, "         -f                  running in foreground\n");
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

int frame_to_net(int net_socket, struct sockaddr *net_addr, struct can_frame *frame) {
    int s;
    uint32_t canid;

    memset(netframe, 0, CAN_ENCAP_SIZE);
    frame->can_id &= CAN_EFF_MASK;
    canid = htonl(frame->can_id);
    memcpy(netframe, &canid, 4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);

    /* send TCP/UDP frame */
    s = sendto(net_socket, netframe, CAN_ENCAP_SIZE, 0, net_addr, sizeof(*net_addr));
    if (s != CAN_ENCAP_SIZE) {
	fprintf(stderr, "%s: error sending TCP/UDP data: %s\n", __func__, strerror(errno));
	syslog(LOG_ERR, "%s: error sending TCP/UDP data: %s\n", __func__, strerror(errno));
	return -1;
    }
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
    usec = (actual_time.tv_sec - last_sent.tv_sec) * 1000000;
    usec += (actual_time.tv_usec - last_sent.tv_usec);
    if (usec < TIME_WAIT_US) {
	to_wait.tv_sec = 0;
	to_wait.tv_nsec = (TIME_WAIT_US - usec) * 1000;
	nanosleep(&to_wait, NULL);
    }

    /* send CAN frame */
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprintf(stderr, "%s: error writing CAN frame: %s\n", __func__, strerror(errno));
	syslog(LOG_ERR, "%s: error writing CAN frame: %s\n", __func__, strerror(errno));
	return -1;
    }

    gettimeofday(&last_sent, NULL);
    return 0;
}

int main(int argc, char **argv) {
    int n, i, max_fds, opt, max_tcp_i, nready, conn_fd, tcp_client[MAX_TCP_CONN];
    struct can_frame frame;
    char timestamp[16];
    /* UDP incoming socket , CAN socket, UDP broadcast socket, TCP socket */
    int sc, st, tcp_socket;
    struct sockaddr_in tcp_addr;
    /* vars for determing broadcast address */
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    socklen_t tcp_client_length = sizeof(tcp_addr);
    fd_set all_fds, read_fds;
    int ret;
    struct timeval tv;

    int local_tcp_port = DEF_TCP_PORT;
    int background = 1;
    /* const int off = 0; */
    char buffer[64];

    /* clear timestamp for last CAN frame sent */
    memset(&last_sent, 0, sizeof(last_sent));

    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strcpy(ifr.ifr_name, "can0");

    while ((opt = getopt(argc, argv, "t:i:vhf?")) != -1) {
	switch (opt) {
	case 't':
	    local_tcp_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
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

    /* prepare TCP socket */
    st = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (st < 0) {
	fprintf(stderr, "creating TCP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_addr.sin_port = htons(local_tcp_port);
    if (bind(st, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
	fprintf(stderr, "binding TCP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (listen(st, MAXPENDING) < 0) {
	fprintf(stderr, "starting TCP listener error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    /* prepare TCP clients array */
    max_tcp_i = -1;		/* index into tcp_client[] array */
    for (i = 0; i < MAX_TCP_CONN; i++)
	tcp_client[i] = -1;	/* -1 indicates available entry */

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

    /* daemonize the process if requested */
    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("lan-schnitte", LOG_CONS | LOG_NDELAY, LOG_DAEMON);

    /* set select timeout -> send periodic CAN Ping */
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO(&all_fds);
    FD_SET(sc, &all_fds);
    FD_SET(st, &all_fds);
    max_fds = MAX(sc, st);

    while (1) {
	read_fds = all_fds;
	nready = select(max_fds + 1, &read_fds, NULL, NULL, &tv);
	if (nready == 0) {
	    /*    send_can_ping(sc); */
	    tv.tv_sec = 1;
	    tv.tv_usec = 0;
	    continue;
	} else if (nready < 0)
	    fprintf(stderr, "select error: %s\n", strerror(errno));

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    /* reading via SockatCAN */
	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "reading CAN frame error: %s\n", strerror(errno));
		syslog(LOG_ERR, "%s: reading CAN frame error: %s\n", __func__, strerror(errno));
	    }
	    /* if CAN Frame is EFF do it */
	    if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		/* send CAN frame to all connected TCP clients */
		for (i = 0; i <= max_tcp_i; i++) {	/* check all clients for data */
		    tcp_socket = tcp_client[i];
		    if (tcp_socket < 0)
			continue;
		    frame_to_net(tcp_socket, (struct sockaddr *)&tcp_addr, (struct can_frame *)&frame);
		    print_can_frame(CAN_TCP_FORMAT_STRG, netframe, !background);
		}
	    }
	}

	/* received a TCP packet */
	if (FD_ISSET(st, &read_fds)) {
	    conn_fd = accept(st, (struct sockaddr *)&tcp_addr, &tcp_client_length);
	    if (!background) {
		printf("new client: %s, port %d conn fd: %d max fds: %d\n", inet_ntop(AF_INET, &(tcp_addr.sin_addr),
			buffer, sizeof(buffer)), ntohs(tcp_addr.sin_port), conn_fd, max_fds);
	    }
	    syslog(LOG_NOTICE, "%s: new client: %s port %d conn fd: %d max fds: %d\n", __func__,
		   inet_ntop(AF_INET, &(tcp_addr.sin_addr), buffer, sizeof(buffer)), ntohs(tcp_addr.sin_port),
		   conn_fd, max_fds);
	    for (i = 0; i < MAX_TCP_CONN; i++) {
		if (tcp_client[i] < 0) {
		    tcp_client[i] = conn_fd;	/* save new TCP client descriptor */
		    break;
		}
	    }
	    if (i == MAX_TCP_CONN) {
		fprintf(stderr, "too many TCP clients\n");
		syslog(LOG_ERR, "%s: too many TCP clients\n", __func__);
	    }

	    FD_SET(conn_fd, &all_fds);	/* add new descriptor to set */
	    max_fds = MAX(conn_fd, max_fds);	/* for select */
	    max_tcp_i = MAX(i, max_tcp_i);	/* max index in tcp_client[] array */

	    if (--nready <= 0)
		continue;		/* no more readable descriptors */
	}

	/* check for already connected TCP clients */
	for (i = 0; i <= max_tcp_i; i++) {	/* check all clients for data */
	    tcp_socket = tcp_client[i];
	    if (tcp_socket < 0)
		continue;
	    if (FD_ISSET(tcp_socket, &read_fds)) {
		if (!background) {
		    time_stamp(timestamp);
		    printf("%s packet from: %s\n", timestamp,
			   inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		}
		n = read(tcp_socket, netframe, MAXDG);
		if (!n) {
		    /* connection closed by client */
		    if (!background) {
			time_stamp(timestamp);
			printf("%s client %s closed connection\n", timestamp,
			       inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		    }
		    syslog(LOG_NOTICE, "%s: client %s closed connection\n", __func__,
			   inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		    close(tcp_socket);
		    FD_CLR(tcp_socket, &all_fds);
		    tcp_client[i] = -1;
		} else {
		    /* check the whole TCP packet, if there are more than one CAN frame included */
		    /* TCP packets with size modulo 13 !=0 are ignored though */
		    if (n % 13) {
			time_stamp(timestamp);
			if (!background)
			    fprintf(stderr, "%s received packet %% 13 : length %d - maybe close connection\n", timestamp, n);
			syslog(LOG_ERR, "%s: received packet %% 13 : length %d - maybe close connection\n", __func__, n);
		    } else {
			for (i = 0; i < n; i += CAN_ENCAP_SIZE) {
			    ret = frame_to_can(sc, &netframe[i]);
			    if (!ret) {
				if (i > 0)
				    print_can_frame(TCP_FORMATS_STRG, &netframe[i], !background);
				else
				    print_can_frame(TCP_FORMAT_STRG, &netframe[i], !background);
			    }
			}
		    }
		}
		if (--nready <= 0)
		    break;	/* no more readable descriptors */
	    }
	}
    }
    closelog();
    close(sc);
    close(st);
    return 0;
}
