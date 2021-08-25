// clientservice.h - adapted for basrcpd project 2020 by Rainer Müller

/* cvs: $Id: clientservice.h 990 2007-12-18 20:18:43Z gscholz $             */

/*
 * Vorliegende Software unterliegt der General Public License,
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */


#ifndef _CLIENTSERVICE_H
#define _CLIENTSERVICE_H

#include "srcp-session.h"

void* thr_doClient(void *v);
int getnbr_commandsessions(void);

#endif
