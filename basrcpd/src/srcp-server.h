/* $Id: srcp-server.h 1725 2016-03-20 17:06:01Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _SRCP_SERVER_H
#define _SRCP_SERVER_H

#include <libxml/tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>

#include "config-srcpd.h" /*for bus_t*/
#include "config.h" /*for VERSION*/


/* SRCP server welcome message */
static const char WELCOME_MSG[] =
    "srcpd V" VERSION "; SRCP 0.8.4; SRCPOTHER 0.8.3\n";

typedef struct _SERVER_DATA {
    unsigned short int TCPPORT;
    char *listenip;
    char *username;
    char *groupname;
} SERVER_DATA;

typedef enum {
    ssInitializing = 0,
    ssRunning,
    ssTerminating,
    ssResetting
} server_state_t;

void set_server_state(server_state_t);
server_state_t get_server_state();

int readconfig_server(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber);
void startup_SERVER();
int init_bus_server(bus_t);
void server_reset(void);
void server_shutdown(void);
int describeSERVER(bus_t bus, int addr, char *reply);
int infoSERVER(char *msg);

#endif

