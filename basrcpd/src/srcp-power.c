/* $Id: srcp-power.c 1456 2010-02-28 20:01:39Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 */

#include <string.h>
#include <stdio.h>

#include "config-srcpd.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "srcp-power.h"


int setPower(bus_t bus, int state, char *msg)
{
    gettimeofday(&buses[bus].power_change_time, NULL);
    buses[bus].power_state = (state == -1) ? 0 : state;
    /* buses[bus].power_state = state; */
    strcpy(buses[bus].power_msg, msg);
    buses[bus].power_changed = (state == -1) ? 0 : 1;
    /* buses[bus].power_changed = 1; */
    /* Resume thread to transmit power change */
    resume_bus_thread(bus);
    return SRCP_OK;
}

int getPower(bus_t bus)
{
    return buses[bus].power_state;
}

int infoPower(bus_t bus, char *msg)
{
    sprintf(msg, "%lu.%.3lu 100 INFO %ld POWER %s %s\n",
            buses[bus].power_change_time.tv_sec,
            buses[bus].power_change_time.tv_usec / 1000, bus,
            buses[bus].power_state ? "ON" : "OFF", buses[bus].power_msg);
    return SRCP_INFO;
}

int initPower(bus_t bus)
{
    return SRCP_OK;
}

int termPower(bus_t bus)
{
    if (1 == getPower(bus))
        return setPower(bus, 0, "Device Termination");

    return SRCP_OK;
}
