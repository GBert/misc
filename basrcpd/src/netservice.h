/* $Id: netservice.h 983 2007-12-16 09:06:42Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _NETSERVICE_H
#define _NETSERVICE_H

#include "config-srcpd.h"

typedef struct _thr_param
{
    bus_t busnumber;
} thr_param;

void create_netservice_thread();
void cancel_netservice_thread();

#endif
