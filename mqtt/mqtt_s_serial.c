#include <libgen.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>
#include <assert.h>
#include <sys/poll.h>

#include <mosquitto.h>

const char *mqtt_broker_host = "localhost";
const int mqtt_broker_port = 1883;

int background;

typedef struct {
    uint32_t baud;
    speed_t speed;
} baudrate_t;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -d<serial_device> -m<mosquitto_ip>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -d <serial_device>  e.g. /dev/ttyUSB0\n");
    fprintf(stderr, "         -s <baudrate>       serial device baudrate\n");
    fprintf(stderr, "         -b <MQTT broker>    Mosquitto broaker - default localhost\n");
    fprintf(stderr, "         -f                  foreground\n\n");
}

speed_t serial_speed(uint32_t baudrate) {
    int i = 0;

    static baudrate_t rates[] = {
	{0, B0},
	{50, B50},
	{75, B75},
	{110, B110},
	{134, B134},
	{150, B150},
	{200, B200},
	{300, B300},
	{600, B600},
	{1200, B1200},
	{1800, B1800},
	{2400, B2400},
	{4800, B4800},
#ifdef B7200
	{7200, B7200},
#endif
	{9600, B9600},
#ifdef B14400
	{14400, B14400},
#endif
	{19200, B19200},
	{19200, EXTA},
#ifdef B28800
	{28800, B28800},
#endif
	{38400, B38400},
	{38400, EXTB},
	{57600, B57600},
#ifdef B76800
	{76800, B76800},
#endif
	{115200, B115200},
	{230400, B230400},
#ifdef B460800
	{460800, B460800},
#endif
#ifdef B500000
	{500000, B500000},
#endif
#ifdef B4000000
	{576000, B576000},
#endif
#ifdef B921600
	{921600, B921600},
#endif
#ifdef B1000000
	{1000000, B1000000},
#endif
#ifdef B1152000
	{1152000, B1152000},
#endif
#ifdef B1500000
	{1500000, B1500000},
#endif
#ifdef B2000000
	{2000000, B2000000},
#endif
#ifdef B2500000
	{2500000, B2500000},
#endif
#ifdef B3000000
	{3000000, B3000000},
#endif
#ifdef B3500000
	{3500000, B3500000},
#endif
#ifdef B4000000
	{4000000, B4000000},
#endif
	{UINT32_MAX, B9600},
    };

    while (baudrate > rates[i++].baud) ;

    return rates[--i].speed;
}

int openDevice(const char *dev, speed_t speed, int tim, int rts, int dtr) {
    int fd;
    struct termios options;

    fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
	return fd;
    }
    if (fcntl(fd, F_SETFL, O_NONBLOCK | fcntl(fd, F_GETFL)) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }
    if (tcgetattr(fd, &options) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }

    /*
     * Raw mode 8N2
     *
     *  Linux TERMIOS(3)
     */

    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~(OPOST);
    options.c_cflag &= ~(CSIZE | PARENB);
    options.c_cflag |= (CS8 | CSTOPB);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    if (cfsetispeed(&options, speed) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }
    if (cfsetospeed(&options, speed) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }
    if (tcflush(fd, TCIOFLUSH) < 0) {
	close(fd);
	return(EXIT_FAILURE);
    }

    return fd;
}

/* called when a message arrives to the subscribed topic,
 */
void mqtt_cb_msg(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Received msg on topic: %s\n", msg->topic);
    if (msg->payload != NULL) {
	printf("Payload: %s\n", (char *)msg->payload);
    }
}

void mqtt_cb_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (!result) {
	mosquitto_subscribe(mosq, NULL, "tetra/#", 2);
    } else {
	fprintf(stderr, "MQTT subscribe failed\n");
    }
}

void mqtt_cb_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    printf("Subscribed (mid: %d): %d\n", mid, granted_qos[0]);
    for (int i = 1; i < qos_count; i++) {
	printf("\t %d", granted_qos[i]);
    }
}

void mqtt_cb_disconnect(struct mosquitto *mosq, void *userdat, int rc) {
    printf("MQTT disconnect, error: %d: %s\n", rc, mosquitto_strerror(rc));
}

void mqtt_cb_log(struct mosquitto *mosq, void *userdata, int level, const char *str) {
    switch (level) {
    case MOSQ_LOG_DEBUG:
	printf("DBG: %s\n", str);
	break;
    case MOSQ_LOG_INFO:
    case MOSQ_LOG_NOTICE:
	printf("INF: %s\n", str);
	break;
    case MOSQ_LOG_WARNING:
	printf("WRN: %s\n", str);
	break;
    case MOSQ_LOG_ERR:
	printf("ERR: %s\n", str);
	break;
    default:
	printf("Unknown MOSQ loglevel!");
    }
}

int main(int argc, char *argv[]) {
    int pid, opt, ret, running, keepalive;
    int baudrate;
    bool clean_session;
    struct mosquitto *mosq = NULL;

    clean_session = true;
    running = 1;
    clean_session = true;
    background = 0;
    keepalive = 5;

    while ((opt = getopt(argc, argv, "d:s:fh?")) != -1) {
	switch (opt) {
	case 'd':
	    break;
	case 's':
	    break;
	case 'b':
	    break;
	case 'f':
	    background = 1;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, clean_session, NULL);
    if (!mosq) {
	fprintf(stderr, "Error: Out of memory.\n");
	return(EXIT_FAILURE);
    }

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

    mosquitto_connect_callback_set(mosq, mqtt_cb_connect);
    mosquitto_message_callback_set(mosq, mqtt_cb_msg);
    mosquitto_subscribe_callback_set(mosq, mqtt_cb_subscribe);
    mosquitto_disconnect_callback_set(mosq, mqtt_cb_disconnect);
    mosquitto_log_callback_set(mosq, mqtt_cb_log);

    // we try until we succeed, or we killed
    while (running) {
	if (mosquitto_connect(mosq, mqtt_broker_host, mqtt_broker_port, keepalive)) {
	    printf("Unable to connect, host: %s, port: %d\n", mqtt_broker_host, mqtt_broker_port);
	    sleep(2);
	    continue;
	}
	break;
    }
    // pfd[0] is for the mosquitto socket, pfd[1] is for stdin, or any
    // other file descriptor we need to handle
    struct pollfd pfd[2];
    pfd[1].fd = 0;
    pfd[1].events = POLLIN;	//these 2 won't change, so enough to set it once
    const int nfds = sizeof(pfd) / sizeof(struct pollfd);

    while (running) {
	// this might change (when reconnecting for example)
	// so better check it always
	int mosq_fd = mosquitto_socket(mosq);
	pfd[0].fd = mosq_fd;
	pfd[0].events = POLLIN;
	// we check whether libmosquitto wants to write, and if yes, we
	// also register the POLLOUT event for poll, so it will return
	// when it's possible to write to the socket.
	if (mosquitto_want_write(mosq)) {
	    printf("Set POLLOUT\n");
	    pfd[0].events |= POLLOUT;
	}
	// we set the poll()-s timeout here to the half
	// of libmosquitto's keepalive value, to stay on the safe side
	if (poll(pfd, nfds, keepalive / 2 * 1000) < 0) {
	    printf("Poll() failed with <%s>, exiting", strerror(errno));
	    return EXIT_FAILURE;
	}
	// first checking the mosquitto socket
	// if we supposed to write:
	if (pfd[0].revents & POLLOUT) {
	    mosquitto_loop_write(mosq, 1);
	}
	// or read:
	if (pfd[0].revents & POLLIN) {
	    ret = mosquitto_loop_read(mosq, 1);
	    if (ret == MOSQ_ERR_CONN_LOST) {
		printf("reconnect...\n");
		mosquitto_reconnect(mosq);
	    }
	}
	// we call the misc() funtion in both cases
	mosquitto_loop_misc(mosq);
	// checking if there is data on the stdin, if yes, reading it
	// and publish
	if (pfd[1].revents & POLLIN) {
	    char input[64];
	    ret = read(0, input, 64);
	    if (ret < 0) {
		fprintf(stderr, "%s: read_error\n", __func__);
		exit(EXIT_FAILURE);
	    }
	    printf("STDIN: %s", input);
	    mosquitto_publish(mosq, NULL, "stdin", strlen(input), input, 0, false);
	}
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    printf("bye\n");

    return EXIT_SUCCESS;
}
