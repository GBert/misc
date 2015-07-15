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

#ifndef _PIC_H
#define _PIC_H

/* ICSP DATA */
#define PIC_BYTLEN (256)
typedef struct {
        uint32_t address;     /* Address of data */
        uint16_t nbytes;      /* Number of bytes */
        uint8_t bytes[PIC_BYTLEN]; /* Data bytes */
} pic_data;

/* ISCP OPERATIONS */
struct pic_ops {
	uint32_t arch;	/* bit mask */
	uint16_t align;	/* hex input alignment */
	void (*selector)(void);
	void (*program_begin)(void);
	uint32_t (*program_data)(uint32_t, pic_data *);
	void (*program_end)(int);
	void (*verify_begin)(void);
	uint32_t (*verify_data)(uint32_t, pic_data *, uint32_t *);
	void (*verify_end)(void);
	void (*view_data)(pic_data *);
	int (*read_config_memory)(void);
	uint32_t (*get_program_size)(uint32_t *);
	uint32_t (*get_data_size)(uint32_t *);
	uint32_t (*get_executive_size)(uint32_t *);
	uint32_t (*get_boot_size)(uint32_t *);
	uint32_t (*read_program_memory_block)(uint32_t *, uint32_t, uint32_t);
	uint32_t (*read_data_memory_block)(uint16_t *, uint32_t, uint16_t);
	void (*write_panel)(uint32_t, uint32_t, uint32_t *, uint32_t);
	void (*bulk_erase)(void);
	uint32_t (*write_osccal)(uint16_t);
	uint32_t (*write_bandgap)(uint16_t);
	uint32_t (*write_calib)(uint16_t, uint16_t);
	void (*row_erase)(uint32_t, uint32_t);
	void (*dumpdeviceid)(void);
	void (*dumpconfig)(int);
	void (*dumposccal)(void);
	void (*dumpdevice)(void);
	uint32_t dumpadj;
	void (*dumphexcode)(uint32_t, uint32_t, uint32_t *);
	void (*dumpinhxcode)(uint32_t, uint32_t, uint32_t *);
	void (*dumphexdata)(uint32_t, uint32_t, uint16_t *);
	void (*dumpinhxdata)(uint32_t, uint32_t, uint16_t *);
};

/*
 * 32-bit keys
 */

/* ENHANCED MID-RANGE ICSP             = MCHP (0x4D434850)          */
#define MCHPKEY (0x4D434850)

/* PIC18/dsPIC33/PIC24 EXECUTIVE       = MCHP (0x4D434850) REVERSED */
#define PHCMKEY (0x0A12C2B2)

/* PIC24 ICSP                          = MCHQ (0x4D434851) REVERSED */
#define QHCMKEY (0x8A12C2B2)

/* dsPIC33F DS70152B-page 37 ICSP/STDP = ???? (0x5B982073) REVERSED */
#define P30FKEY (0xCE0419DA)

uint32_t pic_arch(const char *);

int pic_cmp(const void *, const void *);
#define PIC_NCOLS (4)
void pic_selector(void);

void pic_program_begin(void);
uint32_t pic_program_data(uint32_t, pic_data *);
void pic_program_end(int);

void pic_verify_begin(void);
uint32_t pic_verify_data(uint32_t, pic_data *, uint32_t *);
void pic_verify_end(void);

uint8_t *pic_pe_read_file(char *, uint32_t *);
int pic_pe_lookup(char *, const char *);

int pic_read_config(void);

uint32_t pic_get_program_size(uint32_t *);
uint32_t pic_get_data_size(uint32_t *);
uint32_t pic_get_executive_size(uint32_t *);
uint32_t pic_get_boot_size(uint32_t *);

uint32_t pic_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic_read_data_memory_block(uint16_t *, uint32_t, uint16_t);

#define PIC_VOID (0xFFFF)
void pic_program(char *, int);
uint32_t pic_verify(char *);
void pic_view(char *);

void pic_writebandgap(uint16_t);
void pic_writeosccal(uint16_t);
void pic_bulk_erase(void);
void pic_blank(int);

#define PIC_ERASE_ID (UINT32_MAX)
#define PIC_ERASE_CONFIG (PIC_ERASE_ID - 1)
#define PIC_ERASE_EEPROM (PIC_ERASE_ID - 2)
void pic_erase(uint32_t, uint32_t);

void pic_dumpdeviceid(void);

#define PIC_BRIEF   (0)
#define PIC_VERBOSE (1)
void pic_dumpconfig(void);
void pic_dumposccal(void);

void pic_dumpaddr(uint32_t, int);
void pic_dumpbyte(uint32_t, uint8_t);
void pic_dumpword16(uint32_t, uint16_t);
void pic_dumpword32(uint32_t, uint32_t);

void pic_dumpdevice(void);

void pic_dumpprogram(uint32_t, uint32_t);
void pic_dumpdata(void);
void pic_dumpexec(uint32_t, uint32_t);
void pic_dumpboot(uint32_t, uint32_t);

#define PIC_HEXDEC (1)
#define PIC_INHX32 (2)
#define PIC_CHAR(X) ((((X) >= ' ' && (X) < 127) ? (X) : ('.')))
void pic_dump_program(uint32_t, uint32_t, int);
int pic_mtcode(uint32_t, uint32_t, uint32_t *);
void pic_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic_dump_data(uint32_t, uint32_t, int);
int pic_mtdata(uint16_t, uint32_t, uint16_t *);
void pic_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic_dumpinhxdata(uint32_t, uint32_t, uint16_t *);

/* MEMORY REGIONS */
#define PIC_REGIONNOTSUP (0)	/* NOT BELOW      */
#define PIC_REGIONCODE   (1)	/* PROGRAM CODE   */
#define PIC_REGIONCONFIG (2)	/* CONFIG WORD(S) */
#define PIC_REGIONDATA   (3)	/* DATA EEPROM    */
#define PIC_REGIONID     (4)	/* USER ID        */
#define PIC_REGIONEXEC   (5)	/* EXECUTIVE      */
#define PIC_REGIONBOOT   (6)	/* BOOT SECTOR    */

/* PANEL WRITING */
#define PIC_PANEL_BEGIN  (1)
#define PIC_PANEL_UPDATE (2)
#define PIC_PANEL_END    (3)
#define PIC_TIMEOUT      (1) /* 1 second */
void pic_write_panel(int, uint32_t, uint32_t);

#endif /* !_PIC_H */
