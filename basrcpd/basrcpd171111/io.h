/***************************************************************************
                          io.h  -  description
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
#ifndef _IO_H
#define _IO_H

#include <unistd.h>
#include <stdbool.h>

#include "config-srcpd.h"


int  readByte(bus_t bus, bool wait, unsigned char *the_byte);
void writeByte(bus_t bus, const unsigned char the_byte, unsigned long msec);
void writeString(bus_t bus, const char *the_string, unsigned long msecs);

void restore_comport(bus_t bus);
void save_comport(bus_t bus);
void close_comport(bus_t bus);

ssize_t socket_readline(int Socket, char *line, int len);
ssize_t writen(int fd, const void *vptr, size_t n);

#endif
