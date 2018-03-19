
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "config-srcpd.h"
#include "srcp-gl.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "syslogmessage.h"

#define QUEUELEN 200 //SIG 29.10.16, 50 sind zuwenig!

/* current state */
static struct _GL gl[MAX_BUSES];

/* command queues for each bus */
typedef struct _tQueueGL {
    int out, in;
    gl_data_t queue[QUEUELEN];
} tQueueGL;
typedef struct _tQueue2GL {
    tQueueGL queue[2];  //Extra Queue für Lok Halt Kommandos, diese haben Priorität und können andere überholen. 0=normal, 1=priorität
} tQueue2GL;
static tQueue2GL queue[MAX_BUSES];
#define GET_QUEUE_PRIO(prio) ((prio) ? 1 : 0)

static pthread_mutex_t queue_mutex[MAX_BUSES];

/* write position for queue writers */
static int out[MAX_BUSES], in[MAX_BUSES];

/* forward declaration of internal functions */
// TODO: braucht man das noch?    static int queue_len(bus_t busnumber);
static int queue_isfull(tQueueGL *q);
static int queue_len_one(tQueueGL *q);

/**
 * isValidGL: checks if a given address could be a valid GL.
 * returns true or false. false, if not all requirements are met.
 */
int isValidGL(bus_t busnumber, int addr)
{
    /* in bus 0 GL are not allowed */
    /* only num_buses are configured */
    /* number of GL is set */
    /* address must be greater 0 */
    /* but not more than the maximum address on that bus */
    if (busnumber <= num_buses &&
        gl[busnumber].numberOfGl > 0 && addr > 0 &&
        addr <= gl[busnumber].numberOfGl) {
        return 1 == 1;
    }
    else {
        return 1 == 0;
    }
}

/**
 * getMaxAddrGL: returns the maximum Address for GL on the given bus
 * returns: <0: invalid bus number
            =0: no GL on that bus
      >0: maximum address
 */
unsigned int getMaxAddrGL(bus_t busnumber)
{
    if (busnumber <= num_buses) {
        return gl[busnumber].numberOfGl;
    }
    else {
        return 0;
    }
}

/* es gibt Decoder fr 14, 27, 28 und 128 FS */
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

/* checks whether a GL is already initialized or not
 * returns false even, if it is an invalid address!
 */
bool isInitializedGL(bus_t busnumber, int addr)
{
    if (isValidGL(busnumber, addr)) {
        return (gl[busnumber].glstate[addr].state == 1);
    }
    else {
        return false;
    }
}

static void mutex_unlock(bus_t busnumber, pthread_mutex_t *mutex) {
    int result;
    result = pthread_mutex_unlock(mutex);
    if (result != 0) {
      syslog_bus(busnumber, DBG_ERROR,
                 "pthread_mutex_unlock() failed: %s (errno = %d).",
                 strerror(result), result);
    }
}

/* Uebernehme die neuen Angaben für die Lok, einige wenige Prüfungen.
   Lock wird ignoriert! Lock wird in den SRCP Routinen beachtet, hier
   ist das nicht angebracht (Notstop)
*/

int enqueueGLorig(bus_t busnumber, int addr, int dir, int speed, int maxspeed,
              const int f)
{
    int result;
    struct timeval akt_time;

    if (isValidGL(busnumber, addr)) {
        if (!isInitializedGL(busnumber, addr)) {
            cacheInitGL(busnumber, addr, 'P', 1, 14, 1, NULL);
            syslog_bus(busnumber, DBG_WARN, "GL default init for %d-%d",
                       busnumber, addr);
        }

        result = pthread_mutex_lock(&queue_mutex[busnumber]);
        if (result != 0) {
            syslog_bus(busnumber, DBG_ERROR,
                       "pthread_mutex_lock() failed: %s (errno = %d).",
                       strerror(result), result);
        }

        //Priorität ist dann vorhanden, wenn die Lok noch fährt und nun angehalten werden soll.
        bool prio = (gl[busnumber].glstate[addr].speed > 0) && (speed == 0);
        tQueueGL *q = &(queue[busnumber].queue[GET_QUEUE_PRIO(prio)]);
        if (queue_isfull(q)) {
            //Wenn es die Prio Queue war können wir es noch mit der normalem probieren, besser als Kommando ganz verwerfen.
            if (prio) {
              q = &(queue[busnumber].queue[GET_QUEUE_PRIO(false)]);
              if (queue_isfull(q)) {
                syslog_bus(busnumber, DBG_WARN, "GL Command Queue double full");
                mutex_unlock(busnumber, &queue_mutex[busnumber]);
                return SRCP_TEMPORARILYPROHIBITED;
              }
            }
            else {
              syslog_bus(busnumber, DBG_WARN, "GL Command Queue full");
              mutex_unlock(busnumber, &queue_mutex[busnumber]);
              return SRCP_TEMPORARILYPROHIBITED;
            }
        }
        else if (prio) {
          //Kommando wird der Prio Queue hinzugefügt
          //Wenn in der normalen Queue nun ein Kommandof für die selbe Lok ist wurde dieses überholt und ist nicht mehr gültig
          tQueueGL *qNormal = &(queue[busnumber].queue[GET_QUEUE_PRIO(false)]);
          int i = qNormal -> out;
          while (i != qNormal -> in) {
            if (qNormal -> queue[i].id == addr) {
              //Nun nicht mehr gültiger, überholter, Eintrag gefunden
              qNormal -> queue[i].id = -1;
            }
            i++;
            if (i == QUEUELEN) {
              i = 0;
            }
          }
          //Wenn die Queue zu 75% voll ist und es schon ein Prio Halt Kommando für die selbe Lok hat muss dieses hier nicht auch noch dazu -> verwerfen
          //75% Regel: wenn keine Überlast dürfen mehrere Halt Komnados da sein (z.B. mit Schaltung verschiedener Funktionen).
          if (queue_len_one(q) > (QUEUELEN * 3 / 4)) {
            i = q -> out;
            while (i != q -> in) {
	      if (q -> queue[i].id == addr) {
	        //Schon ein Prio Stop Kommando vorhanden -> neues verwerfen
                mutex_unlock(busnumber, &queue_mutex[busnumber]);
                return SRCP_OK;
	      }
	      i++;
              if (i == QUEUELEN) {
                i = 0;
              }
	    }
	  }
        }
        
        /* Protokollbezeichner und sonstige INIT Werte in die Queue kopieren! */
        (q -> queue[q -> in]).prio = prio;
        (q -> queue[q -> in]).protocol =
            gl[busnumber].glstate[addr].protocol;
        (q -> queue[q -> in]).protocolversion =
            gl[busnumber].glstate[addr].protocolversion;

        (q -> queue[q -> in]).speed =
            calcspeed(speed, maxspeed, gl[busnumber].glstate[addr].n_fs);

        (q -> queue[q -> in]).n_fs =
            gl[busnumber].glstate[addr].n_fs;

        (q -> queue[q -> in]).n_func =
            gl[busnumber].glstate[addr].n_func;

        (q -> queue[q -> in]).direction = dir;
        (q -> queue[q -> in]).funcs = f;
        gettimeofday(&akt_time, NULL);
        (q -> queue[q -> in]).tv = akt_time;
        (q -> queue[q -> in]).id = addr;
        q -> in++;
        if (q -> in == QUEUELEN) {
            q -> in = 0;
        }

        mutex_unlock(busnumber, &queue_mutex[busnumber]);

        /* Restart thread to send GL command */
        resume_bus_thread(busnumber);
        return SRCP_OK;
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

int queue_GL_isempty(bus_t busnumber)
{
    return (queue[busnumber].queue[0].in == queue[busnumber].queue[0].out) &&
           (queue[busnumber].queue[1].in == queue[busnumber].queue[1].out);
}

static int queue_len_one(tQueueGL *q)
{
    if (q ->in >= q->out)
        return q->in - q->out;
    else
        return QUEUELEN + q->in - q->out;
}
/* TODO: braucht man das noch?
static int queue_len(bus_t busnumber)
{
    return queue_len_one(&queue[busnumber].queue[0]) + queue_len_one(&queue[busnumber].queue[1]);
}	*/

/* maybe, 1 element in the queue cannot be used.. */
static int queue_isfull(tQueueGL *q)
{
    return queue_len_one(q) >= QUEUELEN - 1;
}

/** liefert nächsten Eintrag oder -1, setzt fifo pointer neu! */
int dequeueNextGL(bus_t busnumber, gl_data_t * l)
{
    if (queue_GL_isempty(busnumber)) {
        return -1;
    }

    //Zuerst Prio Queue
    tQueueGL *q = &queue[busnumber].queue[GET_QUEUE_PRIO(true)];
    if (queue_len_one(q) == 0) {
      //Keine Prio Kommandos -> normale Queue kommt drann
      q = &queue[busnumber].queue[GET_QUEUE_PRIO(false)];
    }
    *l = q -> queue[q -> out];
    q -> out++;
    if (q -> out == QUEUELEN) {
        q -> out = 0;
    }
    return q -> out;
}

int cacheGetGL(bus_t busnumber, int addr, gl_data_t * l)
{
    if (isInitializedGL(busnumber, addr)) {
        *l = gl[busnumber].glstate[addr]; 
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
int cacheSetGL(bus_t busnumber, int addr, gl_data_t l)
{
    if (isValidGL(busnumber, addr)) {
        char msg[1000];
      	gl[busnumber].glstate[addr].cacheddirection = l.direction;
      	gl[busnumber].glstate[addr].cachedspeed = l.speed;
//      gl[busnumber].glstate[addr].funcs = l.funcs;
//      gl[busnumber].glstate[addr].n_fs = l.n_fs;
//      gl[busnumber].glstate[addr].n_func = l.n_func;
        gettimeofday(&gl[busnumber].glstate[addr].tv, NULL);
        if (gl[busnumber].glstate[addr].state == 2) {
            snprintf(msg, sizeof(msg), "%lu.%.3lu 102 INFO %ld GL %d\n",
                     gl[busnumber].glstate[addr].tv.tv_sec,
                     gl[busnumber].glstate[addr].tv.tv_usec / 1000,
                     busnumber, addr);
            bzero(&gl[busnumber].glstate[addr], sizeof(gl_data_t));
        }
        else {
            cacheInfoGL(busnumber, addr, msg);
        }
        enqueueInfoMessage(msg);
        return SRCP_OK;
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

int cacheInitGL(bus_t busnumber, int addr, const char protocol,
                int protoversion, int n_fs, int n_func, char *optData)
{
    int rc = SRCP_WRONGVALUE;
    if (isValidGL(busnumber, addr)) {
        char msg[1000];
        gl_data_t tgl;
        memset(&tgl, 0, sizeof(tgl));
        rc = bus_supports_protocol(busnumber, protocol);
        if (rc != SRCP_OK) {
            return rc;
        }
        gettimeofday(&tgl.inittime, NULL);
        tgl.tv = tgl.inittime;
        tgl.n_fs = n_fs;
        tgl.n_func = n_func;
        tgl.protocolversion = protoversion;
        tgl.protocol = protocol;
        tgl.id = addr;
        if (buses[busnumber].init_gl_func) {
            rc = (*buses[busnumber].init_gl_func) (busnumber, &tgl, optData);
        }
        if (rc == SRCP_OK) {
            gl[busnumber].glstate[addr] = tgl;
            gl[busnumber].glstate[addr].state = 1;
            cacheDescribeGL(busnumber, addr, msg);
            enqueueInfoMessage(msg);
            enqueueGL(busnumber, addr, 0, 0, 1, 0);
        }
    }
    else {
    printf("7\n");
        rc = SRCP_WRONGVALUE;
    }
    return rc;
}

//SID, 15.9.08
//Alle GL sate aller Buse werden auf 0 gesetzt -> Lok nicht mehr vorhanden
int resetAllGL(bus_t bus)
{
  int glAddr;
  if (bus > 0 && bus <= num_buses) {
    for (glAddr=1; glAddr<=gl[bus].numberOfGl; glAddr++) {
      gl[bus].glstate[glAddr].state = 0;
    }
    int rc = SRCP_OK;
    if (buses[bus].init_func) {
      rc = (*buses[bus].init_func) (bus);
    }
    return rc;
  }
  return SRCP_WRONGVALUE;
}

int cacheTermGL(bus_t busnumber, int addr)
{
    if (isInitializedGL(busnumber, addr)) {
        gl[busnumber].glstate[addr].state = 2;
        //Bei MFX muss eine Abmeldung bis zum Lokdekoder
        if (gl[busnumber].glstate[addr].protocol == 'X') {
          enqueueGL(busnumber, addr, -1, 0, 1, 0); //Direction -1 -> Info Term
        }
        else {
          enqueueGL(busnumber, addr, 0, 0, 1, 0); //Direction -1 -> Info Term
        }
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }

}

/*
 * RESET a GL to its defaults
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
        bzero(&gl[bus].glstate[i], sizeof(gl_data_t));
    }
}


int cacheDescribeGL(bus_t busnumber, int addr, char *msg)
{
    if (isInitializedGL(busnumber, addr)) {
        sprintf(msg, "%lu.%.3lu 101 INFO %ld GL %d %c %d %d %d",
                gl[busnumber].glstate[addr].inittime.tv_sec,
                gl[busnumber].glstate[addr].inittime.tv_usec / 1000,
                busnumber, addr, gl[busnumber].glstate[addr].protocol,
                gl[busnumber].glstate[addr].protocolversion,
                gl[busnumber].glstate[addr].n_fs,
                gl[busnumber].glstate[addr].n_func);
        //Protokoll / Busspezifische Daten ergänzen
        if (buses[busnumber].describe_gl_func) {
          (*buses[busnumber].describe_gl_func) (&(gl[busnumber].glstate[addr]), msg);
        }
        //Noch Newline anhängen
        sprintf(msg + strlen(msg), "\n");
     }
    else {
        strcpy(msg, "");
        return SRCP_NODATA;
    }
    return SRCP_INFO;
}

int cacheInfoGL(bus_t busnumber, int addr, char *msg)
{
    int i;
    char *tmp;

    if (isInitializedGL(busnumber, addr)) {
        sprintf(msg, "%lu.%.3lu 100 INFO %ld GL %d %d %d %d %d",
                gl[busnumber].glstate[addr].tv.tv_sec,
                gl[busnumber].glstate[addr].tv.tv_usec / 1000,
                busnumber, addr, gl[busnumber].glstate[addr].direction,
                gl[busnumber].glstate[addr].speed,
                gl[busnumber].glstate[addr].n_fs,
                (gl[busnumber].glstate[addr].funcs & 0x01) ? 1 : 0);

        for (i = 1; i < gl[busnumber].glstate[addr].n_func; i++) {
            tmp = malloc(strlen(msg) + 100);
            sprintf(tmp, "%s %d", msg,
                    ((gl[busnumber].glstate[addr].
                      funcs >> i) & 0x01) ? 1 : 0);
            strcpy(msg, tmp);
            free(tmp);
        }
        tmp = malloc(strlen(msg) + 2);
        sprintf(tmp, "%s\n", msg);
        strcpy(msg, tmp);
        free(tmp);

    }
    else {
        return SRCP_NODATA;
    }
    return SRCP_INFO;
}

/* has to use a semaphore, must be atomized! */
int cacheLockGL(bus_t busnumber, int addr, long int duration,
                sessionid_t sessionid)
{
    char msg[256];

    if (isInitializedGL(busnumber, addr)) {
        if (gl[busnumber].glstate[addr].locked_by == sessionid
            || gl[busnumber].glstate[addr].locked_by == 0) {
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

int cacheGetLockGL(bus_t busnumber, int addr, sessionid_t * session_id)
{
    if (isInitializedGL(busnumber, addr)) {

        *session_id = gl[busnumber].glstate[addr].locked_by;
        return SRCP_OK;
    }
    else {
        return SRCP_WRONGVALUE;
    }
}

int describeLOCKGL(bus_t bus, int addr, char *reply)
{
    if (isInitializedGL(bus, addr)) {

        sprintf(reply, "%lu.%.3lu 100 INFO %ld LOCK GL %d %ld %ld\n",
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
            sprintf(msg, "%lu.%.3lu 102 INFO %ld LOCK GL %d %ld\n",
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
    int j;
    int number;

    syslog_session(sessionid, DBG_DEBUG, "Unlocking GLs by session-id");
    for (i = 0; i <= num_buses; i++) {
        number = getMaxAddrGL(i);
        for (j = 1; j <= number; j++) {
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
    int j;
    int number;

    for (i = 0; i <= num_buses; i++) {
        number = getMaxAddrGL(i);
        for (j = 1; j <= number; j++) {
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
    int result;
    bus_t i;

    for (i = 0; i < MAX_BUSES; i++) {
        in[i] = 0;
        out[i] = 0;
        gl[i].numberOfGl = 0;
        gl[i].glstate = NULL;

        result = pthread_mutex_init(&queue_mutex[i], NULL);
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
        if (gl[busnumber].glstate == NULL)
            return 1;
        gl[busnumber].numberOfGl = count;
        for (unsigned int i = 0; i <= count; i++) {
            bzero(&gl[busnumber].glstate[i], sizeof(gl_data_t));
        }
    }
    return 0;
}

void debugGL(bus_t busnumber, int start, int end)
{
    gl_data_t *gld;

//  syslog_bus(busnumber, DBG_WARN, "debug GLSTATE from %d to %d", start, end);
    for (int i = start; i <= end; i++) {
        gld = &gl[busnumber].glstate[i];
        syslog_bus(busnumber, DBG_WARN, "*** GLSTATE for %d/%d state %d",
						busnumber, i, gld->state);
        syslog_bus(busnumber, DBG_WARN, "* id %d, protocol %c, protocolversion %d",
		 				gld->id, gld->protocol, gld->protocolversion);
        syslog_bus(busnumber, DBG_WARN, "* n_func %d, n_fs %d", gld->n_func, gld->n_fs);
        syslog_bus(busnumber, DBG_WARN, "* speed %d, direction %d, funcs %x",
						gld->speed, gld->direction, gld->funcs);
//      syslog_bus(busnumber, DBG_WARN, "lockduration %ld",
//                 gld->lockduration);
//      syslog_bus(busnumber, DBG_WARN, "locked_by %ld", gld->locked_by);
        /*  struct timeval tv;
           struct timeval inittime;
           struct timeval locktime; */
    }
}

// Helpers for the mcs-Gateway
static void gl_enqueue(bus_t bus, gl_data_t * p)
{
    syslog_bus(bus, DBG_INFO, "mcs_info enqueued for %d", p->id);
	// TODO: put pointer p in queue
	debugGL(bus, p->id, p->id); 	// nur zum Test
	enqueueGLorig(bus, p->id, p->direction, p->speed, 0, p->funcs);
}

static gl_data_t * get_gldata_ptr(bus_t bus, uint32_t locid)
{
    int rc = SRCP_WRONGVALUE;
	gl_data_t *p = NULL;
    if (isValidGL(bus, locid & 0x3FFF)) {
        p = &gl[bus].glstate[locid & 0x3FFF];
        if (p->state == 0) {			// new item to be considered
        	memset(p, 0, sizeof(gl_data_t));
        	p->id = locid & 0x3FFF;
        	switch(locid >> 14) {
				case 0:	p->protocol = 'M'; 	// MM
        				p->protocolversion = 2;
						p->n_fs = 14;
            			p->n_func = 5;
						break;
				case 1:	p->protocol = 'X';	// mfx
        				p->protocolversion = 1;
						p->n_fs = 128;
            			p->n_func = 16;
						break;
				case 3:	p->protocol = 'N';	// DCC
        				p->protocolversion = 1;
						p->n_fs = 28;
            			p->n_func = 16;
						break;			
			}
			// TODO: was muss sonst noch initialisiert werden ?
        	if (bus_supports_protocol(bus, p->protocol) == SRCP_OK) {
        		if (buses[bus].init_gl_func) {
            		rc = (*buses[bus].init_gl_func) (bus, p, NULL);
        			if (rc == SRCP_OK)  p->state = 1;
        		}
            }
            if (p->state == 0) p = NULL;	// init failed
    		else syslog_bus(bus, DBG_INFO, "data initialized for %x to %c %d NFS %d",
						locid, p->protocol, p->protocolversion, p->n_fs);
		}
    }
    return p;
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

// Interface for the mcs-Gateway
// parms and return values not available are set to -1

int handle_mcs_prot(bus_t bus, uint32_t locid, int val)
{
    syslog_bus(bus, DBG_INFO, "mcs_prot for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid);  
    if (p == NULL) return -1;
    switch (p->protocol) {
    	case 'N':	switch (val) {
    			case 0: p->protocolversion = 1;	p->n_fs = 28;	break;
				case 1:	p->protocolversion = 1;	p->n_fs = 14;	break;
				case 2:	p->protocolversion = 1;	p->n_fs = 126;	break;
				case 3: p->protocolversion = 2;	p->n_fs = 28;	break;
				case 4:	p->protocolversion = 2;	p->n_fs = 126;	break;
					}
					break;
	}
	gl_enqueue(bus, p);
	return val;
}

int handle_mcs_speed(bus_t bus, uint32_t locid, int val)
{
	int oldspeed;
//  syslog_bus(bus, DBG_INFO, "mcs_speed for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid);
    if (p == NULL) return -1;
    if (val >= 0) {
    	oldspeed = p->speed;
		p->speed = calcspeed(val, 1000, p->n_fs);
		if (p->speed != oldspeed) {
        	p->speedchange |= 1;
			gl_enqueue(bus, p);
		}
	}
    else val = (p->speed * 1000) / p->n_fs;	// TODO: mal irgendwas: calcspeed umgekehrt!
	return val;
}

/* val:	0 = no change, 1 = forward, 2 = backward, 3 = change, 4 = emergency */
int handle_mcs_dir(bus_t bus, uint32_t locid, int val)
{
//  syslog_bus(bus, DBG_INFO, "mcs_dir for %d set to %d", locid, val);
    gl_data_t *p = get_gldata_ptr(bus, locid);  
    if (p == NULL) return -1;
    if (val >= 0) {
		switch (val) {
			case 1:	p->direction = 1;	break;	// forward
			case 2:	p->direction = 0;	break;	// backward
			case 3: p->direction ^= 1;	break;	// change
			case 4:	p->direction = 2;	break;	// emergency
		}
		gl_enqueue(bus, p);
	}
	val = 2 - p->direction;
	return val;
}

int handle_mcs_func(bus_t bus, uint32_t locid, int funr, int val)
{
	int oldval;
	uint32_t mask;
//  syslog_bus(bus, DBG_INFO, "mcs_func for %d, f%d set to %d", locid, funr, val);
    gl_data_t *p = get_gldata_ptr(bus, locid);
    if (p == NULL) return -1;
    if (val >= 0) {
        mask = 1 << funr;
        oldval = (p->funcs & mask) ? 1 : 0;
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

int enqueueGL(bus_t bus, int addr, int dir, int speed, int maxspeed, const int f)
{
	int i, olddir, oldspeed, oldfuncs;
	uint32_t itemid;
	char sync[5];
	
	syslog_bus(bus, DBG_INFO, "srcp_info for %d set", addr);
    gl_data_t *p = get_gldata_ptr(bus, addr);
    if (p == NULL) return SRCP_WRONGVALUE;

    olddir = p->direction;
	p->direction = dir;
	if (p->direction != olddir) p->speedchange |= 2;
	
   	oldspeed = p->speed;
	p->speed = calcspeed(speed, maxspeed, p->n_fs);
	if (p->speed != oldspeed)  	p->speedchange |= 1;

    oldfuncs = p->funcs;
    p->funcs = f;
    oldfuncs ^= f;				// changed funcs
    p->funcschange |= oldfuncs;    
        
	gl_enqueue(bus, p);

    itemid = getglid(p);
	sync[0] = 1;	sync[1] = 2 - p->direction;
	if (info_mcs(bus, 0x0B, itemid, sync) < 0) return SRCP_OK;

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
