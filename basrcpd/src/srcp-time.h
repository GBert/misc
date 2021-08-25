/* $Id: srcp-time.h 1294 2009-01-18 20:24:57Z gscholz $ */

/* 
 * Vorliegende Software unterliegt der General Public License, 
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _SRCP_TIME_H
#define _SRCP_TIME_H

#include <sys/time.h>

/* time value */
typedef struct _VTIME
{
  int day;
  int hour;
  int min;
  int sec;
} vtime_t;

/* time distortion */
typedef struct TIMEDISTORT
{
  int ratio_x; /* ratio_x == 0 stops the clock */
  int ratio_y;
  struct timeval inittime;
} time_distort_t;

int startup_TIME(void);
int setTIME(int d, int h, int m, int s);
int initTIME(int fx, int fy);
int termTIME();
int getTIME(vtime_t *vt);
int infoTIME(char *msg);
int waitTIME(int d, int h, int m, int s, char *reply);
int describeTIME(char *reply);
bool time_is_available();

void create_time_thread();
void cancel_time_thread();

#endif
