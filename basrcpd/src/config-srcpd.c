// config_srcpd.c - adapted for basrcpd project 2018 by Rainer Müller 
/*
* This software is published under the terms of the GNU General Public
* License, Version 2, 1991. (c) Matthias Trute, 2000-2001.
*
* 04.07.2001 Frank Schmischke
*            Introducing configuration file
* 05.08.2001 Matthias Trute
*            changed to XML format
* 16.05.2005 Gerard van der Sel
*            addition of Selectrix
*/


#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <netdb.h>

#include "config.h"
#include "config-srcpd.h"
#include "srcp-fb.h"
#include "srcp-power.h"
#include "srcp-server.h"
#include "srcp-error.h"

#ifdef USE_DDL
#include "ddl.h"
#endif

#ifdef USE_DDLS88
#include "ddl-s88.h"
#endif

#ifdef USE_HSI88
#include "hsi-88.h"
#endif

#ifdef USE_I2C
#include "i2c-dev.h"
#endif

#ifdef USE_IB
#include "ib.h"
#endif

#ifdef USE_LI100
#include "li100.h"
#endif

#ifdef USE_LOCONET
#include "loconet.h"
#endif

#ifdef USE_LOOPBACK
#include "loopback.h"
#endif

#ifdef USE_M605X
#include "m605x.h"
#endif

#ifdef USE_SELECTRIX
#include "selectrix.h"
#endif

#ifdef USE_ZIMO
#include "zimo.h"
#endif

#ifdef USE_XBEE
#include "xbee.h"
#endif

#ifdef USE_CANBUS
#include "canbus.h"
#endif

#include "syslogmessage.h"


static const char DISABLE_MSG[] =
    "\"%s\" has been disabled at compile time.\n";


/* check if a bus has a device group or not */
int bus_has_devicegroup(bus_t bus, int dg)
{
    switch (dg) {
        case DG_SESSION:
            return strstr(buses[bus].description, "SESSION") != NULL;
        case DG_POWER:
            return strstr(buses[bus].description, "POWER") != NULL;
        case DG_GA:
            return strstr(buses[bus].description, "GA") != NULL;
        case DG_GL:
            return strstr(buses[bus].description, "GL") != NULL;
        case DG_GM:
            return strstr(buses[bus].description, "GM") != NULL;
        case DG_FB:
            return strstr(buses[bus].description, "FB") != NULL;
        case DG_SM:
            return strstr(buses[bus].description, "SM") != NULL;
        case DG_SERVER:
            return strstr(buses[bus].description, "SERVER") != NULL;
        case DG_TIME:
            return strstr(buses[bus].description, "TIME") != NULL;
        case DG_LOCK:
            return strstr(buses[bus].description, "LOCK") != NULL;
        case DG_DESCRIPTION:
//          return strstr(buses[bus].description, "DESCRIPTION") != NULL;
            return (buses[bus].description[0] != 0);
    }
    return 0;
}

/** 
  check if a given bus supports a specified protocol
  @par Input: bus_t busnumber the bus
  @par Input: const char protocol the protocol
  @return SRCP_OK i.e. the protocol is found, else SRCP Error code */
int bus_supports_protocol(bus_t busnumber, const char protocol)
{
    size_t i;

    if (buses[busnumber].protocols) {
        char const *protocols = buses[busnumber].protocols;
        for (i = 0; i < strlen(protocols); i++) {
            if (protocols[i] == protocol) {
                return SRCP_OK;
            }
        }
    }
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}

static bus_t register_bus(bus_t busnumber, xmlDocPtr doc, xmlNodePtr node)
{
    int result;
    bus_t current_bus = busnumber;

    if (xmlStrcmp(node->name, BAD_CAST "bus"))
        return busnumber;

    if (busnumber >= MAX_BUSES) {
        syslog_bus(0, DBG_ERROR,
                   "Sorry, you have used an invalid bus number (%ld). "
                   "If this is greater than or equal to %d,\n"
                   "you need to recompile the sources.\n",
                   busnumber, MAX_BUSES);
        return busnumber;
    }

    /* some default values */
    buses[current_bus].debuglevel = DBG_INFO;
    buses[current_bus].flags = 0;
    buses[current_bus].protocols = 0;

    /* Function pointers to NULL */
    buses[current_bus].thr_func = NULL;
    buses[current_bus].thr_timer = NULL;
    buses[current_bus].sigio_reader = NULL;
    buses[current_bus].init_func = NULL;
    buses[current_bus].init_gl_func = NULL;
    buses[current_bus].describe_gl_func = NULL;
    buses[current_bus].init_ga_func = NULL;
    buses[current_bus].init_fb_func = NULL;

    /* Communication port to default values */
    buses[current_bus].devicetype = HW_UNDEFINED;
    buses[current_bus].device.file.path = NULL;

    /* Definition of thread synchronisation  */
    /*TODO: this should be (privately) moved to each bus */
    result = pthread_mutex_init(&buses[current_bus].transmit_mutex, NULL);
    if (result != 0) {
        syslog_bus(current_bus, DBG_ERROR,
                   "pthread_mutex_init() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_cond_init(&buses[current_bus].transmit_cond, NULL);
    if (result != 0) {
        syslog_bus(current_bus, DBG_ERROR,
                   "pthread_cond_init() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    xmlNodePtr child = node->children;
    xmlChar *txt = NULL;
    xmlChar *txt2 = NULL;

    while (child != NULL) {

        if ((xmlStrncmp(child->name, BAD_CAST "text", 4) == 0) ||
            (xmlStrncmp(child->name, BAD_CAST "comment", 7) == 0)) {
            /* just do nothing, it is only formatting text or a comment */
        }

        else if (xmlStrncmp(child->name, BAD_CAST "server", 6) == 0) {
            if (busnumber == 0)
                busnumber += readconfig_server(doc, child, busnumber);
            else
                syslog_bus(0, DBG_ERROR,
                           "Sorry, type=server is not allowed "
                           "at bus %ld!\n", busnumber);
        }

        else if (xmlStrcmp(child->name, BAD_CAST "ddl") == 0) {
#ifdef USE_DDL
            busnumber += readconfig_DDL(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "ddl-s88") == 0) {
#ifdef USE_DDLS88
#if defined(linux) || defined(__CYGWIN__) || defined(__FreeBSD__)
            busnumber += readconfig_DDL_S88(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR,
                       "Sorry, DDL-S88 not (yet) available on "
                       "this system.\n");
#endif
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "hsi-88") == 0) {
#ifdef USE_HSI88
            busnumber += readConfig_HSI_88(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "i2c-dev") == 0) {
#ifdef USE_I2C
#ifdef linux
            busnumber += readconfig_I2C_DEV(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, "Sorry, I2C-DEV is only available on "
                       "Linux (yet).\n");
#endif
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "intellibox") == 0) {
#ifdef USE_IB
            busnumber += readConfig_IB(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "li100usb") == 0) {
#ifdef USE_LI100
            busnumber += readConfig_LI100_USB(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "li100") == 0) {
#ifdef USE_LI100
            busnumber += readConfig_LI100_SERIAL(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "loconet") == 0) {
#ifdef USE_LOCONET
            busnumber += readConfig_LOCONET(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "loopback") == 0) {
#ifdef USE_LOOPBACK
            busnumber += readconfig_LOOPBACK(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "m605x") == 0) {
#ifdef USE_M605X
            busnumber += readconfig_m605x(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        else if (xmlStrcmp(child->name, BAD_CAST "selectrix") == 0) {
#ifdef USE_SELECTRIX
            busnumber += readconfig_Selectrix(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }
        else if (xmlStrcmp(child->name, BAD_CAST "zimo") == 0) {
#ifdef USE_ZIMO
            busnumber += readconfig_ZIMO(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }
        else if (xmlStrcmp(child->name, BAD_CAST "dccar") == 0) {
#ifdef USE_DCCAR
            busnumber += readconfig_DCCAR(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }
        else if (xmlStrcmp(child->name, BAD_CAST "xbee") == 0) {
#ifdef USE_XBEE
            busnumber += readconfig_XBEE(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }
        else if (xmlStrcmp(child->name, BAD_CAST "canbus") == 0) {
#ifdef USE_CANBUS
            busnumber += readconfig_CANBUS(doc, child, busnumber);
#else
            syslog_bus(0, DBG_ERROR, DISABLE_MSG, child->name);
#endif
        }

        /* some attributes are common for all (real) buses */
        else if (xmlStrcmp(child->name, BAD_CAST "device") == 0) {
            txt2 = xmlGetProp(child, BAD_CAST "type");
            if (txt2 == NULL || xmlStrcmp(txt2, BAD_CAST "filename") == 0) {
                buses[current_bus].devicetype = HW_FILENAME;
            }
            else if (xmlStrcmp(txt2, BAD_CAST "network") == 0) {
                buses[current_bus].devicetype = HW_NETWORK;
            }
            else {
                syslog_bus(0, DBG_WARN, "WARNING, \"%s\" (bus %ld) is an "
                           "unknown device property!\n", txt2,
                           current_bus);
            }
            free(txt2);
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                switch (buses[current_bus].devicetype) {
                    case HW_FILENAME:
                        free(buses[current_bus].device.file.path);
                        buses[current_bus].device.file.path =
                            malloc(strlen((char *) txt) + 1);
                        strcpy(buses[current_bus].device.file.path,
                               (char *) txt);
                        break;
                    case HW_NETWORK:
                        free(buses[current_bus].device.file.path);
                        buses[current_bus].device.net.hostname =
                            malloc(strlen((char *) txt) + 1);
                        strcpy(buses[current_bus].device.net.hostname,
                               (char *) txt);
                        txt2 = xmlGetProp(child, BAD_CAST "port");
                        if (txt2 != NULL) {
                            free(buses[current_bus].device.net.port);
                            buses[current_bus].device.net.port =
                                malloc(strlen((char *) txt2) + 1);
                            strcpy(buses[current_bus].device.net.port,
                                   (char *) txt2);
                            free(txt2);
                        }
                        else {
                            buses[current_bus].device.net.port = NULL;
                        }
                        txt2 = xmlGetProp(child, BAD_CAST "protocol");
                        if (txt2 != NULL) {
                            struct protoent *p;
                            p = getprotobyname((char *) txt2);
                            buses[current_bus].device.net.protocol =
                                p->p_proto;
                            free(txt2);
                        }
                        else {
                            buses[current_bus].device.net.protocol = 6; /* TCP */
                        }
                        break;
                }
                xmlFree(txt);
            }
            switch (buses[current_bus].devicetype) {
                case HW_FILENAME:
                    syslog_bus(current_bus, DBG_DEBUG, "** Filename='%s'",
                               buses[current_bus].device.file.path);
                    break;
                case HW_NETWORK:
                    syslog_bus(current_bus, DBG_DEBUG,
                               "** Network Host='%s', Protocol=%d Port=%s",
                               buses[current_bus].device.net.hostname,
                               buses[current_bus].device.net.protocol,
                               buses[current_bus].device.net.port);
                    break;
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "verbosity") == 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                int verbosity = atoi((char *) txt);
                if ((verbosity < 0) || (verbosity > DBG_DEBUG)) {
                    syslog_bus(current_bus, DBG_ERROR,
                               "Invalid verbosity value found: %d; using %d instead",
                               verbosity, DBG_DEBUG);
                    verbosity = DBG_DEBUG;
                }
                buses[current_bus].debuglevel = verbosity;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "use_watchdog") == 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    buses[current_bus].flags |= USE_WATCHDOG;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "restore_device_settings")
                 == 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    buses[current_bus].flags |= RESTORE_COM_SETTINGS;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "auto_power_on") == 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    buses[current_bus].flags |= AUTO_POWER_ON;
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "speed") == 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                int speed = atoi((char *) txt);

                switch (speed) {
                    case 2400:
                        buses[current_bus].device.file.baudrate = B2400;
                        break;
                    case 4800:
                        buses[current_bus].device.file.baudrate = B4800;
                        break;
                    case 9600:
                        buses[current_bus].device.file.baudrate = B9600;
                        break;
                    case 19200:
                        buses[current_bus].device.file.baudrate = B19200;
                        break;
                    case 38400:
                        buses[current_bus].device.file.baudrate = B38400;
                        break;
                    case 57600:
                        buses[current_bus].device.file.baudrate = B57600;
                        break;
                    case 115200:
                        buses[current_bus].device.file.baudrate = B115200;
                        break;
                    default:
                        buses[current_bus].device.file.baudrate = B2400;
                        break;
                }
                xmlFree(txt);
            }
        }

        else if (xmlStrcmp(child->name, BAD_CAST "auto_speed_detection") ==
                 0) {
            txt = xmlNodeListGetString(doc, child->children, 1);
            if (txt != NULL) {
                if (xmlStrcmp(txt, BAD_CAST "yes") == 0)
                    buses[current_bus].flags |= USE_AUTODETECTION;
                else if (xmlStrcmp(txt, BAD_CAST "no") == 0)
                    buses[current_bus].flags &= ~USE_AUTODETECTION;
                xmlFree(txt);
                syslog_bus(0, DBG_ERROR,
                        "Setting speed_detection for bus: %ld\n", current_bus);
            }
        }

        else
            syslog_bus(0, DBG_ERROR,
                       "WARNING, \"%s\" (bus %ld) is an unknown tag!\n",
                       child->name, current_bus);

        child = child->next;
    }
    return busnumber;
}

/*walk through xml tree and return number of found buses*/
static bus_t walk_config_xml(xmlDocPtr doc)
{
    bus_t bus = 0;
    xmlNodePtr root, child;

    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        syslog_bus(0, DBG_ERROR, "Error, no XML document root found.\n");
        return bus;
    }
    child = root->children;

    while (child != NULL) {
        bus = register_bus(bus, doc, child);
        num_buses = bus - 1;
        child = child->next;
    }
    return bus;
}

/*read configuration file and return success value if some bus was found*/
int readConfig(char *filename)
{
    xmlDocPtr doc;
    bus_t rb = 0;

    /* something to initialize */
    memset(buses, 0, sizeof(buses));
    num_buses = 0;

    /* some defaults */
    syslog_bus(0, DBG_DEBUG, "Parsing %s", filename);
    doc = xmlParseFile(filename);

    /* always show a message */
    if (doc != NULL) {
        syslog_bus(0, DBG_DEBUG, "Walking %s", filename);
        rb = walk_config_xml(doc);
        syslog_bus(0, DBG_DEBUG, "Done %s; found %ld buses", filename, rb);
        xmlFreeDoc(doc);
        /*
         *Free the global variables that may
         *have been allocated by the parser.
         */
        xmlCleanupParser();
    }
    else {
        syslog_bus(0, DBG_ERROR,
                   "Error, no XML document tree found parsing %s.\n",
                   filename);
    }
    return (rb > 0);
}

/**
 * suspend_bus_thread: Holds the thread until a resume command is given.
        The bus thread waits in this routine.
 * @param busnumber
       bus_t given the bus which thread has to be stopped.
 */
void suspend_bus_thread(bus_t busnumber)
{
    int result;

    syslog_bus(busnumber, DBG_DEBUG, "Bus thread is going to stop.");

    /* Lock thread till new data to process arrives */
    result = pthread_mutex_lock(&buses[busnumber].transmit_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_cond_wait(&buses[busnumber].transmit_cond,
                               &buses[busnumber].transmit_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_cond_wait() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    /* mutex released.       */
    result = pthread_mutex_unlock(&buses[busnumber].transmit_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    syslog_bus(busnumber, DBG_DEBUG, "Bus thread is working again.");
}

/**
 * resume_bus_thread: continue a stopped thread
 * @param busnumber
       bus_t given the bus which thread has to be stopped.
 */
void resume_bus_thread(bus_t busnumber)
{
    int result;
    /* Let thread process a feedback */
    result = pthread_mutex_lock(&buses[busnumber].transmit_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_cond_signal(&buses[busnumber].transmit_cond);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_cond_signal() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_mutex_unlock(&buses[busnumber].transmit_mutex);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }
    syslog_bus(0, DBG_DEBUG, "Thread on bus %d is woken up", busnumber);
}

/*create all bus threads*/
void create_all_bus_threads()
{
    pthread_t ttid_tid;
    int result;
    bus_t i;

    syslog_bus(0, DBG_INFO, "Starting %ld bus interface threads.",
               num_buses);

    /* start threads for all buses */
    for (i = 1; i <= num_buses; i++) {
        syslog_bus(0, DBG_INFO,
                   "Starting interface thread number %ld (type = %d).",
                   i, buses[i].type);

        if (buses[i].thr_timer != NULL) {
            result = pthread_create(&ttid_tid, NULL, buses[i].thr_timer,
                                    (void *) i);
            if (result != 0) {
                syslog(LOG_INFO, "Create timer thread for bus %ld "
                       "failed: %s (errno = %d)\n", i,
                       strerror(result), result);
                exit(EXIT_FAILURE);
            }
            buses[i].tidtimer = ttid_tid;
        }

        if (buses[i].thr_func != NULL) {
            result = pthread_create(&ttid_tid, NULL, buses[i].thr_func,
                                    (void *) i);
            if (result != 0) {
                syslog(LOG_INFO, "Create interface thread for bus %ld "
                       "failed: %s (errno = %d)\n", i,
                       strerror(result), result);
                exit(EXIT_FAILURE);
            }
            buses[i].tid = ttid_tid;
        }

        syslog_bus(i, LOG_INFO, "Interface thread started successfully "
                   "(type =%d, tid = %u)", buses[i].type,
                   (unsigned int) (buses[i].tid));

        if (((buses[i].flags & AUTO_POWER_ON) == AUTO_POWER_ON)) {
            setPower(i, 1, "AUTO POWER ON");
        }
        else {
            setPower(i, 0, "AUTO POWER OFF");
        }
    }
}

/*terminate all running bus threads*/
void cancel_all_bus_threads()
{
    bus_t bus;
    int result = 0;
    void *thr_result;

    for (bus = 1; bus <= num_buses; bus++) {

        if (buses[bus].tidtimer != 0) {
            result = pthread_cancel(buses[bus].tidtimer);
            if (result != 0)
                syslog_bus(bus, DBG_ERROR,
                           "Timer thread cancel failed: %s (errno = %d).",
                           strerror(result), result);

            /*wait until timer thread terminates */
            result = pthread_join(buses[bus].tidtimer, &thr_result);
            if (result != 0)
                syslog_bus(bus, DBG_ERROR,
                           "Timer thread join failed: %s (errno = %d).",
                           strerror(result), result);
        }

        result = pthread_cancel(buses[bus].tid);
        if (result != 0)
            syslog_bus(bus, DBG_ERROR,
                       "Interface thread cancel failed: %s (errno = %d).",
                       strerror(result), result);

        /*wait until thread terminates */
        result = pthread_join(buses[bus].tid, &thr_result);
        if (result != 0)
            syslog_bus(bus, DBG_ERROR,
                       "Interface thread join failed: %s (errno = %d).",
                       strerror(result), result);

        syslog_bus(bus, DBG_INFO, "Bus successfully cancelled.");
    }
}

/* activate watchdog if necessary */
void run_bus_watchdog()
{
    bus_t bus;
    int result;
    pthread_t ttid_tid;

    for (bus = 1; bus <= num_buses; bus++) {
        if ((buses[bus].flags & USE_WATCHDOG)
            && buses[bus].watchdog == 0 && !queue_GL_isempty(bus)
            && !queue_GA_isempty(bus)) {
            syslog_bus(bus, DBG_ERROR, "Oops: Interface thread "
                       "hangs, restarting (old tid = %ld, %d).",
                       (long) buses[bus].tid, buses[bus].watchdog);

            result = pthread_cancel(buses[bus].tid);
            if (result != 0)
                syslog_bus(bus, DBG_ERROR,
                           "Interface thread cancel failed: %s (errno = %d).",
                           strerror(result), result);

            result = pthread_join(buses[bus].tid, NULL);
            if (result != 0)
                syslog_bus(bus, DBG_ERROR,
                           "Interface thread join failed: %s (errno = %d).",
                           strerror(result), result);

            result = pthread_create(&ttid_tid, NULL,
                                    buses[bus].thr_func, (void *) bus);
            if (result != 0) {
                syslog(LOG_INFO, "Recreate interface thread "
                       "failed: %s (errno = %d)\n",
                       strerror(result), result);
                break;
            }
            buses[bus].tid = ttid_tid;
        }
        buses[bus].watchdog = 0;
    }
}
