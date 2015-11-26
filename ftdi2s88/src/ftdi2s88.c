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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <ftdi.h>

#define BAUDRATE	(4096)
#define S88_DEF_BITS	60
#define FIFO_SIZE	256
#define UDP_PORT	15731

#define BIT(x)		(1<<x)
#define S88_LOAD	BIT(0) | BIT(4)
#define S88_PS		BIT(1) | BIT(5)
#define S88_CLOCK	BIT(2) | BIT(6)
#define S88_DATA_I	BIT(3)
#define S88_DATA_II	BIT(7)
#define BITBANG_MASK	S88_LOAD | S88_PS | S88_CLOCK

#define DEFAULT_TASK_T	100
/* S88 interval 20ms (20000 us)- minus default switching time*/
#define S88_INTERVAL	20 * 1000 - DEFAULT_TASK_T

/*
	TXD - 0x01 -> S88 reset
	RXD - 0x02 -> S88 PS
	RTS - 0x04 -> S88 clock
	CTS - 0x08 -> S88 data I
	DTR - 0x10 -> S88 reset
	DSR - 0x20 -> S88 PS
	DCD - 0x40 -> S88 clock
	RI  - 0x80 -> S88 data II
 */

const uint8_t S88_INIT[] = { 0, 0, 0, 0, S88_LOAD, S88_LOAD, S88_LOAD | S88_CLOCK, S88_LOAD | S88_CLOCK, S88_LOAD, S88_LOAD,
			     S88_LOAD | S88_PS, S88_LOAD | S88_PS, S88_LOAD, S88_LOAD, 0, 0 };
const uint8_t test_data [] = 	{0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x08,0x08,0x08,0x08, 0x08,0x08,0x08,0x08,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x80,0x80,0x80,0x80, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x80,0x80,0x80,0x80, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88,
				 0x88,0x88,0x88,0x88, 0x08,0x08,0x08,0x08, 0x00,0x00,0x00,0x00, 0x88,0x88,0x88,0x88};

struct ftdi2s88_t {
    struct ftdi_context *ftdic;
    struct sockaddr_in baddr;
    int sb;
    int baudrate;
    int background;
};

#define PIN_MEM		2

uint32_t bus0_new[PIN_MEM];
uint32_t bus0_old[PIN_MEM];
uint32_t bus1_new[PIN_MEM];
uint32_t bus1_old[PIN_MEM];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b baud\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -d                  going into background\n");
    fprintf(stderr, "         -r <baudrate>       baudrate - default 4096 (~50us)\n");
    fprintf(stderr, "         -l <databits>       databits(e.g. 6088 -> 16 / max&def 60)\n\n");
}

/* prepare the FTDI USB device */
int do_init(struct ftdi2s88_t *fs88) {
    fs88->ftdic = ftdi_new();

    if (fs88->ftdic == 0) {
	fprintf(stderr, "ftdi_new failed\n");
	return -1;
    }

    if (ftdi_usb_open_desc(fs88->ftdic, 0x0403, 0x6001, NULL, NULL) < 0) {
    /* if (ftdi_usb_open_desc(ftdic, 0x0403, 0x6015, NULL, NULL) < 0) { */
	fprintf(stderr, "ftdi_usb_open_desc failed: %s\n", ftdi_get_error_string(fs88->ftdic));
	return -1;
    }

    if (ftdi_set_bitmode(fs88->ftdic, BITBANG_MASK, BITMODE_SYNCBB) < 0) {
	fprintf(stderr, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(fs88->ftdic));
	return -1;
    }

    if (ftdi_set_baudrate(fs88->ftdic, fs88->baudrate) < 0) {
	fprintf(stderr, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(fs88->ftdic));
	return -1;
    }

    if (ftdi_usb_purge_tx_buffer(fs88->ftdic)) {
	fprintf(stderr, "tx buffer flushing failed: %s\n", ftdi_get_error_string(fs88->ftdic));
	return -1;
    }

    if (ftdi_usb_purge_rx_buffer(fs88->ftdic)) {
	fprintf(stderr, "rx buffer flushing failed: %s\n", ftdi_get_error_string(fs88->ftdic));
	return -1;
    }

    return 0;
}

/* prepare bitbanging data */
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
	b[i++ + offset] = S88_CLOCK;
	b[i++ + offset] = S88_CLOCK;
	b[i++ + offset] = 0;
	b[i++ + offset] = 0;
    }
    return (i + offset);
}

void random_fill(uint8_t * b, int s) {
    int i;
    for (i = 0; i < s; i++) {
	b[0] = 1;
    }
}

int send_event(struct ftdi2s88_t *fs88, int bit, int value) {
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
    s = sendto(fs88->sb, netframe, 13, 0, (struct sockaddr *)&fs88->baddr, sizeof(fs88->baddr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
	return -1;
    }
    printf("send UDP packet: bit %d value %d\n", bit, value);
    return 0;
}

int analyze_data(struct ftdi2s88_t *fs88, uint8_t * b, int s88_bits) {
    int i, k;
    uint32_t mask;

    k = 0;
    memcpy(bus0_old, bus0_new, sizeof(PIN_MEM) * 4);
    memcpy(bus1_old, bus1_new, sizeof(PIN_MEM) * 4);

//    printf("b[8] : 0x%02x\n", b[8]);
    /* first bit is different */
    if (b[8] & S88_DATA_I) {
	bus0_new[0] = 1;
    }
    if (b[8] & S88_DATA_II)
	bus1_new[0] = 1;

//    printf("bus0_new[0]: 0x%08X bus1_new[0]: 0x%08X\n", bus0_new[0], bus1_new[0]);

    for (i = 1; i < s88_bits; i++) {
	if ((i & 0x1f) == 0)
	    k++;
	bus0_new[k] <<= 1;
	bus1_new[k] <<= 1;
//       printf("bus0_new[0]: 0x%08X bus1_new[0]: 0x%08X\n", bus0_new[0], bus1_new[0]);
//        printf("b[%d] : 0x%02x\n", 14 + 4 * i, b[14 + 4* i]);
	if (b[14 + i * 4] && S88_DATA_I)
	    bus0_new[k] |= 1;
	if (b[14 + i * 4] && S88_DATA_II)
	    bus1_new[k] |= 1;
    }

    printf("bus0_new[0]: 0x%08X bus1_new[0]: 0x%08X\n", bus0_new[0], bus1_new[0]);
    printf("bus0_old[0]: 0x%08X bus1_old[0]: 0x%08X\n", bus0_old[0], bus1_old[0]);
    printf("bus0_new[1]: 0x%08X bus1_new[1]: 0x%08X\n", bus0_new[1], bus1_new[1]);
    printf("bus0_old[1]: 0x%08X bus1_old[1]: 0x%08X\n", bus0_old[1], bus1_old[1]);

    k = 0;
    for (i = 0; i < s88_bits; i++) {
	mask >>= 1;
	if ((i & 0x1f) == 0) {
	    k++;
	    mask = 1;
	}
	if ((bus0_new[k] ^ bus0_old[k]) & mask) {
	    if (bus0_new[k] & mask) {
		if (send_event(fs88, i, 1))
		    return -1;
		if (!fs88->background)
		    printf("S88 bus 0 event bit %d state on\n", i);
	    } else {
		if (send_event(fs88, i, 0))
		    return -1;
		if (!fs88->background)
		    printf("S88 bus 0 event bit %d state off\n", i);
	    }
	}
	if ((bus1_new[k] ^ bus1_old[k]) & mask) {
	    if (bus1_new[k] & mask) {
		if (send_event(fs88, i, 1))
		    return -1;
		if (!fs88->background)
		    printf("S88 bus 1 event bit %d state on\n", i);
	    } else {
		if (send_event(fs88, i, 0))
		    return -1;
		if (!fs88->background)
		    printf("S88 bus 1 event bit %d state off\n", i);
	    }
	}
    }

    return 0;
}

int main(int argc, char **argv) {
    uint8_t w_data[FIFO_SIZE];
    uint8_t r_data[FIFO_SIZE];
    uint8_t t_data[FIFO_SIZE];
    struct timeval tm1, tm2;
    unsigned long elapsed_time;
    int ti, buffersize, opt, length, s, destination_port, ret;
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *bsa;
    const int on = 1;
    struct ftdi2s88_t fs88;

    bzero(bus0_new, sizeof(bus0_new));
    bzero(bus0_old, sizeof(bus0_old));
    bzero(bus1_new, sizeof(bus1_new));
    bzero(bus1_old, sizeof(bus1_old));

    char *udp_dst_address = (char *)malloc(16);
    if (!udp_dst_address) {
	fprintf(stderr, "can't alloc udp_dst_address memory\n");
	exit(1);
    }
    char *bcast_interface = (char *)malloc(16);
    if (!bcast_interface) {
	fprintf(stderr, "can't alloc bcast_interface memory\n");
	exit(1);
    }

    strcpy(udp_dst_address, "255.255.255.255");
    strcpy(bcast_interface, "br-lan");
    destination_port = 15731;

    /* setting defaults */
    fs88.baudrate = BAUDRATE;
    length = S88_DEF_BITS;
    fs88.background = 0;

    while ((opt = getopt(argc, argv, "b:dr:l:h?")) != -1) {
	switch (opt) {
	case 'b':
	    if (strlen(optarg) <= 15) {
		/* IP address begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
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
	    fs88.background = 1;
	    break;
	case 'r':
	    fs88.baudrate = atoi(optarg);
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

    /* prepare udp sending socket struct */
    bzero(&fs88.baddr, sizeof(fs88.baddr));
    fs88.baddr.sin_family = AF_INET;
    fs88.baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &fs88.baddr.sin_addr);
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "UDP IP address invalid\n");
	} else {
	    fprintf(stderr, "invalid address family\n");
	}
	exit(1);
    }
    if (!fs88.background) {
	printf("using broadcast address %s\n", udp_dst_address);
    }
    /* prepare UDP sending socket */
    fs88.sb = socket(AF_INET, SOCK_DGRAM, 0);
    if (fs88.sb < 0) {
	fprintf(stderr, "error creating UDP sending socket: %s\n", strerror(errno));
	exit(1);
    }
    ret = setsockopt(fs88.sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
    if (ret < 0) {
	fprintf(stderr, "error setup UDP broadcast option: %s\n", strerror(errno));
	exit(1);
    }

    if (do_init(&fs88))
	exit(-1);

    bzero(w_data, sizeof(w_data));
    ret = fill_data(w_data, sizeof(w_data), length);
    if (ret < 0) {
	fprintf(stderr, "to many data bits\n");
	exit(1);
    }

    if (fs88.background) {
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

    buffersize = sizeof(w_data);
#if 1
/* testing: simple bit pattern */
    memcpy(t_data, test_data, sizeof(test_data));
#endif

    for ( ti = 0 ; ti < 3 ; ti++ ) {
	gettimeofday(&tm1, NULL);

	ret = ftdi_write_data(fs88.ftdic, w_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_write_data faild: %s", ftdi_get_error_string(fs88.ftdic));
	    exit(1);
	}
	ret = ftdi_read_data(fs88.ftdic, r_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_read_data faild: %s", ftdi_get_error_string(fs88.ftdic));
	    exit(1);
	}
#if 1
/* testing */
	memcpy(r_data, t_data, buffersize);
#endif
	if (analyze_data(&fs88, r_data, length))
	    exit(1);

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
	if ((S88_INTERVAL - elapsed_time) > DEFAULT_TASK_T)
	    usleep(S88_INTERVAL - elapsed_time);

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
	if (!fs88.background)
	    printf("send %d bytes in %ld usecs\n", FIFO_SIZE, elapsed_time);
    }
    return 0;
}
