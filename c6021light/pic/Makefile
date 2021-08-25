PROJ_DIR	= .
XC8DIR		= /opt/microchip/xc8/v1.38
CC_INCLUDE	= -I$(XC8DIR)/include

PROCESSOR	= 16F1705
MAIN		= main
SRC		= main.c usart.c
CC_FLAGS	= -P -N255 --warn=0 --addrqual=ignore --mode=free# --echo
OPTIMISATION	= --opt=default,+asm,-asmfile,+speed,-space,-debug,9 --runtime=+plib
CC		= $(XC8DIR)/bin/xc8
# CC		= sdcc

all: $(MAIN).hex

$(MAIN).hex: $(SRC)
#	$(CC) $(SRC) $(CC_FLAGS) $(OPTIMISATION) --ROM=default,-0c00-0fff --chip=$(PROCESSOR) $(CC_INCLUDE) --ASMLIST -M
	$(CC) $(SRC) $(CC_FLAGS) $(OPTIMISATION) --chip=$(PROCESSOR) $(CC_INCLUDE) --ASMLIST -M

flash: all
	pk2cmd -B/usr/share/pickit2 -PPIC16F1705 -Fmain.hex -M -R -T
#	p14 lvp p main.hex

clean:
	rm -f $(MAIN).hex funclist $(MAIN).cof $(MAIN).hxl *.p1 $(MAIN).sdb startup.* *.cmf *.rlf *.map *.as *.d *.obj *.lst *.pre $(MAIN).sym

