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

/*****************************************************************************
 *
 * Hardware operations
 *
 *****************************************************************************/

struct pic_ops pic12_ops = {
	.arch				= ARCH12BIT,
	.align				= sizeof(uint16_t),
	.selector			= pic12_selector,
	.program_begin			= NULL,
	.program_data			= pic12_program_data,
	.program_end			= pic12_program_end,
	.verify_begin			= NULL,
	.verify_data			= pic12_verify_data,
	.verify_end			= pic12_standby,
	.view_data			= pic12_view_data,
	.read_config_memory		= pic12_read_config_memory,
	.get_program_size		= pic12_get_program_size,
	.get_data_size			= pic12_get_data_size,
	.get_executive_size		= NULL,
	.get_boot_size			= NULL,
	.read_program_memory_block	= pic12_read_program_memory_block,
	.read_data_memory_block		= pic12_read_data_memory_block,
	.write_panel			= NULL,
	.bulk_erase			= pic12_bulk_erase,
	.write_osccal			= pic12_write_osccal,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= NULL,
	.dumpdeviceid			= pic12_dumpdeviceid,
	.dumpconfig			= pic12_dumpconfig,
	.dumposccal			= pic12_dumposccal,
	.dumpdevice			= pic12_dumpdevice,
	.dumpadj			= 1,
	.dumphexcode			= pic12_dumphexcode,
	.dumpinhxcode			= pic12_dumpinhxcode,
	.dumphexdata			= pic12_dumphexdata,
	.dumpinhxdata			= pic12_dumpinhxdata,
};

uint32_t
pic12_arch(void)
{
	p.pic = &pic12_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic12_config pic12_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic12_dsmap pic12_map[] =
{
/*Device name	Device id 	Flash	Data	Data-sheet Backup OSC	Config 	#Latches*/
{"PIC12F508",	PIC12F508,	512,	0,	DS41227E,  0x0204,	0x03FF,	1},
{"PIC12F509",	PIC12F509,	1024,	0,	DS41227E,  0x0404,	0x07FF,	1},

{"PIC16F505",	PIC16F505,	1024,	0,	DS41226G,  0x0404,	0x07FF,	1},

{"PIC12F510",	PIC12F510,	1024,	0,	DS41257B,  0x0404,	0x07FF,	1},

{"PIC16F506",	PIC16F506,	1024,	0,	DS41258C,  0x0404,	0x07FF,	1},

{"PIC10F200",	PIC10F200,	256,	0,	DS41228F,  0x0104,	0x01FF,	1},
{"PIC10F202",	PIC10F202,	512,	0,	DS41228F,  0x0204,	0x03FF,	1},
{"PIC10F204",	PIC10F204,	256,	0,	DS41228F,  0x0104,	0x01FF,	1},
{"PIC10F206",	PIC10F206,	512,	0,	DS41228F,  0x0204,	0x03FF,	1},

{"PIC16F54",	PIC16F54,	512,	0,	DS41207D,  0,		0x03FF,	1},

{"PIC16F57",	PIC16F57,	2048,	0,	DS41208C,  0,		0x0FFF,	4},

{"PIC16F59",	PIC16F59,	2048,	0,	DS41243B,  0,		0x0FFF,	4},

{"PIC10F220",	PIC10F220,	256,	0,	DS41266C,  0x0104,	0x01FF,	1},
{"PIC10F222",	PIC10F222,	512,	0,	DS41266C,  0x0204,	0x03FF,	1},

{"PIC12F519",	PIC12F519,	1024,	+64,	DS41316C,  0x0444,	0x07FF,	1},

{"PIC16F570",	PIC16F570,	2048,	+64,	DS41670A,  0x0844,	0x0FFF,	4},

{"PIC16F527",	PIC16F527,	1024,	+64,	DS41640A,  0x0444,	0x07FF,	1},

{"PIC16F526",	PIC16F526,	1024,	+64,	DS41317B,  0x0444,	0x07FF,	1},

{"(null)",	0,		0,	0,	0,	   0,		0,	0}
/*Device name	Device id 	Flash	Data	Data-sheet Backup OSC	Config 	#Latches*/
};
#define PIC12_SIZE ((sizeof(pic12_map) / sizeof(struct pic12_dsmap)) - 1)

/* Default device (null) */
uint32_t pic12_index = PIC12_SIZE;

void
pic12_selector(void)
{
	uint32_t i;
	char *dnames[PIC12_SIZE];

	for (i = 0; i < PIC12_SIZE; ++i) {
		dnames[i] = pic12_map[i].devicename;
	}
	qsort(dnames, PIC12_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC12_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %d\n", (uint32_t)PIC12_SIZE);
}

/*****************************************************************************
 *
 * Program/Verify mode
 *
 *****************************************************************************/

/*
 * ENTER HVP PROGRAM/VERIFY MODE
 *
 * ENTRY - VDD FIRST
 *
 * DS41226G-page 4 PIC16F505
 * DS41228F-page 4 PIC10F200
 * DS41207D-page 3 PIC16F54
 * DS41670A-page 5 PIC16F570
 */
void
pic12_program_verify(void)
{
	/* RESET & ACQUIRE GPIO */
	io_set_vpp(LOW);
	/* DS41226G PIC16F505 TPPDP(5us) */
	/* DS41228F PIC10F200 TPPDP(5us) */
	/* DS41207D PIC16F54  TPPDP(5us) */
	/* DS41670A PIC16F570 TPPDP(5us) */
	io_usleep(1000);

	/* PGD + PGC + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_pgm(LOW);
	io_usleep(1000);

	/* INPUT DATA ON CLOCK RISING EDGE */
	io_configure(FALSE);

	/* VPP HIGH */
	io_set_vpp(HIGH);
	/* DS41226G PIC16F505 THLD0(5us) */
	/* DS41228F PIC10F200 THLD0(5us) */
	/* DS41207D PIC16F54  THLD0(5us) */
	/* DS41670A PIC16F570 THLD0(5us) */
	io_usleep(250);
}

/*
 * EXIT HVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 *
 * DS41228F-page 5 PIC10F200
 */
void
pic12_standby(void)
{
	/* PGD + PGC + VPP + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_vpp(LOW);
	io_set_pgm(LOW);
}

/*****************************************************************************
 *
 * Hardware functions
 *
 *****************************************************************************/

/*I/O*************************************************************************/

/*
 * LOAD DATA FOR PROGRAM MEMORY
 *  PC <= 0x07FF (PIC16F505)
 *  WR <= word
 *
 * XX0010 = 0x02
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 * DS41670A-page 5 16F570
 */
static inline void
pic12_load_data_for_program_memory(uint16_t word)
{
	io_program_out(0x02, 6);
	word = (word << 1) | 0x8001;
	io_program_out(word, 16);
}

/*
 * READ DATA FROM PROGRAM MEMORY
 *  RETURN (PC)
 *
 * XX0100 = 0x04
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 * DS41670A-page 5 16F570
 */
static inline uint16_t
pic12_read_data_from_program_memory(void)
{
	uint16_t word;

	io_program_out(0x04, 6);
	word = io_program_in(16);
	word = (word >> 1) & PIC12_MASK;

	return word;
}

/*
 * INCREMENT ADDRESS
 *  PC <= 1 + PC
 *
 * XX0110 = 0x06
 *
 * DS41226G-page 4 16F505
 * DS41227E-page 3 12F508
 * DS41258C-page 4 16F506
 * DS41228F-page 6 10F200
 * DS41207D-page 3 16F54
 * DS41266C-page 3 10F220
 * DS41316C-page 3 12F519
 * DS41670A-page 5 16F570
 */
static inline void
pic12_increment_address(void)
{
	io_program_out(0x06, 6);
}

/*PROG************************************************************************/

/*
 * BEGIN PROGRAMMING
 *  (PC) <= WR
 *
 * XX1000 = 0x08
 *
 * DS41226G-page 4 16F505 TPROG(2ms)
 * DS41227E-page 3 12F508 TPROG(2ms)
 * DS41258C-page 4 16F506 TPROG(2ms)
 * DS41228F-page 6 10F200 TPROG(2ms)
 * DS41207D-page 3 16F54  TPROG(2ms)
 * DS41208C-page 3 16F57  TPROG(2ms)
 * DS41266C-page 3 10F220 TPROG(2ms)
 * DS41316C-page 3 12F519 TPROG(2ms)
 * DS41670A-page 5 16F570 TPROG(2ms)
 */
static inline void
pic12_begin_programming(void)
{
	io_program_out(0x08, 6);
	io_usleep(PIC12_TPROG_DEFAULT);
}

/*
 * END PROGRAMMING
 *
 * XX1110 = 0x0E
 *
 * DS41226G-page 4 16F505 TDIS(100us)
 * DS41227E-page 3 12F508 TDIS(100us)
 * DS41258C-page 4 16F506 TDIS(100us)
 * DS41228F-page 6 10F200 TDIS(100us)
 * DS41207D-page 3 16F54  TDIS(100us)
 * DS41208C-page 3 16F57  TDIS(100us)
 * DS41266C-page 3 10F220 TDIS(100us)
 * DS41316C-page 3 12F519 TDIS(100us)
 * DS41670A-page 5 16F570 TDIS(300us)
 */
static inline void
pic12_end_programming(void)
{
	uint32_t tdis = PIC12_TDISCHARGE_DEFAULT;

	if (pic12_map[pic12_index].datasheet == DS41670A) /* PIC16F570 */
		tdis = PIC12_TDISCHARGE_300US;

	io_program_out(0x0E, 6);
	io_usleep(tdis);
}

/*ERASE***********************************************************************/

/*
 * BULK ERASE PROGRAM MEMORY
 *
 * XX1001 = 0x09
 *
 * DS41226G-page 4 16F505 TERA(10ms)
 * DS41227E-page 3 12F508 TERA(10ms)
 * DS41258C-page 4 16F506 TERA(10ms)
 * DS41228F-page 6 10F200 TERA(10ms)
 * DS41207D-page 3 16F54  TERA(10ms)
 * DS41208C-page 3 16F57  TERA(10ms)
 * DS41266C-page 3 10F220 TERA(10ms)
 * DS41316C-page 3 12F519 TERA(10ms)
 * DS41670A-page 5 16F570 TERA(10ms)
 */
static inline void
pic12_bulk_erase_program_memory(void)
{
	io_program_out(0x09, 6);
	io_usleep(PIC12_TERASE_DEFAULT);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * READ PROGRAM MEMORY
 *
 *  RETURN CODE WORD, INCREMENT PC
 */
uint16_t
pic12_read_program_memory_increment(void)
{
	uint16_t data = pic12_read_data_from_program_memory();

	pic12_increment_address();
	
	return data;
}

/*
 * ERASE DEVICE
 */
void
pic12_erase_device(void)
{
	uint32_t i;

	pic12_program_verify();

	if (pic12_map[pic12_index].dataflash == 0) {
		pic12_increment_address();		/* Skip config word */

		for (i = 0; i < pic12_map[pic12_index].flash; i++)
			pic12_increment_address();	/* Skip program flash */

		pic12_bulk_erase_program_memory();	/* Erase device */
	} else {
		/* PIC12F519 */
		/* PIC16F570 */
		/* PIC16F527 */
		pic12_bulk_erase_program_memory();	/* Erase config & program flash */

		pic12_increment_address();		/* Skip config word */

		for (i = 0; i < pic12_map[pic12_index].flash; i++)
			pic12_increment_address();	/* Skip program flash */

		pic12_bulk_erase_program_memory();	/* Erase data flash */

		for (i = 0; i < pic12_map[pic12_index].dataflash; i++)
			pic12_increment_address();	/* Skip data flash */

		pic12_bulk_erase_program_memory();	/* Erase user id & osccal */
	}

	pic12_standby();
}

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic12_bulk_erase(void)
{
	pic12_erase_device();

	if (pic12_map[pic12_index].backupaddr) {
		pic12_osccal_write(pic12_conf.osccal_backup);
	}
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY
 *
 *  READ CONFIG WORD AND USERID
 */
int
pic12_read_config_memory(void)
{
	uint32_t dev;

	/* NULL device */
	pic12_index = PIC12_SIZE;

	/* Reset configuraton */
	memset(&pic12_conf, 0, sizeof(pic12_conf));

	/* Device detect not available */
	if (!p.devicename[0]) {
		printf("%s: information: device must be selected on this architecture.\n", __func__);
		return -1;
	}

	/* Device selected by user */
	for (dev = 0; pic12_map[dev].deviceid; ++dev) {
		if (strcasecmp(pic12_map[dev].devicename, p.devicename) == 0) {
			break;
		}
	}
	if (pic12_map[dev].deviceid == 0) {
		printf("%s: information: unknown device: [%s]\n", __func__, p.devicename);
		return -1;
	}

	pic12_program_verify();
	pic12_conf.config = pic12_read_program_memory_increment();

	/*
	 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
	 * VELLEMAN K8048 NO POWER	    [3FFF]
	 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
	 */
	if (pic12_conf.config == 0x0000 || pic12_conf.config == 0x3FFF) {
		printf("%s: information: %s.\n", __func__, io_fault(pic12_conf.config));
		pic12_standby();
		return -1;
	}

	/* Device recognised */
	pic12_index = dev;

	/*
	 * Get OSCCAL / USERID / BACKUP OSCCAL
	 *
	 *  Not all devices support OSCCAL but all support USERID
	 */

	for (uint32_t i = 0; i < (pic12_map[pic12_index].flash - 1); ++i)
		pic12_increment_address();	/* Skip program flash */

	/* Get OSCCAL RESET */
	pic12_conf.osccal_reset = pic12_read_program_memory_increment();
	for (uint32_t i = 0; i < pic12_map[pic12_index].dataflash; ++i)
		pic12_increment_address();	/* Skip data flash */

	/* Get USERID 0..3 */
	pic12_conf.userid[0] = pic12_read_program_memory_increment();
	pic12_conf.userid[1] = pic12_read_program_memory_increment();
	pic12_conf.userid[2] = pic12_read_program_memory_increment();
	pic12_conf.userid[3] = pic12_read_program_memory_increment();

	/* Get OSCCAL BACKUP */
	pic12_conf.osccal_backup = pic12_read_program_memory_increment();

	pic12_standby();

	return 0;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic12_get_program_size(uint32_t *addr)
{
	*addr = 0;

	return pic12_map[pic12_index].flash;    
}

/*
 * GET DATA EEPROM/FLASH SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic12_get_data_size(uint32_t *addr)
{
	*addr = pic12_map[pic12_index].flash;

	return pic12_map[pic12_index].dataflash;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic12_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t i;

	pic12_program_verify();
	pic12_increment_address(); /* Skip config word */

	for (i = 0; i < addr; ++i)
		pic12_increment_address();
	for (i = 0; i < size; ++i)
		data[i] = (uint32_t)pic12_read_program_memory_increment();

	pic12_standby();

	return addr;
}

/*
 * READ DATA EEPROM/FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic12_read_data_memory_block(uint16_t *data, uint32_t addr, uint16_t size)
{
	uint32_t i;

	pic12_program_verify();
	pic12_increment_address(); /* Skip config word */

	for (i = 0; i < addr; ++i)
		pic12_increment_address();
	for (i = 0; i < size; ++i)
		data[i] = (uint16_t)pic12_read_program_memory_increment();

	pic12_standby();

	return addr;
}

/*****************************************************************************
 *
 * Read/Write Calibration 
 *
 *****************************************************************************/

/*
 * WRITE OSCILLATOR CALIBRATION WORD
 */
uint32_t
pic12_osccal_write(uint16_t osccal)
{
	uint32_t i;
	
	pic12_program_verify();
	
	pic12_increment_address();		/* Skip config word */
	for (i = 0; i < (pic12_map[pic12_index].flash - 1); i++)
		pic12_increment_address();	/* Skip program flash memory */

	/*
	 * Write OSCCAL RESET
	 */
	pic12_load_data_for_program_memory(osccal);
	pic12_begin_programming();
	pic12_end_programming();
	uint16_t vdata = pic12_read_data_from_program_memory();
	if (vdata != osccal) {
		pic12_standby();
		printf("%s: error: OSCCAL RESET write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, osccal);
		return 0;
	}
	pic12_increment_address();		/* Skip OSCCAL RESET */

	for (i = 0; i < pic12_map[pic12_index].dataflash; i++)
		pic12_increment_address();	/* Skip data flash */

	pic12_increment_address();		/* Skip USERID0 */
	pic12_increment_address();		/* Skip USERID1 */
	pic12_increment_address();		/* Skip USERID2 */
	pic12_increment_address();		/* Skip USERID3 */

	/*
	 * Write OSCCAL BACKUP
	 */
	pic12_load_data_for_program_memory(osccal);
	pic12_begin_programming();
	pic12_end_programming();
	vdata = pic12_read_data_from_program_memory();
	if (vdata != osccal) {
		pic12_standby();
		printf("%s: error: OSCCAL BACKUP write failed: read [%04X] expected [%04X]\n",
			__func__, vdata, osccal);
		return 0;
	}

	pic12_standby();

	return 1; /* OSCCAL WRITTEN */
}

/*****************************************************************************
 *
 * Write Calibration to Blank Device
 *
 *****************************************************************************/

/*
 * WRITE OSCILLATOR CALIBRATION WORD
 */
uint32_t
pic12_write_osccal(uint16_t osccal)
{
	if (pic12_map[pic12_index].backupaddr == 0) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
		return 0;
	}

	pic12_erase_device();

	return pic12_osccal_write(osccal);
}

/*****************************************************************************
 *
 * Program Config
 *
 *****************************************************************************/

/*
 * WRITE CONFIG
 */
uint32_t
pic12_write_config(void)
{
	pic12_program_verify();

	/* Store config in working register */
	pic12_load_data_for_program_memory(pic12_conf.config);

	/* Program working register */
	pic12_begin_programming();
	pic12_end_programming();

	pic12_standby();

	return 1;
}

/*****************************************************************************
 *
 * Region control
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE (PROGRAM FLASH + DATA FLASH + USERID) OR CONFIG
 *
 *  RETURN PIC_REGIONCODE:
 *  	0 .. FLASH SIZE
 *
 *  RETURN PIC_REGIONCONFIG:
 *  	0xFFF
 */
uint16_t
pic12_getregion(uint16_t address)
{
	/* CODE: PROGRAM FLASH/DATA FLASH/USERID */
	uint16_t code_high = pic12_map[pic12_index].flash + pic12_map[pic12_index].dataflash + 3;

	if (address <= code_high) {
		return PIC_REGIONCODE;
	}
	/* CONFIG */
	if (address == PIC12_CONFIG) {
		return PIC_REGIONCONFIG;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: address unsupported [%04X]\n", __func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INITIALISE FOR REGION
 */
uint16_t
pic12_initregion(uint16_t region, uint16_t *address)
{
	switch (region) {
	case PIC_REGIONCODE:
		/* Skip config word */
		pic12_increment_address();
		*address = 0;
		return region;
	case PIC_REGIONCONFIG:
		*address = PIC12_CONFIG;
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	*address = 0;
	return PIC_REGIONNOTSUP;
}

/*
 * LOAD DATA FOR REGION (STORE WORD IN WORKING REGISTER)
 */
void
pic12_loadregion(uint16_t region, uint16_t word)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONCONFIG:
		pic12_load_data_for_program_memory(word);
		return;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
}

/*****************************************************************************
 *
 * Program & verify region
 *
 *****************************************************************************/

/*
 * PROGRAM DATA FOR REGION (CACHE CONFIG)
 */
static inline uint16_t
pic12_programregion(uint16_t address, uint16_t region, uint16_t data)
{
	static int write_pending = 0;

	switch (region) {
        case PIC_REGIONCONFIG:
		pic12_conf.config = data;
		return region;
	case PIC_REGIONCODE:
		if (data != PIC_VOID) {
			if (pic12_map[pic12_index].backupaddr) {
				if (address == (pic12_map[pic12_index].flash - 1))
					break;
				if (address == pic12_map[pic12_index].backupaddr)
					break;
			}
			write_pending = 1;
			pic12_loadregion(PIC_REGIONCODE, data);
		}
		uint16_t mask = pic12_map[pic12_index].nlatches - 1;
		if (write_pending && ((address & mask) == mask)) {
			write_pending = 0;
			pic12_begin_programming();
			pic12_end_programming();
		}
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%04X] [%d]\n", __func__, address, region);
	return PIC_REGIONNOTSUP;
}

/*
 * VERIFY DATA FOR REGION
 *
 *  RETURN BYTE FAILURE COUNT
 */
static inline uint16_t
pic12_verifyregion(uint16_t address, uint16_t region, uint16_t wdata)
{
	uint16_t vdata = 0;

	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONCONFIG:
		vdata = pic12_read_data_from_program_memory();
		break;
	default:if (p.f)
			fprintf(p.f, "%s: warning: region unsupported [%d]\n",
				__func__, region);
		return wdata;
	}
	if (vdata != wdata && p.f) {
		fprintf(p.f, "%s: error: read [%04X] expected [%04X] at [%04X]\n",
			__func__, vdata, wdata, address);
	}
	return vdata;
}

/*****************************************************************************
 *
 * Program & verify
 *
 *****************************************************************************/

/*
 * PROGRAM DATA
 */
uint32_t
pic12_program_data(uint32_t current_region, pic_data *pdata)
{
	static uint16_t PC_address = 0;
	uint16_t address, new_region, wdata;

	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		address = (pdata->address + i) >> 1;
		new_region = pic12_getregion(address);
		if (new_region != current_region) {
			pic12_programregion(PIC_VOID, PIC_REGIONCODE, PIC_VOID);
			pic12_program_verify(); /* PC RESET */
			current_region = pic12_initregion(new_region, &PC_address);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		while (address > PC_address) {
			pic12_programregion(PC_address, PIC_REGIONCODE, PIC_VOID);
			pic12_increment_address();
			PC_address++;
		}
		wdata = pdata->bytes[i] |
			(pdata->bytes[i + 1] << 8);
		wdata &= PIC12_MASK;
		pic12_programregion(address, current_region, wdata);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic12_program_end(int config)
{
	pic12_programregion(PIC_VOID, PIC_REGIONCODE, PIC_VOID);
	pic12_standby();
	if (config)
		pic12_write_config();
}

/*
 * VERIFY DATA
 */
uint32_t
pic12_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	static uint16_t PC_address = 0;
	uint16_t address, new_region, wdata, vdata;

	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		address = (pdata->address + i) >> 1;
		new_region = pic12_getregion(address);
		if (new_region != current_region) {
			pic12_program_verify(); /* PC RESET */
			current_region = pic12_initregion(new_region, &PC_address);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		while (address > PC_address) {
			pic12_increment_address();
			PC_address++;
		}
		wdata = pdata->bytes[i] |
			(pdata->bytes[i + 1] << 8);
		wdata &= PIC12_MASK;
		vdata = pic12_verifyregion(address, current_region, wdata);
		if (vdata != wdata) {
			pdata->bytes[i] = vdata;
			pdata->bytes[i + 1] = vdata >> 8;
			(*fail) += 2;
		}
	}
	return current_region;
}

/*
 * VIEW DATA
 */
void
pic12_view_data(pic_data *pdata)
{
	uint16_t wdata;

	printf("[%04X] ", pdata->address >> 1);
	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		wdata = pdata->bytes[i] | (pdata->bytes[i + 1] << 8);
		wdata &= PIC12_MASK;
		printf("%04X ", wdata);
	}
	putchar('\n');
}

/*****************************************************************************
 *
 * Diagnostic functions
 *
 *****************************************************************************/

/*
 * DUMP DEVICE ID DETAILS
 */
void
pic12_dumpdeviceid(void)
{
	printf("[0000] [PROGRAM]  %04X WORDS\n", pic12_map[pic12_index].flash);

	if (pic12_map[pic12_index].backupaddr) {
		printf("[%04X] [OSCCAL]   %04X\n",
			pic12_map[pic12_index].flash - 1,
			pic12_conf.osccal_reset);
	}

	if (pic12_map[pic12_index].dataflash) {
		printf("[%04X] [DATA]     %04X BYTES\n",
			pic12_map[pic12_index].flash,
			pic12_map[pic12_index].dataflash);
	}

	for (uint32_t i = 0; i < 4; i++) {
		printf("[%04X] [USERID%d]  %04X %c\n",
			pic12_map[pic12_index].flash + pic12_map[pic12_index].dataflash + i, i,
			pic12_conf.userid[i], PIC_CHAR(0xFF & pic12_conf.userid[i]));
	}

	if (pic12_map[pic12_index].backupaddr) {
		printf("[%04X] [BACKUP]   %04X\n",
			pic12_map[pic12_index].backupaddr,
			pic12_conf.osccal_backup);
	}

	pic12_dumpconfig(PIC_BRIEF);
	printf("       [DEVICEID] %s\n", pic12_map[pic12_index].devicename);
}

/*
 * DUMP OSCCAL DETAILS
 */
void
pic12_dumposccal(void)
{
	if (pic12_map[pic12_index].backupaddr == 0) {
		printf("%s: error: OSCCAL is not supported on this device\n", __func__);
	} else {
		printf("[%04X] [OSCCAL]   %04X\n",
			pic12_map[pic12_index].flash - 1,
			pic12_conf.osccal_reset);
		printf("[%04X] [BACKUP]   %04X\n",
			pic12_map[pic12_index].backupaddr,
			pic12_conf.osccal_backup);
	}
}

/*
 * DUMP CONFIG WORD DETAILS
 */
void
pic12_dumpconfig(int mode)
{
	printf("[%04X] [CONFIG]   %04X\n",
		PIC12_CONFIG, pic12_conf.config);
#if VERBOSE
	if (mode == PIC_VERBOSE)
		pic12_dumpconfig_verbose();
#endif
}

/*
 * DUMP VERBOSE CONFIG WORD DETAILS FOR DEVICE
 */
#ifdef VERBOSE
void
pic12_dumpconfig_verbose(void)
{
	/* UNIMPLEMENTED */
}
#endif /* VERBOSE */

/*
 * DUMP HEX FLASH WORDS
 */
void
pic12_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtcode(PIC12_MASK, 8, &data[i]))
			continue;
		printf("[%04X] ", address);
		for (j = 0; j < 8; ++j)
			printf("%03X ", data[i + j] & PIC12_MASK);
		for (j = 0; j < 8; ++j)
			putchar(PIC_CHAR(0xFF & data[i + j]));
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: flash empty\n", __func__);
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic12_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* 12-bit: Extended address = 0x0000 */
	pic_dumpaddr(0, 1);

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtcode(PIC12_MASK, 8, &data[i]))
			continue;

		uint8_t cc, hb, lb;
		hb = address >> 7;
		lb = address << 1;
		printf(":%02X%02X%02X00", 16, hb, lb);
		cc = 16 + hb + lb + 0x00;
		for (j = 0; j < 8; ++j) {
			lb = data[i + j];
			hb = data[i + j] >> 8;
			printf("%02X%02X", lb, hb);
			cc = cc + lb + hb;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}

/*
 * DUMP HEX DATA FLASH WORDS
 */
void
pic12_dumphexdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 16) {
		if (pic_mtdata(0xFFF, 16, &data[i]))
			continue;
		printf("[%04X] ", address);
		for (j = 0; j < 16; ++j)
			printf("%02X ", data[i + j]);
		for (j = 0; j < 16; ++j)
			putchar(PIC_CHAR(0xFF & data[i + j]));
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: data empty\n", __func__);
}

/*
 * DUMP INHX32 DATA FLASH WORDS
 */
void
pic12_dumpinhxdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j;

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtdata(0xFFF, 8, &data[i]))
			continue;

		uint8_t cc, hb, lb;
		hb = address >> 7;
		lb = address << 1;
		printf(":%02X%02X%02X00", 16, hb, lb);
		cc = 16 + hb + lb + 0x00;
		for (j = 0; j < 8; ++j) {
			lb = data[i + j];
			printf("%02X00", lb);
			cc = cc + lb + 0x00;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}

/*
 * DUMP DEVICE CONFIGURATION
 */
void
pic12_dumpdevice(void)
{
	/* 12-bit: Extended address = 0x0000 */
	pic_dumpaddr(0, 1);

	/* IDLOC */
	uint16_t address = pic12_map[pic12_index].flash + pic12_map[pic12_index].dataflash;
	for (uint32_t i = 0; i < 4; ++i) {
		pic_dumpword16(address + i, pic12_conf.userid[i]);
	}
	/* CONFIG */
	pic_dumpword16(PIC12_CONFIG, pic12_conf.config);
}
