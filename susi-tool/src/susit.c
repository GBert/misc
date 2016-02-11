/*
 * Copyright 2016 Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#include <errno.h>
#include <fcntl.h>
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

#include "susi-gpio.h"

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s ", prg);
    fprintf(stderr, "   Version 0.1\n\n");
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
    /* char *timestamp = (char *)malloc(sizeof(char) * 16); */
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    printf("%02d:%02d:%02d.%06d ", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec);
    return 0;
}

int main(int argc, char **argv) {
    int fd, opt;
    struct susi_command susi;

    while ((opt = getopt(argc, argv, "h?")) != -1) {
	switch (opt) {
	case 'h':
	case '?':
	    usage(basename(argv[0]));
	    exit(0);
	default:
	    usage(basename(argv[0]));
	    exit(1);
	}
    }

    memset(&susi, 0, sizeof(susi));

    fd = open("/dev/c2tool-gpio", O_RDWR);
    if (fd < 0) {
	fprintf(stderr, "%s: error: open failed [%s]\n", __func__, strerror(errno));
	exit(-1);
    }
    return 0;
}
