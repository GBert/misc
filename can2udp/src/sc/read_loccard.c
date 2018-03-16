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

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -v -f\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -h                  this help\n\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

unsigned char *read_data(struct loco_config_t *loco_config) {
    FILE *fp;
    unsigned char *data;

    fp = fopen(loco_config->filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loco_config->filename);
	return NULL;
    }

    fseek(fp, 0, SEEK_END);
    loco_config->eeprom_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if ((data = malloc(loco_config->eeprom_size)) == NULL) {
	fprintf(stderr, "%s: can't alloc %d bytes for data\n", __func__, loco_config->eeprom_size);
	fclose(fp);
	return NULL;
    }

    if ((fread((void *)data, 1, loco_config->eeprom_size, fp)) != loco_config->eeprom_size) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, loco_config->filename);
	fclose(fp);
	free(data);
	return NULL;
    }

    fclose(fp);
    return data;
}

int write_data(struct loco_config_t *loco_config) {
    FILE *fp;
    unsigned char *data;

    data = loco_config->bin;

    fp = fopen(loco_config->filename, "wb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loco_config->filename);
	return EXIT_FAILURE;
    }

    if ((fwrite((void *)data, 1, loco_config->eeprom_size, fp)) != loco_config->eeprom_size) {
	fprintf(stderr, "%s: error writing failed [%s]\n", __func__, loco_config->filename);
	fclose(fp);
	return EXIT_FAILURE;
    }

    fclose(fp);
    return 0;
}

int decode_sc_data(struct loco_config_t *loco_config, struct loco_data_t *loco_data) {
    unsigned int i, j, k, func, id, temp;
    unsigned char index, length;
    char *loco_name;
    char *proto_name;

    index = 0;
    /* preamble */
    if (memcmp(loco_config->bin, pre_mfx, 3) == 0)
	printf("type: mfx\n");
    else if (memcmp(loco_config->bin, pre_other, 3) == 0)
	printf("type: other\n");
    else
	return EXIT_FAILURE;

    i = 3;

    while (i < loco_config->eeprom_size) {
	index = loco_config->bin[i++];
	length = loco_config->bin[i++];

	switch (index) {

	case 0:
	    printf("index [0x%02x @ 0x%04x] sub-indexes [%d]: ", index, i, length);
	    temp = loco_config->bin[i++];
	    length = (loco_config->bin[i++] << 8) + temp;
	    printf(" total length [%d]\n", length);
	    id = loco_config->bin[i++];
	    while ((id != 0) && (id != 255)) {
		length = loco_config->bin[i++];
		/* printf("i 0x%02x [i] 0x%02x length %d\n" , i, loco_config->bin[i], length); */
		switch (id) {
		case 0x1e:
		    loco_name = (char *)calloc(length + 1, 1);
		    if (loco_name == NULL)
			return EXIT_FAILURE;
		    memcpy(loco_name, (char *)&loco_config->bin[i], length);
		    i += length;
		    loco_data->name = loco_name;
		    printf("loco name: >%s<\n", loco_name);
		    break;
		case 0x1f:
		    proto_name = (char *)calloc(length + 1, 1);
		    if (proto_name == NULL)
			return EXIT_FAILURE;
		    strncpy(proto_name, (char *)&loco_config->bin[i], length);
		    i += length;
		    printf("proto name: >%s<\n", proto_name);
		    loco_data->proto = proto_name;
		    break;
		default:
		    printf("decoding problem:\n");
		    break;
		}
		id = loco_config->bin[i++];
	    }
	    break;
	case 9:
	    printf("index [0x%02x @ 0x%04x] length [%d]: ", index, i, length);
	    func = 0;
	    printf("\n");
	    for (j = 0; j < length / 10; j++) {
		printf(" function %2d: ", func++);
		for (k = 0; k < 10; k++) {
		    printf(" 0x%02x", loco_config->bin[i++]);
		}
		printf("\n");
	    }
	    break;
	default:
	    printf("index [0x%02x @ 0x%04x] length [%d]: ", index, i, length);
	    if (length <= 4) {
		memcpy(&temp, loco_config->bin, length);
	    }
	    switch (index) {
	    case 1:
		loco_data->long_uid = temp;
		printf("          long UID ");
		break;
	    case 2:
		loco_data->uid = temp;
		printf("         short UID ");
		break;
	    case 3:
		loco_data->acc_delay = temp;
		printf("acceleration delay ");
		break;
	    case 4:
		loco_data->slow_down_delay = temp;
		printf("   slow down delay ");
		break;
	    case 5:
		loco_data->vmin = temp;
		printf("              Vmin ");
		break;
	    case 6:
		loco_data->vmax = temp;
		printf("              Vmax ");
		break;
	    case 8:
		loco_data->volume = temp;
		printf("            Volume ");
		break;
	    default:
		printf("           unknown ");
		break;
	    }

	    for (j = 0; j < length; j++) {
		printf(" 0x%02x", loco_config->bin[i++]);
	    }
	    break;
	}
	printf("\n");
	if (index == 0)
	    break;
    }

    return 0;
}

int main(int argc, char **argv) {
    int opt, verbose;
    struct loco_config_t loco_config;
    struct loco_data_t loco_data;
    char *filename;

    /* defaults */
    memset(&loco_config, 0, sizeof(loco_config));
    memset(&loco_data, 0, sizeof(loco_data));
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
	loco_config.filename = filename;
    }

    loco_config.bin = read_data(&loco_config);
    if (loco_config.bin == NULL)
	return EXIT_FAILURE;

    if (verbose)
	printf("EEPROM Size (%s) : %d\n", filename, loco_config.eeprom_size);

    if (verbose)
	decode_sc_data(&loco_config, &loco_data);

    return 0;
}
