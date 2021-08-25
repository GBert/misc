// srcpd-server.c - adapted for basrcpd project 2018 by Rainer Müller 

/* $Id: srcp-server.c 1725 2016-03-20 17:06:01Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "config-srcpd.h"
#include "srcp-server.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"
#include "platform.h"

#define __server ((SERVER_DATA*)buses[0].driverdata)

xmlFreeFunc xmlFree = (xmlFreeFunc) free;

char PIDFILENAME[MAXPATHLEN] = "/var/run/srcpd.pid";

/* This variable is accessible by several threads at the same time and
 * should be protected by a lock */
static server_state_t server_state = ssInitializing;


int readconfig_server(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber)
{
    struct servent *serviceentry;

    syslog_bus(busnumber, DBG_INFO, "Reading configuration for bus '%s'",
               node->name);

    buses[0].driverdata = malloc(sizeof(struct _SERVER_DATA));

    /* if there is too less memory for server data -> exit process */
    if (buses[0].driverdata == NULL) {
        syslog_bus(busnumber, DBG_ERROR,
                   "Memory allocation error in module '%s'.", node->name);
        exit(EXIT_FAILURE);
    }

    buses[0].type = SERVER_SERVER;
    buses[0].init_func = &init_bus_server;
    strcpy(buses[0].description, "SESSION SERVER TIME GM");

    /* initialize _SERVER_DATA with defaults */
    serviceentry = getservbyname("srcp", "tcp");
    if (serviceentry == NULL) {
        __server->TCPPORT = 4303;
    }
    else {
        __server->TCPPORT = ntohs(serviceentry->s_port);
    }
    __server->groupname = NULL;
    __server->username = NULL;
    __server->listenip = NULL;

    xmlNodePtr child = node->children;
    xmlChar *txt = NULL;

    while (child != NULL) {
        if ((xmlStrncmp(child->name, BAD_CAST "text", 4) == 0) ||
            (xmlStrncmp(child->name, BAD_CAST "comment", 7) == 0)) {
            /* just do nothing, it is only formatting text or a comment */
        }

        else if (xmlStrcmp(child->name, BAD_CAST "tcp-port") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __server->TCPPORT = atoi((char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "listen-ip") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                xmlFree(__server->listenip);
                __server->listenip = malloc(strlen((char *) txt) + 1);
                strcpy(__server->listenip, (char *) txt);
                syslog_bus(busnumber, DBG_INFO, "listen-ip: %s", txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "pid-file") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                strncpy((char *) &PIDFILENAME, (char *) txt,
                        MAXPATHLEN - 2);
                PIDFILENAME[MAXPATHLEN - 1] = 0x00;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "username") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                xmlFree(__server->username);
                __server->username = malloc(strlen((char *) txt) + 1);
                if (__server->username == NULL) {
                    syslog_bus(busnumber, DBG_ERROR,
                               "Cannot allocate memory\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(__server->username, (char *) txt);
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "groupname") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                xmlFree(__server->groupname);
                __server->groupname = malloc(strlen((char *) txt) + 1);
                if (__server->groupname == NULL) {
                    syslog_bus(busnumber, DBG_ERROR,
                               "Cannot allocate memory\n");
                    exit(EXIT_FAILURE);
                }
                strcpy(__server->groupname, (char *) txt);
                xmlFree(txt);
            }
        }

        else
            syslog_bus(busnumber, DBG_WARN,
                       "WARNING, unknown tag found: \"%s\"!\n",
                       child->name);

        child = child->next;
    }

    return (1);
}


void startup_SERVER()
{
}

int describeSERVER(bus_t bus, int addr, char *reply)
{
    if (bus) return SRCP_UNSUPPORTEDOPERATION;
    
	struct timeval tv = { 0, 0 };
	gettimeofday(&tv, NULL);
    sprintf(reply, "%lu.%.3lu 101 INFO 0 SERVER BASRCPD " VERSION 
					" SERIAL %llu TEMP %llu\n",
            		tv.tv_sec, tv.tv_usec / 1000, 
					getPlatformData(PL_SERIAL), getPlatformData(PL_TEMP)/100);

    return SRCP_INFO;
}

int init_bus_server(bus_t bus)
{
    gettimeofday(&buses[0].power_change_time, NULL);
    syslog_bus(bus, DBG_INFO, "init_bus %ld", bus);
    return 0;
}

void set_server_state(server_state_t state)
{
    server_state = state;
}

server_state_t get_server_state()
{
    return server_state;
}

void server_reset()
{
    char msg[100];
    set_server_state(ssResetting);
    infoSERVER(msg);
    enqueueInfoMessage(msg);
    syslog_bus(0, DBG_INFO, "Server reset - cleaning all buses.");

    /* It is not quite clear what the server may do now. This is a
     * proposal:
     *   1) Remove all LOCKs
     *   2) Remove all GLs
     *   3) Remove all GAs
     * After reset all connected clients may send an init for their
     * devices to get them in operation again.
     */
    for (bus_t bus = 1; bus <= num_buses; bus++) {
        cacheCleanGL(bus);
        clean_GA(bus);
    }

    set_server_state(ssRunning);
    infoSERVER(msg);
    enqueueInfoMessage(msg);
}

void server_shutdown()
{
    char msg[100];
    set_server_state(ssTerminating);
    infoSERVER(msg);
    enqueueInfoMessage(msg);
}

int infoSERVER(char *msg)
{
    struct timeval akt_time;
    gettimeofday(&akt_time, NULL);

    switch (get_server_state()) {
        case ssResetting:
            sprintf(msg, "%lu.%.3lu 100 INFO 0 SERVER RESETTING\n",
                    akt_time.tv_sec, akt_time.tv_usec / 1000);
            break;

        case ssTerminating:
            sprintf(msg, "%lu.%.3lu 100 INFO 0 SERVER TERMINATING\n",
                    akt_time.tv_sec, akt_time.tv_usec / 1000);
            break;

        case ssRunning:
            sprintf(msg, "%lu.%.3lu 100 INFO 0 SERVER RUNNING\n",
                    akt_time.tv_sec, akt_time.tv_usec / 1000);
            break;

        default:
            syslog_bus(0, DBG_ERROR,
                       "ERROR, unexpected server state detected!");
            break;
    }
    return SRCP_OK;
}
