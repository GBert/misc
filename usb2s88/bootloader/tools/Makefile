CFLAGS = -fstack-protector -fstack-protector-all
# CFLAGS = -O3

454HEX2DFU_C = 454hex2dfu.c
454HEX2DFU_H = 

all: 454hex2dfu

454hex2dfu: Makefile $(454HEX2DFU_C) $(454HEX2DFU_H)
	gcc $(454HEX2DFU_C) -o $@ $(CFLAGS)

clean:
	rm -f 454hex2dfu 454hex2dfu.exe
