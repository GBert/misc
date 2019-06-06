/*
 * Copyright 2018 Joerg Dorchain
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Joerg Dorchain wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

/* TODO
 * - Implement S88 Events with dlc 4 and 5
 * - Implement S88 Polling (Command 0x10)
 * - check for current implementation scheme limit of 32 busses
 *   per device id
 * - command line configurable bus lengths
 * - use can hardware filters to save cpu cycles fo
 *   uninteresting frames
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "mcan.h"

/* Global stuff */
#define VERS_HIGH 0
#define VERS_LOW 1

#define _str(a) #a
#define str(a) _str(a)
#define VERSION str(VERS_HIGH) "." str(VERS_LOW)

#define BIT(nr) (1UL << (nr))

#define UID 0x53383842		/* S88'42' */
#define SYSID 0x0042		/* default, from above */
#define DEVTYPE 0x0040		/* we try to simulate a LinkS88 */
#define POLLTIME 100		/* ms */
#define MICRODELAY 15		/* us */

#define DEF_INT "can0"

#define PIDFILE "/var/run/s88can.pid"

#define MAXMODULES 31

CanDevice cd = {
    .versHigh = VERS_HIGH,
    .versLow = VERS_LOW,
    .name = "S88Can",
    .artNum = str(SYSID),
    .boardNum = 1,
    .hash = 0,
    .uid = UID,
    .type = DEVTYPE,
};

int canfd;

int config_num = 2;
long pollt = POLLTIME * 1000;	/* us */
long utime = MICRODELAY;
uint16_t sysid = SYSID;

int foreground = 0;
int verbose = 0;
int invert = 0;

#define PIO_BUFFER ((MAXMODULES + 1) / 2)
struct s88_bus {
    int blen;
    uint32_t bus_ct0[PIO_BUFFER];
    uint32_t bus_ct1[PIO_BUFFER];
    uint32_t bus_actual[PIO_BUFFER];
    uint32_t bus_state[PIO_BUFFER];
    void (*poll) (struct s88_bus *);
} *s88 = NULL;

/*
 * Banana PI S88 Connections 
 *        GPIO Usage
 *  J12 Pin3    NC      IO-7            PH5
 *  J12 Pin4    RXD     UART7_RX        PI21
 *  J12 Pin5    NC      IO-8            PH3
 *  J12 Pin6    TXD     UART7_TX        PI20
 */
#define BPI_PH5         (7*32+5)
#define BPI_PI21        (8*32+21)
#define BPI_PH3         (7*32+3)
#define BPI_PI20        (8*32+20)

#define DATA_PIN        BPI_PI20
#define CLOCK_PIN       BPI_PH3
#define LOAD_PIN        BPI_PI21
#define RESET_PIN       BPI_PH5
#include "allwinner.h"

void usage(char *p) {
    fprintf(stderr, "\nUsage %s [-v] [-f] [-b <int>] [-i] [-e <id>] [-t <time>] [-l <len,len,...>]\n", p);
    fprintf(stderr, "  Version: " VERSION "\n");
    fprintf(stderr, "    -v        verbose\n");
    fprintf(stderr, "    -f        run in foreground\n");
    fprintf(stderr, "    -b <int>  connect to can interface; default " DEF_INT "\n");
    fprintf(stderr, "    -i        invert signals - default no\n");
    fprintf(stderr, "    -e <id>   set id; default " str(SYSID) "\n");
    fprintf(stderr, "    -t <time> polltime in ms; default " str(POLLTIME) "\n");
    fprintf(stderr, "    -l <len,len,...> Number of Modules per S88-Bus - default 1\n");
}

uint32_t bptol(uint8_t * p) {
    return *((uint32_t *) p);
}
uint16_t bptos(uint8_t * p) {
    return *((uint16_t *) p);
}

void handlecan() {
    MCANMSG can_frame_in;
    int config_index;
    char configstring[22];	/* attention size below */

    can_frame_in = getCanFrame(canfd);

    /* ping */
    if ((can_frame_in.cmd == PING) && (can_frame_in.resp_bit == 0)) {
	if (verbose)
	    fprintf(stderr, "ping reply\n");
	sendPingFrame(canfd, cd, true);
    }

    /* config */
    if ((can_frame_in.cmd == CONFIG) && (can_frame_in.resp_bit == 0) && (ntohl(bptol(can_frame_in.data)) == cd.uid)) {
	config_index = can_frame_in.data[4];
	if (verbose)
	    fprintf(stderr, "config reply index %d\n", config_index);
	if (config_index == 0)
	    sendDeviceInfo(canfd, cd, config_num);
	else if (config_index == 1)
	    sendConfigInfoSlider(canfd, cd, 1, 1, 65534, sysid, "Geraetekenner:_1_65534");
	else if (config_index == 2)
	    sendConfigInfoSlider(canfd, cd, 2, 9, 999, pollt / 1000, "Pollzeit:_9_999_ms");
	else if (config_index > config_num) {
	    fprintf(stderr, "Ignoring query undefined config point %d\n", config_index);
	} else {
	    snprintf(configstring, sizeof(configstring), "Bus %3d Laenge:_0_" str(MAXMODULES), config_index - 2);
	    sendConfigInfoSlider(canfd, cd, config_index, 0, MAXMODULES, s88[config_index - 3].blen, configstring);
	}
    }
    /* status */
    if ((can_frame_in.cmd == SYS_CMD) && (can_frame_in.data[4] == SYS_STAT) &&
	(ntohl(bptol(can_frame_in.data)) == cd.uid) && (can_frame_in.resp_bit == 0)) {
	config_index = can_frame_in.data[5];
	if (verbose)
	    fprintf(stderr, "status reply index %d\n", config_index);
	if ((config_index == 0) || (config_index > config_num)) {
	    fprintf(stderr, "Ignoring setting undefined config point %d\n", config_index);
	    statusResponse(canfd, cd, config_index, false);
	} else {
	    if (config_index == 1)
		sysid = ntohs(bptos(can_frame_in.data + 6));
	    else if (config_index == 2)
		pollt = can_frame_in.data[7] * 1000;
	    else
		s88[config_index - 3].blen = can_frame_in.data[7];
	    statusResponse(canfd, cd, config_index, true);
	}
    }

    /* id */
    if ((can_frame_in.cmd == SYS_CMD) && (can_frame_in.data[4] == SYS_ID) &&
	(ntohl(bptol(can_frame_in.data)) == cd.uid) && (can_frame_in.resp_bit == 1)) {
	sysid = ntohs(bptos(can_frame_in.data + 5));
	if (verbose)
	    fprintf(stderr, "Assigned new sysid 0x%04x\n", sysid);
    }
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

void bpi_poll(struct s88_bus *s88) {
    int i, j;
    uint32_t mask, s88_bit;
    uint8_t newvalue;

    s88_bit = 0;
    gpio_aw_set(LOAD_PIN, HIGH ^ invert);
    usec_sleep(utime);
    gpio_aw_set(CLOCK_PIN, HIGH ^ invert);
    usec_sleep(utime);
    gpio_aw_set(CLOCK_PIN, LOW ^ invert);
    usec_sleep(utime);
    gpio_aw_set(RESET_PIN, HIGH ^ invert);
    usec_sleep(utime);
    gpio_aw_set(RESET_PIN, LOW ^ invert);
    usec_sleep(utime);
    gpio_aw_set(LOAD_PIN, LOW ^ invert);

    for (i = 0; i < s88->blen; i++) {
	if ((s88_bit & 0x1f) == 0)
	    mask = 0x80000000;
	for (j = 0; j < 16; j++) {
	    gpio_aw_get(DATA_PIN, &newvalue);
	    if (newvalue ^= invert)
		s88->bus_actual[i >> 1] |= mask;
	    else
		s88->bus_actual[i >> 1] &= ~mask;
	    usec_sleep(utime);
	    gpio_aw_set(CLOCK_PIN, HIGH ^ invert);
	    usec_sleep(utime);
	    gpio_aw_set(CLOCK_PIN, LOW ^ invert);
	    s88_bit++;
	    mask >>= 1;
	}
    }
}

void create_event(int bus, int offset, uint32_t changed_bits, uint32_t value) {
    int i;
    uint32_t mask;
    int new, old;

    mask = BIT(31);
    for (i = 0; i < 32; i++) {
	if (changed_bits & mask) {
	    if (value & mask) {
		old = 0;
		new = 1;
	    } else {
		old = 1;
		new = 0;
	    }
	    sendS88Event(canfd, cd, sysid, bus * (MAXMODULES + 1) * 16 + offset + i, old, new);
	    if (verbose)
		fprintf(stderr, "Sending event for Bus %d Contact %d State %d\n", bus, offset + i, new);
	}
	mask >>= 1;
    }
}

void generate_events(int bus) {
    int i;
    uint32_t c;

    /* using Peter Daneggers 2 bit debouncing buffer code */
    for (i = 0; i <= ((s88[bus].blen * 16 - 1) >> 5); i++) {
	c = s88[bus].bus_state[i] ^ ~s88[bus].bus_actual[i];
	s88[bus].bus_ct0[i] = ~(s88[bus].bus_ct0[i] & c);
	s88[bus].bus_ct1[i] = s88[bus].bus_ct0[i] ^ (s88[bus].bus_ct1[i] & c);
	c &= s88[bus].bus_ct0[i] & s88[bus].bus_ct1[i];	/* 2 bit rollover */
	s88[bus].bus_state[i] ^= c;
	create_event(bus, i * 32, c, s88[bus].bus_actual[i]);
    }
}

int main(int argc, char **argv) {
    int opt;
    char *canint = DEF_INT;
    char *lenp = NULL;
    char *tp;
    struct sockaddr_can caddr;
    struct ifreq ifr;
    fd_set rfds;
    struct timeval stv, btv, atv, rtv;
    int x, i;
    int pidfd;

    while ((opt = getopt(argc, argv, "vfib:e:h?")) != -1)
	switch (opt) {
	case 'v':
	    verbose = 1;
	    break;
	case 'f':
	    foreground = 1;
	    break;
	case 'i':
	    invert = 1;
	    break;
	case 'b':
	    canint = strdup(optarg);
	    break;
	case 'e':
	    sysid = strtol(optarg, NULL, 0);
	    break;
	case 't':
	    pollt = atoi(optarg) * 1000;
	    break;
	case 'l':
	    lenp = strdup(optarg);
	    break;
	case 'h':
	case '?':
	    usage(argv[0]);
	    exit(EXIT_SUCCESS);
	default:
	    usage(argv[0]);
	    exit(EXIT_FAILURE);
	}

    cd.hash = generateHash(UID);

    canfd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (canfd < 0) {
	fprintf(stderr, "creating CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    memset(&caddr, 0, sizeof(caddr));
    caddr.can_family = AF_CAN;
    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strncpy(ifr.ifr_name, canint, sizeof(ifr.ifr_name));
    ifr.ifr_name[sizeof(ifr.ifr_name) - 1] = 0;
    if (ioctl(canfd, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN interfacce %s error: %s\n", ifr.ifr_name, strerror(errno));
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;
    if (bind(canfd, (struct sockaddr *)&caddr, sizeof(caddr)) < 0) {
	fprintf(stderr, "binding CAN socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    /* setup S88 busses */
    /* Bananapi Expansion board */
    if (gpio_aw_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    gpio_aw_select_output(CLOCK_PIN);
    gpio_aw_select_output(LOAD_PIN);
    gpio_aw_select_output(RESET_PIN);
    gpio_aw_select_input(DATA_PIN);
    /* Housekeeping:  increas config_num for number of busses  and s88 array */
    config_num++;
    if ((s88 = realloc(s88, (config_num - 2) * sizeof(struct s88_bus))) == NULL) {
	fprintf(stderr, "memory allocation failed: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    memset(s88 + config_num - 3, 0, sizeof(struct s88_bus));
    s88[config_num - 3].blen = 1;
    memset(s88[config_num - 3].bus_actual, 0, sizeof(s88[config_num - 3].bus_actual));
    memset(s88[config_num - 3].bus_state, 0xff, sizeof(s88[config_num - 3].bus_state));
    memset(s88[config_num - 3].bus_ct0, 0xff, sizeof(s88[config_num - 3].bus_ct0));
    memset(s88[config_num - 3].bus_ct1, 0xff, sizeof(s88[config_num - 3].bus_ct1));
    s88[config_num - 3].poll = bpi_poll;

    if (lenp != NULL) {
	for( i = 0, tp = strtok(lenp, ","); (tp != NULL) && (i < config_num - 2); i++) {
	    s88[i].blen = atoi(tp);
	    tp = strtok(NULL, ",");
	}
    }

    if (!foreground) {
	if ((pidfd = open(PIDFILE,  O_RDWR|O_CREAT|O_EXCL|O_NOCTTY, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0 ) {
	    fprintf(stderr, "Cannot open pidfile %s: %s\n", PIDFILE, strerror(errno));
	    exit(EXIT_FAILURE);
        }
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Cannot daemon()ise: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	/* TODO: Catch errors here */
	dprintf(pidfd, "%d\n", getpid());
	close(pidfd);
    }

/* main loop */
    stv.tv_sec = 0;
    stv.tv_usec = pollt;
    while (1) {
	FD_ZERO(&rfds);
	FD_SET(canfd, &rfds);
	gettimeofday(&btv, NULL);
	x = select(canfd + 1, &rfds, NULL, NULL, &stv);
	gettimeofday(&atv, NULL);
	if (x == 0) {		/* timeout */
	    /* poll S88 */
	    for (i = 0; i < config_num - 2; i++) {
		s88[i].poll(s88 + i);
		generate_events(i);
	    }
	} else {		/* our only fd is ready */
	    handlecan();
	}

	timersub(&atv, &btv, &rtv);
	atv.tv_sec = 0;
	atv.tv_usec = pollt;
	timersub(&atv, &rtv, &stv);
	atv.tv_sec = 0;
	atv.tv_usec = 0;
	if (timercmp(&stv, &atv, <)) {
	    stv.tv_sec = 0;
	    stv.tv_usec = pollt;
	}
    }
/* not reached */
}
