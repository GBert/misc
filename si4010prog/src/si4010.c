/*
 * si4010.c - Silicon Labs SI4010 C2 Debug and Control Functions
 *
 * This is mostly a reordering of the c2mon.c code with some additions/changes
 * for the SI4010 microcontroller.
 *
 * The original c2mon code can be found at:
 * http://www.ise.pw.edu.pl/~wzab/cygnal/
 *
 * Copyright 2005 Matt Otto <matt@klox.net> - Original c2mon.c
 * Copyright 2008 David Imhoff <dimhoff.devel@gmail.com> - SI4010 extensions
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.  It is distributed without any
 * warranty, even the implied warranty of merchantability or fitness for a
 * particular purpose.  See http://www.gnu.org/licenses/gpl.txt for the
 * complete license.
 */

#define _POSIX_C_SOURCE 200809L
#include "si4010.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/types.h>
#include <time.h>
#include <assert.h>
#include "c2_bus.h"

// Mostly undocumented registers useful for flashing & debugging
// To access 0x00-0x02, I've only used C2_ReadReg & C2_WriteReg.
// To access 0x20-0x33, you must used C2_ReadRegBlock & C2_WriteRegBlock.
// To access 0x80-0xFF, you can use either method.
#define DEVICEID 0x00   // fixed device ID value (should match DevIDList)
#define REVID    0x01   // fixed revision ID (I've only seen 0x00 on an F331)
#define FPCTL    0x02   // Flash Programming Control (also for debugging)

#define PCL_COPY 0x20   // copy of Program Counter, low byte, while debugging
#define PCH_COPY 0x21   // copy of Program Counter, high byte, while debugging
#define PSW_COPY 0x23   // copy of PSW while debugging
#define R0_COPY  0x24   // copy of R0 (bank 0) while debugging
#define R1_COPY  0x25   // copy of R1 (bank 0) while debugging
#define R2_COPY  0x26   // copy of R2 (bank 0) while debugging
#define DPTRL_COPY 0x27	// Copy of DPTR, low byte
#define DPTRH_COPY 0x28	// Copy of DPTR, high byte

#define FPDAT    0xB4   // Flash Programming Data (also for debugging)
#define PSW      0xD0   // Program Status Word (this is a documented register)
#define BP_EN  0xF1   // Breakpoint Breakpoint bitmask
//#define SFR_UNK  0xF2   // function unknown but queried by ide
#define BP_SEL 0xF3   // Breakpoint Breakpoint selection
#define BPL    0xF4   // Breakpoint address low byte
#define BPH    0xF5   // Breakpoint address high byte

// FPCTL codes
#define INIT_KEY1   0x02        // first key
#define INIT_KEY2   0x04        // second key
#define INIT_KEY3   0x01        // third key
#define RESUME_EXEC 0x08        // resume code execution

// FPDAT commands
#define UNKNOWN1     0x01
#define UNKNOWN2     0x02
#define DEVICE_ERASE 0x03 // TODO: check on SI4010
#define FLASH_READ   0x06 // TODO: check on SI4010
#define FLASH_WRITE  0x07 // TODO: check on SI4010
#define PAGE_ERASE   0x08 // TODO: check on SI4010
#define REG_READ     0x09
#define REG_WRITE    0x0A
#define RAM_READ     0x0B
#define RAM_WRITE    0x0C
#define UNKNOWN3     0x0D
#define XRAM_READ    0x0E
#define XRAM_WRITE   0x0F

// FPDAT status codes
#define INVALID_COMMAND 0x00
#define COMMAND_FAILED  0x02
#define COMMAND_OK      0x0D

// FPI Command Status (C2_ReadAddr) bit masks
#define FLASHING       0x80   // this isn't on continuously during Device Erase
#define BREAKPOINT_HIT 0x04   // after stopping due to a breakpoint, this stays on until the next RESUME_EXEC
#define INBUSY         0x02
#define OUTREADY       0x01

// The worst-case wait should be during a Flash Device Erase, which can take
// up to 20ms per page.  For 16kB (which is the largest C2-based MCU as of
// this writing), that's about 640ms.  Since the WaitFor* routines get
// called often, I use the time() function because it's simple.  The shortest
// timeout using time() is 1-2 seconds, which is good enough.
#define POLL_TIMEOUT 2    // 2 = at most 2 seconds, and as little as 1 second

static struct c2_bus *c2_bus_handle = NULL;  // handle to the c2 bus driver
//TODO: use some kind of state object instead of global...

//TODO: make things static?
//*************** bus access functions **************
static inline uint8_t C2_ReadData(void)
{
  unsigned char tmp;
  c2_bus_data_read(c2_bus_handle, &tmp, 1);
  return (uint8_t)tmp;
}

static inline void C2_WriteData(uint8_t dat)
{
  c2_bus_data_write(c2_bus_handle, &dat, 1);
}

static inline uint8_t C2_ReadAddr(void)
{
  unsigned char tmp;
  c2_bus_addr_read(c2_bus_handle, &tmp);
  return (uint8_t)tmp;
}

static inline void C2_WriteAddr(uint8_t addr)
{
  c2_bus_addr_write(c2_bus_handle, addr);
}

static inline uint8_t C2_ReadReg(uint8_t reg)
{
  C2_WriteAddr(reg);
  return C2_ReadData();
}

static inline void C2_WriteReg(uint8_t reg, uint8_t val)
{
  C2_WriteAddr(reg);
  C2_WriteData(val);
}

static inline void WaitForOutReady(void)
{
  time_t limit = time(NULL) + POLL_TIMEOUT;

  while (!(C2_ReadAddr() & OUTREADY)) {
    assert(time(NULL) < limit);
  }
}

static inline void WaitForInReady(void)
{
  // "InReady" means the InBusy bit is NOT set

  time_t limit = time(NULL) + POLL_TIMEOUT;

  while (C2_ReadAddr() & INBUSY) {
    assert(time(NULL) < limit);
  }
}

//*********************************************************************

int si4010_init(struct c2_bus *bus)
{
	c2_bus_handle = bus;
	return 0;
}

uint16_t c2_get_chip_version()
{
	uint16_t retval;
	retval = C2_ReadReg(DEVICEID);
	retval = (retval << 8) | C2_ReadReg(REVID);
	return retval;
}

/*
int si4010_reset()
{
	return c2_bus_reset(c2_bus_handle); // reset target device
}
*/

int si4010_halt()
{
	return c2_bus_qreset(c2_bus_handle);
}

int si4010_resume()
{
	C2_WriteReg(FPCTL, RESUME_EXEC);
	return 0;
}

int si4010_bp_set(uint8_t bpid, uint16_t addr)
{
	int err;
	uint8_t buf[2];

	if (bpid > 7) {
		return -1;
	}

	buf[0] = bpid;
	err = si4010_sfr_write(BP_SEL, 1, buf);
	if (err != 0) {
		return err;
	}

	buf[0] = addr & 0xff;
	buf[1] = (addr >> 8) & 0xff;
	err = si4010_sfr_write(BPL, 2, buf);
	if (err != 0) {
		return err;
	}

	err = si4010_sfr_read(BP_EN, 1, buf);
	if (err != 0) {
		return err;
	}
	buf[0] |= (1 << bpid);
	return si4010_sfr_write(BP_EN, 1, buf);
}

int si4010_bp_clear(uint8_t bpid)
{
	int err;
	uint8_t bp_en;

	if (bpid > 7) {
		return -1;
	}

	err = si4010_sfr_read(BP_EN, 1, &bp_en);
	if (err != 0) {
		return err;
	}
	bp_en &= ~(1 << bpid);
	return si4010_sfr_write(BP_EN, 1, &bp_en);

}

int si4010_bp_clear_all()
{
	uint8_t bp_en;
	bp_en = 0;
	return si4010_sfr_write(BP_EN, 1, &bp_en);
}

/*
int si4010_bp_wait()
{
}

inline int C2_CheckForBreakpoint(void)
{
//TODO:
	//  return ioctl(driver_handle, C2_IOCIRQCHK, NULL);
	return 0;
}

inline void C2_WaitForBreakpoint(void)
{
	// Keep checking for an interrupt from the MCU,
	// which indicates that the breakpoint was hit.
	while (C2_CheckForBreakpoint() == 0);
}
*/

int si4010_pc_get(uint16_t *pc)
{
	uint8_t buf[2];
	int err;

	err = si4010_sfr_read(PCL_COPY, 2, buf);
	if (err != 0) {
		return err;
	}

	*pc = (buf[1] << 8) | buf[0];

	return 0;
}

int si4010_pc_set(uint16_t pc)
{
	uint8_t buf[2];

	buf[0] = pc & 0xff;
	buf[1] = (pc >> 8) & 0xff;

	return si4010_sfr_write(PCL_COPY, 2, buf);
}

int si4010_sfr_read(uint8_t addr, uint8_t len, void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;

	// Reads a block of registers starting at BlockStart.
	// Returns 0 if successful, 1 otherwise

	C2_WriteReg(FPDAT, REG_READ);    // load the REG_READ command into FPDAT
	WaitForInReady();                // Wait for input acknowledge

	// Check status before starting register access sequence
	WaitForOutReady();
	if (C2_ReadData() != COMMAND_OK) {
		return -1;
	}

	C2_WriteData(addr); WaitForInReady();
	C2_WriteData(len); WaitForInReady();

	do {
		WaitForOutReady();
		*bbuf++ = C2_ReadData();
	} while (--len);

	return 0;
}

int si4010_sfr_write(uint8_t addr, uint8_t len, const void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;

	C2_WriteReg(FPDAT, REG_WRITE);
	WaitForInReady();

	// Check status before starting register access sequence
	WaitForOutReady();
	if (C2_ReadData() != COMMAND_OK) {
		return -1;
	}

	C2_WriteData(addr); WaitForInReady();
	C2_WriteData(len); WaitForInReady();

	do {
		C2_WriteData(*bbuf++);
		WaitForInReady();
	} while (--len);

	return 0;
}

int si4010_ram_read(uint8_t addr, uint8_t len, void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;

	C2_WriteReg(FPDAT, RAM_READ);
	WaitForInReady();

	// Check status before starting RAM access sequence
	WaitForOutReady();
	if (C2_ReadData() != COMMAND_OK) {
		return -1;
	}

	C2_WriteData(addr); WaitForInReady();
	C2_WriteData(len); WaitForInReady();

	do {
		WaitForOutReady();
		*bbuf++ = C2_ReadData();
	} while (--len);

	return 0;
}

int si4010_ram_write(uint8_t addr, uint8_t len, const void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;

	C2_WriteReg(FPDAT, RAM_WRITE);
	WaitForInReady();

	// Check status before starting RAM access sequence
	WaitForOutReady();
	if (C2_ReadData() != COMMAND_OK) {
		return -1;
	}

	C2_WriteData(addr); WaitForInReady();
	C2_WriteData(len); WaitForInReady();

	do {
		C2_WriteData(*bbuf++);
		WaitForInReady();
	} while (--len);

	return 0;
}

int si4010_xram_read(uint16_t addr, size_t len, void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;
	size_t blen;
	int i;

	while (len != 0) {
		blen = (len > 0x100) ?  0x100 : len;

		C2_WriteReg(FPDAT, XRAM_READ);
		WaitForInReady();

		// Check status before starting Flash access sequence
		WaitForOutReady();
		if (C2_ReadData() != COMMAND_OK) {
			return -1;
		}

		C2_WriteData(addr >> 8); WaitForInReady();
		C2_WriteData(addr & 0xFF); WaitForInReady();
		C2_WriteData(blen); WaitForInReady();

		// Read data
		for (i=0; i < blen; i++) {
			WaitForOutReady();
			*bbuf++ = C2_ReadData();
		}

		addr += blen;
		len -= blen;
	}

	return 0;
}

int si4010_xram_write(uint16_t addr, uint8_t len, const void *buf)
{
	uint8_t *bbuf = (uint8_t *) buf;
	size_t blen;
	int i;

	while (len != 0) {
		blen = (len > 0x100) ?  0x100 : len;

		C2_WriteReg(FPDAT, XRAM_WRITE);
		WaitForInReady();

		// Check status before starting Flash access sequence
		WaitForOutReady();
		if (C2_ReadData() != COMMAND_OK) {
			return -1;
		}

		C2_WriteData(addr >> 8); WaitForInReady();
		C2_WriteData(addr & 0x00FF); WaitForInReady();
		C2_WriteData(blen); WaitForInReady();

		// Write Flash block (BlkSize of 0x00 = 256)
		for (i=0; i < blen; i++) {
			C2_WriteData(*bbuf++);
			WaitForInReady();
		}

		// Check status after writing Flash block
		WaitForOutReady();
		if (C2_ReadData() != COMMAND_OK) {
			return -1;
		}

		addr += blen;
		len -= blen;
	}

	return 0;
}

//int si4010_flash_enable_write()
int si4010_reset()
{
	struct timespec tspec;

	c2_bus_reset(c2_bus_handle); // reset target device

	tspec.tv_sec = 0;
	tspec.tv_nsec = 2000L;
	nanosleep(&tspec, NULL);   // Delay for at least 2us
//TODO: check return

	// Initialize the C2 Interface for flashing & debugging
//TODO: check if this is really needed for debugging
	C2_WriteAddr(FPCTL);		// Target the C2 Flash Programming Control
					// register (FPCTL) for C2 Data register accesses

	C2_WriteData(INIT_KEY1);	// Write the first key code to enable C2 control
	C2_WriteData(INIT_KEY2);	// Write the second key code to enable C2 control
	C2_WriteData(INIT_KEY3);	// Write the third key code to enable C2 control

	tspec.tv_sec = 0;
	tspec.tv_nsec = 20000000L;	// Delay for at least 20ms to ensure the
	nanosleep(&tspec, NULL);
//TODO: check return

	return 0;
}

