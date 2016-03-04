/*
 * Copyright 2013 - 2014 Siegfried Lohberg
 *                  2016 Gerhard Bertelsmann
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Siegfried Lohberg wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 */

/*
 * Credit: 
 * In dieses Programm flossen Ideen von Gerhard Bertelsmann 
 * und seinem can2udp Projekt ebenso wie aus dem railuino 
 * Projekt von Joerg Pleumann.
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
    int invert;
    int offset;
    uint32_t count;
    uint16_t hash;
    uint16_t hw_id;
};

void usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -vf [-b <bcast_addr/int>][-i <0|1>][-p <port>][-m <s88modules>][-o <offset>]\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
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
    getchar();
    if (value ^ s88_data->invert)
	printf("set %s high", str);
    else
	printf("set %s low", str);
    gpio_bpi_set(pin, value ^ s88_data->invert);
}

int main(int argc, char **argv) {
    int utime, i, j;
    int opt;
    int modulcount = 1;
    struct s88_t s88_data;
    uint32_t mask, s88_bit;
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

    if (gpio_bpi_open("/dev/mem") < 0) {
	fprintf(stderr, "Can't open IO mem: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    gpio_bpi_select_output(CLOCK_PIN);
    gpio_bpi_select_output(LOAD_PIN);
    gpio_bpi_select_output(RESET_PIN);
    gpio_bpi_select_input(DATA_PIN);

    printf("press entere to continue\n");
    /* loop forever */
    while (1) {
	s88_bit = 0;
	set_pin("LOAD_PIN", LOAD_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("CLOCK_PIN", CLOCK_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("CLOCK_PIN", CLOCK_PIN, &s88_data, LOW ^ s88_data.invert);
	set_pin("RESET_PIN", RESET_PIN, &s88_data, HIGH ^ s88_data.invert);
	set_pin("RESET_PIN", RESET_PIN, &s88_data, LOW ^ s88_data.invert);
	set_pin("LOAD_PIN", LOAD_PIN, &s88_data, LOW ^ s88_data.invert);
	s88_data.count++;
	/* get sensor data */
	for (i = 0; i < modulcount; i++) {
	    if ((s88_bit & 0x1f) == 0)
		mask = BIT(31);
	    for (j = 0; j < 16; j++) {
		gpio_bpi_get(DATA_PIN, &newvalue);
		if (newvalue ^= s88_data.invert)
		    printf("Pin %d high\n", j);
		else
		    printf("Pin %d low\n", j);
		set_pin("CLOCK_PIN", CLOCK_PIN, &s88_data, HIGH ^ s88_data.invert);
		set_pin("CLOCK_PIN", CLOCK_PIN, &s88_data, LOW ^ s88_data.invert);
		s88_bit++;
		mask >>= 1;
	    }
	}
    }
    return 0;
}
