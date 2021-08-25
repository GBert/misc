/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#ifndef __STRBUF_H__
#define __STRBUF_H__

#include <stdarg.h>
#include "compiler.h"

struct strbuf {
	char *buf;	/* data buffer */
	char *ptr;	/* write position */
	size_t left;	/* bytes left in buffer */
	size_t len;	/* bytes stored in buffer */
};

static inline const char*
strbuf_cstr(const struct strbuf *s)
{
	return s->buf;
}

static inline char*
strbuf_str(struct strbuf *s)
{
	return s->buf;
}

void strbuf_init(struct strbuf *buf, char *buffer, size_t length);
void strbuf_append(struct strbuf *s, const char *format, ...) __check_format(2,3);
void strbuf_vappend(struct strbuf *s, const char *format, va_list ap);
char strbuf_lastchar(const struct strbuf *s);

#endif //__STRBUF_H__
