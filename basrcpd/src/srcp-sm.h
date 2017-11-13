/***************************************************************************
                          srcp-sm.h  -  description
                             -------------------
    begin                : Mon Aug 12 2002
    copyright            : (C) 2002 by Dipl.-Ing. Frank Schmischke
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

#ifndef _SRCP_SM_H
#define _SRCP_SM_H

#include <sys/time.h>
#include "config-srcpd.h"

typedef enum SMPROTOCOL {
    PROTO_NMRA = 0,
    PROTO_MFX
} sm_protocol_t;

typedef enum COMMAND {
    SET = 0,
    GET,
    VERIFY,
    INIT,
    TERM
} sm_command_t;

typedef enum TYPE {
    REGISTER = 0,
    PAGE,
    CV,
    CV_BIT,
    CV_MFX,
    CA_MFX
} sm_type_t;

/* Loco decoder */
typedef struct _SM {
    sm_protocol_t protocol;  /* currently only NMRA and MFX is supported */
                             /* (for IB, but not completely, work in progress) */
    sm_type_t type;
    sm_command_t command;
    int protocolversion;
    int addr;
    int blockNr; //CA_MFX
    int caNr; //CA_MFX
    int cv_caIndex; //CV Nummer, bei CA_MFX x. Vorkommen CA in Block
    int bit_index; //CV_BIT: Birnr 0..7, MFX: Index innerhlab CV
    int value;
    struct timeval tv;          /* time of change */
} sm_t;

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
              int blockNr, int caNr, int cv_caIndex, int bit_index, int value);
              
int queue_SM_isempty(bus_t busnumber);
int dequeueNextSM(bus_t, sm_t*);

int getSM(bus_t busnumber, int addr, sm_t*);
int setSM(bus_t busnumber, int type, int addr, int typeaddr, int bit,
          int value, int return_value);
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
           int blockNr, int caNr, int cv_caIndex, int bit_index, int value, char *info);

#endif
