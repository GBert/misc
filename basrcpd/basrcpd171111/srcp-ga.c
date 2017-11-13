/* $Id: srcp-ga.c 1739 2016-04-22 20:42:26Z gscholz $ */
/*
 * This software is published under the restrictions of the
 * GNU License Version2
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "config-srcpd.h"
#include "srcp-session.h"
#include "srcp-ga.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"

#define QUEUELEN 50

/* current state state */
static volatile ga_t ga[MAX_BUSES];

/* command queues for each bus */
static ga_data_t queue[MAX_BUSES][QUEUELEN];
static pthread_mutex_t queue_mutex[MAX_BUSES];
static int out[MAX_BUSES], in[MAX_BUSES];

/* forward declaration of internal functions */
static int queue_len(bus_t busnumber);
static int queue_isfull(bus_t busnumber);


int get_number_ga(bus_t busnumber)
{
    return ga[busnumber].numberOfGa;
}

/* Uebernehme die neuen Angaben fuer die Weiche, einige wenige Pruefungen */
int enqueueGA(bus_t busnumber, int addr, int port, int action,
              long int activetime)
{
    struct timeval now;
    int number_ga = get_number_ga(busnumber);

    if ((addr > 0) && (addr <= number_ga)) {
        if (queue_isfull(busnumber)) {
            syslog_bus(busnumber, DBG_WARN, "GA Command Queue full");
            return SRCP_TEMPORARILYPROHIBITED;
        }

        int result = pthread_mutex_lock(&queue_mutex[busnumber]);
        if (result != 0) {
            syslog_bus(busnumber, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        queue[busnumber][in[busnumber]].protocol =
            ga[busnumber].gastate[addr].protocol;
        queue[busnumber][in[busnumber]].action = action;
        queue[busnumber][in[busnumber]].port = port;
        queue[busnumber][in[busnumber]].activetime = activetime;
        gettimeofday(&now, NULL);
        queue[busnumber][in[busnumber]].tv[port] = now;
        queue[busnumber][in[busnumber]].id = addr;
        in[busnumber]++;
        if (in[busnumber] == QUEUELEN)
            in[busnumber] = 0;

        result = pthread_mutex_unlock(&queue_mutex[busnumber]);
        if (result != 0) {
            syslog_bus(busnumber, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
        }
        /* Restart thread to send GL command */
        resume_bus_thread(busnumber);
    }
    else {
        return SRCP_WRONGVALUE;
    }
    return SRCP_OK;
}

int queue_GA_isempty(bus_t busnumber)
{
    return (in[busnumber] == out[busnumber]);
}

static int queue_len(bus_t busnumber)
{
    if (in[busnumber] >= out[busnumber])
        return in[busnumber] - out[busnumber];
    else
        return QUEUELEN + in[busnumber] - out[busnumber];
}

/* maybe, 1 element in the queue cannot be used.. */
static int queue_isfull(bus_t busnumber)
{
    return queue_len(busnumber) >= QUEUELEN - 1;
}

/** liefert naechsten Eintrag oder -1, setzt fifo pointer neu! */
int dequeueNextGA(bus_t busnumber, ga_data_t *a)
{
    if (in[busnumber] == out[busnumber])
        return -1;

    *a = queue[busnumber][out[busnumber]];
    out[busnumber]++;
    if (out[busnumber] == QUEUELEN)
        out[busnumber] = 0;
    return out[busnumber];
}

int getGA(bus_t busnumber, int addr, ga_data_t *a)
{
    int number_ga = get_number_ga(busnumber);

    if ((addr > 0) && (addr <= number_ga)) {
        *a = ga[busnumber].gastate[addr];
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

bool isInitializedGA(bus_t busnumber, int addr)
{
    return ga[busnumber].gastate[addr].protocol != 0x00;
}

/* ********************
 *   SRCP commands
 */
int setGA(bus_t busnumber, int addr, ga_data_t a)
{
    int number_ga = get_number_ga(busnumber);

    if ((addr > 0) && (addr <= number_ga)) {
        char msg[1000];
        if (!isInitializedGA(busnumber, addr))
            initGA(busnumber, addr, 'P');
        ga[busnumber].gastate[addr].id = a.id;
        ga[busnumber].gastate[addr].action = a.action;
        ga[busnumber].gastate[addr].port = a.port;
        gettimeofday(&ga[busnumber].gastate[addr].
                     tv[ga[busnumber].gastate[addr].port], NULL);

        infoGA(busnumber, addr, a.port, msg);
        enqueueInfoMessage(msg);
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

int termGA(bus_t busnumber, int addr)
{
    /*pointer to ga state data*/
    ga_data_t* ga_tmp;

    if (isInitializedGA(busnumber, addr)) {

        ga_tmp = &ga[busnumber].gastate[addr];
        ga_tmp->state = gasTerm;

        /* send termination info GA */
        char msg[256];
        struct timeval now;
        gettimeofday(&now, NULL);
        snprintf(msg, sizeof(msg), "%lu.%.3lu 102 INFO %ld GA %d\n",
                now.tv_sec, now.tv_usec / 1000, busnumber, addr);
        enqueueInfoMessage(msg);

        /* at least clear GA data */
        memset(ga_tmp, 0, sizeof(ga_data_t));

        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

int describeGA(bus_t busnumber, int addr, char *msg)
{
    int number_ga = get_number_ga(busnumber);

    if (number_ga <= 0)
        return SRCP_UNSUPPORTEDDEVICEGROUP;

    if ((addr > 0) && (addr <= number_ga)
        && (ga[busnumber].gastate[addr].protocol)) {
        sprintf(msg, "%lu.%.3lu 101 INFO %ld GA %d %c\n",
                ga[busnumber].gastate[addr].inittime.tv_sec,
                ga[busnumber].gastate[addr].inittime.tv_usec / 1000,
                busnumber, addr, ga[busnumber].gastate[addr].protocol);
    }
    else {
        strcpy(msg, "");
        return SRCP_NODATA;
    }
    return SRCP_INFO;
}

int infoGA(bus_t busnumber, int addr, int port, char *msg)
{
    int number_ga = get_number_ga(busnumber);

    if ((addr > 0) && (addr <= number_ga) && (port >= 0)
        && (port < MAXGAPORT)
        && (ga[busnumber].gastate[addr].tv[port].tv_sec > 0)) {
        sprintf(msg, "%lu.%.3lu 100 INFO %ld GA %d %d %d\n",
                ga[busnumber].gastate[addr].tv[port].tv_sec,
                ga[busnumber].gastate[addr].tv[port].tv_usec / 1000,
                busnumber, addr, port, ga[busnumber].gastate[addr].action);
    }
    else {
        struct timeval t;
        gettimeofday(&t, NULL);
        srcp_fmt_msg(SRCP_NODATA, msg, t);
        return SRCP_NODATA;
    }
    return SRCP_INFO;
}

int initGA(bus_t busnumber, int addr, const char protocol)
{
    int rc = SRCP_OK;
    int number_ga = get_number_ga(busnumber);
    syslog_bus(busnumber, DBG_INFO, "init GA: %d %c", addr, protocol);
    if ((addr > 0) && (addr <= number_ga)) {
        char msg[100];
        rc = bus_supports_protocol(busnumber, protocol);
        if (rc != SRCP_OK) {
            return rc;
        }
        ga[busnumber].gastate[addr].protocol = protocol;
        gettimeofday(&ga[busnumber].gastate[addr].inittime, NULL);
        ga[busnumber].gastate[addr].activetime = 0;
        ga[busnumber].gastate[addr].action = 0;
        for (int i = 0; i < MAXGAPORT; i++) {
            ga[busnumber].gastate[addr].tv[i].tv_sec = 0;
            ga[busnumber].gastate[addr].tv[i].tv_usec = 0;
        }
        if (buses[busnumber].init_ga_func != NULL)
            rc = (*buses[busnumber].init_ga_func) (&ga[busnumber].
                                                   gastate[addr]);
        if (rc == SRCP_OK) {
            ga[busnumber].gastate[addr].state = gasActive;
            describeGA(busnumber, addr, msg);
            enqueueInfoMessage(msg);
        }
        return rc;
    }
    else {
        return SRCP_UNSUPPORTEDDEVICE;
    }
}

int lockGA(bus_t busnumber, int addr, long int duration,
           sessionid_t sessionid)
{
    char msg[256];

    if (ga[busnumber].gastate[addr].locked_by == sessionid ||
        ga[busnumber].gastate[addr].locked_by == 0) {
        ga[busnumber].gastate[addr].locked_by = sessionid;
        ga[busnumber].gastate[addr].lockduration = duration;
        gettimeofday(&ga[busnumber].gastate[addr].locktime, NULL);
        describeLOCKGA(busnumber, addr, msg);
        enqueueInfoMessage(msg);
        return SRCP_OK;
    }
    else {
        return SRCP_DEVICELOCKED;
    }
    /* unreached */
    return SRCP_UNSUPPORTEDOPERATION;
}

int getlockGA(bus_t busnumber, int addr, sessionid_t *sessionid)
{
    *sessionid = ga[busnumber].gastate[addr].locked_by;
    return SRCP_OK;

}

int describeLOCKGA(bus_t bus, int addr, char *reply)
{
    sprintf(reply, "%lu.%.3lu 100 INFO %ld LOCK GA %d %ld %ld\n",
            ga[bus].gastate[addr].locktime.tv_sec,
            ga[bus].gastate[addr].locktime.tv_usec / 1000,
            bus, addr, ga[bus].gastate[addr].lockduration,
            ga[bus].gastate[addr].locked_by);
    return SRCP_OK;
}

int unlockGA(bus_t busnumber, int addr, sessionid_t sessionid)
{
    /*pointer to ga state data*/
    ga_data_t* ga_tmp;
    char msg[256];

    ga_tmp = &ga[busnumber].gastate[addr];

    if (ga_tmp->locked_by == 0 || ga_tmp->locked_by == sessionid) {
        ga_tmp->locked_by = 0;
        gettimeofday(&ga_tmp->locktime, NULL);
        sprintf(msg, "%lu.%.3lu 102 INFO %ld LOCK GA %d\n",
                ga_tmp->locktime.tv_sec,
                ga_tmp->locktime.tv_usec / 1000,
                busnumber, addr);
        enqueueInfoMessage(msg);
        return SRCP_OK;
    }
    else {
        return SRCP_DEVICELOCKED;
    }
}

void unlock_ga_bysessionid(sessionid_t sessionid)
{
    syslog_session(sessionid, DBG_DEBUG, "Unlocking GAs by session-id");
    /* buses start with 1 */
    for (bus_t i = 1; i <= num_buses; i++) {
        int number = get_number_ga(i);
        for (int j = 1; j <= number; j++) {
            if (ga[i].gastate[j].locked_by == sessionid) {
                unlockGA(i, j, sessionid);
            }
        }
    }
}

/* must be called exactly once per second */
void unlock_ga_bytime(void)
{
    /* buses start with 1 */
    for (bus_t i = 1; i <= num_buses; i++) {
        int number = get_number_ga(i);
        for (int j = 1; j <= number; j++) {
            if (ga[i].gastate[j].lockduration-- == 1) {
                unlockGA(i, j, ga[i].gastate[j].locked_by);
            }
        }
    }
}

/*init GA mutexes for all buses*/
void startup_GA()
{
    for (bus_t i = 0; i < MAX_BUSES; i++) {
        in[i] = 0;
        out[i] = 0;
        ga[i].numberOfGa = 0;
        ga[i].gastate = NULL;

        int result = pthread_mutex_init(&queue_mutex[i], NULL);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_init() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

/*destroy all occupied mutexes*/
void shutdown_GA()
{
    for (bus_t i = 0; i < MAX_BUSES; i++) {
        free(ga[i].gastate);
        int result = pthread_mutex_destroy(&queue_mutex[i]);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_destroy() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

int init_GA(bus_t busnumber, int number)
{
    if (busnumber >= MAX_BUSES)
        return 1;

    if (number > 0) {
        /* one more, 'cause we do not use index 0, but start with 1 */
        ga[busnumber].gastate = malloc((number + 1) * sizeof(ga_data_t));
        if (ga[busnumber].gastate == NULL)
            return 1;
        ga[busnumber].numberOfGa = number;
        memset(&ga[busnumber].gastate[0], 0,
                 (number + 1) * sizeof(ga_data_t));
    }
    return 0;
}

/*clean up GA content for server reset*/
void clean_GA(bus_t bus)
{
    for (int i = 1; i <= ga[bus].numberOfGa; i++) {
        bzero(&ga[bus].gastate[i], sizeof(ga_data_t));
    }
}
