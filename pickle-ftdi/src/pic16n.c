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

struct pic_ops pic16n_ops = {
	.arch				= ARCH16BIT,
	.align				= sizeof(uint8_t),
	.selector			= pic16n_selector,
	.program_begin			= pic16n_program_begin,
	.program_data			= pic16n_program_data,
	.program_end			= pic16n_program_end,
	.verify_begin			= pic16n_program_verify,
	.verify_data			= pic16n_verify_data,
	.verify_end			= pic16n_standby,
	.view_data			= pic16n_view_data,
	.read_config_memory		= pic16n_read_config_memory,
	.get_program_size		= pic16n_get_program_size,
	.get_data_size			= pic16n_get_data_size,
	.get_executive_size		= NULL,
	.get_boot_size			= NULL,
	.read_program_memory_block	= pic16n_read_program_memory_block,
	.read_data_memory_block		= pic16n_read_data_memory_block,
	.write_panel			= pic16n_write_panel,
	.bulk_erase			= pic16n_bulk_erase,
	.write_osccal			= NULL,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= NULL,
	.dumpdeviceid			= pic16n_dumpdeviceid,
	.dumpconfig			= pic16n_dumpconfig,
	.dumposccal			= NULL,
	.dumpdevice			= pic16n_dumpdevice,
	.dumpadj			= 2,
	.dumphexcode			= pic16n_dumphexcode,
	.dumpinhxcode			= pic16n_dumpinhxcode,
	.dumphexdata			= pic16n_dumphexdata,
	.dumpinhxdata			= pic16n_dumpinhxdata,
};

uint32_t
pic16n_arch(void)
{
	p.pic = &pic16n_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic16n_config pic16n_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic16n_dsmap pic16n_map[] =
{
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches*/
{"PIC18F24K40", PIC18F24K40,    DS40001772B,	PIC16N_WORD(16), 12,      	256,	64},
{"PIC18F25K40", PIC18F25K40,    DS40001772B,	PIC16N_WORD(32), 12,      	256,	64},
{"PIC18F45K40", PIC18F45K40,    DS40001772B,	PIC16N_WORD(32), 12,      	256,	64},
{"PIC18F26K40", PIC18F26K40,    DS40001772B,	PIC16N_WORD(64), 12,      	1024,	64},
{"PIC18F46K40", PIC18F46K40,    DS40001772B,	PIC16N_WORD(64), 12,      	1024,	64},
{"PIC18F27K40", PIC18F27K40,    DS40001772B,	PIC16N_WORD(128),12,      	1024,	128},
{"PIC18F47K40", PIC18F47K40,    DS40001772B,	PIC16N_WORD(128),12,      	1024,	128},
{"PIC18LF24K40",PIC18LF24K40,   DS40001772B,	PIC16N_WORD(16), 12,      	256,	64},
{"PIC18LF25K40",PIC18LF25K40,   DS40001772B,	PIC16N_WORD(32), 12,      	256,	64},
{"PIC18LF45K40",PIC18LF45K40,   DS40001772B,	PIC16N_WORD(32), 12,      	256,	64},
{"PIC18LF26K40",PIC18LF26K40,   DS40001772B,	PIC16N_WORD(64), 12,      	1024,	64},
{"PIC18LF46K40",PIC18LF46K40,   DS40001772B,	PIC16N_WORD(64), 12,      	1024,	64},
{"PIC18LF27K40",PIC18LF27K40,   DS40001772B,	PIC16N_WORD(128),12,      	1024,	128},
{"PIC18LF47K40",PIC18LF47K40,   DS40001772B,	PIC16N_WORD(128),12,      	1024,	128},

{"(null)",      0,              0,              0,               0,             0,      0},
/*Device name	Device id	Data-sheet	Flash		 Config		EEProm	Latches*/
};
#define PIC16N_SIZE ((sizeof(pic16n_map) / sizeof(struct pic16n_dsmap)) - 1)

/* Default device (null) */
uint32_t pic16n_index = PIC16N_SIZE;

void
pic16n_selector(void)
{
	uint32_t i;
	char *dnames[PIC16N_SIZE];

	for (i = 0; i < PIC16N_SIZE; ++i) {
		dnames[i] = pic16n_map[i].devicename;
	}
	qsort(dnames, PIC16N_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC16N_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %u\n", (uint32_t)PIC16N_SIZE);
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
pic16n_program_verify(void)
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
pic16n_standby(void)
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
 *  0x02 PC = PC + 2 CODE
 *  0x02 PC = PC + 1 DATA
 *
 * DS40001772B-page 12
 */
static inline void
pic16n_load_data_for_nvm(uint32_t word, uint8_t j /* 0 || 1 */)
{
	io_program_out(j << 1, 8);
	/* TDLY 1us */
	io_program_out(word << 1, 24);
	/* TDLY 1us */
}

/*
 * READ DATA FROM NVM
 *  0xFC PC = PC
 *  0xFE PC = PC + 2 CODE
 *  0xFE PC = PC + 1 DATA
 *
 * DS40001772B-page 10
 */
static inline uint32_t
pic16n_read_data_from_nvm(uint8_t j /* 0 || 1 */)
{
	uint32_t word;

	io_program_out(0xFC | (j << 1), 8);
	/* TDLY 1us */
	word = io_program_in(24);
	/* TDLY 1us */

	word = (word >> 1) & PIC16N_WORD_MASK;

	return word;
}

/*
 * INCREMENT ADDRESS
 *  0xF8 PC = PC + 2 CODE
 *  0xF8 PC = PC + 1 DATA
 *
 * DS40001772B-page 10
 */
static inline void
pic16n_increment_address(void)
{
	io_program_out(0xF8, 8);
	/* TDLY 1us */
}

/*
 * LOAD PC ADDRESS
 *  0x80 PC = address
 *
 * DS40001772B-page 10
 */
static inline void
pic16n_load_pc_address(uint32_t address)
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
 * DS40001772B-page 12
 *
 * DS40001772B TPINT(2.8ms PROGRAM) TPINT(5.6ms CONFIG/EEPROM)
 */
static inline void
pic16n_begin_internally_timed_programming(uint32_t t)
{
	io_program_out(0xE0, 8);
	/* TPINT */
	io_usleep(t);
}

/*
 * BEGIN EXTERNALLY TIMED PROGRAMMING
 *  0xC0
 *
 * DS40001772B-page 12
 *
 * DS40001772B TPEXT(2.1ms PROGRAM) (NOT CONFIG)
 */
static inline void
pic16n_begin_externally_timed_programming(uint32_t t)
{
	io_program_out(0xC0, 8);
	/* TPEXT */
	io_usleep(t);
}

/*
 * END EXTERNALLY TIMED PROGRAMMING
 *  0x82
 *
 * DS40001772B-page 12
 *
 * DS40001772B TDIS(300us PROGRAM)
 */
static inline void
pic16n_end_externally_timed_programming(uint32_t t)
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
 * PC = 0x000000 ERASE PROGRAM AND CONFIG
 * PC = 0x300000 ERASE PROGRAM AND CONFIG AND USERID
 * PC = 0x310000 ERASE EEPROM
 *
 * DS40001772B-page 12
 *
 * DS40001772B TERAB(25.2ms PIC18F26K40)
 */
static inline void
pic16n_bulk_erase_memory(uint32_t t)
{
	io_program_out(0x18, 8);
	io_usleep(t);
}

/*
 * ROW ERASE MEMORY (PRELOAD PC)
 *  0xF0
 *
 * DS40001772B-page 12
 *
 * DS40001772B TERAR(2.8ms)
 */
static inline void
pic16n_row_erase_memory(uint32_t t)
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
pic16n_bulk_erase(void)
{
	pic16n_program_verify();

	pic16n_load_pc_address(PIC16N_USERID_ADDR);
	pic16n_bulk_erase_memory(PIC16N_TERAB);

	if (pic16n_map[pic16n_index].eeprom) {
		pic16n_load_pc_address(PIC16N_EEPROM_ADDR);
		pic16n_bulk_erase_memory(PIC16N_TERAB);
	}

	pic16n_standby();
}

#if 0
void
pic16n_row_erase(uint32_t row, uint32_t nrows)
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
 * READ CONFIGURATION MEMORY AND STORE DEVICE MAP INDEX
 */
int
pic16n_read_config_memory(void)
{
	uint32_t dev;
	uint16_t word;

	/* NULL device */
	pic16n_index = PIC16N_SIZE;

	/* Reset configuraton */
	memset(&pic16n_conf, 0, sizeof(pic16n_conf));

	pic16n_program_verify();
	pic16n_load_pc_address(PIC16N_REVID_ADDR);

	pic16n_conf.revisionid = pic16n_read_data_from_nvm(1);	/* Revisionid */
	pic16n_conf.deviceid = pic16n_read_data_from_nvm(1);	/* Deviceid   */

	/* Device detect */
	for (dev = 0; pic16n_map[dev].deviceid; ++dev) {
		if (pic16n_map[dev].deviceid == pic16n_conf.deviceid) {
			break;
		}
	}
	if (pic16n_map[dev].deviceid == 0) {
		/*
		 * VELLEMAN K8048 SWITCH IN STANDBY [0000]
		 * VELLEMAN K8048 NO POWER          [3FFF]
		 * VELLEMAN K0848 SWITCH IN RUN     [3FFF]
		 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
		 */
		if (pic16n_conf.deviceid == 0x0000 || pic16n_conf.deviceid == 0x3FFF) {
			printf("%s: information: %s.\n",
				__func__, io_fault(pic16n_conf.deviceid));
		} else {
			printf("%s: information: device unknown: [%04X]\n",
				__func__, pic16n_conf.deviceid);
		}
		pic16n_standby();
		return -1;
	}

	/* Device recognised */
	pic16n_index = dev;

	/* User ID */
	pic16n_load_pc_address(PIC16N_USERID_ADDR);
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; i += 2) {
		word = pic16n_read_data_from_nvm(1);
		pic16n_conf.userid[i] = word;
		pic16n_conf.userid[i + 1] = word >> 8;
	}

	/* Config word(s) */
	pic16n_load_pc_address(PIC16N_CONFIG_ADDR);
	for (uint32_t i = 0; i < pic16n_map[pic16n_index].config; i += 2) {
		word = pic16n_read_data_from_nvm(1);
		pic16n_conf.config[i] = word;
		pic16n_conf.config[i + 1] = word >> 8;
	}

	pic16n_standby();

	return 0;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic16n_get_program_size(uint32_t *addr)
{
	*addr = 0;

	return pic16n_map[pic16n_index].flash;
}

/*
 * GET DATA EEPROM/FLASH SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic16n_get_data_size(uint32_t *addr)
{
	*addr = PIC16N_EEPROM_ADDR;

	return pic16n_map[pic16n_index].eeprom;
}

/*
 * REWRITE EEPROM/FLASH ADDRESS
 */
#if 0
static inline uint32_t
pic16n_get_data_addr(uint32_t addr)
{
	if (addr >= PIC16N_EEFAKE_ADDR)
        	addr = addr - PIC16N_EEFAKE_ADDR + PIC16N_EEPROM_ADDR;

	return addr;
}
#endif

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16n_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	uint32_t i;

	pic16n_program_verify();
	pic16n_load_pc_address(addr);

	for (i = 0; i < size; ++i)
		data[i] = (uint32_t)pic16n_read_data_from_nvm(1);

	pic16n_standby();

	return addr;
}

/*
 * READ DATA EEPROM BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16n_read_data_memory_block(uint16_t *data, uint32_t addr, uint16_t size)
{
	uint32_t i;

	pic16n_program_verify();
	pic16n_load_pc_address(addr);

	for (i = 0; i < size; i++)
		data[i] = (uint16_t)pic16n_read_data_from_nvm(1);

	pic16n_standby();

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
pic16n_write_panel(uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	uint16_t word;

	for (uint32_t i = 0; i < panel_size; i += 2) {
		word = panel[i] | (panel[i + 1] << 8);
		pic16n_load_data_for_nvm(word, 1);
	}
	pic16n_load_pc_address(address);
	pic16n_begin_internally_timed_programming(PIC16N_TPINT_CODE);
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
pic16n_write_data_memory(uint32_t address, uint8_t data)
{
	pic16n_load_pc_address(address);
	pic16n_load_data_for_nvm(data, 0);
	pic16n_begin_internally_timed_programming(PIC16N_TPINT_CONFIG);
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
pic16n_write_config_word(uint16_t word)
{
	pic16n_load_data_for_nvm(word, 0);
	pic16n_begin_internally_timed_programming(PIC16N_TPINT_CONFIG);

	return pic16n_read_data_from_nvm(1);
}

/*
 * WRITE CONFIG
 *
 *  RETURN 0 ON ERROR ELSE PROGRAM COUNT
 */
uint32_t
pic16n_write_config(void)
{
	uint16_t wdata, vdata;

	pic16n_program_verify();

	pic16n_load_pc_address(PIC16N_USERID_ADDR);
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; i += 2) {
		wdata = pic16n_conf.userid[i] | pic16n_conf.userid[i + 1] << 8;
		vdata = pic16n_write_config_word(wdata);
		if (vdata != wdata) {
			printf("%s: error: USERID%d write failed: read [%04X] expected [%04X]\n",
				__func__, i, vdata, wdata);
			pic16n_standby();
			return 0;
		}
	}

	pic16n_load_pc_address(PIC16N_CONFIG_ADDR);
	for (uint32_t i = 0; i < PIC16N_CONFIG_MAX; i += 2) {
		wdata = pic16n_conf.config[i] | pic16n_conf.config[i + 1] << 8;
		vdata = pic16n_write_config_word(wdata);
		if (vdata != wdata) {
			printf("%s: error: CONFIG%d write failed: read [%04X] expected [%04X]\n",
				__func__, i + 1, vdata, wdata);
			pic16n_standby();
			return 0;
		}
	}

	pic16n_standby();

	return PIC16N_USERID_MAX + PIC16N_CONFIG_MAX;
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
 *      0x200000 .. 0x200007
 *
 *  RETURN PIC_REGIONCONFIG:
 *      0x300000 .. 0x30000B
 *
 *  RETURN PIC_REGIONDATA:
 *      0x310000 .. 0x31XXXX
 */
uint16_t
pic16n_getregion(uint32_t address)
{
	/* CODE */
	if (address < pic16n_map[pic16n_index].flash) {
		return PIC_REGIONCODE;
	}
	/* ID */
	if (address >= PIC16N_USERID_ADDR && address < (PIC16N_USERID_ADDR +
		PIC16N_USERID_MAX)) {
		return PIC_REGIONID;
	}
	/* CONFIG */
	if (address >= PIC16N_CONFIG_ADDR && address < (PIC16N_CONFIG_ADDR +
		pic16n_map[pic16n_index].config)) {
		return PIC_REGIONCONFIG;
	}
	/* DATA EEPROM */
	if (pic16n_map[pic16n_index].eeprom) {
		if (address >= PIC16N_EEPROM_ADDR && address < (PIC16N_EEPROM_ADDR +
			pic16n_map[pic16n_index].eeprom))
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
pic16n_init_writeregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic16n_load_pc_address(0); /* Reset latch address */
		pic_write_panel(PIC_PANEL_BEGIN, region, pic16n_map[pic16n_index].latches);
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
pic16n_writeregion(uint32_t address, uint32_t region, uint16_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic_write_panel(PIC_PANEL_UPDATE, address, data);
		return;
	case PIC_REGIONID:
		pic16n_conf.userid[address - PIC16N_USERID_ADDR] = data;
		return;
	case PIC_REGIONCONFIG:
		pic16n_conf.config[address - PIC16N_CONFIG_ADDR] = data;
		return;
	case PIC_REGIONDATA:
		pic16n_write_data_memory(address, data);
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
pic16n_init_verifyregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
	case PIC_REGIONCONFIG:
	case PIC_REGIONDATA:
		/* Nothing to do on PIC16 new */
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
static uint32_t
pic16n_verifyregion(uint32_t address, uint32_t region, uint16_t index, uint16_t wdata, uint8_t size)
{
	uint16_t vdata = 0;

	if (region == PIC_REGIONNOTSUP) {
		if (p.f)
			fprintf(p.f, "%s: warning: region unsupported [%d]\n",
				__func__, region);
		return wdata;
	}
	if (index == 0)
		pic16n_load_pc_address(address);
	switch (size) {
	case 0:	vdata = pic16n_read_data_from_nvm(1) & 0xFF;		/* EEPROM BYTE */
		break;
	case 1: if (address & 1)					/* CODE/USERID/CONFIG BYTE */
			vdata = pic16n_read_data_from_nvm(1) >> 8;	/* HIGH BYTE */
		else
			vdata = pic16n_read_data_from_nvm(1) & 0xFF;	/* LOW BYTE */
		break;
	default:
	case 2: vdata = pic16n_read_data_from_nvm(1);			/* CODE/USERID/CONFIG WORD */
		break;
	}
	if (vdata != wdata && p.f) {
		if (size == 2)
			printf("%s: error: read [%04X] expected [%04X] at [%06X]\n",
				__func__, vdata, wdata, address);
		else
			printf("%s: error: read [%02X] expected [%02X] at [%06X]\n",
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
pic16n_program_begin(void)
{
	pic16n_program_verify();

	memset(pic16n_conf.userid, -1, sizeof(uint8_t) * PIC16N_USERID_MAX);
	memset(pic16n_conf.config, -1, sizeof(uint8_t) * PIC16N_CONFIG_MAX);
}

/*
 * PROGRAM DATA
 */
uint32_t
pic16n_program_data(uint32_t current_region, pic_data *pdata)
{
	uint32_t address, new_region;

	for (uint32_t i = 0; i < pdata->nbytes; ++i) {
		address = pdata->address + i;
		new_region = pic16n_getregion(address);
		if (new_region != current_region) {
			pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic16n_init_writeregion(new_region);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		pic16n_writeregion(address, current_region, pdata->bytes[i]);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic16n_program_end(int config)
{
	pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
	pic16n_standby();
	if (config)
		pic16n_write_config();
}

/*
 * VERIFY DATA
 */
uint32_t
pic16n_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	uint32_t address, new_region;
	uint16_t wdata, vdata;
	uint8_t size;

	for (uint32_t i = 0; i < pdata->nbytes;) {
		address = pdata->address + i;
		new_region = pic16n_getregion(address);
		if (new_region != current_region)
			current_region = pic16n_init_verifyregion(new_region);
		if (current_region == PIC_REGIONNOTSUP) {
			i += 1;
		} else if (current_region == PIC_REGIONDATA) {
			wdata = pdata->bytes[i];
			vdata = pic16n_verifyregion(address, current_region, i, wdata, 0);
			if (vdata != wdata) {
				pdata->bytes[i] = vdata;
				(*fail) += 1;
			}
			i += 1;
		} else {
			wdata = pdata->bytes[i];
			size = 1; /* BYTE */
			if ((i + 1) < pdata->nbytes) {
				wdata |= pdata->bytes[i + 1] << 8;
				size = 2; /* WORD */
			}
			vdata = pic16n_verifyregion(address, current_region, i, wdata, size);
			if (vdata != wdata) {
				pdata->bytes[i] = vdata;
				if (size == 2)
					pdata->bytes[i + 1] = vdata >> 8;
				(*fail) += size;
			}
			i += size;
		}
	}
	return current_region;
}

/*
 * VIEW DATA
 */
void
pic16n_view_data(pic_data *pdata)
{
	printf("[%06X] ", pdata->address);
	for (uint32_t i = 0; i < pdata->nbytes; ++i)
		printf("%02X ", pdata->bytes[i]);
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
pic16n_dumpdeviceid(void)
{
	printf("[000000] [PROGRAM]     %04X WORDS (%04X ROWS OF %04X WORDS)\n",
		pic16n_map[pic16n_index].flash,
		2 * pic16n_map[pic16n_index].flash / pic16n_map[pic16n_index].latches,
		pic16n_map[pic16n_index].latches / 2);
	for (uint32_t i = 0; i < PIC16N_USERID_MAX; ++i) {
		printf("[%06X] [IDLOCATION%d] %02X %c\n",
			PIC16N_USERID_ADDR + i,
			i + 1,
			pic16n_conf.userid[i], PIC_CHAR(0xFF & pic16n_conf.userid[i]));
	}

	pic16n_dumpconfig(PIC_BRIEF);

	if (pic16n_map[pic16n_index].eeprom) {
		printf("[%06X] [DATA]        %04X BYTES\n",
			PIC16N_EEPROM_ADDR, pic16n_map[pic16n_index].eeprom);
        }
	printf("[3FFFFC] [REVISION]    %04X MAJ:%02X MIN:%02X\n",
		pic16n_conf.revisionid,
		(pic16n_conf.revisionid >> PIC16N_MAJOR_SHIFT) & PIC16N_REV_MASK,
		pic16n_conf.revisionid & PIC16N_REV_MASK);
	printf("[3FFFFE] [DEVICEID]    %04X %s\n", pic16n_conf.deviceid,
		pic16n_map[pic16n_index].devicename);
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic16n_dumpconfig(int mode)
{
	for (uint32_t i = 0; i < PIC16N_CONFIG_MAX; i += 2) {
		printf("[%06X] [CONFIG%d]     %04X\n",
			PIC16N_CONFIG_ADDR + i,
			i / 2 + 1,
			(pic16n_conf.config[i + 1] << 8) | pic16n_conf.config[i]);
	}
#ifdef VERBOSE
	if (mode == PIC_VERBOSE) switch (pic16n_map[pic16n_index].datasheet) {
	default:break;
	}
#endif
}

/*
 * DUMP HEX FLASH WORDS
 */
void
pic16n_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 8) {
		if (pic_mtcode(PIC16N_WORD_MASK, 8, &data[i]))
			continue;
		printf("[%06X] ", address);
		for (j = 0; j < 8; ++j)
			printf("%04X ", data[i + j] & PIC16N_WORD_MASK);
		for (j = 0; j < 8; ++j) {
			putchar(PIC_CHAR(0xFF & data[i + j]));
			putchar(PIC_CHAR(0xFF & (data[i + j] >> 8)));
		}
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
pic16n_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* 16-bit: Extended address */
	pic_dumpaddr(address, 1);

	for (i = 0; i < size; address += 16, i += 8) {
		if (pic_mtcode(PIC16N_WORD_MASK, 8, &data[i]))
			continue;
		/* 16-bit: Extended address */
		pic_dumpaddr(address, 0);

		uint8_t cc, hb, lb;
		hb = address >> 8;
		lb = address;
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
pic16n_dumphexdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 16) {
		if (pic_mtdata(0xFF, 16, &data[i]))
			continue;
		printf("[%06X] ", address);
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
 * DUMP INHX32 DATA EEPROM BYTES
 */
void
pic16n_dumpinhxdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j;

	/* PIC18: Extended address = 0x0031 (EEPROM: 0x310000) */
	pic_dumpaddr(PIC16N_EEPROM_ADDR, 1);

	for (i = 0; i < size; address += 16, i += 16) {
		if (pic_mtdata(0xFF, 16, &data[i]))
			continue;

		uint8_t cc, hb, lb;
		hb = address >> 8;
		lb = address;
		printf(":%02X%02X%02X00", 16, hb, lb);
		cc = 16 + hb + lb + 0x00;
		for (j = 0; j < 16; ++j) {
			lb = data[i + j];
			printf("%02X", lb);
			cc = cc + lb;
		}
		printf("%02X\n", (0x0100 - cc) & 0xFF);
	}
}

/*
 * DUMP INHX32 DEVICE CONFIGURATION
 */
void
pic16n_dumpdevice(void)
{
	uint32_t i;

	/* PIC18: Extended address = 0x0020 (USERID: 0x200000) */
	pic_dumpaddr(PIC16N_USERID_ADDR, 1);

	for (i = 0; i < PIC16N_USERID_MAX; ++i)
		pic_dumpbyte(i, pic16n_conf.userid[i]);

	/* PIC18: Extended address = 0x0030 (CONFIG: 0x300000) */
	pic_dumpaddr(PIC16N_CONFIG_ADDR, 1);

	for (i = 0; i < pic16n_map[pic16n_index].config; ++i)
		pic_dumpbyte(i, pic16n_conf.config[i]);
}
