/*
 *  Silicon Labs C2 port Linux support for c2tool
 *
 *  Copyright (c) 2015 Gerhard Bertelsmann <info@gerhard-bertelsmann.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation
 */

#ifndef _SUSI_GPIO_H
#define _SUSI_GPIO_H

#define MAX_DATA	4

struct susi_command {
	uint8_t length;
	uint8_t ack;
	uint8_t data[MAX_DATA];
};

#define SUSI_GPIO_MAJOR (181)
#define SUSI_COMMAND		_IOWR(SUSI_GPIO_MAJOR, 100, struct susi_command *)
#define SUSI_COMMAND_ACK	_IOWR(SUSI_GPIO_MAJOR, 101, struct susi_command *)

#ifndef __KERNEL__

#endif

#endif /* _SUSI_GPIO_H */
