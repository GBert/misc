PROJ_DIR	= .
XC8DIR		= /opt/microchip/xc8/v1.32
CC_INCLUDE	= -I$(XC8DIR)/include

PROCESSOR	= 16F1614
MAIN		= main
SRC		= main.c
CC_FLAGS	= -P -N255 --warn=0 --addrqual=ignore --mode=free# --echo
OPTIMISATION	= --opt=default,+asm,-asmfile,+speed,-space,-debug,9 --runtime=+plib
CC		= $(XC8DIR)/bin/xc8

TARGET		= $(MAIN)

all: $(MAIN).hex

$(MAIN).hex: $(SRC)
	$(CC) $(SRC) $(CC_FLAGS) $(OPTIMISATION) --ROM=default,-fc00-ffff --chip=$(PROCESSOR) $(CC_INCLUDE) --ASMLIST -M

flash: 
	pk2cmd -PPIC$(PROCESSOR) -F$(TARGET).hex -M -R -T


power: ${TARGET}
	pk2cmd -B/usr/share/pk2/ -PPIC$(PROCESSOR) -T -R

serial: ${TARGET}
	./pk2serial -r -p -b 57600

clean:
	rm -f $(MAIN).hex funclist $(MAIN).cof $(MAIN).hxl *.p1 $(MAIN).sdb startup.* *.cmf *.rlf *.map *.as *.d *.obj *.lst *.pre $(MAIN).sym

