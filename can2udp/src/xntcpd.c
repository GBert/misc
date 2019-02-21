/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>

#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#define XNTCPPORT	61235
#define MAXDG   	4096
#define MAXPENDING	16
#define SMALL_BUFFER	16
#define MAX_TCP_CONN	16
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define fprint_syslog(pipe, spipe, text) \
	syslog( spipe, "%s: " text "\n", __func__); } while (0)

#define TERM_SPEED	B115200

static unsigned char XNTP_VERSION[] = { 0x02, 0x01, 0x80, 0x00 };

int tcp_client[MAX_TCP_CONN];
int max_tcp_i, se, st;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -p <tcp_port> -i <RS485 interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -p <port>           listening TCP port for the server - default %d\n", XNTCPPORT);
    fprintf(stderr, "         -i <RS485 int>      RS485 interface - default /dev/ttyUSB0\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
}

void INThandler(int sig) {
    int i;

    signal(sig, SIG_IGN);
    for (i = 0; i <= max_tcp_i; i++) {
	if (tcp_client[i])
	    close(tcp_client[i]);
    }
    close(st);
    close(se);
    exit(0);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

void add_crc(uint8_t *data, int length) {
    int i;
    uint8_t xor = 0;

    for (i = 0; i < (length - 2); i++) {
	xor = xor ^ *data;
	data++;
    }
    *data = xor;
}

#if 0
uint8_t add_parity(uint8_t data) {
    uint8_t temp, parity = 0;
    data &= 0x7f;
    temp = data;

    while (temp) {
	parity = !parity;
	temp &= (temp - 1);
    }
    if (parity)
	data |= 0x80;
    return data;
}
#endif

int rawframe_to_socket(int net_socket, unsigned char *netframe, int length) {
    int s;
    int on = 1;

    s = send(net_socket, netframe, length, 0);
    if (s != length) {
	fprintf(stderr, "%s: error sending TCP data; %s\n", __func__, strerror(errno));
	return -1;
    }
    /* disable Nagle algorithm - force PUSH to send immediatly */
    if (setsockopt(net_socket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error disabling Nagle - TCP_NODELAY on: %s\n", strerror(errno));
	return -1;
    }
    return 0;
}

int send_to_clients(uint8_t *frame, int length) {
    int i;

    for (i = 0; i <= max_tcp_i; i++) {
	if (tcp_client[i] > 0)
	    rawframe_to_socket(tcp_client[i], frame, length);
    }
    return 0;
}

void send_version(void) {
    uint8_t buffer[SMALL_BUFFER];

    memcpy(buffer, XNTP_VERSION, sizeof(XNTP_VERSION));
    add_crc(buffer, sizeof(XNTP_VERSION));
    send_to_clients(buffer, sizeof(XNTP_VERSION));
}

void xntcp_internal(unsigned char *frame) {

    switch (frame[0]) {
    case 0xF0:
	if (frame[1] == 0xF0)
	    send_version();
	break;
    case 0xF2:
	break;
    default:
	break;
    }
}

int print_frame(unsigned char *frame, int length, int background) {
    if (!background) {
	for (int i = 0; i < length; i++)
	    printf("0x%02x ", frame[i]);
	printf("\n");
    }
    return 0;
}

int main(int argc, char **argv) {
    int length, rs485_length, n, i, max_fds, nready, opt, local_tcp_port, conn_fd;
    char timestamp[16];
    /* UDP incoming socket , CAN socket, UDP broadcast socket, TCP socket */
    int eci, ret, tcp_socket, ec_index;
    struct sockaddr_in tcp_addr;
    /* vars for determing broadcast address */
    socklen_t tcp_client_length = sizeof(tcp_addr);
    fd_set all_fds, read_fds;
    struct timeval tv;
    struct termios term_attr;

    int background = 1;
    length = 0;
    ec_index = 0;
    unsigned char ec_frame[64];
    char buffer[64];
    char rs485_interface[64];
    unsigned char frame[MAXDG];
    local_tcp_port = XNTCPPORT;

    while ((opt = getopt(argc, argv, "p:i:fh?")) != -1) {
	switch (opt) {
	case 'p':
	    local_tcp_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'i':
	    strncpy(rs485_interface, optarg, sizeof(rs485_interface) - 1);
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

    /* prepare RS485 interface */
    if ((se = open(rs485_interface, O_RDWR | O_TRUNC | O_NONBLOCK | O_NOCTTY)) < 0) {
	fprintf(stderr, "opening serial interface >%s< error: %s\n", rs485_interface, strerror(errno));
	exit(EXIT_FAILURE);
    } else {
	memset(&term_attr, 0, sizeof(term_attr));
	if (tcgetattr(se, &term_attr) < 0) {
	    fprintf(stderr, "can't get terminal settings error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	term_attr.c_cflag = CS8 | CLOCAL | CREAD;
	term_attr.c_iflag = 0;
	term_attr.c_oflag = 0;
	term_attr.c_lflag = 0;

	/* the DIY RS485 Xpressnet adpapter ignores speed setting */
	if (cfsetospeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "serial interface >%s< ospeed error: %s\n", rs485_interface, strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (cfsetispeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "serial interface >%s< ispeed error: %s\n", rs485_interface, strerror(errno));
	    exit(EXIT_FAILURE);
	}

	if (tcsetattr(se, TCSANOW, &term_attr) < 0) {
	    fprintf(stderr, "serial interface >%s< set error: %s\n", rs485_interface, strerror(errno));
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

#if 0
    /* prepare TCP clients array */
    max_tcp_i = -1;		/* index into tcp_client[] array */
    for (i = 0; i < MAX_TCP_CONN; i++)
	tcp_client[i] = -1;	/* -1 indicates available entry */
#endif

    signal(SIGINT, INThandler);

    /* daemonize the process if requested */
    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    FD_ZERO(&all_fds);
    FD_SET(se, &all_fds);
    FD_SET(st, &all_fds);
    max_fds = MAX(se, st);

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

	/* serial interface */
	if (FD_ISSET(se, &read_fds)) {
	    while ((ret = read(se, buffer, sizeof(buffer))) > 0) {
		time_stamp(timestamp);
		printf("%s RS485 received length %d\n", timestamp, ret);
		for (eci = 0; eci < ret; eci++) {
		    if (ec_index == 0) {
			length = (buffer[0] & 0x0F) + 2;
			time_stamp(timestamp);
			printf("%s RS485 frame length %d\n", timestamp, length);
		    }
		    ec_frame[ec_index++] = (unsigned char)buffer[eci];
		}
	    }
	    /* send frame to connected clients */
	    if (ec_index == length) {
		print_frame(ec_frame, ec_index, background);
		for (i = 0; i <= max_tcp_i; i++)
		    rawframe_to_socket(tcp_client[i], ec_frame, ec_index);
		ec_index = 0;
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
		   inet_ntop(AF_INET, &(tcp_addr.sin_addr), buffer, sizeof(buffer)), ntohs(tcp_addr.sin_port), conn_fd, max_fds);
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
		continue;	/* no more readable descriptors */
	}

	/* check for already connected TCP clients */
	for (i = 0; i <= max_tcp_i; i++) {	/* check all clients for data */
	    tcp_socket = tcp_client[i];
	    if (tcp_socket < 0)
		continue;
	    time_stamp(timestamp);
	    if (FD_ISSET(tcp_socket, &read_fds)) {
		if (!background) {
		    time_stamp(timestamp);
		    printf("%s tcp packet received from client #%d  max_tcp_i:%d todo:%d\n", timestamp, i, max_tcp_i, nready);
		    /* printf("%s packet from: %s\n", timestamp, inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer))); */
		}
		n = read(tcp_socket, frame, MAXDG);
		if (!n) {
		    /* connection closed by client */
		    if (!background) {
			time_stamp(timestamp);
			printf("%s client %s closed connection\n", timestamp, inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		    }
		    syslog(LOG_NOTICE, "%s: client %s closed connection\n", __func__, inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		    close(tcp_socket);
		    FD_CLR(tcp_socket, &all_fds);
		    tcp_client[i] = -1;
		} else {
		    if (n == 0) {
			if (!background) {
			    time_stamp(timestamp);
			    fprintf(stderr, "%s received packet %% 2 : length %d - maybe close connection\n", timestamp, n);
			}
			syslog(LOG_ERR, "%s: received packet %% 2 : length %d - maybe close connection\n", __func__, n);
		    } else {
			print_frame(frame, n, background);
			if ((frame[0] & 0xF0) == 0xF0) {
			    xntcp_internal(frame);
			} else {
			    /* we need to check if TCP consist of more than one command */
			    if (n > 1) {
				rs485_length = (frame[1] & 0x0F) + 3;
				if (write(se, frame, rs485_length) != rs485_length) {
				    fprintf(stderr, "%s: error sending RS485 frame: %s\n", __func__, strerror(errno));
				    usec_sleep(rs485_length * 200);
				}
				if (n > rs485_length) {
				    /* TODO - maybe for RS485 Adapter */
				    usec_sleep(rs485_length * 200);
				    if (write(se, &frame[rs485_length], n - rs485_length) != n - rs485_length)
					fprintf(stderr, "%s: error sending RS485 frame: %s\n", __func__, strerror(errno));
				}
			    } else {
				if (write(se, frame, 1) != 1)
				    fprintf(stderr, "%s: error sending RS485 frame: %s\n", __func__, strerror(errno));
			    }
			    /* send packet to all connected clients */
			    for (i = 0; i <= max_tcp_i; i++) {
				if (tcp_socket == tcp_client[i])
				    continue;
				rawframe_to_socket(tcp_client[i], frame, n);
			    }
			}
		    }
		}
		if (--nready <= 0)
		    break;	/* no more readable descriptors */
	    }
	}
    }
    return 0;
}
