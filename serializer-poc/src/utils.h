/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/types.h>
#include "compiler.h"

/**
 * Dumps memory block as hex/ascii to stdout like hexdump(1)
 * utility.
 *
 * @arg data pointer
 * @arg data length in bytes
 * @arg number of columns and flags (0 - means default).
 */
#define HEXDUMP_32BIT		0x8000
#define HEXDUMP_BIN 		0x10000
#define HEXDUMP_16BIT		0x4000
#define HEXDUMP_8BIT		0x2000
#define HEXDUMP_ASCII		0x1000
#define HEXDUMP_SIZEMASK	(HEXDUMP_32BIT|HEXDUMP_16BIT|HEXDUMP_8BIT|HEXDUMP_BIN)
#define HEXDUMP_FLAGMASK	(HEXDUMP_ASCII|HEXDUMP_32BIT|HEXDUMP_16BIT|HEXDUMP_8BIT|HEXDUMP_BIN)
#define HEXDUMP_ISSET(__x, __f)	(((__x) & HEXDUMP_SIZEMASK) == (__f))

#define HEXDUMP_COUNT(x)	((x) & 0xFF)

void __noreturn die(const char *msg, ...) __check_format(1,2);
void __noreturn die_errno(const char *msg, ...) __check_format(1,2);

void hexdump(const void *data, size_t length, size_t flags);
void __hexdump(unsigned long, const void *data, size_t length, size_t flags);

int parse_uint32(unsigned int *number, const char *str);
int parse_ulong(unsigned long *number, const char *str);
#endif //__UTILS_H__
