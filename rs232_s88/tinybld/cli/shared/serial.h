/**
 * Serial port abstraction
 *
 * Copyright (c) 1999-2009, Joseph Heenan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Joseph Heenan nor the names of any other
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Joseph Heenan ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SERIAL_H
#define SERIAL_H

/**
 * Open serial port
 *
 * @param port     Serial device to use (eg. /dev/ttyS0)
 * @param baudrate Baudrate to use
 * @param stopbits Stops bits
 *
 * @return fd for serial port or -1 on failure
 */
int serial_openport(const char *port, int baudrate, int stopbits);

int serial_write(int fd, const char *ptr, int bytes);

int serial_read(int fd, unsigned char *ptr, int bytes);

/**
 * Read from serial port with timeout
 * 
 * Note: returns as soon as any data read, does not wait for buffer
 * to fill.
 * 
 * @param fd    fd to read from
 * @param ptr   ptr to write data to
 * @param bytes maximum number of bytes to read
 * @param msecs timeout in milliseconds
 * 
 * @retval -2 Timed out
 * @retval -1 Error
 * @return Otherwise, returns number of bytes read
 */
int serial_readtimeout(int fd, unsigned char *ptr, int bytes,
                       int msecs);

void serial_close(int fd);

/**
 * Connect stdin to serial out & serial in to stdout
 *
 * @param fd         Serial port fd to use
 * @param escapechar Character that user presents to exit terminal mode
 */
void serial_terminal(int fd, unsigned char escapechar);

void set_rts(int fd, int rts);
#endif
