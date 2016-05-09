#
# Copyright (C) 2005-2015 Darron Broad
# All rights reserved.
#
# This file is part of Pickle Microchip PIC ICSP.
#
# Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation.
#
# Pickle Microchip PIC ICSP is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
#

# build targets (enable:1 disable:0)
P12=1
P14=1
P16=1
P24=1
P32=1
PCTRL=1
PTEST=1
PIO=1
PLOAD=1

# back/front-ends (where available) (enable:1 disable:0)
TTY=1
RPI=1
BPI=0
MCP23017=1
BITBANG=1
FTDI=1

# gdb else strip (gdb:1 strip:0)
GDB=0

# output verbose config (enable:1 disable:0)
VERBOSE=1

# use QSORT (quick sort) else TSEARCH (tree sort) (qsort:1 tsearch:0)
QSORT=1
