/* Copyright 2016 Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <sys/time.h>

#include <sys/ioctl.h>
#include "s88udp-bpi.h"

#define BIT(x)		(1<<x)
#define MICRODELAY	15	/* clock frequency 1/MICRODELAY[us] */
#define MINDELAY	2	/* min delay in usec */
#define MAXMODULES	32	/* max numbers of S88 modules */

struct s88_t {
    int background;
    int verbose;
    uint8_t invert;
    int offset;
    uint32_t count;
    uint16_t hash;
    uint16_t hw_id;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <s88modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 0.2\n\n");
    fprintf(stderr, "         -i [0|1]            invert signals - default 0 -> not inverting\n");
    fprintf(stderr, "         -t <time in usec>   microtiming in usec - default %d usec\n", MICRODELAY);
}

void usec_sleep(int usec) {
    struct timespec to_wait;

    to_wait.tv_sec = 0;
    to_wait.tv_nsec = usec * 1000;
    nanosleep(&to_wait, NULL);
}

void set_pin(char *str, int pin, struct s88_t *s88_data, int value) {
    /* wait for next step */
    getchar();
    if (value ^ s88_data->invert)
	printf("set %s high\n", str);
    else
	printf("set %s low\n", str);
    gpio_aw_set(pin, value ^ s88_data->invert);
}

void get_data(struct s88_t *s88_data, uint8_t *newvalue) {

    gpio_aw_get(DATA_PIN, newvalue);
    if (*newvalue ^ s88_data->invert)
	printf("Data pin high\n");
    else
	printf("Data pin low\n");
}


int main(int argc, char **argv) {
    int utime;
    int opt;
    int modulcount = 1;
    struct s88_t s88_data;
    uint8_t newvalue;

    utime = MICRODELAY;

    memset(&s88_data, 0, sizeof(s88_data));

    while ((opt = getopt(argc, argv, "i:m:t:vh?")) != -1) {
	switch (opt) {
	case 'i':
	    s88_data.invert = atoi(optarg) & 1;
	    break;
	case 'm':
	    modulcount = atoi(optarg);
	    if (modulcount < 1 || modulcount > MAXMODULES) {
		usage(basename(argv[0]));
		exit(EXIT_FAILURE);
	    }
	    break;
	case 't':
	    utime = atoi(optarg);
	    if (utime < MINDELAY) {
		fprintf(stderr, "microtiming value to low: %d\n", utime);
		exit(EXIT_FAILURE);
	    }
	    break;
	case 'v':
	    s88_data.verbose = 1;
	    break;
	case 'f':
	    s88_data.background = 0;
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

    if (gpio_aw_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    gpio_aw_select_output(CLOCK_PIN);
    gpio_aw_select_output(LOAD_PIN);
    gpio_aw_select_output(RESET_PIN);
    gpio_aw_select_input(DATA_PIN);

    printf("#########################\n");
    printf("# S88 test\n\n");
    printf("#########################\n");
    printf("# Test with multimeter\n\n");
    gpio_aw_set(CLOCK_PIN, LOW);
    gpio_aw_set(LOAD_PIN, LOW);
    gpio_aw_set(RESET_PIN, LOW);
    printf("CLOCK_PIN, LOAD_PIN & RESET_PIN low   - ( U3,U4,U5 Pin2 1V4 - 1V9) - press enter to continue\n");
    getchar();
    printf("\n\n");
    gpio_aw_set(CLOCK_PIN, HIGH);
    gpio_aw_set(LOAD_PIN, HIGH);
    gpio_aw_set(RESET_PIN, HIGH);
    printf("CLOCK_PIN, LOAD_PIN & RESET_PIN high  - ( U3,U4,U5 Pin2 3V3)  - press enter to continue\n");
    getchar();

    printf("#########################\n");
    printf("# Test without multimeter\n\n");

    printf("connect RESET pin to DATA pin on S88 bus - press enter to continue\n");
    set_pin("RESET pin", RESET_PIN, &s88_data, LOW ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    set_pin("RESET pin", RESET_PIN, &s88_data, HIGH ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    printf("\n\n");

    printf("connect LOAD pin to DATA pin on S88 bus - press enter to continue\n");
    set_pin("LOAD pin", LOAD_PIN, &s88_data, LOW ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    set_pin("LOAD pin", LOAD_PIN, &s88_data, HIGH ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    printf("\n\n");

    printf("connect CLOCK pin to DATA pin on S88 bus - press enter to continue\n");
    set_pin("CLOCK pin", CLOCK_PIN, &s88_data, LOW ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    set_pin("CLOCK pin", CLOCK_PIN, &s88_data, HIGH ^ s88_data.invert);
    get_data(&s88_data, &newvalue);
    printf("\n\n");

#if 0
    printf("S88 Step by step");
    while (1) {
	s88_bit = 0;
	set_pin("LOAD pin", LOAD_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("CLOCK pin", CLOCK_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("CLOCK pin", CLOCK_PIN, &s88_data, LOW ^ s88_data.invert);
	set_pin("RESET pin", RESET_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("RESET pin", RESET_PIN, &s88_data, LOW ^ s88_data.invert);
	set_pin("LOAD pin", LOAD_PIN, &s88_data, LOW ^ s88_data.invert);
	s88_data.count++;
	/* get sensor data */
	for (i = 0; i < modulcount; i++) {
	    if ((s88_bit & 0x1f) == 0)
		mask = BIT(31);
	    for (j = 0; j < 16; j++) {
		gpio_aw_get(DATA_PIN, &newvalue);
		if (newvalue ^= s88_data.invert)
		    printf("Pin %d high\n", j);
		else
		    printf("Pin %d low\n", j);
		set_pin("CLOCK pin", CLOCK_PIN, &s88_data, HIGH ^ s88_data.invert);
		set_pin("CLOCK pin", CLOCK_PIN, &s88_data, LOW ^ s88_data.invert);
		s88_bit++;
		mask >>= 1;
	    }
	}
    }
#endif
    return 0;
}
