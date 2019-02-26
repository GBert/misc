// srcp-info.c - adapted for basrcpd project 2018 by Rainer Müller 
//
/***************************************************************************
                          srcp-info.c  -  description
                             -------------------
    begin                : Mon May 20 2002
    copyright            : (C) 2002 by
    email                :
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
   This code manages INFO SESSIONs. Every hardware driver, alias »bus
   process«, must call (directly or via set<devicegroup> functions) the
   enqueueInfoMessage() function. This function is delegated to the
   session function session_enqueue_info_message() which writes the
   preformated string into the session specific message pipes.

   On the other end of each session pipe the information session process
   is waiting for new enqueued messages to write them to the socket file
   descriptor. The process is blocked by the select() function to only
   be bussy when real work has to be done.

   When a new INFO session starts, it will first send all available
   status data and then wait for newly arriving messages.
 */

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config-srcpd.h"
#include "io.h"
#include "srcp-gl.h"
#include "srcp-ga.h"
#include "srcp-fb.h"
#include "srcp-sm.h"
#include "srcp-power.h"
#include "srcp-info.h"
#include "srcp-error.h"
#include "srcp-descr.h"
#include "srcp-time.h"
#include "srcp-session.h"
#include "syslogmessage.h"

#define max(a,b)   ((a) > (b) ? (a) : (b))


/* Enqueue a pre-formatted message */
int enqueueInfoMessage(char *msg)
{
    session_enqueue_info_message(0, msg);
    return SRCP_OK;
}

/* There is nothing to do here. */
void startup_INFO()
{
}

/**
 * Handler for info mode client thread;
 * terminates on write failure or if cancelled externaly
 **/
int doInfoClient(session_node_t * sn)
{
    int i, n, number, value, result, linelen, bufferlen;
    char reply[MAXSRCPLINELEN], description[MAXSRCPLINELEN];
    char pipebuffer[2048];
    char *buffer = &pipebuffer[0];
    char *bufferend = &pipebuffer[sizeof(pipebuffer) - 1];
    char *linestart;
    struct timeval cmp_time;
    bus_t bus;
    fd_set rset;
    int maxfdp1;
    ssize_t rwresult = 0;
    ssize_t swritten = 0;
    ssize_t total = 0;

    result = pipe(sn->pipefd);
    if (-1 == result) {
        syslog_session(sn->session, DBG_ERROR,
                       "Pipe create failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        return (-1);
    }

    maxfdp1 = max(sn->socket, sn->pipefd[0]) + 1;
    FD_ZERO(&rset);

    syslog_session(sn->session, DBG_DEBUG, "New INFO client requested.");

    /* send start up-information to a new client */
    for (bus = 0; bus <= num_buses; bus++) {
        pthread_testcancel();
        syslog_session(sn->session, DBG_DEBUG,
                       "Send all data for bus number %d to new client.",
                       bus);

        /* first some global bus data */
        /* send Descriptions for buses */
        describeBus(bus, reply);
        if (writen(sn->socket, reply, strlen(reply)) == -1) {
            syslog_session(sn->session, DBG_ERROR,
                           "Socket write failed: %s (errno = %d)\n",
                           strerror(errno), errno);
            return -1;
        }
        strcpy(description, reply);
        *reply = 0x00;

        if (strstr(description, "POWER")) {
            infoPower(bus, reply);
            if (writen(sn->socket, reply, strlen(reply)) == -1) {
                syslog_session(sn->session, DBG_ERROR,
                               "Socket write failed: %s (errno = %d)\n",
                               strerror(errno), errno);
                return -1;
            }
            *reply = 0x00;
        }

        if (strstr(description, "TIME")) {
            describeTIME(reply);
            if (writen(sn->socket, reply, strlen(reply)) == -1) {
                syslog_session(sn->session, DBG_ERROR,
                               "Socket write failed: %s (errno = %d)\n",
                               strerror(errno), errno);
                return -1;
            }
            *reply = 0x00;
            infoTIME(reply);
            if (writen(sn->socket, reply, strlen(reply)) == -1) {
                syslog_session(sn->session, DBG_ERROR,
                               "Socket write failed: %s (errno = %d)\n",
                               strerror(errno), errno);
                return -1;
            }
            *reply = 0x00;
        }

        /* send all needed generic locomotives */
        if (strstr(description, "GL")) {
            number = getMaxAddrGL(bus);
            for (n = 1; n <= number; n++) {
                i = isInitializedGL(bus, n);
				if (i > 0) {
                    sessionid_t lockid;
                    cacheDescribeGL(bus, i, reply);
                    if (writen(sn->socket, reply, strlen(reply)) == -1) {
                        syslog_session(sn->session, DBG_ERROR,
                                       "Socket write failed: %s (errno = %d)\n",
                                       strerror(errno), errno);
                        return -1;
                    }
                    *reply = 0x00;
                    if (cacheInfoGL(bus, i, reply) == SRCP_INFO) {
                    	if (writen(sn->socket, reply, strlen(reply)) == -1) {
                        	syslog_session(sn->session, DBG_ERROR,
                                       "Socket write failed: %s (errno = %d)\n",
                                       strerror(errno), errno);
                        	return -1;
                        }
                    }
                    *reply = 0x00;
                    cacheGetLockGL(bus, i, &lockid);
                    if (lockid != 0) {
                        describeLOCKGL(bus, i, reply);
                        if (writen(sn->socket, reply, strlen(reply)) == -1) {
                            syslog_session(sn->session, DBG_ERROR,
                                           "Socket write failed: %s (errno = %d)\n",
                                           strerror(errno), errno);
                            return -1;
                        }
                        *reply = 0x00;
                    }
                }
            }
        }

        /* send all needed generic accessories */
        if (strstr(description, "GA")) {
            number = get_number_ga(bus);
            for (i = 1; i <= number; i++) {
                if (isInitializedGA(bus, i)) {
                    sessionid_t lockid;
                    int rc, port;
                    describeGA(bus, i, reply);
                    if (writen(sn->socket, reply, strlen(reply)) == -1) {
                        syslog_session(sn->session, DBG_ERROR,
                                       "Socket write failed: %s (errno = %d)\n",
                                       strerror(errno), errno);
                        return -1;
                    }
                    *reply = 0x00;
                    for (port = 0; port <= 1; port++) {
                        rc = infoGA(bus, i, port, reply);
                        if (rc == SRCP_INFO) {
                            if (writen(sn->socket, reply, strlen(reply)) ==
                                -1) {
                                syslog_session(sn->session, DBG_ERROR,
                                               "Socket write failed: %s (errno = %d)\n",
                                               strerror(errno), errno);
                                return -1;
                            }
                            *reply = 0x00;
                        }
                    }
                    getlockGA(bus, i, &lockid);
                    if (lockid != 0) {
                        describeLOCKGA(bus, i, reply);
                        if (writen(sn->socket, reply, strlen(reply)) == -1) {
                            syslog_session(sn->session, DBG_ERROR,
                                           "Socket write failed: %s (errno = %d)\n",
                                           strerror(errno), errno);
                            return -1;
                        }
                        *reply = 0x00;
                    }
                }
            }
        }

        /* send all needed feedbacks */
        if (strstr(description, "FB")) {
            number = get_number_fb(bus);
            for (i = 1; i <= number; i++) {
                int rc = getFB(bus, i, &cmp_time, &value);
                if (rc == SRCP_OK && value != 0) {
                    infoFB(bus, i, reply, sizeof(reply));
                    if (writen(sn->socket, reply, strlen(reply)) == -1) {
                        syslog_session(sn->session, DBG_ERROR,
                                       "Socket write failed: %s (errno = %d)\n",
                                       strerror(errno), errno);
                        return -1;
                    }
                    *reply = 0x00;
                }
            }
        }
    }
    syslog_session(sn->session, DBG_DEBUG,
                   "All messages send to new INFO client.\n");

    /*
     * There is a kind of race condition: Newly piped messages may
     * be ignored until we reach this point. But there is no message
     * loss because the following action will properly detect and send
     * them.
     */

    /*
     * This while loop has two tasks:
     *   1) Wait for enqueued info messages to send them to the
     *      connected client.
     *   2) Detect a client connection close in a timely manner.
     *
     *  The select() function helps to detect activities on these two
     *  diffent communication channels each having its own file
     *  descriptor.
     */
    while (true) {
        pthread_testcancel();

        FD_SET(sn->pipefd[0], &rset);
        FD_SET(sn->socket, &rset);

        if ((result = select(maxfdp1, &rset, NULL, NULL, NULL)) == -1) {
            if (errno == EINTR)
                continue;
            else {
                syslog_session(sn->session, DBG_ERROR,
                               "Select failed: %s (errno = %d)\n",
                               strerror(errno), errno);
                return (-1);
            }
        }

        /* some socket activity was detected */
        if (FD_ISSET(sn->socket, &rset)) {
            memset(reply, 0, sizeof(reply));
            rwresult = read(sn->socket, &reply, sizeof(reply));

            if (0 == rwresult) {
                syslog_session(sn->session, DBG_INFO,
                               "Client terminated INFO session.\n");
                return (-1);
            }

            if (-1 == rwresult) {
                syslog_session(sn->session, DBG_INFO,
                               "Socket read failed: %s (errno = %d).\n",
                               strerror(errno), errno);
                return (-1);
            }

            syslog_session(sn->session, DBG_INFO,
                           "Unknown client message for INFO session: %s.\n",
                           reply);
        }

        /* Message from enqueueing process arrived; dequeue piped info
         * messages and write them to socket. Watch out for truncated
         * messages if pipe or read buffer is completely full.*/
        if (FD_ISSET(sn->pipefd[0], &rset)) {

            linelen = 0;

            do {
                /* start of read buffer has an offset if there is a
                 * truncated message from last read */
                buffer = &pipebuffer[0] + linelen;
                bufferlen = sizeof(pipebuffer) - linelen;
                linelen = 0;
                total = 0;
                swritten = 0;
                linestart = NULL;

                rwresult = read(sn->pipefd[0], buffer, bufferlen);

                /* pipe read error */
                if (-1 == rwresult) {
                    syslog_session(sn->session, DBG_ERROR,
                                   "Pipe read failed: %s (errno = %d)\n",
                                   strerror(errno), errno);
                    return -1;
                }

                /* EOF from other end of pipe */
                if (0 == rwresult) {
                    syslog_session(sn->session, DBG_ERROR,
                                   "Pipe closed unexpectedly.\n");
                    return -1;
                }

                /* check if buffer is full and last line was truncated;
                 * if truncated line found, search start of line */
                if (rwresult == bufferlen && *bufferend != '\0') {
                    linestart = bufferend - 1;
                    while (linestart >= &pipebuffer[0]) {
                        if (*linestart == '\0') {
                            linelen = bufferend - linestart;
                            linestart++;
                            break;
                        }
                        linestart--;
                    }
                    if (linestart == &pipebuffer[0]) {
                        syslog_session(sn->session, DBG_ERROR,
                                       "Pipe read buffer overfilled.");
                        return -1;
                    }
                }

                /* normal operation; write several times to socket if pipe
                 * containes more than one message string, which is
                 * terminated by '\0' */
                buffer = &pipebuffer[0];
                do {
                    swritten = writen(sn->socket, buffer, strlen(buffer));
                    total += swritten + 1;
                    buffer += swritten + 1;

                    /* if there is a truncated message, move it to start of
                     * buffer to be used after next read*/
                    if (buffer == linestart) {
                        memmove(&pipebuffer[0], linestart, linelen);
                        break;
                    }
                }
                while (total < rwresult && swritten > 0);

                /* socket write error */
                if (-1 == swritten) {
                    syslog_session(sn->session, DBG_ERROR,
                                   "Socket write failed: %s (errno = %d)\n",
                                   strerror(errno), errno);
                    return -1;
                }

                /* EOF, client terminated connection */
                if (0 == swritten) {
                    syslog_session(sn->session, DBG_WARN,
                                   "Socket write failed (connection terminated "
                                   "by client).\n");
                    return -1;
                }

            } while (linestart != NULL);
        }
    }
    return 0;
}
