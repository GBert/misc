/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* M*rklin smartcard (loco card) tool
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

#include "cs2-config.h"

#define check_free(a) \
            do { if ( a ) free(a); } while (0)

unsigned char pre_mm[]    = { 0x02, 0x75, 0x00 };
unsigned char pre_mfx[]   = { 0x02, 0xf5, 0x00 };
unsigned char pre_other[] = { 0x02, 0xc5, 0x00 };

static char *I2C_DEF_PATH = "/sys/bus/i2c/devices/1-0050/eeprom";

static const char *loco_function_string [] = {
    " ",
    "Licht",
    "ka",
    "ka",
    "ka",
    "ka",
    "ka",
    "ka",
    "ka"
};

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -v -f\n", prg);
    fprintf(stderr, "   Version 0.2\n\n");
    fprintf(stderr, "         -h                  this help\n\n");
    fprintf(stderr, "         -v                  verbose output\n\n");
}

void print_bitmap(unsigned char *data) {
    uint8_t i, j, mask;
    unsigned char *line;

    line = data;   
    for (i = 0; i < 8; i++) {
	mask = 0x80;
	for ( j = 0; j < 8 ; j++) {
	   if (*line & mask)
		printf("*");
	   else
		printf(".");
	   mask >>= 1;
	}
	printf("\n");
	line++;
    }
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
	fprintf(stderr, "%s: can't alloc %u bytes for data\n", __func__, loco_config->eeprom_size);
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

char *extract_string(unsigned int *index, unsigned char *bin, unsigned int length) {
    char *s;
    s = calloc(length + 1, 1);
    if (s == NULL)
	return NULL;
    strncpy(s, (char *)&bin[*index], length);
    *index += length;
    return s;
}

int decode_sc_data(struct loco_config_t *loco_config, struct loco_data_t *loco_data) {
    unsigned int i, j, k, func, id, temp, png_size;
    unsigned char index, length;
    char *loco_name;
    char *proto_name;
    char *png_name;

    index = 0;

    /* preamble */
    if (memcmp(loco_config->bin, pre_mfx, 3) == 0)
	printf("type: mfx\n");
    else if (memcmp(loco_config->bin, pre_mm, 3) == 0)
	printf("type: mm\n");
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
	    printf("index [0x%02x @ 0x%04x] sub-index [%u]: ", index, i, length);
	    temp = loco_config->bin[i++];
	    length = (loco_config->bin[i++] << 8) + temp;
	    printf(" total length [%u]\n", length);
	    id = loco_config->bin[i++];
	    while ((id != 0) && (id != 255)) {
		length = loco_config->bin[i++];
		/* printf("i 0x%02x [i] 0x%02x length %u\n" , i, loco_config->bin[i], length); */
		switch (id) {
		case 0x1e:
		    loco_name = extract_string(&i, loco_config->bin, length);
		    if (loco_name == NULL)
			return EXIT_FAILURE;
		    loco_data->name = loco_name;
		    printf("loco name: >%s<\n", loco_name);
		    break;
		case 0x1f:
		    proto_name = extract_string(&i, loco_config->bin, length);
		    if (proto_name == NULL)
			return EXIT_FAILURE;
		    printf("proto name: >%s<\n", proto_name);
		    loco_data->type = proto_name;
		    break;
		case 0x20:
		    png_name = extract_string(&i, loco_config->bin, length);
		    if (png_name == NULL)
			return EXIT_FAILURE;
		    printf("png name: >%s<\n", png_name);
		    loco_data->icon = png_name;
		    break;
		case 0x05:
		    png_size = length + (loco_config->bin[i++] << 8);
		    printf("png start: 0x%04x  end: 0x%04x  size: 0x%04x  %u bytes\n",
			   i, i + png_size, png_size, png_size);
		    return EXIT_SUCCESS;
		default:
		    printf("decoding problem: 0x%02x\n", id);
		    break;
		}
		id = loco_config->bin[i++];
		if (id == 0)
		    id = loco_config->bin[i++];
	    }
	    break;
	/* Loco functions */
	case 9:
	    printf("index [0x%02x @ 0x%04x] length [%3d]: ", index, i, length);
	    func = 0;
	    printf("\n");
	    for (j = 0; j < length / 10; j++) {
		printf(" function %2u: ", func++);
		for (k = 0; k < 10; k++) {
		    printf(" 0x%02x", loco_config->bin[i++]);
		}
		printf("\n");
		i -= 10;
		for (k = 0; k < 10; k++) {
		    uint8_t ti = loco_config->bin[i++];
		    switch(k) {
		    case 0:
			printf(" %10s 0x%02x", loco_function_string[ti & 0x07], ti);
			break;
		    case 99: /* TODO */
			printf("\n");
			print_bitmap(&loco_config->bin[i]);
			break;
		    default:
			break;
		    }
		}
		printf("\n");
	    }
	    break;
	default:
	    printf("index [0x%02x @ 0x%04x] length [%3d]: ", index, i, length);
	    if (length <= 4)
		memcpy(&temp, loco_config->bin, length);
	    else
		temp = 0;
	    switch (index) {
	    case 1:
		loco_data->long_uid = temp;
		printf("           mfx UID ");
		break;
	    case 2:
		loco_data->uid = temp;
		printf("               UID ");
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
	    case 7:
		loco_data->tacho = temp;
		printf("             tacho ");
		break;
	    case 8:
		loco_data->volume = temp;
		printf("            volume ");
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
	filename = (char *)calloc(strlen(argv[optind]) + 1, 1);
	if (filename == NULL)
	    return EXIT_FAILURE;
	strncpy(filename, argv[optind], strlen(argv[optind]));
    } else {
	filename = (char *)calloc(strlen(I2C_DEF_PATH) + 1, 1);
	if (filename == NULL)
	    return EXIT_FAILURE;
	strncpy(filename, I2C_DEF_PATH, strlen(I2C_DEF_PATH));
    }

    loco_config.filename = filename;
    loco_config.bin = read_data(&loco_config);
    if (loco_config.bin == NULL)
	return EXIT_FAILURE;

    if (verbose)
	printf("EEPROM Size (%s) : %u\n", filename, loco_config.eeprom_size);

    if (verbose)
	decode_sc_data(&loco_config, &loco_data);

    check_free(loco_data.icon);
    check_free(loco_data.name);
    check_free(loco_data.type);

    return EXIT_SUCCESS;
}
