/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* Thanks to Stefan Krauss and the SocketCAN team
 */

#include "scan2lan.h"

static char *CAN_FORMAT_STRG		= "      CAN->  CANID 0x%06X R [%d]";
static char *UDP_FORMAT_STRG		= "->CAN>UDP    CANID 0x%06X   [%d]";
static char *TCP_FORMAT_STRG		= "->TCP>CAN    CANID 0x%06X   [%d]";
static char *TCP_FORMATS_STRG		= "->TCP>CAN*   CANID 0x%06X   [%d]";
static char *CAN_TCP_FORMAT_STRG	= "->CAN>TCP    CANID 0x%06X   [%d]";
static char *NET_UDP_FORMAT_STRG	= "      UDP->  CANID 0x%06X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE[] = { 0x00, 0x36, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00 };

char config_dir[MAXLINE];
char config_file[MAXLINE];
char **page_name;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -c <config_dir> -u <udp_port> -t <tcp_port> -d <udp_dest_port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.95\n\n");
    fprintf(stderr, "         -c <config_dir>     set the config directory\n");
    fprintf(stderr, "         -u <port>           listening UDP port for the server - default 15731\n");
    fprintf(stderr, "         -t <port>           listening TCP port for the server - default 15731\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15730\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        CAN interface - default /dev/ttyUSB0\n");
    fprintf(stderr, "         -f                  running in foreground\n\n");
    fprintf(stderr, "         -v                  verbose output (in foreground)\n\n");
}

int send_magic_start_60113_frame(int can_socket, int verbose) {
    if (frame_to_can(can_socket, M_GLEISBOX_MAGIC_START_SEQUENCE) < 0) {
	fprintf(stderr, "can't send CAN magic 60113 start sequence\n");
	return -1;
    } else {
	if (verbose) {
	    printf("                CAN magic 60113 start written\n");
	    print_can_frame(CAN_FORMAT_STRG, M_GLEISBOX_MAGIC_START_SEQUENCE);
	}
    }
    return 0;
}

int check_data(int tcp_socket, unsigned char *netframe) {
    uint32_t canid;
    char config_name[9];
    char gbs_name[MAXLINE];
    int ret;

    gbs_name[0] = 0;
    ret = 0;

    memcpy(&canid, netframe, 4);
    canid = ntohl(canid);
    switch (canid & 0x00FF0000UL) {
    case (0x00400000UL):	/* config data */
	ret = 1;
	strncpy(config_name, (char *)&netframe[5], 8);
	config_name[8] = '\0';
	printf("%s ID 0x%08x %s\n", __func__, canid, (char *)&netframe[5]);
	netframe[1] |= 1;
	net_to_net(tcp_socket, NULL, netframe, 13);
	if (strcmp("loks", config_name) == 0) {
	    send_tcp_config_data("lokomotive.cs2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strcmp("mags", config_name) == 0) {
	    send_tcp_config_data("magnetartikel.cs2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strncmp("gbs-", config_name, 4) == 0) {
	    int page_number;
	    page_number = atoi(&config_name[5]);
	    strcat(gbs_name, "gleisbilder/");
	    if (page_name) {
		strcat(gbs_name, page_name[page_number]);
		strcat(gbs_name, ".cs2");
		send_tcp_config_data(gbs_name, config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    }
	    break;
	} else if (strcmp("gbs", config_name) == 0) {
	    send_tcp_config_data("gleisbild.cs2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strcmp("fs", config_name) == 0) {
	    send_tcp_config_data("fahrstrassen.cs2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	}
	/* TODO : these files depends on different internal states */
	else if (strcmp("lokstat", config_name) == 0) {
	    fprintf(stderr, "%s: lokstat (lokomotive.sr2) not implemented yet\n", __func__);
	    send_tcp_config_data("lokomotive.sr2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strcmp("magstat", config_name) == 0) {
	    fprintf(stderr, "%s: magstat (magnetartikel.sr2) not implemented yet\n\n", __func__);
	    send_tcp_config_data("magnetartikel.sr2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strcmp("gbsstat", config_name) == 0) {
	    fprintf(stderr, "%s: gbsstat (gbsstat.sr2) not implemented yet\n\n", __func__);
	    send_tcp_config_data("gbsstat.sr2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	} else if (strcmp("fsstat", config_name) == 0) {
	    fprintf(stderr, "%s: fsstat (fahrstrassen.sr2) not implemented yet\n\n", __func__);
	    send_tcp_config_data("fahrstrassen.sr2", config_dir, canid, tcp_socket, CRC | COMPRESSED);
	    break;
	}
    }
    return ret;
}

int main(int argc, char **argv) {
    int n, i, max_fds, opt, max_tcp_i, nready, conn_fd, tcp_client[MAX_TCP_CONN];
    int background, verbose, ec_index, on;
    char timestamp[16];
    struct termios term_attr;
    int sa, sc, sb, st, tcp_socket;	/* UDP incoming socket , CAN socket, UDP broadcast socket, TCP socket */
    struct sockaddr_in saddr, baddr, tcp_addr;
    char if_name[MAXSTRING];
    char udp_dst_address[16];
    socklen_t tcp_client_length = sizeof(tcp_addr);
    fd_set all_fds, read_fds;
    uint32_t canid;
    int eci, s, ret;
    int local_udp_port, local_tcp_port, destination_port;
    char buffer[64];

    verbose = 0;
    background = 1;
    on = 1;
    strcpy(udp_dst_address, "255.255.255.255");
    ec_index = 0;
    page_name = calloc(64, sizeof(char *));

    local_udp_port = 15731;
    local_tcp_port = 15731;
    destination_port = 15730;
    strcpy(if_name, "/dev/ttyUSB0");

    config_file[0] = '\0';

    while ((opt = getopt(argc, argv, "c:u:t:d:b:i:vhf?")) != -1) {
	switch (opt) {
	case 'c':
	    if (strlen(optarg) < MAXLINE) {
		strncpy(config_dir, optarg, sizeof(config_dir) - 1);
	    } else {
		fprintf(stderr, "config file dir to long\n");
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'u':
	    local_udp_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 't':
	    local_tcp_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'b':
	    if (strlen(optarg) <= 15) {
		strncpy(udp_dst_address, optarg, sizeof(udp_dst_address) - 1);
	    } else {
		fprintf(stderr, "UDP broadcast address error: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    if (strlen(optarg) < MAXSTRING) {
		strncpy(if_name, optarg, sizeof(if_name) - 1);
	    } else {
		fprintf(stderr, "interface name to long\n");
		exit(EXIT_FAILURE);
	    }
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

    /* read track file */
    if (config_dir[0] == 0) {
	strcat(config_file, ".");
    }
    strcat(config_file, config_dir);
    if (config_file[strlen(config_dir)] != '/') {
	strcat(config_file, "/");
    }
    strcat(config_file, "gleisbild.cs2");

    page_name = read_track_file(config_file, page_name);

    /* prepare udp sending socket struct */
    memset(&baddr, 0, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &baddr.sin_addr);
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "UDP IP address invalid\n");
	} else {
	    fprintf(stderr, "invalid address family\n");
	}
	exit(EXIT_FAILURE);
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "error creating UDP sending socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error setup UDP broadcast option: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare reading UDP socket */
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(local_udp_port);
    if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "creating UDP reading socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (bind(sa, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
	fprintf(stderr, "binding UDP reading socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* prepare TCP socket */
    if ((st = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
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

    /* prepare simple CAN interface */
    if ((sc = open(if_name, O_RDWR | O_TRUNC | O_NONBLOCK | O_NOCTTY)) < 0) {
	fprintf(stderr, "opening CAN interface error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    } else {
	memset(&term_attr, 0, sizeof(term_attr));
	if (tcgetattr(sc, &term_attr) < 0) {
	    fprintf(stderr, "can't get terminal settings error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	term_attr.c_cflag = CS8 | CRTSCTS | CLOCAL | CREAD;
	term_attr.c_iflag = 0;
	term_attr.c_oflag = 0;
	term_attr.c_lflag = NOFLSH;
	if (cfsetospeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "CAN interface ospeed error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (cfsetispeed(&term_attr, TERM_SPEED) < 0) {
	    fprintf(stderr, "CAN interface ispeed error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (tcsetattr(sc, TCSANOW, &term_attr) < 0) {
	    fprintf(stderr, "CAN interface set error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    /* start Maerklin 60113 box */
    send_magic_start_60113_frame(sc, verbose);

    /* daemonize the process if requested */
    if (background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    FD_ZERO(&all_fds);
    FD_SET(sc, &all_fds);
    FD_SET(sa, &all_fds);
    FD_SET(st, &all_fds);
    max_fds = MAX(MAX(sc, sa), st);

    while (1) {
	read_fds = all_fds;
	if ((nready = select(max_fds + 1, &read_fds, NULL, NULL, NULL)) < 0) {
	    fprintf(stderr, "select error: %s\n", strerror(errno));
	}

	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
	    /* copy the CAN frame to UDP broadcast and all connected TCP clients */
	    while ((ret = read(sc, buffer, sizeof(buffer))) > 0) {
		for (eci = 0; eci < ret; eci++) {
		    ec_frame[ec_index++] = (unsigned char)buffer[eci];
		    if (ec_index == 13) {
			/* we got a complete CAN frame */
			ec_index = 0;
			if (net_to_net(sb, (struct sockaddr *)&baddr, ec_frame, 13)) {
			    fprintf(stderr, "sending UDP data error:%s \n", strerror(errno));
			} else if (verbose && !background) {
			    print_can_frame(UDP_FORMAT_STRG, ec_frame);
			}
			/* send CAN frame to all connected TCP clients */
			/* TODO: need all clients the packets ? */
			for (i = 0; i <= max_tcp_i; i++) {
			    if ((tcp_socket = tcp_client[i]) < 0)
				continue;
			    net_to_net(tcp_socket, (struct sockaddr *)&tcp_addr, ec_frame, 13);
			    if (verbose && !background)
				print_can_frame(CAN_TCP_FORMAT_STRG, ec_frame);
			}
		    }
		}
	    }
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &read_fds)) {
	    if (read(sa, netframe, MAXDG) == 13) {
		/* send packet on CAN */
		ret = frame_to_can(sc, netframe);
		if (verbose && !background)
		    print_can_frame(NET_UDP_FORMAT_STRG, netframe);
		memcpy(&canid, netframe, 4);
		canid = ntohl(canid);
		/* answer to encapsulated CAN ping from LAN to LAN */
		if (((canid & 0x00FF0000UL) == 0x00310000UL)
		    && (netframe[11] == 0xEE) && (netframe[12] == 0xEE)) {
		    if (verbose && !background)
			printf("                received CAN ping\n");
		    netframe[0] = 0x00;
		    netframe[1] = 0x30;
		    netframe[2] = 0x00;
		    netframe[3] = 0x00;
		    netframe[4] = 0x00;
		    if (net_to_net(sb, (struct sockaddr *)&baddr, netframe, 13)) {
			fprintf(stderr, "sending UDP data (CAN Ping) error:%s \n", strerror(errno));
		    } else if (verbose && !background) {
			print_can_frame(NET_UDP_FORMAT_STRG, netframe);
			printf("                replied CAN ping\n");
		    }
		}
	    }
	}
	/* received a TCP packet */
	if (FD_ISSET(st, &read_fds)) {
	    conn_fd = accept(st, (struct sockaddr *)&tcp_addr, &tcp_client_length);
	    if (verbose && !background) {
		printf("new client: %s, port %d conn fd: %d max fds: %d\n", inet_ntop(AF_INET, &(tcp_addr.sin_addr),
		       buffer, sizeof(buffer)), ntohs(tcp_addr.sin_port), conn_fd, max_fds);
	    }
	    for (i = 0; i < MAX_TCP_CONN; i++) {
		if (tcp_client[i] < 0) {
		    tcp_client[i] = conn_fd;	/* save new TCP client descriptor */
		    break;
		}
	    }
	    if (i == MAX_TCP_CONN)
		fprintf(stderr, "too many TCP clients\n");

	    FD_SET(conn_fd, &all_fds);	/* add new descriptor to set */
	    max_fds = MAX(conn_fd, max_fds);	/* for select */
	    max_tcp_i = MAX(i, max_tcp_i);	/* max index in tcp_client[] array */
	    if (--nready <= 0)
		continue;	/* no more readable descriptors */
	}
	/* check for already connected TCP clients */
	for (i = 0; i <= max_tcp_i; i++) {	/* check all clients for data */
	    if ((tcp_socket = tcp_client[i]) < 0)
		continue;
	    /* printf("%s tcp packet received from client #%d  max_tcp_i:%d todo:%d\n", time_stamp(timestamp), i, max_tcp_i,nready); */
	    if (FD_ISSET(tcp_socket, &read_fds)) {
		if (verbose && !background) {
		    time_stamp(timestamp);
		    printf("%s packet from: %s\n", timestamp, inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		}
		if ((n = read(tcp_socket, netframe, MAXDG)) == 0) {
		    /* connection closed by client */
		    if (verbose && !background) {
			time_stamp(timestamp);
			printf("%s client %s closed connection\n", timestamp,
			       inet_ntop(AF_INET, &tcp_addr.sin_addr, buffer, sizeof(buffer)));
		    }
		    close(tcp_socket);
		    FD_CLR(tcp_socket, &all_fds);
		    tcp_client[i] = -1;
		} else {
		    /* check the whole TCP packet, if there are more than one CAN frame included */
		    /* TCP packets with size modulo 13 !=0 are ignored though */
		    if (n % 13) {
			time_stamp(timestamp);
			fprintf(stderr, "%s received packet %% 13 : length %d\n", timestamp, n);
		    } else {
			for (i = 0; i < n; i += 13) {
			    /* check if we need to forward the message to CAN */
			    if (!check_data(tcp_socket, &netframe[i])) {
				ret = frame_to_can(sc, &netframe[i]);
				if ((ret == 0) && (verbose && !background)) {
				    if (i > 0)
					print_can_frame(TCP_FORMATS_STRG, &netframe[i]);
				    else
					print_can_frame(TCP_FORMAT_STRG, &netframe[i]);
				}
			    }
			}
		    }
		}
		if (--nready <= 0)
		    break;	/* no more readable descriptors */
	    }
	}
    }
    close(sc);
    close(sa);
    close(sb);
    close(st);
    return 0;
}
