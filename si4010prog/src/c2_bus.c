/**
 * c2_bus.c - C2 Bus Master interface functions
 *
 * Copyright (c) 2014, David Imhoff <dimhoff.devel@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors may
 *       be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//#define C2_DEBUG

#include "config.h"

#ifdef C2_DEBUG
# include <stdio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "c2_bus.h"
#include "c2_bus_module.h"

// implementations
#include "c2_bus_c2drv.h"
#include "c2_bus_fx2.h"
#include "c2_bus_ftdi.h"


void c2_bus_set_error(struct c2_bus *bus, const char *msg)
{
	if (bus->error_msg != NULL) {
		free(bus->error_msg);
	}
	bus->error_msg = strdup(msg);
}

void c2_bus_set_perror(struct c2_bus *bus, const char *msg)
{
	int err = errno;

	if (bus->error_msg != NULL) {
		free(bus->error_msg);
	}
//TODO: append strerror message
	bus->error_msg = strdup(msg);
}

const char *c2_bus_get_error(struct c2_bus *bus)
{
	return bus->error_msg;
}

int c2_bus_open(struct c2_bus *bus, const char *type, const char *path)
{
	memset(bus, 0, sizeof(struct c2_bus));

#ifdef WITH_C2_BUS_FX2
	if (strcmp(type, "fx2") == 0) {
		return c2_bus_fx2_init(bus, path);
	}
#endif // WITH_C2_BUS_FX2
#ifdef WITH_C2_BUS_FT232
	if (strcmp(type, "ft232") == 0) {
		return c2_bus_ftdi_init(bus, path);
	}
#endif // WITH_C2_BUS_FT232
#ifdef WITH_C2_BUS_C2DRV
	if (strcmp(type, "c2drv") == 0) {
		return c2_bus_c2drv_init(bus, path);
	}
#endif // WITH_C2_BUS_C2DRV

	c2_bus_set_error(bus, "Unknown C2 bus type");
	return 1;
}

int c2_bus_reset(struct c2_bus *bus)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s\n", __func__);
#endif
	return bus->ops->reset(bus);
}

int c2_bus_qreset(struct c2_bus *bus)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s\n", __func__);
#endif
	return bus->ops->qreset(bus);
}

int c2_bus_addr_read(struct c2_bus *bus, uint8_t *addr)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s\n", __func__);
#endif
	return bus->ops->addr_read(bus, addr);
}

int c2_bus_addr_write(struct c2_bus *bus, uint8_t addr)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s %.2x\n", __func__, addr);
#endif
	return bus->ops->addr_write(bus, addr);
}

int c2_bus_data_read(struct c2_bus *bus, uint8_t *data, size_t len)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s %d\n", __func__, len);
#endif
	return bus->ops->data_read(bus, data, len);
}

int c2_bus_data_write(struct c2_bus *bus, const uint8_t *data, size_t len)
{
#ifdef C2_DEBUG
	fprintf(stderr, "%s %d %.2x\n", __func__, len, data[0]);
#endif
	return bus->ops->data_write(bus, data, len);
}

void c2_bus_destroy(struct c2_bus *bus)
{
	if (bus->error_msg)
		free(bus->error_msg);
	if (bus->ctx != NULL)
		bus->ops->destruct(bus);
}
