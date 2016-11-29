/*
 * gpio-poll.c - demonstrate catching a GPIO change event without polling
 *               ironically using the poll(2) system call
 *
 * Author: Tim Harvey <tharvey@gateworks.com>
 */

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

/* seconds to block waiting for edge
 *  - this also defines the interval between calls if button is held down
 */
#define BLOCK_TIME 5
/* seconds before press-and-release count is cleared */
#define MIN_REST_TIME 2

/* called when a gpio changes value
 * @value - 0 is low, 1 is high
 *
 * The GSC pushbutton has an internal pullup, thus a 'button' should be
 * normally open, and when pressed should short to ground. Therefore a 0
 * is 'down', and a 1 is 'up'
 */
time_t held_time = 0;		/* num seconds button held down */
time_t last = 0;		/* last event time */
int num_releases = 0;		/* number of successive press-and-release events */

void gpio_change(int gpio, int value) {
    time_t now = time(NULL);

    printf("gpio%d: %d last=%ld\n", gpio, value, (long)(now - last));
    if (!value) {		/* down */
	long held;
	if (!held_time)
	    held_time = now;

	held = (long)(now - held_time);
	printf("held=%ld\n", held);

	/* if held for more than 5 seconds */
	if (held >= 5) {
	    printf("Powering system down\n");
	    while (1) ;
	}
    } else {			/* up */
	/* cancel held time counter */
	held_time = 0;
	/* increment or reset num_releases */
	if (now - last < MIN_REST_TIME) {
	    num_releases++;
	    printf("count=%d\n", num_releases);

	    /* perform something on number of presses? */
	} else {
	    printf("reset count\n");
	    num_releases = 0;
	}

	last = time(NULL);
    }
}

int main(int argc, char **argv) {
    struct pollfd fdset;
    char path[256];
    char buf[32];
    int gpio;
    int fd, rz, c;

    if (argc < 3) {
	fprintf(stderr, "usage: %s <gpio> <rising|falling|both>\n", argv[0]);
	exit(-1);
    }

    gpio = atoi(argv[1]);

    /* configure gpio trigger:
     *   edge trigger of 'both' (falling and rising) allows catching
     *   changes in both directions vs level triggerd.
     */
    sprintf(path, "/sys/class/gpio/gpio%d/edge", gpio);
    if ((fd = open(path, O_WRONLY)) < 0) {
	perror("open() failed\n");
	fprintf(stderr, "non-exported or non-input gpio: %s\n", path);
	exit(-1);
    }
    write(fd, argv[2], strlen(argv[2]));
    close(fd);

    /* open gpio sysfs value for reading in blocking mode */
    sprintf(path, "/sys/class/gpio/gpio%d/value", gpio);
    if ((fd = open(path, O_RDONLY | O_NONBLOCK)) < 0) {
	perror("open() failed\n");
	fprintf(stderr, "invalid or non-exported gpio: %s\n", path);
	exit(-1);
    }

    printf("monitoring %s for interrupt using poll()\n", path);
    while (1) {
	/* use poll(2) to block for 10s or until interrupt occurs */
	fdset.fd = fd;
	fdset.events = POLLPRI;
	fdset.revents = 0;
	if ((c = poll(&fdset, 1, 10000)) < 0) {
	    perror("poll() failed");
	    break;
	}

	if (fdset.revents & POLLPRI) {
	    /* show gpio value */
	    /* Note that both an lseek and read are necessary
	     * in order to clear the interrupt detected by poll
	     */
	    lseek(fdset.fd, 0, SEEK_SET);
	    rz = read(fdset.fd, buf, sizeof(buf));
	    buf[rz ? rz - 1 : 0] = 0;
	    gpio_change(gpio, atoi(buf));
	}
    }
    close(fd);

    return 0;
}
