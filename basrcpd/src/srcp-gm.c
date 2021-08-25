/* $Id: srcp-gm.c 1456 2010-02-28 20:01:39Z gscholz $ */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config-srcpd.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "srcp-gm.h"
#include "syslogmessage.h"


int setGM(sessionid_t sid, sessionid_t rid, char *msg)
{
    struct timeval akt_time;
    char msgtmp[MAXSRCPLINELEN];

    syslog_bus(0, DBG_DEBUG, "GM message: %s", msg);

    if (sid != 0 && !is_valid_info_session(sid))
        return SRCP_WRONGVALUE;

    if (rid != 0 && !is_valid_info_session(rid))
        return SRCP_WRONGVALUE;

    /*
       <time> 100 INFO 0 GM <send_to_id> <reply_to_id> <message_type> <message>
     */
    gettimeofday(&akt_time, NULL);
    snprintf(msgtmp, sizeof(msgtmp),
             "%lu.%.3lu 100 INFO 0 GM %lu %lu %s\n",
             akt_time.tv_sec, akt_time.tv_usec / 1000, sid, rid, msg);

    session_enqueue_info_message(sid, msgtmp);

    return SRCP_OK;
}
