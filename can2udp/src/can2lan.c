/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* Thanks to Stefan Krauss and the SocketCAN team
 */

#include "can2lan.h"

char *CAN_FORMAT_STRG       ="      CAN->  CANID 0x%06X R [%d]";
char *TO_CAN_FORMAT_STRG    ="      CAN    CANID 0x%06X   [%d]";
char *UDP_FORMAT_STRG       ="->CAN>UDP    CANID 0x%06X   [%d]";
char *TCP_FORMAT_STRG       ="->TCP>CAN    CANID 0x%06X   [%d]";
char *TCP_FORMATS_STRG      ="->TCP>CAN*   CANID 0x%06X   [%d]";
char *CAN_TCP_FORMAT_STRG   ="->CAN>TCP    CANID 0x%06X   [%d]";
char *NET_UDP_FORMAT_STRG   ="      UDP->  CANID 0x%06X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE [] = {0x00,0x36,0x03,0x01,0x05,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00};

char config_dir[MAXLINE];
char config_file[MAXLINE];
char line[MAXLINE];
char **page_name;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -c <config_dir> -u <udp_port> -t <tcp_port> -d <udp_dest_port> -i <can interface>\n", prg);
    fprintf(stderr, "   Version 0.92\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -s <config_dir>     set the config directory\n");
    fprintf(stderr, "         -u <port>           listening UDP port for the server - default 15731\n");
    fprintf(stderr, "         -t <port>           listening TCP port for the server - default 15731\n");
    fprintf(stderr, "         -d <port>           destination UDP port for the server - default 15730\n");
    fprintf(stderr, "         -b <broadcast_addr> broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        can interface - default can0\n");
    fprintf(stderr, "         -f                  running in foreground\n");
    fprintf(stderr, "\n");
}

int send_magic_start_60113_frame(int can_socket, int verbose) {
    int ret;
    ret = frame_to_can(can_socket, M_GLEISBOX_MAGIC_START_SEQUENCE);
    if (ret < 0 ) {
	fprintf(stderr, "can't send CAN magic 60113 start sequence\n");
	return -1;
    } else {
	if (verbose)
	    printf("                CAN magic 60113 start written\n");
	    print_can_frame(CAN_FORMAT_STRG, M_GLEISBOX_MAGIC_START_SEQUENCE);
    }
    return 0;
}

int check_data(int tcp_socket, unsigned char *netframe) {
    uint32_t canid;
    char config_name[8];
    int page_number;
    char gbs_name[MAXLINE];
    gbs_name[0]='\0';

    memcpy(&canid, netframe, 4);
    canid = ntohl(canid);
    /* printf("%s ID 0x%08x\n", __func__, canid); */
    switch (canid & 0x00FF0000UL) {
        case (0x00400000UL) : /* config data */
             strncpy(config_name,(char *) &netframe[5], 8);
             printf("%s ID 0x%08x %s\n", __func__, canid, (char *) &netframe[5]);
             netframe[1] |= 1;
             net_to_net(tcp_socket, NULL, netframe, 13);
             if (strcmp("loks", config_name)==0) {
                 send_tcp_config_data("lokomotive.cs2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("mags", config_name)==0) {
                 send_tcp_config_data("magnetartikel.cs2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strncmp("gbs-", config_name,4)==0) {
                 page_number=atoi(&config_name[5]);
                 strcat(gbs_name,"gleisbilder/");
                 strcat(gbs_name,page_name[page_number]);
                 strcat(gbs_name,".cs2");
                 send_tcp_config_data(gbs_name, config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("gbs", config_name)==0) {
                 send_tcp_config_data("gleisbild.cs2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("fs", config_name)==0) {
                 send_tcp_config_data("fahrstrassen.cs2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             /* TODO : these files depends on different internal states */
             else if (strcmp("lokstat", config_name)==0) {
                 fprintf(stderr, "%s: lokstat (lokomotive.sr2) not implemented yet\n", __func__); 
                 send_tcp_config_data("lokomotive.sr2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("magstat", config_name)==0) {
                 fprintf(stderr, "%s: magstat (magnetartikel.sr2) not implemented yet\n\n", __func__); 
                 send_tcp_config_data("magnetartikel.sr2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("gbsstat", config_name)==0) {
                 fprintf(stderr, "%s: gbsstat (gbsstat.sr2) not implemented yet\n\n", __func__); 
                 send_tcp_config_data("gbsstat.sr2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
             else if (strcmp("fsstat", config_name)==0) {
                 fprintf(stderr, "%s: fsstat (fahrstrassen.sr2) not implemented yet\n\n", __func__); 
                 send_tcp_config_data("fahrstrassen.sr2", config_dir, canid, tcp_socket, CRC|COMPRESSED);
                 break;
             }
    }
    return 0;
}

int main(int argc, char **argv) {
    pid_t pid;
    /* struct cs2_config cs2_config; */
    extern int optind, opterr, optopt;
    int n, i, max_fds, opt, max_tcp_i, nready, conn_fd, tcp_client[MAX_TCP_CONN];;
    struct can_frame frame;
    char timestamp[16];

    int sa, sc, sb, st, tcp_socket;		/* UDP socket , CAN socket, UDP Broadcast Socket, TCP Socket */
    struct sockaddr_in saddr, baddr, tcp_addr;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    socklen_t caddrlen = sizeof(caddr);
    socklen_t tcp_client_length = sizeof(tcp_addr);

    fd_set all_fds, read_fds;

    uint32_t	canid;
    int s, nbytes, ret;

    int local_udp_port = 15731;
    int local_tcp_port = 15731;
    int destination_port = 15730;
    int verbose = 1;
    int background = 1;
    const int on = 1;
    char udp_dst_address[] = "255.255.255.255";
    char buffer[64];
    page_name = calloc(64, sizeof(char *));

    strcpy(ifr.ifr_name, "can0");
    config_file[0] = '\0';

    while ((opt = getopt(argc, argv, "s:u:t:d:b:i:vhf?")) != -1) {
	switch (opt) {
	case 's':
            if (strlen(optarg) <MAXLINE) {
	        strcpy(config_dir, optarg);
            } else {
                fprintf(stderr, "config file dir to long\n");
                exit(1);
            }
	    break;
	case 'u':
	    local_udp_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 't':
	    local_tcp_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **) NULL, 10);
	    break;
	case 'b':
	    if ( strlen(optarg) <=15 ) {
		strcpy(udp_dst_address, optarg);
	    } else {
		fprintf(stderr, "UDP broadcast address error: %s\n", optarg);
                exit(1);
	    }
	    break;
	case 'i':
	    strcpy(ifr.ifr_name, optarg);
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
	    exit(0);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(1);
	    break;
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

    page_name=read_track_file(config_file, page_name);

    /* prepare udp sending socket struct */
    bzero(&baddr, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &baddr.sin_addr);
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "UDP IP invalid\n");
	} else {
	    fprintf(stderr, "invalid address family\n");
	}
    	exit(1);
    }

    /* prepare UDP sending socket */
    if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "error creating UDP sending socket: %s\n", strerror(errno));
	exit(1);
    }
    if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "error setup UDP broadcast option: %s\n", strerror(errno));
	exit(1);
    }

    /* prepare reading UDP socket */
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(local_udp_port);
    if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "creating UDP reading socket error: %s\n", strerror(errno));
	exit(1);
    }
    if  ( bind(sa, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
	fprintf(stderr, "binding UDP reading socket error: %s\n", strerror(errno));
	exit(1);
    }

    /* prepare TCP socket */
    if ((st = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	fprintf(stderr, "creating TCP socket error: %s\n", strerror(errno));
	exit(1);
    }
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_addr.sin_port = htons(local_tcp_port);
    if ( bind(st, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
	fprintf(stderr, "binding TCP socket error: %s\n", strerror(errno));
	exit(1);
    }
    if ( listen(st, MAXPENDING) < 0) {
	fprintf(stderr, "starting TCP listener error: %s\n", strerror(errno));
	exit(1);
    }
    /* prepare TCP clients array */
    max_tcp_i = -1;			/* index into tcp_client[] array */
    for (i = 0; i < MAX_TCP_CONN; i++)
	tcp_client[i] = -1;		/* -1 indicates available entry */

    /* prepare CAN socket */
    bzero(&caddr, sizeof(caddr));
    if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "creating CAN socket error: %s\n", strerror(errno));
	exit(1);
    }
    caddr.can_family = AF_CAN;
    if ( ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN error: %s\n", strerror(errno));
	exit(1);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if ( bind(sc, (struct sockaddr *) &caddr, caddrlen) < 0) {
	fprintf(stderr, "binding CAN socket error: %s\n", strerror(errno));
	exit(1);
    }

    /* start Maerklin 60113 box */
    send_magic_start_60113_frame(sc, verbose);

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

    FD_ZERO(&all_fds);
    FD_SET(sc, &all_fds);
    FD_SET(sa, &all_fds);
    FD_SET(st, &all_fds);
    max_fds = MAX(MAX(sc, sa),st);

    while (1) {
	read_fds = all_fds;
	nready = select(max_fds + 1 , &read_fds, NULL, NULL, NULL);
	if (nready<0)
	    fprintf(stderr, "select error: %s\n", strerror(errno));

	/* received a CAN frame */
	if (FD_ISSET(sc, &read_fds)) {
            if ((nbytes = read(sc, &frame, sizeof(struct can_frame))) < 0) {
		fprintf(stderr, "reading CAN frame: %s\n", strerror(errno));
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		/* send UDP frame */
		frame_to_net(sb, (struct sockaddr *) &baddr, (struct can_frame *) &frame);
		if (verbose && !background)
	            print_can_frame(UDP_FORMAT_STRG, netframe);
		/* send CAN frame to all connected TCP clients */
		/* TODO: need all clients the packets ? */ 
		for (i = 0; i <= max_tcp_i; i++) {	/* check all clients for data */
		    if ( (tcp_socket = tcp_client[i]) < 0)
			continue;
		    frame_to_net(tcp_socket, (struct sockaddr *) &tcp_addr, (struct can_frame *) &frame);
		    if (verbose && !background)
			print_can_frame(CAN_TCP_FORMAT_STRG, netframe);
		}
	    /* printf("%s tcp packet received from client #%d  max_tcp_i:%d todo:%d\n", time_stamp(timestamp), i, max_tcp_i,nready); */
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
    		canid=ntohl(canid);
		/* answer to encapsulated CAN ping from LAN to LAN */
		if (((canid & 0x00FF0000UL) == 0x00310000UL) 
		      && (netframe[11] = 0xEE) && (netframe[12] = 0xEE)) {
		    if (verbose & !background)
                        printf("                received CAN ping\n");
		    netframe[0] = 0x00;
		    netframe[1] = 0x30;
		    netframe[2] = 0x00;
		    netframe[3] = 0x00;
		    netframe[4] = 0x00;
		    if (net_to_net(sb,(struct sockaddr *) &baddr, netframe, 13)) {
			fprintf(stderr, "sending UDP data (CAN Ping) error:%s \n", strerror(errno));
		    } else if (verbose & !background) {
			print_can_frame(NET_UDP_FORMAT_STRG, netframe);
			printf("                replied CAN ping\n");
		    }
		}
	    }
	}
	/* received a TCP packet */
        if (FD_ISSET(st, &read_fds)) {
	    conn_fd = accept(st, (struct sockaddr *) &tcp_addr, &tcp_client_length);
	    if (verbose && !background) {
		printf("new client: %s, port %d conn fd: %d max fds: %d\n", inet_ntop(AF_INET,
		    &(tcp_addr.sin_addr), buffer, sizeof(buffer)), ntohs(tcp_addr.sin_port), conn_fd, max_fds);
	    }
	    for (i = 0; i < MAX_TCP_CONN; i++) {
		if (tcp_client[i] < 0) {
		    tcp_client[i] = conn_fd;		/* save new TCP client descriptor */
		    break;
		}
	    }
	    if (i == MAX_TCP_CONN)
		fprintf(stderr, "too many TCP clients\n");

	    FD_SET(conn_fd, &all_fds);			/* add new descriptor to set */
	    max_fds = MAX(conn_fd,max_fds);		/* for select */
	    max_tcp_i = MAX(i, max_tcp_i);		/* max index in tcp_client[] array */
	    if (--nready <= 0)
		continue;				/* no more readable descriptors */
	}
	/* check for already connected TCP clients */
	for (i = 0; i <= max_tcp_i; i++) {		/* check all clients for data */
	    if ( (tcp_socket = tcp_client[i]) < 0)
		continue;
	    /* printf("%s tcp packet received from client #%d  max_tcp_i:%d todo:%d\n", time_stamp(timestamp), i, max_tcp_i,nready); */
	    if (FD_ISSET(tcp_socket, &read_fds)) {
		if (verbose && !background) {
                    time_stamp(timestamp);
		    printf("%s packet from: %s\n", timestamp,  inet_ntop(AF_INET,
			&tcp_addr.sin_addr, buffer, sizeof(buffer)));
		}
		if ( (n = read(tcp_socket, netframe, MAXDG)) == 0) {
		    /* connection closed by client */
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
			for (i = 0; i < n; i +=13 ) {
			    ret = frame_to_can(sc, &netframe[i]);
                            check_data(tcp_socket, &netframe[i]);
		            if ((ret == 0) && (verbose && !background)) {
                                if (i > 0)
			             print_can_frame(TCP_FORMATS_STRG, &netframe[i]);
                                else
                                     print_can_frame(TCP_FORMAT_STRG, &netframe[i]);
                            }
                        }
		    }
		}
		if (--nready <= 0)
		    break;                  /* no more readable descriptors */
	    }
	}
    }
    close(sc);
    close(sa);
    close(sb);
    close(st);
    return 0;
}
