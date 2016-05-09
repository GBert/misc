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

struct pic_ops pic16_ops = {
	.arch				= ARCH16BIT,
	.align				= sizeof(uint8_t),
	.selector			= pic16_selector,
	.program_begin			= pic16_program_begin,
	.program_data			= pic16_program_data,
	.program_end			= pic16_program_end,
	.verify_begin			= pic16_program_verify,
	.verify_data			= pic16_verify_data,
	.verify_end			= pic16_standby,
	.view_data			= pic16_view_data,
	.read_config_memory		= pic16_read_config_memory,
	.get_program_size		= pic16_get_program_size,
	.get_data_size			= pic16_get_data_size,
	.get_executive_size		= NULL,
	.get_boot_size			= NULL,
	.read_program_memory_block	= pic16_read_program_memory_block,
	.read_data_memory_block		= pic16_read_data_memory_block,
	.write_panel			= pic16_write_panel,
	.bulk_erase			= pic16_bulk_erase,
	.write_osccal			= NULL,
	.write_bandgap			= NULL,
	.write_calib			= NULL,
	.row_erase			= pic16_row_erase,
	.dumpdeviceid			= pic16_dumpdeviceid,
	.dumpconfig			= pic16_dumpconfig,
	.dumposccal			= NULL,
	.dumpdevice			= pic16_dumpdevice,
	.dumpadj			= 2,
	.dumphexcode			= pic16_dumphexcode,
	.dumpinhxcode			= pic16_dumpinhxcode,
	.dumphexdata			= pic16_dumphexdata,
	.dumpinhxdata			= pic16_dumpinhxdata,
};

uint32_t
pic16_arch(void)
{
	p.pic = &pic16_ops;

	return p.pic->arch;
}

/*****************************************************************************
 *
 * Hardware configuration
 *
 *****************************************************************************/

struct pic16_config pic16_conf;

/*****************************************************************************
 *
 * Hardware algorithm map
 *
 *****************************************************************************/

struct pic16_dsmap pic16_map[] =
{
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panelsize  P9 P9A P10 P11  Configuration #Config EraseSize*/
{"PIC18F2331",	PIC18F2331,	4096,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  0x300000,     14,     64},
{"PIC18F4331",	PIC18F4331,	4096,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  0x300000,     14,     64},
{"PIC18F2431",	PIC18F2431,	8192,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  0x300000,     14,     64},
{"PIC18F4431",	PIC18F4431,	8192,		256,	DS30500A,  0,		8,	   1000,0,5,5000,  0x300000,     14,     64},

{"PIC18F242",	PIC18F242,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F248",	PIC18F248,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F252",	PIC18F252,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F258",	PIC18F258,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F442",	PIC18F442,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F448",	PIC18F448,	8192,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F452",	PIC18F452,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F458",	PIC18F458,	16384,		256,	DS39576B,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},

{"PIC18F2439",	PIC18F2439,	6144,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F2539",	PIC18F2539,	12288,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F4439",	PIC18F4439,	6144,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F4539",	PIC18F4539,	12288,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18LF2439",	PIC18F2439,	6144,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18LF2539",	PIC18F2539,	12288,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18LF4439",	PIC18F4439,	6144,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18LF4539",	PIC18F4539,	12288,		256,	DS30480C,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},

{"PIC18F1220",	PIC18F1220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F1320",	PIC18F1320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F2220",	PIC18F2220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F2320",	PIC18F2320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F4220",	PIC18F4220,	2048,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},
{"PIC18F4320",	PIC18F4320,	4096,		256,	DS39592E,  0,		8,	   1000,0,5,10000, 0x300000,     14,     64},

{"PIC18F2221",	PIC18F2221,	2048,		256,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2321",	PIC18F2321,	4096,		256,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2410",	PIC18F2410,	8192,		0,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2420",	PIC18F2420,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2423",	PIC18F2423,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2450",	PIC18F2450,	8192,		0,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2455",	PIC18F2455,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2458",	PIC18F2458,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2480",	PIC18F2480,	8192,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2510",	PIC18F2510,	16384,		0,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2515",	PIC18F2515,	24576,		0,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2520",	PIC18F2520,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2523",	PIC18F2523,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2525",	PIC18F2525,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2550",	PIC18F2550,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2553",	PIC18F2553,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2580",	PIC18F2580,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2585",	PIC18F2585,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2610",	PIC18F2610,	32768,		0,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2620",	PIC18F2620,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2680",	PIC18F2680,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2682",	PIC18F2682,	40960,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F2685",	PIC18F2685,	49152,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4221",	PIC18F4221,	2048,		256,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4321",	PIC18F4321,	4096,		256,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4410",	PIC18F4410,	8192,		0,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4420",	PIC18F4420,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4423",	PIC18F4423,	8192,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4450",	PIC18F4450,	8192,		0,	DS39622K,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4455",	PIC18F4455,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4458",	PIC18F4458,	12288,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4480",	PIC18F4480,	8192,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4510",	PIC18F4510,	16384,		0,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4515",	PIC18F4515,	24576,		0,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4520",	PIC18F4520,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4523",	PIC18F4523,	16384,		256,	DS39622K,  1,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4525",	PIC18F4525,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4550",	PIC18F4550,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4553",	PIC18F4553,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4580",	PIC18F4580,	16384,		256,	DS39622K,  0,		32,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4585",	PIC18F4585,	24576,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4610",	PIC18F4610,	32768,		0,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4620",	PIC18F4620,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4680",	PIC18F4680,	32768,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4682",	PIC18F4682,	40960,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F4685",	PIC18F4685,	49152,		1024,	DS39622K,  0,		64,	   1000,0,100,5000,0x300000,     14,     64},

{"PIC18F1230",	PIC18F1230,	2048,		128,	DS39752B,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F1330",	PIC18F1330,	4096,		128,	DS39752B,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},
{"PIC18F1330-ICD",
		PIC18F1330ICD,	4096,		128,	DS39752B,  0,		8,	   1000,0,100,5000,0x300000,     14,     64},

{"PIC18F45K22",	PIC18F45K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F25K22",	PIC18F25K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F23K22",	PIC18F23K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F24K22",	PIC18F24K22,	8192,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F26K22",	PIC18F26K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F43K22",	PIC18F43K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F46K22",	PIC18F46K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF45K22",PIC18LF45K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF25K22",PIC18LF25K22,	16384,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF23K22",PIC18LF23K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF24K22",PIC18LF24K22,	8192,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF26K22",PIC18LF26K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF43K22",PIC18LF43K22,	4096,		256,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF46K22",PIC18LF46K22,	32768,		1024,	DS41398B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},

{"PIC18F65K80",	PIC18F65K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F45K80",	PIC18F45K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F25K80",	PIC18F25K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F66K80",	PIC18F66K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F46K80",	PIC18F46K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F26K80",	PIC18F26K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF65K80",PIC18LF65K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF45K80",PIC18LF45K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF25K80",PIC18LF25K80,	16384,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF66K80",PIC18LF66K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF46K80",PIC18LF46K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF26K80",PIC18LF26K80,	32768,		1024,	DS39972B,  0,		64,	   1000,5000,100,5000,0x300000,  14,     64},

{"PIC18F24K50",	PIC18F24K50,	8192,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F25K50",	PIC18F25K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F45K50",	PIC18F45K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F26K50",	PIC18F26K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18F46K50",	PIC18F46K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF24K50",PIC18LF24K50,	8192,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF25K50",PIC18LF25K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF45K50",PIC18LF45K50,	16384,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF26K50",PIC18LF26K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},
{"PIC18LF46K50",PIC18LF46K50,	32768,		256,	DS41630B,  0,		64,	   1000,5000,200,15000,0x300000, 14,     64},

{"PIC18F13K50",	PIC18F13K50,	4096,		256,	DS41342E,  0,		8,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F14K50",	PIC18F14K50,	8192,		256,	DS41342E,  0,		16,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF13K50",PIC18LF13K50,	4096,		256,	DS41342E,  0,		8,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF14K50",PIC18LF14K50,	8192,		256,	DS41342E,  0,		16,	   1000,5000,100,5000,0x300000,  14,     64},

{"PIC18F24J10",	PIC18F24J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,   8,      1024},
{"PIC18F44J10",	PIC18F44J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,   8,      1024},
{"PIC18F25J10",	PIC18F25J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,   8,      1024},
{"PIC18F45J10",	PIC18F45J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,   8,      1024},
{"PIC18F24J11",	PIC18F24J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18F44J11",	PIC18F44J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18F25J11",	PIC18F25J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18F45J11",	PIC18F45J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18F26J11",	PIC18F26J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18F46J11",	PIC18F46J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18F26J13",	PIC18F26J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18F46J13",	PIC18F46J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18F27J13",	PIC18F27J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18F47J13",	PIC18F47J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18F24J50",	PIC18F24J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18F44J50",	PIC18F44J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18F25J50",	PIC18F25J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18F45J50",	PIC18F45J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18F26J50",	PIC18F26J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18F46J50",	PIC18F46J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18F26J53",	PIC18F26J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18F46J53",	PIC18F46J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18F27J53",	PIC18F27J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18F47J53",	PIC18F47J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18LF24J10",PIC18LF24J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,   8,      1024},
{"PIC18LF44J10",PIC18LF44J10,	8192,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x3FF8,   8,      1024},
{"PIC18LF25J10",PIC18LF25J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,   8,      1024},
{"PIC18LF45J10",PIC18LF45J10,	16384,		0,	DS39687E,  0,		64,	   3400,0,49000,475000,0x7FF8,   8,      1024},
{"PIC18LF24J11",PIC18LF24J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18LF44J11",PIC18LF44J11,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18LF25J11",PIC18LF25J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18LF45J11",PIC18LF45J11,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18LF26J11",PIC18LF26J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18LF46J11",PIC18LF46J11,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18LF26J13",PIC18LF26J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18LF46J13",PIC18LF46J13,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18LF27J13",PIC18LF27J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18LF47J13",PIC18LF47J13,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18LF24J50",PIC18LF24J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18LF44J50",PIC18LF44J50,	8192,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x3FF8,   8,      1024},
{"PIC18LF25J50",PIC18LF25J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18LF45J50",PIC18LF45J50,	16384,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0x7FF8,   8,      1024},
{"PIC18LF26J50",PIC18LF26J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18LF46J50",PIC18LF46J50,	32768,		0,	DS39687E,  0,		64,	   1200,0,54000,524000,0xFFF8,   8,      1024},
{"PIC18LF26J53",PIC18LF26J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18LF46J53",PIC18LF46J53,	32768,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0xFFF8,   8,      1024},
{"PIC18LF27J53",PIC18LF27J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},
{"PIC18LF47J53",PIC18LF47J53,	65536,		0,	DS39687E,  0,		64,	   1200,0,49000,475000,0x1FFF8,  8,      1024},

{"PIC18F13K22",	PIC18F13K22,	4096,		256,	DS41357B,  0,		8,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18F14K22",	PIC18F14K22,	8192,		256,	DS41357B,  0,		16,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF13K22",PIC18LF13K22,	4096,		256,	DS41357B,  0,		8,	   1000,5000,100,5000,0x300000,  14,     64},
{"PIC18LF14K22",PIC18LF14K22,	8192,		256,	DS41357B,  0,		16,	   1000,5000,100,5000,0x300000,  14,     64},

{"PIC18F23K20",	PIC18F23K20,	4096,		256,	DS41297F,  0,		16,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F24K20",	PIC18F24K20,	8192,		256,	DS41297F,  0,		32,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F25K20",	PIC18F25K20,	16384,		256,	DS41297F,  0,		32,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F26K20",	PIC18F26K20,	32768,		1024,	DS41297F,  0,		64,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F43K20",	PIC18F43K20,	4096,		256,	DS41297F,  0,		16,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F44K20",	PIC18F44K20,	8192,		256,	DS41297F,  0,		32,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F45K20",	PIC18F45K20,	16384,		256,	DS41297F,  0,		32,	   1000,5000,200,5000,0x300000,  14,     64},
{"PIC18F46K20",	PIC18F46K20,	32768,		1024,	DS41297F,  0,		64,	   1000,5000,200,5000,0x300000,  14,     64},

{"(null)",	0,		0,		0,	0,	   0,		0,	   0, 0,  0,  0,   0,            0,      0}
/*Device name	Device id	Flash		EEPROM	Data-sheet DEVID1 REV4	Panelsize  P9 P9A P10 P11  Configuration #Config EraseSize*/
};
#define PIC16_SIZE ((sizeof(pic16_map) / sizeof(struct pic16_dsmap)) - 1)

/* Default device (null) */
uint32_t pic16_index = PIC16_SIZE;

void
pic16_selector(void)
{
	int i;
	char *dnames[PIC16_SIZE];

	for (i = 0; i < PIC16_SIZE; ++i) {
		dnames[i] = pic16_map[i].devicename;
	}
	qsort(dnames, PIC16_SIZE, sizeof(char *), pic_cmp);
	for (i = 0; i < PIC16_SIZE; ++i) {
		if ((i % PIC_NCOLS) == (PIC_NCOLS - 1))
			printf("%s\n", dnames[i]);
		else
			printf("%s\t", dnames[i]);
	}
	if (i % PIC_NCOLS)
		printf("\n");
	printf("Total: %d\n", (int)PIC16_SIZE);
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
pic16_program_verify(void)
{
	/* RESET & ACQUIRE GPIO */
	io_set_vpp(LOW);
	/* UNKNOWN */
	io_usleep(1000);

	/* PGD + PGC + PGM LOW */
	io_set_pgd(LOW);
	io_set_pgc(LOW);
	io_set_pgm(LOW);
	io_usleep(1000);

	/* INPUT DATA ON CLOCK RISING EDGE */
	io_configure(FALSE);

	/* LVP(KEY) */
	if (p.key == LVPKEY) {
		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* DS39972B PIC18F26K80 P12(250us) */
		io_usleep(250);

		/* VPP LOW */
		io_set_vpp(LOW);
		/* DS39687E PIC18LF27J53 P19(4ms)   */
		/* DS39972B PIC18F26K80  P12(250us) */
		/* DS41398B PIC18F25K22  P18(1ms)   */
		io_usleep(4000); /* 4ms */

		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(PHCMKEY, 32);
		/* DS39687E PIC18LF27J53 P20(50ns)  */
		/* DS39972B PIC18F26K80  ?	  */
		/* DS41398B PIC18F25K22  P20(40ns)  */
		io_usleep(250);

		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* DS39687E PIC18LF27J53 P12(400us) */
		/* DS39972B PIC18F26K80  ?	  */
		/* DS41398B PIC18F25K22  P15(400us) */
		io_usleep(400);
	}
	/* HVP(KEY) */
	else if (p.key == HVPKEY) {
		/* VPP HIGH */
		io_set_vpp(HIGH);
		/* DS39972B PIC18F26K80 P12(250us) */
		io_usleep(250);

		/* PROGRAM/VERIFY ENTRY CODE */
		io_program_out(PHCMKEY, 32);
		/* UNSPECIFIED */
		io_usleep(250);
	}
	/* LVP(PGM) | HVP */
	else {
		/* PGM HIGH */
		io_set_pgm(HIGH);
		io_usleep(10);

		/* VPP HIGH */
		io_set_vpp(HIGH);
		io_usleep(250);
	}
}

/*
 * EXIT LVP/HVP PROGRAM/VERIFY MODE
 *
 * EXIT - VDD LAST
 */
void
pic16_standby(void)
{
	/* PGD + PGC + VPP + PGM LOW */
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

/*
 * 0000
 *
 * CORE INSTRUCTION (SHIFT IN 16 BIT INSTRUCTION)
 */
static inline void
pic16_core_instruction(uint16_t word)
{
	io_program_out(0x00, 4);
	io_program_out(word, 16);
}

/*
 * 0000
 *
 * CORE INSTRUCTION (PROGRAMMING NOP)
 *
 * DS39752B-page 32 PIC18F1330   HIGH=P9(1ms)		LOW=P10(100us)
 * DS39576B-page 34 PIC18F252    HIGH=P9(1ms)		LOW=P10(5us)
 * DS41398B-page 38 PIC18F25K22  HIGH=P9(1ms)/P9A(5ms)	LOW=P10(200us)
 * DS39972B-page 47 PIC18F26K80  HIGH=P9(1ms)/P9A(5ms)	LOW=P10(100us)
 * DS39687E-page 30 PIC18LF27J53 HIGH=P9(3.4ms)		LOW=NONE
 */
static inline void
pic16_core_instruction_nopp(uint32_t h, uint32_t l)
{
	io_program_out(0x00, 3);
	io_set_pgc(HIGH);		/* Clock high         */
	io_usleep(h);		/* Delay high P9      */
	io_set_pgc(LOW);		/* Clock low          */
	io_usleep(l);		/* Delay low P10/NONE */ 
	io_program_out(0, 16);
}

/*
 * 0000
 *
 * CORE INSTRUCTION (ERASE NOP)
 *
 * DS39752B-page 32 PIC18F1330   P11(5ms)   + P10(100us)
 * DS39576C-page 34 PIC18F252    P11(10ms)  + P10(5us)
 * DS41398B-page 38 PIC18F25K22  P11(15ms)  + P10(200us)
 * DS39972B-page 48 PIC18F26K80  P11(5ms)   + P10(100us)
 * DS39687E-page 30 PIC18LF27J53 P11(524ms) + P10(54ms)
 */
static inline void
pic16_core_instruction_nope(void)
{
	uint32_t l = pic16_map[pic16_index].p11 + pic16_map[pic16_index].p10;

	io_program_out(0x00, 4);
	io_usleep(l);		/* Delay low P11 + P10 */
	io_program_out(0, 16);
}

/*
 * 0010 = 0x02
 *
 * SHIFT OUT TABLAT REGISTER
 */
static inline uint8_t
pic16_shift_out_tablat_register(void)
{
	io_program_out(0x02, 4);
	return io_program_in(16) >> 8;
}

#if 0
/*
 * 1000 = 0x08
 *
 * TABLE READ
 */
static inline uint8_t
pic16_table_read(void)
{
	io_program_out(0x08, 4);
	return io_program_in(16) >> 8;
}
#endif

/*
 * 1001 = 0x09
 *
 * TABLE READ POST INCREMENT
 */
static inline uint8_t
pic16_table_read_post_increment(void)
{
	io_program_out(0x09, 4);
	return io_program_in(16) >> 8;
}

#if 0
/*
 * 1010 = 0x0A
 *
 * TABLE READ POST DECREMENT
 */
static inline uint8_t
pic16_table_read_post_decrement(void)
{
	io_program_out(0x0A, 4);
	return io_program_in(16) >> 8;
}

/*
 * 1011 = 0x0B
 *
 * TABLE READ PRE INCREMENT
 */
static inline uint8_t
pic16_table_read_pre_increment(void)
{
	io_program_out(0x0B, 4);
	return io_program_in(16) >> 8;
}
#endif

/*
 * 1100 = 0x0C
 *
 * TABLE WRITE
 */
static inline void
pic16_table_write(uint16_t word)
{
	io_program_out(0x0C, 4);
	io_program_out(word, 16);
}

/*
 * 1101 = 0x0D
 *
 * TABLE WRITE POST INCREMENT BY 2
 */
static inline void
pic16_table_write_post_increment_2(uint16_t word)
{
	io_program_out(0x0D, 4);
	io_program_out(word, 16);
}

#if 0
/*
 * 1110 = 0x0E
 *
 * TABLE WRITE POST DECREMENT BY 2
 */
static inline void
pic16_table_write_post_decrement_2(uint16_t word)
{
	io_program_out(0x0E, 4);
	io_program_out(word, 16);
}

/*
 * 1110 = 0x0E
 *
 * TABLE WRITE START PROGRAMMING POST INCREMENT BY 2
 *
 * DS41398B-page 12
 */
static inline void
pic16_table_write_start_programming_2(uint16_t word)
{
	io_program_out(0x0E, 4);
	io_program_out(word, 16);
}
#endif

/*
 * 1111 = 0x0F
 *
 * TABLE WRITE START PROGRAMMING
 */
static inline void
pic16_table_write_start_programming(uint16_t word)
{
	io_program_out(0x0F, 4);
	io_program_out(word, 16);
}

/*****************************************************************************
 *
 * SFR address mapping
 *
 *****************************************************************************/

/*
 * GET EEDATA ADDRESS
 */
static inline uint8_t
pic16_eedata(void)
{
	if (pic16_map[pic16_index].datasheet == DS39972B)
		return 0x73;	/* @ 0x0F73 */
	return 0xA8;		/* @ 0x0FA8 */
}

/*
 * GET EEADR ADDRESS
 */
static inline uint8_t
pic16_eeadr(void)
{
	if (pic16_map[pic16_index].datasheet == DS39972B)
		return 0x74;	/* @ 0x0F74 */
	return 0xA9;		/* @ 0x0FA9 */
}

/*
 * GET EECON1 ADDRESS
 */
static inline uint8_t
pic16_eecon1(void)
{
	if (pic16_map[pic16_index].datasheet == DS39972B)
		return 0x7F;	/* @ 0x0F7F */
	return 0xA6;		/* @ 0x0FA6 */
}

/*****************************************************************************
 *
 * Compound functions
 *
 *****************************************************************************/

/*
 * INIT DIRECT ACCESS TO CODE MEMORY (FLASH)
 */
void
pic16_init_code_memory_access(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * INIT DIRECT ACCESS TO CONFIG MEMORY (FLASH)
 */
void
pic16_init_config_memory_access(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x8E00 | eecon1);	/* BSF EECON1, EEPGD	*/
	pic16_core_instruction(0x8C00 | eecon1);	/* BSF EECON1, CFGS	*/
}

/*
 * INIT DIRECT ACCESS TO DATA MEMORY/EEPROM
 */
void
pic16_init_data_memory_access(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x9E00 | eecon1);	/* BCF EECON1, EEPGD	*/
	pic16_core_instruction(0x9C00 | eecon1);	/* BCF EECON1, CFGS	*/
}

/*
 * WRITE ENABLE
 */
void
pic16_write_enable(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x8400 | eecon1);	/* BSF EECON1, WREN	*/
}

/*
 * WRITE DISABLE
 */
void
pic16_write_disable(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x9400 | eecon1);	/* BCF EECON1, WREN	*/
}

/*
 * FLASH ROW ERASE ENABLE
 */
void
pic16_free(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x8800 | eecon1);	/* BSF EECON1, FREE	*/
}

/*
 * WRITE
 */
void
pic16_write(void)
{
	uint8_t eecon1 = pic16_eecon1();
	pic16_core_instruction(0x8200 | eecon1);	/* BSF EECON1, WR	*/
}

/*
 * SET TABLE POINTER
 */
void
pic16_set_table_pointer(uint32_t address)
{
	uint8_t addrl =  (address & 0x000000ff);	/*  7:0	 */
	uint8_t addrh = ((address & 0x0000ff00) >> 8);	/* 15:8	 */
	uint8_t addru = ((address & 0x003f0000) >> 16);	/* 21:16 */

	pic16_core_instruction(0x0E00 | addru);	/* MOVLW <Addr[21:16]>	*/
	pic16_core_instruction(0x6EF8);		/* MOVWF TBLPTRU	*/
	pic16_core_instruction(0x0E00 | addrh);	/* MOVLW <Addr[15:8]>	*/
	pic16_core_instruction(0x6EF7);		/* MOVWF TBLPTRH	*/
	pic16_core_instruction(0x0E00 | addrl);	/* MOVLW <Addr[7:0]>	*/
	pic16_core_instruction(0x6EF6);		/* MOVWF TBLPTRL	*/
}

/*
 * BLOCK ERASE
 *
 * DS39972B-page 16
 */
void
pic16_erase_block(uint32_t block)
{
	uint8_t datal =  (block & 0x000000ff);		/*  7:0  */
	uint8_t datah = ((block & 0x0000ff00) >> 8);	/* 15:8  */
	uint8_t datau = ((block & 0x00ff0000) >> 16);	/* 24:16 */

	pic16_set_table_pointer(0x3C0004);
	pic16_table_write((datal << 8) | datal);	/* Write datal to 3C0004h */
	pic16_core_instruction(0x0E05);		/* MOVLW 05h		  */
	pic16_core_instruction(0x6EF6);		/* MOVWF TBLPTRL	  */
	pic16_table_write((datah << 8) | datah);	/* Write datah to 3C0005h */
	pic16_core_instruction(0x0E06);		/* MOVLW 06h		  */
	pic16_core_instruction(0x6EF6);		/* MOVWF TBLPTRL	  */
	pic16_table_write((datau << 8) | datau);	/* Write datah to 3C0006h */
	pic16_core_instruction(0x0000);		/* NOP			  */
	pic16_core_instruction_nope();			/* NOP ERASE P11 + P10	  */
}

/*
 * ROW ERASE (PROGRAM FLASH OR IDLOCATIONS)
 *
 * DS39972B-page 19 PIC18F26K80  HIGH=P9(1ms)   LOW=P10(100us)
 * DS39622L-page 16 PIC18F2XXX   HIGH=P9(1ms)   LOW=P10(100us)
 * DS39687E-page 10 PIC18LF27J53 HIGH=P10(54ms) LOW=NDELAY
 */
void
pic16_erase_row(uint32_t address, uint32_t nrows)
{
	int h, l;

	if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
		h = pic16_map[pic16_index].p10;		/* minimum 49 or 54ms	*/
		l = 0;
	} else {
		h = pic16_map[pic16_index].p9;		/* normally 1ms		*/
		l = pic16_map[pic16_index].p10;		/* normally 100us	*/
	}
	pic16_init_code_memory_access();		/* BSF EECON1, EEPGD	*/
							/* BCF EECON1, CFGS	*/
	pic16_write_enable();				/* BSF EECON1, WREN	*/
	while (nrows--) {
		pic16_set_table_pointer(address);
		pic16_free();				/* BSF EECON1, FREE	*/
		pic16_write();				/* BSF EECON1, WR	*/
		switch (pic16_map[pic16_index].datasheet) {
		case DS30500A: /* PIC18F2431  DS30500A-page 11 */
		case DS39576B: /* PIC18F252   */
		case DS39592E: /* PIC18F1320  */
		case DS30480C: /* PIC18LF2539 */
			pic16_table_write_start_programming(0);
			break;
		case DS39972B: /* PIC18F25K80 */
			pic16_core_instruction(0x0E55);/* MOVLW 0x55		*/
			pic16_core_instruction(0x6E7E);/* MOVWF EECON2	*/
			pic16_core_instruction(0x0EAA);/* MOVLW 0xAA		*/
			pic16_core_instruction(0x6E7E);/* MOVWF EECON2	*/
		default:break;
		}
		pic16_core_instruction_nopp(h, l);	/* ROW ERASE		*/
		address += pic16_map[pic16_index].erasesize;
	}
	pic16_write_disable();				/* BCF EECON1, WREN	*/
}

/*
 * BLANK DEVICE
 *
 * DISABLE PROTECTION AND BULK ERASE
 */
void
pic16_bulk_erase(void)
{	
	pic16_program_verify();

	switch (pic16_map[pic16_index].datasheet) {	/* CHIP ERASE		*/
	case DS30500A: /* PIC18F2431  DS30500A-page 9  */
	case DS39576B: /* PIC18F252   */
	case DS39592E: /* PIC18F1320  DS39592F-page 10 */
		       /* PIC18F2320  DS39592F-page 10 */
	case DS30480C: /* PIC18LF2539 */
		pic16_set_table_pointer(0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x0080);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(0x0000);	/* NOP			*/
		pic16_core_instruction_nope();		/* NOP ERASE P11 + P10	*/
		break;

	case DS39622K: /* PIC18F2550 */
		pic16_set_table_pointer(0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x3f3f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(0x0000);	/* NOP			*/
		pic16_core_instruction_nope();		/* NOP ERASE P11 + P10	*/
		break;

	case DS39752B: /* PIC18F1230/1330 */
		pic16_set_table_pointer(0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x8787);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(0x0000);	/* NOP			*/
		pic16_core_instruction_nope();		/* NOP ERASE P11 + P10	*/
		break;

	case DS41342E: /* PIC18F14K50 */
	case DS41398B: /* PIC18F25K22 */
	case DS41630B: /* PIC18F25K50 */
	case DS41357B: /* PIC18F14K22 */
	case DS41297F: /* PIC18F26K20 */
		pic16_set_table_pointer(0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x0f0f);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x8f8f);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(0x0000);	/* NOP			*/
		pic16_core_instruction_nope();		/* NOP ERASE P11 + P10	*/
		break;

	case DS39972B: /* PIC18F26K80 */
		/*
		 * Description			Data (3C0006h:3C0004h)
	 	 * Erase Data EEPROM		800004h
		 * Erase Boot Block		800005h
		 * Erase Config Bits		800002h
		 * Erase Code EEPROM Block 0    800104h
		 * Erase Code EEPROM Block 1    800204h
		 * Erase Code EEPROM Block 2    800404h
		 * Erase Code EEPROM Block 3    800804h
		 */
		pic16_erase_block(0x800104);		/* ERASE CODE BLOCK 0 TABLE 3-2 */
		pic16_erase_block(0x800204);		/* ERASE CODE BLOCK 1 TABLE 3-3 */
		pic16_erase_block(0x800404);		/* ERASE CODE BLOCK 2 TABLE 3-4 */
		pic16_erase_block(0x800804);		/* ERASE CODE BLOCK 3 TABLE 3-5 */
		pic16_erase_block(0x800005);		/* ERASE BOOT BLOCK   TABLE 3-6 */
		pic16_erase_block(0x800002);		/* ERASE CONFIG BITS  TABLE 3-7 */
		pic16_erase_row(PIC16_ID_LOW, 1);	/* ERASE IDLOC		  	*/
		break;

	case DS39687E: /* PIC18LF27J53 */
		pic16_set_table_pointer(0x3C0005);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x0101);		/* BULK ERASE CHIP	*/
		pic16_set_table_pointer(0x3C0004);	/* BULK ERASE CONFIG &	*/
		pic16_table_write(0x8080);		/* BULK ERASE CHIP	*/
		pic16_core_instruction(0x0000);	/* NOP			*/
		pic16_core_instruction_nope();		/* NOP ERASE P11 + P10	*/
		break;

	default:printf("%s: information: unimplemented\n", __func__);
		break;
	}

	pic16_standby();
}

/*
 * ROW ERASE
 */
void
pic16_row_erase(uint32_t row, uint32_t nrows)
{	
	if (row == PIC_ERASE_EEPROM) {
		if (pic16_map[pic16_index].eeprom == 0) {
			printf("%s: information: data EEPROM/flash is not supported on this device\n", __func__);
			return; /* NO EEPROM */
		}

		uint16_t address = PIC_REGIONDATA;
		
		pic16_program_verify();

		/* ERASE EEPROM */
		for (int i = 0; i < pic16_map[pic16_index].eeprom; ++i) {
			pic16_set_data_pointer(address++);
			pic16_write_data_memory(0xFF);
		}

		pic16_standby();
		return;
	}

	if (row == PIC_ERASE_ID) {
		if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
			printf("%s: information: IDLOCATIONS are not supported on this device\n", __func__);
			return;
		}

		pic16_program_verify();

		/* ERASE IDLOCATIONS */
		pic16_erase_row(PIC16_ID_LOW, 1);

		pic16_standby();
		return;
	}
#if 0
	if (row == PIC_ERASE_CONFIG) {
		if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
			pic16_program_verify();

			/* ERASE CONFIG */
			pic16_erase_row(pic16_map[pic16_index].configaddr, 1);

			pic16_standby();
		} else {
			for (int i = 0; i < pic16_map[pic16_index].configsize; i++) {
				pic16_conf.config[i] = 0xFF;
			}
			/* ERASE CONFIG */
			pic16_write_config();
		}
		return;
	}
#endif
	/*
	 * ERASE PROGRAM FLASH ROW(S)
	 */

	uint32_t maxrows = 2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize;
	if (row >= maxrows) {
		printf("%s: information: program flash row is out of range\n", __func__);
		return;
	}

	uint32_t numrows = maxrows - row;
	if (nrows > numrows) {
		nrows = numrows;
	}

	uint32_t address = row * pic16_map[pic16_index].erasesize;

	pic16_program_verify();

	/* ERASE PROGRAM FLASH */
	pic16_erase_row(address, nrows);

	pic16_standby();
}

/*****************************************************************************
 *
 * Read block data
 *
 *****************************************************************************/

/*
 * READ CONFIGURATION MEMORY
 *
 * IDLOC    200000 ... 200007 if supported
 * CONFIG   300000 ... 30000d or XXXXXX ... XXXXXX + 0xd
 * DEVICEID 3ffffe ... 3fffff
 */
int
pic16_read_config_memory(void)
{
	uint32_t dev;

	/* NULL device */
	pic16_index = PIC16_SIZE;

	/* Reset configuraton */
	memset(&pic16_conf, 0, sizeof(pic16_conf));

	pic16_program_verify();

	/* Device selected by user */
	if (p.devicename[0]) {
		for (dev = 0; pic16_map[dev].deviceid; ++dev) {
			if (strcasecmp(pic16_map[dev].devicename, p.devicename) == 0) {
				pic16_conf.deviceid = pic16_map[dev].deviceid;
				break;
			}
		}
		if (pic16_map[dev].deviceid == 0) {
			printf("%s: information: unknown device: [%s]\n", __func__, p.devicename);
			pic16_standby();
			return -1;
		}
	}
	/* Device detect */
	else {
		/* Device id */
		pic16_set_table_pointer(PIC16_DEVICEID_LOW);
		pic16_conf.deviceid = pic16_table_read_post_increment();
		pic16_conf.deviceid |= pic16_table_read_post_increment() << 8;
		dev = 0;
		while (pic16_map[dev].deviceid) {
			uint16_t deviceid;
			if (!pic16_map[dev].devidrev4)
				deviceid = pic16_conf.deviceid & PIC16_DEVICEID_IDMASK;
			else
				deviceid = pic16_conf.deviceid & PIC16_DEVICEID_IDMASK_REV4;
			if (pic16_map[dev].deviceid == deviceid)
				break;
			++dev;
		}
		if (!pic16_map[dev].deviceid) {
			/*
			 * VELLEMAN K8048 SWITCH IN STANDBY [0000] 0000 rev  0
			 * VELLEMAN K8048 NO POWER	    [FFFF] ffe0 rev 31
			 * VELLEMAN K0848 SWITCH IN RUN     [FFFF] ffe0 rev 31
			 * VELLEMAN K0848 SWITCH IN PROG    [XXXX]
			 */
			if (pic16_conf.deviceid == 0x0000 || pic16_conf.deviceid == 0xFFFF) {
				printf("%s: information: %s.\n",
					__func__, io_fault(pic16_conf.deviceid));
			} else {
				printf("%s: information: device unknown: [%04X]\n",
					__func__, pic16_conf.deviceid);
			}
			pic16_standby();
			return -1;
		}
	}

	/* Device recognised */
	pic16_index = dev;

	/* Id location 1..8 */
	if (pic16_map[pic16_index].datasheet != DS39687E) { /* !PIC18J */
		pic16_set_table_pointer(PIC16_ID_LOW);
		for (uint32_t i = 0; i < 8; i++)
			pic16_conf.idloc[i] = pic16_table_read_post_increment();
	}

	/* Config 1..N */
	pic16_set_table_pointer(pic16_map[pic16_index].configaddr);
	for (uint32_t i = 0; i < pic16_map[pic16_index].configsize; i++)
		pic16_conf.config[i] = pic16_table_read_post_increment();

	pic16_standby();

	return 0;
}

/*
 * GET PROGRAM FLASH SIZE
 *
 *  RETURN SIZE IN WORDS
 */
uint32_t
pic16_get_program_size(uint32_t *addr)
{
	*addr = 0;

	return pic16_map[pic16_index].flash;
}

/*
 * GET DATA EEPROM/FLASH SIZE
 *
 *  RETURN SIZE IN BYTES
 */
uint32_t
pic16_get_data_size(uint32_t *addr)
{
	*addr = PIC16_DATA_LOW;

	return pic16_map[pic16_index].eeprom;
}

/*
 * READ PROGRAM FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16_read_program_memory_block(uint32_t *data, uint32_t addr, uint32_t size)
{
	pic16_program_verify();
	pic16_init_code_memory_access();

	pic16_set_table_pointer(addr);
	for (uint32_t i = 0; i < size; ++i) {
		uint16_t word = pic16_table_read_post_increment();
		word |= pic16_table_read_post_increment() << 8;
		data[i] = (int32_t)word;
	}

	pic16_standby();

	return addr;
}

/*
 * READ DATA EEPROM/FLASH MEMORY BLOCK ADDR .. ADDR + SIZE
 *
 *  RETURN ADDR
 */
uint32_t
pic16_read_data_memory_block(uint16_t *data, uint32_t addr, uint16_t size)
{
	pic16_program_verify();
	pic16_init_data_memory_access();

	for (uint32_t i = 0; i < size; ++i) {
		pic16_set_data_pointer(addr + i);
		data[i] = (uint16_t)pic16_read_data_memory();
	}

	pic16_standby();

	return addr;
}

/*****************************************************************************
 *
 * Program Data (EEPROM)
 *
 *****************************************************************************/

/*
 * SET DATA POINTER (EEPROM)
 */
void
pic16_set_data_pointer(uint16_t address)
{
	uint8_t addrl =  (address & 0x00ff);		/*  7:0	*/
	uint8_t addrh = ((address & 0xff00) >> 8);	/* 15:8	*/
	uint8_t eeadrl = pic16_eeadr();
	uint8_t eeadrh = eeadrl + 1;

	pic16_core_instruction(0x0E00 | addrl);	/* MOVLW <Addr>	*/
	pic16_core_instruction(0x6E00 | eeadrl);	/* MOVWF EEADR	*/
	pic16_core_instruction(0x0E00 | addrh);	/* MOVLW <AddrH>*/
	pic16_core_instruction(0x6E00 | eeadrh);	/* MOVWF EEADRH	*/
}

/*
 * READ DATA MEMORY (EEPROM)
 */
uint8_t
pic16_read_data_memory(void)
{
	uint8_t eedata = pic16_eedata();
	uint8_t eecon1 = pic16_eecon1();

	pic16_core_instruction(0x8000 | eecon1);	/* BSF EECON1, RD	*/
	pic16_core_instruction(0x5000 | eedata);	/* MOVF EEDATA, W, 0	*/
	pic16_core_instruction(0x6EF5);		/* MOVWF TABLAT		*/
	pic16_core_instruction(0x0000);		/* NOP			*/

	return pic16_shift_out_tablat_register();
}

/*
 * WRITE DATA MEMORY (EEPROM)
 *
 * DS39592F-page 17
 */
void
pic16_write_data_memory(uint8_t data)
{
	struct timeval tv1, tv2, tv3;
	uint8_t eedata = pic16_eedata();
	uint8_t eecon1 = pic16_eecon1();
	uint8_t incomplete;

	pic16_core_instruction(0x0E00 | data);	/* MOVLW <Data>		*/
	pic16_core_instruction(0x6E00 | eedata);	/* MOVWF EEDATA		*/

	pic16_write_enable();				/* BSF EECON1, WREN     */

	switch (pic16_map[pic16_index].datasheet) {
	case DS30500A: /* PIC18F2431  */
	case DS39576B: /* PIC18F252   */
	case DS39592E: /* PIC18F1320  */
		       /* PIC18F2320  */
	case DS30480C: /* PIC18LF2539 */
		pic16_core_instruction(0x0E55);	/* MOVLW 0x55		*/
		pic16_core_instruction(0x6EA7);	/* MOVWF EECON2		*/
		pic16_core_instruction(0x0EAA);	/* MOVLW 0xAA		*/
		pic16_core_instruction(0x6EA7);	/* MOVWF EECON2		*/
		break;
	}
	pic16_write();					/* BSF EECON1, WR       */

	gettimeofday(&tv1, NULL);
	do	/* Until write completes or times out */
	{
		pic16_core_instruction(0x5000 | eecon1);/* MOVF EECON1, W, 0	*/
		pic16_core_instruction(0x6EF5);	/* MOVWF TABLAT		*/
		pic16_core_instruction(0x0000);	/* NOP			*/

		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &tv3);

		incomplete = pic16_shift_out_tablat_register() & 0x02;
	}
	while (incomplete && (tv3.tv_sec < PIC_TIMEOUT));

	pic16_write_disable();				/* BCF EECON1, WREN     */

	if (tv3.tv_sec >= PIC_TIMEOUT)
		printf("%s: information: data write timed out.\n", __func__);
}

/*****************************************************************************
 *
 * Program Code/ID Panel
 *
 *****************************************************************************/

/*
 * WRITE PANEL INIT
 */
void
pic16_write_panel_init(void)
{
	switch (pic16_map[pic16_index].datasheet) {
 		/* INIT SINGLE PANEL WRITES */
	case DS30500A: /* PIC18F2431  */
	case DS39576B: /* PIC18F252   */
	case DS39592E: /* PIC18F1320  */
		       /* PIC18F2320  */
	case DS30480C: /* PIC18LF2539 */
		pic16_init_config_memory_access();	/* SWITCH TO CONFIG	*/
		pic16_set_table_pointer(0x3C0006);	/* PANEL MODE CONFIG &	*/
		pic16_table_write(0x0000);		/* SINGLE PANEL WRITES	*/
		break;
	default:/* MODERN DEVICES ARE ALL SINGLE PANEL */
		break;
	}
	pic16_init_code_memory_access();		/* SWITCH TO CODE	*/
}

/*
 * WRITE PANEL
 *
 * DS39622L-page 18 HIGH=P9(1ms)   LOW=P10(100us)
 * DS39687E-page 13 HIGH=P9(3.4ms) LOW=NDELAY
 */
void
pic16_write_panel(uint32_t region, uint32_t address, uint32_t *panel, uint32_t panel_size)
{
	uint32_t h, l, i;
	uint16_t word;

	switch (pic16_map[pic16_index].datasheet) {
	case DS39687E: /* PIC18LF27J53 */
		h = pic16_map[pic16_index].p9;		/* minimum 1.2 or 3.5ms */
		l = 0;
		break;
	default:h = pic16_map[pic16_index].p9;
		l = pic16_map[pic16_index].p10;
		break;
	}
#if 0
	pic16_write_enable();				/* BSF EECON1, WREN	*/
#endif
	pic16_set_table_pointer(address);
	for (i = 0; i < (panel_size - 2); i += 2) {
		word = (panel[i + 1] << 8) | panel[i];
		pic16_table_write_post_increment_2(word);
	}
	word = (panel[i + 1] << 8) | panel[i];
	pic16_table_write_start_programming(word);
	pic16_core_instruction_nopp(h, l);		/* PANEL PROGRAM	*/
#if 0
	pic16_write_disable();				/* BCF EECON1, WREN	*/
#endif
}

/*****************************************************************************
 *
 * Program Configuration
 *
 * NOT PIC18J
 *
 *****************************************************************************/

/*
 * GOTO 100000
 */
void
pic16_goto100000(void)
{
	pic16_core_instruction(0xEF00);		/* GOTO 0x100000	*/
	pic16_core_instruction(0xF800);		/* GOTO 0x100000	*/
}

/*
 * WRITE CONFIG REGISTER
 */
void
pic16_write_configreg(uint8_t index, uint32_t h, uint32_t l)
{
	uint32_t address = PIC16_CONFIG_LOW + index;
	uint16_t data = (pic16_conf.config[index + 1] << 8) | pic16_conf.config[index];

	pic16_set_table_pointer(address);		/* EVEN ADDRESS			 */
	pic16_table_write_start_programming(data);	/* <MSB IGNORED><LSB>		 */
	pic16_core_instruction_nopp(h, l);		/* CONFIG PROGRAM H=P9/P9A L=P10 */
	pic16_set_table_pointer(address + 1);	/* ODD ADDRESS			 */
	pic16_table_write_start_programming(data);	/* <MSB><LSB IGNORED>		 */
	pic16_core_instruction_nopp(h, l);		/* CONFIG PROGRAM H=P9/P9A L=P10 */
}

/*
 * WRITE CONFIGURATION (CONFIG_6H MUST BE WRITTEN LAST)
 */
uint32_t
pic16_write_config(void)
{
	uint32_t h, l;

	if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
		return 0;
	}

	h = (pic16_map[pic16_index].p9a != 0)
		? (pic16_map[pic16_index].p9a) : (pic16_map[pic16_index].p9);
	l = pic16_map[pic16_index].p10;

	pic16_program_verify();

	pic16_init_config_memory_access();
	pic16_goto100000();
#if 0
	pic16_write_enable();				/* BSF EECON1, WREN	*/
#endif
	pic16_write_configreg(PIC16_CONFIG_1L, h, l);
	pic16_write_configreg(PIC16_CONFIG_2L, h, l);
	pic16_write_configreg(PIC16_CONFIG_3L, h, l);
	pic16_write_configreg(PIC16_CONFIG_4L, h, l);
	pic16_write_configreg(PIC16_CONFIG_5L, h, l);
	pic16_write_configreg(PIC16_CONFIG_7L, h, l);
	pic16_write_configreg(PIC16_CONFIG_6L, h, l);
#if 0	
	pic16_write_disable();				/* BCF EECON1, WREN	*/
#endif
	pic16_standby();

	return 14;
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
 *	000000 .. 1fffff
 *
 *  RETURN PIC_REGIONID:
 *	200000 .. 200007
 *
 *  RETURN PIC_REGIONCONFIG:
 *	300000 .. 30000d
 *
 *  RETURN PIC_REGIONDATA:
 *	f00000 .. ffffff
 */
uint32_t
pic16_getregion(uint32_t address)
{
	/* CODE */
	uint32_t code_high = (pic16_map[pic16_index].flash << 1) - 1;

	if (address <= code_high) {
		return PIC_REGIONCODE;
	}
	/* EEPROM */
	if (pic16_map[pic16_index].eeprom) {
		uint32_t data_high = PIC16_DATA_LOW + pic16_map[pic16_index].eeprom - 1;

		if (address >= PIC16_DATA_LOW && address <= data_high) {
			return PIC_REGIONDATA;
		}
	}
	/* IDLOC/CONFIG */
	if (pic16_map[pic16_index].datasheet != DS39687E) { /* !PIC18J */
		/* IDLOC */
		if (address >= PIC16_ID_LOW && address <= PIC16_ID_HIGH) {
			return PIC_REGIONID;
		}
		/* CONFIG */
		if (address >= PIC16_CONFIG_LOW && address <= PIC16_CONFIG_HIGH) {
			return PIC_REGIONCONFIG;
		}
	}
	if (p.f)
	        fprintf(p.f, "%s: warning: address unsupported [%06X]\n", __func__, address);
	return PIC_REGIONNOTSUP;
}

/*
 * INIT REGION FOR WRITING (CONFIG DELAYED)
 *
 *  RETURN REGION IF WRITING SUPPORTED
 */
static inline uint32_t
pic16_init_writeregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
		pic_write_panel(PIC_PANEL_BEGIN, PIC_REGIONCODE, pic16_map[pic16_index].panelsize);
		return region;
	case PIC_REGIONID:
		pic_write_panel(PIC_PANEL_BEGIN, PIC_REGIONID, PIC16_ID_PANEL_SIZE);
		return region;
	case PIC_REGIONCONFIG:
		return region;
	case PIC_REGIONDATA:
		pic16_init_data_memory_access();
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * WRITE REGION (CACHE CONFIG)
 */
static inline void
pic16_writeregion(uint32_t address, uint32_t region, uint8_t data)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
		pic_write_panel(PIC_PANEL_UPDATE, address, data);
		return;
	case PIC_REGIONCONFIG:
		pic16_conf.config[address & PIC16_CONFIG_MASK] = data;
		return;
	case PIC_REGIONDATA:
		pic16_set_data_pointer(address & PIC16_DATA_MASK);
		pic16_write_data_memory(data);
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
pic16_init_verifyregion(uint32_t region)
{
	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
	case PIC_REGIONCONFIG:
		pic16_init_code_memory_access();
		return region;
	case PIC_REGIONDATA:
		pic16_init_data_memory_access();
		return region;
	}
	if (p.f)
		fprintf(p.f, "%s: warning: region unsupported [%d]\n", __func__, region);
	return PIC_REGIONNOTSUP;
}

/*
 * VERIFY REGION
 *
 *  RETURN BYTE FAILURE COUNT
 */
static inline uint32_t
pic16_verifyregion(uint32_t address, uint32_t region, uint16_t index, uint8_t wdata)
{
	uint8_t vdata = 0;

	switch (region) {
	case PIC_REGIONCODE:
	case PIC_REGIONID:
		if (index == 0)
			pic16_set_table_pointer(address);
		vdata = pic16_table_read_post_increment();
		break;
	case PIC_REGIONCONFIG:
		/* Can't verify config */
		return wdata;
	case PIC_REGIONDATA:
		pic16_set_data_pointer(address & PIC16_DATA_MASK);
		vdata = pic16_read_data_memory();
		break;
	default:if (p.f)
			fprintf(p.f, "%s: warning: region unsupported [%d]\n",
				__func__, region);
		return wdata;
	}
	if (vdata != wdata && p.f) {
		fprintf(p.f, "%s: error: read [%02X] expected [%02X] at [%06X]\n",
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
pic16_program_begin(void)
{
	pic16_program_verify();
	pic16_write_panel_init();
}

/*
 * PROGRAM DATA
 */
uint32_t
pic16_program_data(uint32_t current_region, pic_data *pdata)
{
	uint32_t address, new_region;

	for (uint32_t i = 0; i < pdata->nbytes; ++i) {
		address = pdata->address + i;
		new_region = pic16_getregion(address);
		if (new_region != current_region) {
			pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
			current_region = pic16_init_writeregion(new_region);
		}
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		pic16_writeregion(address, current_region, pdata->bytes[i]);
	}
	return current_region;
}

/*
 * END PROGRAMMING
 */
void
pic16_program_end(int config)
{
	pic_write_panel(PIC_PANEL_END, PIC_VOID, PIC_VOID);
	pic16_standby();
        if (config)
                pic16_write_config();
}

/*
 * VERIFY DATA
 */
uint32_t
pic16_verify_data(uint32_t current_region, pic_data *pdata, uint32_t *fail)
{
	uint32_t address, new_region, vdata;

	for (uint32_t i = 0; i < pdata->nbytes; ++i) {
		address = pdata->address + i;
		new_region = pic16_getregion(address);
		if (new_region != current_region)
			current_region = pic16_init_verifyregion(new_region);
		if (current_region == PIC_REGIONNOTSUP)
			continue;
		vdata = pic16_verifyregion(address, current_region, i, pdata->bytes[i]);
		if (vdata != pdata->bytes[i]) {
			pdata->bytes[i] = vdata;
			(*fail)++;
		}
	}
	return current_region;
}

/*
 * VIEW DATA
 */
void
pic16_view_data(pic_data *pdata)
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
pic16_dumpdeviceid(void)
{
	if (pic16_map[pic16_index].flash < 0x10000) {
		printf("[000000] [PROGRAM]     %04X WORDS (%04X ROWS OF %04X WORDS)\n",
			pic16_map[pic16_index].flash,
				2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize,
				pic16_map[pic16_index].erasesize / 2
				);
	} else {
		printf("[000000] [PROGRAM]    %05X WORDS (%04X ROWS OF %04X WORDS)\n",
			pic16_map[pic16_index].flash,
				2 * pic16_map[pic16_index].flash / pic16_map[pic16_index].erasesize,
				pic16_map[pic16_index].erasesize / 2
				);
	}

	if (pic16_map[pic16_index].datasheet != DS39687E) { /* !PIC18J */
		for (uint32_t i = 0; i < 8; ++i) {
			printf("[20000%d] [IDLOCATION%d] %02X %c\n",
				i, 1 + i,
				pic16_conf.idloc[i], PIC_CHAR(0xFF & pic16_conf.idloc[i]));
		}
	}

	pic16_dumpconfig(PIC_BRIEF);
	
	if (p.devicename[0]) {
		printf("[3FFFFE] [DEVICEID]    %04X", pic16_conf.deviceid);
	} else {
		uint16_t deviceid, devshift, revision;
		if (!pic16_map[pic16_index].devidrev4) {
			deviceid = pic16_conf.deviceid & PIC16_DEVICEID_IDMASK;
			devshift = deviceid >> PIC16_DEVICEID_SHIFT;
			revision = pic16_conf.deviceid & PIC16_DEVICEID_REVMASK;
		} else {
			deviceid = pic16_conf.deviceid & PIC16_DEVICEID_IDMASK_REV4;
			devshift = deviceid >> PIC16_DEVICEID_SHIFT_REV4;
			revision = pic16_conf.deviceid & PIC16_DEVICEID_REVMASK_REV4;
		}
		printf("[3FFFFE] [DEVICEID]    %04X DEV:%03X REV:%02X",
		/* Physical */	pic16_conf.deviceid,
		/* Shifted  */	devshift,
		/* Revision */	revision);
	}
	printf(" %s\n", pic16_map[pic16_index].devicename);

	if (pic16_map[pic16_index].eeprom) {
		printf("[%06X] [DATA]	       %04X BYTES\n",
			PIC16_DATA_LOW, pic16_map[pic16_index].eeprom);
	}
}

/*
 * DUMP CONFIG WORD DETAILS FOR DEVICE
 */
void
pic16_dumpconfig(int mode)
{
	for (uint32_t i = 0; i < pic16_map[pic16_index].configsize; i += 2) {
		printf("[%06X] [CONFIG%d]     %04X\n",
			pic16_map[pic16_index].configaddr + i,
			i / 2 + 1,
			(pic16_conf.config[i + 1] << 8) | pic16_conf.config[i]);
	}
#ifdef VERBOSE
	if (mode == PIC_VERBOSE)
		pic16_dumpconfig_verbose();
#endif
}

/*
 * DUMP VERBOSE CONFIG WORD DETAILS FOR DEVICE (INCOMPLETE)
 */
#ifdef VERBOSE
void
pic16_dumpconfig_verbose(void)
{
	uint8_t w;

	if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
		return;
	}

	printf("\n[CONFIG_1H] [%02X]\n", pic16_conf.config[PIC16_CONFIG_1H]);
	w = pic16_conf.config[PIC16_CONFIG_1H] & 0x80;
	if (w)
		printf("\t[IESO] Enabled\n");
	else
		printf("\t[IESO] Disabled\n");
	w = pic16_conf.config[PIC16_CONFIG_1H] & 0x40;
	if (w)
		printf("\t[FCMEN/FSCM] Enabled\n");
	else
		printf("\t[FCMEN/FSCM] Disabled\n");
	w = pic16_conf.config[PIC16_CONFIG_1H] & 0x0F;
	switch (w) {
	case 15:
	case 14:
	case 13:
	case 12:printf("\t[FOSC3:FOSC0] 11XX EXT RC CLKOUT\n");	break;
	case 11:printf("\t[FOSC3:FOSC0] 1011 UNKNOWN\n");	break;
	case 10:printf("\t[FOSC3:FOSC0] 1010 UNKNOWN\n");	break;
	case 9: printf("\t[FOSC3:FOSC0] 1001 INT RC CLKOUT\n");	break;
	case 8: printf("\t[FOSC3:FOSC0] 1000 INT RC NO CLKOUT\n");break;
	case 7: printf("\t[FOSC3:FOSC0] 0111 EXT RC\n");	break;
	case 6: printf("\t[FOSC3:FOSC0] 0110 HS\n");		break;
	case 5: printf("\t[FOSC3:FOSC0] 0101 EC\n");		break;
	case 4: printf("\t[FOSC3:FOSC0] 0100 EC CLKOUT\n");	break;
	case 3: printf("\t[FOSC3:FOSC0] 0011 RC\n");		break;
	case 2: printf("\t[FOSC3:FOSC0] 0010 HS\n");		break;
	case 1: printf("\t[FOSC3:FOSC0] 0001 XT\n");		break;
	case 0: printf("\t[FOSC3:FOSC0] 0000 LP\n");		break;
	}

	printf("\n[CONFIG_2L] [%02X]\n", pic16_conf.config[PIC16_CONFIG_2L]);
	w = (pic16_conf.config[PIC16_CONFIG_2L] & 0x0C) >> 2;
	switch (w) {
	case 3: printf("\t[BORV1:BORV0] 2.0/2.5V\n");	break;
	case 2: printf("\t[BORV1:BORV0] 2.7V\n");	break;
	case 1: printf("\t[BORV1:BORV0] 4.2V\n");	break;
	case 0: printf("\t[BORV1:BORV0] 4.5V\n");	break;
	}
	w = pic16_conf.config[PIC16_CONFIG_2L] & 0x02;
	if (w)
		printf("\t[BOREN] Enabled\n");
	else
		printf("\t[BOREN] Disabled\n");

	w = pic16_conf.config[PIC16_CONFIG_2L] & 0x01;
	if (w)
		printf("\t[PWRTEN] Disabled\n");
	else
		printf("\t[PWRTEN] Enabled\n");

	/* XXX WDT o/p is wrong for the PIC18F25K22 */
	printf("\n[CONFIG_2H] [%02X]\n", pic16_conf.config[PIC16_CONFIG_2H]);
	w = (pic16_conf.config[PIC16_CONFIG_2H] & 0x1E) >> 1;
	switch (w) {
	case 15: printf("\t[WDTP3:WDTP0] 1:32768\n"); break;
	case 14: printf("\t[WDTP3:WDTP0] 1:16384\n"); break;
	case 13: printf("\t[WDTP3:WDTP0] 1:8192\n");  break;
	case 12: printf("\t[WDTP3:WDTP0] 1:4096\n");  break;
	case 11: printf("\t[WDTP3:WDTP0] 1:2048\n");  break;
	case 10: printf("\t[WDTP3:WDTP0] 1:1024\n");  break;
	case  9: printf("\t[WDTP3:WDTP0] 1:512\n");   break;
	case  8: printf("\t[WDTP3:WDTP0] 1:256\n");   break;
	case  7: printf("\t[WDTP3:WDTP0] 1:128\n");   break;
	case  6: printf("\t[WDTP3:WDTP0] 1:64\n");    break;
	case  5: printf("\t[WDTP3:WDTP0] 1:32\n");    break;
	case  4: printf("\t[WDTP3:WDTP0] 1:16\n");    break;
	case  3: printf("\t[WDTP3:WDTP0] 1:8\n");     break;
	case  2: printf("\t[WDTP3:WDTP0] 1:4\n");     break;
	case  1: printf("\t[WDTP3:WDTP0] 1:2\n");     break;
	case  0: printf("\t[WDTP3:WDTP0] 1:1\n");     break;
	}
	w = pic16_conf.config[PIC16_CONFIG_2H] & 0x01;
	if (w)
		printf("\t[WDTEN] Enabled\n");
	else
		printf("\t[WDTEN] Disabled\n");

	printf("\n[CONFIG_3H] [%02X]\n", pic16_conf.config[PIC16_CONFIG_3H]);
	w = pic16_conf.config[PIC16_CONFIG_3H] & 0x80;
	if (w)
		printf("\t[MCLRE] Enabled\n");
	else
		printf("\t[MCLRE] Disabled\n");
	w = pic16_conf.config[PIC16_CONFIG_3H] & 0x02;
	if (w)
		printf("\t[PBADEN/PBAD] Enabled (*)\n");
	else
		printf("\t[PBADEN/PBAD] Disabled (*)\n");
	w = pic16_conf.config[PIC16_CONFIG_3H] & 0x01;
	if (w)
		printf("\t[CCP2MX] RC1 (*)\n");
	else
		printf("\t[CCP2MX] RB3 (*)\n");

	printf("\n[CONFIG_4L] [%02X]\n", pic16_conf.config[PIC16_CONFIG_4L]);
	w = pic16_conf.config[PIC16_CONFIG_4L] & 0x80;
	if (w)
		printf("\t[DEBUG] Disabled\n");
	else
		printf("\t[DEBUG] Enabled\n");
	w = pic16_conf.config[PIC16_CONFIG_4L] & 0x04;
	if (w)
		printf("\t[LVP] Enabled\n");
	else
		printf("\t[LVP] Disabled\n");
	w = pic16_conf.config[PIC16_CONFIG_4L] & 0x01;
	if (w)
		printf("\t[STVREN] Enabled\n");
	else
		printf("\t[STVREN] Disabled\n");

	/*
	 * Protection bits
	 */
	printf("\n[CONFIG_5L] [%02X]", pic16_conf.config[PIC16_CONFIG_5L]);
	w = pic16_conf.config[PIC16_CONFIG_5L] & 0x08;
	if (w)
		printf("\t[CP3]   Off");
	else
		printf("\t[CP3]   On");
	w = pic16_conf.config[PIC16_CONFIG_5L] & 0x04;
	if (w)
		printf("\t[CP2]   Off");
	else
		printf("\t[CP2]   On");
	w = pic16_conf.config[PIC16_CONFIG_5L] & 0x02;
	if (w)
		printf("\t[CP1]   Off");
	else
		printf("\t[CP1]   On");
	w = pic16_conf.config[PIC16_CONFIG_5L] & 0x01;
	if (w)
		printf("\t[CP0]   Off");
	else
		printf("\t[CP0]   On");

	printf("\n[CONFIG_5H] [%02X]", pic16_conf.config[PIC16_CONFIG_5H]);
	w = pic16_conf.config[PIC16_CONFIG_5H] & 0x80;
	if (w)
		printf("\t[CPD]   Off");
	else
		printf("\t[CPD]   On");
	w = pic16_conf.config[PIC16_CONFIG_5H] & 0x40;
	if (w)
		printf("\t[CPB]   Off");
	else
		printf("\t[CPB]   On");

	printf("\n[CONFIG_6L] [%02X]", pic16_conf.config[PIC16_CONFIG_6L]);
	w = pic16_conf.config[PIC16_CONFIG_6L] & 0x08;
	if (w)
		printf("\t[WRT3]  Off");
	else
		printf("\t[WRT3]  On");
	w = pic16_conf.config[PIC16_CONFIG_6L] & 0x04;
	if (w)
		printf("\t[WRT2]  Off");
	else
		printf("\t[WRT2]  On");
	w = pic16_conf.config[PIC16_CONFIG_6L] & 0x02;
	if (w)
		printf("\t[WRT1]  Off");
	else
		printf("\t[WRT1]  On");
	w = pic16_conf.config[PIC16_CONFIG_6L] & 0x01;
	if (w)
		printf("\t[WRT0]  Off");
	else
		printf("\t[WRT0]  On");

	printf("\n[CONFIG_6H] [%02X]", pic16_conf.config[PIC16_CONFIG_6H]);
	w = pic16_conf.config[PIC16_CONFIG_6H] & 0x80;
	if (w)
		printf("\t[WRTD]  Off");
	else
		printf("\t[WRTD]  On");
	w = pic16_conf.config[PIC16_CONFIG_6H] & 0x40;
	if (w)
		printf("\t[WRTB]  Off");
	else
		printf("\t[WRTB]  On");
	w = pic16_conf.config[PIC16_CONFIG_6H] & 0x20;
	if (w)
		printf("\t[WRTC]  Off");
	else
		printf("\t[WRTC]  On");

	printf("\n[CONFIG_7L] [%02X]", pic16_conf.config[PIC16_CONFIG_7L]);
	w = pic16_conf.config[PIC16_CONFIG_7L] & 0x08;
	if (w)
		printf("\t[EBTR3] Off");
	else
		printf("\t[EBTR3] On");
	w = pic16_conf.config[PIC16_CONFIG_7L] & 0x04;
	if (w)
		printf("\t[EBTR2] Off");
	else
		printf("\t[EBTR2] On");
	w = pic16_conf.config[PIC16_CONFIG_7L] & 0x02;
	if (w)
		printf("\t[EBTR1] Off");
	else
		printf("\t[EBTR1] On");
	w = pic16_conf.config[PIC16_CONFIG_7L] & 0x01;
	if (w)
		printf("\t[EBTR0] Off");
	else
		printf("\t[EBTR0] On");

	printf("\n[CONFIG_7H] [%02X]", pic16_conf.config[PIC16_CONFIG_7H]);
	w = pic16_conf.config[PIC16_CONFIG_7H] & 0x40;
	if (w)
		printf("\t[EBTRB] Off\n");
	else
		printf("\t[EBTRB] On\n");

	printf("\n(*) Feature is device dependant.\n");
}
#endif /* VERBOSE */

/*
 * DUMP HEX FLASH WORDS
 */
void
pic16_dumphexcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j, nlines = 0;

	for (i = 0; i < size; address += 16, i += 8) {
		if (pic_mtcode(PIC16_MASK, 8, &data[i]))
			continue;
		printf("[%06X] ", address);
		for (j = 0; j < 8; ++j)
			printf("%04X ", data[i + j] & PIC16_MASK);
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
pic16_dumpinhxcode(uint32_t address, uint32_t size, uint32_t *data)
{
	uint32_t i, j;

	/* 16-bit: Extended address */
	pic_dumpaddr(address, 1);

	for (i = 0; i < size; address += 16, i += 8) {
		if (pic_mtcode(PIC16_MASK, 8, &data[i]))
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
pic16_dumphexdata(uint32_t address, uint32_t size, uint16_t *data)
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
pic16_dumpinhxdata(uint32_t address, uint32_t size, uint16_t *data)
{
	uint32_t i, j;

	/* PIC18: Extended address = 0x00F0 (EEPROM: 0xF00000) */
	pic_dumpaddr(PIC16_DATA_LOW, 1);

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
 * DUMP DEVICE CONFIGURATION
 */
void
pic16_dumpdevice(void)
{
	uint32_t i;

	if (pic16_map[pic16_index].datasheet == DS39687E) { /* PIC18J */
		/* NOT SUPPORTED */
		return;
	}

	/* PIC18: Extended address = 0x0020 (IDLOC: 0x200000) */
	pic_dumpaddr(PIC16_ID_LOW, 1);

	for (i = 0; i < 8; ++i)
		pic_dumpbyte(i, pic16_conf.idloc[i]);

	/* PIC18: Extended address = 0x0030 (CONFIG: 0x300000) */
	pic_dumpaddr(PIC16_CONFIG_LOW, 1);

	for (i = 0; i < pic16_map[pic16_index].configsize; ++i)
		pic_dumpbyte(i, pic16_conf.config[i]);
}
