/* $Id: srcp-gl.h 1346 2009-06-06 18:15:28Z mtrute $ */

#ifndef _SRCP_GL_H
#define _SRCP_GL_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#include "config-srcpd.h"
#include "srcp-session.h"


/* locomotive decoder */
typedef struct _GLSTATE {
    int state;                  /* 0==dead, 1==living, 2==terminating */
    char protocol;
    int protocolversion;
    int n_func;
    int n_fs;
    int id;                     /* address  */
    int speed;                  /* Sollgeschwindigkeit skal. auf 0..14 */
    int direction;              /* 0/1/2                               */
    int funcs;                  /* Fx..F1, F                           */
    struct timeval tv;          /* Last time of change                 */
    struct timeval inittime;
    struct timeval locktime;
    long int lockduration;
    sessionid_t locked_by;
    bool prio;                  //Priorität ist gesetzt wenn Lok angehalten werden soll (Aktuelle speed>0, neuer speed==0)
    //Protokollabhängige zusätzliche Informationen
    union {
      //Bei MFX Protokoll
      struct {
        uint32_t uid;
        char name[17];
        uint32_t fx[16];
      } mfx;
    } optData; 
} gl_data_t;

typedef struct _GL {
    int numberOfGl;
    gl_data_t *glstate;
} GL;

void startup_GL();
void shutdown_GL();
int init_GL(bus_t busnumber, unsigned int count);
unsigned int getMaxAddrGL(bus_t busnumber);
bool isInitializedGL(bus_t busnumber, int addr);
int isValidGL(bus_t busnumber, int addr);
int enqueueGL(bus_t busnumber, int addr, int dir, int speed,
        int maxspeed, int f);
int queue_GL_isempty(bus_t busnumber);
int dequeueNextGL(bus_t busnumber, gl_data_t * l);
int cacheGetGL(bus_t busnumber, int addr, gl_data_t * l);
int cacheSetGL(bus_t busnumber, int addr, gl_data_t l);
int cacheInfoGL(bus_t busnumber, int addr, char *info);
int cacheDescribeGL(bus_t busnumber, int addr, char *msg);
int cacheInitGL(bus_t busnumber, int addr, const char protocol,
                int protoversion, int n_fs, int n_func, char *optData);
int cacheTermGL(bus_t busnumber, int addr);
void cacheCleanGL(bus_t bus);
int cacheLockGL(bus_t busnumber, int addr, long int duration,
                sessionid_t sessionid);
int cacheGetLockGL(bus_t busnumber, int addr, sessionid_t * sessionid);
int cacheUnlockGL(bus_t busnumber, int addr, sessionid_t sessionid);
void unlock_gl_bysessionid(sessionid_t sessionid);
void unlock_gl_bytime(void);
int describeLOCKGL(bus_t bus, int addr, char *reply);
void debugGL(bus_t busnumber, int start, int end);
int resetAllGL(bus_t bus);

#endif
