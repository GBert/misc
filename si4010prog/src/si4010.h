/*
 * si4010.h - Silicon Labs SI4010 C2 Debug and Control Functions
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
#ifndef __SI4010_H__
#define __SI4010_H__

#include <stdlib.h>
#include <stdint.h>

#include "c2_bus.h"

int si4010_init(struct c2_bus *bus);
uint16_t c2_get_chip_version();
int si4010_reset();
int si4010_halt();
int si4010_resume();
int si4010_bp_set(uint8_t bpid, uint16_t addr);
int si4010_bp_clear(uint8_t bpid);
int si4010_bp_clear_all();
int si4010_pc_get(uint16_t *pc);
int si4010_pc_set(uint16_t pc);
int si4010_sfr_read(uint8_t addr, uint8_t len, void *buf);
int si4010_sfr_write(uint8_t addr, uint8_t len, const void *buf);
int si4010_ram_read(uint8_t addr, uint8_t len, void *buf);
int si4010_ram_write(uint8_t addr, uint8_t len, const void *buf);
int si4010_xram_read(uint16_t addr, size_t len, void *buf);
int si4010_xram_write(uint16_t addr, uint8_t len, const void *buf);

#endif // __SI4010_H__
