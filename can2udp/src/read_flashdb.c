/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

/* M*rklin MS2 loco flash tool
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

#include "uthash.h"

#define OFFSET_LOCO_ID		0
#define OFFSET_LOCO_NAME	7
#define OFFSET_PROTO		25
#define OFFSET_DEFAULT_ID	26
#define DATA_SET_LENGTH		64

#define PROTO_DELTA_ID	0
#define PROTO_MM2_ID	1
#define PROTO_MFX_ID	2
#define PROTO_DCC_ID	4


static char PROTO_DELTA[] = "delta";
static char PROTO_MM[] = "motorola";

struct loco_func {
    uint8_t number;
    uint8_t type;
    uint16_t duration;
    uint16_t value;
};

struct loco_data_t {
    unsigned int major;
    unsigned int minor;
    unsigned int id;
    unsigned int uid;
    unsigned int direction;
    unsigned int velocity;
    unsigned int long_uid;
    char *name;
    char *surname;
    char *type;
    char *icon;
    unsigned int address;
    unsigned int sid;
    unsigned int mfxuid;
    unsigned int symbol;
    unsigned int acc_delay;		/* av */
    unsigned int slow_down_delay;	/* bv */
    unsigned int volume;
    unsigned int progmask;
    unsigned int tmax;
    unsigned int vmin;
    unsigned int vmax;
    unsigned int xprot;
    unsigned int traction;
    unsigned int spm;
    unsigned int ft;
    unsigned int mfxtype;
    struct loco_func function[16];
    struct mfxAdr_t *mfxAdr;
    UT_hash_handle hh;
};

struct loco_db_t {
    int eeprom_max_size;
    unsigned int eeprom_size;
    FILE *fp;
    char *id;
    char *filename;
    unsigned char *bin;
};

static char *DEF_PATH = "/mnt/home/cs2/update/flashdb.ms2";

void print_usage(char *prg) {
    fprintf(stderr, "\nUsage: %s -v -f\n", prg);
    fprintf(stderr, "   Version 0.1\n\n");
    fprintf(stderr, "         -h      this help\n");
    fprintf(stderr, "         -v      verbose output\n");
}

unsigned char *read_data(struct loco_db_t *loco_db) {
    FILE *fp;
    unsigned char *data;

    fp = fopen(loco_db->filename, "rb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loco_db->filename);
	return NULL;
    }

    fseek(fp, 0, SEEK_END);
    loco_db->eeprom_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if ((data = malloc(loco_db->eeprom_size)) == NULL) {
	fprintf(stderr, "%s: can't alloc %u bytes for data\n", __func__, loco_db->eeprom_size);
	fclose(fp);
	return NULL;
    }

    if ((fread((void *)data, 1, loco_db->eeprom_size, fp)) != loco_db->eeprom_size) {
	fprintf(stderr, "%s: error: fread failed for [%s]\n", __func__, loco_db->filename);
	fclose(fp);
	free(data);
	return NULL;
    }

    fclose(fp);
    return data;
}

int write_data(struct loco_db_t *loco_db) {
    FILE *fp;
    unsigned char *data;

    data = loco_db->bin;

    fp = fopen(loco_db->filename, "wb");
    if (fp == NULL) {
	fprintf(stderr, "%s: error fopen failed [%s]\n", __func__, loco_db->filename);
	return EXIT_FAILURE;
    }

    if ((fwrite((void *)data, 1, loco_db->eeprom_size, fp)) != loco_db->eeprom_size) {
	fprintf(stderr, "%s: error writing failed [%s]\n", __func__, loco_db->filename);
	fclose(fp);
	return EXIT_FAILURE;
    }

    fclose(fp);
    return 0;
}

int decode_sc_data(struct loco_db_t *loco_db, struct loco_data_t *loco_data) {
    unsigned int i, n, proto, address, length;
    char *loco_id;
    char *loco_name;
    char *proto_name;

    /* preamble */
    i = DATA_SET_LENGTH;
    n = 0;

    while (i < loco_db->eeprom_size) {
	n++;
	length = strlen((char *)&loco_db->bin[i + OFFSET_LOCO_ID]);
	loco_id = (char *)calloc(length + 1, 1);
	if (loco_id == NULL)
	    return EXIT_FAILURE;
	memcpy(loco_id, (char *)&loco_db->bin[i], length);

	length = strlen((char *)&loco_db->bin[i + OFFSET_LOCO_NAME]);
	loco_name = (char *)calloc(length + 1, 1);
	if (loco_name == NULL) {
	    free(loco_id);
	    return EXIT_FAILURE;
	}
	memcpy(loco_name, (char *)&loco_db->bin[i + OFFSET_LOCO_NAME], length);

	proto = loco_db->bin[i + OFFSET_PROTO];
	address = loco_db->bin[i + OFFSET_DEFAULT_ID];

	switch (proto) {
	case PROTO_DELTA_ID:
	    proto_name = PROTO_DELTA;
	    break;
	case PROTO_MM2_ID:
	    proto_name = PROTO_MM;
	    break;
	}

	printf("%4u loco id: %8s %15s proto: %8s  default address: %2u\n", n, loco_id, loco_name, proto_name, address);
	free(loco_id);
	free(loco_name);

	i += DATA_SET_LENGTH;
    }

#if 0

    while (i < loco_db->eeprom_size) {
	index = loco_db->bin[i++];
	length = loco_db->bin[i++];

	switch (index) {

	case 0:
	    printf("index [0x%02x @ 0x%04x] sub-indexes [%d]: ", index, i, length);
	    temp = loco_db->bin[i++];
	    length = (loco_db->bin[i++] << 8) + temp;
	    printf(" total length [%d]\n", length);
	    id = loco_db->bin[i++];
	    while ((id != 0) && (id != 255)) {
		length = loco_db->bin[i++];
		/* printf("i 0x%02x [i] 0x%02x length %d\n" , i, loco_db->bin[i], length); */
		switch (id) {
		case 0x1e:
		    loco_name = (char *)calloc(length + 1, 1);
		    if (loco_name == NULL)
			return EXIT_FAILURE;
		    memcpy(loco_name, (char *)&loco_db->bin[i], length);
		    i += length;
		    loco_data->name = loco_name;
		    printf("loco name: >%s<\n", loco_name);
		    break;
		case 0x1f:
		    proto_name = (char *)calloc(length + 1, 1);
		    if (proto_name == NULL)
			return EXIT_FAILURE;
		    strncpy(proto_name, (char *)&loco_db->bin[i], length);
		    i += length;
		    printf("proto name: >%s<\n", proto_name);
		    loco_data->type = proto_name;
		    break;
		default:
		    printf("decoding problem:\n");
		    break;
		}
		id = loco_db->bin[i++];
	    }
	    break;
	case 9:
	    printf("index [0x%02x @ 0x%04x] length [%d]: ", index, i, length);
	    func = 0;
	    printf("\n");
	    for (j = 0; j < length / 10; j++) {
		printf(" function %2d: ", func++);
		for (k = 0; k < 10; k++) {
		    printf(" 0x%02x", loco_db->bin[i++]);
		}
		printf("\n");
	    }
	    break;
	default:
	    printf("index [0x%02x @ 0x%04x] length [%d]: ", index, i, length);
	    if (length <= 4) {
		memcpy(&temp, loco_db->bin, length);
	    }
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
	    case 8:
		loco_data->volume = temp;
		printf("            Volume ");
		break;
	    default:
		printf("           unknown ");
		break;
	    }

	    for (j = 0; j < length; j++) {
		printf(" 0x%02x", loco_db->bin[i++]);
	    }
	    break;
	}
	printf("\n");
	if (index == 0)
	    break;
    }
#endif

    return 0;
}

int main(int argc, char **argv) {
    int opt, verbose;
    struct loco_db_t loco_db;
    struct loco_data_t loco_data;
    char *filename;

    /* defaults */
    memset(&loco_db, 0, sizeof(loco_db));
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
	filename = (char *)calloc(strlen(DEF_PATH) + 1, 1);
	if (filename == NULL)
	    return EXIT_FAILURE;
	strncpy(filename, DEF_PATH, strlen(DEF_PATH));
    }

    loco_db.filename = filename;
    loco_db.bin = read_data(&loco_db);
    if (loco_db.bin == NULL)
	return EXIT_FAILURE;

    if (verbose)
	printf("Flash DB Size (%s) : %u\n", filename, loco_db.eeprom_size);

    if (verbose)
	decode_sc_data(&loco_db, &loco_data);

    free(filename);
    free(loco_db.bin);
    return 0;
}
