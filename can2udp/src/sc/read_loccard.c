/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* M*rklin smartcard (loc card) tool
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lokinfo.h"
#include "uthash.h"

struct loc_config_t {
    int eeprom_max_size;
    unsigned int eeprom_size;
    int id;
    FILE *fp;
    char *filename;
    unsigned char *bin;
};

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -v -f\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -h                  this help\n\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

unsigned char *read_data(struct loc_config_t *loc_config) {
    FILE *fp;
    unsigned char *data;

    fp = fopen(loc_config->filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loc_config->filename);
	return NULL;
    }

    fseek(fp, 0, SEEK_END);
    loc_config->eeprom_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if ((data = malloc(loc_config->eeprom_size)) == NULL) {
	fprintf(stderr, "%s: can't alloc %d bytes for data\n", __func__, loc_config->eeprom_size);
	fclose(fp);
	return NULL;
    }

    if ((fread((void *)data, 1, loc_config->eeprom_size, fp)) != loc_config->eeprom_size) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, loc_config->filename);
	fclose(fp);
	free(data);
	return NULL;
    }

    fclose(fp);
    return data;
}

int write_data(struct loc_config_t *loc_config) {
    FILE *fp;
    unsigned char *data;

    data = loc_config->bin;

    fp = fopen(loc_config->filename, "wb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loc_config->filename);
	return EXIT_FAILURE;
    }

    if ((fwrite((void *)data, 1, loc_config->eeprom_size, fp)) != loc_config->eeprom_size) {
	fprintf(stderr, "%s: error writing failed [%s]\n", __func__, loc_config->filename);
	return EXIT_FAILURE;
    }

    fclose(fp);
    return 0;
}

int print_data(struct loc_config_t *loc_config) {
    unsigned int i, j, k, func, id;
    unsigned char index, length;
    char *loco_name;
    char *proto_name;

    index = 0;
    /* preamble */
    if (memcmp(loc_config->bin, pre_mfx, 3) == 0)
	printf("MFX:\n");
    else if (memcmp(loc_config->bin, pre_other, 3) == 0)
	printf("Other:\n");
    else
	return EXIT_FAILURE;

    i = 3;

    while (i < loc_config->eeprom_size) {
	index = loc_config->bin[i++];
	length = loc_config->bin[i++];

	printf("index [%d @ 0x%04x] length [%d]:\n", index, i, length);
	switch (index) {

	case 0:
	    id = loc_config->bin[i++];
	    length = loc_config->bin[i];
	    printf("[0x%04x] length [%d]: %d\n", i, length, id);
	    if ((id != 0) && (id != 255)) {
		length = loc_config->bin[i++];
		switch (id) {
		case 0x1e:
		    loco_name = (char *)malloc(length + 1);
		    if (loco_name == NULL)
			return EXIT_FAILURE;
		    strncpy(loco_name, (char *)&loc_config->bin[i], length);
		    i += length;
		    printf("loco name: %s\n", loco_name);
		    break;
		case 0x1f:
		    proto_name = (char *)malloc(length + 1);
		    if (proto_name == NULL)
			return EXIT_FAILURE;
		    strncpy(proto_name, (char *)&loc_config->bin[i], length);
		    i += length;
		    printf("proto name: %s\n", proto_name);
		    break;
		default:
		    printf("decoding problem:\n");
		    break;
		}
	    }
	    break;
	case 9:
	    func = 0;
	    for (j = 0; j < length / 10; j++) {
		printf("function %d:\n", func++);
		for (k = 0; k < 10; k++) {
		    printf(" 0x%02x", loc_config->bin[i++]);
		}
		printf("\n");
	    }
	    break;
	default:
	    for (j = 0; j < length; j++) {
		printf(" 0x%02x", loc_config->bin[i++]);
	    }
	    break;
	}
	printf("\n");
	if (index == 0)
	    break;
    }
    printf("loco name: %s\n", loco_name);

    return 0;
}

int main(int argc, char **argv) {
    int opt, verbose;
    struct loc_config_t loc_config;
    char *filename;

    /* defaults */
    memset(&loc_config, 0, sizeof(loc_config));
    verbose = 1;

    while ((opt = getopt(argc, argv, "vh?")) != -1) {
	switch (opt) {

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

    if (optind < argc) {
	filename = (char *)malloc(strlen(argv[optind]) + 1);
	if (filename == NULL)
	    return EXIT_FAILURE;
	strncpy(filename, argv[optind], strlen(argv[optind]));
	loc_config.filename = filename;
    }

    loc_config.bin = read_data(&loc_config);
    if (loc_config.bin == NULL)
	return EXIT_FAILURE;

    if (verbose)
	printf("EEPROM Size (%s) : %d\n", filename, loc_config.eeprom_size);

    if (verbose)
	print_data(&loc_config);

    return 0;
}
