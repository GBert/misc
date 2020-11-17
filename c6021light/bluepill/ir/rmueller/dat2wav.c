// dat2wav.c : Konsolenanwendung zur Umwandlung einer Digitalsignalaufzeichnung.
// C 2005 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#include <stdio.h>

#ifdef WIN32
  #include <windows.h>
  #define CP_UTF8 65001
#endif

FILE *infile, *outfile;
unsigned char db;

#define WRTXT(x)	fwrite(x, 1, sizeof(x)-1, outfile)	

void WRDAT(unsigned long u, int s)
{
	fwrite(&u, 1, s, outfile);
}

void wrkopf(unsigned long datalen)
{
	// 52494646 datalen+ 57415645666D7420 10000000 0100 0100 88580100 88580100 0100 0800 64617461 datalen

	WRTXT("RIFF");			/* chunk ID */
	WRDAT(datalen + 36, 4);	/* chunk size */
	WRTXT("WAVEfmt ");		/* form type or list type */
	WRDAT(16, 4);			/* offset of data portion of chunk */
	WRDAT(1, 2);			/* format type */
	WRDAT(1, 2);			/* number of channels (i.e. mono, stereo...) */
	WRDAT(88200, 4);		/* sample rate */
	WRDAT(88200, 4);		/* for buffer estimation */
	WRDAT(1, 2);			/* block size of data */
	WRDAT(8, 2);			/* number of bits per sample of mono data */
	WRTXT("data");			/* data indicator */
	WRDAT(datalen, 4);		/* data size */
}

int main(int argc, char *argv[], char *envp[])
{
    int n;
    
#ifdef WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif

	printf("dat2wav vom %s\n", __DATE__);
	if (argc < 3) {
		printf("Aufruf: dat2wav datfilename wavfilename\n");
		return 8;	
    }
	infile = fopen(argv[1], "rb");
	if (infile == NULL) {
		printf("Fehler beim Öffnen der Eingabedatei \n");
		return 4;
	}
	outfile = fopen(argv[2], "wb");
	if (outfile == NULL) {
		printf("Fehler beim Öffnen der Ausgabedatei \n");
		return 4;
	}
	fseek(infile, 0L, SEEK_END);
	wrkopf(ftell(infile) * 8);
	fseek(infile, 0L, SEEK_SET);

	while (fread(&db, 1, 1, infile) > 0) for (n = 128; n>0; n >>= 1) {
			WRDAT((db & n) ? 0x40 : 0xc0, 1);
	}

	fclose(infile);
	fclose(outfile);
	printf("Umwandlung durchgeführt \n");
	return 0;
}
