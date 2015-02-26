all: main

main: main.c
	sdcc -V --use-non-free -mpic14 -p16f1704 main.c

upload: main
	pk2cmd -B/usr/share/pk2/ -PPIC12F1704 -Fmain.hex -M -R -T

power: main
	pk2cmd -B/usr/share/pk2/ -PPIC12F1572 -T

clean:
	rm *.asm *.cod *.hex *.lst *.o


