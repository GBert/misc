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

#undef DEBUG

#ifdef DEBUG
#define DPRINT(...) printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

/******************************************************************************
 *
 * STK500V2 listener
 *
 *****************************************************************************/

/* I/O timeout */
extern int stk_timeout;

/* I/O buffer and Sign-on signature */
extern uint8_t stk_msg[], stk_signon_msg[11];

/* I/O data row address */
extern uint32_t stk_addr;

/* Device ready for programming, else in standby */
static int stk_progmode;

/******************************************************************************
 * CLEAN UP DEAD SESSION
 */
static void
stk500v2_cleanup(void)
{
	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_WRITE)
		pic_program_end(1);
	else if (stk_progmode == STK_PROGMODE_READ)
		pic_verify_end();

	stk_progmode = STK_PROGMODE_NONE;
}

/******************************************************************************
 * RESPOND WITH COMMAND OKAY
 */
static inline void
stk500v2_ok(void)
{
	stk_msg[1] = STK_STATUS_CMD_OK;
	stk500v2_send(2);
}

/******************************************************************************
 * RESPOND WITH COMMAND FAILED
 */
static inline void
stk500v2_failed(void)
{
	stk_msg[1] = STK_STATUS_CMD_FAILED;
	stk500v2_send(2);
}

/******************************************************************************
 * RESPOND TO COMMAND SIGN-ON
 */
static inline void
stk500v2_signon(void)
{
	DPRINT("%s()\n", __func__);

	memcpy((void *)stk_msg, (void *)stk_signon_msg, sizeof(stk_signon_msg));
	stk500v2_send(sizeof(stk_signon_msg));
}

/******************************************************************************
 * RESPOND TO COMMAND SET PARAMETER
 */
static inline void
stk500v2_set_parameter(void)
{
	DPRINT("%s()\n", __func__);

	stk500v2_ok();
}

/******************************************************************************
 * RESPOND TO COMMAND GET PARAMETER
 */
static inline void
stk500v2_get_parameter(void)
{
	DPRINT("%s()\n", __func__);

	stk_msg[1] = STK_STATUS_CMD_OK;
	stk_msg[2] = /* Value */ 0;
	stk500v2_send(3);
}

/******************************************************************************
 * RESPOND TO COMMAND SPI MULTI
 */
static inline void
stk500v2_spi_multi(void)
{
	DPRINT("%s()\n", __func__);

	if (stk_msg[1] == 4 && stk_msg[2] == 4 && stk_msg[3] == 0) {
		stk_msg[1] = STK_STATUS_CMD_OK;
		stk_msg[2] = 0;
		stk_msg[3] = 0;
		if (stk_msg[4] == 0x30) {
			/* Signature query */
			stk_msg[4] = 0;
			switch (stk_msg[6]) {
			case 0: stk_msg[5] = 0x50; /* chipKIT PIC32 [0] */
				break;
			case 1: stk_msg[5] = 0x49; /* chipKIT PIC32 [1] */
				break;
			case 2: stk_msg[5] = 0x43; /* chipKIT PIC32 [2] */
				break;
			}
		} else {
			stk_msg[4] = 0;
			stk_msg[5] = 0;
		}
		stk_msg[6] = STK_STATUS_CMD_OK;
		stk500v2_send(7);
	} else {
		stk500v2_failed();
	}
}

/******************************************************************************
 * RESPOND TO COMMAND CHIP ERASE ISP
 */
static inline void
stk500v2_chip_erase_isp(void)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup();

	if (pic_read_config() == 0) {
		pic_bulk_erase();
		stk500v2_ok();
		return;
	}
	stk500v2_failed();
}

/******************************************************************************
 * RESPOND TO COMMAND ENTER PROGMODE ISP
 */
static inline void
stk500v2_enter_progmode_isp(void)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup();

	if (pic_read_config() == 0) {
		stk500v2_ok();
		return;
	}
	stk500v2_failed();
}

/******************************************************************************
 * RESPOND TO COMMAND LOAD ADDRESS
 */
static inline void
stk500v2_load_address(void)
{
	stk_addr = stk_msg[1] << 24 | stk_msg[2] << 16 | stk_msg[3] << 8 | stk_msg[4];
	if (stk_addr & 0x80000000) {
		stk_addr <<= 1;			/* 32-BIT EXTENDED ADDRESS */
	} else {
		stk_addr <<= 1;			/* 24-BIT ADDRESS */
		stk_addr &= 0xFFFFFF;
	}
	DPRINT("%s() stk_addr=0x%08X\n", __func__, stk_addr);
	stk500v2_ok();
}

/******************************************************************************
 * RESPOND TO COMMAND PROGRAM FLASH ISP
 */
static inline void
stk500v2_program_flash_isp(void)
{
	static uint32_t region;
	pic_data pdata;

	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_READ) {
		pic_verify_end();
	}
	if (stk_progmode != STK_PROGMODE_WRITE) {
		if (pic_read_config() == 0) {
			pic_program_begin();
			stk_progmode = STK_PROGMODE_WRITE;
			region = PIC_REGIONNOTSUP;
		} else  {
			stk_progmode = STK_PROGMODE_NONE;
			stk500v2_failed();
			return;
		}
	}

	pdata.nbytes = stk_msg[1] << 8 | stk_msg[2];

	if (pdata.nbytes <= PIC_BYTLEN) {
		memcpy((void *)&pdata.bytes, (void *)&stk_msg[10], pdata.nbytes);
		pdata.address = stk_addr;
		stk_addr += pdata.nbytes;
		region = pic_program_data(region, &pdata);
		stk500v2_ok();
	} else  {
		stk500v2_failed();
	}
}

/******************************************************************************
 * RESPOND TO COMMAND READ FLASH ISP
 */
static inline void
stk500v2_read_flash_isp(void)
{
	static uint32_t region;
	uint32_t fail;
	pic_data pdata;

	DPRINT("%s()\n", __func__);

	if (stk_progmode == STK_PROGMODE_WRITE) {
		pic_program_end(1);
	}
	if (stk_progmode != STK_PROGMODE_READ) {
		if (pic_read_config() == 0) {
			pic_verify_begin();
			stk_progmode = STK_PROGMODE_READ;
			region = PIC_REGIONNOTSUP;
		} else  {
			stk_progmode = STK_PROGMODE_NONE;
			stk500v2_failed();
			return;
		}
	}

	pdata.nbytes = stk_msg[1] << 8 | stk_msg[2];

	if (pdata.nbytes <= PIC_BYTLEN) {
        	memset((void *)pdata.bytes, -1, pdata.nbytes);
		pdata.address = stk_addr;
		stk_addr += pdata.nbytes;
		region = pic_verify_data(region, &pdata, &fail);
		stk_msg[1] = STK_STATUS_CMD_OK;
		memcpy((void *)&stk_msg[2], (void *)pdata.bytes, pdata.nbytes);
		stk_msg[2 + pdata.nbytes] = STK_STATUS_CMD_OK;
		stk500v2_send(3 + pdata.nbytes);
	} else {
		stk500v2_failed();
	}
}

/******************************************************************************
 * RESPOND TO COMMAND LEAVE PROGMODE ISP
 */
static inline void
stk500v2_leave_progmode_isp(void)
{
	DPRINT("%s()\n", __func__);

	stk500v2_cleanup();
	stk500v2_ok();
}

/******************************************************************************
 * LISTEN FOR REMOTE PROGRAMMING
 */
int
stk500v2_listen(char *interface, int port)
{
	int rc;

	DPRINT("%s()\n", __func__);

	/* Listen */
	rc = stk500v2_open(interface, port, STK_LISTEN);
	if (rc < 0) {
		printf("%s: failed to open interface\n", __func__);
		return rc;
	}

	/* Init */
	p.f = NULL; /* No console messages */
	stk_addr = 0;
	stk_timeout = STK_TIMEOUT;
	stk_progmode = STK_PROGMODE_NONE;

	while (1) {
		rc = stk500v2_recv(STK_SEQECHO);
		if (rc == -EIO) {
			stk500v2_cleanup();
			break;
		}
		if (rc == -ETIMEDOUT) {
			continue;
		}
		if (rc == STK_PROTOCOL_ERROR) {
			continue;
		}
		if (rc == /* Empty message */ 0) {
			continue;
		}
		if (stk_msg[0] == STK_CMD_LEAVE_PROGMODE_ISP) {
			stk500v2_leave_progmode_isp();
			break;
		}
		switch (stk_msg[0]) {
		default:stk500v2_failed();
			break;
		case STK_CMD_SIGN_ON:
			stk500v2_signon();
			break;
		case STK_CMD_SET_PARAMETER:
			stk500v2_set_parameter();
			break;
		case STK_CMD_GET_PARAMETER:
			stk500v2_get_parameter();
			break;
		case STK_CMD_SPI_MULTI:
			stk500v2_spi_multi();
			break;
		case STK_CMD_CHIP_ERASE_ISP:
			stk500v2_chip_erase_isp();
			break;
		case STK_CMD_ENTER_PROGMODE_ISP:
			stk500v2_enter_progmode_isp();
			break;
		case STK_CMD_LOAD_ADDRESS:
			stk500v2_load_address();
			break;
		case STK_CMD_PROGRAM_FLASH_ISP:
			stk500v2_program_flash_isp();
			break;
		case STK_CMD_READ_FLASH_ISP:
			stk500v2_read_flash_isp();
			break;
		}
	}
	stk500v2_close(interface);

	return rc;
}
