/*
 * Copyright (C) 2005-2015 Darron Broad
 * All rights reserved.
 * 
 * This file is part of Pickle Microchip PIC ICSP.
 * 
 * Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation. 
 * 
 * Pickle Microchip PIC ICSP is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License along
 * with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
 */

#include "pickle.h"

/******************************************************************************
 *
 * Session
 *
 *****************************************************************************/
extern struct pickle p;

#define DEBUG

#ifdef DEBUG
#define DPRINT(...) printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

/*******************************************************************************
 *
 * STK500V2 emulator
 *
 * Only one instance of either a sender or receiver shall exist.
 *
 ******************************************************************************/

/* I/O descriptor */
int stk_fsock = -1;

/* I/O buffer */
uint8_t stk_msg[STK_LEN + /* Checksum */ 1];

/* Packet sequence number */
uint8_t stk_seqnum;

/* I/O timeout */
int stk_timeout;

/* I/O data row address */
uint32_t stk_addr;

/* I/O address size (32-bit extended or 16-bit) */
uint8_t stk_addr_size;

/* Sign-on signature */
uint8_t stk_signon_msg[11] = {
	STK_CMD_SIGN_ON, STK_STATUS_CMD_OK,
	/* SIG LENGTH   */ 0x08,
	/* SIG STK500V2 */ 0x53,0x54,0x4B,0x35,0x30,0x30,0x5F,0x32
};

/*******************************************************************************
 *
 * Open interface
 */
int16_t
stk500v2_open(const char *interface, int port, int flag)
{
	if (interface[0] == '/') {
		stk_fsock = serial_open(interface, serial_speed(p.baudrate));
	} else {
		if (flag == STK_CONNECT) {
			stk_fsock = ip_connect(interface, port);
		} else { /* STK _LISTEN */
			stk_fsock = ip_listen(interface, port);
		}
	}
	return stk_fsock;
}

/*
 * Close interface
 */
void
stk500v2_close(const char *interface)
{
	if (interface[0] == '/') {
		if (stk_fsock >= 0)
			serial_close(stk_fsock);
	} else {
		if (stk_fsock >= 0)
			close(stk_fsock);
	}
	stk_fsock = -1;
}

/*******************************************************************************
 *
 * AVR068: STK500V2 Communication Protocol
 *
 *  Receive Packet
 *
 * Message Format & Protocol Layer State Table
 *
 * 2591C-AVR-06/06 Pages 2 & 3
 *
 ******************************************************************************/
int16_t
stk500v2_recv(int flag)
{
	int rc;
	uint8_t sum = 0;
	uint16_t msgsize, i;

	/* Get message start */
	rc = rw_get(stk_fsock, (char *)stk_msg, 1, stk_timeout);
	if (rc < 0) {
		DPRINT("%s:rw_get MESSAGE_START failed\n", __func__);
		return rc;
	}

	/* Validate message start */
	if (stk_msg[0] != STK_MESSAGE_START) {
		DPRINT("%s:invalid MESSAGE_START\n", __func__);
		return STK_PROTOCOL_ERROR;
	}

	/* Message start checksum */
	sum ^= stk_msg[0];

	/* Get sequence number */
	rc = rw_get(stk_fsock, (char *)stk_msg, 1, stk_timeout);
	if (rc < 0) {
		DPRINT("%s:rw_get SEQNUM failed\n", __func__);
		return rc;
	}

	/* Echo sequence number */
	if (flag == STK_SEQECHO) {
		stk_seqnum = stk_msg[0];
	}
	/* Validate sequence number */
	else if (flag == STK_SEQTEST) {
		if (stk_msg[0] != stk_seqnum) {
			DPRINT("%s:invalid SEQNUM\n", __func__);
			return STK_PROTOCOL_ERROR;
		}
		stk_seqnum++;
	}

	/* Sequence number checksum */
	sum ^= stk_msg[0];

	/* Get message size 1/2 */
	rc = rw_read(stk_fsock, (char *)stk_msg, 2, stk_timeout);
	if (rc < 0) {
		DPRINT("%s:rw_read MESSAGE_SIZE failed\n", __func__);
		return rc;
	}

	/* Validate message size */
	msgsize = (stk_msg[0] << 8) | stk_msg[1];
	if (msgsize > STK_LEN) {
		DPRINT("%s:invalid MESSAGE_SIZE\n", __func__);
		return STK_PROTOCOL_ERROR;
	}

	/* Message size checksum */
	sum ^= stk_msg[0];
	sum ^= stk_msg[1];
	
	/* Get token */
	rc = rw_get(stk_fsock, (char *)stk_msg, 1, stk_timeout);
	if (rc < 0) {
		DPRINT("%s:rw_get TOKEN failed\n", __func__);
		return rc;
	}

	/* Validate token */
	if (stk_msg[0] != STK_TOKEN) {
		DPRINT("%s:invalid TOKEN\n", __func__);
		return STK_PROTOCOL_ERROR;
	}

	/* Token checksum */
	sum ^= stk_msg[0];

	/* Get data & checksum */
	rc = rw_read(stk_fsock, (char *)stk_msg, ++msgsize, stk_timeout);
	if (rc < 0) {
		DPRINT("%s:rw_read DATA + CHKSUM failed\n", __func__);
		return rc;
	}
	msgsize--;

	/* Message checksum */
	for (i = 0; i < msgsize; ++i)
		sum ^= stk_msg[i];

	/* Validate packet checksum */
	if (sum != stk_msg[msgsize]) {
		DPRINT("%s:invalid CHKSUM\n", __func__);
		return STK_PROTOCOL_ERROR;
	}

	/* Return with number of bytes in message */
	return msgsize;
}

/*******************************************************************************
 *
 * AVR068: STK500V2 Communication Protocol
 *
 *  Send Packet
 *
 * Message Format & General Commands
 *
 * 2591C-AVR-06/06 Pages 2 & 4
 *
 ******************************************************************************/
int16_t
stk500v2_send(uint16_t msgsize)
{
	int rc;
	uint8_t header[5], sum = 0;
	uint16_t i;

	/* Send header */
	header[0] = STK_MESSAGE_START;
	header[1] = stk_seqnum;
	header[2] = msgsize >> 8;
	header[3] = msgsize & 0xFF;
	header[4] = STK_TOKEN;
	rc = rw_write(stk_fsock, (char *)header, 5, stk_timeout);
	if (rc < 0)
		return rc;
	if (rc != 5)
		return -1;

	/* Header checksum */
	for (i = 0; i < 5; ++i)
		sum ^= header[i];

	/* Message checksum */
	for (i = 0; i < msgsize; ++i)
		sum ^= stk_msg[i];

	/* Packet checksum */
	stk_msg[msgsize] = sum;

	/* Send message & Checksum */
	rc = rw_write(stk_fsock, (char *)stk_msg, ++msgsize, stk_timeout);
	if (rc < 0)
		return rc;
	if (rc != msgsize--)
		return -1;

	/* Return with number of bytes in message */
	return msgsize;
}
