// srcp-sm.c - adapted for basrcpd project 2018 by Rainer Müller 

/***************************************************************************
                         srcp-sm.c  -  description
                            -------------------
   begin                : Mon Aug 12 2002
   copyright            : (C) 2002 by Dipl.-Ing. Frank Schmischke
   email                : frank.schmischke@t-online.de
   
   MFX Code:            : daniel@siggsoftware.ch
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
#include <stdio.h>
#include <string.h>

#include "config-srcpd.h"
#include "srcp-error.h"
#include "srcp-info.h"
#include "srcp-session.h"
#include "srcp-sm.h"
#include "syslogmessage.h"


#define QUEUELEN 2

/*
 * Important:
 *   - Only NMRA and MFX is supported at this time
 * NMRA:
 *   - Set address of decoder is only available at programming track
 *   - Every GET/VERIFY only available at programming track
 *   - The answer of GET is delivered via INFO-port!
 *   - Address = 0 or -1, means to use program-track
 *   - Address > 0, means to use POM (only available with CV)
 *
 *   For instance:
 *     SET 1 SM -1 CV 29 2        write 2 into CV 29 at program-track
 *     SET 1 SM 1210 CV 29 2      write 2 into CV 29 of decoder with
 *                                address 1210 on the main-line
 *     SET 1 SM -1 CVBIT 29 1 1   set the 2-nd bit of CV 29 at program-track
 *     SET 1 SM -1 REG 5 2        same as first, but using register-mode
 *
 * MFX:
 *   - Schreiben CV ist immer möglich
 *   - Es wird SET, GET, VERIFY unterstützt (bei GET kein Value im Kommando)
 *   - Lesen CV und schreiben/lesen auf Basis CA ist dann möglich, wenn 
 *     - die Lok auf einem MFX Rückmeldefähigen Booster ist.
 *     - SM MFX Initialisiert ist "INIT x SM MFX"
 *       Achtung: meine MFX Loksuche während aktiviertem SM, mit "TERM x SM MFX" beenden!
 *   Beispiele:
 *     SET 1 SM 10 CVMFX 15 2 3      schreibe 3 an CV 15 Index 2 der Lok 10
 */


/* Command queues pro Bus */
static sm_t queue[MAX_BUSES][QUEUELEN];
static pthread_mutex_t queue_mutex[MAX_BUSES];
static volatile int out[MAX_BUSES], in[MAX_BUSES];

/* internal functions */
static int queue_len(bus_t busnumber);
static int queue_isfull(bus_t busnumber);


/**
 * SM Kommando in Queue zur Ausführung. Wenn ein Paramter für ein Kommando nciht benötigt wird, muss -1 übergeben werden.
 * @param busnumber
 * @param protocol Zu verwendendes Protokoll
 * @param command Auszuführendes SM Kommando (SET, GET etc.)
 * @param type SM Type (CV, CV Bit etc.)
 * @param addr Adresse der Lok. Bei NMRA auf Programmiergleis kann hier -1 übergeben werden (Broadcast),
 *             sonst eine Adresse >0
 * @param caNr Bei allem anderen: CV Nr
 * @param bit_index Bei type==CV_BIT: die Bitnummer 0..7
 *                  Bei type==CV_MFX und CA_MFX: Der Index innerhalb CV Zeile
 * @param value Bei type==CV_BIT 0 oder 1, sondet 0..255 Byte Value zum schreiben oder verify
 */
int enqueueSM(bus_t busnumber, sm_protocol_t protocol, sm_command_t command, sm_type_t type, 
				int addr, int typeaddr, int bit_index, int value)
{
    int result;
    struct timeval akt_time;

    syslog_bus(busnumber, DBG_INFO, "enqueueSM for %i (in = %d, out = %d)",
               addr, in[busnumber], out[busnumber]);
    /* 
     * addr values:
     *    -1: using separate program-track
     *  > -1: programming on the main (only available with CV)
     */

    /* if ((addr == -1) || ((addr > 0)
     *  && (addr <= number_sm) && (type == CV))) */

    if (queue_isfull(busnumber)) {
        syslog_bus(busnumber, DBG_DEBUG, "SM Queue is full");
        return SRCP_TEMPORARILYPROHIBITED;
    }

    result = pthread_mutex_lock(&queue_mutex[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_lock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    queue[busnumber][in[busnumber]].index = bit_index;
    queue[busnumber][in[busnumber]].type = type;
    queue[busnumber][in[busnumber]].value = value;
    queue[busnumber][in[busnumber]].cvaddr = typeaddr;
    queue[busnumber][in[busnumber]].command = command;
    gettimeofday(&akt_time, NULL);
    queue[busnumber][in[busnumber]].tv = akt_time;
    queue[busnumber][in[busnumber]].addr = addr;
    /* SRCP 0.8.x has not yet defined a protocolversion for SM
       which is needed for programming on the main
       (at least for access to the lower 127 addresses of 14-bit addresses)
       defaults to protocol 1 (short addresses)
     */
    queue[busnumber][in[busnumber]].protocolversion = 1;
    queue[busnumber][in[busnumber]].protocol = protocol;
    in[busnumber]++;
    if (in[busnumber] == QUEUELEN)
        in[busnumber] = 0;

    result = pthread_mutex_unlock(&queue_mutex[busnumber]);
    if (result != 0) {
        syslog_bus(busnumber, DBG_ERROR,
                   "pthread_mutex_unlock() failed: %s (errno = %d).",
                   strerror(result), result);
    }

    syslog_bus(busnumber, DBG_INFO, "SM enqueued (in = %d, out = %d)",
               in[busnumber], out[busnumber]);
    return SRCP_OK;
}

int queue_SM_isempty(bus_t busnumber)
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

/** return next entry with rc >=0, or return -1, if no more entries */
int getNextSM(bus_t busnumber, sm_t * l)
{
    if (in[busnumber] == out[busnumber])
        return -1;
    *l = queue[busnumber][out[busnumber]];
    return out[busnumber];
}

/** return next entry or -1, set fifo pointer to new position! */
int dequeueNextSM(bus_t busnumber, sm_t * l)
{
    if (in[busnumber] == out[busnumber])
        return -1;

    *l = queue[busnumber][out[busnumber]];
    out[busnumber]++;
    if (out[busnumber] == QUEUELEN)
        out[busnumber] = 0;
    return out[busnumber];
}

/**
 * SM Kommando ausführen. Wenn ein Paramter für ein Kommando nciht benötigt wird, muss -1 übergeben werden.
 * @param busnumber
 * @param protocol Zu verwendendes Protokoll
 * @param command Auszuführendes SM Kommando (SET, GET etc.)
 * @param type SM Type (CV, CV Bit etc.)
 * @param addr Adresse der Lok. Bei NMRA auf Programmiergleis kann hier -1 übergeben werden (Broadcast),
 *             sonst eine Adresse >0
 * @param caNr Bei allem anderen: CV Nr
 * @param bit_index Bei type==CV_BIT: die Bitnummer 0..7
 *                  Bei type==CV_MFX und CA_MFX: Der Index innerhalb CV Zeile
 * @param value Bei type==CV_BIT 0 oder 1, sondet 0..255 Byte Value zum schreiben oder verify
 * @param info Buffer für Meldungsrückgabe
 */
int infoSM(bus_t busnumber, sm_protocol_t protocol, sm_command_t command, sm_type_t type,
			int addr, int typeaddr, int bit_index, int value, char *info)
{
    int status, result;
    struct timeval now;

    syslog_bus(busnumber, DBG_INFO,
               "SM - TYPE: %d, CV: %d, BIT: %d, VALUE: 0x%02x", type, typeaddr,
               bit_index, value);
    session_lock_wait(busnumber);
    status = enqueueSM(busnumber, protocol, command, type, addr,
												typeaddr, bit_index, value);

    if (session_condt_wait(busnumber, 90, &result) == ETIMEDOUT) {
    	status = SRCP_TIMEOUT;
    }
    else {
        gettimeofday(&now, NULL);
        if (result == -2) {
            status = SRCP_TEMPORARILYPROHIBITED;
		}
        else if ((result == -1) || 
				(command == VERIFY && value != result && type != BIND_MFX)) {
            status = SRCP_WRONGVALUE;
        }
        else {
        	char minfo[6] = { 0 };
			switch (type) {
                case CV:
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d CV %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, typeaddr, result);
                    if (addr < 0) addr = -addr;
					minfo[0] = 3;
					minfo[1] = typeaddr >> 8;
					minfo[2] = typeaddr & 0xFF;
					minfo[3] = result;
                    if (command == SET) {
                        minfo[0] = 4;
                        minfo[4] = 0xC0;
                        info_mcs(busnumber, 0x11, addr | 0xC000, minfo);
                    }
                    else {
                        info_mcs(busnumber, 0x0F, addr | 0xC000, minfo);
                    }
                    break;
                case REGISTER:
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d REG %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, typeaddr, result);
                    break;
                case PAGE:
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d PAGE %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, typeaddr, result);
                    break;
                case CV_BIT:
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d CVBIT %d %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, typeaddr, bit_index, result);
                    break;
                case MM_REG:
                    minfo[0] = 1;	
                    minfo[1] = 33;
                    info_mcs(busnumber, 3, result, minfo);
                    break;
                case BIND_MFX:
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d BIND %u\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, value);
                    if (addr == 0) {			// SET ^ GET x GM 0 BIND
       	                minfo[0] = 3;	minfo[1] = 9;	
						minfo[2] = result >> 8; minfo[3] = result & 0xFF;
                    	info_mcs(busnumber, 1, 0, minfo);
					}
					else { 						// SET ^ GET x GM y BIND
						minfo[0] = 2;	
						minfo[1] = addr >> 8; 	minfo[2] = addr & 0xFF;
						minfo[5] = 5;
						if (command != SET) {	// VERIFY x GM y BIND
							if (result) {	// Amplitude Shift Keying value
								minfo[0] = 3;	minfo[3] = result;	
							}
							minfo[5] = 7;
						}
                    	info_mcs(busnumber, minfo[5], value, minfo);
                    }
                    break;
                case CV_MFX:
					// HACK: dirty fast trial for write config
					if (command == SET) result = value;
                    snprintf(info, MAXSRCPLINELEN,
                            "%lu.%.3lu 100 INFO %lu SM %d CVMFX %d %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, typeaddr, bit_index, result);
					minfo[0] = 4;
					minfo[1] = (typeaddr >> 8) | (bit_index << 2);
					minfo[2] = typeaddr & 0xFF;
					minfo[3] = value;
					minfo[4] = 0x80;	//result;
                    info_mcs(busnumber, 0x11, addr, minfo);
                    break;
        	}
   			if (info[0]) {
				enqueueInfoMessage(info);
			}
		}
    }
    session_unlock_wait(busnumber);
    return status;
}

// Helper for the mcs-Gateway
void handle_mfx_bind_verify(bus_t bus, sm_command_t cmnd, uint32_t val, int addr)
{
	char reply[MAXSRCPLINELEN];
	infoSM(bus, PROTO_MFX, cmnd, BIND_MFX, addr, -1, -1, val, reply);
}

void handle_mcs_config(bus_t bus, sm_command_t command, 
					int uid, int cvaddr, int cvindex, int value, int ctrl)
{
	char reply[MAXSRCPLINELEN];
	sm_protocol_t protocol;
    sm_type_t type;
    int addr = uid & 0x3fff;
	
    switch(uid >> 14) {
		case 0:	protocol = PROTO_MM;
				if (command == GET) goto defmsg;
				type = MM_REG; 
				break;
		case 1:	protocol = PROTO_MFX;
				type = CV_MFX;
				break;
		case 3:	protocol = PROTO_NMRA;
				switch (ctrl & 0x30) {
					case 0x00:	type = CV;
								break; 			
					case 0x10:	type = REGISTER;
								break; 			
					case 0x20:	type = CV_BIT;
								cvindex = value & 7;
								value = (value >> 3) & 1;
								break; 			
					default:	goto defmsg;	
				}
				if ((ctrl & 0x80) == 0) addr = -addr;		// programming track
				break;	
		default:
		defmsg:	syslog_bus(bus, DBG_WARN, 
					"*** Config command for UID %x not supported.", uid);
				return;		
	}
	infoSM(bus, protocol, command, type, addr, cvaddr, cvindex, value, reply);
}

void handle_mcs_discovery(bus_t bus, int proto, int uid)
{
	char reply[MAXSRCPLINELEN];

    switch(proto) {
        case 33:    infoSM(bus, PROTO_MM, GET, MM_REG, 0, -1, -1, -1, reply);
                    break;
        default:    syslog_bus(bus, DBG_WARN, 
                            "*** MCS discovery for prot %d not supported.", proto);
    }
}
