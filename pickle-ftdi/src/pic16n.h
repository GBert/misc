/*
 * Copyright (C) 2005-2017 Darron Broad
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

#ifndef _PIC16N_H
#define _PIC16N_H

/******************************************************************************
 * 16-bit CONFIGURATION
 *****************************************************************************/

#define PIC16N_WORD_MASK (0x0000FFFF)
#define PIC16N_DATA_MASK (0x000000FF)
#define PIC16N_USERID_MAX (8)
#define PIC16N_CONFIG_MAX (12)

struct pic16n_config {
	uint8_t userid[PIC16N_USERID_MAX];	/* 200000 .. 200007 */
	uint8_t config[PIC16N_CONFIG_MAX]; 	/* 300000 .. 30000B */
	uint16_t revisionid;     	        /* 3FFFFC           */
	uint16_t deviceid;              	/* 3FFFFE           */
#if 0
	uint16_t devinfo[PIC16N_DEVINFO_MAX];   /* */
	uint16_t devconf[PIC16N_DEVCONF_MAX];   /* */
#endif
};

struct pic16n_dsmap {
	char devicename[STRLEN];	/*                           */
	uint16_t deviceid;    		/*                           */
	uint32_t datasheet;     	/* programming specification */
	uint32_t flash;    		/* program flash size        */
	uint32_t config;		/* configuration size        */
	uint32_t eeprom;		/* nvm eeprom size           */
	uint32_t latches;		/* latch / erase row size    */
#if 0
	uint32_t devinfo;               /* devinfo size              */
	uint32_t devconf;               /* devconf size              */
#endif
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

#define PIC16N_WORD(X) ((X) * 512) /* KB to words */

#define PIC16N_USERID_ADDR  (0x200000)
#define PIC16N_CONFIG_ADDR  (0x300000)
#define PIC16N_EEPROM_ADDR  (0x310000) /* EEPROM physical address */
#define PIC16N_REVID_ADDR   (0x3FFFFC)
#define PIC16N_DEVID_ADDR   (0x3FFFFE)
#define PIC16N_EEFAKE_ADDR  (0xF00000) /* EEPROM pseudo address */

#define PIC16N_TPINT_CODE   (2800)	/*  2.8 ms */
#define PIC16N_TPINT_CONFIG (5600)	/*  5.6 ms */
#define PIC16N_TERAB        (25200)	/* 25.2 ms */
#define PIC16N_TERAR        (2800)	/*  2.8 ms */

#define PIC16N_MAJOR_SHIFT   (6)
#define PIC16N_REV_MASK      (0x003F)

/******************************************************************************
 * PICMicro devices (8-bit data devices using new programming protocol)
 *****************************************************************************/

#define DS40001772B (40001772)
#define PIC18F47K40  (0x6900)
#define PIC18F46K40  (0x6920)
#define PIC18F45K40  (0x6940)
#define PIC18F27K40  (0x6960)
#define PIC18F26K40  (0x6980)
#define PIC18F25K40  (0x69A0)
#define PIC18F24K40  (0x69C0)
#define PIC18LF47K40 (0x69E0)
#define PIC18LF46K40 (0x6A00)
#define PIC18LF45K40 (0x6A20)
#define PIC18LF27K40 (0x6A40)
#define PIC18LF26K40 (0x6A60)
#define PIC18LF25K40 (0x6A80)
#define PIC18LF24K40 (0x6AA0)

/******************************************************************************/

uint32_t pic16n_arch(void);
void pic16n_selector(void);
void pic16n_program_verify(void);
void pic16n_standby(void);
void pic16n_bulk_erase(void);
void pic16n_row_erase(uint32_t, uint32_t);
int pic16n_read_config_memory(void);
void pic16n_write_panel(uint32_t, uint32_t, uint32_t *, uint32_t);
uint32_t pic16n_get_program_size(uint32_t *);
uint32_t pic16n_get_data_size(uint32_t *);
uint32_t pic16n_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic16n_read_data_memory_block(uint16_t *, uint32_t, uint16_t);
uint32_t pic16n_write_config(void);
uint16_t pic16n_getregion(uint32_t);
uint32_t pic16n_program_data(uint32_t, pic_data *);
void pic16n_program_begin(void);
void pic16n_program_end(int);
uint32_t pic16n_verify_data(uint32_t, pic_data *, uint32_t *);
void pic16n_view_data(pic_data *);
void pic16n_dumpdeviceid(void);
void pic16n_dumpconfig(int);
void pic16n_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic16n_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic16n_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic16n_dumpinhxdata(uint32_t, uint32_t, uint16_t *);
void pic16n_dumpdevice(void);

#endif /* !_PIC16N_H */
