// io.c - adapted for basrcpd project 2018 by Rainer Müller 

/***************************************************************************
                          io.c  -  description
                             -------------------
    begin                : Wed Jul 4 2001
    copyright            : (C) 2001 by Dipl.-Ing. Frank Schmischke
    email                : frank.schmischke@t-online.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
  changes:

    25.11.2007 Frank Schmischke
    in isvalidchar() change 'char' to 'unsigned char' to avoid compiler
			warning
*/

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdarg.h>
#include <sys/ioctl.h>

#include "config-srcpd.h"
#include "io.h"
#include "syslogmessage.h"

#if 0
// TODO: check if UART code could be reused, eg for Railcom
int readByte(bus_t bus, bool wait, unsigned char *the_byte)
{
    ssize_t i;
    int status;
    i=0; /* make sure every bit of the integer gets initialized, 
      prevents some ioctl's from beeing misinterpreted */
    /* with debug level beyond DBG_DEBUG, we will not really work on hardware */
    if (buses[bus].debuglevel > DBG_DEBUG) {
        i = 1;
        *the_byte = 0;
    }
    else {
        status = ioctl(buses[bus].device.file.fd, FIONREAD, &i);
        if (status == -1) {
            syslog_bus(bus, DBG_ERROR,
                       "readbyte(): ioctl() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
            return -1;
        }
        syslog_bus(bus, DBG_DEBUG,
                   "readbyte(): (fd = %d), there are %d bytes to read.",
                   buses[bus].device.file.fd, i);

        /* read only, if there is really an input or if "wait" is true
         * to do a blocking read */
        if ((i > 0) || wait) {
            i = read(buses[bus].device.file.fd, the_byte, 1);
            if (i == -1) {
                syslog_bus(bus, DBG_ERROR,
                           "readbyte(): read() failed: %s (errno = %d)\n",
                           strerror(errno), errno);
            }
            if (i > 0)
                syslog_bus(bus, DBG_DEBUG, "readbyte(): byte read: 0x%02x",
                           *the_byte);
        }
    }
    return (i > 0 ? 0 : -1);
}

void writeByte(bus_t bus, const unsigned char b, unsigned long msecs)
{
    ssize_t i = 0;
    char byte = b;

    if (buses[bus].debuglevel <= DBG_DEBUG) {
        i = write(buses[bus].device.file.fd, &byte, 1);
        syslog_bus(bus, DBG_DEBUG, "(FD: %d) %i byte sent: 0x%02x (%d)\n",
                   buses[bus].device.file.fd, i, b, b);
        if (i < 0) {
            syslog_bus(bus, DBG_ERROR, "(FD: %d) write failed: %s "
                       "(errno = %d)\n",
                       buses[bus].device.file.fd, strerror(errno), errno);
        }
        tcdrain(buses[bus].device.file.fd);
    }
    else {
        syslog_bus(bus, DBG_DEBUG, "(FD: %d) %i byte sent: 0x%02x (%d)\n",
                   buses[bus].device.file.fd, i, b, b);
    }
    if (usleep(msecs * 1000) == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "usleep() failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }
}

void writeString(bus_t bus, const char *s, unsigned long msecs)
{
    size_t l = strlen(s);
    size_t i;

    for (i = 0; i < l; i++) {
        writeByte(bus, s[i], msecs);
    }
}

void save_comport(bus_t bus)
{
    int fd;

    fd = open(buses[bus].device.file.path, O_RDWR);
    if (fd == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "Open serial line failed: %s (errno = %d).\n",
                   strerror(errno), errno);
    }
    else {
        tcgetattr(fd, &buses[bus].device.file.devicesettings);
        close(fd);
    }
}

void restore_comport(bus_t bus)
{
    int fd;

    syslog_bus(bus, DBG_INFO, "Restoring attributes for serial line %s",
               buses[bus].device.file.path);
    fd = open(buses[bus].device.file.path, O_RDWR);
    if (fd == -1) {
        syslog_bus(bus, DBG_ERROR,
                   "Open serial line failed: %s (errno = %d).\n",
                   strerror(errno), errno);
    }
    else {
        syslog_bus(bus, DBG_INFO, "Restoring old values...");
        tcsetattr(fd, TCSANOW, &buses[bus].device.file.devicesettings);
        close(fd);
        syslog_bus(bus, DBG_INFO, "Old values successfully restored");
    }
}

void close_comport(bus_t bus)
{
    struct termios interface;
    syslog_bus(bus, DBG_INFO, "Closing serial line");

    tcgetattr(buses[bus].device.file.fd, &interface);
    cfsetispeed(&interface, B0);
    cfsetospeed(&interface, B0);
    tcsetattr(buses[bus].device.file.fd, TCSANOW, &interface);
    close(buses[bus].device.file.fd);
}
#endif

// ssplitstr splits a string str into n tokens by assigning n pointers
// if the nr of tokens is bigger than the nr of pointers, last points to remainder
// original string is modified with term zeros, retval is nr of assigned pointers
int ssplitstr(char * str, int n, ...)
{
	int s = 1, i = 0;
	unsigned char c;

    va_list vl;
    va_start(vl,n);
	while ((c = *str)) {
		if (s && (c > 0x20)) {
			*(va_arg(vl, char **)) = str;	// fill pointer
			if(++i == n) break; 			// no more pointer available
		}
		if ((s = (c <= 0x20))) *str = 0;	// end of token terminated with 0
		str++;
	}
	while (n-- > i) 						// no more token available
		*(va_arg(vl, char **)) = str; 		// point to terminating zero
	va_end(vl);
	return i;
}

/* Zeilenweises Lesen vom Socket      */
/* nicht eben trivial!                */
static int isvalidchar(unsigned char c)
{
    return ((c >= 0x20 && c <= 127) || c == 0x09 || c == '\n');
}

/*
 * Read a text line from socket descriptor including newline character
 * (like fgets()).
 * return values
 *   -1: error
 *    0: end of file (EOF), client terminated connection
 *   >0: number of read characters
 * */
ssize_t socket_readline(int Socket, char *line, int len)
{
    char c;
    int i = 0;
    ssize_t bytes_read;

  again:
    bytes_read = read(Socket, &c, 1);
    if (bytes_read == -1) {

        /* handle interrupt */
        if (errno == EINTR)
            goto again;

        /* normal read error */
        else
            return -1;
    }

    /*EOF detected, client closed connection */
    else if (bytes_read == 0) {
        return 0;
    }

    /*normal operation */
    else {
        if (isvalidchar(c))
            line[i++] = c;
        /* die Reihenfolge beachten! */
        /*TODO: handle (errno == EINTR), message part will get lost */
        while (read(Socket, &c, 1) > 0) {
            if (isvalidchar(c) && (i < len - 1))
                line[i++] = c;
            /* stop at newline character */
            if (c == '\n')
                break;
        }
    }
    line[i++] = 0x00;
    return (i - 1);
}

/* Write "n" bytes to a descriptor. Stevens, UNP;
 * srcp messages must end with '\n' to use this function directly 
 * return values:
 *   -1: write error
 *  >=0: number of written characters */
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            /* if EINTR call write() again */
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            /* a real error */
            else
                return (-1);
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}
