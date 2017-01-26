#include "mqtt_gpio.h"

char topic[MAX_BUFFER];

static int run = 1;
/* testing ... */
static int background = 0;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b <MQTT broker> -t <topic>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -b <MQTT broker>    Mosquitto broker - default localhost\n");
    fprintf(stderr, "         -t <topic>          topic - default gpio\n");
    fprintf(stderr, "         -f                  foreground\n\n");
}

void handle_signal(int signal) {
    run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result) {
    printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    bool match = 0;

    printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char *)message->payload, message->topic);

    mosquitto_topic_matches_sub("/gpio/+", message->topic, &match);
    if (match) {
	printf("got message for GPIO topic\n");
    }
}

int main(int argc, char *argv[]) {
    char clientid[24];
    struct mosquitto *mosq;
    int pid, opt, rc;
    char broker[MAX_BUFFER];

    rc = 0;

    while ((opt = getopt(argc, argv, "b:d:s:t:fh?")) != -1) {
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

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    mosquitto_lib_init();

    memset(clientid, 0, 24);
    mosq = mosquitto_new(clientid, true, 0);

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

    if (mosq) {
	mosquitto_connect_callback_set(mosq, connect_callback);
	mosquitto_message_callback_set(mosq, message_callback);

	rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

	mosquitto_subscribe(mosq, NULL, "/gpio/+", 0);

	while (run) {
	    rc = mosquitto_loop(mosq, -1, 1);
	    if (run && rc) {
		printf("connection error!\n");
		sleep(10);
		mosquitto_reconnect(mosq);
	    }
	}
	mosquitto_destroy(mosq);
    }

    mosquitto_lib_cleanup();
    return rc;
}
