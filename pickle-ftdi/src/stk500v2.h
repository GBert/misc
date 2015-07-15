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

#ifndef _STK500V2_H
#define _STK500V2_H

/* stk500v2 max message size */
#define STK_LEN (275)

#define STK_MESSAGE_START (0x1B)
#define STK_TOKEN	  (0x0E)

#define STK_CMD_SIGN_ON	       	   (0x01)
#define STK_CMD_SET_PARAMETER      (0x02)
#define STK_CMD_GET_PARAMETER      (0x03)
#define STK_CMD_LOAD_ADDRESS       (0x06)
#define STK_CMD_ENTER_PROGMODE_ISP (0x10)
#define STK_CMD_LEAVE_PROGMODE_ISP (0x11)
#define STK_CMD_CHIP_ERASE_ISP     (0x12)
#define STK_CMD_PROGRAM_FLASH_ISP  (0x13)
#define STK_CMD_READ_FLASH_ISP     (0x14)
#define STK_CMD_SPI_MULTI	   (0x1D)

#define STK_STATUS_CMD_OK	     (0x00)
#define STK_STATUS_CMD_TOUT	     (0x80)
#define STK_STATUS_RDY_BSY_TOUT	     (0x81)
#define STK_STATUS_SET_PARAM_MISSING (0x82)
#define STK_STATUS_CMD_FAILED        (0xC0)
#define STK_STATUS_CKSUM_ERROR	     (0xC1)

/* Our protocol error code */
#define STK_PROTOCOL_ERROR (-200)

/* Receiver echos sequence number received */
#define STK_SEQECHO (0)
/* Sender validates sequence number received */
#define STK_SEQTEST (1)

#define STK_TIMEOUT (2)
#define STK_RETRY   (5)

/* our stk500v2 get/set params */
#define STK_PARAM_ARCH    (0xF0) /* TODO set ARCH     */
#define STK_PARAM_ROWSIZE (0xF1) /* TODO get row size */

#define STK_PROGMODE_NONE  (0) /* current programming mode */
#define STK_PROGMODE_WRITE (1) /* program */
#define STK_PROGMODE_READ  (2) /* verify  */

#define STK_LISTEN (1) 
#define STK_CONNECT (2) 
int16_t stk500v2_open(const char *, int, int);
void stk500v2_close(const char *);
int16_t stk500v2_recv(int);
int16_t stk500v2_send(uint16_t);

#endif /* !_STK500V2_H */
