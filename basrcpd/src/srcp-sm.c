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
 *   - Address = -1, means to use program-track
 *   - Address > 0, means to use pom (only available with CV)
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
 *     SET 1 SM 10 CAMFX 5 19 0 2 50 schreibe 50 an 1. Vorkommen (CA Index 0) CA 19 in Block 5 an Index 2 (=Bremsverzögerung)
 */


/* Command queues pro Bus */
static sm_t queue[MAX_BUSES][QUEUELEN];
static pthread_mutex_t queue_mutex[MAX_BUSES];
static volatile int out[MAX_BUSES], in[MAX_BUSES];

/* internal functions */
static int queue_len(bus_t busnumber);
static int queue_isfull(bus_t busnumber);


int enqueueInfoSM(bus_t busnumber, int addr, int type, int typeaddr,
                  int bit, int value, int return_code,
                  struct timeval *akt_time)
{
    char buffer[1000], msg[1000];
    char tmp[100];

    if (return_code == 0) {
        sprintf(buffer, "%lu.%.3lu 100 INFO %ld SM %d",
                akt_time->tv_sec, akt_time->tv_usec / 1000, busnumber,
                addr);
        switch (type) {
            case REGISTER:
                sprintf(tmp, "REG %d %d", typeaddr, value);
                break;
            case CV:
                sprintf(tmp, "CV %d %d", typeaddr, value);
                break;
            case CV_BIT:
                sprintf(tmp, "CVBIT %d %d %d", typeaddr, bit, value);
                break;
            case PAGE:
                sprintf(tmp, "PAGE %d %d", typeaddr, value);
                break;
        }
    }
    else {
        sprintf(buffer, "%lu.%.3lu 600 ERROR %ld SM %d %d",
                akt_time->tv_sec, akt_time->tv_usec / 1000,
                busnumber, addr, return_code);
        switch (return_code) {
            case 0xF2:
                sprintf(tmp, "Cannot terminate task ");
                break;
            case 0xF3:
                sprintf(tmp, "No task to terminate");
                break;
            case 0xF4:
                sprintf(tmp, "Task terminated");
                break;
            case 0xF6:
                sprintf(tmp, "XPT_DCCQD: Not Ok (direct bit read mode "
                        "is (probably) not supported)");
                break;
            case 0xF7:
                sprintf(tmp, "XPT_DCCQD: Ok (direct bit read mode is "
                        "(probably) supported)");
                break;
            case 0xF8:
                sprintf(tmp, "Error during Selectrix read");
                break;
            case 0xF9:
                sprintf(tmp, "No acknowledge to paged operation "
                        "(paged r/w not supported?)");
                break;
            case 0xFA:
                sprintf(tmp, "Error during DCC direct bit mode operation");
                break;
            case 0xFB:
                sprintf(tmp, "Generic Error");
                break;
            case 0xFC:
                sprintf(tmp, "No decoder detected");
                break;
            case 0xFD:
                sprintf(tmp, "Short! (on the PT)");
                break;
            case 0xFE:
                sprintf(tmp, "No acknowledge from decoder (but a write "
                        "maybe was successful)");
                break;
            case 0xFF:
                sprintf(tmp, "Timeout");
                break;
        }
    }
    sprintf(msg, "%s %s\n", buffer, tmp);
    enqueueInfoMessage(msg);
    return SRCP_OK;
}

/**
 * SM Kommando in Queue zur Ausführung. Wenn ein Paramter für ein Kommando nciht benötigt wird, muss -1 übergeben werden.
 * @param busnumber
 * @param protocol Zu verwendendes Protokoll
 * @param command Auszuführendes SM Kommando (SET, GET etc.)
 * @param type SM Type (CV, CV Bit etc.)
 * @param addr Adresse der Lok. Bei NMRA auf Programmiergleis kann hier -1 übergeben werden (Broadcast),
 *             sonst eine Adresse >0
 * @param blockNr Nur bei type==CA_MFX: MFX Blocknr.
 * @param caNr Nur bei type==CA_MFX: MFX CA Typnr.
 * @param caNr Bei type==CA_MFX: 1 für erstes Vorkommen CA im Block, 2. für 2. Vorkommen etc.
 *             Bei allem anderen: CV Nr
 * @param bit_index Bei type==CV_BIT: die Bitnummer 0..7
 *                  Bei type==CV_MFX und CA_MFX: Der Index innerhalb CV Zeile
 * @param value Bei type==CV_BIT 0 oder 1, sondet 0..255 Byte Value zum schreiben oder verify
 */
int enqueueSM(bus_t busnumber, sm_protocol_t protocol, sm_command_t command, sm_type_t type, int addr,
              int blockNr, int caNr, int cv_caIndex, int bit_index, int value)
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

    queue[busnumber][in[busnumber]].blockNr = blockNr;
    queue[busnumber][in[busnumber]].caNr = caNr;
    queue[busnumber][in[busnumber]].bit_index = bit_index;
    queue[busnumber][in[busnumber]].type = type;
    queue[busnumber][in[busnumber]].value = value;
    queue[busnumber][in[busnumber]].cv_caIndex = cv_caIndex;
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

int setSM(bus_t busnumber, int type, int addr, int typeaddr, int bit,
          int value, int return_code)
{
    struct timeval tv;

    syslog_bus(busnumber, DBG_DEBUG,
               "CV: %d         BIT: %d         VALUE: 0x%02x", typeaddr,
               bit, value);
    if (addr == -1) {
        gettimeofday(&tv, NULL);
        if (type == CV_BIT)
            value = (value & (1 << bit)) ? 1 : 0;
        enqueueInfoSM(busnumber, addr, type, typeaddr, bit, value,
                      return_code, &tv);
        return SRCP_OK;
    }
    else {
        return SRCP_NODATA;
    }
}

/**
 * SM Kommando ausführen. Wenn ein Paramter für ein Kommando nciht benötigt wird, muss -1 übergeben werden.
 * @param busnumber
 * @param protocol Zu verwendendes Protokoll
 * @param command Auszuführendes SM Kommando (SET, GET etc.)
 * @param type SM Type (CV, CV Bit etc.)
 * @param addr Adresse der Lok. Bei NMRA auf Programmiergleis kann hier -1 übergeben werden (Broadcast),
 *             sonst eine Adresse >0
 * @param blockNr Nur bei type==CA_MFX: MFX Blocknr.
 * @param caNr Nur bei type==CA_MFX: MFX CA Typnr.
 * @param caNr Bei type==CA_MFX: 1 für erstes Vorkommen CA im Block, 2. für 2. Vorkommen etc.
 *             Bei allem anderen: CV Nr
 * @param bit_index Bei type==CV_BIT: die Bitnummer 0..7
 *                  Bei type==CV_MFX und CA_MFX: Der Index innerhalb CV Zeile
 * @param value Bei type==CV_BIT 0 oder 1, sondet 0..255 Byte Value zum schreiben oder verify
 * @param info Buffer für Meldungsrückgabe
 */
int infoSM(bus_t busnumber, sm_protocol_t protocol, sm_command_t command, sm_type_t type, int addr,
           int blockNr, int caNr, int cv_caIndex, int bit_index, int value, char *info)
{
    int status, result;
    struct timeval now;

    syslog_bus(busnumber, DBG_INFO,
               "TYPE: %d, CV: %d, BIT: %d, VALUE: 0x%02x", type, cv_caIndex,
               bit_index, value);
    session_lock_wait(busnumber);
    status =
        enqueueSM(busnumber, protocol, command, type, addr, blockNr, caNr, cv_caIndex, bit_index, value);

    if (session_condt_wait(busnumber, 90, &result) == ETIMEDOUT) {
        gettimeofday(&now, NULL);
        sprintf(info, "%lu.%.3lu 417 ERROR timeout\n", now.tv_sec,
                now.tv_usec / 1000);
    }
    else {
        gettimeofday(&now, NULL);
        if ((result == -1) || (command == VERIFY && value != result)) {
            sprintf(info, "%lu.%.3lu 412 ERROR wrong value\n", now.tv_sec,
                    now.tv_usec / 1000);
            status = SRCP_WRONGVALUE;
        }
        else {
            switch (type) {
                case CV:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d CV %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, cv_caIndex, result);
                    break;
                case REGISTER:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d REG %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, cv_caIndex, result);
                    break;
                case PAGE:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d PAGE %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, cv_caIndex, result);
                    break;
                case CV_BIT:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d CVBIT %d %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, cv_caIndex, bit_index, result);
                    break;
                case CV_MFX:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d CVMFX %d %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, cv_caIndex, bit_index, result);
                    break;
                case CA_MFX:
                    sprintf(info,
                            "%lu.%.3lu 100 INFO %ld SM %d CAMFX %d %d %d %d %d\n",
                            now.tv_sec, now.tv_usec / 1000, busnumber,
                            addr, blockNr, caNr, cv_caIndex, bit_index, result);
                    break;
            }
        }
    }
    session_unlock_wait(busnumber);
    return status;
}
