#-------------------------------------------------------------------------------
#
#   The Cowboy Boot Loader for Enhanced PIC12/PIC16
#
#   Copyright (c) 2017 Darron M Broad
#
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   This file is part of The Cowboy Boot Loader.
#
#   The Cowboy Boot Loader is free software: you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as published
#   by the Free Software Foundation.
#
#   The Cowboy Boot Loader is distributed in the hope that it will be
#   useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with The Cowboy Boot Loader. If not, see http://www.gnu.org/licenses/
#-------------------------------------------------------------------------------

build:
	cd boot && $(MAKE) build
	cd demo && $(MAKE) build

install:build
	cd boot && $(MAKE) install
	cd demo && $(MAKE) install

clean:
	cd boot && $(MAKE) clean
	cd demo && $(MAKE) clean
	find . -name "*~" -exec rm {} \;

uninstall:clean
	cd boot && $(MAKE) uninstall
	cd demo && $(MAKE) uninstall
