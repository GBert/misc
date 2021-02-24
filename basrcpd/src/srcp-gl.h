// srcp-gl.h - adapted for basrcpd project 2018 by Rainer Müller 

/* $Id: srcp-gl.h 1346 2009-06-06 18:15:28Z mtrute $ */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _SRCP_GL_H
#define _SRCP_GL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "config-srcpd.h"
#include "srcp-session.h"

#define MAXMFXGL	16380
#define MAXNMRAGL	10240
#define MAXMMGL		256
#define MAXSRCPGL	256

#define SCSPEED     1       /* bit mask for speedchange */
#define SCDIREC     2
#define SCEMERG     4

typedef struct _GLDIR {
	uint16_t	mfxdir[MAXMFXGL];	/* list of used mfx addresses */
	uint16_t	nmradir[MAXNMRAGL];	/* list of used DCC addresses */
	uint16_t	mmdir[MAXMMGL];		/* list of used MM addresses  */
	char		proto[MAXSRCPGL];	/* protocol used by srcp */
} gl_dir_t;

enum gl_state_t {glsNone = 0, glsTerm, glsInit, glsActive};

/* locomotive decoder */
typedef struct _GLSTATE {
    char state;                 /* 0=dead, 1=terminating, 2=init, 3=active */
    char protocol;
    char protocolversion;
    uint8_t	 n_func;
    uint16_t id;                /* address  */
    uint8_t  n_fs;
    uint8_t  speed;             /* Sollgeschwindigkeit skal. auf 0..14 */
    char direction;             /* 0/1/2                               */
    uint32_t funcs;             /* Fx, F1, ... F31                     */
    uint32_t funcschange;
    uint8_t	 speedchange;
    uint8_t  cachedspeed; 
    char cacheddirection;  	   
    struct timeval tv;          /* Last time of change                 */
    struct timeval inittime;
    struct timeval locktime;
    long int lockduration;
    sessionid_t locked_by;
    uint32_t decuid; 			/* mfx decoder UID					   */
} gl_data_t;

typedef struct _GL {
    int numberOfGl;
    gl_data_t *glstate;
    gl_dir_t  *gldir;
} GL;

void startup_GL();
void shutdown_GL();
int init_GL(bus_t busnumber, unsigned int count);
unsigned int getMaxAddrGL(bus_t busnumber);
uint16_t isInitializedGL(bus_t busnumber, int index);
int enqueueGL(bus_t busnumber, uint32_t locid, int dir, int speed,
        int maxspeed, int f);
int queue_GL_isempty(bus_t busnumber);
gl_data_t * dequeueNextGL(bus_t busnumber);
int cacheGetGL(bus_t busnumber, int addr, gl_data_t * l);
void cacheSetGL(bus_t busnumber, gl_data_t *glp, gl_data_t *l);
int cacheInfoGL(bus_t busnumber, uint32_t locid, char *info);
int cacheDescribeGL(bus_t busnumber, uint32_t locid, char *msg);
int cacheInitGL(bus_t busnumber, uint32_t locid, const char protocol,
                int protoversion, int n_fs, int n_func, char *optData);
int cacheTermGL(bus_t busnumber, uint32_t locid);
void cacheCleanGL(bus_t bus);
int cacheLockGL(bus_t busnumber, int addr, long int duration,
                sessionid_t sessionid);
int cacheGetLockGL(bus_t busnumber, uint32_t locid, sessionid_t * sessionid);
int cacheUnlockGL(bus_t busnumber, int addr, sessionid_t sessionid);
void unlock_gl_bysessionid(sessionid_t sessionid);
void unlock_gl_bytime(void);
int describeLOCKGL(bus_t bus, int addr, char *reply);
void debugGL(bus_t busnumber, gl_data_t *glp);

int handle_mcs_prot(bus_t bus, uint32_t locid, int val);
int handle_mcs_speed(bus_t bus, uint32_t locid, int val);
int handle_mcs_dir(bus_t bus, uint32_t locid, int val);
int handle_mcs_func(bus_t bus, uint32_t locid, int funr, int val);

#endif
