/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* 
	TXD - 0x01
	RXD - 0x02
	RTS - 0x04
	CTS - 0x08
	DTR - 0x10
	DSR - 0x20
	DCD - 0x40
	RI  - 0x80
 */

#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/time.h>
#include <ftdi.h>

#define BAUDRATE	(8192l)
#define FIFO_SIZE	384

static struct ftdi_context *ftdic;

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -b baud\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -b <baud>    baudrate\n\n");
}

int do_init(int baudrate) {
    // ftdi_init(ftdic);
    ftdic = ftdi_new();

    if (ftdic == 0) {
	fprintf(stderr, "ftdi_new failed\n");
	return -1;
    }

    /* if (ftdi_usb_open_desc(ftdic, 0x0403, 0x6001, NULL, NULL) < 0) { */
    if (ftdi_usb_open_desc(ftdic, 0x0403, 0x6015, NULL, NULL) < 0) {
	fprintf(stderr, "ftdi_usb_open_desc failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    if (ftdi_set_bitmode(ftdic, 0x0f, BITMODE_SYNCBB) < 0) {
	fprintf(stderr, "ftdi_set_bitmode failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }

    printf("using baudrate %d\n", baudrate);
    if (ftdi_set_baudrate(ftdic, baudrate) < 0) {
	fprintf(stderr, "ftdi_set_baudrate failed: %s\n", ftdi_get_error_string(ftdic));
	return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
    uint8_t w_data[FIFO_SIZE];
    uint8_t r_data[FIFO_SIZE];
    struct timeval tm1, tm2;
    unsigned long elapsed_time;
    unsigned int baudrate;
    int opt;

    baudrate = BAUDRATE;

    while ((opt = getopt(argc, argv, "b:h?")) != -1) {
	switch (opt) {
        case 'b':
            baudrate = atoi(optarg);
            break;
        case 'h':
        case '?':
            print_usage(basename(argv[0]));
            exit(0);
            break;
        default:
            fprintf(stderr, "Unknown option %c\n", opt);
            print_usage(basename(argv[0]));
            exit(1);
        }
    }

    if (do_init(baudrate))
	exit(-1);

    for (int i = 0; i < sizeof(w_data); i++) {
	w_data[i] = (uint8_t) i & 0xff;
    }

    for (;;) {
	gettimeofday(&tm1, NULL);

	ftdi_write_data(ftdic, w_data, sizeof(w_data));
	ftdi_read_data(ftdic, r_data, sizeof(r_data));

	gettimeofday(&tm2, NULL);
	elapsed_time = 1E6 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec);
	printf("send %d bytes in %ld usecs\n", FIFO_SIZE, elapsed_time);
    }

    return 0;
}
