/*
 * syslogmessage.h
 */

/**************************************************************************
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 **************************************************************************/

#ifndef _SYSLOGMESSAGE_H
#define _SYSLOGMESSAGE_H

#include <stdarg.h>
#include "config-srcpd.h"


#define DBG_NONE 0
#define DBG_FATAL 1
#define DBG_ERROR 2
#define DBG_WARN 3
#define DBG_INFO 4
#define DBG_DEBUG 5

void syslog_bus(bus_t busnumber, int dbglevel, const char *fmt, ...);
void syslog_session(sessionid_t session, int dbglevel, const char *fmt, ...);

#endif
