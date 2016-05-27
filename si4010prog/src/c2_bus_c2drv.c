/**
 * c2_bus_c2drv.c - SiLabs C2 master bus interface using Linux kernel module
 *
 * Based on code written by Wojciech M. Zabolotny (wzab@ise.pw.edu.pl)
 * and is published under the GPL (Gnu Public License) version 2.0 or later
 */
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#include "c2_bus.h"
#include "c2_bus_module.h"
#include "c2_ioctls.h"

struct c2_bus_c2drv_ctx {
	int fd;
};

int c2_c2drv_reset(struct c2_bus *bus);
int c2_c2drv_qreset(struct c2_bus *bus);
int c2_c2drv_addr_read(struct c2_bus *bus, unsigned char *addr);
int c2_c2drv_addr_write(struct c2_bus *bus, unsigned char addr);
int c2_c2drv_data_read(struct c2_bus *bus, unsigned char *data, size_t len);
int c2_c2drv_data_write(struct c2_bus *bus,
			const unsigned char *data, size_t len);
void c2_c2drv_destroy(struct c2_bus *bus);

struct c2_bus_ops c2_bus_c2drv_ops = {
	.reset = c2_c2drv_reset,
	.qreset = c2_c2drv_qreset,
	.addr_read = c2_c2drv_addr_read,
	.addr_write = c2_c2drv_addr_write,
	.data_read = c2_c2drv_data_read,
	.data_write = c2_c2drv_data_write,
	.destruct = c2_c2drv_destroy,
};

int c2_bus_c2drv_init(struct c2_bus *bus, const char *path)
{
	struct c2_bus_c2drv_ctx *hw;

	hw = (struct c2_bus_c2drv_ctx *) malloc(sizeof(struct c2_bus_c2drv_ctx));
	if (hw == NULL) {
		c2_bus_set_error(bus, "Unable to allocate memory");
		return -1;
	}

	if (path[0] == '\0') {
		path = "/dev/c2_bus";
	}

	hw->fd = open(path, O_RDWR);
	if (hw->fd < 0) {
		c2_bus_set_perror(bus, "Unable to open device file");
		free(hw);
		return -1;
	}
	
	bus->ops = &c2_bus_c2drv_ops;
	bus->ctx = hw;

	return 0;
}

int c2_c2drv_reset(struct c2_bus *bus)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	ioctl(hw->fd, C2_IOCRESET, NULL);
	usleep(2);

	return 0;
}

int c2_c2drv_qreset(struct c2_bus *bus)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	ioctl(hw->fd, C2_IOCQRESET, NULL);    // stop execution on the MCU

	return 0;
}

int c2_c2drv_addr_read(struct c2_bus *bus, unsigned char *addr)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	ioctl(hw->fd, C2_IOCAREAD, addr);

	return 0;
}

int c2_c2drv_addr_write(struct c2_bus *bus, unsigned char addr)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	ioctl(hw->fd, C2_IOCAWRITE, &addr);

	return 0;
}

int c2_c2drv_data_read(struct c2_bus *bus, unsigned char *data, size_t len)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;
	unsigned long v = 0;

	assert(len == 1);
	ioctl(hw->fd, C2_IOCD1READ, &v);
	*data = v;

	return 0;
}

int c2_c2drv_data_write(struct c2_bus *bus, const unsigned char *data, size_t len)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	assert(len == 1);
	ioctl(hw->fd, C2_IOCD1WRITE, data);

	return 0;
}

void c2_c2drv_destroy(struct c2_bus *bus)
{
	struct c2_bus_c2drv_ctx *hw = (struct c2_bus_c2drv_ctx *) bus->ctx;

	if (hw == NULL) return;

	if (hw->fd != -1) {
		close(hw->fd);
	}
	free(hw);
}
