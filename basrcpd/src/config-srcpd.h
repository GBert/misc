// config_srcpd.h - adapted for basrcpd project 2018 by Rainer Müller 

/***************************************************************************
                          config_srcpd.h  -  description
                             -------------------
    begin                : Wed Jul 4 2001
    copyright            : (C) 2001 by Dipl.-Ing. Frank Schmischke
    email                : frank.schmischke@t-online.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  Changes:                                                               *
 *    26.10.2006 Frank Schmischke                                          *
 *               split LI100 to LI100-SERIAL and LI100-USB                 *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/

#ifndef _CONFIG_SRCPD_H
#define _CONFIG_SRCPD_H

#include <termios.h>
#include <syslog.h>

/*due to cross references, this types are defined here*/
typedef unsigned long int bus_t;
typedef unsigned long int sessionid_t;

#include "srcp-gl.h"
#include "srcp-ga.h"


#define MAX_BUSES               8 /* max number of integrated buses in srcpd */
#define MAXSRCPLINELEN       1001 /* max number of bytes per line plus 0x00 */

/* srcp bus types */
#define SERVER_SERVER           0    /* server */
#define SERVER_DDL              1    /* DDL-server */
#define SERVER_M605X            2    /* M605X-server */
#define SERVER_IB               3    /* IB-server */
#define SERVER_LI100_SERIAL     4    /* Lenz-server with serial interface */
#define SERVER_LI100_USB        5    /* Lenz-server with usb-interface */
#define SERVER_LOOPBACK         6    /* dummy driver, no real hardware */
#define SERVER_S88              7    /* S88 at parallelport */
#define SERVER_HSI_88           8    /* HSI88-server */
#define SERVER_I2C_DEV          9    /* I2C-DEV-server */
#define SERVER_ZIMO            10    /* Zimo MX1 */
#define SERVER_SELECTRIX       11    /* Selectrix-server */
#define SERVER_LOCONET         12    /* Loconet Gateway */
#define SERVER_DCCAR           13    /* DC-Car Gateway */
#define SERVER_XBEE            14    /* Digi XBee Modules */
#define SERVER_CANBUS          15    /* CANBUS Gateway */

/* generic flags */
#define USE_WATCHDOG          0x0001 /* use watchdog */
#define AUTO_POWER_ON         0x0002 /* start Power on startup */
#define RESTORE_COM_SETTINGS  0x0004 /* restore com-port settings after close */
#define USE_AUTODETECTION     0x0008 /* use com-port baudrate auto detection */

/* driver specific flags */
#define FB_ORDER_0            0x0200 /* feedback port 0 is bit 0 */
#define FB_16_PORTS           0x0400 /* feedback-modul has 16 ports */
#define FB_4_PORTS            0x0800 /* used for Lenz, sending 2x4 ports instead 8 at once */

/* useful constants */
#define HW_UNDEFINED 0
#define HW_FILENAME  1
#define HW_NETWORK   2

/*basic data for bus threads*/
typedef struct bt {
  int fd;
  bus_t bus;
} bus_thread_t;

typedef enum {devNONE = 0, devOK, devFAIL} DeviceState;

/* bus data struct */
typedef struct _BUS
{
  int debuglevel;  /* verbosity level of syslog */
  int type;        /* which bustype */
  char *protocols; /* array of supported protocols */
  char description[100]; /* bus description */

  int devicetype;  /* file, network, constants start with HW_ */
  union {
      struct {
        char *path;    /* Path to device, if not null */
        int fd;        /* file descriptor */
        speed_t baudrate;
        struct termios devicesettings; /* save device settings, if used */
      } file;
      struct {
        char *hostname;  /* DNS resolvable hostname */
        int protocol;    /* Values are taken from /etc/protocols: 6==tcp */
        char *port;      /* if using tcp or udp: portnumber or servicename to connect to. */
        int sockfd;      /* socket descriptor */
      } net;
  } device;
  DeviceState devicestate;
  /* thread synchronisation variables */
  pthread_mutex_t transmit_mutex;
  pthread_cond_t transmit_cond;

  pthread_t tid;               /* ID of the thread */
  pthread_t tidtimer;          /* ID of the timer thread */
  void *thr_func;              /* address of the thread function */
  void *thr_timer;             /* address of the timer thread */
  void (*sigio_reader)(bus_t); /* address of the SIGIO based device reader */
  int (*init_func)(bus_t);             /* address of init function */
  int (*init_gl_func) (gl_data_t *, char *);  /* called to check default init */
  int (*init_ga_func) (ga_data_t *);  /* called to check default init */
  int (*init_fb_func) (bus_t bus, int addr,
          const char protocolb, int index);  /* called to check default init */
  void (*describe_gl_func) (gl_data_t *, char *);  /* called to check default init */

  int watchdog;                /* watchdog to monitor bus thread */

  /* Power management on the track */
  int power_state;
  int power_changed;
  struct timeval power_change_time;
  char power_msg[100];
  /* driver specific */
  void *driverdata;       /* pointer to driverspecific data */
  int flags;              /* Watchdog flag */
} bus_data_t;

bus_data_t buses[MAX_BUSES];
bus_t num_buses;

int readConfig(char *filename);
void run_bus_watchdog();
void create_all_bus_threads();
void cancel_all_bus_threads();
void suspend_bus_thread(bus_t bus);
void resume_bus_thread(bus_t bus);
int bus_supports_protocol(bus_t bus, const char protocol);

#define DG_SESSION 1
#define DG_TIME 2
#define DG_GA 3
#define DG_GL 4
#define DG_FB 5
#define DG_SM 6
#define DG_LOCK 7
#define DG_DESCRIPTION 8
#define DG_SERVER 9
#define DG_POWER 10
#define DG_GM 11

int bus_has_devicegroup(bus_t bus, int dg);

#endif
