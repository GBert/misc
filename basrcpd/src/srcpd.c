// srcpd.c - adapted for basrcpd project 2018 by Rainer Müller 

/***************************************************************************
                          srcpd.c  -  description
                          -----------------------
    begin                : Wed Jul 4 2001
    copyright            : (C) 2001 - 2007 by the srcpd team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>

#include "config-srcpd.h"
#include "netservice.h"
#include "srcp-descr.h"
#include "srcp-fb.h"
#include "srcp-ga.h"
#include "srcp-gl.h"
#include "srcp-info.h"
#include "srcp-server.h"
#include "srcp-session.h"
#include "srcp-time.h"
#include "syslogmessage.h"

#ifndef SYSCONFDIR
	#define SYSCONFDIR "/etc"
#endif

#if defined BANANAPI
    #include <a20hw.h>
    #define HWLIBINIT   a20_init()
    #define HWLIBCLOSE  a20_close()
#elif defined RASPBERRYPI
    #include <bcm2835.h>
    #define HWLIBINIT   bcm2835_init()
    #define HWLIBCLOSE  bcm2835_close()
#endif

#define	MAXFD	64              /* for daemon_init */


/* structures to determine which port needs to be served */
fd_set rfds;
int maxfd;
char conffile[MAXPATHLEN];

extern char PIDFILENAME;


void CreatePIDFile(int pid)
{
    FILE *f;
    f = fopen(&PIDFILENAME, "wb");
    if (f == NULL)
        syslog(LOG_INFO, "Opening pid file '%s' failed: %s (errno = %d)\n",
               &PIDFILENAME, strerror(errno), errno);
    else {
        fprintf(f, "%d\n", pid);
        fflush(f);
        fclose(f);
    }
}

void DeletePIDFile()
{
    int result;
    result = unlink(&PIDFILENAME);
    if (result != 0)
        syslog(LOG_INFO,
               "Unlinking pid file '%s' failed: %s (errno = %d)\n",
               &PIDFILENAME, strerror(errno), errno);
}

/* initialisize all found buses, communication line devices are opened */
void init_all_buses()
{
    bus_t i;

    maxfd = 0;
    FD_ZERO(&rfds);
    for (i = 0; i <= num_buses; i++) {
        if (buses[i].init_func != NULL) {

            /* initialize each bus and report error on failure */
            if ((*buses[i].init_func) (i) != 0) {
                syslog(LOG_INFO, "Initialization of bus %ld failed.", i);
                exit(EXIT_FAILURE);
            }

            /* Configure descriptors for Selectrix module to throw SIGIO */
            if ((buses[i].device.file.fd != -1) &&
                (buses[i].type == SERVER_SELECTRIX)) {
                FD_SET(buses[i].device.file.fd, &rfds);
                maxfd = (maxfd > buses[i].device.file.fd
                         ? maxfd : buses[i].device.file.fd);
                fcntl(buses[i].device.file.fd, F_SETOWN, getpid());
#ifdef linux
                fcntl(buses[i].device.file.fd, F_SETFL, FASYNC);
#endif
            }
        }
    }
}

/*create all kind of server threads*/
void create_all_threads()
{
    set_server_state(ssInitializing);
    create_time_thread();
    create_all_bus_threads();
    create_netservice_thread();
    set_server_state(ssRunning);
    syslog(LOG_INFO, "All threads started");
}

/* cancel all server threads*/
void cancel_all_threads()
{
    int result;

    syslog(LOG_INFO, "Terminating SRCP service...");
    server_shutdown();

    /* if service is going to terminate, first wait 2 seconds,
     * this is according to protocol specification */
    result = sleep(2);
    if (result != 0) {
        syslog_bus(0, DBG_ERROR,
                   "sleep() interrupted, %d seconds left\n", result);
    }

    cancel_netservice_thread();
    cancel_time_thread();
    cancel_all_bus_threads();
    terminate_all_sessions();

    syslog(LOG_INFO, "SRCP service terminated.");
}

/* signal SIGHUP(1) caught */
void sighup_handler(int s)
{
    signal(s, sighup_handler);
    syslog(LOG_INFO, "SIGHUP(1) received, "
           "going to re-read configuration file.");
    cancel_all_threads();
    if (0 == readConfig(conffile)) {
        syslog_bus(0, DBG_ERROR, "Error, no valid bus setup found in "
                   "configuration file. Terminating.\n");
        exit(EXIT_FAILURE);
    }
    init_all_buses();
    create_all_threads();
}

/* signal SIGTERM(15) caught */
void sigterm_handler(int s)
{
    syslog(LOG_INFO, "SIGTERM(15) received! Terminating ...");
    set_server_state(ssTerminating);
}

/** sigio_handler
 * Signal handler for I/O interrupt signal (SIGIO)
 */
void sigio_handler(int status)
{
    struct timeval tv;
    int retval;
    bus_t i;

    /* Don't wait */
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    /* select descriptor that triggered SIGIO */
    retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);

    /* something strange happened, report error */
    if (retval == -1) {
        syslog_bus(0, DBG_ERROR, "Select failed: %s (errno = %d)\n",
                   strerror(errno), errno);
    }

    /* nothing changed */
    else if (retval == 0) {
        return;
    }

    /* find bus matching the triggering descriptor */
    else {
        for (i = 1; i <= num_buses; i++) {
            if ((buses[i].device.file.fd != -1) &&
                (FD_ISSET(buses[i].device.file.fd, &rfds))) {
                if (buses[i].sigio_reader != NULL) {
                    (*buses[i].sigio_reader) (i);
                }
            }
        }
    }
}

void install_signal_handlers()
{
    struct sigaction saio;

    signal(SIGTERM, sigterm_handler);
    signal(SIGHUP, sighup_handler);
    /* important, because write() on sockets should return errors */
    signal(SIGPIPE, SIG_IGN);

    /* Register signal handler for I/O interrupt handling */
    saio.sa_flags = 0;
    saio.sa_handler = &sigio_handler;
    sigemptyset(&saio.sa_mask);
    sigaction(SIGIO, &saio, NULL);

}

/*
 * daemonize process
 * this is from "UNIX Network Programming, W. R. Stevens et al."
 */
int daemon_init(int flag)
{
    int i, result;
    pid_t pid;

    if (!flag) {
        return (0);
    }

    if ((pid = fork()) < 0)
        return (-1);
    else if (pid)
        /* parent terminates */
        _exit(0);

    /* child 1 continues... */

    /* become session leader */
    if (setsid() < 0)
        return (-1);

    signal(SIGHUP, SIG_IGN);
    if ((pid = fork()) < 0)
        return (-1);
    else if (pid)
        /* child 1 terminates */
        _exit(0);

    /* child 2 continues... */

    /* change working directory */
    result = chdir("/");
    if (-1 == result) {
        printf("chdir() failed: %s (errno = %d)\n",
               strerror(errno), errno);
    }

    /* close off file descriptors */
    for (i = 0; i < MAXFD; i++)
        close(i);

    /* redirect stdin, stdout, and stderr to /dev/null */
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    /* success */
    return 0;
}



int main(int argc, char **argv)
{
    int sleep_ctr, c;
    int daemonflag = 1;
    /* First: Init the device data used internally */
    startup_GL();
    startup_GA();
    startup_FB();
    startup_INFO();
    startup_DESCRIPTION();
    startup_TIME();
    startup_SERVER();
    startup_SESSION();

    snprintf(conffile, sizeof(conffile), "%s/srcpd.conf", SYSCONFDIR);

    /* read command line parameters */
    opterr = 0;
    logprint = 0;
    while ((c = getopt(argc, argv, "f:hvnp")) != EOF) {
        switch (c) {
            case 'f':
                if (strlen(optarg) < MAXPATHLEN - 1)
                    strcpy(conffile, optarg);
                break;
            case 'v':
                printf(WELCOME_MSG);
                exit(EXIT_FAILURE);
                break;
            case 'n':
                daemonflag = 0;
                break;
            case 'p':
                logprint = 1;
                break;
            case 'h':
                printf(WELCOME_MSG);
                printf("Usage: srcpd -f <conffile> -v -h\n\n");
                printf("Options:\n");
                printf("  -v  Show program version and quit.\n");
                printf("  -f  Use another config file (default %s).\n",
                       conffile);
                printf("  -n  Do not daemonize at startup.\n");
                printf("  -p  Print log messages on stdout.\n");
                printf("  -h  Show this help text and quit.\n");
                exit(EXIT_FAILURE);
                break;
            default:
                printf("Unknown option: %c\n", c);
                printf("Use: \"srcpd -h\" for help, terminating.\n");
                exit(EXIT_FAILURE);
                break;
        }
    }

    openlog("srcpd", LOG_PID, LOG_USER);
    syslog_bus(0, DBG_INFO, "conffile = \"%s\"\n", conffile);

    if (0 == readConfig(conffile)) {
        syslog_bus(0, DBG_ERROR, "Error, no valid bus setup found in "
                   "configuration file '%s'.\n", conffile);
        exit(EXIT_FAILURE);
    }

    /*daemonize process */
    if (0 != daemon_init(daemonflag)) {
        syslog_bus(0, DBG_ERROR, "Daemonization failed!\n");
        exit(EXIT_FAILURE);
    }

    #ifdef HWLIBINIT
        if (!HWLIBINIT) {
            syslog_bus(0, DBG_ERROR, "PI HWLIBINIT failed!\n");
            exit(EXIT_FAILURE);
        }
    #endif

    CreatePIDFile(getpid());
    syslog(LOG_INFO, "%s", WELCOME_MSG);
    install_signal_handlers();
    init_all_buses();
    create_all_threads();
    sleep_ctr = 10;

    /*
     * Main loop: Wait for _real_ tasks: shutdown, reset and watch for
     * hanging processes
     */
    while (true) {
        if (get_server_state() == ssTerminating)
            break;

        /* wait 100 ms */
        if (usleep(100000) == -1) {
            syslog_bus(0, DBG_ERROR,
                       "usleep() failed in srcpd line %d: %s (errno = %d)",
                       __LINE__, strerror(errno), errno);
        }
        sleep_ctr--;

        if (sleep_ctr == 0) {

            /* clear LOCKs */
            unlock_gl_bytime();
            unlock_ga_bytime();

            run_bus_watchdog();
            sleep_ctr = 10;
        }
    }

    cancel_all_threads();

    if (seteuid(0) != 0) {
        syslog(LOG_INFO, "seteuid() failed: %s (errno = %d)\n",
               strerror(errno), errno);
    }

    shutdown_SESSION();
    shutdown_GA();
    shutdown_GL();

    #ifdef HWLIBCLOSE
        if (!HWLIBCLOSE) {
            syslog_bus(0, DBG_ERROR, "PI HWLIBCLOSE Fail!\n");
        }
    #endif

    DeletePIDFile();
    closelog();
    exit(EXIT_SUCCESS);
}
