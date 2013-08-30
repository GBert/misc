/*
 * Copyright (C) 2012 Žilvinas Valinskas
 * See LICENSE for more information.
 */
#ifndef __COMPILER_H__
#define __COMPILER_H__

#if defined(__GNUC__) && __GNUC__ >= 4
#define __noreturn	__attribute__((noreturn))
#define __init		__attribute__((constructor))
#define __exit		__attribute__((destructor))
#define __used		__attribute__((used))
#define __align(x)	__attribute__((__aligned__(x)))
#define __packed	__attribute__((__packed__))
#define __section(x)	__attribute__((__section__ (#x)))

#define __check_format(x, y) \
	__attribute__((format(printf, x, y)))
#else
#error Unsupported compiler.
#endif

#endif //__COMPILER_H__
