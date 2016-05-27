/**
 * c2_bus.h - C2 Bus Master interface functions
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
#ifndef __C2_BUS_H__
#define __C2_BUS_H__

#include <stdint.h>

struct c2_bus {
	struct c2_bus_ops *ops;
	void *ctx;
	char *error_msg;
};

/**
 * Get last error message
 *
 * If one of the C2 bus functions fail an error message will be set in the
 * context. This function returns this error message.
 *
 * @returns	String containing an error message of the last error
 */
const char *c2_bus_get_error(struct c2_bus *bus);

/**
 * Open C2 hardware bus
 *
 * Open a C2 hardware bus interface from the given type at the given path.
 *
 * @param type	Type of the bus
 * @param path	The type specific path that uniquely identifies the bus
 */
int c2_bus_open(struct c2_bus *bus, const char *type, const char *path);

/**
 * Issue Reset 
 *
 * Causes attached device to reset.
 */
int c2_bus_reset(struct c2_bus *bus);

/**
 * Issue 'quick' reset
 *
 * Issues a 'quick' reset on the bus. This is used to halt the attached
 * microcontroller. Issuing a quick reset when the attached microcontroller is
 * not running will break the communication.
 */
int c2_bus_qreset(struct c2_bus *bus);

/**
 * Read address register
 *
 * Read the address register from the attached device.
 *
 * @param addr	Variable used to return address in
 */
int c2_bus_addr_read(struct c2_bus *bus, uint8_t *addr);

/**
 * Write address register
 *
 * Write the address register of the attached device.
 *
 * @param addr	Value to write to the address register
 */
int c2_bus_addr_write(struct c2_bus *bus, uint8_t addr);

/**
 * Read data register
 *
 * Read data registers at the current address. A maximum of 4 byte can be read
 * at once.
 *
 * @param data	Data buffer to store bytes in
 * @param len	Amount of bytes to read, must be >= 1 and <= 4
 */
int c2_bus_data_read(struct c2_bus *bus, uint8_t *data, size_t len);

/**
 * Write data bytes
 *
 * Write data registers at the current address. A maximum of 4 byte can be
 * written at once.
 *
 * @param data	Data bytes to write
 * @param len	Amount of bytes to write, must be >= 1 and <= 4
 */
int c2_bus_data_write(struct c2_bus *bus, const uint8_t *data, size_t len);

/**
 * Close C2 bus and free all associated resources
 *
 * Closes the C2 bus and free all associated resources. This function also has
 * to be called if c2_bus_open() has failed.
 */
void c2_bus_destroy(struct c2_bus *bus);

#endif // __C2_BUS_H__
