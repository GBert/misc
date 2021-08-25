/*
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Gerhard Bertelsmann wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <netdb.h>

#include <linux/can.h>
#include <linux/i2c-dev.h>

#define BIT(x)		(1<<x)
#define MICRODELAY	5000	/* clock frequency [us] */
#define I2C_ADDRESS	0x20	/* default I2C bus number */
#define I2C_BUS		1	/* default I2C bus number */
#define MINDELAY	2	/* min delay in usec */
#define MAXMODULES	4	/* max numbers of PI02 modules */
#define MAXIPLEN	40	/* maximum IP string length */
#define UDPPORT		15730
/* the maximum amount of pin buffer - assuming 32 bit*/
#define PIO_BUFFER	(MAXMODULES)

uint32_t bus_ct0[PIO_BUFFER];
uint32_t bus_ct1[PIO_BUFFER];
uint32_t bus_actual[PIO_BUFFER];
uint32_t bus_state[PIO_BUFFER];

const char *i2c_dev_def_name = "/dev/i2c-";

struct pio_t {
    struct sockaddr_in baddr;
    int socket;
    int background;
    int verbose;
    int invert;
    int offset;
    uint32_t count;
    uint16_t hash;
    uint16_t hw_id;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <PIO2 modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 1.0\n\n");
    fprintf(stderr, "         -a                  I2C address - default 0x%2x\n", I2C_ADDRESS);
    fprintf(stderr, "         -n                  I2C bus number - default %d\n", I2C_BUS);
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -i [0|1]            invert signals - default 1 -> inverting\n");
    fprintf(stderr, "         -e <event id>       using event id - default 0\n");
    fprintf(stderr, "         -m <PIO chips>      number of connected SPI-PIO chips - default 2\n");
    fprintf(stderr, "         -o <offset>         number of PIO2 modules to skip in addressing - default 0\n");
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
    fprintf(stderr, "         -t <time in usec>   microtiming in usec - default %d usec\n", MICRODELAY);
    fprintf(stderr, "         -f                  run in foreground (for debugging)\n");
    fprintf(stderr, "         -v                  be verbose\n\n");
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

int time_stamp(void) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    printf("%02d:%02d:%02d.%06d ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec);
    return 0;
}

void print_net_frame(unsigned char *netframe, uint32_t count) {
    uint32_t canid;
    int i, dlc;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp();
    printf("[0x%08X] ->S88>UDP  CANID 0x%08X   [%u]", count, ntohl(canid), netframe[4]);
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

int create_event(struct pio_t *pio, int bus, int offset, uint32_t changed_bits, uint32_t value) {
    int s;
    uint32_t i, mask, canid, temp32;
    uint16_t temp16;
    uint8_t netframe[13];

    /* allow usefull M*rklin hashes only */
    canid = 0x00230300 | (pio->hash & 0x0000ffff);

    temp32 = htonl(canid);
    memcpy(netframe, &temp32, 4);
    /* sensor event 8 bytes */
    netframe[4] = 8;
    /* we don't set the time value in the S88 event as of today */
    netframe[11] = 0;
    netframe[12] = 0;

    mask = BIT(0);
    for (i = 0; i < 32; i++) {
	if (changed_bits & mask) {
	    temp16 = htons(pio->hw_id);
	    memcpy(&netframe[5], &temp16, 2);
	    /* TODO */
	    temp16 = htons(bus * 256 + offset + i + 1);
	    memcpy(&netframe[7], &temp16, 2);
	    if (value & mask) {
		netframe[9] = 0;
		netframe[10] = 1;
	    } else {
		netframe[9] = 1;
		netframe[10] = 0;
	    }
	    s = sendto(pio->socket, netframe, 13, 0, (struct sockaddr *)&pio->baddr, sizeof(pio->baddr));
	    if (s != 13) {
		fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
		return -1;
	    }
	    if (!pio->background)
		print_net_frame(netframe, pio->count);
	}
	mask <<= 1;
    }
    return 0;
}

int analyze_data(struct pio_t *pio, int pio_bits) {
    int ret, i;
    uint32_t c;

    /* using Peter Daneggers 2 bit debouncing buffer code */
    for (i = 0; i <= ((pio_bits - 1) >> 5); i++) {
	c = bus_state[i] ^ ~bus_actual[i];
	bus_ct0[i] = ~(bus_ct0[i] & c);
	bus_ct1[i] = bus_ct0[i] ^ (bus_ct1[i] & c);
	/* 2 bit roll over */
	c &= bus_ct0[i] & bus_ct1[i];
	bus_state[i] ^= c;
	ret = create_event(pio, 0, i * 32, c, bus_actual[i]);
	if (ret)
	    return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int utime, i, opt, ret;
    int pio_chips = 2;
    int i2c_bus = I2C_BUS;
    int i2c_address = I2C_ADDRESS;
    char *i2c_dev_name;
    struct sockaddr_in destaddr, *bsa;
    struct ifaddrs *ifap, *ifa;
    struct pio_t pio_data;
    char *udp_dst_address, *bcast_interface;
    int file, data_buf_start;
    uint32_t i2c_data_temp, i2c_data;
    uint8_t buffer = 0x12;

    const int on = 1;

    int destination_port = UDPPORT;
    utime = MICRODELAY;

    memset(&pio_data, 0, sizeof(pio_data));
    pio_data.background = 1;
    pio_data.invert = 1;

    /* prepare debouncing buffer */
    memset(bus_actual, 0, sizeof(bus_actual));
    memset(bus_state, 0xff, sizeof(bus_state));
    memset(bus_ct0, 0xff, sizeof(bus_ct0));
    memset(bus_ct1, 0xff, sizeof(bus_ct1));

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

    /* prepare udp sending socket struct */
    memset(&destaddr, 0, sizeof(destaddr));
    destaddr.sin_family = AF_INET;
    destaddr.sin_port = htons(destination_port);

    while ((opt = getopt(argc, argv, "a:n:b:e:i:p:m:o:t:fvh?")) != -1) {
	switch (opt) {
	case 'a':
	    i2c_address = atoi(optarg) & 0xff;
	    break;
	case 'n':
	    /* restict /dev/i2c-0 - /dev/i2c-7 */
	    i2c_bus = atoi(optarg) & 0x07;
	    break;
	case 'p':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr.sin_port = htons(destination_port);
	    break;
	case 'b':
	    if (strnlen(optarg, MAXIPLEN) <= MAXIPLEN - 1) {
		/* broadcat IP begins with a number */
		if ((optarg[0] >= '0') && (optarg[0] <= '9')) {
		    strncpy(udp_dst_address, optarg, MAXIPLEN - 1);
		} else {
		    strncpy(bcast_interface, optarg, MAXIPLEN - 1);
		}
	    } else {
		fprintf(stderr, "UDP broadcast address or interface error: %s\n", optarg);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'e':
	    pio_data.hash = atoi(optarg) & 0xffff;
	    break;
	case 'i':
	    pio_data.invert = atoi(optarg) & 1;
	    break;
	case 'm':
	    pio_chips = atoi(optarg);
	    if (pio_chips < 1 || pio_chips > MAXMODULES) {
		usage(basename(argv[0]));
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'o':
	    pio_data.offset = atoi(optarg);
	    if (pio_data.offset >= MAXMODULES) {
		usage(basename(argv[0]));
		exit(EXIT_FAILURE);
	    }
	    break;
	case 't':
	    utime = atoi(optarg);
	    if (utime < MINDELAY) {
		fprintf(stderr, "microtiming value to low: %d\n", utime);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'v':
	    pio_data.verbose = 1;
	    break;
	case 'f':
	    pio_data.background = 0;
	    break;
	case 'h':
	case '?':
	    usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	default:
	    usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    i2c_dev_name  = strdup(i2c_dev_def_name);
    if (!i2c_dev_name) {
	fprintf(stderr, "can't alloc memory for I2C device name: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    i2c_dev_name[strlen(i2c_dev_def_name)] = i2c_bus + 0x30;

    if (!pio_data.background && pio_data.verbose)
	printf("i2c device: %s\n", i2c_dev_name);

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

    ret = inet_pton(AF_INET, udp_dst_address, &destaddr.sin_addr);
    if (ret <= 0) {
	if (ret == 0)
	    fprintf(stderr, "UDP IP invalid\n");
	else
	    fprintf(stderr, "invalid address family\n");
	exit(EXIT_FAILURE);
    }
    if (!pio_data.background && pio_data.verbose)
	printf("using broadcast address %s\n", udp_dst_address);
    /* open udp socket */
    if ((pio_data.socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(pio_data.socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set broadcast option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    pio_data.baddr = destaddr;

    if (pio_data.background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    if ((file = open(i2c_dev_name, O_RDWR)) < 0) {
	fprintf(stderr, "failed to open I2C bus: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (!pio_data.background && pio_data.verbose)
	printf("PIO ICs: %d , timing: %.1f ms\n", pio_chips, (float)utime / 1000);

    /* loop forever */
    while (1) {
	pio_data.count++;
	data_buf_start = 0;

	i2c_data_temp = 0;
	i2c_data = 0;

	for (i = 0; i < pio_chips; i++) {
	    /* printf("i2c address 0x%02x\n", i2c_address + i); */
	    if (ioctl(file, I2C_SLAVE, i2c_address + i) < 0) {
		fprintf(stderr, "failed to acquire bus access for address 0x%02x: %s\n", i2c_address, strerror(errno));
		exit(EXIT_FAILURE);
	    }

	    buffer = 0x12;
	    if ((write(file, &buffer, 1)) != 1) {
		fprintf(stderr, "failed to to set reg address 0x%02x: %s\n", 0x12, strerror(errno));
		exit(EXIT_FAILURE);
	    }

	    if (read(file, &i2c_data_temp, 2) != 2) {
		fprintf(stderr, "failed to read from I2C bus: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	    }
	    if (i & 1) {
		i2c_data |= i2c_data_temp << 16;
		if (pio_data.invert)
		    i2c_data ^= 0xffffffff;
		bus_actual[data_buf_start] = i2c_data;

		/* printf("%08d %02d data : 0x%08x\n", pio_data.count, i, bus_actual[data_buf_start]); */
		data_buf_start++;
	    } else {
		i2c_data = i2c_data_temp & 0xffff;
	    }
	}
	/* now check data */
	ret = analyze_data(&pio_data, pio_chips * 16);
	if (ret < 0) {
	    fprintf(stderr, "problem sending event data - terminating\n");
	    exit(EXIT_FAILURE);
	}
	usec_sleep(utime);
    }
    return 0;
}
