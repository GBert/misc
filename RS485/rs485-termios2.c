/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <asm/termbits.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#define XPN_SPEED	62500
char *interface = "/dev/ttyUSB2";

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -i <interface>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -i <interface> - default %s\n", interface);
}

int time_stamp(char *timestamp) {
    struct timeval tv;
    struct tm *tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);

    sprintf(timestamp, "%02d:%02d:%02d.%03d", tm->tm_hour, tm->tm_min, tm->tm_sec, (int)tv.tv_usec / 1000);
    return 0;
}

int xpn_send(int fd, struct termios2 *config, unsigned char *data, int length) {
    int ret;
    int mask;

    /* use parity mark for address */
    /* check if we need to change */
    mask = PARENB | CMSPAR | PARODD;
    if ((config->c_cflag & mask) != mask) {
	config->c_cflag |= PARENB | CMSPAR | PARODD;
	// ioctl(fd, TCSANOW, config);
	ioctl(fd, TCSETS2, config);
    }
    ret = write(fd, data, 1);
    if (ret < 0) {
	fprintf(stderr, "can't write address - %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    if (length == 1)
	return EXIT_SUCCESS;

    /* use parity space for data */
    config->c_cflag &= ~PARODD;
    // ioctl(fd, TCSANOW, config);
    ioctl(fd, TCSETS2, config);
    ret = write(fd, data + 1, length - 1);
    if (ret < 0) {
	fprintf(stderr, "can't write data - %s\n", strerror(errno));
	return EXIT_FAILURE;
    }
    //ioctl(fd, TCSANOW, config);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    int fd, opt, ret;

    unsigned char data[] = { 0x40, 0x00 };

    struct termios2 config;

    memset(&config, 0, sizeof(struct termios2));

    while ((opt = getopt(argc, argv, "i:h?")) != -1) {
	switch (opt) {
	case 'i':
	    interface = optarg;
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

    fd = open(interface, O_RDWR);
    if (fd < 0) {
	fprintf(stderr, "Failed to open %s - %s\n", interface, strerror(errno));
	exit(EXIT_FAILURE);
    }
    // config.c_cflag &= ~CBAUD;
    config.c_cflag |= BOTHER | CS8 | PARENB | CMSPAR | PARODD;
    config.c_ispeed = XPN_SPEED;
    config.c_ospeed = XPN_SPEED;
    ret = ioctl(fd, TCSETS2, &config);
    if (ret < 0) {
	fprintf(stderr, "can't set speed - %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }
    xpn_send(fd, &config, data, 1);

    return 0;
}
