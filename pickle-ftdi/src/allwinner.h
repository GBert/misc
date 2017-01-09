/*
 * Copyright (C) 2015 Darron Broad
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

#ifndef _ALLWINNER_H
#define _ALLWINNER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <sysexits.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BPI_BASE_ADDR (0x01C20000)
#define BPI_BASE_OFS  (0x00000800)
#define BPI_MAP_LEN   (0x2000)

/* Maximum possible number of pins */
#define GPIO_BPI_NPINS (288)

/*
 * PIO Controller Port Register List
 *
 * A20 User Manual Pages 237-238
 *
 * Port A(PA): 18 input/output port 
 * Port B(PB): 24 input/output port
 * Port C(PC): 25 input/output port
 * Port D(PD): 28 input/output port
 * Port E(PE): 12 input/output port
 * Port F(PF):  6 input/output port
 * Port G(PG): 12 input/output port
 * Port H(PH): 28 input/output port
 * Port I(PI): 22 input/output port
 */

#define PIO_BASE_ADDR (0x01C20800)

#define PA_CFG0 ((0 * 0x24 + 0x00) / 4)
#define PB_CFG0 ((1 * 0x24 + 0x00) / 4)
#define PC_CFG0 ((2 * 0x24 + 0x00) / 4)
#define PD_CFG0 ((3 * 0x24 + 0x00) / 4)
#define PE_CFG0 ((4 * 0x24 + 0x00) / 4)
#define PF_CFG0 ((5 * 0x24 + 0x00) / 4)
#define PG_CFG0 ((6 * 0x24 + 0x00) / 4)
#define PH_CFG0 ((7 * 0x24 + 0x00) / 4)
#define PI_CFG0 ((8 * 0x24 + 0x00) / 4)

#define PA_CFG1 ((0 * 0x24 + 0x04) / 4)
#define PB_CFG1 ((1 * 0x24 + 0x04) / 4)
#define PC_CFG1 ((2 * 0x24 + 0x04) / 4)
#define PD_CFG1 ((3 * 0x24 + 0x04) / 4)
#define PE_CFG1 ((4 * 0x24 + 0x04) / 4)
#define PF_CFG1 ((5 * 0x24 + 0x04) / 4)
#define PG_CFG1 ((6 * 0x24 + 0x04) / 4)
#define PH_CFG1 ((7 * 0x24 + 0x04) / 4)
#define PI_CFG1 ((8 * 0x24 + 0x04) / 4)

#define PA_CFG2 ((0 * 0x24 + 0x08) / 4)
#define PB_CFG2 ((1 * 0x24 + 0x08) / 4)
#define PC_CFG2 ((2 * 0x24 + 0x08) / 4)
#define PD_CFG2 ((3 * 0x24 + 0x08) / 4)
#define PE_CFG2 ((4 * 0x24 + 0x08) / 4)
#define PF_CFG2 ((5 * 0x24 + 0x08) / 4)
#define PG_CFG2 ((6 * 0x24 + 0x08) / 4)
#define PH_CFG2 ((7 * 0x24 + 0x08) / 4)
#define PI_CFG2 ((8 * 0x24 + 0x08) / 4)

#define PA_CFG3 ((0 * 0x24 + 0x0C) / 4)
#define PB_CFG3 ((1 * 0x24 + 0x0C) / 4)
#define PC_CFG3 ((2 * 0x24 + 0x0C) / 4)
#define PD_CFG3 ((3 * 0x24 + 0x0C) / 4)
#define PE_CFG3 ((4 * 0x24 + 0x0C) / 4)
#define PF_CFG3 ((5 * 0x24 + 0x0C) / 4)
#define PG_CFG3 ((6 * 0x24 + 0x0C) / 4)
#define PH_CFG3 ((7 * 0x24 + 0x0C) / 4)
#define PI_CFG3 ((8 * 0x24 + 0x0C) / 4)

#define PA_DAT ((0 * 0x24 + 0x10) / 4)
#define PB_DAT ((1 * 0x24 + 0x10) / 4)
#define PC_DAT ((2 * 0x24 + 0x10) / 4)
#define PD_DAT ((3 * 0x24 + 0x10) / 4)
#define PE_DAT ((4 * 0x24 + 0x10) / 4)
#define PF_DAT ((5 * 0x24 + 0x10) / 4)
#define PG_DAT ((6 * 0x24 + 0x10) / 4)
#define PH_DAT ((7 * 0x24 + 0x10) / 4)
#define PI_DAT ((8 * 0x24 + 0x10) / 4)

#define PA_DRV0 ((0 * 0x24 + 0x14) / 4)
#define PB_DRV0 ((1 * 0x24 + 0x14) / 4)
#define PC_DRV0 ((2 * 0x24 + 0x14) / 4)
#define PD_DRV0 ((3 * 0x24 + 0x14) / 4)
#define PE_DRV0 ((4 * 0x24 + 0x14) / 4)
#define PF_DRV0 ((5 * 0x24 + 0x14) / 4)
#define PG_DRV0 ((6 * 0x24 + 0x14) / 4)
#define PH_DRV0 ((7 * 0x24 + 0x14) / 4)
#define PI_DRV0 ((8 * 0x24 + 0x14) / 4)

#define PA_DRV1 ((0 * 0x24 + 0x18) / 4)
#define PB_DRV1 ((1 * 0x24 + 0x18) / 4)
#define PC_DRV1 ((2 * 0x24 + 0x18) / 4)
#define PD_DRV1 ((3 * 0x24 + 0x18) / 4)
#define PE_DRV1 ((4 * 0x24 + 0x18) / 4)
#define PF_DRV1 ((5 * 0x24 + 0x18) / 4)
#define PG_DRV1 ((6 * 0x24 + 0x18) / 4)
#define PH_DRV1 ((7 * 0x24 + 0x18) / 4)
#define PI_DRV1 ((8 * 0x24 + 0x18) / 4)

#define PA_PUL0 ((0 * 0x24 + 0x1C) / 4)
#define PB_PUL0 ((1 * 0x24 + 0x1C) / 4)
#define PC_PUL0 ((2 * 0x24 + 0x1C) / 4)
#define PD_PUL0 ((3 * 0x24 + 0x1C) / 4)
#define PE_PUL0 ((4 * 0x24 + 0x1C) / 4)
#define PF_PUL0 ((5 * 0x24 + 0x1C) / 4)
#define PG_PUL0 ((6 * 0x24 + 0x1C) / 4)
#define PH_PUL0 ((7 * 0x24 + 0x1C) / 4)
#define PI_PUL0 ((8 * 0x24 + 0x1C) / 4)

#define PA_PUL1 ((0 * 0x24 + 0x20) / 4)
#define PB_PUL1 ((1 * 0x24 + 0x20) / 4)
#define PC_PUL1 ((2 * 0x24 + 0x20) / 4)
#define PD_PUL1 ((3 * 0x24 + 0x20) / 4)
#define PE_PUL1 ((4 * 0x24 + 0x20) / 4)
#define PF_PUL1 ((5 * 0x24 + 0x20) / 4)
#define PG_PUL1 ((6 * 0x24 + 0x20) / 4)
#define PH_PUL1 ((7 * 0x24 + 0x20) / 4)
#define PI_PUL1 ((8 * 0x24 + 0x20) / 4)

#define PX_PULL_DIS  (0)
#define PX_PULL_UP   (1)
#define PX_PULL_DOWN (2)
#define PX_PULL_RES  (3)

#define PX_SELECT0 (0)
#define PX_SELECT1 (1)
#define PX_SELECT2 (2)
#define PX_SELECT3 (3)
#define PX_SELECT4 (4)
#define PX_SELECT5 (5)
#define PX_SELECT6 (6)
#define PX_SELECT7 (7)

int gpio_bpi_open(const char *);
void gpio_bpi_close(void);

void gpio_bpi_delay(void);
int gpio_bpi_get(uint16_t, uint8_t *);
int gpio_bpi_set(uint16_t, uint8_t);
int gpio_bpi_release(uint16_t, uint8_t);

#endif /* !_ALLWINNER_H */
