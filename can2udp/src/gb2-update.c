/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* the hard analyzing work was done by Karsten - Thx */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <linux/can.h>

#define MAXDG   4096		/* maximum datagram size */
#define MAXUDP  16		/* maximum datagram size */
#define TIMEOUT 5		/* wait seconds for response */

static char *GB2_DEFAULT_FILENAME = "016-gb2.bin";
static char *MS2_DEFAULT_FILENAME = "050-ms2.bin";

#define GB2_BLOCK_SHIFT		9	/* 2^9 = 512 */
#define GB2_BLOCK_SIZE		(1<<GB2_BLOCK_SHIFT)
#define GB2_BOOT_BLOCK_SIZE	(1024/GB2_BLOCK_SIZE)
#define GB2_FILL_SBLOCK		(GB2_BLOCK_SIZE-1)
#define GB2_ID			0x47

#define MS2_BLOCK_SHIFT		10	/* 2^10 = 1024 */
#define MS2_BLOCK_SIZE		(1<<MS2_BLOCK_SHIFT)
#define MS2_BOOT_BLOCK_SIZE	(4096/MS2_BLOCK_SIZE)
#define MS2_FILL_SBLOCK		(MS2_BLOCK_SIZE-1)
#define MS2_ID			0x4d

struct update_config {
    unsigned int block_size;
    unsigned int shift;
    unsigned int boot_blocks;
    unsigned int padding;
    unsigned int id;
    char *filename;
};

struct updatefile {
    char *name;
    char *version_name;
    char *filename;
    int version_type;
    int version_storage;
    int blocksize;
    int bootblock;
    int fill_upto;
    uint8_t fill;
};

#define ACTUAL	0
#define OLD	1
#define	BLOCK	1024

struct updatefile gb2_update_data[] = {
/*    name, version_name,	filename    , version type, version_storage, blocksize, bootblocks, fill upto,  fill */
    {"gb2s",	   "gb2",	"016-gb2.bin",	ACTUAL,		  6,		 512, 		2,   512,	0xff},
    {0},
};
#define GB2_UPDATE_DATA_SIZE ((sizeof(gb2_update_data) / sizeof(struct updatefile)) - 1)

struct updatefile ms2_update_data[] = {
/*    name, version name,	filename    , version type, version_storage, blocksize, bootblocks, fill upto,  fill */
    {"ms2s",	"",		"050-ms2.bin",	ACTUAL,		252, 		1024, 		4,   1024,	0xff},
    {"ms2",	"ms2ver",	"050-ms2.bin",	ACTUAL,		252,		1024, 		0,   	8,	0x00},
    {"gb2",	"gb2ver",	"016-gb2.bin",	ACTUAL,		  6,		   0,		0,	8,	0x00},
    {"gfp",	"gfpver",	"000-gfp.bin",	ACTUAL,		  6,		   0,		0,	8,	0x00},
    {"ldb",	"ldbver",	"flashdb.ms2",	OLD,		  0, 		   0,		0,	8,	0x00},
    {"lang",	"langver",	"lang.ms2",	ACTUAL,		  0, 		   0, 		0,	8,	0x00},
    {"mfx",	"mfxver",	"mfxdefs.ms2",	ACTUAL,		  0, 		   0,		0,	8,	0x00},
    {"mfxb",	"mfxbver",	"mfxdefs.bin",	ACTUAL,		  0, 	 	   0,		0,	8,	0x00},
    {"ms2x",	"ms2xver",	"051-ms2.bin",	ACTUAL,		  0, 	 	   0,		0,	8,	0x00},
    {0},
};

#define MS2_UPDATE_DATA_SIZE ((sizeof(ms2_update_data) / sizeof(struct updatefile)) - 1)

extern uint16_t CRCCCITT(uint8_t * data, size_t length, uint16_t seed);

unsigned char netframe[MAXDG];

static char *CECK_FORMAT_STRG   = "  CHEKCAN    CANID 0x%08X   [%d]";

unsigned char M_GLEISBOX_MAGIC_START_SEQUENCE[]	= { 0x00, 0x36, 0x03, 0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00 };
unsigned char M_CAN_PING[] 			= { 0x00, 0x30, 0x47, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char M_RESET[] 			= { 0x00, 0x00, 0x47, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00 };
unsigned char M_INIT_BOOTLOADER[] 		= { 0x00, 0x36, 0x47, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
unsigned char M_BLOCK[] 			= { 0x00, 0x36, 0x47, 0x11, 0x06, 0x00, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00 };
unsigned char M_DATA[] 				= { 0x00, 0x36, 0x03, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00 };
unsigned char M_CRC[] 				= { 0x00, 0x36, 0x47, 0x11, 0x07, 0x00, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00 };

unsigned char M_MS2_MARK_END[] 			= { 0x00, 0x36, 0x47, 0x11, 0x05, 0x00, 0x00, 0x00, 0x00, 0xf5, 0x00, 0x00, 0x00 };
unsigned char M_MS2_SOFT_RESET[] 		= { 0x00, 0x36, 0x47, 0x11, 0x05, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00 };

unsigned char udpframe[MAXDG];

unsigned char *binfile;
int device_fsize, fsize;
int force = 0, verbose = 0;
uint16_t device_file_version, version = 0;
unsigned int device_id = 0;
int sc, sb;			/* CAN socket, UDP Broadcast Socket */
int can_mode = 0;
unsigned char lastframe[13];
unsigned char checkframe[13];
unsigned char checkframe_nack[13];
unsigned char checkframe_block_id[13];
int dev_bin_blocks;
int last_bin_block;
int finished;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -l <port> -d <port> -b <broacast_addr> -i <can interface> <update file>\n", prg);
    fprintf(stderr, "   Version 0.2\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "         -d <port>           destination UDP port - default 15731\n");
    fprintf(stderr, "         -l <port>           listening UDP port   - default 15730\n");
    fprintf(stderr, "         -b <broacast_addr>  broadcast address - default 255.255.255.255\n");
    fprintf(stderr, "         -i <can int>        switch to can using interface <can int>\n");
    fprintf(stderr, "         -f                  force update even if device has already the same version\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

uint32_t be32(uint8_t *u) {
    return (u[0] << 24) | (u[1] << 16) | (u[2] << 8) | u[3];
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
    int i, dlc;
    char timestamp[16];

    if (!verbose)
	return;

    dlc = netframe[4];
    time_stamp(timestamp);
    printf("%s   ", timestamp);
    printf(format_string, be32(netframe) & CAN_EFF_MASK, netframe[4]);
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
    printf("  ");
    for (i = 5; i < 13; i++) {
	if (isprint(netframe[i]))
	    printf("%c", netframe[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

void format_can_to_netframe(struct can_frame *frame, unsigned char *netframe) {
    uint32_t canid;

    frame->can_id &= CAN_EFF_MASK;
    canid = htonl(frame->can_id);
    memcpy(netframe, &canid, 4);
    netframe[4] = frame->can_dlc;
    memcpy(&netframe[5], &frame->data, frame->can_dlc);
}

int netframe_to_net(int net_socket, unsigned char *netframe, int length) {
    if (send(net_socket, netframe, length, 0) != length) {
	fprintf(stderr, "couldn't send on UDP socket: %s\n", strerror(errno));
	return -1;
    }
    return 0;
}

int netframe_to_can(int can_socket, unsigned char *netframe) {
    struct can_frame frame;
    struct timespec to_wait;

    memset(&frame, 0, sizeof(frame));
    frame.can_id = be32(netframe);
    frame.can_id &= CAN_EFF_MASK;
    frame.can_id |= CAN_EFF_FLAG;
    frame.can_dlc = netframe[4];
    memcpy(&frame.data, &netframe[5], 8);

    /* send CAN frame */
    if (write(can_socket, &frame, sizeof(frame)) != sizeof(frame)) {
	fprintf(stderr, "%s: error writing CAN frame: %s\n", __func__, strerror(errno));
	return -1;
    }
    /* TODO : it seems Gleisbox needs a short break after every CAN message -> 20ms */
    to_wait.tv_sec = 0;
    to_wait.tv_nsec = 20 * 1000000;
    nanosleep(&to_wait, NULL);
    return 0;
}

int send_frame(unsigned char *netframe) {
    int ret;
    if (can_mode)
	ret = netframe_to_can(sc, netframe);
    else
	ret = netframe_to_net(sb, netframe, 13);
    return ret;
}

unsigned char *read_data(struct update_config *device_config) {
    FILE *fp;
    unsigned char *data;

    fp = fopen(device_config->filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, device_config->filename);
	return NULL;
    }

    fseek(fp, 0, SEEK_END);
    fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    /* prepare padding */
    device_fsize = (fsize + device_config->padding) & (0xFFFFFFFFUL - device_config->padding);

    if ((data = malloc(device_fsize)) == NULL) {
	fprintf(stderr, "%s: can't alloc %d bytes for data\n", __func__, fsize);
	fclose(fp);
	return NULL;
    }

    /* padding with 0xff */
    memset(&data[device_fsize - device_config->padding], 0xff, device_config->padding);
    if ((fread((void *)data, 1, fsize, fp)) != (unsigned int)fsize) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, device_config->filename);
	fclose(fp);
	free(data);
	return NULL;
    }

    fclose(fp);
    if (device_config->id == GB2_ID) {
	memcpy(&device_file_version, &data[6], 2);
	printf("[%s] Device File Version %u.%u\n", device_config->filename, data[6], data[7]);
    } else if (device_config->id == MS2_ID) {
	memcpy(&device_file_version, &data[252], 2);
	printf("[%s] Device File Version %u.%u\n", device_config->filename, data[252], data[253]);
    }
    return data;
}

int print_versions(struct update_config *device_config) {
    unsigned int i, read_data_size, version_major, version_minor;
    FILE *fp;
    uint8_t *data;

    if ((data = malloc(BLOCK)) == NULL) {
        fprintf(stderr, "%s: can't alloc %d bytes for data\n", __func__, BLOCK);
        return(EXIT_FAILURE);
    }

    if (device_config->id == GB2_ID) {
    } else if (device_config->id == MS2_ID) {
	for (i = 0; i < MS2_UPDATE_DATA_SIZE; ++i) {
	    fp = fopen(ms2_update_data[i].filename, "rb");
	    if (fp == NULL) {
		fprintf(stderr, "%s: error opening file [%s]\n", __func__, ms2_update_data[i].filename);
		free(data);
		return(EXIT_FAILURE);
	    }
	    fseek(fp, 0, SEEK_END);
	    fsize = ftell(fp);
	    fseek(fp, 0, SEEK_SET);
	    read_data_size = fsize > BLOCK ? BLOCK : fsize;
	    if ((fread((void *)data, 1, read_data_size, fp)) != read_data_size) {
		fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, ms2_update_data[i].filename);
		fclose(fp);
		free(data);
		return(EXIT_FAILURE);
	    }
	    version_major = data[ms2_update_data[i].version_storage];
	    version_minor = data[ms2_update_data[i].version_storage+1];
	    printf("[%s] Device File Version %u.%u\n", ms2_update_data[i].filename, version_major, version_minor);
	    fclose(fp);
	}
    }
    printf("\n");
    free(data);
    return(EXIT_SUCCESS);
}

/*
 .version=14
 .monat=5
 .jahr=2014
 .anzahl=1143
*/

/*
 .vhigh=2
 .vlow=7
 .bytes=84192
*/

int send_next_block_id(int block, unsigned char *netframe) {
    memcpy(netframe, M_BLOCK, 13);
    memcpy(&netframe[5], &device_id, 4);
    netframe[10] = block;
    send_frame(netframe);
    memcpy(checkframe_block_id, checkframe, 10);
    checkframe_block_id[4] = 6;
    checkframe_block_id[9] = 0x44;
    checkframe_block_id[10] = block;
    return 0;
}

int device_setup(char *device, struct update_config *device_config) {
    int ret = -1;
    if (strncmp(device, "gb2", 3) == 0) {
	device_config->block_size = GB2_BLOCK_SIZE;
	device_config->shift = GB2_BLOCK_SHIFT;
	device_config->boot_blocks = GB2_BOOT_BLOCK_SIZE;
	device_config->padding = GB2_FILL_SBLOCK;
	device_config->filename = GB2_DEFAULT_FILENAME;
	device_config->id = GB2_ID;
	ret = 1;
    } else if (strncmp(device, "ms2", 3) == 0) {
	device_config->block_size = MS2_BLOCK_SIZE;
	device_config->shift = MS2_BLOCK_SHIFT;
	device_config->boot_blocks = MS2_BOOT_BLOCK_SIZE;
	device_config->padding = MS2_FILL_SBLOCK;
	device_config->filename = MS2_DEFAULT_FILENAME;
	device_config->id = MS2_ID;
	ret = 2;
    }
    return ret;
}

int send_block(unsigned char *binfile, int length, unsigned char *netframe) {
    int i, part = 0;
    uint16_t crc;

    for (i = 0; i < length; i += 8) {
	memcpy(netframe, M_DATA, 5);
	netframe[3] = part;
	part++;
	memcpy(&netframe[5], &binfile[i], 8);
	send_frame(netframe);
    }
    memcpy(netframe, &M_CRC, 10);
    memcpy(&netframe[5], &device_id, 4);
    crc = htons(CRCCCITT(binfile, length, 0xFFFF));
    memcpy(&netframe[10], &crc, 2);
    send_frame(netframe);
    return 0;
}

void fsm(unsigned char *netframe, struct update_config *device_config) {
    unsigned int canid;
    unsigned char next_frame[13];
    struct timespec to_wait;

    canid = be32(netframe);
    switch (canid & 0xFFFF0000UL) {
    case (0x00310000UL):
	printf("received CAN Ping answer\n");
	/* print_can_frame(" ", netframe, 1); */
	if ((netframe[4] == 8) && (netframe[5] == device_config->id)) {
	    memcpy(&version, &netframe[9], 2);
	    if (netframe[5] == GB2_ID)
		printf("found Gleisbox with ID 0x%08X  Version %u.%u\n", be32(&netframe[5]), netframe[9], netframe[10]);
	    if (netframe[5] == MS2_ID)
		printf("found MS2 with ID 0x%08X  Version %u.%u\n", be32(&netframe[5]), netframe[9], netframe[10]);
	    if ((version == device_file_version) && (!force)) {
		printf("file and device version are the same - use -f to force update\n");
		exit(EXIT_FAILURE);
	    }
	    printf("Start update ...\n");
	    memcpy(next_frame, M_RESET, 13);
	    memcpy(&next_frame[5], &device_id, 4);
	    send_frame(next_frame);
	    /* delay for boot ? */
	    to_wait.tv_sec = 0;
	    to_wait.tv_nsec = 500 * 1000000;
	    nanosleep(&to_wait, NULL);

	    memcpy(next_frame, M_INIT_BOOTLOADER, 13);
	    send_frame(next_frame);
	}
	break;
    case (0x00370000UL):
	/* should always be true */
	if (device_id != 0) {
	    if ((netframe[4] == 8) && (memcmp(&netframe[5], &device_id, 4) == 0) && (netframe[12] == 0x10)) {
		/* prepare test frame for later use */
		memcpy(checkframe, netframe, 10);
		memset(&checkframe[10], 0, 3);
		checkframe[1] = 0x37;
		checkframe[4] = 5;
		checkframe[9] = 0x88;
		memcpy(checkframe_nack, checkframe, 13);
		/* print_can_frame(CECK_FORMAT_STRG, checkframe, 1); */
		/* print_can_frame(CECK_FORMAT_STRG, checkframe_nack, 1); */
		last_bin_block = dev_bin_blocks;
		send_next_block_id(last_bin_block + device_config->boot_blocks, lastframe);
	    } else {
		/* first data block */
		if ((memcmp(&netframe[4], &lastframe[4], 9) == 0) && (last_bin_block == dev_bin_blocks)) {
		    if (last_bin_block == dev_bin_blocks) {
			printf("sending block 0x%02X 0x%04X\n", last_bin_block + device_config->boot_blocks,
			       last_bin_block * device_config->block_size);
			send_block(&binfile[((last_bin_block) * device_config->block_size)],
				   device_fsize - dev_bin_blocks * device_config->block_size, lastframe);
			last_bin_block--;
		    }
		} else {
		    /* there seems to be different NACK types : 0xF1 and 0xF2 */
		    if ((memcmp(netframe, checkframe_nack, 9) == 0) && ((netframe[9] == 0xf1) || (netframe[9] == 0xf2))) {
			print_can_frame(CECK_FORMAT_STRG, netframe, 1);
			fprintf(stderr, "Aiiee got NACK ! Aborting ...\n\n");
			finished = -1;
		    }
		    /* MS2 may use 0x0000 as hash -> begin compare at data length field */
		    if (memcmp(&netframe[4], &checkframe_block_id[4], 7) == 0) {
			printf("sending block 0x%02X 0x%04X\n", last_bin_block + device_config->boot_blocks,
			       last_bin_block * device_config->block_size);
			send_block(&binfile[((last_bin_block) * device_config->block_size)], device_config->block_size,
				   lastframe);
			last_bin_block--;
		    }
		    /* MS2 may use 0x0000 as hash -> begin compare at data length field */
		    if (memcmp(&netframe[4], &checkframe[4], 6) == 0) {
			if (last_bin_block >= 0) {
			    send_next_block_id(last_bin_block + device_config->boot_blocks, lastframe);
			} else {
			    if (device_config->id == MS2_ID) {
				send_frame(M_MS2_MARK_END);
				to_wait.tv_sec = 0;
				to_wait.tv_nsec = 2 * 1000000;
				nanosleep(&to_wait, NULL);
				send_frame(M_MS2_SOFT_RESET);
			    }
			    finished = 1;
			}
		    }
		}
	    }
	}
	break;
    default:
	break;
    }
}

int main(int argc, char **argv) {
    int ret, s, opt;
    struct can_frame frame;
    struct timeval timeout;
    int local_port, destination_port;
    int on, sa;			/* UDP socket */
    struct sockaddr_in saddr, baddr;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    struct update_config device_config;
    char broadcast_address[16];
    socklen_t caddrlen;
    fd_set readfds;
    char *filename;

    /* wait for response */
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    /* socklen_t sin_size = sizeof(clientaddr); */
    caddrlen = sizeof(caddr);

    if (device_setup(basename(argv[0]), &device_config) < 0) {
	fprintf(stderr, "invalid device\n");
	exit(EXIT_FAILURE);
    }

    local_port = 15731;
    destination_port = 15730;
    on = 1;
    strcpy(broadcast_address, "255.255.255.255");

    memset(&saddr, 0, sizeof(saddr));
    memset(&baddr, 0, sizeof(baddr));
    memset(&caddr, 0, sizeof(caddr));
    memset(&frame, 0, sizeof(frame));
    memset(udpframe, 0, sizeof(udpframe));

    /* prepare udp destination struct with defaults */
    baddr.sin_family = AF_INET;
    baddr.sin_port = htons(destination_port);
    s = inet_pton(AF_INET, broadcast_address, &baddr.sin_addr);
    if (s <= 0) {
	if (s == 0) {
	    fprintf(stderr, "UDP IP invalid\n");
	} else {
	    fprintf(stderr, "invalid address family\n");
	}
	exit(EXIT_FAILURE);
    }

    while ((opt = getopt(argc, argv, "d:l:b:i:fvh?")) != -1) {
	switch (opt) {
	case 'l':
	    local_port = strtoul(optarg, (char **)NULL, 10);
	    break;
	case 'd':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    baddr.sin_port = htons(destination_port);
	    break;
	case 'b':
	    s = inet_pton(AF_INET, optarg, &baddr.sin_addr);
	    if (s <= 0) {
		if (s == 0) {
		    fprintf(stderr, "invalid IP address: %s\n", strerror(errno));
		} else {
		    fprintf(stderr, "inet_pton error: %s\n", strerror(errno));
		}
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    can_mode = 1;
	    break;

	case 'f':
	    force = 1;
	    break;

	case 'v':
	    verbose = 1;
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
    if (optind < argc) {
	filename = (char *)malloc(strlen(argv[optind] + 1));
	strncpy(filename, argv[optind], sizeof(*filename) - 1);
	device_config.filename = filename;
    }

    print_versions(&device_config);

    binfile = read_data(&device_config);
    if (binfile == NULL)
	exit(EXIT_FAILURE);
    dev_bin_blocks = (device_fsize - 1) >> device_config.shift;
    /* printf("%s: fsize 0x%04X device_fsize 0x%04X blocks 0x%02X last 0x%04X\n", filename, fsize, device_fsize,
       dev_bin_blocks, device_fsize - dev_bin_blocks * device_config.block_size); */

    if (can_mode) {
	if ((sc = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	    fprintf(stderr, "CAN socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	caddr.can_family = AF_CAN;
	if (ioctl(sc, SIOCGIFINDEX, &ifr) < 0) {
	    fprintf(stderr, "CAN setup error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	caddr.can_ifindex = ifr.ifr_ifindex;

	if (bind(sc, (struct sockaddr *)&caddr, caddrlen) < 0) {
	    fprintf(stderr, "CAN bind error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* start Maerklin 60113 box */
	if (netframe_to_can(sc, M_GLEISBOX_MAGIC_START_SEQUENCE) < 0) {
	    fprintf(stderr, "can't send CAN magic 60113 start sequence: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* send CAN Ping */
	if (netframe_to_can(sc, M_CAN_PING) < 0) {
	    fprintf(stderr, "can't send CAN Ping: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	sa = 0;
	sb = 0;

    } else {
	if ((sa = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	    fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(local_port);

	if (bind(sa, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
	    fprintf(stderr, "UDP bind error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* prepare UDP sending socket */
	if ((sb = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	    fprintf(stderr, "UDP sending socket error %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (setsockopt(sb, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	    fprintf(stderr, "UDP set socket option error: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	sc = 0;
	/* send CAN Ping */
	if (netframe_to_net(sb, M_CAN_PING, 13) < 0) {
	    fprintf(stderr, "can't send CAN Ping: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    while (1) {
	FD_ZERO(&readfds);
	FD_SET(sc, &readfds);
	FD_SET(sa, &readfds);

	ret = select((sc > sa) ? sc + 1 : sa + 1, &readfds, NULL, NULL, &timeout);
	/* reset timer */
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;

	if (ret == 0) {
	    fprintf(stderr, "no response for %d seconds - terminating\n", TIMEOUT);
	    exit(EXIT_FAILURE);
	}

	if (ret < 0)
	    fprintf(stderr, "select error: %s\n", strerror(errno));

	/* received a CAN frame */
	if (FD_ISSET(sc, &readfds)) {

	    if (read(sc, &frame, sizeof(struct can_frame)) < 0) {
		fprintf(stderr, "CAN read error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	    } else if (frame.can_id & CAN_EFF_FLAG) {	/* only EFF frames are valid */
		format_can_to_netframe(&frame, netframe);
		fsm(netframe, &device_config);
		if (finished == 1)
		    break;
		if (finished < 0)
		    exit(EXIT_FAILURE);
	    }
	}
	/* received a UDP packet */
	if (FD_ISSET(sa, &readfds)) {
	    if (read(sa, udpframe, MAXDG) == 13) {
		fsm(udpframe, &device_config);
		if (finished == 1)
		    break;
		if (finished < 0)
		    exit(EXIT_FAILURE);
	    }
	}
    }
    printf("Update successfull !\n\n");
    free(binfile);
    close(sc);
    close(sa);
    close(sb);
    return 0;
}
