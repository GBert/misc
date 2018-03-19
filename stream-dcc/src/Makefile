all: main

main: main.c
	sdcc -V --use-non-free -mpic14 -p16f1705 main.c

upload: main
	pk2cmd -B/usr/share/pk2/ -PPIC12F1705 -Fmain.hex -M -R -T

power: main
	pk2cmd -B/usr/share/pk2/ -PPIC16F1705 -T

clean:
	rm *.asm *.cod *.hex *.lst *.o


