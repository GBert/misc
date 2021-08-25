// canbus.c : new bus type CANBUS for the srcpd project
// allows communication srcp <-> canbus
//
// C 2015 Rainer Müller

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <errno.h>
//#include <sys/socket.h>
#include <sys/ioctl.h>

#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>
//#include <unistd.h>

#include "config-srcpd.h"
#include "canbus.h"
#include "toolbox.h"
#include "srcp-fb.h"
#include "srcp-ga.h"
//#include "srcp-gl.h"
//#include "srcp-sm.h"
#include "srcp-power.h"
#include "srcp-server.h"
#include "srcp-info.h"
#include "srcp-error.h"
#include "syslogmessage.h"

#define INVALID_SOCKET -1
#define __canbus ((CANBUS_DATA*)buses[busnumber].driverdata)
#define __canbust ((CANBUS_DATA*)buses[btd->bus].driverdata)

//#define MAX_CV_NUMBER 255


int readconfig_CANBUS(xmlDocPtr doc, xmlNodePtr node, bus_t busnumber)
{
    buses[busnumber].driverdata = malloc(sizeof(struct _CANBUS_DATA));

    if (buses[busnumber].driverdata == NULL) {
        syslog_bus(busnumber, DBG_ERROR,
                   "Memory allocation error in module '%s'.", node->name);
        return 0;
    }

    buses[busnumber].type = SERVER_CANBUS;
    buses[busnumber].init_func = &init_bus_CANBUS;
    buses[busnumber].thr_func = &thr_sendrec_CANBUS;
//    buses[busnumber].init_gl_func = &init_gl_CANBUS;
    buses[busnumber].init_ga_func = &init_ga_CANBUS;
    strcpy(buses[busnumber].description,
           "GA GL FB SM POWER LOCK DESCRIPTION");

    __canbus->number_fb = 0;  /* max 31 */
    __canbus->number_ga = 256;
//    __canbus->number_gl = 80;

    xmlNodePtr child = node->children;
    xmlChar *txt = NULL;

    while (child != NULL) {

        if ((xmlStrncmp(child->name, BAD_CAST "text", 4) == 0) ||
            (xmlStrncmp(child->name, BAD_CAST "comment", 7) == 0)) {
            /* just do nothing, it is only formatting text or a comment */
        }

        else if (xmlStrcmp(child->name, BAD_CAST "number_fb") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __canbus->number_fb = atoi((char *) txt);
                xmlFree(txt);
            }
        }
//        else if (xmlStrcmp(child->name, BAD_CAST "number_gl") == 0) {
//            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
//            if (txt != NULL) {
//                __canbus->number_gl = atoi((char *) txt);
//                xmlFree(txt);
//            }
//        }
        else if (xmlStrcmp(child->name, BAD_CAST "number_ga") == 0) {
            txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
            if (txt != NULL) {
                __canbus->number_ga = atoi((char *) txt);
                xmlFree(txt);
            }
        }

		else if (xmlStrcmp(child->name, BAD_CAST "pause_between_commands") == 0) {
			txt = xmlNodeListGetString(doc, child->xmlChildrenNode, 1);
			if (txt != NULL) {
				__canbus->pause_between_cmd = atoi((char *) txt);
				xmlFree(txt);
		}
	}
        else
            syslog_bus(busnumber, DBG_WARN,
                       "WARNING, unknown tag found: \"%s\"!\n",
                       child->name);;

        child = child->next;
    }

//    if (init_GL(busnumber, __canbus->number_gl)) {
//        __canbus->number_gl = 0;
//        syslog_bus(busnumber, DBG_ERROR,
//                   "Can't create array for locomotives");
//    }

    if (init_GA(busnumber, __canbus->number_ga)) {
        __canbus->number_ga = 0;
        syslog_bus(busnumber, DBG_ERROR,
                   "Can't create array for accessoires");
    }

    if (init_FB(busnumber, __canbus->number_fb)) {
        __canbus->number_fb = 0;
        syslog_bus(busnumber, DBG_ERROR,
                   "Can't create array for feedback");
    }
    return (1);
}


// Initialize socket. Returns false if socket could not be opened.
static bool initSocketCan(bus_t bus)
{
    struct sockaddr_can addr;
    struct ifreq ifr;
    int ret;

	// Configure the socket can layer to report errors, see /linux/can/error.h
    can_err_mask_t err_mask = ( CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF );

    buses[bus].device.file.fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    // Get index for a certain name
    strcpy(ifr.ifr_name, buses[bus].device.file.path);
    ret = ioctl(buses[bus].device.file.fd, SIOCGIFINDEX, &ifr);
    if(ret)
    {
        buses[bus].device.file.fd = INVALID_SOCKET;
        return false;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    ret = bind(buses[bus].device.file.fd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret)
    {
        close(buses[bus].device.file.fd);
        buses[bus].device.file.fd = INVALID_SOCKET;
        return false;
    }

    ret = setsockopt(buses[bus].device.file.fd, SOL_CAN_RAW, CAN_RAW_ERR_FILTER,
               &err_mask, sizeof(err_mask));
    return true;
}


// Close an open connection. Use when changing can net.
void closeSocketCan(bus_t bus)
{
    if(buses[bus].device.file.fd > 0)
    {
        // We don't want to read or write anything more
        shutdown(buses[bus].device.file.fd, SHUT_RDWR);

        close(buses[bus].device.file.fd);
        buses[bus].device.file.fd = INVALID_SOCKET;
    }
}


// Send a message on the CAN-bus. Returns true if ok, false if not.
// Parameters:
// msg - the can message to send
// extended - set to true to send an extended frame
// rtr - set to true to send a remot request (rtr)
// Common errors:
//#define ENETDOWN        100     /* Network is down - use ifconfig up to start */
//#define EAGAIN          11      /* Try again - buffer is full */
//#define EBADF            9      /* Bad file number - can net not opened */
bool sendCanMsg(bus_t bus, struct can_frame msg, bool extended, bool rtr)
{
    int res;

    if(buses[bus].device.file.fd <= 0) return false;

    if(extended) msg.can_id |= CAN_EFF_FLAG;
    if(rtr) msg.can_id |= CAN_RTR_FLAG;

    res = write(buses[bus].device.file.fd, &msg, sizeof(struct can_frame));
    if(res < 0) return false;

    return true;
}


// Check if a CAN message available. If socket is blocking (default) -
// this call will block until data is received or until 1ms timeout period has expired.
// If socket is non blocking, it will return false if there is no data or if there is any error.
// If socket is blocking, it will return false if there is an error or at timeout.
// Common errors:
//#define ENETDOWN        100     /* Network is down - use ifconfig up to start*/
//#define EAGAIN          11      /* Try again - no data available*/
//#define EBADF            9      /* Bad file number - can net not opened */
// timeout - GetMsg will return false after timeout period
bool checkCanMsg(bus_t bus)
{
    int bytesRead, j;
    int ret;
    unsigned int type;
    fd_set rfds;
  	struct can_frame msg;

	struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    // Set up a file descriptor set only containing one socket
    FD_ZERO(&rfds);
    FD_SET(buses[bus].device.file.fd, &rfds);

    // Use select to be able to use a timeout
    ret = select(buses[bus].device.file.fd+1, &rfds, NULL, NULL, &tv);
    if(ret < 0) return false;

    if(ret > 0) {
        bytesRead = read(buses[bus].device.file.fd, &msg, sizeof(msg));

        if(bytesRead == sizeof(msg)) {
            if (msg.can_id & CAN_ERR_FLAG) printf("ERR ");
            if (msg.can_id & CAN_EFF_FLAG) printf("EXT ");
            if (msg.can_id & CAN_RTR_FLAG) printf("RTR ");
    		printf("recvd: %x, DLC %i", msg.can_id & CAN_EFF_MASK, msg.can_dlc);
    		for(j = 0; j < msg.can_dlc; j++) printf(" %x", msg.data[j]);
    		printf("\n");

    		type = (msg.can_id & 0x1FFFFF00) + (msg.can_dlc ? msg.data[0] : 0);
    		switch (type) {
    			case 0x1A1:	case 0x1A2:
    				setFB(bus, (msg.can_id & 0xFF) * 16 + msg.data[1] + 1, type & 1);
    				break;
    		}
    	}
        return true;
    }

    return false;
}


/**
 * initGA: modifies the ga data used to initialize the device
 **/
int init_ga_CANBUS(ga_data_t * ga)
{
    if ((ga->protocol == 'M') || (ga->protocol == 'N')
        || (ga->protocol == 'P') || (ga->protocol == 'S'))
        return SRCP_OK;
    return SRCP_UNSUPPORTEDDEVICEPROTOCOL;
}


/* Initialisiere den Bus, signalisiere Fehler
 * Einmal aufgerufen mit busnummer als einzigem Parameter
 * return code wird ignoriert (vorerst)
 */
int init_bus_CANBUS(bus_t i)
{
    static char *protocols = "LSPMN";

    buses[i].protocols = protocols;

    syslog_bus(i, DBG_INFO, "canbus start initialization (verbosity = %d).",
               buses[i].debuglevel);

    if (initSocketCan(i))
    	syslog_bus(i, DBG_INFO, "canbus initialization done.");
    else
        syslog_bus(i, DBG_ERROR, "Error %d when opening canbus", errno);


	// nur zum Test
    int	j;
    struct can_frame msg;
    msg.can_id = 0x100;
    msg.can_dlc = 8;
    for(j = 0; j < 8; j++) msg.data[j] = j * 10;
	// sendCanMsg(bus_t bus, struct can_frame msg, bool extended, bool rtr, int *errorCode)
	sendCanMsg(i, msg, false, false);


    return 0;
}


static void handle_power_command(bus_t bus)
{
    buses[bus].power_changed = 0;
    char msg[110];

    infoPower(bus, msg);
    enqueueInfoMessage(msg);
    buses[bus].watchdog++;
}


static void handle_ga_command(bus_t busnumber)
{
    ga_data_t gatmp;
    int addr, i;
    struct timeval akt_time;
  	struct can_frame msg;

    dequeueNextGA(busnumber, &gatmp);
    addr = gatmp.id;

    gettimeofday(&akt_time, NULL);
    gatmp.tv[gatmp.port] = akt_time;
    printf("Nr. %i Port %i auf %i\n", addr, gatmp.port, gatmp.action);

    switch (gatmp.port) {
    	case 0:		msg.data[0] = 0x31;	break;	// Stellung R
    	case 1:		msg.data[0] = 0x32;	break;	// Stellung G
    	default:	return;
    }
    msg.can_id = 0x200 + addr / 8;
    msg.can_dlc = 2;
    msg.data[1] = addr & 7;
	sendCanMsg(busnumber, msg, false, false);

    setGA(busnumber, addr, gatmp);

    if (gatmp.action && (gatmp.activetime > 0)) {
        for (i = 0; i < 50; i++) {
            if (__canbus->tga[i].id == 0) {
                gatmp.t = akt_time;
                gatmp.t.tv_sec += gatmp.activetime / 1000;
                gatmp.t.tv_usec += (gatmp.activetime % 1000) * 1000;
                if (gatmp.t.tv_usec > 1000000) {
                    gatmp.t.tv_sec++;
                    gatmp.t.tv_usec -= 1000000;
                }
                __canbus->tga[i] = gatmp;
                break;
            }
        }
    }
    buses[busnumber].watchdog++;
}


/*thread cleanup routine for this bus*/
static void end_bus_thread(bus_thread_t * btd)
{
    int result;

	closeSocketCan(btd->bus);
    syslog_bus(btd->bus, DBG_INFO, "canbus bus terminated.");

    result = pthread_mutex_destroy(&buses[btd->bus].transmit_mutex);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_WARN,
                   "pthread_mutex_destroy() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    result = pthread_cond_destroy(&buses[btd->bus].transmit_cond);
    if (result != 0) {
        syslog_bus(btd->bus, DBG_WARN,
                   "pthread_mutex_init() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    free(buses[btd->bus].driverdata);
    free(btd);
}

/*main thread routine for this bus*/
void *thr_sendrec_CANBUS(void *v)
{
    int addr, ctr, pause;
    struct timeval akt_time, cmp_time;
    ga_data_t gatmp;
    int last_cancel_state, last_cancel_type;

    bus_thread_t *btd = (bus_thread_t *) malloc(sizeof(bus_thread_t));

    if (btd == NULL)
        pthread_exit((void *) 1);
    btd->bus = (bus_t) v;
    btd->fd = -1;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &last_cancel_type);

    /*register cleanup routine */
    pthread_cleanup_push((void *) end_bus_thread, (void *) btd);

    /* initialize tga-structure */
    for (ctr = 0; ctr < 50; ctr++)
        __canbust->tga[ctr].id = 0;

    syslog_bus(btd->bus, DBG_INFO, "canbus bus started (device = %s).",
               buses[btd->bus].device.file.path);

    /*enter endless loop to process work tasks */
    while (true) {

        buses[btd->bus].watchdog = 1;

        /*POWER action arrived */
        if (buses[btd->bus].power_changed == 1)
            handle_power_command(btd->bus);

        /*SM action arrived */
//        if (!queue_SM_isempty(btd->bus))
//            handle_sm_command(btd->bus);

        /* loop shortcut to prevent processing of GA, GL (and FB)
         * without power on; arriving commands will flood the command
         * queue */
        if (buses[btd->bus].power_state == 0) {

            /* wait 1 ms */
            if (usleep(1000) == -1) {
                syslog_bus(btd->bus, DBG_ERROR,
                       "usleep() failed in canbus line %d: %s (errno = %d)",
                       __LINE__, strerror(errno), errno);
            }
            continue;
        }

        /*GL action arrived */
//        if (!queue_GL_isempty(btd->bus))
//            handle_gl_command(btd->bus);


        /* handle delayed switching of GAs (there is a better place) */
        gettimeofday(&akt_time, NULL);
        for (ctr = 0; ctr < 50; ctr++) {
            if (__canbust->tga[ctr].id > 0) {
                cmp_time = __canbust->tga[ctr].t;

                /* switch off time reached? */
                if (cmpTime(&cmp_time, &akt_time)) {
                    gatmp = __canbust->tga[ctr];
                    addr = gatmp.id;
                    gatmp.action = 0;
                    setGA(btd->bus, addr, gatmp);
                    __canbust->tga[ctr].id = 0;
                }
            }
        }

        /*GA action arrived */
        if (pause > 0)	pause--;
        else {
        	if (!queue_GA_isempty(btd->bus)) {
            	handle_ga_command(btd->bus);
            	pause = __canbust->pause_between_cmd;
            }
        }

        /*FB action arrived */
        /* currently nothing to do here */
        buses[btd->bus].watchdog++;

        /* busy wait and continue loop */
        /* wait 1 ms */
        checkCanMsg(btd->bus);
        /*if (usleep(1000) == -1) {
            syslog_bus(btd->bus, DBG_ERROR,
                       "usleep() failed: %s (errno = %d)\n",
                       strerror(errno), errno);
        }*/
    }

    /*run the cleanup routine */
    pthread_cleanup_pop(1);
    return NULL;
}
