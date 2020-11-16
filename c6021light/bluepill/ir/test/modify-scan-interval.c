/* ------------------------------------------- ---------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s <input_file> <output_file>\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -d  number of scans to sum\n\n");
}

int main(int argc, char **argv) {
    int i, in_mask, out_mask, n, nbits, opt, scans;
    char c, out_c;
    FILE *infile, *outfile;

    scans = 2;

    while ((opt = getopt(argc, argv, "d:v")) != -1) {
	switch (opt) {
	case 'd':
	    scans = atoi(optarg);
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	    break;
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    infile = fopen(argv[optind], "rb");
    if (infile == NULL) {
	printf("Error opening input file %s\n", argv[optind]);
	return (EXIT_FAILURE);
    }
    outfile = fopen(argv[++optind], "wb");
    if (outfile == NULL) {
	printf("Error opening output file %s\n", argv[optind]);
	fclose(infile);
	return (EXIT_FAILURE);
    }

    nbits = 0;
    n = 0;
    out_mask = 0x80;
    out_c = 0;
    while (!feof(infile)) {
	in_mask = 0x80;
	c = fgetc(infile);
	for (i = 0; i < 8; i++) {
	    nbits++;
	    if (c & in_mask)
		n++;
	    /* printf("nbits %d scans %d out_mask %d\n", nbits, scans, out_mask); */
	    if (nbits == scans) {
		if (n > (nbits / 2))
		    out_c |= out_mask;
		if (out_mask == 0) {
		    fputc(out_c, outfile);
		    out_c = 0;
		    out_mask = 0x80;
		}
		out_mask >>= 1;
		n = 0;
		nbits = 0;
	    }
	    in_mask >>= 1;
	}
    }
    fclose(infile);
    fclose(outfile);
    return 0;
}
