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

#ifndef _INHX32_H
#define _INHX32_H

/* intel hex format 32 definitions */
#define BB (1)
#define AAAA (3)
#define TT (7)
#define HHHH (9)
#define TT_DATA (0)
#define TT_EOF (1)
#define TT_EXTENDED_SEGMENT_ADDRESS (2)
#define TT_START_SEGMENT_ADDRESS (3)
#define TT_EXTENDED_LINEAR_ADDRESS (4)
#define TT_START_LINEAR_ADDRESS (5)

/* prototypes */
uint32_t inhx32_array_create(pic_data ***, const char *, uint32_t *);
void inhx32_array_free(pic_data **, uint32_t);
#if P32
uint32_t inhx32_memory_create(uint8_t **, const char *);
#endif /* P32 */

#endif /* !_INHX32_H */
