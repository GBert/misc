/*
 * Copyright (C) 2012 Zilvinas Valinskas
 * See LICENSE for more information.
 *
 * mod
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

#define BIT(x) (1UL << (x))
#define BIT_MASK(a, b) (((unsigned) -1 >> (31 - (b))) & ~((1U << (a)) - 1))

#define RT5350_I2S_CFG			0x0a00
#define RT5350_I2S_INT_STATUS		0x0a04
#define RT5350_I2S_TXRX_FIFO_STATUS	0x0a0C
#define RT5350_I2S_TX_FIFO_WREG		0x0a10
#define RT5350_I2S_RX_FIFO_WREG		0x0a14

#define RT5350_I2S_DIVCOM_CFG		0x0a20
#define RT5350_I2S_DIVINT_CFG		0x0a24

#define I2S_CFG_I2S_EN		BIT(31)
#define I2S_DIVCOMP_CLK_EN	BIT(31)
#define I2S_DIVCOMP_MASK	BIT_MASK(0,8)
#define I2S_DIVINT_MASK		BIT_MASK(0,9)

#define MSECS                   1000ULL
#define USECS                   1000000ULL
#define NSECS                   1000000000ULL

static void inline timespec_from_ms(struct timespec *ts, unsigned long ms) {
    ts->tv_sec = ms / MSECS;
    ts->tv_nsec = (ms % MSECS) * USECS;
}

static void inline i2s_start(struct mmio *io) {
    mmio_writel(io, RT5350_I2S_CFG, mmio_readl(io, RT5350_I2S_CFG) | I2S_CFG_I2S_EN);
}

static void inline i2s_set_divider(struct mmio *io, unsigned long period) {
    period = (period * 20) & I2S_DIVINT_MASK;
    mmio_writel(io, RT5350_I2S_DIVINT_CFG, period);
}

int main(int argc, char *argv[]) {
    char chip[16];
    uint32_t period;
    uint32_t magic1, magic2;
    struct mmio io;

    if (argc > 2) die("too many arguments.");
    if (argc == 2)
        period = atoi(argv[1]);
    else
        period = 10;

    if (mmio_map(&io, 0x10000000, 0x1000))
	die_errno("mmio_map() failed");

    memset(chip, 0, sizeof(chip));

    magic1 = mmio_readl(&io, 0x0);
    memcpy(chip + 0, &magic1, sizeof(magic1));

    magic2 = mmio_readl(&io, 0x4);
    memcpy(chip + 4, &magic2, sizeof(magic2));

    if (strncmp(chip, "RT5350  ", 8) != 0) die("unsupported chip: %08X/%08X", magic1, magic2);

    i2s_set_divider(&io, 10);
    i2s_start(&io);
    printf("i2s started ...\n");

    mmio_unmap(&io);

    return 0;
}
