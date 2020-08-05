/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/*  LED		PI14	270
 *  bush button PI10	266
 */

#define _GNU_SOURCE
#include <ctype.h>
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

#include <net/if.h>
#include <netinet/in.h>
#include <linux/can.h>

#include <syslog.h>

#include "cs2-config.h"
#include "read-cs2-config.h"

extern struct loco_data_t *loco_data;
extern struct loco_names_t *loco_names;
int do_loop;

#define OUR_HASH	0x4712
#define BIT(x)		(1<<x)
#define MINDELAY	1000000	/* min delay in usec */
#define MAXLEN		64	/* maximum string length */
#define MAX_BUFFER	8
#define MAX_SYSFS_LEN	256
#define DEF_INTERVAL	300
#define MAXLINE		256
#define FSM_WATCHDOG_T	60
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define fprint_syslog(pipe, spipe, text) \
	   do { fprintf( pipe, "%s: " text "\n", __func__); \
		syslog( spipe, "%s: " text "\n", __func__); } while (0)

char loco_dir[MAXLINE];

uint16_t CRCCCITT(uint8_t * data, size_t length, uint16_t seed);

unsigned int led_period;
pthread_mutex_t lock;

static unsigned char GET_MS2_LOCO_LIST[]   = { 0x6c, 0x6f, 0x6b, 0x6c, 0x69, 0x73, 0x74, 0x65 };	/* lokliste */
static unsigned char GET_MS2_LOCO_NAMES[]  = { 0x6c, 0x6f, 0x6b, 0x6e, 0x61, 0x6d, 0x65, 0x6e };	/* loknamen */
static unsigned char GET_MS2_CONFIG_LOCO[] = { 0x6c, 0x6f, 0x6b, 0x69, 0x6e, 0x66, 0x6f, 0x00 };	/* lokinfo  */

static char *T_CAN_FORMAT_STRG	= "   -> CAN     0x%08X   [%d]";
static char *F_CAN_FORMAT_STRG	= "      CAN ->  0x%08X   [%d]";

static char *LOCOLIST = "Lokliste";

enum gpio_edges {
    EDGE_NONE,
    EDGE_RISING,
    EDGE_FALLING,
    EDGE_BOTH
};

enum fsm_get_data {
    FSM_IDLE,
    FSM_START,
    FSM_GET_LOCO_NAMES,
    FSM_GET_LOCOS_BY_NAME
};

#define LED_PATTERN_MAX	4
uint32_t LED_HB_SLOW[]	= { 100, 200, 100, 800 };
uint32_t LED_HB_FAST[]	= {  70,  70,  70, 220 };
uint32_t LED_ERROR[]	= { 600, 400, 600, 400 };

enum LED_STATUS {
    LED_ST_HB_SLOW,
    LED_ST_HB_FAST,
    LED_ST_ERROR,
};

struct trigger_t {
    struct sockaddr_can caddr;
    int socket;
    int background;
    int verbose;
    int interval;
    int fsm_watchdog;
    int led_pin;
    int led_pattern;
    int pb_pin;
    int pb_fd;
    int fsm_state;
    int loco_number;
    int loco_counter;
    int loco_uid;
    uint16_t hash;
    uint16_t hw_id;
    uint16_t length;
    uint16_t crc;
    int data_index;
    int v3x;
    uint8_t *data;
    char *loco_file;
    struct loco_names_t *loco_names;
};

uint16_t be16(uint8_t *u) {
    return (u[0] << 8) | u[1];
}

void signal_handler(int sig) {
    syslog(LOG_WARNING, "got signal %s\n", strsignal(sig));
    do_loop = 0;
}

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -kfv [-i <CAN int>][-t <sec>][-l <LED pin>][-p <push button pin>]\n", prg);
    fprintf(stderr, "   Version 1.10\n\n");
    fprintf(stderr, "         -c <loco_dir>        set the locomotive file dir - default %s\n", loco_dir);
    fprintf(stderr, "         -i <CAN interface>   using can interface\n");
    fprintf(stderr, "         -t <interval in sec> using timer in sec\n");
    fprintf(stderr, "         -l <LED pin>         LED pin (e.g. BPi PI14 -> 270)\n");
    fprintf(stderr, "         -p <push button>     push button (e.g. BPi PI10 -> 266)\n");
    fprintf(stderr, "         -k                   use loco 'Lokliste' F0 as trigger\n");
    fprintf(stderr, "         -n                   MS2 Version >=3.55\n");
    fprintf(stderr, "         -f                   run in foreground (for debugging)\n");
    fprintf(stderr, "         -v                   be verbose\n\n");
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

void print_can_frame(char *format_string, struct can_frame *frame) {
    int i;
    time_stamp();
    printf(format_string, frame->can_id & CAN_EFF_MASK, frame->can_dlc);
    for (i = 0; i < frame->can_dlc; i++) {
	printf(" %02x", frame->data[i]);
    }
    if (frame->can_dlc < 8) {
	for (i = frame->can_dlc; i < 8; i++) {
	    printf("   ");
	}
    }
    printf("  ");
    for (i = 0; i < frame->can_dlc; i++) {
	if (isprint(frame->data[i]))
	    printf("%c", frame->data[i]);
	else
	    putchar(46);
    }
    printf("\n");
}

int send_can_frame(int can_socket, struct can_frame *frame, int verbose) {

    frame->can_id &= CAN_EFF_MASK;
    frame->can_id |= CAN_EFF_FLAG;
    /* send CAN frame */
    if (write(can_socket, frame, sizeof(*frame)) != sizeof(*frame)) {
	fprintf(stderr, "error writing CAN frame: %s\n", strerror(errno));
	return -1;
    }
    if (verbose)
	print_can_frame(T_CAN_FORMAT_STRG, frame);
    return 0;
}

int get_ms2_loco_names(struct trigger_t *trigger, uint8_t start, int8_t end) {
    struct can_frame frame;

    memset(&frame, 0, sizeof(frame));
    if ((start > 99) || (end > 99))
	return (EXIT_FAILURE);
    /* get Config Data */
    frame.can_id = 0x00400300 | trigger->hash;

    /* first frame */
    frame.can_dlc = 8;
    if (trigger->v3x) {
	memcpy(frame.data, GET_MS2_LOCO_LIST, sizeof(frame.data));
	if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	    return (EXIT_FAILURE);
    } else {
	memcpy(frame.data, GET_MS2_LOCO_NAMES, sizeof(frame.data));
	if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	    return (EXIT_FAILURE);
	/* second frame */
	memset(frame.data, 0, sizeof(frame.data));
	snprintf((char *)frame.data, sizeof(frame.data), "%d %d", start, end);

	if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	    return (EXIT_FAILURE);
    }
    return 0;
}

int get_ms2_dbsize(struct trigger_t *trigger) {
    int ret;

    ret = get_ms2_loco_names(trigger, 0, 0);
    return ret;
}

void reset_loco_list(void) {
    shrink_loco_list(LOCOLIST);
}

int get_ms2_locoinfo(struct trigger_t *trigger, char *loco_name) {
    struct can_frame frame;

    memset(&frame, 0, sizeof(frame));

    /* get Config Data */
    frame.can_id = 0x00400300 | trigger->hash;

    frame.can_dlc = 8;
    memcpy(frame.data, GET_MS2_CONFIG_LOCO, sizeof(frame.data));
    if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	return (EXIT_FAILURE);

    memset(frame.data, 0, 8);
    if (strnlen(loco_name , 16) >= 8)
	memcpy(frame.data, loco_name, 8);
    else
	strncpy((char *)frame.data, loco_name, 8 - 1);

    if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	return (EXIT_FAILURE);

    memset(frame.data, 0, 8);
    if (strnlen(loco_name, 16) == 16)
	memcpy(frame.data, &loco_name[8], 8);
    else if (strnlen(loco_name, 16) > 8)
	strncpy((char *)frame.data, &loco_name[8], 8 - 1);

    if (send_can_frame(trigger->socket, &frame, trigger->verbose) < 0)
	return (EXIT_FAILURE);

    return 0;
}

int get_locos(struct trigger_t *trigger, char *loco_file) {
    FILE *fp;
    int ret;

    ret = get_ms2_dbsize(trigger);
    /* TODO */
    fp = fopen(loco_file, "wb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error writing locomotive file [%s]\n", __func__, loco_file);
	return EXIT_FAILURE;
    } else {
	/* print_locos(fp); */
    }
    fclose(fp);
    return ret;
}

int gpio_export(int pin) {
    char buffer[MAX_BUFFER];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open GPIO export for writing: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    bytes_written = snprintf(buffer, MAX_BUFFER, "%d", pin);
    if (write(fd, buffer, bytes_written) < 0) {
	fprintf(stderr, "error writing GPIO config: %s\n", strerror(errno));
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
	fprintf(stderr, "error writing GPIO config: %s\n", strerror(errno));
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
	return (EXIT_FAILURE);
    }

    if (dir)
	ret = write(fd, "in", 3);
    else
	ret = write(fd, "out", 4);

    if (ret == -1) {
	fprintf(stderr, "%s: Failed to set direction: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    close(fd);
    return (0);
}

int gpio_open(int pin) {
    char path[MAX_SYSFS_LEN];
    int fd;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open gpio value for reading: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    return fd;
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

int gpio_edge(int pin, int value) {
    char path[MAX_SYSFS_LEN];
    int fd, ret;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/edge", pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "%s: Failed to open gpio value for writing: %s\n", __func__, strerror(errno));
	return (EXIT_FAILURE);
    }

    switch (value) {
    case EDGE_RISING:
	ret = write(fd, "rising", 7);
	break;
    case EDGE_FALLING:
	ret = write(fd, "falling", 8);
	break;
    case EDGE_BOTH:
	ret = write(fd, "both", 5);
	break;
    case EDGE_NONE:
	ret = write(fd, "none", 5);
	break;
    default:
	fprintf(stderr, "%s: gpio edge type unknown\n", __func__);
	return (EXIT_FAILURE);
    }
    return ret;
}

void set_led_pattern(struct trigger_t *trigger, int pattern) {
    pthread_mutex_lock(&lock);
    trigger->led_pattern = pattern;
    pthread_mutex_unlock(&lock);
}

int get_data(struct trigger_t *trigger, struct can_frame *frame) {
    FILE *fp;
    uint16_t crc;

    if ((frame->can_dlc == 6) || (frame->can_dlc == 7)) {
	trigger->length = be16(&frame->data[2]);
	trigger->crc    = be16(&frame->data[4]);
	if (!trigger->background)
	    printf("length 0x%04x  crc 0x%04x\n", trigger->length, trigger->crc);

	trigger->data = (uint8_t *) calloc(trigger->length + 1, 1);
	if (!trigger->data) {
	    fprintf(stderr, "can't alloc memory for config data: %s\n", strerror(errno));
	    return (EXIT_FAILURE);
	}
	trigger->data_index = 0;
	return 0;
    }

    if (trigger->data_index < trigger->length) {
	memcpy(&trigger->data[trigger->data_index], frame->data, 8);
	trigger->data_index += 8;
    } else {
	printf("Error: unexpected data\n");
	return (EXIT_FAILURE);
    }

    if (trigger->data_index == trigger->length) {
	if (!trigger->background && trigger->verbose)
	    printf("data complete %d %d\n", trigger->data_index, trigger->length);
	crc = CRCCCITT(trigger->data, trigger->length, 0xFFFF);

	if (crc == trigger->crc) {
	    if (!trigger->background && trigger->verbose)
		printf("crc 0x%04x 0x%04x\n", crc, trigger->crc);

	    strip_ms2_spaces(trigger->data, trigger->length);
	}
	if (!trigger->background && trigger->verbose)
	    printf("Data:\n%s\n", trigger->data);

	switch (trigger->fsm_state) {
	case FSM_START:
	    set_led_pattern(trigger, LED_ST_HB_FAST);
	    trigger->loco_number = get_value((char *)trigger->data, " .wert=");
	    if (!trigger->background && trigger->verbose)
		printf("Number of new locos: %d\n", trigger->loco_number);
	    if (trigger->loco_number) {
		trigger->fsm_state = FSM_GET_LOCO_NAMES;
		get_ms2_loco_names(trigger, 0, 1);
	    }
	    if (trigger->v3x) {
		trigger->fsm_state = FSM_GET_LOCOS_BY_NAME;
		read_loco_names((char *)trigger->data);
		trigger->loco_names = loco_names;
		if (trigger->loco_names) {
		    get_ms2_locoinfo(trigger, trigger->loco_names->name);
		    trigger->loco_names = trigger->loco_names->hh.next;
		}
		if (!trigger->background && trigger->verbose)
		    printf("FSM: V3.x State change FSM_START -> FSM_GET_LOCOS_BY_NAME\n");
	    }
	    break;
	case FSM_GET_LOCO_NAMES:
	    read_loco_names((char *)trigger->data);
	    if (trigger->loco_counter + 1 <= trigger->loco_number) {
		get_ms2_loco_names(trigger, trigger->loco_counter + 1, 2);
		trigger->loco_counter += 2;
	    } else {
		trigger->fsm_state = FSM_GET_LOCOS_BY_NAME;
		trigger->loco_names = loco_names;
		if (trigger->loco_names) {
		    get_ms2_locoinfo(trigger, trigger->loco_names->name);
		    trigger->loco_names = trigger->loco_names->hh.next;
		} else {
		    trigger->fsm_state = FSM_START;
		}
		if (!trigger->background && trigger->verbose)
		    print_loco_names(stdout);
	    }
	    break;
	case FSM_GET_LOCOS_BY_NAME:
	    read_loco_data((char *)trigger->data, CONFIG_STRING);
	    if (trigger->loco_names) {
		if (!trigger->background && trigger->verbose)
		    printf("delete loco [%s] and read data (again)\n", trigger->loco_names->name);
		delete_loco_by_name(trigger->loco_names->name);
		get_ms2_locoinfo(trigger, trigger->loco_names->name);
		trigger->loco_names = trigger->loco_names->hh.next;
	    } else {
		fp = fopen(trigger->loco_file, "wb");
		if (fp == NULL) {
		    fprintf(stderr, "%s: error writing loco file [%s]\n", __func__, trigger->loco_file);
		} else {
		    if (!trigger->background && trigger->verbose)
			printf("writing new loco file [%s]\n", trigger->loco_file);
		    print_locos(fp);
		    fclose(fp);
		}
		/* start over with a new list */
		delete_all_loco_names();
		set_led_pattern(trigger, LED_ST_HB_SLOW);
		if (!trigger->background && trigger->verbose)
		    printf("FSM_IDLE done\n");
		trigger->fsm_state = FSM_IDLE;
		trigger->loco_counter = 0;
	    }
	    break;
	default:
	    break;
	}

	/* if (!trigger->background && trigger->verbose) {
	    print_locos(stdout);
	    printf("max locos : %d\n", get_loco_max());
	} */
	free(trigger->data);
    }
    return 0;
}

/* Blink LED */
void *LEDMod(void *ptr) {
    int fd, led_pattern, i;
    char path[MAX_SYSFS_LEN];
    struct trigger_t *trigger = (struct trigger_t *)ptr;
    uint32_t *led_pattern_p;

    snprintf(path, MAX_SYSFS_LEN, "/sys/class/gpio/gpio%d/value", trigger->led_pin);
    fd = open(path, O_WRONLY);
    if (fd < 0) {
	fprintf(stderr, "Failed to open gpio value for writing: %s\n", strerror(errno));
	exit(1);
    }

    while (1) {
	pthread_mutex_lock(&lock);
	led_pattern = trigger->led_pattern;
	pthread_mutex_unlock(&lock);

	switch (led_pattern) {
	case LED_ST_HB_SLOW:
	    led_pattern_p = LED_HB_SLOW;
	    break;
	case LED_ST_HB_FAST:
	    led_pattern_p = LED_HB_FAST;
	    break;
	case LED_ST_ERROR:
	    led_pattern_p = LED_ERROR;
	    break;
	default:
	    led_pattern_p = LED_ERROR;
	    break;
	}

	for (i = 0; i < LED_PATTERN_MAX; i++) {
	    if (write(fd, "0", 2) != 2)
		fprintf(stderr, "%s: Failed to write GPIO value!\n", __func__);
	    usec_sleep(led_pattern_p[i++] * 1000);
	    if (write(fd, "1", 2) != 2)
		fprintf(stderr, "%s: Failed to write GPIO value!\n", __func__);
	    usec_sleep(led_pattern_p[i] * 1000);
	}
    }
}

int main(int argc, char **argv) {
    pthread_t pth;
    struct sigaction sigact;
    sigset_t blockset, emptyset;
    int opt, nready, interval, fsm_watchdog;
    struct ifreq ifr;
    struct trigger_t trigger_data;
    struct sockaddr_can caddr;
    fd_set readfds, exceptfds;
    struct can_frame frame;
    uint16_t uid;
    uint8_t buffer[MAXLEN];
    struct timespec ts;

    memset(&trigger_data, 0, sizeof(trigger_data));
    memset(ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    strcpy(ifr.ifr_name, "can0");
    do_loop = 1;
    interval = 0;
    fsm_watchdog = FSM_WATCHDOG_T;

    strcpy(loco_dir, "/www/config");

    trigger_data.hash = OUR_HASH;
    trigger_data.fsm_state = FSM_IDLE;
    trigger_data.led_pin = -1;
    trigger_data.pb_pin = -1;

    trigger_data.background = 1;

    while ((opt = getopt(argc, argv, "c:i:l:p:t:knfvh?")) != -1) {
	switch (opt) {
	case 'c':
	    if (strnlen(optarg, MAXLINE) < MAXLINE) {
		strncpy(loco_dir, optarg, sizeof(loco_dir) - 1);
	    } else {
		fprintf(stderr, "config file dir to long\n");
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'i':
	    strncpy(ifr.ifr_name, optarg, sizeof(ifr.ifr_name) - 1);
	    break;
	case 't':
	    trigger_data.interval = atoi(optarg);
	    interval = trigger_data.interval;
	    break;
	case 'l':
	    trigger_data.led_pin = atoi(optarg);
	    break;
	case 'p':
	    trigger_data.pb_pin = atoi(optarg);
	    break;
	case 'k':
	    trigger_data.loco_uid = 1;
	    break;
	case 'n':
	    trigger_data.v3x = 1;
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

    /* prepare CAN socket */
    if ((trigger_data.socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
	fprintf(stderr, "error creating CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    memset(&caddr, 0, sizeof(caddr));
    caddr.can_family = AF_CAN;
    if (ioctl(trigger_data.socket, SIOCGIFINDEX, &ifr) < 0) {
	fprintf(stderr, "setup CAN socket error: %s %s\n", strerror(errno), ifr.ifr_name);
	exit(EXIT_FAILURE);
    }
    caddr.can_ifindex = ifr.ifr_ifindex;

    if (bind(trigger_data.socket, (struct sockaddr *)&caddr, sizeof(caddr)) < 0) {
	fprintf(stderr, "error binding CAN socket: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    trigger_data.caddr = caddr;

    if (trigger_data.background) {
	if (daemon(0, 0) < 0) {
	    fprintf(stderr, "Going into background failed: %s\n", strerror(errno));
	    exit(EXIT_FAILURE);
	}
    }

    /* Create thread if LED pin defined */
    if ((trigger_data.led_pin) > 0) {
	trigger_data.led_pattern = LED_ST_HB_SLOW;
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

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog("clone-ms2-config", LOG_CONS | LOG_NDELAY, LOG_DAEMON);

    sigemptyset(&blockset);
    sigaddset(&sigact.sa_mask, SIGHUP);
    sigaddset(&sigact.sa_mask, SIGINT);
    sigaddset(&sigact.sa_mask, SIGTERM);
    if (sigprocmask(SIG_BLOCK, &blockset, NULL) < 0) {
	fprint_syslog(stderr, LOG_ERR, "cannot set SIGNAL block mask\n");
	return (EXIT_FAILURE);
    }

    sigact.sa_handler = signal_handler;
    sigact.sa_flags = 0;
    sigemptyset(&sigact.sa_mask);
    sigaction(SIGHUP, &sigact, NULL);
    sigaction(SIGINT, &sigact, NULL);
    sigaction(SIGTERM, &sigact, NULL);
    sigemptyset(&emptyset);

    if (asprintf(&trigger_data.loco_file, "%s/%s", loco_dir, "lokomotive.cs2") < 0) {
        fprintf(stderr, "can't alloc buffer for loco_name: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    read_loco_data(trigger_data.loco_file, CONFIG_FILE);
    /* print_locos(stdout);
    printf("max locos : %d\n", get_loco_max()); */

    /* find trigger loco if requested */
    if (trigger_data.loco_uid) {
	trigger_data.loco_uid = get_loco_uid(LOCOLIST);
	if (!trigger_data.background && trigger_data.verbose)
	    printf("trigger set: Lokliste F0 UID 0x%04x\n", trigger_data.loco_uid);
    }

    /* initialize push button */
    if ((trigger_data.pb_pin) > 0) {
	/* first free pin */
	gpio_unexport(trigger_data.pb_pin);
	gpio_export(trigger_data.pb_pin);
	gpio_direction(trigger_data.pb_pin, 1);
	gpio_edge(trigger_data.pb_pin, EDGE_FALLING);
	trigger_data.pb_fd = gpio_open(trigger_data.pb_pin);
    }

    ts.tv_sec = 1;
    ts.tv_nsec = 0;

    FD_ZERO(&readfds);
    FD_ZERO(&exceptfds);
    /* delete pending push button event */
    if ((trigger_data.pb_pin) > 0) {
	if (read(trigger_data.pb_fd, NULL, 100) < 0)
	    fprintf(stderr, "error reading GPIO trigger: %s\n", strerror(errno));
	lseek(trigger_data.pb_fd, 0, SEEK_SET);
    }
    while (do_loop) {
	FD_SET(trigger_data.socket, &readfds);
	/* extend FD_SET only if push button pin is set */
	if (trigger_data.pb_pin > 0)
	    FD_SET(trigger_data.pb_fd, &exceptfds);
	nready = pselect(MAX(trigger_data.socket, trigger_data.pb_fd) + 1, &readfds, NULL, &exceptfds, &ts, &emptyset);
	if (nready == 0) {
	    /* periodic task check */
	    ts.tv_sec = 1;
	    ts.tv_nsec = 0;
	    if (trigger_data.interval) {
		if (interval-- == 0) {
		    trigger_data.fsm_state = FSM_START;
		    get_ms2_dbsize(&trigger_data);
		    interval = trigger_data.interval;
		}
	    }
	    if (trigger_data.fsm_state == FSM_IDLE) {
		fsm_watchdog = FSM_WATCHDOG_T;
	    } else {
		if (fsm_watchdog-- == 0) {
		    trigger_data.fsm_state = FSM_IDLE;
		    fsm_watchdog = FSM_WATCHDOG_T;
		}
	    }
	    continue;
	} else if (nready < 0) {
	    fprintf(stderr, "pselect exception: %s\n", strerror(errno));
	    syslog(LOG_WARNING, "pselect exception: %s\n", strerror(errno));
	    /* will be interrupted by do_loop = 0 */
	    continue;
	}
	/* CAN frame event */
	if (FD_ISSET(trigger_data.socket, &readfds)) {
	    if (read(trigger_data.socket, &frame, sizeof(struct can_frame)) < 0)
		fprintf(stderr, "error reading CAN frame: %s\n", strerror(errno));

	    if (frame.can_id & CAN_EFF_FLAG) {
		switch ((frame.can_id & 0x00FF0000UL) >> 16) {
		case 0x31:
		    if (trigger_data.verbose)
			print_can_frame(F_CAN_FORMAT_STRG, &frame);
		    break;
		case 0x0C:
		    uid = be16(&frame.data[2]);
		    if (trigger_data.verbose)
			print_can_frame(F_CAN_FORMAT_STRG, &frame);
		    /* initiate trigger when loco "Lokliste" and F0 pressed */
		    if ((uid == trigger_data.loco_uid) && (frame.data[4] == 0)) {
			if (trigger_data.fsm_state == FSM_IDLE) {
			    trigger_data.fsm_state = FSM_START;
			    if (!trigger_data.background && trigger_data.verbose)
				printf("FSM_START clone MS2 locos\n");
			    get_ms2_dbsize(&trigger_data);
			}
		    }
		    /* delete all locos if "Lokliste" exists and F4 pressed */
		    if ((uid == trigger_data.loco_uid) && (frame.data[4] == 4)) {
			if (trigger_data.fsm_state == FSM_IDLE) {
			    trigger_data.fsm_state = FSM_START;
			    reset_loco_list();
			    if (!trigger_data.background && trigger_data.verbose)
				printf("FSM_START reset loco file & read MS2 locos\n");
			    get_ms2_dbsize(&trigger_data);
			}
		    }
		    break;
		case 0x41:
		    if (trigger_data.verbose)
			print_can_frame(F_CAN_FORMAT_STRG, &frame);
		    break;
		case 0x42:
		    if (trigger_data.verbose)
			print_can_frame(F_CAN_FORMAT_STRG, &frame);
		    /* check if the data belongs to us */
		    /* TODO */
		    if (((frame.can_id & 0x0000FFFF) != OUR_HASH) && !(trigger_data.v3x))
			break;
		    get_data(&trigger_data, &frame);
		    break;
		default:
		    if (trigger_data.verbose)
			print_can_frame(F_CAN_FORMAT_STRG, &frame);
		    break;
		}
	    }
	}
	/* push button event */
	if (FD_ISSET(trigger_data.pb_fd, &exceptfds)) {
	    trigger_data.fsm_state = FSM_START;
	    get_ms2_dbsize(&trigger_data);

	    lseek(trigger_data.pb_fd, 0, SEEK_SET);
	    if (read(trigger_data.pb_fd, buffer, sizeof(buffer)) < 0)
		fprintf(stderr, "error reading GPIO status: %s\n", strerror(errno));
	    printf("push button event\n");
	}
    }
    if ((trigger_data.pb_pin) > 0)
	gpio_unexport(trigger_data.pb_pin);
    if ((trigger_data.led_pin) > 0) {
	/* pthread_join(pth, (void *)&trigger_data);
	pthread_mutex_unlock(&lock); */
	gpio_unexport(trigger_data.led_pin);
    }
    return EXIT_SUCCESS;
}
