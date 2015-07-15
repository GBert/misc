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

#undef DEBUG

#ifdef DEBUG
#define DPRINT(...) printf(__VA_ARGS__)
#else
#define DPRINT(...)
#endif

/******************************************************************************
 *
 * STK500V2 sender
 *
 *****************************************************************************/

/* I/O timeout */
extern int stk_timeout;

/* I/O buffer, Sign-on signature, Packet sequence number and I/O addr. size */
extern uint8_t stk_msg[], stk_signon_msg[11], stk_seqnum, stk_addr_size;

/* I/O data row address */
extern uint32_t stk_addr;

/******************************************************************************
 * SEND COMMAND
 */
static int
stk500v2_cmd(uint16_t msgsize)
{
	int16_t rc;
	int r = STK_RETRY;

	DPRINT("%s()\n", __func__);

	while (1) {
		rc = stk500v2_send(msgsize);
		if (rc < 0)
			break;
		rc = stk500v2_recv(STK_SEQTEST);
		if (--r && rc == -ETIMEDOUT) {
			/*
			 * Resend the command.
			 *
			 * This will occur should the MCT232 driver on Linux
			 * decide to eat a packet.
			 *
			 * It will also occur after opening a connection to
			 * an Arduino with a bootloader that both becomes
			 * active on serial port open (DTR reset).
			 */
			printf("%s: command timed out, resending ...\n", __func__);
			continue;
		}
		break;
	}
	return rc;
}

/******************************************************************************
 * SEND COMMAND SIGN-ON
 */
static inline int16_t
stk500v2_signon(void)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_msg[0] = STK_CMD_SIGN_ON;

	rc = stk500v2_cmd(1);

	if (rc < 0)
		return rc;
	if (rc != sizeof(stk_signon_msg))
		return -1;
	if (memcmp((void *)stk_msg, (void *)stk_signon_msg, sizeof(stk_signon_msg)) != 0)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND CHIP ERASE
 */
static inline int16_t
stk500v2_chip_erase_isp(void)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_msg[0] = STK_CMD_CHIP_ERASE_ISP;
	stk_msg[1] = 0;	/* eraseDelay */
	stk_msg[2] = 0;	/* pollMethod */
	stk_msg[3] = 0;	/* cmd1       */
	stk_msg[4] = 0;	/* cmd2       */
	stk_msg[5] = 0;	/* cmd3       */
	stk_msg[6] = 0;	/* cmd4       */

	rc = stk500v2_cmd(7);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND ENTER PROGMODE ISP
 */
static inline int16_t
stk500v2_enter_progmode_isp(void)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_msg[0] = STK_CMD_ENTER_PROGMODE_ISP;;
	stk_msg[1] = 0;	/* timeout     */
	stk_msg[2] = 0;	/* stabDelay   */
	stk_msg[3] = 0;	/* cmdexeDelay */
	stk_msg[4] = 0;	/* synchLoops  */
	stk_msg[5] = 0;	/* byteDelay   */
	stk_msg[6] = 0;	/* pollValue   */
	stk_msg[7] = 0;	/* pollIndex   */
	stk_msg[8] = 0;	/* cmd1       */
	stk_msg[9] = 0;	/* cmd2       */
	stk_msg[10] = 0;/* cmd3       */
	stk_msg[11] = 0;/* cmd4       */

	rc = stk500v2_cmd(12);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND LOAD ADDRESS
 */
static inline int16_t
stk500v2_load_address(uint32_t address)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	if (stk_addr == address)
		return 0;

	stk_addr = address;

	stk_msg[0] = STK_CMD_LOAD_ADDRESS;
	address >>= 1;
	if (stk_addr_size == 16) {
		address &= 0x00007FFF;	/* 16-BIT ADDRESS */
	} else if (stk_addr_size == 24) {
		address &= 0x007FFFFF;	/* 24-BIT ADDRESS */
	} else {
		address |= 0x80000000;	/* 32-BIT ADDRESS */
	}
	stk_msg[1] = address >> 24;
	stk_msg[2] = address >> 16;
	stk_msg[3] = address >> 8;
	stk_msg[4] = address;

	rc = stk500v2_cmd(5);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND PROGRAM FLASH ISP
 */
static int16_t
stk500v2_program_flash_isp(uint8_t *panel, uint16_t panel_size)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_addr += panel_size;

	stk_msg[0] = STK_CMD_PROGRAM_FLASH_ISP;
	stk_msg[1] = panel_size >> 8;
	stk_msg[2] = panel_size;
	stk_msg[3] = 1;	/* mode  */
	stk_msg[4] = 0;	/* delay */
	stk_msg[5] = 0;	/* cmd1  */
	stk_msg[6] = 0;	/* cmd2  */
	stk_msg[7] = 0;	/* cmd3  */
	stk_msg[8] = 0;	/* poll1 */
	stk_msg[9] = 0;	/* poll2 */
	memcpy((void *)&stk_msg[10], (void *)panel, panel_size);

	rc = stk500v2_cmd(10 + panel_size);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMMAND READ FLASH ISP
 */
static int16_t
stk500v2_read_flash_isp(uint16_t panel_size)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_addr += panel_size;

	stk_msg[0] = STK_CMD_READ_FLASH_ISP;
	stk_msg[1] = panel_size >> 8;
	stk_msg[2] = panel_size;
	stk_msg[3] = 0;	/* cmd1  */

	rc = stk500v2_cmd(4);

	if (rc < 0)
		return rc;
	if (rc != (3 + panel_size))
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * SEND COMAND LEAVE PROGMODE ISP
 */
static inline int16_t
stk500v2_leave_progmode_isp(void)
{
	int16_t rc;

	DPRINT("%s()\n", __func__);

	stk_msg[0] = STK_CMD_LEAVE_PROGMODE_ISP;;
	stk_msg[1] = 0;	/* preDelay  */
	stk_msg[2] = 0;	/* postDelay */

	rc = stk500v2_cmd(3);

	if (rc < 0)
		return rc;
	if (rc != 2)
		return -1;
	if (stk_msg[1] != STK_STATUS_CMD_OK)
		return -1;

	return 0;
}

/******************************************************************************
 * PROCESS PANEL
 */
int
stk500v2_panel(int prog_mode, uint32_t panel_address,
	uint8_t *panel, uint16_t panel_size, uint16_t *panel_count, uint32_t *fail)
{
	int rc, i;

	DPRINT("%s()\n", __func__);

	if (*panel_count == 0) {
		return 0;
	}
	rc = stk500v2_load_address(panel_address);
	if (rc < 0) {
		printf("%s: failed to load address\n", __func__);
		return rc;
	}
	if (prog_mode == 'p'/* rogram */) {
		rc = stk500v2_program_flash_isp(panel, panel_size);
		if (rc < 0) {
			printf("%s: failed to program flash\n", __func__);
			return rc;
		}
	} else { /* 'v' erify */
		rc = stk500v2_read_flash_isp(panel_size);
		if (rc < 0) {
			printf("%s: failed to read flash\n", __func__);
			return rc;
		}
		for (i = 0; i < panel_size; ++i) {
			if (panel[i] != stk_msg[2 + i]) {
				printf("%s: error: read [%02X] expected [%02X] at [%08X]\n",
					__func__, stk_msg[2 + i], panel[i], panel_address + i);
				(*fail)++;
			}
		}
	}
	/* Reset panel */
	*panel_count = 0;
	memset((void *)panel, -1, panel_size);

	return 0;
}

/******************************************************************************
 * PROGRAM OR VERIFY FILE
 */
int
stk500v2_load(int prog_mode, char *interface, char *filename, uint8_t addr_size)
{
	int rc;
	uint32_t count = 0, fail = 0, nbytes, i, j;
	pic_data **pdata = NULL;

	DPRINT("%s()\n", __func__);

	/* Panel */
	uint8_t panel[PIC_BYTLEN];
	uint16_t panel_size, panel_count;
	uint32_t panel_address, boundary, mask, address, new_address;

	/* Connect */
	rc = stk500v2_open(interface, 8048, STK_CONNECT);
	if (rc < 0) {
		printf("%s: failed to open interface\n", __func__);
		return rc;
	}
	stk_seqnum = 0;
	stk_timeout = STK_TIMEOUT;

	/* Get HEX */
	nbytes = inhx32_array_create(&pdata, filename, &count);
	if (nbytes == 0) {
		stk500v2_close(interface);
		return -1;
	}

	/* Sign-on */
	rc = stk500v2_signon();
	if (rc < 0) {
		printf("%s: failed to sign-on\n", __func__);
		goto error;
	}

	/* Erase */
	if (prog_mode == 'p') {
		/* Enter programming mode */
		rc = stk500v2_enter_progmode_isp();
		if (rc < 0) {
			printf("%s: failed to enter programming mode for erase\n", __func__);
			goto error;
		}
		
		/* Chip erase */
		rc = stk500v2_chip_erase_isp();
		if (rc < 0) {
			printf("%s: failed to erase chip\n", __func__);
			goto error;
		}
	}

	/* Enter programming mode */
	rc = stk500v2_enter_progmode_isp();
	if (rc < 0) {
		printf("%s: failed to enter programming mode for programming\n", __func__);
		goto error;
	}

	/* Init target */
	stk_addr = UINT32_MAX;
	stk_addr_size = addr_size;

	/* Init panel */
	panel_address = UINT32_MAX;
	panel_size = PIC_BYTLEN;
	boundary = 0 - panel_size;
	mask = panel_size - 1;

	/* Reset panel */
	panel_count = 0;
	memset((void *)panel, -1, panel_size);

	/* For each line */
	for (i = 0; i < count; ++i) {
		/* For each byte in line */
		for (j = 0; j < pdata[i]->nbytes; ++j) {
			address = pdata[i]->address + j;
			new_address = address & boundary;
			if (new_address != panel_address) {
				rc = stk500v2_panel(prog_mode, panel_address,
					panel, panel_size, &panel_count, &fail);
				if (rc < 0)
					goto error;
				panel_address = new_address;
			}
			panel[address & mask] = pdata[i]->bytes[j];
			panel_count++;
		}
	}
	rc = stk500v2_panel(prog_mode, panel_address,
		panel, panel_size, &panel_count, &fail);
	if (rc < 0)
		goto error;

	/* Exit programming mode */
	rc = stk500v2_leave_progmode_isp();
	if (rc < 0) {
		printf("%s: failed to leave programming mode\n", __func__);
		goto error;
	}

	if (prog_mode == 'p')
		printf("Total: %u\n", nbytes);
	else
		printf("Total: %u Fail: %u\n", nbytes, fail);
error:
	/* Clean up */
	inhx32_array_free(pdata, count);
	
	/* Close interface */
	stk500v2_close(interface);

	return rc;
}
