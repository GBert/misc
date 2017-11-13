/* $Id: srcp-descr.c 1725 2016-03-20 17:06:01Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 */

#include <stdio.h>

#include "srcp-descr.h"
#include "srcp-error.h"
#include "config-srcpd.h"

void startup_DESCRIPTION()
{
}

int describeBus(bus_t bus, char *reply)
{
    sprintf(reply, "%lu.%.3lu 100 INFO %ld DESCRIPTION %s\n",
            buses[bus].power_change_time.tv_sec,
            buses[bus].power_change_time.tv_usec / 1000, bus,
            buses[bus].description);
    return SRCP_INFO;
}
