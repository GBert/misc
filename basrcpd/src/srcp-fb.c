
/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config-srcpd.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "srcp-fb.h"
#include "syslogmessage.h"


/* one array for all buses             */
/* not visible outside of this module   */
static fb_t fb[MAX_BUSES];
static int min_time[MAX_BUSES];

#define QUEUELENGTH_FB 1000

/* reset fb queue */
static fb_reset_t reset_queue[MAX_BUSES][QUEUELENGTH_FB];
static pthread_mutex_t queue_mutex_fb, queue_mutex_reset[MAX_BUSES];
static int out[MAX_BUSES], in[MAX_BUSES];


/* internal functions */

static int enqueueInfoFB(bus_t busnumber, int port)
{
    char msg[MAXSRCPLINELEN];
    infoFB(busnumber, port, msg, sizeof(msg));
    enqueueInfoMessage(msg);
    return SRCP_OK;
}

static int queueLengthFB(bus_t busnumber)
{
    if (in[busnumber] >= out[busnumber])
        return in[busnumber] - out[busnumber];
    else
        return QUEUELENGTH_FB + in[busnumber] - out[busnumber];
}

static int queueIsFullFB(bus_t busnumber)
{
    return queueLengthFB(busnumber) >= QUEUELENGTH_FB - 1;
}

int queueIsEmptyFB(bus_t busnumber)
{
    return (in[busnumber] == out[busnumber]);
}

/** returns next entry and >=0, or -1 */
static int getNextFB(bus_t busnumber, fb_reset_t * info)
{
    int result;

    if (in[busnumber] == out[busnumber])
        return -1;

    result = pthread_mutex_lock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    info->port = reset_queue[busnumber][out[busnumber]].port;
    info->timestamp = reset_queue[busnumber][out[busnumber]].timestamp;

    result = pthread_mutex_unlock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    return out[busnumber];
}

/** returns next entry or -1, resets fifo pointer  ! */
static void unqueueNextFB(bus_t busnumber)
{
    int result;

    result = pthread_mutex_lock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    out[busnumber]++;
    if (out[busnumber] == QUEUELENGTH_FB)
        out[busnumber] = 0;
    result = pthread_mutex_unlock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

static void queue_reset_fb(bus_t busnumber, int port,
                           struct timeval *ctime)
{
    int result;

    while (queueIsFullFB(busnumber)) {
        /* wait 1 ms */
        if (usleep(1000) == -1) {
            syslog_bus(busnumber, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }
    }

    result = pthread_mutex_lock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    reset_queue[busnumber][in[busnumber]].port = port;
    reset_queue[busnumber][in[busnumber]].timestamp = *ctime;
    in[busnumber]++;
    if (in[busnumber] == QUEUELENGTH_FB)
        in[busnumber] = 0;

    result = pthread_mutex_unlock(&queue_mutex_reset[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

int getFB(bus_t bus, int port, struct timeval *time, int *value)
{
    port--;
    if (get_number_fb(bus) <= 0 || (port < 0)
        || (port >= get_number_fb(bus)))
        return SRCP_NODATA;

    *value = fb[bus].fbstate[port].state;
    if (time != NULL)
        *time = fb[bus].fbstate[port].timestamp;
    return SRCP_OK;
}

int setFB(bus_t bus, int port, int value)
{
    return updateFB(bus, port, value);
}

int updateFB(bus_t bus, int port, int value)
{
    int result;
    int port_t;
    struct timezone dummy;
    struct timeval akt_time;

    /* check range to disallow segmentation fault */
    if ((port > get_number_fb(bus)) || (port < 1))
        return SRCP_WRONGVALUE;

    port_t = port - 1;
    /* we read 8 or 16 ports at once, but we will only change those ports, */
    /* which are really changed */
    /*  */
    /* if changed contact is reset, we will wait 'min_time[bus]' seconds to */
    /* minimize problems from contacts between track and train */
    if (fb[bus].fbstate[port_t].state != value) {
        syslog_bus(bus, DBG_DEBUG, "FB %02i/%04i is set to \"%i\"", bus,
                   port, value);

        gettimeofday(&akt_time, &dummy);
        if (value == 0) {
            if (min_time[bus] == 0) {
                fb[bus].fbstate[port_t].state = value;
                fb[bus].fbstate[port_t].timestamp = akt_time;
                fb[bus].fbstate[port_t].change = 0;
                enqueueInfoFB(bus, port);
            }
            else {
                result = pthread_mutex_lock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(bus, DBG_ERROR,
                               "pthread_mutex_lock() failed: %s (errno = %d).",
                               strerror(result), result);
                }

                fb[bus].fbstate[port_t].change = -1;

                result = pthread_mutex_unlock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(bus, DBG_ERROR,
                               "pthread_mutex_unlock() failed: %s (errno = %d).",
                               strerror(result), result);
                }

                queue_reset_fb(bus, port_t, &akt_time);
            }
        }
        else {
            if (fb[bus].fbstate[port_t].change < 0) {
                fb[bus].fbstate[port_t].change = 0;
            }
            else {
                result = pthread_mutex_lock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(bus, DBG_ERROR,
                               "pthread_mutex_lock() failed: %s (errno = %d).",
                               strerror(result), result);
                }

                fb[bus].fbstate[port_t].state = value;
                fb[bus].fbstate[port_t].timestamp = akt_time;
                fb[bus].fbstate[port_t].change = 0;

                result = pthread_mutex_unlock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(bus, DBG_ERROR,
                               "pthread_mutex_unlock() failed: %s (errno = %d).",
                               strerror(result), result);
                }

                /* queue changes for writing info-message */
                enqueueInfoFB(bus, port);
            }
        }
    }
    return SRCP_OK;
}

/* all modules with 8 or 16 ports */
int setFBmodul(bus_t bus, int modul, int values)
{
    int i;
    int c;
    int fb_contact;
    int ports;
    int dir;
    int mask;
    int rc;
    rc = SRCP_OK;

    ports = ((buses[bus].flags & FB_16_PORTS) == FB_16_PORTS) ? 16 : 8;
    if (buses[bus].flags & FB_4_PORTS)
        ports = 4;
    if ((buses[bus].flags & FB_ORDER_0) == FB_ORDER_0) {
        dir = 0;
        mask = 1;
    }
    else {
        dir = 1;
        mask = 1 << (ports - 1);
    }
    /* compute start contact ( (module - 1) * ports + 1 ) */
    fb_contact = modul - 1;
    fb_contact *= ports;
    fb_contact++;

    for (i = 0; i < ports; i++) {
        c = (values & mask) ? 1 : 0;
        rc = updateFB(bus, fb_contact++, c);
        if (dir)
            mask >>= 1;
        else
            mask <<= 1;
    }
    return rc;
}

int infoFB(bus_t bus, int port, char *msg, size_t length)
{
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;
    int state = 0;
    int rc = getFB(bus, port, &time, &state);
    msg[0] = 0x00;

    if (rc >= SRCP_OK) {
        snprintf(msg, length, "%lu.%.3lu 100 INFO %ld FB %d %d\n",
                 time.tv_sec, time.tv_usec / 1000, bus, port, state);
        return SRCP_INFO;
    }
    else {
        return SRCP_NODATA;
    }
}

int describeFB(bus_t bus, int addr, char *reply)
{
    return SRCP_UNSUPPORTEDOPERATION;
}

void startup_FB()
{
    for (int i = 0; i < MAX_BUSES; i++) {
        fb[i].numberOfFb = 0;
        fb[i].fbstate = NULL;
        out[i] = 0;
        in[i] = 0;
        min_time[i] = 0;
    }
}

void set_min_time(bus_t busnumber, int mt)
{
    if (busnumber < MAX_BUSES)
        min_time[busnumber] = mt * 1000;
}

int init_FB(bus_t bus, int number)
{
    struct timeval akt_time;
    int i;

    if (bus >= MAX_BUSES)
        return 1;

    if (number > 0) {
        gettimeofday(&akt_time, NULL);

        fb[bus].fbstate = malloc(number * sizeof(fb_state_t));
        if (fb[bus].fbstate == NULL)
            return 1;
        fb[bus].numberOfFb = number;
        for (i = 0; i < number; i++) {
            fb[bus].fbstate[i].state = 0;
            fb[bus].fbstate[i].change = 0;
            fb[bus].fbstate[i].timestamp = akt_time;
        }
    }
    return 0;
}

int initFB(bus_t busnumber, int adres, const char protocol, int index)
{
    int rc = SRCP_OK;
    int number_fb = get_number_fb(busnumber);
    syslog_bus(busnumber, DBG_DEBUG, "init FB: %d %c %d", adres, protocol,
               index);
    if ((adres > 0) && (adres <= number_fb)) {
        if (buses[busnumber].init_fb_func != NULL)
            rc = (*buses[busnumber].init_fb_func) (busnumber, adres,
                                                   protocol, index);
    }
    return rc;
}

int get_number_fb(bus_t bus)
{
    return fb[bus].numberOfFb;
}

void check_reset_fb(bus_t busnumber)
{
    int result;
    fb_reset_t reset_fb;
    struct timeval cmp_time, diff_time;

    while (getNextFB(busnumber, &reset_fb) != -1) {
        if (fb[busnumber].fbstate[reset_fb.port].change == 0) {
            /* drop this reset of feedback, because we've got an new impulse */
            unqueueNextFB(busnumber);
        }
        else {
            gettimeofday(&cmp_time, NULL);
            syslog_bus(busnumber, DBG_DEBUG,
                       "FB %02i/%03i  time: %ld.%ld  compare: %ld.%ld",
                       busnumber, reset_fb.port, cmp_time.tv_sec,
                       cmp_time.tv_usec, reset_fb.timestamp.tv_sec,
                       reset_fb.timestamp.tv_usec);
            diff_time.tv_sec = cmp_time.tv_sec - reset_fb.timestamp.tv_sec;
            diff_time.tv_usec =
                cmp_time.tv_usec - reset_fb.timestamp.tv_usec;
            diff_time.tv_usec += (diff_time.tv_sec * 1000000);
            syslog_bus(busnumber, DBG_DEBUG,
                       "FB %02i/%03i  time-diff = %ld us (need %d us)",
                       busnumber, reset_fb.port, diff_time.tv_usec,
                       min_time[busnumber]);
            if (diff_time.tv_usec < min_time[busnumber]) {
                break;
            }
            else {
                syslog_bus(busnumber, DBG_DEBUG, "set %d feedback to 0",
                           reset_fb.port);
                unqueueNextFB(busnumber);

                result = pthread_mutex_lock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(busnumber, DBG_ERROR,
                               "pthread_mutex_lock() failed: %s (errno = %d).",
                               strerror(result), result);
                }

                fb[busnumber].fbstate[reset_fb.port].state = 0;
                fb[busnumber].fbstate[reset_fb.port].timestamp =
                    reset_fb.timestamp;
                fb[busnumber].fbstate[reset_fb.port].change = 0;

                result = pthread_mutex_unlock(&queue_mutex_fb);
                if (result != 0) {
                    syslog_bus(busnumber, DBG_ERROR,
                               "pthread_mutex_unlock() failed: %s (errno = %d).",
                               strerror(result), result);
                }
                enqueueInfoFB(busnumber, reset_fb.port + 1);
            }
        }
    }
}
