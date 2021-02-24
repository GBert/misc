// srcp-gl.c - adapted for basrcpd project 2018 by Rainer Müller

/* $Id: srcp-gl.c 1740 2016-04-24 12:36:36Z gscholz $ */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config-srcpd.h"
#include "srcp-gl.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"

#define QUEUELEN 200 //SIG 29.10.16, 50 sind zuwenig!
#define MCSADDRINDICATOR	0x10000

/* current state */
static struct _GL gl[MAX_BUSES];

/* command queues for each bus */
static gl_data_t * queue[MAX_BUSES][QUEUELEN];

static pthread_mutex_t queue_mutex[MAX_BUSES];

/* write position for queue writers */
static int out[MAX_BUSES], in[MAX_BUSES];

/* forward declaration of internal functions */
static int queue_len(bus_t busnumber);
static int queue_isfull(bus_t busnumber);

/**
 * getMaxAddrGL: returns the maximum address for GL on the given bus
 * returns: =0: no GL on that bus, or busid not valid
 *          >0: maximum address
 */
unsigned int getMaxAddrGL(bus_t busnumber)
{
    if (busnumber <= num_buses) {
        return gl[busnumber].numberOfGl;
    }
    else
        return 0;
}

/* there are decoders for 14, 27, 28 and 128 speed steps */
static int calcspeed(int vs, int vmax, int n_fs)
{
    int rs;

    if (vmax == 0)
        return vs;
    if (vs < 0)
        vs = 0;
    if (vs > vmax)
        vs = vmax;
    /* rs = (vs * n_fs) / vmax; */
    /* for test: rs = ((vs * n_fs) / v_max) + 0.5 */
    /* ==> rs = ((2 * vs * n_fs) + v_max) / (2 * v_max) */
    rs = vs * n_fs;             /* vs * n_fs */
    rs <<= 1;                   /* 2 * vs * n_fs */
    rs += vmax;                 /* (2 * vs * n_fs) + v_max */
    rs /= vmax;                 /* ((2 * vs * n_fs) + v_max) / v_max */
    rs >>= 1;                   /* ((2 * vs * n_fs) + v_max) / (2 * v_max) */
    if ((rs == 0) && (vs != 0))
        rs = 1;

    return rs;
}

uint32_t getglid(gl_data_t *p)
{
	switch (p->protocol) {
		case 'M':	return p->id;
	    case 'X':   return p->id + 0x4000;
	    case 'N':   return p->id + 0xC000;
	}
	return 0;
}

/*	evaluates index to glstate table,
	returns a NULL ptr if invalid, else a ptr to an index or to 0 if free
*/
uint16_t * getGLIndex(bus_t bus, uint32_t locid, char prot)
{
	if ((bus <= num_buses) && (gl[bus].gldir != NULL)) {
		if ((locid < MAXSRCPGL) && prot) gl[bus].gldir->proto[locid] = prot;

		uint32_t id = locid & 0x3FFF;
        if (id) switch(locid >> 14) {
        	case 0:	// srcp all protocols
        			if (locid < MAXSRCPGL) switch(gl[bus].gldir->proto[locid]) {
						case 'M':	return &(gl[bus].gldir->mmdir[id]);
						case 'N':   return &(gl[bus].gldir->nmradir[id]);
						case 'X':   return &(gl[bus].gldir->mfxdir[id]);
					}
					if (prot && (prot != 'N')) break;	// else fallthru
			case 3:	// DCC internal
			case 7:	// DCC via MCS
					if (id < MAXNMRAGL) return &(gl[bus].gldir->nmradir[id]);
        	        break;
			case 4:	// MM via MCS
					if (id < MAXMMGL) return &(gl[bus].gldir->mmdir[id]);
                    break;
        	case 1:	// mfx via srcp-x
					if (prot && (prot != 'X')) break;	// else fallthru
			case 5:	// mfx via MCS
					if (id < MAXMFXGL) return &(gl[bus].gldir->mfxdir[id]);
		}
	}
	return NULL;	// invalid parameters
}

/* 	search free entry and return its index
*/
uint16_t addGLEntry(bus_t bus, uint32_t id)
{
    uint16_t entry = 0;
    do {
		if (++entry > gl[bus].numberOfGl) {
			syslog_bus(bus, DBG_WARN, "No free entry available for id %x", id);
			return 0;
		}
	} while (gl[bus].glstate[entry].state != glsNone);

	syslog_bus(bus, DBG_DEBUG, "New GL entry %d added for id %x", entry, id);
	return entry;	//(id & 0x3FFF);
}

/* checks whether a GL is already initialized or not
 * returns loc-id if OK, or 0 if it is an invalid address!
*/
uint16_t isInitializedGL(bus_t busnumber, int index)
{
    if (busnumber <= num_buses && gl[busnumber].numberOfGl > 0 &&
		index > 0 && index <= gl[busnumber].numberOfGl &&
        (gl[busnumber].glstate[index].state != glsNone)) {
        	uint16_t i = gl[busnumber].glstate[index].id;	// check kind of locid
   			if ((i >= MAXSRCPGL) || (gl[busnumber].gldir->proto[i] !=
			   				gl[busnumber].glstate[index].protocol)) {
        		if (gl[busnumber].glstate[index].protocol == 'X') i += 0x4000;
			}
// printf("idx %d -> ID %x\n", index, i);
        	return i;
    }
    else return 0;
}

/* Take new locomotive data and make some checks.
 * Lock is ignored! Lock is payed attention to in SRCP procedures, here not
 * necessary (emergency stop)*/

// TODO: brauchen wir die Defaultinitialisierung?
static void gl_enqueue(bus_t busnumber, gl_data_t * p)
{
    syslog_bus(busnumber, DBG_DEBUG, "gl_info enqueued for addr %d", p->id);
	if (buses[busnumber].debuglevel >= DBG_DEBUG)
			debugGL(busnumber, p);

    int result;
	int addr = p->id;

    if (p->state == glsNone) {
        cacheInitGL(busnumber, addr, 'P', 1, 14, 1, NULL);
        syslog_bus(busnumber, DBG_WARN, "GL default init for %d-%d",
                       busnumber, addr);
   		if (p->state == glsInit)  p->state = glsActive;
    }

    if (queue_isfull(busnumber)) {
        syslog_bus(busnumber, DBG_WARN, "GL command queue full");
        return;
    }

    result = pthread_mutex_lock(&queue_mutex[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

	/* avoid double queue entries */
    int i = out[busnumber];
    while (i != in[busnumber]) {
      	if (queue[busnumber][i] == p) goto enqueue_unlock;
       	i++;
        if (i == QUEUELEN) i = 0;
    }

    /* copy pointer to new values to queue */
    queue[busnumber][in[busnumber]] = p;
    in[busnumber]++;
    if (in[busnumber] == QUEUELEN) in[busnumber] = 0;

enqueue_unlock:
    result = pthread_mutex_unlock(&queue_mutex[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                       "pthread_mutex_unlock() failed: %s (errno = %d).",
                       strerror(result), result);
    }

    /* Restart thread to send GL command */
    resume_bus_thread(busnumber);
}

int queue_GL_isempty(bus_t busnumber)
{
    return (in[busnumber] == out[busnumber]);
}

static int queue_len(bus_t busnumber)
{
    if (in[busnumber] >= out[busnumber])
        return in[busnumber] - out[busnumber];
    else
        return QUEUELEN + in[busnumber] - out[busnumber];
}

/* maybe, 1 element in the queue cannot be used.. */
static int queue_isfull(bus_t busnumber)
{
    return queue_len(busnumber) >= QUEUELEN - 1;
}

/** result is pointer to next item or NULL, updates fifo pointer! */

gl_data_t * dequeueNextGL(bus_t busnumber)
{
    if (in[busnumber] == out[busnumber])
        return NULL;

    gl_data_t *p  = queue[busnumber][out[busnumber]];
    out[busnumber]++;
    if (out[busnumber] == QUEUELEN)
        out[busnumber] = 0;
    return p;
}

// TODO: check if this procedure can be removed
int cacheGetGL(bus_t busnumber, int addr, gl_data_t *gld)
{
    if (isInitializedGL(busnumber, addr)) {
        *gld = gl[busnumber].glstate[addr];
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

/**
 * cacheSetGL is called from the hardware drivers to keep the
 * the data and the info mode informed. It is called from
 * within the SRCP SET Command code.
 * It respects the TERM function.
*/
void cacheSetGL(bus_t busnumber, gl_data_t *glp, gl_data_t *l)
{
   	char msg[1000];
   	int rc = SRCP_WRONGVALUE;
   	int addr = glp->id; // getglid(glp);     //isInitializedGL(busnumber, i);
    if ((addr >= MAXSRCPGL) || (gl[busnumber].gldir->proto[addr] == 0)) addr = getglid(glp);
   	glp->cacheddirection = l->direction;
    glp->cachedspeed = l->speed;
    gettimeofday(&glp->tv, NULL);
    if (glp->state == glsTerm) {
       	rc = SRCP_INFO;
        snprintf(msg, sizeof(msg), "%lu.%.3lu 102 INFO %lu GL %d\n",
                glp->tv.tv_sec, glp->tv.tv_usec / 1000, busnumber, addr);
        // delete all data of terminated entry
        uint16_t *gli = getGLIndex(busnumber, getglid(glp) | MCSADDRINDICATOR, 0);
        *gli = 0;
        bzero(glp, sizeof(gl_data_t));
		if (addr < MAXSRCPGL) gl[busnumber].gldir->proto[addr] = 0;
    }
    else {
        rc = cacheInfoGL(busnumber, addr, msg);
        if (glp->speedchange) gl_enqueue(busnumber, glp);   // remaining action
    }
    if (rc == SRCP_INFO) enqueueInfoMessage(msg);
}

int cacheInitGL(bus_t busnumber, uint32_t locid, const char protocol,
                int protoversion, int n_fs, int n_func, char *optData)
{
    int rc = SRCP_WRONGVALUE;
    gl_data_t *p = NULL;
    uint16_t *gli = getGLIndex(busnumber, locid, protocol);
    if (gli) {
    	if (*gli == 0) *gli = addGLEntry(busnumber, locid);
    	if (*gli) {
    		char msg[1000];
    		p = &gl[busnumber].glstate[*gli];
    		if (p->state == glsNone) {			// new item to be considered
		        rc = bus_supports_protocol(busnumber, protocol);
				if (rc != SRCP_OK) return rc;
        		gettimeofday(&p->inittime, NULL);
        		p->tv = p->inittime;
        		p->n_fs = n_fs ? n_fs : 14;
        		p->n_func = n_func;
        		p->protocolversion = protoversion;
        		p->protocol = protocol;
        		p->id = locid & 0x3FFF;
        		if (buses[busnumber].init_gl_func)
            		rc = (*buses[busnumber].init_gl_func) (p, optData);
		        if (rc == SRCP_OK) {
					p->state = glsInit;
					gl_enqueue(busnumber, p);	// take into refresh cycle
				}
            }
            else rc = SRCP_DEVICEREINITIALIZED;

            if (cacheDescribeGL(busnumber, p->id, msg) == SRCP_INFO)
            		enqueueInfoMessage(msg);
        }
    }
    return rc;
}

int cacheTermGL(bus_t busnumber, uint32_t locid)
{
    uint16_t *gli = getGLIndex(busnumber, locid, 0);
    int i = gli ? *gli : 0;
    if (i && (gl[busnumber].glstate[i].state >= glsInit)) {
        gl[busnumber].glstate[i].state = glsTerm;
        //Bei MFX muss eine Abmeldung bis zum Lokdekoder
        if (gl[busnumber].glstate[i].protocol == 'X') {
          enqueueGL(busnumber, locid, -1, 0, 1, 0); //Direction -1 -> Info Term
        }
        else {
          enqueueGL(busnumber, locid, 0, 0, 1, 0);
        }
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

/*
 * RESET a GL to its defaults, TODO: check why nowhere used
 */
int resetGL(bus_t busnumber, int addr)
{
    if (isInitializedGL(busnumber, addr)) {
        enqueueGL(busnumber, addr, 0, 0, 1, 0);
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}


void cacheCleanGL(bus_t bus)
{
    for (unsigned int i = 1; i <= gl[bus].numberOfGl; i++) {
//        bzero(&gl[bus].glstate[i], sizeof(gl_data_t));
// TODO: mixture of original and Danis code, what to use?
      	gl[bus].glstate[i].state = glsNone;
    }
    if (gl[bus].gldir) bzero(gl[bus].gldir, sizeof(gl_dir_t));
    if (buses[bus].init_func)  	(*buses[bus].init_func) (bus);
}


int cacheDescribeGL(bus_t busnumber, uint32_t locid, char *msg)
{
    uint16_t *gli = getGLIndex(busnumber, locid, 0);
    int i = gli ? *gli : 0;
    if (i && (gl[busnumber].glstate[i].state >= glsInit)) {
        sprintf(msg, "%lu.%.3lu 101 INFO %lu GL %d %c %d %d %d",
                gl[busnumber].glstate[i].inittime.tv_sec,
                gl[busnumber].glstate[i].inittime.tv_usec / 1000,
                busnumber, locid,
				gl[busnumber].glstate[i].protocol,
                gl[busnumber].glstate[i].protocolversion,
                gl[busnumber].glstate[i].n_fs,
                gl[busnumber].glstate[i].n_func);
        // add bus specific data and newline
        if (buses[busnumber].describe_gl_func) {
          (*buses[busnumber].describe_gl_func) (&(gl[busnumber].glstate[i]), msg);
        }
        sprintf(msg + strlen(msg), "\n");
     }
    else {
        strcpy(msg, "");
        return SRCP_NODATA;
    }
    return SRCP_INFO;
}

int cacheInfoGL(bus_t busnumber, uint32_t locid, char *msg)
{
    uint16_t *gli = getGLIndex(busnumber, locid, 0);
    int i = gli ? *gli : 0;
    if (i && (gl[busnumber].glstate[i].state == glsActive)) {
    	char *tmp;
        sprintf(msg, "%lu.%.3lu 100 INFO %lu GL %d %d %d %d %d",
                gl[busnumber].glstate[i].tv.tv_sec,
                gl[busnumber].glstate[i].tv.tv_usec / 1000,
                busnumber, locid,
				gl[busnumber].glstate[i].direction,
                gl[busnumber].glstate[i].speed,
                gl[busnumber].glstate[i].n_fs,
                (gl[busnumber].glstate[i].funcs & 0x01) ? 1 : 0);

        for (int f = 1; f < gl[busnumber].glstate[i].n_func; f++) {
            tmp = malloc(strlen(msg) + 100);
            sprintf(tmp, "%s %d", msg,
                    ((gl[busnumber].glstate[i].
                      funcs >> f) & 0x01) ? 1 : 0);
            strcpy(msg, tmp);
            free(tmp);
        }
        tmp = malloc(strlen(msg) + 2);
        sprintf(tmp, "%s\n", msg);
        strcpy(msg, tmp);
        free(tmp);

        return SRCP_INFO;
    }
    return SRCP_NODATA;
}

// TODO: check if we will really support LOCK feature
//		if yes the following procedures have to be adapted to extended addr !

/* has to use a semaphore, must be atomized! */
int cacheLockGL(bus_t busnumber, int addr, long int duration,
                sessionid_t sessionid)
{
    if (isInitializedGL(busnumber, addr)) {
        if (gl[busnumber].glstate[addr].locked_by == sessionid
            		|| gl[busnumber].glstate[addr].locked_by == 0) {
            char msg[256];
            gl[busnumber].glstate[addr].locked_by = sessionid;
            gl[busnumber].glstate[addr].lockduration = duration;
            gettimeofday(&gl[busnumber].glstate[addr].locktime, NULL);
            describeLOCKGL(busnumber, addr, msg);
            enqueueInfoMessage(msg);
            return SRCP_OK;
        }
        else {
            return SRCP_DEVICELOCKED;
        }
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

int cacheGetLockGL(bus_t busnumber, uint32_t locid, sessionid_t * session_id)
{
//    if (isInitializedGL(busnumber, addr)) {
    uint16_t *gli = getGLIndex(busnumber, locid, 0);
    int i = gli ? *gli : 0;
    if (i && (gl[busnumber].glstate[i].state >= glsInit)) {
        *session_id = gl[busnumber].glstate[i].locked_by;
        return SRCP_OK;
    }
    else return SRCP_WRONGVALUE;
}

int describeLOCKGL(bus_t bus, int addr, char *reply)
{
    if (isInitializedGL(bus, addr)) {

        sprintf(reply, "%lu.%.3lu 100 INFO %lu LOCK GL %d %ld %lu\n",
                gl[bus].glstate[addr].locktime.tv_sec,
                gl[bus].glstate[addr].locktime.tv_usec / 1000,
                bus, addr, gl[bus].glstate[addr].lockduration,
                gl[bus].glstate[addr].locked_by);
        return SRCP_OK;
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

int cacheUnlockGL(bus_t busnumber, int addr, sessionid_t sessionid)
{
    if (isInitializedGL(busnumber, addr)) {

        if (gl[busnumber].glstate[addr].locked_by == sessionid
            || gl[busnumber].glstate[addr].locked_by == 0) {
            char msg[256];
            gl[busnumber].glstate[addr].locked_by = 0;
            gettimeofday(&gl[busnumber].glstate[addr].locktime, NULL);
            sprintf(msg, "%lu.%.3lu 102 INFO %lu LOCK GL %d %lu\n",
                    gl[busnumber].glstate[addr].locktime.tv_sec,
                    gl[busnumber].glstate[addr].locktime.tv_usec / 1000,
                    busnumber, addr, sessionid);
            enqueueInfoMessage(msg);
            return SRCP_OK;
        }
        else {
            return SRCP_DEVICELOCKED;
        }
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

/**
 * called when a session is terminating
 */
void unlock_gl_bysessionid(sessionid_t sessionid)
{
    bus_t i;

    syslog_session(sessionid, DBG_DEBUG, "Unlocking GLs by session-id");
    for (i = 0; i <= num_buses; i++) {
        int number = getMaxAddrGL(i);
        for (int j = 1; j <= number; j++) {
            if (gl[i].glstate[j].locked_by == sessionid) {
                cacheUnlockGL(i, j, sessionid);
            }
        }
    }
}

/**
 * called once per second to unlock
 */
void unlock_gl_bytime(void)
{
    bus_t i;

    for (i = 0; i <= num_buses; i++) {
        int number = getMaxAddrGL(i);
        for (int j = 1; j <= number; j++) {
            if (gl[i].glstate[j].lockduration > 0
                && gl[i].glstate[j].lockduration-- == 1) {
                cacheUnlockGL(i, j, gl[i].glstate[j].locked_by);
            }
        }
    }
}

/**
 * First initialisation after program start up
 */
void startup_GL(void)
{
    bus_t i;

    for (i = 0; i < MAX_BUSES; i++) {
        in[i] = 0;
        out[i] = 0;
        gl[i].numberOfGl = 0;
        gl[i].glstate = NULL;
        gl[i].gldir = NULL;

        int result = pthread_mutex_init(&queue_mutex[i], NULL);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_init() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

/*destroy all occupied mutexes*/
void shutdown_GL()
{
    for (bus_t i = 0; i < MAX_BUSES; i++) {
        free(gl[i].glstate);
        free(gl[i].gldir);
        int result = pthread_mutex_destroy(&queue_mutex[i]);
        if (result != 0) {
            syslog_bus(0, DBG_ERROR,
                       "pthread_mutex_destroy() failed: %s (errno = %d).",
                       strerror(result), result);
        }
    }
}

/**
 * allocates memory to hold all the data
 * called from the configuration routines
 */
int init_GL(bus_t busnumber, unsigned int count)
{
    syslog_bus(busnumber, DBG_INFO, "init GL: %d", count);
    if (busnumber >= MAX_BUSES)
        return 1;

    if (count > 0) {
        gl[busnumber].glstate = malloc((count + 1) * sizeof(gl_data_t));
        if (gl[busnumber].glstate == NULL) return 1;
        gl[busnumber].numberOfGl = count;
        for (unsigned int i = 0; i <= count; i++) {
            bzero(&gl[busnumber].glstate[i], sizeof(gl_data_t));
        }
        gl[busnumber].gldir = malloc(sizeof(gl_dir_t));
        if (gl[busnumber].gldir == NULL) return 1;
        bzero(gl[busnumber].gldir, sizeof(gl_dir_t));
    }
    return 0;
}

void debugGL(bus_t busnumber, gl_data_t *glp)
{
	syslog_bus(busnumber, DBG_WARN,
		"*** GLSTATE for %d/%x: state %d, protocol %c, protocolversion %d",
		busnumber, glp->id, glp->state, glp->protocol, glp->protocolversion);
    syslog_bus(busnumber, DBG_WARN,
		"* n_func %d, n_fs %d speed %d, direction %d, funcs %x, UID %x",
		glp->n_func, glp->n_fs, glp->speed, glp->direction, glp->funcs, glp->decuid);

//      syslog_bus(busnumber, DBG_WARN, "lockduration %ld",
//                 glp->lockduration);
//      syslog_bus(busnumber, DBG_WARN, "locked_by %ld", glp->locked_by);
        /*  struct timeval tv;
           struct timeval inittime;
           struct timeval locktime; */
}

// Helpers for the mcs-Gateway
static gl_data_t * get_gldata_ptr(bus_t bus, uint32_t locid)
{
	gl_data_t *p = NULL;
    uint16_t *gli = getGLIndex(bus, locid, 0);
    if (gli) {
    	if (*gli == 0) *gli = addGLEntry(bus, locid);
    	if (*gli) {
        	p = &gl[bus].glstate[*gli];
        	if (p->state == glsNone) {			// new item to be considered
        		memset(p, 0, sizeof(gl_data_t));
        		p->id = locid & 0x3FFF;
        		switch(locid >> 14) {
					case 4:	p->protocol = 'M'; 	// MM
        					p->protocolversion = 2;
							p->n_fs = 14;
            				p->n_func = 5;
							break;
					case 5:	p->protocol = 'X';	// mfx
        					p->protocolversion = 1;
							p->n_fs = 128;
            				p->n_func = 16;
							break;
					case 7:	p->protocol = 'N';	// DCC
        					p->protocolversion = 1;
							p->n_fs = 28;
            				p->n_func = 16;
							break;
				}
				// TODO: was muss sonst noch initialisiert werden ?    z.B.
				gettimeofday(&p->inittime, NULL);
				p->tv = p->inittime;
        		if (bus_supports_protocol(bus, p->protocol) == SRCP_OK) {
        			if (buses[bus].init_gl_func) {
    					if ((*buses[bus].init_gl_func) (p, "") == SRCP_OK) {
                            p->cacheddirection = p->direction;
							p->state = glsActive;
                        }
        			}
            	}
            	if (p->state == glsNone) p = NULL;	// init failed
    			else syslog_bus(bus, DBG_INFO, "data initialized for %x to %c %d NFS %d at %p",
						locid, p->protocol, p->protocolversion, p->n_fs, p);
			}
		}
    }
    return p;
}

// Interface for the mcs-Gateway
// parms and return values not available are set to -1

int handle_mcs_prot(bus_t bus, uint32_t locid, int val)
{
    syslog_bus(bus, DBG_DEBUG, "mcs_prot for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid | MCSADDRINDICATOR);
    if (p == NULL) return -1;
    switch (p->protocol) {
    	case 'N':	switch (val) {
    			case 0: p->protocolversion = 1;	p->n_fs = 28;	break;
				case 1:	p->protocolversion = 1;	p->n_fs = 14;	break;
				case 2:	p->protocolversion = 1;	p->n_fs = 126;	break;
				case 3: p->protocolversion = 2;	p->n_fs = 28;	break;
				case 4:	p->protocolversion = 2;	p->n_fs = 126;	break;
				default:	return -2;
					}
					break;
		default:	return -2;
	}
//	TODO: needed?	gl_enqueue(bus, p);
	return val;
}

int handle_mcs_speed(bus_t bus, uint32_t locid, int val)
{
//  syslog_bus(bus, DBG_DEBUG, "mcs_speed for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid | MCSADDRINDICATOR);
    if (p == NULL) return -1;
    if (val >= 0) {
    	uint8_t oldspeed = p->speed;
		p->speed = calcspeed(val, 1000, p->n_fs);
		if (p->speed != oldspeed) {
        	p->speedchange |= SCSPEED;
			gl_enqueue(bus, p);
		}
	}
    else val = (p->speed * 1000) / p->n_fs;	// TODO: mal irgendwas: calcspeed umgekehrt!
	return val;
}

/* val:	0 = no change, 1 = forward, 2 = backward, 3 = change, 4 = emergency */
int handle_mcs_dir(bus_t bus, uint32_t locid, int val)
{
//  syslog_bus(bus, DBG_DEBUG, "mcs_dir for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid | MCSADDRINDICATOR);
    if (p == NULL) return -1;
    if (val >= 0) {
		switch (val) {
			case 1:	p->direction = 1;	break;	// forward
			case 2:	p->direction = 0;	break;	// backward
			case 3: p->direction ^= 1;	break;	// change
			case 4:	p->direction = 2;
                    p->speedchange |= SCEMERG; break;	// emergency
		}
		if (p->direction != p->cacheddirection) {
            p->speed = 0;
            p->speedchange |= SCDIREC;
        }
		gl_enqueue(bus, p);
	}
	val = 2 - p->direction;
	return val;
}

int handle_mcs_func(bus_t bus, uint32_t locid, int funr, int val)
{
//  syslog_bus(bus, DBG_DEBUG, "mcs_func for %d, f%d set to %d", locid, funr, val);
    gl_data_t *p = get_gldata_ptr(bus, locid | MCSADDRINDICATOR);
    if (p == NULL) return -1;
    if (val >= 0) {
        uint32_t mask = 1 << funr;
        uint32_t oldval = (p->funcs & mask) ? 1 : 0;
        if (val) p->funcs |= mask;
        else    p->funcs &= ~mask;
        if (val != oldval) {
        	p->funcschange |= mask;
			gl_enqueue(bus, p);
		}
	}
    else val = (p->funcs >> funr) & 1;
	return val;
}

int enqueueGL(bus_t bus, uint32_t locid, int dir, int speed, int maxspeed, const int f)
{
	int i, oldspeed, oldfuncs;
	uint32_t itemid;
	char sync[5];

	syslog_bus(bus, DBG_DEBUG, "srcp_info for %d set", locid);
    gl_data_t *p = get_gldata_ptr(bus, locid);
    if (p == NULL) return SRCP_WRONGVALUE;

	if (p->direction != dir) {
        p->direction = dir;
        p->speedchange |= SCDIREC;
        if (dir == 2) p->speedchange |= SCEMERG;  // emergency
    }

   	oldspeed = p->speed;
	p->speed = calcspeed(speed, maxspeed, p->n_fs);
	if (p->speed != oldspeed)  	p->speedchange |= SCSPEED;

    oldfuncs = p->funcs;
    p->funcs = f;
    oldfuncs ^= f;				// changed funcs
    p->funcschange |= oldfuncs;

   	if (p->state == glsInit) {
        p->cacheddirection = p->direction;
        p->state = glsActive;
    }
	gl_enqueue(bus, p);

    itemid = getglid(p);
	sync[0] = 1;	sync[1] = 2 - p->direction;
	if (sync[1]) {
		if (info_mcs(bus, 0x0B, itemid, sync) < 0) return SRCP_OK;
	} else {
		sync[1] = 3;			// emergency
		if (info_mcs(bus, 0x01, itemid, sync) < 0) return SRCP_OK;
	}

	i = (p->speed * 1000) / p->n_fs;
	sync[0] = 2;	sync[1] = i >> 8; 	sync[2] = i & 0xFF;
	info_mcs(bus, 0x09, itemid, sync);

	for (i = 0; i < p->n_func; i++){
		if (oldfuncs & (1 << i)) {
			sync[1] = i;	sync[2] = (f >> i) & 1;
			info_mcs(bus, 0x0D, itemid, sync);
		}
	}
	return SRCP_OK;
}
