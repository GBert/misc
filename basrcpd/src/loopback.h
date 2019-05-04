// loopback.h - adapted for basrcpd project 2018 by Rainer Müller 

/* $Id: loopback.h 1176 2008-01-29 22:42:17Z schmischi $ */
/* This software is published under the terms of the GNU General Public
   License, Version 2, 1991. 
*/

#ifndef _LOOPBACK_H
#define _LOOPBACK_H

#include <libxml/tree.h> /*xmlDocPtr, xmlNodePtr*/

typedef struct _LOOPBACK_DATA {
    int number_ga;
    int number_gl;
    int number_fb;
    ga_data_t tga[50];
} LOOPBACK_DATA;

int readconfig_LOOPBACK(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber);
int init_bus_LOOPBACK(bus_t );
int init_gl_LOOPBACK(gl_data_t *, char *optData);
int init_ga_LOOPBACK(ga_data_t *);
int getDescription_LOOPBACK(char *reply);
void* thr_sendrec_LOOPBACK(void *);

#endif
