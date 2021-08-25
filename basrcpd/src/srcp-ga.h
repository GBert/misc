/* $Id: srcp-ga.h 1725 2016-03-20 17:06:01Z gscholz $ */

#ifndef _SRCP_GA_H
#define _SRCP_GA_H

#include <stdbool.h>
#include <sys/time.h>

#include "config-srcpd.h"
#include "srcp-session.h"

#define MAXGAPORT 2

enum ga_state_t {gasNone = 0, gasActive, gasTerm};

/* GA device data set (single accessory decoder) */
typedef struct {
    enum ga_state_t state;      /* 0==none, 1==active, 2==terminating */
    char protocol;              /* Protocol Id */
    int id;                     /* Identification */
    int port;                   /* Port number     */
    int action;                 /* 0, 1, 2, 3...  */
    long activetime;            /* Aktivierungszeit in msec bis das
                                   automatische AUS kommen soll */
    struct timeval inittime;
    struct timeval tv[MAXGAPORT];   /* time of last activation, each port
                                       gets its own time value */
    struct timeval t;           /* switch off time */
    struct timeval locktime;
    sessionid_t locked_by;      /* who has the LOCK? */
    long int lockduration;
} ga_data_t;

/* GA device group data set */
typedef struct {
    int numberOfGa;
    ga_data_t* gastate;
} ga_t;

void startup_GA();
void shutdown_GA();
int init_GA(bus_t busnumber, int number);
int get_number_ga(bus_t busnumber);

int enqueueGA(bus_t busnumber, int addr, int port, int action, int activetime);
int dequeueNextGA(bus_t busnumber, ga_data_t *);
int queue_GA_isempty(bus_t busnumber);

int getGA(bus_t busnumber, int addr, ga_data_t *a);
int setGA(bus_t busnumber, int addr, ga_data_t a);
int initGA(bus_t busnumber, int addr, const char protocol);
int termGA(bus_t busnumber, int addr);
int describeGA(bus_t busnumber, int addr, char *msg);
int infoGA(bus_t busnumber, int addr, int port, char *msg);
int cmpGA(ga_data_t a, ga_data_t b);
bool isInitializedGA(bus_t busnumber, int addr);
void clean_GA(bus_t bus);

int lockGA(bus_t busnumber, int addr, long int duration,
           sessionid_t sessionid);
int getlockGA(bus_t busnumber, int addr, sessionid_t * sessionid);
int unlockGA(bus_t busnumber, int addr, sessionid_t sessionid);
void unlock_ga_bysessionid(sessionid_t);
void unlock_ga_bytime(void);
int describeLOCKGA(bus_t bus, int addr, char *reply);

void handle_mcs_gacc(bus_t bus, char protocol, int addr, int port,
			int action, int activetime);
#endif
