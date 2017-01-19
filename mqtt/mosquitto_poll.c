/*  file main.c
 *  author: Zoltan Gyarmati <mr.zoltan.gyarmati@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
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
/* compile:
 * cc -o mosquitto-polltest  main.c  -lmosquitto
 *
 * small example code to experiment with poll-based external loop and
 * libmosquitto, here we set up libmosquitto, subscribing to a topic,
 * and with a poll() call we watch both the libmosquitto socket file
 * descriptor and the stdin file descriptor. If a message arrives to the
 * subscribed topic, we simply log it, at if a line comes from the
 * terminal via stdin, we publish it via libmosquitto to the topic
 * 'stdin'
 *
 * send event with:
 * mosquitto_pub -t topic/subtopic -m "ahoi" -q 1
 *
 * listening to the published event (the msg from stdin) with
 * mosquitto_sub -t stdin
 */

/*
 * change this if you need remote broker
 */
const char *mqtt_broker_host = "localhost";
const int mqtt_broker_port = 1883;

// forward declarations
void mqtt_cb_msg(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg);
void mqtt_cb_connect(struct mosquitto *mosq, void *userdata, int result);

void mqtt_cb_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos);
void mqtt_cb_disconnect(struct mosquitto *mosq, void *userdat, int rc);

void mqtt_cb_log(struct mosquitto *mosq, void *userdata, int level, const char *str);

int main(int argc, char *argv[]) {

    //set up mosquitto
    int keepalive = 3;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;
    int running = 1;

    mosquitto_lib_init();
    mosq = mosquitto_new(NULL, clean_session, NULL);
    if (!mosq) {
	fprintf(stderr, "Error: Out of memory.\n");
	return -1;
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
	    int ret = mosquitto_loop_read(mosq, 1);
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
	    read(0, input, 64);
	    printf("STDIN: %s", input);
	    mosquitto_publish(mosq, NULL, "stdin", strlen(input), input, 0, false);
	}
    }

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    printf("bye\n");

    return EXIT_SUCCESS;
}

/* Called when a message arrives to the subscribed topic,
 */
void mqtt_cb_msg(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Received msg on topic: %s\n", msg->topic);
    if (msg->payload != NULL) {
	printf("Payload: %s\n", (char *)msg->payload);
    }
}

void mqtt_cb_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (!result) {
	mosquitto_subscribe(mosq, NULL, "topic/#", 2);
    } else {
	printf("MQTT subscribe failed\n");
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
