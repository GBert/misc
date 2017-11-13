/***************************************************************************
                          srcp-session.h  -  description
                             -------------------
    begin                : Don Apr 25 2002
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

#ifndef _SRCP_SESSION_H
#define _SRCP_SESSION_H

#include <stdbool.h>
#include <pthread.h>

#include "config-srcpd.h"

/*session modes*/
typedef enum {smUndefined = 0, smCommand, smInfo} SessionMode;

/*session list node to store session data*/
typedef struct sn {
    sessionid_t session;
    pthread_t thread;
    int socket;
    SessionMode mode;
    int pipefd[2];
    struct sn *next;
} session_node_t;


void startup_SESSION();
void shutdown_SESSION();

session_node_t* create_anonymous_session(int);
void register_session(session_node_t*);
void destroy_anonymous_session(session_node_t*);
void destroy_session(sessionid_t);
void terminate_all_sessions();
bool is_valid_info_session(sessionid_t);
void session_enqueue_info_message(sessionid_t, char*);

int start_session(session_node_t*);
int stop_session(sessionid_t);
int describeSESSION(bus_t, sessionid_t, char*);
int termSESSION(bus_t, sessionid_t, sessionid_t, char*);

int session_lock_wait(bus_t);
int session_condt_wait(bus_t, unsigned int timeout, int *result);
int session_unlock_wait(bus_t);
int session_endwait(bus_t, int returnvalue);

#endif
