/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "strbuf.h"

static void hexdump_addr(unsigned long base)
{
	printf("%08lX: ", base);
}

static void hexdump_eol(void)
{
	printf("\n");
}

static void hexdump_bin(const void *data, size_t length, size_t count)
{
	uint32_t v = *((uint32_t*)data);
	int shift, i;

	for (i = 1, shift = 31; shift >= 0; ++i, -- shift) {
		printf("%c", ((v >> shift) & 1) ? '1' : '0');

		if ((i % 8) == 0)
			printf(" ");
	}
}

static void hexdump_8bit(const void *data, size_t length, size_t count)
{
	const unsigned char *ptr = data;
	int i;

	for (i = 0; i < length; ++i) {
		printf("%02X ", *ptr ++);
	}

	for (i = length; i < count; ++i) {
		printf("   ");
	}
}

static void hexdump_16bit(const void *data, size_t length, size_t count)
{
	const uint16_t *ptr = data;
	int i;

	length /= sizeof(uint16_t);
	count  /= sizeof(uint16_t);

	for (i = 0; i < length; ++i) {
		printf("%04hX ", *ptr ++);
	}

	for (i = length; i < count; ++i) {
		printf("         ");
	}
}

static void hexdump_32bit(const void *data, size_t length, size_t count)
{
	const uint32_t *ptr = data;
	int i;

	length /= sizeof(uint32_t);
	count  /= sizeof(uint32_t);

	for (i = 0; i < length; ++i) {
		printf("%08X ", *ptr ++);
	}

	for (i = length; i < count; ++i) {
		printf("         ");
	}
}

static void hexdump_ascii(const void *data, size_t length, size_t count)
{
	const unsigned char *ptr = data;
	int i;

	printf("|");

	for (i = 0; i < length; ++i) {
		char ch = *ptr ++;
		printf("%c", isprint(ch) ? ch : '.');
	}

	printf("|");
}

void __hexdump(unsigned long base, const void *data,
	       size_t length, size_t flags)
{
	size_t count;
	int nleft;

	count = HEXDUMP_COUNT(flags);
	if (count == 0)
		count = 16;	/* default */

	if (count > 32)
		count = 32;	/* max allowed */

	if ((flags & HEXDUMP_SIZEMASK) == 0)
		flags |= HEXDUMP_8BIT;

	if (HEXDUMP_ISSET(flags, HEXDUMP_BIN))
		count = 4;

	while (length > 0) {
		nleft = length > count ? count : length;

		hexdump_addr(base);

		if (HEXDUMP_ISSET(flags, HEXDUMP_BIN)) {
			hexdump_bin(data, nleft, count);
		} else if (HEXDUMP_ISSET(flags, HEXDUMP_8BIT)) {
			hexdump_8bit(data, nleft, count);
		} else if (HEXDUMP_ISSET(flags, HEXDUMP_16BIT)) {
			hexdump_16bit(data, nleft, count);
		} else if (HEXDUMP_ISSET(flags, HEXDUMP_32BIT)) {
			hexdump_32bit(data, nleft, count);
		} else {
			die("__hexdump(): invalid %0zX flags given", flags);
		}

		if (flags & HEXDUMP_ASCII) {
			hexdump_ascii(data, nleft, count);
		}

		hexdump_eol();

		length -= nleft;
		data   += count;
		base   += count;
	}

}

void hexdump(const void *data, size_t length, size_t flags)
{
	__hexdump(0, data, length, flags);
}

void __noreturn die(const char *format, ... )
{
	char buffer[PATH_MAX];
	struct strbuf s;

	strbuf_init(&s, buffer, sizeof(buffer));

	if (format != NULL) {
		va_list ap;

		va_start(ap, format);
		strbuf_vappend(&s, format, ap);
		va_end(ap);
	}

	if (strbuf_lastchar(&s) != '\n')
		strbuf_append(&s, "\n");

	fprintf(stderr, "%s", strbuf_cstr(&s));
	exit(1);
}

void __noreturn die_errno(const char *format, ... )
{
	char buffer[PATH_MAX];
	struct strbuf s;
	int saved = errno;

	strbuf_init(&s, buffer, sizeof(buffer));

	if (format != NULL) {
		va_list ap;

		va_start(ap, format);
		strbuf_vappend(&s, format, ap);
		va_end(ap);
	}

	if (strbuf_lastchar(&s) == '\n') {
		s.ptr --;
		s.len --;
	}

	if (saved != 0)
		strbuf_append(&s, ": %s (%d)\n", strerror(saved), saved);
	else
		strbuf_append(&s, "\n");

	fprintf(stderr, "%s", strbuf_cstr(&s));
	exit(1);
}

int parse_ulong(unsigned long *number, const char *str)
{
	long value;
	char *endptr = NULL;

	errno = 0;
	value = strtoul(str, &endptr, 0);
	if ((errno == ERANGE) &&
	    (value == LONG_MAX || value == LONG_MIN))
		return -1;
	else if (errno != 0 && value == 0)
		return -1;

	/* empty string. */
	if (endptr == str) {
		return -1;
	}

	if (*endptr != '\0') {
		return -1;
	}

	*number = value;
	return 0;
}

int parse_uint32(unsigned int *number, const char *str)
{
	long value;
	char *endptr = NULL;

	errno = 0;
	value = strtoul(str, &endptr, 0);
	if ((errno == ERANGE) &&
	    (value == LONG_MAX || value == LONG_MIN))
		return -1;
	else if (errno != 0 && value == 0)
		return -1;

	/* empty string. */
	if (endptr == str) {
		return -1;
	}

	if (*endptr != '\0') {
		return -1;
	}

	*number = value;
	return 0;
}
