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

#ifndef _PTEST_H
#define _PTEST_H

#include "pickle.h"

/* prototypes */
void usage(char *, char *);
void test_pin(int, int);
void test_dsub9(int);
void test_icsp(int);
void test_toggle(int);
void test_debug(int);
void test_icspio(int);
void test_rpi(int);
void test_bpi(int);
int main(int, char **);

#endif /* !_PTEST_H */
