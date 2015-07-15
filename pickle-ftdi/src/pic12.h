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

#ifndef _PIC12_H
#define _PIC12_H

/******************************************************************************
 * 12-bit CONFIGURATION
 *****************************************************************************/

#define PIC12_MASK (0x0FFF)

struct pic12_config {
	uint16_t userid[4];		/* PIC16F505: 0x0400 */
	uint16_t osccal_backup;		/* PIC16F505: 0x0404 PIC12F519: 0x0444 */
	uint16_t osccal_reset;		/* PIC16F505: 0x03FF */
	uint16_t config;		/* PIC16F505: 0x07FF */
};

struct pic12_dsmap {
	char devicename[STRLEN];
	uint32_t deviceid;		/* derived from part number              */
	uint16_t flash;			/* program flash size in words           */
	uint16_t dataflash;		/* data flash size in bytes 0=none       */
	uint32_t datasheet;
	uint16_t backupaddr;		/* osccal backup address (0=unsupported) */
	uint16_t configaddr;
	uint8_t nlatches;
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

#define PIC12_WORD(X) (((X) * 8192.0) / 12) /* KB to words */

/*
 * MEMORY MAP
 */
#if 0
#define PIC12_CODE_LOW  (0x0000)
#endif
#define PIC12_CODE_HIGH (0x07FF)	/* 2K words (3KB) is the largest flash size available */
#define PIC12_CONFIG    (0x0FFF)	/* default config address */

/*
 * PROGRAM / ERASE
 */
#define PIC12_TPROG_DEFAULT      (2000)  /*   2ms DS41226G-page 18 */
#define PIC12_TERASE_DEFAULT     (10000) /*  10ms DS41226G-page 18 */
#define PIC12_TDISCHARGE_DEFAULT (100)   /* 100us DS41226G-page 18 */
#define PIC12_TDISCHARGE_300US   (300)   /* 300us DS41670A-page 20 */

/******************************************************************************
 * PICMicro devices (12-bit word devices have no embedded device id)
 *****************************************************************************/

/* DS41228F PIC10F200/202/204/206 */
#define DS41228F (41228)
#define PIC10F200 (10200)
#define PIC10F202 (10202)
#define PIC10F204 (10204)
#define PIC10F206 (10206)

/* DS41227E PIC12F508/509 */
#define DS41227E (41227)
#define PIC12F508 (12508)
#define PIC12F509 (12509)

/* DS41207D PIC16F54 */
#define DS41207D (41207)
#define PIC16F54 (1654)

/* DS41208C PIC16F57 */
#define DS41208C (41208)
#define PIC16F57 (1657)

/* DS41243B PIC16F59 */
#define DS41243B (41243)
#define PIC16F59 (1659)

/* DS41226G PIC16F505 */
#define DS41226G (41226)
#define PIC16F505 (16505)

/* DS41258C PIC16F506 */
#define DS41258C (41258)
#define PIC16F506 (16506)

/* DS41257B PIC12F510 */
#define DS41257B (41257)
#define PIC12F510 (12510)

/* DS41266C PIC12F220/222 */
#define DS41266C (41266)
#define PIC10F220 (10220)
#define PIC10F222 (10222)

/* DS41316C PIC12F519 */
#define DS41316C (41316)
#define PIC12F519 (12519)

/* DS41670A PIC16F570 */
#define DS41670A (41670)
#define PIC16F570 (16570)

/* DS41640A PIC16F527 */
#define DS41640A (41640)
#define PIC16F527 (16527)

/* DS41317B PIC16F526 */
#define DS41317B (41317)
#define PIC16F526 (16526)

/******************************************************************************/

uint32_t pic12_arch(void);
void pic12_selector(void);
void pic12_program_verify(void);
void pic12_standby(void);
uint16_t pic12_read_program_memory_increment(void);
int pic12_read_config_memory(void);
void pic12_bulk_erase(void);
uint32_t pic12_get_program_size(uint32_t *);
uint32_t pic12_get_data_size(uint32_t *);
uint32_t pic12_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic12_read_data_memory_block(uint16_t *, uint32_t, uint16_t);
uint32_t pic12_osccal_write(uint16_t);
uint32_t pic12_write_osccal(uint16_t);
uint32_t pic12_write_config(void);
uint32_t pic12_program_data(uint32_t, pic_data *);
void pic12_program_end(int);
uint32_t pic12_verify_data(uint32_t, pic_data *, uint32_t *);
void pic12_view_data(pic_data *);
void pic12_dumpdeviceid(void);
void pic12_dumposccal(void);
void pic12_dumpconfig(int);
void pic12_dumpconfig_verbose(void);
void pic12_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic12_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic12_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic12_dumpinhxdata(uint32_t, uint32_t, uint16_t *);
void pic12_dumpdevice(void);
#endif /* !_PIC12_H */
