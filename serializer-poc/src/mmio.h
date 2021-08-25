/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#ifndef __MMIO_H__
#define __MMIO_H__

#include <sys/types.h>
#include <stdint.h>

struct mmio {
	unsigned long 	iobase;		/* getpagesize() aligned, see mmap(2) */
	unsigned long	offset;		/* additional offset from iobase */
	unsigned long	range;		/* N * uint32_t read/write ops. */

	void		*iomem;
	size_t		iosize;
	int		kmem;		/* 0 - /dev/mem, 1 - /dev/kmem */
};

int  mmio_map(struct mmio *io, unsigned long base, size_t length);
void mmio_unmap(struct mmio *io);
void mmio_hexdump(const struct mmio *io, size_t length, size_t flags);

uint32_t mmio_readl(const struct mmio *io, unsigned int offset);
void mmio_writel(const struct mmio *io, unsigned int offset, uint32_t value);

#endif //__MMIO_H__
