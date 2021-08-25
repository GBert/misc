/* $Id: srcp-descr.h 1725 2016-03-20 17:06:01Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */

#ifndef _SRCP_DESCR_H
#define _SRCP_DESCR_H

#include "config-srcpd.h"

void startup_DESCRIPTION();
int describeBus(bus_t bus, char *reply);
#endif
