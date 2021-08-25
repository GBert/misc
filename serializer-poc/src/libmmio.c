/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "mmio.h"
#include "utils.h"

static inline uint32_t readl(void *ptr)
{
	uint32_t *data = ptr;
	return *data;
}

static inline void writel(uint32_t value, void *ptr)
{
	uint32_t *data = ptr;
	*data = value;
}

uint32_t mmio_readl(const struct mmio *io, unsigned int offset)
{
	void *addr;

	addr = io->iomem + io->offset + offset;
	return readl(addr);
}

void mmio_writel(const struct mmio *io, unsigned int offset, uint32_t value)
{
	void *addr;

	addr = io->iomem + io->offset + offset;
	writel(value, addr);
}

static void mmio_normalize(struct mmio *mo)
{
	int npages = 0;

	mo->iobase += mo->offset;
	mo->offset = mo->iobase & (getpagesize() - 1);
	mo->iobase = mo->iobase & ~(getpagesize() - 1);

	npages += (mo->range * sizeof(uint32_t)) / getpagesize();
	npages += 1;
	mo->iosize = npages * getpagesize();
}

static void mmio_init(struct mmio *mo)
{
	char *device;
	int iofd;

	if (!mo->kmem)
		device = "/dev/mem";
	else
		device = "/dev/kmem";

	iofd = open(device, O_RDWR);
	if (iofd < 0)
		die_errno("open() failed");

	mo->iomem = mmap(NULL, mo->iosize,
			 PROT_READ|PROT_WRITE,
			 MAP_SHARED,
			 iofd, mo->iobase);

	if (mo->iomem == MAP_FAILED)
		die_errno("can't map @ %0lX", mo->iobase);

	close(iofd);
}

int mmio_map(struct mmio *io, unsigned long base, size_t length)
{
	memset(io, 0, sizeof(*io));

	io->iobase = base;
	io->offset = 0;
	io->range  = length;

	mmio_normalize(io);
	mmio_init(io);

	return 0;
}

void mmio_unmap(struct mmio *io)
{
	if (munmap(io->iomem, io->iosize))
		die_errno("can't unmap @ %lX", io->iobase);

	memset(io, 0, sizeof(*io));
}

void mmio_hexdump(const struct mmio *io, size_t length, size_t flags)
{
	__hexdump(io->iobase + io->offset,
		  io->iomem + io->offset,
		  length, flags);
}
