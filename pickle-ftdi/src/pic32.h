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

#ifndef _PIC32_H
#define _PIC32_H

/******************************************************************************
 * 32-bit CONFIGURATION
 *****************************************************************************/

#define PIC32_MASK (0xFFFFFFFF)
#define PIC32_CONFIG_MAX (4)

struct pic32_config {
	uint32_t deviceid;
	uint32_t config[PIC32_CONFIG_MAX];	/* CONFIG WORDS              */
	uint32_t configaddr;			/* BOOT ADDR + SIZE * 4 - 16 */
	uint32_t status;			/* INITIAL DEVICE STATUS     */
	char pepath[STRLEN];			/* PE FILE PATH              */
};

struct pic32_dstab {
	uint32_t datasheet;		/* DEVICE APP. DATASHEET */
	char filename[STRLEN];		/* PE HEX FILE NAME      */
};

struct pic32_dsmap {
	char devicename[STRLEN];
	uint32_t deviceid;
	uint32_t datasheet;		/* DEVICE APP. DATASHEET       */
	uint32_t boot;			/* BOOT FLASH SIZE IN WORDS    */
	uint32_t prog;			/* PROGRAM FLASH SIZE IN WORDS */
	uint32_t row;			/* PROGRAM ROW SIZE IN WORDS   */
	uint32_t page;			/* PROGRAM PAGE SIZE IN WORDS  */
};

/******************************************************************************
 * MEMORY
 *****************************************************************************/

/*
 * DEBUG SEGMENT
 */
#define PIC32_DMSEG (0xFF200000)	/* 0xFF200000 .. 0xFF2XXXXX */

/*
 * KSEG1 0xA0000000 .. 0xBFFFFFFF (CACHED DISABLED)
 */
#define PIC32_RAM   (0xA0000000)	/* 0xA0000000 .. 0xAXXXXXXX RAM           */
#define PIC32_CODE  (0xBD000000)	/* 0xBD000000 .. 0xBDXXXXXX PROGRAM FLASH */
#define PIC32_BOOT  (0xBFC00000)	/* 0xBFC00000 .. 0xBFC02FFF BOOT FLASH    */

/*
 * KSEG1 PERIPHERALS
 */
#define PIC32_PERI   (0xBF800000)	/* 0xBF800000 .. 0xBF8FFFFF PERIPHERAL    */
#define PIC32_DEVID  (0xBF80F220)	/* DEVICE ID */
#define PIC32_IDMASK (0x0FFFFFFF)
/*
 * KSEG2 0x80000000 .. 0x9FFFFFFF (CACHE ENABLED)
 */

/******************************************************************************
 * JTAG/EJTAG/ICSP
 *****************************************************************************/

/* TMS HEADER */
#define PIC32_SELECT_DR			1,0x01	/* ENTER SELECT-DR-SCAN STATE   */
#define PIC32_CAPTURE_DR		2,0x01	/* ENTER CAPTURE-DR STATE       */
#define PIC32_SHIFT_DR			3,0x01	/* ENTER SHIFT-DR STATE         */

#define PIC32_SELECT_IR			2,0x03	/* ENTER SELECT-IR-SCAN STATE   */
#define PIC32_CAPTURE_IR		3,0x03	/* ENTER CAPTURE-IR STATE       */
#define PIC32_SHIFT_IR			4,0x03	/* ENTER SHIFT-IR STATE         */

/* TMS HEADER + FOOTER */
#define PIC32_RESET			5,0x1F	/* ENTER TEST-LOGIC-RESET STATE */
#define PIC32_IDLE6			6,0x1F	/* ENTER RUN-TEST/IDLE STATE    */

/* TMS FOOTER */
#define PIC32_UPDATE			1,0x01	/* ENTER UPDATE-DR/IR STATE     */
#define PIC32_IDLE2			2,0x01	/* ENTER RUN-TEST/IDLE STATE    */

#define PIC32_MTAP_COMMAND		5,0x07	/* SELECT MCHP COMMAND REGISTER */
#define PIC32_MTAP_SW_MTAP		5,0x04	/* SWITCH TO MCHP CONTROLLER    */
#define PIC32_MTAP_SW_ETAP		5,0x05	/* SWITCH TO EJTAG CONTROLLER   */
#define PIC32_MTAP_IDCODE		5,0x01	/* SELECT DEVICE ID REGISTER    */
#define PIC32_MCHP_STATUS		8,0x00	/* RETURN MCHP STATUS           */
#define PIC32_MCHP_ASSERT_RST		8,0xD1	/* PUT DEVICE IN RESET          */
#define PIC32_MCHP_DE_ASSERT_RST	8,0xD0	/* TAKE DEVICE OUT OF RESET     */
#define PIC32_MCHP_ERASE		8,0xFC	/* CHIP ERASE                   */
#define PIC32_MCHP_FLASH_ENABLE		8,0xFE	/* SET FLASH ENABLE BIT         */
#define PIC32_MCHP_FLASH_DISABLE	8,0xFD	/* CLEAR FLASH ENABLE BIT       */
#define PIC32_ETAP_ADDRESS		5,0x08	/* SELECT ADDRESS REGISTER      */
#define PIC32_ETAP_DATA			5,0x09	/* SELECT DATA REGISTER         */
#define PIC32_ETAP_CONTROL		5,0x0A	/* SELECT CONTROL REGISTER      */
#define PIC32_ETAP_EJTAGBOOT		5,0x0C	/* EXECUTE BOOT CODE            */
#define PIC32_ETAP_FASTDATA		5,0x0E	/* SELECT FASTDATA REGISTER     */

#define PIC32_MCHP_STATUS_CPS    (0x80) /* CODE PROTECT  */
#define PIC32_MCHP_STATUS_NVMERR (0x20) /* NVM ERROR     */
	/* NVMERR N/A PIC32MX320/340/360/420/440/460	 */
#define PIC32_MCHP_STATUS_CFGRDY (0x08) /* CONFIG READY  */
#define PIC32_MCHP_STATUS_FCBUSY (0x04) /* FLASH BUSY    */
#define PIC32_MCHP_STATUS_FAEN   (0x02) /* FLASH ENABLED */
	/* FAEN N/A ON PIC32MZ EC			 */
#define PIC32_MCHP_STATUS_DEVRST (0x01) /* DEVICE RESET  */

#define PIC32_EJTAG_CONTROL_ROCC     (0x80000000) /* PROCESSOR RESET  */
/* 30-29:PSZ     */
/* 23:VPED       */
/* 22:DOZE       */
/* 21:HALT       */
/* 20:PERRST     */
#define PIC32_EJTAG_CONTROL_PRNW     (0x00080000) /* PROCESSOR R/W    */
#define PIC32_EJTAG_CONTROL_PRACC    (0x00040000) /* PROCESSOR ACCESS */
/* 16:PRRST      */
#define PIC32_EJTAG_CONTROL_PROBEN   (0x00008000) /* PROBE ENABLED    */
#define PIC32_EJTAG_CONTROL_PROBTRAP (0x00004000) /* PROBE ADDRESS    */
/* 13:ISAONDEBUG */
#define PIC32_EJTAG_CONTROL_EJTAGBRK (0x00001000) /* DEBUG INTERRUPT  */
#define PIC32_EJTAG_CONTROL_DM       (0x00000008) /* DEBUG MODE       */

/* PIC32 PROGRAMMING EXECUTIVE */
#define PIC32_PE_READWORDS (0x00000100UL) /* PE BLOCK READ CACHE */
#define PIC32_PE_READMASK  (PIC32_PE_READWORDS - 1)
#define PIC32_PE_READBYTES (PIC32_PE_READWORDS << 2)
#define PIC32_PE_READPAGE  (0 - PIC32_PE_READBYTES)

/******************************************************************************
 * PICMicro devices
 *****************************************************************************/

/* 
 * App. Data-sheet
 *
 * PIC32MX3XX/4XX			DS61143H
 * PIC32MX5XX/6XX/7XX			DS61156H
 * PIC32MX1XX/2XX			DS60001168F
 * PIC32MX330/350/370/430/450/470	DS60001185C
 * PIC32MZ EC				DS60001191C
 */
#define DS61143H    (61143)
#define DS61156H    (61156)
#define DS60001168F (60001168)
#define DS60001185C (60001185)
#define DS60001191C (60001191)

/*
 * Programming Spec.
 *
 * DS60001145M
 */
#define DS60001145M (60001145)
#define PIC32MX110F016B (0x04A07053)
#define PIC32MX110F016C (0x04A09053)
#define PIC32MX110F016D (0x04A0B053)
#define PIC32MX120F032B (0x04A06053)
#define PIC32MX120F032C (0x04A08053)
#define PIC32MX120F032D (0x04A0A053)
#define PIC32MX130F064B (0x04D07053)
#define PIC32MX130F064C (0x04D09053)
#define PIC32MX130F064D (0x04D0B053)
#define PIC32MX150F128B (0x04D06053)
#define PIC32MX150F128C (0x04D08053)
#define PIC32MX150F128D (0x04D0A053)
#define PIC32MX210F016B (0x04A01053)
#define PIC32MX210F016C (0x04A03053)
#define PIC32MX210F016D (0x04A05053)
#define PIC32MX220F032B (0x04A00053)
#define PIC32MX220F032C (0x04A02053)
#define PIC32MX220F032D (0x04A04053)
#define PIC32MX230F064B (0x04D01053)
#define PIC32MX230F064C (0x04D03053)
#define PIC32MX230F064D (0x04D05053)
#define PIC32MX250F128B (0x04D00053)
#define PIC32MX250F128C (0x04D02053)
#define PIC32MX250F128D (0x04D04053)

#define PIC32MX330F064H (0x05600053)
#define PIC32MX330F064L (0x05601053)
#define PIC32MX430F064H (0x05602053)
#define PIC32MX430F064L (0x05603053)
#define PIC32MX350F128H (0x0570C053)
#define PIC32MX350F128L (0x0570D053)
#define PIC32MX450F128H (0x0570E053)
#define PIC32MX450F128L (0x0570F053)
#define PIC32MX350F256H (0x05704053)
#define PIC32MX350F256L (0x05705053)
#define PIC32MX450F256H (0x05706053)
#define PIC32MX450F256L (0x05707053)
#define PIC32MX370F512H (0x05808053)
#define PIC32MX370F512L (0x05809053)
#define PIC32MX470F512H (0x0580A053)
#define PIC32MX470F512L (0x0580B053)

#define PIC32MX360F512L (0x00938053)
#define PIC32MX360F256L (0x00934053)
#define PIC32MX340F128L (0x0092D053)
#define PIC32MX320F128L (0x0092A053)
#define PIC32MX340F512H (0x00916053)
#define PIC32MX340F256H (0x00912053)
#define PIC32MX340F128H (0x0090D053)
#define PIC32MX320F128H (0x0090A053)
#define PIC32MX320F064H (0x00906053)
#define PIC32MX320F032H (0x00902053)
#define PIC32MX460F512L (0x00978053)
#define PIC32MX460F256L (0x00974053)
#define PIC32MX440F128L (0x0096D053)
#define PIC32MX440F256H (0x00952053)
#define PIC32MX440F512H (0x00956053)
#define PIC32MX440F128H (0x0094D053)
#define PIC32MX420F032H (0x00942053)

#define PIC32MX534F064H (0x04400053)
#define PIC32MX534F064L (0x0440C053)
#define PIC32MX564F064H (0x04401053)
#define PIC32MX564F064L (0x0440D053)
#define PIC32MX564F128H (0x04403053)
#define PIC32MX564F128L (0x0440F053)
#define PIC32MX575F256H (0x04317053)
#define PIC32MX575F256L (0x04333053)
#define PIC32MX575F512H (0x04309053)
#define PIC32MX575F512L (0x0430F053)
#define PIC32MX664F064H (0x04405053)
#define PIC32MX664F064L (0x04411053)
#define PIC32MX664F128H (0x04407053)
#define PIC32MX664F128L (0x04413053)
#define PIC32MX675F256H (0x0430B053)
#define PIC32MX675F256L (0x04305053)
#define PIC32MX675F512H (0x0430C053)
#define PIC32MX675F512L (0x04311053)
#define PIC32MX695F512H (0x04325053)
#define PIC32MX695F512L (0x04341053)
#define PIC32MX764F128H (0x0440B053)
#define PIC32MX764F128L (0x04417053)
#define PIC32MX775F256H (0x04303053)
#define PIC32MX775F256L (0x04312053)
#define PIC32MX775F512H (0x0430D053)
#define PIC32MX775F512L (0x04306053)
#define PIC32MX795F512H (0x0430E053)
#define PIC32MX795F512L (0x04307053)

#define PIC32MZ1024ECG064 (0x05103053)
#define PIC32MZ1024ECG100 (0x0510D053)
#define PIC32MZ1024ECG124 (0x05117053)
#define PIC32MZ1024ECG144 (0x05121053)
#define PIC32MZ1024ECH064 (0x05108053)
#define PIC32MZ1024ECH100 (0x05112053)
#define PIC32MZ1024ECH124 (0x0511C053)
#define PIC32MZ1024ECH144 (0x05126053)
#define PIC32MZ2048ECG064 (0x05104053)
#define PIC32MZ2048ECG100 (0x0510E053)
#define PIC32MZ2048ECG124 (0x05118053)
#define PIC32MZ2048ECG144 (0x05122053)
#define PIC32MZ2048ECH064 (0x05109053)
#define PIC32MZ2048ECH100 (0x05113053)
#define PIC32MZ2048ECH124 (0x0511D053)
#define PIC32MZ2048ECH144 (0x05127053)
#define PIC32MZ1024ECM064 (0x05130053)
#define PIC32MZ2048ECM064 (0x05131053)
#define PIC32MZ1024ECM100 (0x0513A053)
#define PIC32MZ2048ECM100 (0x0513B053)
#define PIC32MZ1024ECM124 (0x05144053)
#define PIC32MZ2048ECM124 (0x05145053)
#define PIC32MZ1024ECM144 (0x0514E053)
#define PIC32MZ2048ECM144 (0x0514F053)

/*
 * Programming Spec.
 *
 * DS60001145N
 */
#define PIC32MX170F256B (0x06610053)
#define PIC32MX170F256D (0x0661A053)
#define PIC32MX270F256B (0x06600053)
#define PIC32MX270F256D (0x0660A053)

/******************************************************************************/

uint32_t pic32_arch(void);
void pic32_selector(void);
void pic32_program_verify(void);
void pic32_standby(void);
void pic32_setmode(uint8_t, uint32_t);
int32_t pic32_read_config_memory(void);
uint32_t pic32_get_program_size(uint32_t *);
uint32_t pic32_get_boot_size(uint32_t *);
uint32_t pic32_read_program_memory_block(uint32_t *, uint32_t, uint32_t);
void pic32_bulk_erase(void);
void pic32_write_panel(uint32_t, uint32_t, uint32_t *, uint32_t);
uint32_t pic32_getregion(uint32_t);
void pic32_program_begin(void);
uint32_t pic32_program_data(uint32_t, pic_data *);
void pic32_program_end(int);
uint32_t pic32_verify_data(uint32_t, pic_data *, uint32_t *);
void pic32_view_data(pic_data *);
void pic32_dumpdeviceid(void);
void pic32_dumpconfig(int);
void pic32_dumphexcode(uint32_t, uint32_t, uint32_t *);
void pic32_dumpinhxcode(uint32_t, uint32_t, uint32_t *);

#endif
