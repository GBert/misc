/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <errno.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_SIZE	16
#define UBOOT_SIZE	192*1024
#define UBOOT_ENV_SIZE	64*1024
#define MTD2_SIZE	64*1024

#define INSERT_DATA(filename, data, max) \
    do { \
	if (read_data(argv[optind], data, max) == 0) { \
	    return EXIT_FAILURE; \
	} \
	optind++; \
    } while(0)

char OUTPUT_FILE[] = "a5v11_XXXXXXXXXXXX.img\0\0\0\0";
int verbose = 0;
uint8_t *data;
char mac[13];

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -h -m <meg> -v <uboot.img> <uboot-env.img> <mtd2.bin> <openwrt.img>\n", prg);
    fprintf(stderr, "   Version 0.2\n\n");
    fprintf(stderr, "         -h                  this help\n");
    fprintf(stderr, "         -m <meg>            image size e.g. 8 or 16M default %d M\n\n", DEFAULT_SIZE);
}

int read_data(char *filename, uint8_t *position, int max) {
    FILE *fp;
    int size;

    if (verbose)
	printf("read filename: %s writing at offset 0x%08X\n", filename, (int) (position - data));
	      
    fp = fopen(filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, filename);
	return 0;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (size > max) {
	fprintf(stderr, "%s: error: data to large size[%s] > %d\n", __func__, filename, max);
	fclose(fp);
	return 0;
    }

    if ((fread((void *)position, 1, size, fp)) != size) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, filename);
	fclose(fp);
	return 0;
    }

    if ((position - data) == UBOOT_SIZE + UBOOT_ENV_SIZE) {
	memset(mac, 0, sizeof(mac));
	for (int i = 0; i <= 5; i++)
	    sprintf(&mac[i*2], "%02x", position[0x28 + i]);
	if (verbose)
	    printf(" MAC is %s\n", mac);
    }
    return size;
}

int main(int argc, char **argv) {
    FILE *fp;
    int opt, ret, size;
    char *filename = NULL;

    size = DEFAULT_SIZE * 1024 *1024;

    while ((opt = getopt(argc, argv, "m:vh?")) != -1) {
	switch (opt) {
	case 'm':
	    size = atoi(optarg) * 1024 * 1024;
	    break;
	case 'v':
	    verbose = 1;
	    break;
	case 'h':
	case '?':
	    print_usage(basename(argv[0]));
	    exit(EXIT_SUCCESS);
	default:
	    fprintf(stderr, "Unknown option %c\n", opt);
	    print_usage(basename(argv[0]));
	    exit(EXIT_FAILURE);
	}
    }

    data = malloc(size);
    if (!data) {
	fprintf(stderr, "%s: can't alloc image data\n", __func__);
	return EXIT_FAILURE;
    }
    memset(data, 0xff, size);

    if ((argc - optind) != 4) {
	free(data);
	return EXIT_FAILURE;
    }
	
    if (optind < argc) {
	INSERT_DATA(filename, data, UBOOT_SIZE);
	INSERT_DATA(filename, data + UBOOT_SIZE, UBOOT_ENV_SIZE);
	INSERT_DATA(filename, data + UBOOT_SIZE + UBOOT_ENV_SIZE, MTD2_SIZE);
	INSERT_DATA(filename, data + UBOOT_SIZE + UBOOT_ENV_SIZE + MTD2_SIZE, size - MTD2_SIZE - UBOOT_ENV_SIZE - UBOOT_SIZE);
    }
    sprintf(OUTPUT_FILE, "a5v11_%s.img", mac); 
    printf("output to %s\n", OUTPUT_FILE);
    fp = fopen(OUTPUT_FILE, "w");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, filename);
	return EXIT_FAILURE;
    }
    ret = fwrite(data, 1, size, fp);
    if (ret != size) {
	fprintf(stderr, "%s: error writing failed [%s]\n", __func__, filename);
	return EXIT_FAILURE;
    }

    free(data);
    return 0;
}
