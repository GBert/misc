/* $Id: srcp-fb.h 1725 2016-03-20 17:06:01Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _SRCP_FB_H
#define _SRCP_FB_H
#include "config-srcpd.h"
#include <sys/time.h>

/* reserve 16 bit of space (short int) for 1 bit of information, two times */
typedef struct _FBSTATE
{
  struct timeval timestamp;
  short int state;
  short int change;
} fb_state_t;

typedef struct _FB
{
  int numberOfFb;
  fb_state_t *fbstate;
} fb_t;

typedef struct _RESET_FB
{
  int port;
  struct timeval timestamp;
} fb_reset_t;

void startup_FB();
int init_FB(bus_t bus, int number);
int get_number_fb(bus_t bus);
int initFB(bus_t busnumber, int addr, const char protocol, int index);

int getFB(bus_t bus, int port, struct timeval *time, int *value);
int setFB( bus_t bus, int port,  int value );
int updateFB(bus_t bus, int port, int value);
int setFBmodul(bus_t bus, int mod, int values);
int infoFB(bus_t bus, int port, char *msg, size_t length);
int describeFB(bus_t bus, int addr, char *reply);
void check_reset_fb(bus_t busnumber);
void set_min_time(bus_t busnumber, int mt);
#endif
