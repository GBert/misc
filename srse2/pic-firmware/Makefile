#------------------------------------------------------------------------------
#
# Can-Can Makefile
#
# Copyright (c) 2014 Darron M Broad
#
#------------------------------------------------------------------------------
#
# This file is part of the Can-Can CAN bus interface project.
#
# Can-Can is licensed under the CC BY-NC-SA 4.0.
#
# See file /LICENSE for details.
# 
#------------------------------------------------------------------------------

build:
	cd firmware && $(MAKE) build

clean:
	cd firmware && $(MAKE) clean
	rm -f *~ etc/*~ include/*~

flash:
	pk2cmd -B/usr/share/pickit2 -PPIC18F45K80 -E -MC -MP -F firmware/canbuster_4xk80.hex -R -T 
