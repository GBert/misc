#include <libgen.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <termios.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#define MAX_BUFFER	256

const char *mqtt_broker_host = "localhost";
const int mqtt_broker_port = 1883;

struct pollfd pfd[2];

char topic[MAX_BUFFER];
char topic_in[MAX_BUFFER];
const char crlf[] = { 0x0d, 0x0a, 0x00 };

int background;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b <mosquitto_ip>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -b <MQTT broker>    Mosquitto broker - default localhost\n");
    fprintf(stderr, "         -f                  foreground\n\n");
}

/* called when a message arrives to the subscribed topic,
 */
void mqtt_cb_msg(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Received msg on topic: %s\n", msg->topic);
    if (msg->payload != NULL) {
	if (!background)
	    printf("Payload: %s\n", (char *)msg->payload);
	dprintf(pfd[1].fd, "%s\r\n", (char *)msg->payload);
    }
}

void mqtt_cb_connect(struct mosquitto *mosq, void *userdata, int result) {
    /* TODO: length */
    char subscribe[MAX_BUFFER];

    snprintf(subscribe, MAX_BUFFER - 4, "%s/out", topic);
    if (!result) {
	mosquitto_subscribe(mosq, NULL, subscribe, 2);
    } else {
	fprintf(stderr, "MQTT subscribe failed\n");
    }
}

void mqtt_cb_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    printf("Subscribed (mid: %d): %d\n", mid, granted_qos[0]);
    for (int i = 1; i < qos_count; i++) {
	if (!background)
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
    bool clean_session;
    struct mosquitto *mosq = NULL;
    char broker[MAX_BUFFER];

    clean_session = true;
    running = 1;
    clean_session = true;
    background = 0;
    keepalive = 5;

    memset(topic, 0, sizeof(topic));
    memset(broker, 0, sizeof(broker));
    memcpy(broker, mqtt_broker_host, strlen(mqtt_broker_host));

    while ((opt = getopt(argc, argv, "b:t:fh?")) != -1) {
	switch (opt) {
	case 'b':
	    strncpy(broker, optarg, sizeof(broker) - 1);
	    break;
	case 't':
	    strncpy(topic, optarg, sizeof(topic) - 1);
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
	return (EXIT_FAILURE);
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

    snprintf(topic_in, MAX_BUFFER - 3, "%s/in", topic);

    printf("open serial device fd : %d\n", pfd[1].fd);

    mosquitto_connect_callback_set(mosq, mqtt_cb_connect);
    mosquitto_message_callback_set(mosq, mqtt_cb_msg);
    mosquitto_subscribe_callback_set(mosq, mqtt_cb_subscribe);
    mosquitto_disconnect_callback_set(mosq, mqtt_cb_disconnect);
    if (!background)
	mosquitto_log_callback_set(mosq, mqtt_cb_log);

    // we try until we succeed, or we killed
    while (running) {
	if (mosquitto_connect(mosq, broker, mqtt_broker_port, keepalive)) {
	    printf("Unable to connect, host: %s, port: %d\n", broker, mqtt_broker_port);
	    sleep(2);
	    continue;
	}
	break;
    }
    // pfd[0] is for the mosquitto socket, pfd[1] is for UART, or any
    // other file descriptor we need to handle
    // pfd[1].fd = 0;
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
	// checking if there is data on the UART, if yes, reading it
	// and publish
	if (pfd[1].revents & POLLIN) {
	    char input[64];
	    memset(input, 0, sizeof(input));
	    ret = read(pfd[1].fd, input, 64);
	    if (ret < 0) {
		fprintf(stderr, "%s: read_error\n", __func__);
		exit(EXIT_FAILURE);
	    }
	    mosquitto_publish(mosq, NULL, topic_in, strlen(input), input, 0, false);
	}
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    printf("bye\n");

    return EXIT_SUCCESS;
}
