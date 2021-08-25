/* $Id: srcp-power.h 1099 2008-01-07 17:37:36Z gscholz $ */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _SRCP_POWER_H
#define _SRCP_POWER_H

int initPower(bus_t);
int infoPower(bus_t bus, char *msg);
int setPower(bus_t bus, int state, char *msg);
int getPower(bus_t bus);
int termPower(bus_t);

#endif
