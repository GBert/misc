/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#include "mmio.h"

#define MSECS                   1000ULL
#define USECS                   1000000ULL
#define NSECS                   1000000000ULL

static void inline
timespec_from_ms(struct timespec *ts, unsigned long ms)
{
        ts->tv_sec = ms / MSECS;
        ts->tv_nsec = (ms % MSECS) * USECS;
}

int main(int argc, char *argv[])
{
	char chip[16];
	int period;
	uint32_t magic1, magic2;
	struct mmio io;

	if (argc > 2)
		die("too many arguments.");
	if (argc == 2)
		period = atoi(argv[1]) * 1000;
	else
		period = 1000;

	if (mmio_map(&io, 0x10000000, 0x1000))
		die_errno("mmio_map() failed");

	memset(chip, 0, sizeof(chip));

	magic1 = mmio_readl(&io, 0x0);
	memcpy(chip + 0, &magic1, sizeof(magic1));

	magic2 = mmio_readl(&io, 0x4);
	memcpy(chip + 4, &magic2, sizeof(magic2));

	if (strncmp(chip, "RT3050  ", 8) != 0 &&
	    strncmp(chip, "RT3052  ", 8) != 0 &&
	    strncmp(chip, "RT3350  ", 8) != 0 &&
	    strncmp(chip, "RT3352  ", 8) != 0)
		die("unknown chip: %08X/%08X", magic1, magic2);

	mmio_unmap(&io);

	if (mmio_map(&io, 0x10180000, 0x4000))
		die_errno("mmio_map() failed");

#define RTS_SUCCESS(x) ((x) & 0xFFFF)
#define RTS_FAILURE(x) ((x) >> 16)

	mmio_readl(&io, 0x1744);	/* clear initial value */
	while (1) {
		struct timespec ts;
		uint32_t v1, v2;
		uint16_t success, failure;

		timespec_from_ms(&ts, period);

		v1 = mmio_readl(&io, 0x1744);
		nanosleep(&ts, NULL);
		v2 = mmio_readl(&io, 0x1744);

		success = RTS_SUCCESS(v2) - RTS_SUCCESS(v1);
		failure = RTS_FAILURE(v2) - RTS_FAILURE(v1);

		printf("RTS/CTS success: %hu, failure: %hu\n",
		       success, failure);
	}

	printf("RTS/CTS: %08X, %08X\n",
	       mmio_readl(&io, 0x1744),
	       mmio_readl(&io, 0x1744));

	mmio_unmap(&io);
	return 0;
}
