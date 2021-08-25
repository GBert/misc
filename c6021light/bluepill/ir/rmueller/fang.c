// fang.c : Konsolenanwendung zur Digitalsignalaufzeichnung auf einem PC mit Linux
// C 2017 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#include <stdio.h>
#include <sys/io.h>

unsigned char puffer[500000];
long unsigned int n, anzahl;

int main(int argc, char **argv)
{
	unsigned char act, start, b, mask;

	anzahl = 60000;

	if (iopl(3) != 0) {
		printf("%s benötigt Root-Rechte\n", argv[0]);
		return 255;
	}

	printf("%ld Samples <= ", anzahl * 8);
	asm ("cli");					// ab hier Echtzeitbetrieb

	// Timer 2 einstellen
	outb(0xB6, 0x43);
	outb(0xFF, 0x42);
	outb(0xFF, 0x42);
	for (n=0; n<anzahl; n++) puffer[n] = 0;

	start = inb(0x42);
	for (n=0; n<anzahl; n++) {
//		puffer[n] = 0;
		for (b=0; b<8; b++)	{
			mask = 128 >> b;
			do {					// 11us warten
				act = inb(0x42);	// oberes Byte verwerfen
				act = inb(0x42);	// unteres Byte 
			} while ((unsigned char)(start - act) < 24);
			start = act;

			if (inb(0x3FE) & 0x40) puffer[n] += mask;
		}
	}
	act = inb(0x42);				// oberes Byte verwerfen

	asm ("sti");					// Ende Echtzeitbetrieb
	printf("=>\n");


	for (n=0; n<300; n++) {
		printf("%.2x  ", puffer[n]);
	}	

	FILE *aus = fopen("test.dat", "wb");
	fwrite(puffer, 1, anzahl, aus);
	fclose(aus);

	return 0;
}

