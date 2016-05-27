/**
 * c2_bus_module.h - Things needed to write C2 Bus master modules
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

#ifndef __C2_BUS_MODULE_H__
#define __C2_BUS_MODULE_H__

/**
 * Structure containing pointers to the implementations
 *
 * This structure contains pointers to the functions that implement the various
 * bus action for the underlying bus hardware type.
 */
struct c2_bus_ops {
	int (*reset) (struct c2_bus *bus);
	int (*qreset) (struct c2_bus *bus);
	int (*addr_read) (struct c2_bus *bus, unsigned char *addr);
	int (*addr_write) (struct c2_bus *bus, unsigned char addr);
	int (*data_read) (struct c2_bus *bus, unsigned char *data, size_t len);
	int (*data_write) (struct c2_bus *bus, const unsigned char *data, size_t len);
	void (*destruct) (struct c2_bus *bus);
};

/**
 * Set error message in C2 bus context
 */
void c2_bus_set_error(struct c2_bus *bus, const char *msg);
 
/**
 * Set error message + system error in C2 bus context
 *
 * Set the C2 Bus context error message to the given error and automatically
 * append a system error based on the current value of errno.
 */
void c2_bus_set_perror(struct c2_bus *bus, const char *msg);

#endif // __C2_BUS_MODULE_H__
