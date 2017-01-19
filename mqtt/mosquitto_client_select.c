/*  file mosquitto_client_select.c
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
#include <sys/select.h>

#include <mosquitto.h>
/* compile:
 * cc -o mosquitto-selecttest  mosquitto_client_select.c  -lmosquitto
 *
 * small example code to experiment with poll-based external loop and
 * libmosquitto
 *
 * send event with:
 * mosquitto_pub -t topic/subtopic -m "ahoi" -q 1
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

int main(int argc, char *argv[]) {

    //set up mosquitto
    int keepalive = 10;
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

    while (running) {		//we try until we succeed, or we killed
	if (mosquitto_connect(mosq, mqtt_broker_host, mqtt_broker_port, keepalive)) {
	    printf("Unable to connect, host: %s, port: %d\n", mqtt_broker_host, mqtt_broker_port);
	    sleep(2);
	    continue;
	}
	break;
    }
//    if (running){
//        mosquitto_loop_forever(mosq, -1, 1);
//    }
    int mosq_fd = mosquitto_socket(mosq);
    printf("Connected, socketfd: %d\n", mosq_fd);
    fd_set fd_set;
    struct timeval tv;

    while (1) {
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&fd_set);
	FD_SET(mosq_fd, &fd_set);
//        printf("-\n");

//with this select() always returns immediately:
//       int rc = select (mosq_fd+1, &fd_set,&fd_set, NULL, &tv);

//with this most likely the client won't have a chance to write, so
//it gets into disconnected status at the first PINGREQ
	int rc = select(mosq_fd + 1, &fd_set, NULL, NULL, &tv);

	if (rc < 0) {
	    perror("select");
	    exit(EXIT_FAILURE);
	} else if (rc == 0) {
	    printf("timeout\n");
	    continue;
	} else if (FD_ISSET(mosq_fd, &fd_set)) {
//            printf("mosquitto event\n");
	    int ret = mosquitto_loop_read(mosq, 1);
	    if (ret == MOSQ_ERR_CONN_LOST) {
		/* We've been disconnected from the server */
		printf("Reconnect...\n");
		mosquitto_reconnect(mosq);
	    }
	    if (mosquitto_want_write(mosq)) {
		mosquitto_loop_write(mosq, 8);
	    }
	    mosquitto_loop_misc(mosq);
	    mosq_fd = mosquitto_socket(mosq);
//            printf("socketfd: %d\n",mosq_fd);
	} else {
	    printf("FD wasn't set!\n");
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
    //calling the URL with the payload
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
