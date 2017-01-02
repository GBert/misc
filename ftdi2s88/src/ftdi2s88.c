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
#include <time.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <ftdi.h>

#define BAUDRATE	(65536)
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

uint8_t test_data[256];

struct ftdi2s88_t {
    struct ftdi_context *ftdic;
    struct sockaddr_in baddr;
    int sb;
    int baudrate;
    int background;
    int inverting;
    uint16_t hash;
    uint16_t hw_id;
};

#define PIN_MEM		2

uint32_t bus0_actual[PIN_MEM];
uint32_t bus1_actual[PIN_MEM];

uint32_t bus0_ct0[PIN_MEM];
uint32_t bus0_ct1[PIN_MEM];
uint32_t bus1_ct0[PIN_MEM];
uint32_t bus1_ct1[PIN_MEM];

uint32_t bus0_state[PIN_MEM];
uint32_t bus1_state[PIN_MEM];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b baud\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -H <hash>           M*rklin hash\n");
    fprintf(stderr, "         -I <ID>             hardware id <default 0x5338>\n");
    fprintf(stderr, "         -i                  inverting signals\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -d                  going into background\n");
    fprintf(stderr, "         -r <baudrate>       baudrate - default 4096 (~50us)\n");
    fprintf(stderr, "         -l <databits>       databits(e.g. 6088 -> 16 / max&def 60)\n\n");
}

void time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
}

void print_net_frame(unsigned char *netframe) {
    uint32_t canid;
    int i, dlc;
    char timestamp[16];

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s   ", timestamp);
    printf("->S88>UDP    CANID 0x%08X   [%d]", ntohl(canid), netframe[4]);
    for (i = 5; i < 5 + dlc; i++) {
	printf(" %02x", netframe[i]);
    }
    if (dlc < 8) {
	printf("(%02x", netframe[i]);
	for (i = 6 + dlc; i < 13; i++) {
	    printf(" %02x", netframe[i]);
	}
	printf(")");
    } else {
	printf(" ");
    }
    printf("\n");
}

void manipulate_test_data(uint8_t * b, int bit, int value) {
    if (bit > 0 && bit <= 60) {
	memset(&b[16 + 4 * bit], (uint8_t) value, 4);
    }
}

/* prepare the FTDI USB device */
int do_init(struct ftdi2s88_t *fs88) {
    fs88->ftdic = ftdi_new();

    if (!fs88->ftdic) {
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
int fill_data(struct ftdi2s88_t *fs88, uint8_t * b, size_t s, int s88_bits) {
    int i, offset;
    uint8_t invert;

    offset = sizeof(S88_INIT);

    if (s < s88_bits * 4 + offset) {
	fprintf(stderr, "to less space (%d) for %d bits\n", (int)s, s88_bits);
	return -1;
    }

    if (fs88->inverting)
	invert = 0xff;
    else
	invert = 0;

    for (i = 0; i < s88_bits * 4; i++)
	b[i] = S88_INIT[i] ^ invert;

    while (i < s88_bits * 4) {
	b[i++ + offset] = (S88_CLOCK) ^ invert;
	b[i++ + offset] = (S88_CLOCK) ^ invert;
	b[i++ + offset] = invert;
	b[i++ + offset] = invert;
    }
    return (i + offset);
}

void random_fill(uint8_t * b, int s) {
    int i;
    for (i = 0; i < s; i++) {
	b[0] = 1;
    }
}

int create_event(struct ftdi2s88_t *fs88, int bus, int offset, uint32_t changed_bits, uint32_t value) {
    int s;
    uint32_t i, mask, canid, temp32;
    uint16_t temp16;
    uint8_t netframe[13];

    /* allow only usefull M*rklin hashes */
    canid = 0x00220300 | (fs88->hash & 0x0000ff7f);

    bzero(netframe, 13);
    temp32 = htonl(canid);
    memcpy(netframe, &temp32, 4);
    /* sensor event 8 bytes */
    netframe[4] = 8;

    mask = BIT(31);
    for (i = 0; i < 32; i++) {
	if (changed_bits & mask) {
	    temp16 = htons(fs88->hw_id);
	    memcpy(&netframe[5], &temp16, 2);
	    /* TODO */
	    temp16 = htons(bus * 256 + offset + i);
	    memcpy(&netframe[7], &temp16, 2);
	    if (value & mask) {
		netframe[9] = 0;
		netframe[10] = 1;
		if (!fs88->background)
		    printf("send UDP packet: bit %d 1\n", i + offset);
	    } else {
		netframe[9] = 1;
		netframe[10] = 0;
		if (!fs88->background)
		    printf("send UDP packet: bit %d 0\n", i + offset);
	    }
#if 1
	    s = sendto(fs88->sb, netframe, 13, 0, (struct sockaddr *)&fs88->baddr, sizeof(fs88->baddr));
	    if (s != 13) {
		fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
		return -1;
	    }
#endif
	    if (!fs88->background)
		print_net_frame(netframe);
	}
	mask >>= 1;
    }
    return 0;
}

int analyze_data(struct ftdi2s88_t *fs88, uint8_t * b, int s88_bits) {
    int ret, i, k;
    uint32_t c;

    k = 0;
    bzero(bus0_actual, sizeof(bus0_actual));
    bzero(bus1_actual, sizeof(bus1_actual));

    if (b[8] & S88_DATA_I) {
	bus0_actual[0] = 1;
    }
    if (b[8] & S88_DATA_II)
	bus1_actual[0] = 1;

    for (i = 1; i < s88_bits; i++) {
	if ((i & 0x1f) == 0)
	    k++;
	bus0_actual[k] <<= 1;
	bus1_actual[k] <<= 1;
	if (b[14 + i * 4] & S88_DATA_I)
	    bus0_actual[k] |= 1;
	if (b[14 + i * 4] & S88_DATA_II)
	    bus1_actual[k] |= 1;
    }
    /* align left */
    if (i & 0x1f) {
	bus0_actual[k] <<= (32 - (s88_bits & 0x1f));
	bus1_actual[k] <<= (32 - (s88_bits & 0x1f));
    }

    if (!fs88->background)
	printf("bus0_actual[0]: 0x%08X bus1_actual[0]: 0x%08X\n", bus0_actual[0], bus1_actual[0]);

    /* debouncing - tricky part */
    for (i = 0; i <= 0; i++) {
	c = bus0_state[i] ^ ~bus0_actual[i];
	bus0_ct0[i] = ~(bus0_ct0[i] & c);
	bus0_ct1[i] = bus0_ct0[i] ^ (bus0_ct1[i] & c);
	/* 2 bit roll over */
	c &= bus0_ct0[i] & bus0_ct1[i];
	bus0_state[i] ^= c;
	ret = create_event(fs88, 0, i * 32, c, bus0_actual[i]);
	if (ret)
	    return -1;

	c = bus1_state[i] ^ ~bus1_actual[i];
	bus1_ct0[i] = ~(bus1_ct0[i] & c);
	bus1_ct1[i] = bus1_ct0[i] ^ (bus1_ct1[i] & c);
	/* 2 bit roll over */
	c &= bus1_ct0[i] & bus1_ct1[i];
	bus1_state[i] ^= c;
	ret = create_event(fs88, 0, 256 + i * 32, c, bus1_actual[i]);
	if (ret)
	    return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    uint8_t w_data[FIFO_SIZE];
    uint8_t r_data[FIFO_SIZE];
    uint8_t t_data[FIFO_SIZE];
    struct timeval tm1, tm2;
    unsigned long elapsed_time;
    int buffersize, opt, length, s, destination_port, ret;
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *bsa;
    const int on = 1;
    struct ftdi2s88_t fs88;

    bzero(bus0_actual, sizeof(bus0_actual));
    bzero(bus1_actual, sizeof(bus1_actual));

    bzero(bus0_state, sizeof(bus0_state));
    bzero(bus1_state, sizeof(bus1_state));

    memset(bus0_ct0, 0xff, sizeof(bus0_ct0));
    memset(bus0_ct1, 0xff, sizeof(bus0_ct1));
    memset(bus1_ct0, 0xff, sizeof(bus1_ct0));
    memset(bus1_ct1, 0xff, sizeof(bus1_ct1));

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
    fs88.inverting = 0;
    fs88.baudrate = BAUDRATE;
    length = S88_DEF_BITS;
    fs88.background = 0;
    fs88.hw_id = 0x5338;
    fs88.hash = 0x5338;

    while ((opt = getopt(argc, argv, "H:I:b:dr:l:h?")) != -1) {
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
	case 'H':
	    fs88.hash = atoi(optarg);
	    break;
	case 'I':
	    fs88.hw_id = atoi(optarg);
	    break;
	case 'i':
	    fs88.inverting = 1;
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
		if (!strncmp(ifa->ifa_name, bcast_interface, strlen(bcast_interface)))
		    udp_dst_address = inet_ntoa(bsa->sin_addr);
	    }
	}
    }

    /* prepare udp socket struct */
    bzero(&fs88.baddr, sizeof(fs88.baddr));
    fs88.baddr.sin_family = AF_INET;
    fs88.baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, udp_dst_address, &fs88.baddr.sin_addr);
    if (s <= 0) {
	if (!s) {
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
    ret = fill_data(&fs88, w_data, sizeof(w_data), length);
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
	/* If we got a valid PID, then we can exit the parent process. */
	if (pid > 0) {
	    printf("Going into background ...\n");
	    exit(0);
	}
    }

    buffersize = sizeof(w_data);
    bzero(test_data, sizeof(test_data));
    memset(&t_data[16 + 17 * 4], 0x88, 16);
#if 0
/* testing: simple bit pattern */
    memcpy(t_data, test_data, sizeof(test_data));
#endif

//    for (ti = 0; ti < 20; ti++) {
    for (;;) {
	gettimeofday(&tm1, NULL);

	ret = ftdi_write_data(fs88.ftdic, w_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_write_data faild: %s", ftdi_get_error_string(fs88.ftdic));
	    break;
	}
	ret = ftdi_read_data(fs88.ftdic, r_data, buffersize);
	if (ret < 0) {
	    fprintf(stderr, "ftdi_read_data faild: %s", ftdi_get_error_string(fs88.ftdic));
	    break;
	}
#if 0
/* testing */
	if (ti == 6) {
	    manipulate_test_data(t_data, 5, 0x08);
	    manipulate_test_data(t_data, 9, 0x88);
	}
	if (ti == 7) {
	    manipulate_test_data(t_data, 8, 0x88);
	    manipulate_test_data(t_data, 9, 0x80);
	}
	if (ti == 11)
	    manipulate_test_data(t_data, 8, 0x00);
	memcpy(r_data, t_data, sizeof(r_data));
#endif
	if (analyze_data(&fs88, r_data, length))
	    break;

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
	if ((S88_INTERVAL - elapsed_time) > DEFAULT_TASK_T)
	    usleep(S88_INTERVAL - elapsed_time);

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
#if 0
	if (!fs88.background)
	    printf("send %d bytes in %ld usecs\n", FIFO_SIZE, elapsed_time);
#endif

    }
    /* free(udp_dst_address);
    free(bcast_interface); */
    exit(1);
}
