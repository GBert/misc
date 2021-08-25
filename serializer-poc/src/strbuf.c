/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#include <stdarg.h>
#include <stdio.h>
#include "utils.h"
#include "strbuf.h"

void strbuf_init(struct strbuf *s, char *buffer, size_t length)
{
	s->buf = s->ptr = buffer;
	s->len = 0;
	s->left = length;
}

void strbuf_append(struct strbuf *s, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	strbuf_vappend(s, format, ap);
	va_end(ap);
}

void strbuf_vappend(struct strbuf *s, const char *format, va_list ap)
{
	int n;

	n = vsnprintf(s->ptr, s->left, format, ap);
	if (n > 0) {
		s->ptr += n;
		s->len += n;
		s->left -= n;
	}
}

char strbuf_lastchar(const struct strbuf *s)
{
	if (s->len > 0)
		return s->buf[s->len-1];
	else
		return -1;
}
