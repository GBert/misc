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

#ifndef _PIC16_H
#define _PIC16_H

/******************************************************************************
 * 16-bit CONFIGURATION
 *****************************************************************************/

#define PIC16_MASK (0xFFFF)

struct pic16_config {
	uint16_t deviceid;	/* 0x3ffffe .. 0x3fffff */
	uint8_t idloc[8];	/* 0x200000 .. 0x200007 */
	uint8_t config[16];	/* 0x300000 .. 0x30000f */
};

struct pic16_dsmap {
	char devicename[STRLEN];
	uint16_t deviceid;
	uint32_t flash;		/* flash size in words                    */
	uint16_t eeprom;	/* eeprom size in bytes 0=unsupported     */
	uint32_t datasheet;
	uint8_t devidrev4;	/* DEVICE REVISION 4 used                 */
	uint8_t panelsize;	/* PROGRAM PANEL SIZE                     */
	uint32_t p9, p9a;	/* Program delay times P9 and P9A in us   */
	uint32_t p10, p11;	/* Erase delay times P10 and P11 in us    */
	uint32_t configaddr;	/* CONFIG ADDRESS                         */
	uint8_t configsize;	/* CONFIG SIZE                            */
	uint16_t erasesize;	/* ERASE BLOCK SIZE 64 or 1024 for PIC18J */
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

#define PIC16_WORD(X) ((X) * 512) /* KB to words */

/*
 * MEMORY MAP
 */
#if 0
#define PIC16_CODE_LOW    (0x000000)	/* 128KB MAX  */
#endif
#define PIC16_CODE_HIGH   (0x1fffff)
#define PIC16_ID_LOW      (0x200000)	/* not PIC18J */
#define PIC16_ID_HIGH     (0x200007)
#define PIC16_CONFIG_LOW  (0x300000)	/* not PIC18J */
#define PIC16_CONFIG_HIGH (0x30000d)
#define PIC16_CONFIG_MASK (0x00000f)
#define PIC16_DATA_LOW    (0xf00000)	/* pseudo address */
#define PIC16_DATA_HIGH   (0xffffff)
#define PIC16_DATA_MASK   (0x0fffff)

/*
 * DEVICE ID / REVISION
 */
#define PIC16_DEVICEID_LOW	(0x3ffffe)	/*  DEV2 DEV1 DEV0 REV4 DEV3 REV2 REV1 REV0 */
#define PIC16_DEVICEID_HIGH	(0x3fffff)	/* DEV10 DEV9 DEV8 DEV7 DEV6 DEV5 DEV4 DEV3 */
#define PIC16_DEVICEID_IDMASK	(0xFFE0)	/* REV4 not used */
#define PIC16_DEVICEID_REVMASK	(0x001F)	/* REV4 not used */
#define PIC16_DEVICEID_SHIFT    (5)		/* REV4 not used */
#define PIC16_DEVICEID_IDMASK_REV4  (0xFFF0)	/* REV4 in use   */
#define PIC16_DEVICEID_REVMASK_REV4 (0x000F)	/* REV4 in use   */
#define PIC16_DEVICEID_SHIFT_REV4   (4)		/* REV4 in use   */

/*
 * CONFIG
 */
#define PIC16_CONFIG_1L (0)
#define PIC16_CONFIG_1H (1)
#define PIC16_CONFIG_2L (2)
#define PIC16_CONFIG_2H (3)
#define PIC16_CONFIG_3L (4)
#define PIC16_CONFIG_3H (5)
#define PIC16_CONFIG_4L (6)
#define PIC16_CONFIG_4H (7)
#define PIC16_CONFIG_5L (8)
#define PIC16_CONFIG_5H (9)
#define PIC16_CONFIG_6L (10)
#define PIC16_CONFIG_6H (11)
#define PIC16_CONFIG_7L (12)
#define PIC16_CONFIG_7H (13)
#define PIC16_CONFIG_8L (14)
#define PIC16_CONFIG_8H (15)

/*
 * BLOCKS
 */
#define PIC16_BOOTBLOCK_LOW  (0x000000)
#define PIC16_BOOTBLOCK_HIGH (0x0001FF)
#define PIC16_BLOCK0_LOW     (0x000200)
#define PIC16_BLOCK0_HIGH    (0x001FFF)
#define PIC16_BLOCK1_LOW     (0x002000)
#define PIC16_BLOCK1_HIGH    (0x003FFF)
#define PIC16_BLOCK2_LOW     (0x004000)
#define PIC16_BLOCK2_HIGH    (0x005FFF)
#define PIC16_BLOCK3_LOW     (0x006000)
#define PIC16_BLOCK3_HIGH    (0x007FFF)

/*
 * PROGRAM
 */
#define PIC16_PANEL_BEGIN   (1)
#define PIC16_PANEL_UPDATE  (2)
#define PIC16_PANEL_END     (3)
#define PIC16_ID_PANEL_SIZE (8)

/******************************************************************************
 * PICMicro devices
 *****************************************************************************/

/*
 * DS39576B (MATURE PRODUCT 2013-11-12)
 */
#define DS39576B (39576)
#define PIC18F242 (0x0480) /* Device id shared with PIC18F2439 */
#define PIC18F248 (0x0800)
#define PIC18F252 (0x0400) /* Device id shared with PIC18F2539 */
#define PIC18F258 (0x0840)
#define PIC18F442 (0x04a0) /* Device id shared with PIC18F4439 */
#define PIC18F448 (0x0820)
#define PIC18F452 (0x0420) /* Device id shared with PIC18F4539 */
#define PIC18F458 (0x0860)

/*
 * DS30480C (MATURE PRODUCT 2013-11-30)
 *
 * THESE DEVICES SHARE THEIR DEVID WITH THE DEVICES LISTED ABOVE.
 *
 * THE MAIN DIFFERENCE BETWEEN THESE AND THEIR SHARED DEVICES
 * IS THE FLASH SIZE, FOR EXAMPLE THE PIC18F252 HAS 32KB AND
 * THE PIC18F2539 HAS 24KB, HOWEVER, THE SAMPLE PIC18LF2539
 * DEVICE HAS 32KB FLASH AND THE 8KB WHICH WAS UTILISED BY THE
 * MOTOR CONTROL FIRMWARE AT 0x6000 IS COMPLETELY RE-USABLE.
 *
 * THESE DEVICES ARE NOW OBSOLETE ALONG WITH THOSE ABOVE.
 */
#define DS30480C (30480)
#define PIC18F2439  (0x0480) /* Device id shared with PIC18F242 */
#define PIC18F2539  (0x0400) /* Device id shared with PIC18F252 */
#define PIC18F4439  (0x04a0) /* Device id shared with PIC18F442 */
#define PIC18F4539  (0x0420) /* Device id shared with PIC18F452 */

/*
 * DS39592E == DS39576B
 */
#define DS39592E (39592)
#define PIC18F1220 (0x07e0)
#define PIC18F1320 (0x07c0)
#define PIC18F2220 (0x0580)
#define PIC18F2320 (0x0500)
#define PIC18F4220 (0x05a0)
#define PIC18F4320 (0x0520)

/*
 * DS30500A == DS39567B
 */
#define DS30500A (30500)
#define PIC18F2331 (0x08e0)
#define PIC18F2431 (0x08c0)
#define PIC18F4331 (0x08a0)
#define PIC18F4431 (0x0880)

/*
 * DS39622J/K != DS39576B
 *  1. NEW BULK ERASE
 *  2. VARIOUS PANEL SIZES
 *  3. EEPROM WRITE OMITS EECON2 0x55/0xaa WRITE
 *  4. EEPROM TABLAT SHIFT NEEDS PRE NOP
 *  5. SUPPORTS ONLY SINGLE PANEL WRITES
 */
#define DS39622K (39622)
#define PIC18F2221 (0x2160)
#define PIC18F2321 (0x2120)
#define PIC18F2410 (0x1160)
#define PIC18F2420 (0x1140)    /* DEVID1 REV4=0 */
#define PIC18F2423 (0x1150)    /* DEVID1 REV4=1 */
#define PIC18F2450 (0x2420)
#define PIC18F2455 (0x1260)
#define PIC18F2458 (0x2a60)
#define PIC18F2480 (0x1ae0)
#define PIC18F2510 (0x1120)
#define PIC18F2515 (0x0ce0)
#define PIC18F2520 (0x1100)    /* DEVID1 REV4=0 */
#define PIC18F2523 (0x1110)    /* DEVID1 REV4=1 */
#define PIC18F2525 (0x0cc0)
#define PIC18F2550 (0x1240)
#define PIC18F2553 (0x2a40)
#define PIC18F2580 (0x1ac0)
#define PIC18F2585 (0x0ee0)
#define PIC18F2610 (0x0ca0)
#define PIC18F2620 (0x0c80)
#define PIC18F2680 (0x0ec0)
#define PIC18F2682 (0x2700)
#define PIC18F2685 (0x2720)
#define PIC18F4221 (0x2140)
#define PIC18F4321 (0x2100)
#define PIC18F4410 (0x10e0)
#define PIC18F4420 (0x10c0)    /* DEVID1 REV4=0 */
#define PIC18F4423 (0x10d0)    /* DEVID1 REV4=1 */
#define PIC18F4450 (0x2400)
#define PIC18F4455 (0x1220)
#define PIC18F4458 (0x2a20)
#define PIC18F4480 (0x1aa0)
#define PIC18F4510 (0x10a0)
#define PIC18F4515 (0x0c60)
#define PIC18F4520 (0x1080)    /* DEVID1 REV4=0 */
#define PIC18F4523 (0x1090)    /* DEVID1 REV4=1 */
#define PIC18F4525 (0x0c40)
#define PIC18F4550 (0x1200)
#define PIC18F4553 (0x2a00)
#define PIC18F4580 (0x1a80)
#define PIC18F4585 (0x0ea0)
#define PIC18F4610 (0x0c20)
#define PIC18F4620 (0x0c00)
#define PIC18F4680 (0x0e80)
#define PIC18F4682 (0x2740)
#define PIC18F4685 (0x2760)

/*
 * DS39752A ~= DS39622J (new chip erase)
 */
#define DS39752B (39752)
#define PIC18F1230 (0x1e00)
#define PIC18F1330 (0x1e20)
#define PIC18F1330ICD (0x1fe0)

/*
 * DS41398B
 */
#define DS41398B (41398)
#define PIC18F45K22  (0x5500)
#define PIC18LF45K22 (0x5520)
#define PIC18F25K22  (0x5540)
#define PIC18LF25K22 (0x5560)
#define PIC18F23K22  (0x5740)
#define PIC18LF23K22 (0x5760)
#define PIC18F24K22  (0x5640)
#define PIC18LF24K22 (0x5660)
#define PIC18F26K22  (0x5440)
#define PIC18LF26K22 (0x5460)
#define PIC18F43K22  (0x5700)
#define PIC18LF43K22 (0x5720)
#define PIC18F44K22  (0x5600)
#define PIC18LF44K22 (0x5620)
#define PIC18F46K22  (0x5400)
#define PIC18LF46K22 (0x5420)

/*
 * DS39972B
 */
#define DS39972B (39972)
#define PIC18F66K80  (0x60e0)
#define PIC18F46K80  (0x6100)
#define PIC18F26K80  (0x6120)
#define PIC18F65K80  (0x6140)
#define PIC18F45K80  (0x6160)
#define PIC18F25K80  (0x6180)
#define PIC18LF66K80 (0x61c0)
#define PIC18LF46K80 (0x61e0)
#define PIC18LF26K80 (0x6200)
#define PIC18LF65K80 (0x6220)
#define PIC18LF45K80 (0x6240)
#define PIC18LF25K80 (0x6260)

/*
 * DS41630B
 */
#define DS41630B (41630)
#define PIC18F45K50  (0x5c00)
#define PIC18LF45K50 (0x5c80)
#define PIC18F25K50  (0x5c20)
#define PIC18LF25K50 (0x5ca0)
#define PIC18F24K50  (0x5c60)
#define PIC18LF24K50 (0x5ce0)
#define PIC18F26K50  (0x5d20)
#define PIC18LF26K50 (0x5d60)
#define PIC18F46K50  (0x5d00)
#define PIC18LF46K50 (0x5d40)

/*
 * DS41342E
 */
#define DS41342E (41342)
#define PIC18LF13K50 (0x4700)
#define PIC18LF14K50 (0x4720)
#define PIC18F13K50  (0x4740)
#define PIC18F14K50  (0x4760)

/*
 * DS39687E
 */
#define DS39687E (39687)
#define PIC18F24J10  (0x1D00)
#define PIC18F25J10  (0x1C00)
#define PIC18F44J10  (0x1D20)
#define PIC18F45J10  (0x1C20)
#define PIC18F25J11  (0x4DA0)
#define PIC18F24J11  (0x4D80)
#define PIC18F26J11  (0x4DC0)
#define PIC18F45J11  (0x4E00)
#define PIC18F44J11  (0x4DE0)
#define PIC18F46J11  (0x4E20)
#define PIC18F24J50  (0x4C00)
#define PIC18F25J50  (0x4C20)
#define PIC18F26J50  (0x4C40)
#define PIC18F44J50  (0x4C60)
#define PIC18F45J50  (0x4C80)
#define PIC18F46J50  (0x4CA0)
#define PIC18F26J13  (0x5920)
#define PIC18F27J13  (0x5960)
#define PIC18F46J13  (0x59A0)
#define PIC18F47J13  (0x59E0)
#define PIC18F26J53  (0x5820)
#define PIC18F27J53  (0x5860)
#define PIC18F46J53  (0x58A0)
#define PIC18F47J53  (0x58E0)
#define PIC18LF24J10 (0x1D40)
#define PIC18LF25J10 (0x1C40)
#define PIC18LF44J10 (0x1D60)
#define PIC18LF45J10 (0x1C60)
#define PIC18LF24J50 (0x4CC0)
#define PIC18LF25J50 (0x4CE0)
#define PIC18LF26J50 (0x4D00)
#define PIC18LF44J50 (0x4D20)
#define PIC18LF45J50 (0x4D40)
#define PIC18LF46J50 (0x4D60)
#define PIC18LF24J11 (0x4E40)
#define PIC18LF25J11 (0x4E60)
#define PIC18LF26J11 (0x4E80)
#define PIC18LF44J11 (0x4EA0)
#define PIC18LF45J11 (0x4EC0)
#define PIC18LF46J11 (0x4EE0)
#define PIC18LF26J13 (0x5B20)
#define PIC18LF27J13 (0x5B60)
#define PIC18LF46J13 (0x5BA0)
#define PIC18LF47J13 (0x5BE0)
#define PIC18LF26J53 (0x5A20)
#define PIC18LF27J53 (0x5A60)
#define PIC18LF46J53 (0x5AA0)
#define PIC18LF47J53 (0x5AE0)

/*
 * DS41357B
 */
#define DS41357B (41357)
#define PIC18LF13K22 (0x4F80)
#define PIC18LF14K22 (0x4F60)
#define PIC18F13K22  (0x4F40)
#define PIC18F14K22  (0x4F20)

/*
 * DS41297F
 */
#define DS41297F (41297)
#define PIC18F23K20 (0x20E0)
#define PIC18F24K20 (0x20A0)
#define PIC18F25K20 (0x2060)
#define PIC18F26K20 (0x2020)
#define PIC18F43K20 (0x20C0)
#define PIC18F44K20 (0x2080)
#define PIC18F45K20 (0x2040)
#define PIC18F46K20 (0x2000)

/******************************************************************************/

uint32_t pic16_arch(void);
void pic16_selector(void);
void pic16_program_verify(void);
void pic16_standby(void);
void pic16_init_code_memory_access(void);
void pic16_init_config_memory_access(void);
void pic16_init_data_memory_access(void);
void pic16_write_enable(void);
void pic16_write_disable(void);
void pic16_free(void);
void pic16_write(void);
void pic16_set_table_pointer(uint32_t);
void pic16_erase_block(uint32_t);
void pic16_erase_row(uint32_t, uint32_t);
void pic16_bulk_erase(void);
void pic16_row_erase(uint32_t, uint32_t);
int pic16_read_config_memory(void);
uint32_t pic16_get_program_size(uint32_t *);
uint32_t pic16_get_data_size(uint32_t *);
uint32_t pic16_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
uint32_t pic16_read_data_memory_block(uint16_t *, uint32_t, uint16_t);
void pic16_set_data_pointer(uint16_t);
uint8_t pic16_read_data_memory(void);
void pic16_write_data_memory(uint8_t);
void pic16_write_panel(uint32_t, uint32_t, uint32_t *, uint32_t);
void pic16_write_panel_init(void);
void pic16_goto100000(void);
void pic16_write_configreg(uint8_t, uint32_t, uint32_t);
uint32_t pic16_write_config(void);
uint32_t pic16_getregion(uint32_t);
void pic16_program_begin(void);
uint32_t pic16_program_data(uint32_t, pic_data *);
void pic16_program_end(int);
uint32_t pic16_verify_data(uint32_t, pic_data *, uint32_t *);
void pic16_view_data(pic_data *);
void pic16_dumpdeviceid(void);
void pic16_dumpconfig(int);
void pic16_dumpconfig_verbose(void);
void pic16_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic16_dumpinhxcode(uint32_t, uint32_t, uint32_t *);
void pic16_dumphexdata(uint32_t, uint32_t, uint16_t *);
void pic16_dumpinhxdata(uint32_t, uint32_t, uint16_t *);
void pic16_dumpdevice(void);

#endif /* !_PIC16_H */
