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

struct pic_ops pic14n_ops = {
	.arch				= ARCH14BIT,
	.align				= sizeof(uint16_t),
	.selector			= pic14n_selector,
	.program_begin			= pic14n_program_begin,
	.program_data			= pic14n_program_data,
	.program_end			= pic14n_program_end,
	.verify_begin			= pic14n_program_verify,
	.verify_data			= pic14n_verify_data,
	.verify_end			= pic14n_standby,
	.view_data			= pic14n_view_data,
	.read_config_memory		= pic14n_read_config_memory,
	.get_program_size		= pic14n_get_program_size,
	.get_data_size			= pic14n_get_data_size,
	.get_executive_size		= NULL,
	.get_boot_size			= NULL,
	.read_program_memory_block	= pic14n_read_program_memory_block,
	.read_data_memory_block		= pic14n_read_data_memory_block,
	.write_panel			= pic14n_write_panel,
	.bulk_erase			= pic14n_bulk_erase,
	.write_osccal			= NULL,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= NULL,
	.dumpdeviceid			= pic14n_dumpdeviceid,
	.dumpconfig			= pic14n_dumpconfig,
	.dumposccal			= NULL,
	.dumpdevice			= pic14n_dumpdevice,
	.dumpadj			= 1,
	.dumphexcode			= pic14n_dumphexcode,
	.dumpinhxcode			= pic14n_dumpinhxcode,
	.dumphexdata			= pic14n_dumphexdata,
	.dumpinhxdata			= pic14n_dumpinhxdata,
};

uint32_t
pic14n_arch(void)
{
	p.pic = &pic14n_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic14n_config pic14n_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic14n_dsmap pic14n_map[] =
{
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches	Inf/Con/Cal*/
{"PIC16F18854", PIC16F18854,    DS40001753B,	PIC14N_WORD(7),  5,      	256,	32,	0,0,4},
{"PIC16LF18854",PIC16LF18854,   DS40001753B,	PIC14N_WORD(7),  5,      	256,	32,	0,0,4},
{"PIC16F18855", PIC16F18855,    DS40001753B,	PIC14N_WORD(14), 5,      	256,	32,	0,0,4},
{"PIC16LF18855",PIC16LF18855,   DS40001753B,	PIC14N_WORD(14), 5,      	256,	32,	0,0,4},
{"PIC16F18875", PIC16F18875,    DS40001753B,	PIC14N_WORD(14), 5,      	256,	32,	0,0,4},
{"PIC16LF18875",PIC16LF18875,   DS40001753B,	PIC14N_WORD(14), 5,      	256,	32,	0,0,4},
{"PIC16F18856", PIC16F18856,    DS40001753B,	PIC14N_WORD(28), 5,      	256,	32,	0,0,4},
{"PIC16LF18856",PIC16LF18856,   DS40001753B,	PIC14N_WORD(28), 5,      	256,	32,	0,0,4},
{"PIC16F18876", PIC16F18876,    DS40001753B,	PIC14N_WORD(28), 5,      	256,	32,	0,0,4},
{"PIC16LF18876",PIC16LF18876,   DS40001753B,	PIC14N_WORD(28), 5,      	256,	32,	0,0,4},
{"PIC16F18857", PIC16F18857,    DS40001753B,	PIC14N_WORD(56), 5,      	256,	32,	0,0,4},
{"PIC16LF18857",PIC16LF18857,   DS40001753B,	PIC14N_WORD(56), 5,      	256,	32,	0,0,4},
{"PIC16F18877", PIC16F18877,    DS40001753B,	PIC14N_WORD(56), 5,      	256,	32,	0,0,4},
{"PIC16LF18877",PIC16LF18877,   DS40001753B,	PIC14N_WORD(56), 5,      	256,	32,	0,0,4},

{"PIC16F15313", PIC16F15313,    DS40001838B,	PIC14N_WORD(3.5),5,      	0,	32,	32,5,4},
{"PIC16LF15313",PIC16LF15313,   DS40001838B,	PIC14N_WORD(3.5),5,      	0,	32,	32,5,4},
{"PIC16F15323", PIC16F15323,    DS40001838B,	PIC14N_WORD(3.5),5,      	0,	32,	32,5,4},
{"PIC16LF15323",PIC16LF15323,   DS40001838B,	PIC14N_WORD(3.5),5,      	0,	32,	32,5,4},
{"PIC16F15324", PIC16F15324,    DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16LF15324",PIC16LF15324,   DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16F15344", PIC16F15344,    DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16LF15344",PIC16LF15344,   DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16F15325", PIC16F15325,    DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16LF15325",PIC16LF15325,   DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16F15345", PIC16F15345,    DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16LF15345",PIC16LF15345,   DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16F15354", PIC16F15354,    DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16LF15354",PIC16LF15354,   DS40001838B,	PIC14N_WORD(7),  5,      	0,	32,	32,5,4},
{"PIC16F15355", PIC16F15355,    DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16LF15355",PIC16LF15355,   DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16F15356", PIC16F15356,    DS40001838B,	PIC14N_WORD(28), 5,      	0,	32,	32,5,4},
{"PIC16LF15356",PIC16LF15356,   DS40001838B,	PIC14N_WORD(28), 5,      	0,	32,	32,5,4},
{"PIC16F15375", PIC16F15375,    DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16LF15375",PIC16LF15375,   DS40001838B,	PIC14N_WORD(14), 5,      	0,	32,	32,5,4},
{"PIC16F15376", PIC16F15376,    DS40001838B,	PIC14N_WORD(28), 5,      	0,	32,	32,5,4},
{"PIC16LF15376",PIC16LF15376,   DS40001838B,	PIC14N_WORD(28), 5,      	0,	32,	32,5,4},

{"(null)",      0,              0,              0,               0,             0,      0,      0,0,0},
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches	Inf/Con/Cal*/
};
#define PIC14N_SIZE ((sizeof(pic14n_map) / sizeof(struct pic14n_dsmap)) - 1)

/* Default device (null) */
uint32_t pic14n_index = PIC14N_SIZE;

void
pic14n_selector(void)
{
	uint32_t i;
	char *dnames[PIC14N_SIZE];

	for (i = 0; i < PIC14N_SIZE; ++i) {
		dnames[i] = pic14n_map[i].devicename;
	}
	qsort(dnames, PIC14N_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC14N_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC14N_SIZE);
}

/*****************************************************************************
 *
 * Program/Verify mode
 *
 *****************************************************************************/

/*
 * ENTER HVP/LVP PROGRAM/VERIFY MODE
 *
 * ENTRY - VDD FIRST
 */
void
pic14n_program_verify(void)
{
	/* RESET & ACQUIRE GPIO */
	io_set_vpp(LOW);
	/* DS40001753B TENTS(100ns) */
	io_usleep(1000);

	/* PGD + PGC + PGM(N/A) LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_pgm(LOW);
	io_usleep(1000);

	/* INPUT DATA ON CLOCK RISING EDGE, MSB FIRST */
	io_configure(FALSE, TRUE);

	/* LVP */
	if (p.key == LVPKEY) {

		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* TENTH(250ns) */
		io_usleep(250);

		/* VPP LOW */
		io_set_vpp(LOW);
		/* TENTH(250ns) */
		io_usleep(250);

		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(MCHPKEY, 32);
		/* UNSPECIFIED */
		io_usleep(250);
	}
	/* HVP */
	else {
		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* DS40001753B TENTS(100ns) */
		io_usleep(250);
	}
}

/*
 * EXIT HVP/LVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic14n_standby(void)
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
 * LOAD DATA FOR NVM
 *  0x00 PC = PC
 *  0x02 PC = PC + 1
 *
 * DS40001753B-page 10/11
 */
static inline void
pic14n_load_data_for_nvm(uint32_t word, uint8_t j /* 0 || 1 */)
{
	io_program_out(j << 1, 8);
	/* TDLY 1us */
	io_program_out(word << 1, 24);
	/* TDLY 1us */
}

/*
 * READ DATA FROM NVM
 *  0xFC PC = PC
 *  0xFE PC = PC + 1
 *
 * DS40001753B-page 10/11
 */
static inline uint32_t
pic14n_read_data_from_nvm(uint8_t j /* 0 || 1 */)
{
	uint32_t word;

	io_program_out(0xFC | (j << 1), 8);
	/* TDLY 1us */
	word = io_program_in(24);
	/* TDLY 1us */

	word = (word >> 1) & PIC14N_MASK;

	return word;
}

/*
 * INCREMENT ADDRESS
 *  0xF8 PC = PC + 1
 *
 * DS40001753B-page 13
 */
static inline void
pic14n_increment_address(void)
{
	io_program_out(0xF8, 8);
	/* TDLY 1us */
}

/*
 * LOAD PC ADDRESS
 *  0x80 PC = address
 *
 * DS40001753B-page 13
 */
static inline void
pic14n_load_pc_address(uint32_t address)
{
	io_program_out(0x80, 8);
	/* TDLY 1us */
	io_program_out(address << 1, 24);
	/* TDLY 1us */
}

/*PROG************************************************************************/

/*
 * BEGIN INTERNALLY TIMED PROGRAMMING
 *  0xE0
 *
 * DS40001753B-page 14
 *
 * DS40001753B TPINT(2.8ms PROGRAM) TPINT(5.6ms CONFIG)
 */
static inline void
pic14n_begin_internally_timed_programming(uint32_t t)
{
	io_program_out(0xE0, 8);
	/* TPINT */
	io_usleep(t);
}

/*
 * BEGIN EXTERNALLY TIMED PROGRAMMING
 *  0xC0
 *
 * DS40001753B-page 14
 *
 * DS40001753B TPEXT(2.8ms PROGRAM) (NOT CONFIG)
 */
static inline void
pic14n_begin_externally_timed_programming(uint32_t t)
{
	io_program_out(0xC0, 8);
	/* TPEXT */
	io_usleep(t);
}

/*
 * END EXTERNALLY TIMED PROGRAMMING
 *  0x82
 *
 * DS40001753B-page 14
 *
 * DS40001753B TDIS(300us PROGRAM)
 */
static inline void
pic14n_end_externally_timed_programming(uint32_t t)
{
	io_program_out(0x82, 8);
	/* TDIS */
	io_usleep(t);
}

/*ERASE***********************************************************************/

/*
 * BULK ERASE MEMORY (PRELOAD PC)
 *  0x18
 *
 * PC = 0x0000 ERASE PROGRAM AND CONFIG
 * PC = 0x8000 ERASE PROGRAM AND CONFIG AND USERID
 * PC = 0xF000 ERASE EEPROM
 *
 * DS40001753B-page 15
 *
 * DS40001753B TERAB(8.4ms PIC16F188X6)
 */
static inline void
pic14n_bulk_erase_memory(uint32_t t)
{
	io_program_out(0x18, 8);
	io_usleep(t);
}

/*
 * ROW ERASE MEMORY (PRELOAD PC)
 *  0xF0
 *
 * DS40001753B-page 15
 *
 * DS40001753B TERAR(2.8ms)
 */
static inline void
pic14n_row_erase_memory(uint32_t t)
{
	io_program_out(0xF0, 8);
	io_usleep(t);
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

void
pic14n_bulk_erase(void)
{
	pic14n_program_verify();

	pic14n_load_pc_address(PIC14N_USERID_ADDR);
	pic14n_bulk_erase_memory(PIC14N_TERAB);

	if (pic14n_map[pic14n_index].eeprom) {
		pic14n_load_pc_address(PIC14N_EEPROM_ADDR);
		pic14n_bulk_erase_memory(PIC14N_TERAB);
	}

	pic14n_standby();
}

#if 0
void
pic14n_row_erase(uint32_t row, uint32_t nrows)
{
	fprintf(stderr, "%s()\n", __func__);
}
#endif

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY 8000 .. 8XXX AND STORE DEVICE MAP INDEX
 */
int
pic14n_read_config_memory(void)
{
	uint32_t dev;

	/* NULL device */
	pic14n_index = PIC14N_SIZE;

	/* Reset configuraton */
	memset(&pic14n_conf, 0, sizeof(pic14n_conf));

	pic14n_program_verify();
	pic14n_load_pc_address(PIC14N_USERID_ADDR);

	for (uint32_t i = 0; i < PIC14N_USERID_MAX; ++i)
		pic14n_conf.userid[i] = pic14n_read_data_from_nvm(1);	/* Userid     */
	pic14n_conf.reserved = pic14n_read_data_from_nvm(1);		/* Reserved   */
	pic14n_conf.revisionid = pic14n_read_data_from_nvm(1);		/* Revisionid */
	pic14n_conf.deviceid = pic14n_read_data_from_nvm(1);		/* Deviceid   */

	/* Device detect */
	for (dev = 0; pic14n_map[dev].deviceid; ++dev) {
		if (pic14n_map[dev].deviceid == pic14n_conf.deviceid) {
			break;
		}
	}
	if (pic14n_map[dev].deviceid == 0) {
		/*
		 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
		 * VELLEMAN K8048 NO POWER          [3FFF]
		 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
		 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
		 */
		if (pic14n_conf.deviceid == 0x0000 || pic14n_conf.deviceid == 0x3FFF) {
			printf("%s: information: %s.\n",
				__func__, io_fault(pic14n_conf.deviceid));
		} else {
			printf("%s: information: device unknown: [%04X]\n",
				__func__, pic14n_conf.deviceid);
		}
		pic14n_standby();
		return -1;
	}

	/* Device recognised */
	pic14n_index = dev;

	/* Config word(s) */
	for (uint32_t i = 0; i < pic14n_map[pic14n_index].config; ++i)
		pic14n_conf.config[i] = pic14n_read_data_from_nvm(1);

	/* Device information area */
	if (pic14n_map[pic14n_index].devinfo) {
		pic14n_load_pc_address(PIC14N_DEVINFO_ADDR);
		for (uint32_t i = 0; i < pic14n_map[pic14n_index].devinfo; ++i)
			pic14n_conf.devinfo[i] = pic14n_read_data_from_nvm(1);
	}

	/* Device configuration information */
	if (pic14n_map[pic14n_index].devconf) {
		pic14n_load_pc_address(PIC14N_DEVCONF_ADDR);
		for (uint32_t i = 0; i < pic14n_map[pic14n_index].devconf; ++i)
			pic14n_conf.devconf[i] = pic14n_read_data_from_nvm(1);
	}

	/* Device calibration */
	if (pic14n_map[pic14n_index].calib) {
		pic14n_load_pc_address(PIC14N_CALIB_ADDR);
		for (uint32_t i = 0; i < pic14n_map[pic14n_index].calib; ++i)
			pic14n_conf.calib[i] = pic14n_read_data_from_nvm(1);
	}

	pic14n_standby();

	return 0;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic14n_get_program_size(uint32_t *addr)
{
	*addr = 0;

	return pic14n_map[pic14n_index].flash;
}

/*
 * GET DATA EEPROM/FLASH SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic14n_get_data_size(uint32_t *addr)
{
	*addr = PIC14N_EEPROM_ADDR;

	return pic14n_map[pic14n_index].eeprom;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic14n_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t i;

	pic14n_program_verify();
	pic14n_load_pc_address(addr);

	for (i = 0; i < size; i++)
		data[i] = (uint32_t)pic14n_read_data_from_nvm(1);

	pic14n_standby();

	return addr;
}

/*
 * READ DATA EEPROM BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic14n_read_data_memory_block(uint16_t *data, uint32_t addr, uint16_t size)
{
	uint32_t i;

	pic14n_program_verify();
	pic14n_load_pc_address(addr);

	for (i = 0; i < size; i++)
		data[i] = (uint16_t)pic14n_read_data_from_nvm(1);

	pic14n_standby();

	return addr;
}

/*****************************************************************************
 *
 * Program Code Panel
 *
 *****************************************************************************/

/*
 * WRITE PANEL
 */
void
pic14n_write_panel(uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	for (uint32_t i = 0; i < panel_size; ++i)
		pic14n_load_data_for_nvm(panel[i], 1);

	pic14n_load_pc_address(address);
	pic14n_begin_internally_timed_programming(PIC14N_TPINT_CODE);
}

/*****************************************************************************
 *
 * Program Data/EEPROM
 *
 *****************************************************************************/

/*
 * WRITE Data/EEPROM BYTE
 */
static inline
void
pic14n_write_data_memory(uint32_t address, uint8_t data)
{
	pic14n_load_pc_address(address);
	pic14n_load_data_for_nvm(data, 0);
	pic14n_begin_internally_timed_programming(PIC16N_TPINT_CONFIG);
}

/*****************************************************************************
 *
 * Program UserID/Config
 *
 *****************************************************************************/

/*
 * WRITE USERID/CONFIG WORD
 *  
 *  RETURN WORD
 */
static inline
uint16_t
pic14n_write_config_word(uint16_t word)
{
	pic14n_load_data_for_nvm(word, 0);
	pic14n_begin_internally_timed_programming(PIC14N_TPINT_CONFIG);

	return pic14n_read_data_from_nvm(1);
}

/*
 * WRITE CONFIG
 *
 *  RETURN 0 ON ERROR ELSE PROGRAM COUNT
 */
uint32_t
pic14n_write_config(void)
{
	uint16_t vdata;

	pic14n_program_verify();

	pic14n_load_pc_address(PIC14N_USERID_ADDR);
	for (uint32_t i = 0; i < PIC14N_USERID_MAX; ++i) {
		vdata = pic14n_write_config_word(pic14n_conf.userid[i]);
		if (vdata != pic14n_conf.userid[i]) {
			printf("%s: error: USERID%d write failed: read [%04X] expected [%04X]\n",
				__func__, i, vdata, pic14n_conf.userid[i]);
			pic14n_standby();
			return 0;
		}
	}

	pic14n_load_pc_address(PIC14N_CONFIG_ADDR);
	for (uint32_t i = 0; i < PIC14N_CONFIG_MAX; ++i) {
		vdata = pic14n_write_config_word(pic14n_conf.config[i]);
		if (vdata != pic14n_conf.config[i]) {
			printf("%s: error: CONFIG%d write failed: read [%04X] expected [%04X]\n",
				__func__, i + 1, vdata, pic14n_conf.config[i]);
			pic14n_standby();
			return 0;
		}
	}

	pic14n_standby();

	return (PIC14N_USERID_MAX + PIC14N_CONFIG_MAX) << 1;
}

/*****************************************************************************
 *
 * Programming
 *
 *****************************************************************************/

/*
 * DETERMINE MEMORY REGION: CODE, ID, CONFIG or DATA
 *
 *  RETURN PIC_REGIONCODE:
 *      0 .. FLASH SIZE - 1
 *
 *  RETURN PIC_REGIONID:
 *      0x8000 .. 0x8003
 *
 *  RETURN PIC_REGIONCONFIG:
 *      0x8007 .. 0x80XX
 *
 *  RETURN PIC_REGIONDATA:
 *      0xF000 .. 0xFXXX
 */
uint16_t
pic14n_getregion(uint32_t address)
{
	/* CODE */
	if (address < pic14n_map[pic14n_index].flash) {
		return PIC_REGIONCODE;
	}
	/* ID */
	if (address >= PIC14N_USERID_ADDR && address < (PIC14N_USERID_ADDR + PIC14N_USERID_MAX)) {
		return PIC_REGIONID;
	}
	/* CONFIG */
	uint16_t config_high = PIC14N_CONFIG_ADDR + pic14n_map[pic14n_index].config;
	if (address >= PIC14N_CONFIG_ADDR && address < config_high) {
		return PIC_REGIONCONFIG;
	}
	/* DATA EEPROM */
	if (pic14n_map[pic14n_index].eeprom) {
		uint16_t data_high = PIC14N_EEPROM_ADDR + pic14n_map[pic14n_index].eeprom;
		if (address >= PIC14N_EEPROM_ADDR && address < data_high)
			return PIC_REGIONDATA;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: address unsupported [%04X]\n", __func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING (ID & CONFIG DELAYED)
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
static inline uint32_t
pic14n_init_writeregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic14n_load_pc_address(0); /* Reset latch address */
		pic_write_panel(PIC_PANEL_BEGIN, region, pic14n_map[pic14n_index].latches);
		/* Follow through */
	case PIC_REGIONID:
	case PIC_REGIONCONFIG:
	case PIC_REGIONDATA:
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * WRITE REGION (CACHE ID & CONFIG)
 */
static inline void
pic14n_writeregion(uint32_t address, uint32_t region, uint16_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic_write_panel(PIC_PANEL_UPDATE, address, data);
		return;
	case PIC_REGIONID:
		pic14n_conf.userid[address - PIC14N_USERID_ADDR] = data;
		return;
	case PIC_REGIONCONFIG:
		pic14n_conf.config[address - PIC14N_CONFIG_ADDR] = data;
		return;
	case PIC_REGIONDATA:
		pic14n_write_data_memory(address, data);
		return;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
}

/*
 * INIT REGION FOR VERIFY
 *
 *  RETURN REGION IF VERIFY SUPPORTED
 */
static inline uint32_t
pic14n_init_verifyregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
	case PIC_REGIONCONFIG:
	case PIC_REGIONDATA:
		/* Nothing to do on PIC14 new */
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * VERIFY REGION
 *
 *  RETURN DATA
 */
static inline uint32_t
pic14n_verifyregion(uint32_t address, uint32_t region, uint16_t index, uint16_t wdata)
{
	uint16_t vdata = 0;

	if (region == PIC_REGIONNOTSUP) {
		if (p.f)
			fprintf(p.f, "%s: warning: region unsupported [%d]\n",
				__func__, region);
		return wdata;
	}
	if (index == 0)
		pic14n_load_pc_address(address);
	vdata = pic14n_read_data_from_nvm(1);
	if (vdata != wdata && p.f) {
		printf("%s: error: read [%04X] expected [%04X] at [%04X]\n",
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
 * BEGIN PROGRAMMING
 */
void
pic14n_program_begin(void)
{
	pic14n_program_verify();

	memset(pic14n_conf.userid, -1, sizeof(uint16_t) * PIC14N_USERID_MAX);
	memset(pic14n_conf.config, -1, sizeof(uint16_t) * PIC14N_CONFIG_MAX);
}

/*
 * PROGRAM DATA
 */
uint32_t
pic14n_program_data(uint32_t current_region, pic_data *pdata)
{
	uint32_t address, new_region;
	uint16_t wdata;

	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		address = (pdata->address + i) >> 1;
		new_region = pic14n_getregion(address);
		if (new_region != current_region) {
			pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic14n_init_writeregion(new_region);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		wdata = pdata->bytes[i] |
			(pdata->bytes[i + 1] << 8);
		wdata &= PIC14N_MASK;
		pic14n_writeregion(address, current_region, wdata);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic14n_program_end(int config)
{
	pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
	pic14n_standby();
	if (config)
		pic14n_write_config();
}

/*
 * VERIFY DATA
 */
uint32_t
pic14n_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	uint32_t address, new_region;
	uint16_t wdata, vdata;

	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		address = (pdata->address + i) >> 1;
		new_region = pic14n_getregion(address);
		if (new_region != current_region)
			current_region = pic14n_init_verifyregion(new_region);
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		wdata = pdata->bytes[i] |
			(pdata->bytes[i + 1] << 8);
		wdata &= PIC14N_MASK;
		vdata = pic14n_verifyregion(address, current_region, i, wdata);
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
pic14n_view_data(pic_data *pdata)
{
	uint16_t wdata;

	printf("[%04X] ", pdata->address >> 1);
	for (uint32_t i = 0; i < pdata->nbytes; i += 2) {
		wdata = pdata->bytes[i] | (pdata->bytes[i + 1] << 8);
		wdata &= PIC14_MASK;
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
pic14n_dumpdeviceid(void)
{
	printf("[0000] [PROGRAM]  %04X WORDS", pic14n_map[pic14n_index].flash);
	printf(" (%04X ROWS OF %04X WORDS)\n",
		pic14n_map[pic14n_index].flash / pic14n_map[pic14n_index].latches,
		pic14n_map[pic14n_index].latches);

	for (uint32_t i = 0; i < PIC14N_USERID_MAX; i++) {
		printf("[%04X] [USERID%d]  ", PIC14N_USERID_ADDR + i, i);
		printf("%04X %c\n", pic14n_conf.userid[i], PIC_CHAR(0xFF & pic14n_conf.userid[i]));
	}
	printf("[%04X] [RESERVED] %04X\n", PIC14N_RESRV_ADDR, pic14n_conf.reserved);
	printf("[%04X] [REVISION] %04X MAJ:%04X MIN:%04X\n",
		PIC14N_REVID_ADDR, pic14n_conf.revisionid,
		(pic14n_conf.revisionid >> PIC14N_MAJOR_SHIFT) & PIC14N_REV_MASK,
		pic14n_conf.revisionid & PIC14N_REV_MASK);
	printf("[%04X] [DEVICEID] %04X DEV:%04X %s\n", PIC14N_DEVID_ADDR, pic14n_conf.deviceid,
		pic14n_conf.deviceid & PIC14N_DEVICEID_MASK, pic14n_map[pic14n_index].devicename);

	pic14n_dumpconfig(PIC_BRIEF);

	/* Device information area */
	if (pic14n_map[pic14n_index].devinfo) {
		for (uint32_t i = 0; i < pic14n_map[pic14n_index].devinfo; i += 8) {
			printf("[%04X] [DEVINF%02X] ", PIC14N_DEVINFO_ADDR + i, i);
			uint32_t j;
			for (j = 0; j < 7; ++j)
				printf("%04X ", pic14n_conf.devinfo[i + j]);
			printf("%04X\n", pic14n_conf.devinfo[i + j]);
		}
	}

	/* Device configuration information */
	if (pic14n_map[pic14n_index].devconf) {
		printf("[%04X] [ERASE]    %04X\n", PIC14N_ERASE, pic14n_conf.devconf[0]);
		printf("[%04X] [LATCHES]  %04X\n", PIC14N_LATCHES, pic14n_conf.devconf[1]);
		printf("[%04X] [ROWS]     %04X\n", PIC14N_ROWS, pic14n_conf.devconf[2]);
		printf("[%04X] [EEPROM]   %04X\n", PIC14N_EEPROM, pic14n_conf.devconf[3]);
		printf("[%04X] [PINS]     %04X\n", PIC14N_PINS, pic14n_conf.devconf[4]);
	}

	/* Device calibration */
	if (pic14n_map[pic14n_index].calib) {
		for (uint32_t i = 0; i < pic14n_map[pic14n_index].calib; ++i) {
			printf("[%04X] [CALIB%d]   %04X\n", PIC14N_CALIB_ADDR + i, 1 + i,
				pic14n_conf.calib[i]);
		}
	}

	if (pic14n_map[pic14n_index].eeprom)
		printf("[%04X] [DATA]     %04X BYTES\n", PIC14N_EEPROM_ADDR, pic14n_map[pic14n_index].eeprom);
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic14n_dumpconfig(int mode)
{
	for (uint32_t i = 0; i < pic14n_map[pic14n_index].config; ++i) {
		printf("[%04X] [CONFIG%d]  %04X\n",
			PIC14N_CONFIG_ADDR + i, 1 + i, pic14n_conf.config[i]);
	}
#ifdef VERBOSE
	if (mode == PIC_VERBOSE) switch (pic14n_map[pic14n_index].datasheet) {
	default:break;
	}
#endif
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic14n_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtcode(PIC14_MASK, 8, &data[i]))
			continue;
		printf("[%04X] ", address);
		for (j = 0; j < 8; ++j)
			printf("%04X ", data[i + j] & PIC14_MASK);
		for (j = 0; j < 8; ++j)
			putchar(PIC_CHAR(0xFF & data[i + j]));
		putchar('\n');
		nlines++;
	}
	if (!nlines)
		printf("%s: information: program flash empty\n", __func__);
}

/*
 * DUMP INHX32 FLASH WORDS
 */
void
pic14n_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* 14-bit: Extended address = 0x0000 */
	pic_dumpaddr(0, 1);

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtcode(PIC14_MASK, 8, &data[i]))
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
 * DUMP HEX DATA EEPROM BYTES
 */
void
pic14n_dumphexdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 16) {
		if (pic_mtdata(0xFF, 16, &data[i]))
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
		printf("%s: information: data eeprom empty\n", __func__);
}

/*
 * DUMP INHX32 DATA EEPROM BYTES
 */
void
pic14n_dumpinhxdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j;

	/* Enhanced 14-bit: Extended address = 0x0001 (EEPROM: 0xF000) */
	pic_dumpaddr(address << 1, 1);

	for (i = 0; i < size; address += 8, i += 8) {
		if (pic_mtdata(0xFF, 8, &data[i]))
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
 * DUMP INHX32 DEVICE CONFIGURATION
 */
void
pic14n_dumpdevice(void)
{
	/* Enhanced 14-bit: Extended address = 0x0001 (USERID: 0x8000) */
	pic_dumpaddr(PIC14N_USERID_ADDR << 1, 1);

	/* USERID */
	for (uint32_t i = 0; i < PIC14N_USERID_MAX; ++i)
		pic_dumpword16(PIC14N_USERID_ADDR + i, pic14n_conf.userid[i]);

	/* Enhanced 14-bit: Extended address = 0x0001 (CONFIG: 0x8007) */
	pic_dumpaddr(PIC14N_CONFIG_ADDR << 1, 1);

	/* CONFIG */
	for (uint32_t i = 0; i < pic14n_map[pic14n_index].config; ++i)
		pic_dumpword16(PIC14N_CONFIG_ADDR + i, pic14n_conf.config[i]);
}
