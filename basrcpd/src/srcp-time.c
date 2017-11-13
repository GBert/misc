/* 
 * Vorliegende Software unterliegt der General Public License, 
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "config-srcpd.h"
#include "srcp-time.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"


/*local variables for time thread*/
static pthread_t time_tid;
static pthread_mutex_t time_value_mutex = PTHREAD_MUTEX_INITIALIZER;
static vtime_t vtime;
static pthread_mutex_t time_distort_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t time_distort_cond = PTHREAD_COND_INITIALIZER;
static time_distort_t distort;



int startup_TIME(void)
{
    gettimeofday(&distort.inittime, NULL);
    return 0;
}

static void set_time(vtime_t * vt)
{
    int result;

    result = pthread_mutex_lock(&time_value_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    vtime = *vt;

    result = pthread_mutex_unlock(&time_value_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
}

int setTIME(int d, int h, int m, int s)
{
    vtime_t vt;
    char msg[100];

    if (d < 0 || h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59)
        return SRCP_WRONGVALUE;

    vt.day = d;
    vt.hour = h;
    vt.min = m;
    vt.sec = s;

    set_time(&vt);

    infoTIME(msg);
    enqueueInfoMessage(msg);

    return SRCP_OK;
}

int initTIME(int fx, int fy)
{
    if (fx < 0 || fy <= 0)
        return SRCP_WRONGVALUE;

    char msg[100];
    int result;

    result = pthread_mutex_lock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    distort.ratio_x = fx;
    distort.ratio_y = fy;
    gettimeofday(&distort.inittime, NULL);

    result = pthread_cond_signal(&time_distort_cond);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_cond_signal() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_mutex_unlock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    describeTIME(msg);
    enqueueInfoMessage(msg);
    return SRCP_OK;
}

int termTIME()
{
    int result;
    char msg[100];
    struct timeval current_time;

    result = pthread_mutex_lock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    distort.ratio_x = distort.ratio_y = 0;

    result = pthread_mutex_unlock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    gettimeofday(&current_time, NULL);
    snprintf(msg, sizeof(msg), "%lu.%.3lu 102 INFO 0 TIME\n",
             current_time.tv_sec, current_time.tv_usec / 1000);
    enqueueInfoMessage(msg);

    return SRCP_OK;
}

int getTIME(vtime_t * vt)
{
    int result;

    result = pthread_mutex_lock(&time_value_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    *vt = vtime;

    result = pthread_mutex_unlock(&time_value_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    return SRCP_OK;
}

static int get_distortion(time_distort_t * td)
{
    int result;

    result = pthread_mutex_lock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    *td = distort;

    result = pthread_mutex_unlock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    return SRCP_OK;
}

bool time_is_available()
{
    int result;
    bool available;

    result = pthread_mutex_lock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    available = ((distort.ratio_x > 0) && (distort.ratio_y > 0));

    result = pthread_mutex_unlock(&time_distort_mutex);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    return available;
}

int infoTIME(char *msg)
{
    msg[0] = '\0';
    int result;
    vtime_t vt;
    struct timeval akt_time;

    if (!time_is_available())
        return SRCP_NODATA;

    result = getTIME(&vt);
    if (result != SRCP_OK)
        return result;

    gettimeofday(&akt_time, NULL);

    sprintf(msg, "%lu.%.3lu 100 INFO 0 TIME %d %d %d %d\n",
            akt_time.tv_sec, akt_time.tv_usec / 1000,
            vt.day, vt.hour, vt.min, vt.sec);

    return SRCP_INFO;
}

int describeTIME(char *reply)
{
    time_distort_t td;
    int result;

    result = get_distortion(&td);
    if (result != SRCP_OK)
        return result;

    sprintf(reply, "%lu.%.3lu 101 INFO 0 TIME %d %d\n",
            td.inittime.tv_sec, td.inittime.tv_usec / 1000,
            td.ratio_x, td.ratio_y);

    return SRCP_INFO;
}

/***********************************************************************
 * time thread, updates time value in model time scale *
 ***********************************************************************/
void *thr_clock(void *v)
{
    int result;
    vtime_t vt;
    unsigned long sleeptime;
    bool sendinfo = false;
    distort.ratio_x = distort.ratio_y = 0;

    while (true) {

        result = pthread_mutex_lock(&time_distort_mutex);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        /*wait here suspended if no timer is needed */
        while (distort.ratio_x == 0 || distort.ratio_y == 0) {
            result =
                pthread_cond_wait(&time_distort_cond, &time_distort_mutex);
            if (result != 0) {
                syslog_bus(0, DBG_ERROR,
                           "pthread_cond_wait() failed: %s (errno = %d).",
                           strerror(result), result);
            }
        }

        /* delta Modellzeit = delta real time * ratio_x/ratio_y */
        sleeptime = (1000000 * distort.ratio_y) / distort.ratio_x;

        result = pthread_mutex_unlock(&time_distort_mutex);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        if (usleep(sleeptime) == -1) {
            syslog_bus(0, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }

        /* use temporary copy for calculations, so vtime is always valid */
        getTIME(&vt);
        vt.sec++;
        if (vt.sec >= 60) {
            vt.sec = 0;
            vt.min++;
            sendinfo = true;
        }
        if (vt.min >= 60) {
            vt.hour++;
            vt.min = 0;
        }
        if (vt.hour >= 24) {
            vt.day++;
            vt.hour = 0;
        }
        set_time(&vt);

        if (sendinfo) {
            sendinfo = false;
            char msg[100];
            infoTIME(msg);
            enqueueInfoMessage(msg);
        }
    }
}


/*create time/clock thread*/
void create_time_thread()
{
    int result;

    result = pthread_create(&time_tid, NULL, thr_clock, NULL);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR, "Create time thread failed: %s "
                   "(errno = %d)\n", strerror(result), result);
        return;
    }

    syslog_bus(0, DBG_DEBUG, "Time thread created.");
}

/*cancel time/clock thread*/
void cancel_time_thread()
{
    int result;

    result = pthread_cancel(time_tid);
    if (result != 0)
        syslog_bus(0, DBG_ERROR,
                   "Time thread cancel failed: %s (errno = %d).",
                   strerror(result), result);

    /*wait until time thread terminates */
    result = pthread_join(time_tid, NULL);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "Time thread join failed: %s (errno = %d).",
                   strerror(result), result);
        return;
    }

    syslog_bus(0, DBG_DEBUG, "Time thread cancelled.");
}
