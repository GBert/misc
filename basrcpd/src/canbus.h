// canbus.h : new bus type CANBUS for the srcpd project
// allows communication srcp <-> canbus
//
// C 2015 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#ifndef _CANBUS_H
#define _CANBUS_H

#include <libxml/tree.h> /*xmlDocPtr, xmlNodePtr*/

typedef struct _CANBUS_DATA {
    int number_ga;
//    int number_gl;
    int number_fb;
	unsigned int pause_between_cmd;
    ga_data_t tga[50];
} CANBUS_DATA;

int readconfig_CANBUS(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber);
int init_bus_CANBUS(bus_t );
//int init_gl_CANBUS(gl_state_t *);
int init_ga_CANBUS(ga_data_t *);
int getDescription_CANBUS(char *reply);
void* thr_sendrec_CANBUS(void *);

#endif
