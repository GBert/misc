/*
 * syslogmessage.c
 */

/**************************************************************************
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "syslogmessage.h"

/*
 * DBG: write some syslog information is current debug level of the
        bus is greater then the the debug level of the message. e.g.
        if a debug message is deb_info and the bus is configured
        to inform only about deb_error, no message will be generated.
 * @param busnumber
   integer, busnumber
 * @param dbglevel
   one of the constants DBG_FATAL, DBG_ERROR, DBG_WARN, DBG_INFO, DBG_DEBUG
 * @param fmt
   const char *: standard c format string
 * @param ...
   remaining parameters according to format string
 */

void syslog_bus(bus_t busnumber, int dbglevel, const char *fmt, ...)
{
    if (dbglevel <= buses[busnumber].debuglevel) {
        va_list parm;
        va_start(parm, fmt);
        char *msg;
        msg = (char *) malloc(sizeof(char) * (strlen(fmt) + 18));
        if (msg == NULL)
            return;
        sprintf(msg, "[bus %ld] %s", busnumber, fmt);
        vsyslog(LOG_INFO, msg, parm);
        free(msg);

        va_end(parm);
    }
}

void syslog_session(sessionid_t session, int dbglevel, const char *fmt,
                    ...)
{
    if (dbglevel <= buses[0].debuglevel) {
        va_list parm;
        va_start(parm, fmt);

        char *msg;
        msg = (char *) malloc(sizeof(char) * (strlen(fmt) + 22));
        if (msg == NULL)
            return;
        sprintf(msg, "[session %ld] %s", session, fmt);
        vsyslog(LOG_INFO, msg, parm);
        free(msg);

        va_end(parm);
    }
}
