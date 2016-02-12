/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * 2016 Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
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
#include "dcc.h"

int susi_fd;

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s ", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -c                  reading CV\n");
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

int read_cv(int cv, int *data) {
    int i, ret;
    struct susi_command susi;

    susi.length = 3;
    susi.data[0]  = DCC_DIRECT_CV | DIRECT_CV_BITM;
    susi.data[0] |= (cv >> 8) & DIRECT_ADDRESS_MASK;
    susi.data[1]  = cv & 0xff;

    *data = 0;
    /* we are parsing all bits */
    for (i = 0; i < 8; i++) {
	*data >>= 1;
	/*  1 1 1 K D B B B
	 *  K=0: verify bit
	 *  K=1: write bit
	 *  D: bit value to verify/write
	 *  BBB bit # 0 ..7 to verify/write
	 */
	susi.data[2] = 0xe8 | i;

	ret = ioctl(susi_fd, SUSI_COMMAND_ACK, &susi);
	if (susi.ack)
	    *data |= 0x80;
    }
    return ret;
}

int main(int argc, char **argv) {
    int opt, cv, data;
    struct susi_command susi;

    while ((opt = getopt(argc, argv, "c:sh?")) != -1) {
	switch (opt) {
	case 'c':
	    cv = atoi(optarg) & 0x3ff;
	    break;
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

    susi_fd = open("/dev/susi-gpio", O_RDWR);
    if (susi_fd < 0) {
	fprintf(stderr, "%s: error: open failed [%s]\n", __func__, strerror(errno));
	exit(-1);
    }

    read_cv(cv, &data);
    printf("CV %d = 0x%02x\n", cv, data);

    close(susi_fd);
    return 0;
}
