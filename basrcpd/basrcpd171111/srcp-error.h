/* $Id: srcp-error.h 1073 2008-01-04 09:11:38Z gscholz $ */

/* 
 * Vorliegende Software unterliegt der General Public License, 
 * Version 2, 1991. (c) Matthias Trute, 2000-2001.
 *
 */

#ifndef _SRCP_ERROR_H
#define _SRCP_ERROR_H

#include <sys/time.h>


/* Handshake */
#define SRCP_OK_GO                      200
#define SRCP_OK_PROTOCOL                201
#define SRCP_OK_CONNMODE                202

/* HandShake */
#define SRCP_HS_WRONGPROTOCOL   400
#define SRCP_HS_WRONGCONNMODE   401
#define SRCP_HS_NODATA          402

/* COMMAND MODE */
#define SRCP_INFO                       100
#define SRCP_OK                         200
#define SRCP_UNKNOWNCOMMAND             410
#define SRCP_UNKNOWNVALUE               411
#define SRCP_WRONGVALUE                 412
#define SRCP_TEMPORARILYPROHIBITED      413
#define SRCP_DEVICELOCKED               414
#define SRCP_FORBIDDEN                  415
#define SRCP_NODATA                     416
#define SRCP_TIMEOUT                    417
#define SRCP_LISTTOOLONG                418
#define SRCP_LISTTOOSHORT               419
#define SRCP_UNSUPPORTEDDEVICEPROTOCOL  420
#define SRCP_UNSUPPORTEDDEVICE          421
#define SRCP_UNSUPPORTEDDEVICEGROUP     422
#define SRCP_UNSUPPORTEDOPERATION       423
#define SRCP_DEVICEREINITIALIZED        424

#define SRCP_OUTOFRESOURCES             500


int srcp_fmt_msg(int errorcode, char *msg, struct timeval);

#endif
