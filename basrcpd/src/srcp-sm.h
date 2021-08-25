// srcp-sm.h - adapted for basrcpd project 2018 by Rainer Müller 

/***************************************************************************
                          srcp-sm.h  -  description
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

#ifndef _SRCP_SM_H
#define _SRCP_SM_H

#include <sys/time.h>
#include "config-srcpd.h"

typedef enum SMPROTOCOL {
    PROTO_NMRA = 0,
    PROTO_MM,
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
    MM_REG,
    BIND_MFX,
    CV_MFX
} sm_type_t;

/* Loco decoder */
typedef struct _SM {
    sm_protocol_t protocol;  
    sm_type_t type;
    sm_command_t command;
    int protocolversion;
    int addr;
    //				MMREG    REG      PAGE	  CV	  CVBIT	  CVMFX	  BIND	
    int cvaddr;	//	regaddr  regaddr  cvaddr  cvaddr  cvaddr  cvline  ----
    int index;	//	----	 ----	  ----	  ----	  bitnr	  cvindex ----
    int value; 	//	regval	 regval	  cvval	  cvval   bitval  cvval	  uid / regcnt
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
 * @param caNr Bei allem anderen: CV Nr
 * @param bit_index Bei type==CV_BIT: die Bitnummer 0..7
 *                  Bei type==CV_MFX und CA_MFX: Der Index innerhalb CV Zeile
 * @param value Bei type==CV_BIT 0 oder 1, sondet 0..255 Byte Value zum schreiben oder verify
 */
int enqueueSM(bus_t busnumber, sm_protocol_t protocol, sm_command_t command, sm_type_t type, 
						int addr, int typeaddr, int bit_index, int value);
              
int queue_SM_isempty(bus_t busnumber);
int getNextSM(bus_t busnumber, sm_t * l);
int dequeueNextSM(bus_t, sm_t*);

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
				int addr, int typeaddr, int bit_index, int value, char *info);

void handle_mfx_bind_verify(bus_t bus, sm_command_t cmnd, uint32_t val, int addr);
void handle_mcs_config(bus_t bus, sm_command_t command, 
						int uid, int cvaddr, int cvindex, int value, int ctrl);
void handle_mcs_discovery(bus_t bus, int proto, int uid);

#endif
