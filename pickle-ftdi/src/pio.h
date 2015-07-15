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

#ifndef _PIO_H
#define _PIO_H

#include "pickle.h"

/* prototypes */
void usage(char *, char *);
uint8_t getbytearg(char *, char *);
uint16_t getshortarg(char *, char *);
uint32_t get24arg(char *, char *);
uint32_t getintarg(char *, char *);
uint8_t getportarg(char *, char *);
int main(int, char **);

#endif /* !_PIO_H */
