/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*  LED		PI14	270
 *  switch CE0	PI10	266
 */

#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/in.h>

#define BIT(x)		(1<<x)
#define MINDELAY	1000000	/* min delay in usec */
#define MAXIPLEN	40	/* maximum IP string length */
#define UDPPORT		15731
#define MAX_BUFFER	8
#define MAX_SYSFS_LEN	256
#define DEF_INTERVAL	300

unsigned int led_period;
pthread_mutex_t lock;

unsigned char CLONE_CONFIG_REQUEST[] = { 0x00, 0x40, 0xaf, 0x7e, 0x00 };

struct trigger_t {
    struct sockaddr_in baddr;
    int socket;
    int background;
    int verbose;
    int interval;
    int led_pin;
    int switch_pin;
    int led_fd;
    int switch_fd;
    uint16_t hash;
    uint16_t hw_id;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <sec>][-p <port>]\n", prg);
    fprintf(stderr, "   Version 1.0\n\n");
    fprintf(stderr, "         -b <bcast_addr/int> broadcast address or interface - default 255.255.255.255/br-lan\n");
    fprintf(stderr, "         -i <interval in sec>   using timer in sec / 0 -> only once - default %d\n", DEF_INTERVAL);
    fprintf(stderr, "         -p <port>           destination port of the server - default %d\n", UDPPORT);
    fprintf(stderr, "         -l                  LED pin (e.g. BPi PI14 -> 270)\n");
    fprintf(stderr, "         -s                  switch pin (e.g. BPi PI10 -> 266)\n");
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

void print_net_frame(unsigned char *netframe) {
    uint32_t canid;
    int i, dlc;

    memcpy(&canid, netframe, 4);
    dlc = netframe[4];
    time_stamp();
    printf("         UDP->  0x%08X   [%u]", ntohl(canid), netframe[4]);
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

int gpio_export(int pin) {
    char buffer[MAX_BUFFER];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open export for writing: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    bytes_written = snprintf(buffer, MAX_BUFFER, "%d", pin);
    if (write(fd, buffer, bytes_written) < 0) {
	fprintf(stderr, "%s: Failed to write: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }
    close(fd);
    return (0);
}

int gpio_unexport(int pin) {
    char buffer[MAX_BUFFER];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open unexport for writing %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    bytes_written = snprintf(buffer, MAX_BUFFER, "%d", pin);
    if (write(fd, buffer, bytes_written) < 0) {
	fprintf(stderr, "%s: Failed to write: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }
    close(fd);
    return (0);
}

int gpio_direction(int pin, int dir) {
    char path[MAX_SYSFS_LEN];
    int fd, ret;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open gpio direction for writing: %s\n", __func__, strerror(errno));
	return (-1);
    }

    if (dir)
	ret = write(fd, "in", 3);
    else
	ret = write(fd, "out", 4);

    if (ret == -1) {
	fprintf(stderr, "%s: Failed to set direction!\n", __func__);
	return (-1);
    }

    close(fd);
    return (0);
}

int gpio_read(int pin) {
    char path[MAX_SYSFS_LEN];
    char value_str[3];
    int fd, ret;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open gpio value for reading: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    ret = read(fd, value_str, 3);
    if (ret < 0) {
	fprintf(stderr, "%s: Failed to read value!\n", __func__);
	return (EXIT_FAILURE);
    }

    close(fd);

    return (atoi(value_str));
}

int gpio_set(int pin, int value) {
    char path[MAX_SYSFS_LEN];
    int fd, ret;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open gpio value for writing: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    if (value)
	ret = write(fd, "1", 2);
    else
	ret = write(fd, "0", 2);

    if (ret != 2) {
	fprintf(stderr, "%s: Failed to write value!\n", __func__);
	return (EXIT_FAILURE);
    }

    close(fd);
    return (0);
}

/* Blink LED */
void *LEDMod(void *ptr) {
    int fd;
    char path[MAX_SYSFS_LEN];
    struct trigger_t *trigger = (struct trigger_t *)ptr;

#if 1
    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/value", trigger->led_pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "Failed to open gpio value for writing: %s\n", strerror(errno));
	exit(1);
    }

    while (1) {
    	if (write(fd, "1", 2) != 2)
	    fprintf(stderr, "%s: Failed to write value!\n", __func__);
	usec_sleep(led_period);
    	if (write(fd, "0", 2) != 2)
	    fprintf(stderr, "%s: Failed to write value!\n", __func__);
	usec_sleep(led_period);
    }
#else
    while (1) {
	usec_sleep(led_period);
	printf("*");
	usec_sleep(led_period);
	printf("-");
    }
#endif

}

int create_event(struct trigger_t *trigger) {
    int s;
    uint8_t netframe[13];

    memset(netframe, 0, sizeof(netframe));
    memcpy(netframe, CLONE_CONFIG_REQUEST, sizeof(CLONE_CONFIG_REQUEST));

    s = sendto(trigger->socket, netframe, 13, 0, (struct sockaddr *)&trigger->baddr, sizeof(trigger->baddr));
    if (s != 13) {
	fprintf(stderr, "%s: error sending UDP data: %s\n", __func__, strerror(errno));
	return -1;
    }
    if (!trigger->background)
	print_net_frame(netframe);
    return 0;
}

int main(int argc, char **argv) {
    pthread_t pth;
    int opt, ret, destination_port;
    struct sockaddr_in destaddr, *bsa;
    struct ifaddrs *ifap, *ifa;
    struct trigger_t trigger_data;
    char *udp_dst_address;
    char *bcast_interface;

    const int on = 1;
    memset(&trigger_data, 0, sizeof(trigger_data));
    trigger_data.led_pin = -1;
    trigger_data.switch_pin = -1;

    destination_port = UDPPORT;

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

    trigger_data.interval = DEF_INTERVAL;

    while ((opt = getopt(argc, argv, "b:l:i:p:s:fvh?")) != -1) {
	switch (opt) {
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
	case 'i':
	    trigger_data.interval = atoi(optarg);
	    break;
	case 'l':
	    trigger_data.led_pin = atoi(optarg);
	    break;
	case 'p':
	    destination_port = strtoul(optarg, (char **)NULL, 10);
	    destaddr.sin_port = htons(destination_port);
	    break;
	case 's':
	    trigger_data.switch_pin = atoi(optarg);
	    break;
	case 'v':
	    trigger_data.verbose = 1;
	    break;
	case 'f':
	    trigger_data.background = 0;
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

    if (bcast_interface)
	free(bcast_interface);

    freeifaddrs(ifap);

    ret = inet_pton(AF_INET, udp_dst_address, &destaddr.sin_addr);
    if (ret <= 0) {
	if (ret == 0)
	    fprintf(stderr, "UDP IP invalid\n");
	else
	    fprintf(stderr, "invalid address family\n");
	exit(EXIT_FAILURE);
    }
    if (!trigger_data.background && trigger_data.verbose)
	printf("using broadcast address %s interval %d sec\n", udp_dst_address, trigger_data.interval);
    /* open udp socket */
    if ((trigger_data.socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "UDP socket error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    if (setsockopt(trigger_data.socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
	fprintf(stderr, "UDP set broadcast option error: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    trigger_data.baddr = destaddr;

#if 0
    if (!trigger_data.interval) {
	create_event(&trigger_data);
	return EXIT_SUCCESS;
    }
#endif

    /* Create thread if LED */
    if ((trigger_data.led_pin) > 0) {
	led_period = 200000;
	gpio_export(trigger_data.led_pin);
	gpio_direction(trigger_data.led_pin, 0);

	if (pthread_mutex_init(&lock, NULL)) {
	    fprintf(stderr, "can't init mutex %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (pthread_create(&pth, NULL, LEDMod, &trigger_data)) {
	    fprintf(stderr, "can't create pthread %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
	if (!trigger_data.background && trigger_data.verbose)
	    printf("created LED thread\n");
    }

    if (trigger_data.background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    /* loop forever if interval is set */
    if (trigger_data.interval) {
	while (1) {
	    ret = create_event(&trigger_data);
	    if (ret < 0) {
		fprintf(stderr, "problem sending event data - terminating\n");
		exit(EXIT_FAILURE);
	    }
	    /* TODO : wait until copy is done */
	    sleep(trigger_data.interval);
	}
    } else {
	ret = create_event(&trigger_data);
	if (ret < 0) {
	    fprintf(stderr, "problem sending event data - terminating\n");
	    exit(EXIT_FAILURE);
	}
    }
    /* TODO : wait until copy is done */

    if ((trigger_data.switch_pin) > 0)
	gpio_unexport(trigger_data.switch_pin);
    if ((trigger_data.led_pin) > 0) {
	gpio_unexport(trigger_data.led_pin);
	pthread_join(pth, (void *)&trigger_data);
	pthread_mutex_unlock(&lock);
    }
    return 0;
}
