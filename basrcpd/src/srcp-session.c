/**************************************************************************
                          srcp-session.c
                         -------------------
    begin                : Don Apr 25 2002
    copyright            : (C) 2002 by
    email                :
 **************************************************************************/

/**************************************************************************
 *                                                                        *
 *  This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation; either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 **************************************************************************/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "srcp-session.h"
#include "srcp-ga.h"
#include "srcp-gl.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"


/*
 * A linked list stores the data of all connected sessions. This code was
 * mainly impressed by:
 *   http://en.wikipedia.org/wiki/Linked_list#Language_support
 */

/*session counter, session list root and mutex to lock access*/
static sessionid_t lastsession = 0;
static unsigned int runningsessions = 0;
static session_node_t *session_list = NULL;
static pthread_mutex_t session_list_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t session_list_cond = PTHREAD_COND_INITIALIZER;

static pthread_mutex_t cb_mutex[MAX_BUSES];
static pthread_cond_t cb_cond[MAX_BUSES];
static int cb_data[MAX_BUSES];



/*add new session data node to list with a valid socket*/
static session_node_t *list_add(session_node_t ** p, int s)
{
    session_node_t *n;

    n = (session_node_t *) malloc(sizeof(session_node_t));

    if (NULL == n)
        return NULL;

    n->next = *p;
    *p = n;
    n->session = 0;
    n->socket = s;
    n->thread = 0;
    n->mode = smUndefined;
    n->pipefd[0] = -1;
    n->pipefd[1] = -1;
    return n;
}

/*remove session data node from list, returns "true" for success*/
static bool list_remove(session_node_t ** p)
{
    bool returnvalue = false;

    if (*p != NULL) {
        session_node_t *n = *p;
        *p = (*p)->next;
        free(n);
        returnvalue = true;
    }
    return returnvalue;
}

/* search sessionid in list, return pointer to node pointer */
static session_node_t **list_search_session(session_node_t ** n,
                                            sessionid_t sid)
{
    while (*n != NULL) {
        if ((*n)->session == sid) {
            return n;
        }
        n = &(*n)->next;
    }
    return NULL;
}

/* search sessionid in list, return pointer to node */
static session_node_t *list_search_session_node(session_node_t ** n,
                                                sessionid_t sid)
{
    while (*n != NULL) {
        if ((*n)->session == sid) {
            return *n;
        }
        n = &(*n)->next;
    }
    return NULL;
}

/* search thread id by sessionid, return thread id */
static pthread_t list_search_thread_by_sessionid(session_node_t ** n,
                                                 sessionid_t sid)
{
    while (*n != NULL) {
        if ((*n)->session == sid) {
            return (*n)->thread;
        }
        n = &(*n)->next;
    }
    return 0;
}

/* search valid info sessionid, return "true" if found */
static bool list_has_info_sessionid(session_node_t ** n, sessionid_t sid)
{
    bool returnvalue = false;

    while (*n != NULL) {
        if ((*n)->session == sid && (*n)->mode == smInfo) {
            returnvalue = true;
            break;
        }
        n = &(*n)->next;
    }
    return returnvalue;
}

/**
 * First initialisation after program start up
 */
void startup_SESSION()
{
    for (int i = 0; i < MAX_BUSES; i++) {
        int result = pthread_mutex_init(&cb_mutex[i], NULL);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_init() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        result = pthread_cond_init(&cb_cond[i], NULL);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_cond_init() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

/*destroy all occupied mutexes and condition variables*/
void shutdown_SESSION()
{
    for (int i = 0; i < MAX_BUSES; i++) {
        int result = pthread_mutex_destroy(&cb_mutex[i]);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_destroy() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        result = pthread_cond_destroy(&cb_cond[i]);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_cond_destroy() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

/*create a node with an anonymous session*/
session_node_t *create_anonymous_session(int s)
{
    int result;
    session_node_t *node = NULL;

    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(s, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    node = list_add(&session_list, s);
    if (NULL != node)
        runningsessions++;
    else
        syslog_session(s, DBG_ERROR, "Could not add session node.");

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(s, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }
    return node;
}

/*destroy a node with an anonymous session*/
void destroy_anonymous_session(session_node_t * n)
{
    int result;
    sessionid_t sid;

    if (n == NULL)
        return;

    sid = n->session;
    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(sid, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    if (list_remove(&n))
        runningsessions--;
    else
        syslog_session(sid, DBG_ERROR, "Could not remove session node.");

    result = pthread_cond_signal(&session_list_cond);
    if (result != 0) {
        syslog_session(n->session, DBG_ERROR,
                       "pthread_cond_signal() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(n->session, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }
}

/*destroy a fully functionalized session*/
void destroy_session(sessionid_t sid)
{
    int result;

    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(sid, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    if (list_remove(list_search_session(&session_list, sid)))
        runningsessions--;
    else
        syslog_session(sid, DBG_ERROR, "Could not remove session node.");

    result = pthread_cond_signal(&session_list_cond);
    if (result != 0) {
        syslog_session(sid, DBG_ERROR,
                       "pthread_cond_signal() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(sid, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }
}

/*register a new session id*/
void register_session(session_node_t * n)
{
    int result;

    if (NULL == n)
        return;

    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(lastsession + 1, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    lastsession++;
    /* check possible overflow, restart numbering
     * FIXME: new session ids now may still be used*/
    if (0 == lastsession) {
        lastsession++;
        syslog_session(lastsession, DBG_ERROR, "Session id overflow,"
                " restarting at 1");
    }
    n->session = lastsession;

    /*set default mode if mode was not set explicitly */
    if (n->mode == smUndefined)
        n->mode = smCommand;

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(n->session, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }
}

/* search for valid session id */
bool is_valid_info_session(sessionid_t session)
{
    int result;
    bool isvalid;

    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(session, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    isvalid = list_has_info_sessionid(&session_list, session);

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(session, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }
    return isvalid;
}

/*
 * Enqueue a new info message to the appropriate session pipe.
 *
 * There is no write locking because writing to pipes is atomic as
 * far as MAXSRCPLEN < PIPE_BUF and pipe write access is blocking (see
 * "man 3 write" for more details).
 */
void session_enqueue_info_message(sessionid_t sid, char *msg)
{
    session_node_t *n;
    int result;
    ssize_t nwritten;

    /*return immediately, if no session is running */
    if (NULL == session_list)
        return;

    /*enqueue message for all info sessions */
    if (sid == 0) {
        n = session_list;
        result = pthread_mutex_lock(&session_list_mutex);
        if (result != 0) {
            syslog_session(sid, DBG_ERROR,
                           "pthread_mutex_lock() failed: %s (errno = %d).",
                           strerror(result), result);
        }

        while (n != NULL) {
            if (n->mode == smInfo && n->pipefd[1] != -1) {
                nwritten = write(n->pipefd[1], msg, strlen(msg) + 1);
                if (nwritten == -1) {
                    syslog_session(n->session, DBG_ERROR,
                                   "Write to pipe failed: %s (errno = %d).",
                                   strerror(errno), errno);
                }
            }
            n = n->next;
        }

        result = pthread_mutex_unlock(&session_list_mutex);
        if (result != 0) {
            syslog_session(sid, DBG_ERROR,
                           "pthread_mutex_unlock() failed: %s (errno = %d).",
                           strerror(result), result);
        }
    }

    /*enqueue message for a single info session */
    else {
        result = pthread_mutex_lock(&session_list_mutex);
        if (result != 0) {
            syslog_session(sid, DBG_ERROR,
                           "pthread_mutex_lock() failed: %s (errno = %d).",
                           strerror(result), result);
        }

        n = list_search_session_node(&session_list, sid);

        if (n != NULL && n->mode == smInfo && n->pipefd[1] != -1) {
            nwritten = write(n->pipefd[1], msg, strlen(msg) + 1);
            if (nwritten == -1) {
                syslog_session(n->session, DBG_ERROR,
                               "Write to pipe failed: %s (errno = %d).",
                               strerror(errno), errno);
            }
        }

        result = pthread_mutex_unlock(&session_list_mutex);
        if (result != 0) {
            syslog_session(sid, DBG_ERROR,
                           "pthread_mutex_unlock() failed: %s (errno = %d).",
                           strerror(result), result);
        }
    }
}

/* terminate a session by cancellation of client thread */
void session_terminate(sessionid_t session)
{
    int result;
    pthread_t pc;

    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_session(session, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    pc = list_search_thread_by_sessionid(&session_list, session);

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_session(session, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    if (0 != pc) {
        result = pthread_cancel(pc);
        if (result != 0) {
            syslog_session(session, DBG_ERROR,
                           "pthread_cancel() failed: %s (errno = %d).",
                           strerror(result), result);
        }
    }
    else
        syslog_session(session, DBG_ERROR,
                       "Found unvalid thread id to cancel.");
}

/* terminate all active sessions */
void terminate_all_sessions()
{
    int result;

    if (session_list == NULL)
        return;

    session_node_t *node = session_list;

    /*first cancel all session threads ... */
    result = pthread_mutex_lock(&session_list_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    while (node != NULL) {
        result = pthread_cancel(node->thread);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_cancel() failed: %s (errno = %d).",
                       strerror(result), result);
        }
        node = node->next;
    }

    /*... then wait for complete termination */
    while (runningsessions != 0) {
        result =
            pthread_cond_wait(&session_list_cond, &session_list_mutex);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_cond_wait() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }

    result = pthread_mutex_unlock(&session_list_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    syslog_bus(0, DBG_INFO, "Session thread termination completed.");
}

/*this function is used by clientservice starting the session*/
int start_session(session_node_t * sn)
{
    char msg[1000];
    struct timeval akt_time;

    gettimeofday(&akt_time, NULL);

    snprintf(msg, sizeof(msg), "%lu.%.3lu 101 INFO 0 SESSION %lu %s\n",
             akt_time.tv_sec, akt_time.tv_usec / 1000, sn->session,
             (sn->mode == 1 ? "COMMAND" : "INFO"));
    enqueueInfoMessage(msg);

    syslog_session(sn->session, DBG_INFO,
                   "Session started (mode = %d).", sn->mode);
    return SRCP_OK;
}

/**
 * this funtion is called by clientservice when the client thread
 * terminates
 */
int stop_session(sessionid_t sid)
{
    char msg[1000];
    struct timeval akt_time;
    gettimeofday(&akt_time, NULL);

    /* clean all locks */
    unlock_ga_bysessionid(sid);
    unlock_gl_bysessionid(sid);

    snprintf(msg, sizeof(msg), "%lu.%.3lu 102 INFO 0 SESSION %lu\n",
             akt_time.tv_sec, akt_time.tv_usec / 1000, sid);
    enqueueInfoMessage(msg);

    return SRCP_OK;
}

int describeSESSION(bus_t bus, sessionid_t sessionid, char *reply)
{
    return SRCP_UNSUPPORTEDOPERATION;
}

/**
 * called by srcp command session finishing a session;
 * return negative value of SRCP_OK to ack the request.
 */
int termSESSION(bus_t bus, sessionid_t sessionid,
                sessionid_t termsessionid, char *reply)
{
    if (sessionid == termsessionid) {
        session_terminate(termsessionid);
        return -SRCP_OK;
    }
    return SRCP_FORBIDDEN;
}

int session_lock_wait(bus_t bus)
{
    int result;

    syslog_bus(bus, DBG_DEBUG, "SESSION process wait1 for bus.");

    result = pthread_mutex_lock(&cb_mutex[bus]);
    if (result != 0) {
        syslog_bus(bus, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    syslog_bus(bus, DBG_DEBUG, "SESSION process wait2 for bus.");
    return result;
}

int session_unlock_wait(bus_t bus)
{
    int result;

    syslog_bus(bus, DBG_DEBUG, "SESSION cleanup wait for bus.");
    result = pthread_mutex_unlock(&cb_mutex[bus]);
    if (result != 0) {
        syslog_bus(bus, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    return result;
}

int session_condt_wait(bus_t bus, unsigned int timeout, int *value)
{
    int result;
    struct timespec stimeout;
    struct timeval now;

    gettimeofday(&now, NULL);
    stimeout.tv_sec = now.tv_sec + timeout;
    stimeout.tv_nsec = now.tv_usec * 1000;

    syslog_bus(bus, DBG_DEBUG, "SESSION start wait1");

    result = pthread_cond_timedwait(&cb_cond[bus], &cb_mutex[bus],
                                    &stimeout);
    if (result != 0) {
        syslog_bus(bus, DBG_ERROR,
                   "pthread_cond_timedwait() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    *value = cb_data[bus];
    syslog_bus(bus, DBG_DEBUG, "SESSION start wait2");
    return result;
}

int session_endwait(bus_t bus, int returnvalue)
{
    int result;

    syslog_bus(bus, DBG_DEBUG, "SESSION end wait1 for bus.");
    cb_data[bus] = returnvalue;

    result = pthread_cond_broadcast(&cb_cond[bus]);
    if (result != 0) {
        syslog_bus(bus, DBG_ERROR,
                   "pthread_cond_broadcast() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_mutex_unlock(&cb_mutex[bus]);
    if (result != 0) {
        syslog_bus(bus, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    syslog_bus(bus, DBG_DEBUG, "SESSION end wait2 for bus.");
    return returnvalue;
}
