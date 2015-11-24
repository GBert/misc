/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ftdi.h>

#define BAUDRATE	(4096)
#define S88_DEF_BITS	16
#define FIFO_SIZE	256
#define UDP_PORT	15731

#define BIT(x)		(1<<x)
#define S88_LOAD	BIT(0) | BIT(4)
#define S88_PS		BIT(1) | BIT(5)
#define S88_CLOCK	BIT(2) | BIT(6)
#define S88_DATA_I	BIT(3)
#define S88_DATA_II	BIT(7)

static struct ftdi_context *ftdic;

/*
	TXD - 0x01 -> S88 reset
	RXD - 0x02 -> S88 PS
	RTS - 0x04 -> S88 clock
	CTS - 0x08 -> S88 data
	DTR - 0x10
	DSR - 0x20
	DCD - 0x40
	RI  - 0x80
 */
/* S88 init    */
/* reset bit 1 */
/* ps    bit 2 */
/* clock bit 3 */
const uint8_t S88_INIT[] = { 0, 0, S88_LOAD, S88_LOAD, S88_LOAD | S88_CLOCK, S88_LOAD | S88_CLOCK, S88_LOAD, S88_LOAD,
			     S88_LOAD | S88_PS, S88_LOAD | S88_PS, S88_LOAD, S88_LOAD, 0, 0 };

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b baud\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -d                  going into background\n");
    fprintf(stderr, "         -r <baudrate>       baudrate - default 4096 (~50us)\n");
    fprintf(stderr, "         -l <databits>       databits(e.g. 6088 -> 16)\n\n");
}

int do_init(int baudrate) {
    ftdic = ftdi_new();

    if (ftdic == 0) {
	fprintf(stderr, "ftdi_new failed\n");
	return -1;
    }

    if (ftdi_usb_open_desc(ftdic, 0x0403, 0x6001, NULL, NULL) < 0) {
    /* if (ftdi_usb_open_desc(ftdic, 0x0403, 0x6015, NULL, NULL) < 0) { */
	fprintf(stderr, "ftdi_usb_open_desc failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    if (ftdi_set_bitmode(ftdic, 0xff, BITMODE_SYNCBB) < 0) {
	fprintf(stderr, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    printf("using baudrate %d\n", baudrate);
    if (ftdi_set_baudrate(ftdic, baudrate) < 0) {
	fprintf(stderr, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    if (ftdi_usb_purge_tx_buffer(ftdic)) {
	fprintf(stderr, "tx buffer flushing failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    if (ftdi_usb_purge_rx_buffer(ftdic)) {
	fprintf(stderr, "rx buffer flushing failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    return 0;
}

int fill_data(uint8_t * b, size_t s, int s88_bits) {
    int i, offset;

    i = 0;
    offset = sizeof(S88_INIT);

    if (s < s88_bits * 4 + offset) {
	fprintf(stderr, "to less space (%d) for %d bits\n", (int)s, s88_bits);
	return -1;
    }
    memcpy(b, S88_INIT, offset);

    while (i < s88_bits * 4) {
	b[i++ + offset] = 0;
	b[i++ + offset] = 0;
	b[i++ + offset] = S88_CLOCK;
	b[i++ + offset] = S88_CLOCK;
    }
    return (i + offset);
}

int send_event(int net_socket, struct sockaddr *net_addr, int bit, int value) {
    int s;
    uint32_t canid, temp;
    uint8_t netframe[13];

    /* TODO: change ID to something standard */
    canid = 0x00220301;

    bzero(netframe, 13);
    temp = htonl(canid);
    memcpy(netframe, &temp, 4);
    /* sensor event 8 bytes */
    netframe[4] = 8;
    /* TODO */
    /* memcpy(&netframe[5] */

    /* send UDP frame */
    s = sendto(net_socket, netframe, 13, 0, net_addr, sizeof(*net_addr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending TCP/UDP data %s\n", __func__, strerror(errno));
	return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    uint8_t w_data[FIFO_SIZE];
    uint8_t r_data[FIFO_SIZE];
    struct timeval tm1, tm2;
    unsigned long elapsed_time;
    unsigned int baudrate;
    int buffersize, opt, length, background, s, sb, destination_port, ret;
    struct sockaddr_in baddr;
    const int on = 1;

    char *udp_dst_address = (char *)malloc(16);
    if (udp_dst_address == NULL) {
	fprintf(stderr, "can't alloc udp_dst_address memory\n");
	exit(1);
    }
    char *bcast_interface = (char *)malloc(16);
    if (bcast_interface == NULL) {
	fprintf(stderr, "can't alloc bcast_interface memory\n");
	exit(1);
    }

    strcpy(udp_dst_address, "255.255.255.255");
    strcpy(bcast_interface, "br-lan");
    destination_port = 0;

    baudrate = BAUDRATE;
    length = S88_DEF_BITS;
    background = 0;

    while ((opt = getopt(argc, argv, "b:dr:l:h?")) != -1) {
	switch (opt) {
	case 'b':
	    if (strlen(optarg) <= 15) {
		/* IP address begins with a number */
		if ((optarg[0] >= '0') || (optarg[0] <= '9')) {
		    strcpy(udp_dst_address, optarg);
		} else {
		    bzero(bcast_interface, 16);
		    strcpy(bcast_interface, optarg);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(1);
	    }
	    break;
	case 'd':
	    background = 1;
	    break;
	case 'r':
	    baudrate = atoi(optarg);
	    break;
	case 'l':
	    length = atoi(optarg);
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
	}
    }

    /* prepare udp sending socket struct */
    bzero(&baddr, sizeof(baddr));
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &baddr.sin_addr);
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "UDP IP address invalid\n");
	} else {
	    fprintf(stderr, "invalid address family\n");
	}
	exit(1);
    }
    if (!background)
	printf("using broadcast address %s\n", udp_dst_address);

    /* prepare UDP sending socket */
    sb = socket(AF_INET, SOCK_DGRAM, 0);
    if (sb < 0) {
	fprintf(stderr, "error creating UDP sending socket: %s\n", strerror(errno));
	exit(1);
    }
    ret = setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    if (ret < 0) {
	fprintf(stderr, "error setup UDP broadcast option: %s\n", strerror(errno));
	exit(1);
    }

    if (background) {
	pid_t pid;

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
	    exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(0);
	}
    }

    if (do_init(baudrate))
	exit(-1);

    bzero(w_data, sizeof(w_data));

    ret = fill_data(w_data, sizeof(w_data), length);

    if (ret < 0) {
	fprintf(stderr, "to many data bits\n");
	exit(1);
    }

#if 1
/* testing: simple bit pattern */
    buffersize = sizeof(w_data);
    for (int i = 0; i < buffersize; i++) {
	w_data[i] = (uint8_t) i & 0xff;
    }
#endif

    for (;;) {
	gettimeofday(&tm1, NULL);

	ret = ftdi_write_data(ftdic, w_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_write_data faild: %s", ftdi_get_error_string(ftdic));
	    exit(1);
	}
	ret = ftdi_read_data(ftdic, r_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_read_data faild: %s", ftdi_get_error_string(ftdic));
	    exit(1);
	}

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
	printf("send %d bytes in %ld usecs\n", FIFO_SIZE, elapsed_time);
    }

    return 0;
}
