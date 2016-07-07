/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <assert.h>
#include <libgen.h>

#include "uthash.h"

#define FRAME_SIZE	13
#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

char *gleisbild_dir  = { "/gleisbilder" };
char *gleisbild_name = { "gleisbild.cs2" };
char *gbs_default    = { "gbs-0" };

struct config_data {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    int verbose;
    uint16_t crc;
    char *name;
    char *directory;
    char *filename;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};

int main(int argc, char **argv) {
    struct config_data config_data;
    int gbs_valid;
    char gbs[MAXGBS];
    char gbs_name[MAXNAME];
    char line[MAXSIZE];
    char *dir;
    char *gleisbild;
    FILE *fp;

    dir = calloc(MAXDIR, 1);
    if (dir == NULL) {
	fprintf(stderr, "can't alloc bufer for directory string: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if ( argc == 2 ) {
	strncpy(dir, argv[1], MAXDIR - 1);
	config_data.directory = dir;
    } else {
	fprintf(stderr, "usage: %s <dir> \n", basename(argv[0]));
	exit(EXIT_FAILURE);
    }

    config_data.verbose = 1;

    gleisbild = calloc(strlen(gleisbild_name) + strlen(config_data.directory) + 2, 1);
    strcpy(gleisbild, config_data.directory);
    strcat(gleisbild, "/");
    strcat(gleisbild, gleisbild_name);

    printf("gbn : >%s<\n", gleisbild);

    gbs_valid = 0;
    config_data.name = gbs_default;
    strcat(config_data.directory, gleisbild_dir);

    if ((fp = fopen(gleisbild, "r")) != NULL) {
	while (fgets(line, MAXSIZE, fp) != NULL) {
	    if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = 0;
	    if (strstr(line, "seite") == line) {
		gbs_valid = 1;
		printf("match seite: >%s<\n", line);
	    } else if (strstr(line, " .id=") == line) {
		strncpy(gbs, &line[5], strlen(&line[5]));
		printf("match id:    >%s<\n", line);
		config_data.name = gbs;
	    } else if (strstr(line, " .name=") == line) {
		printf("match name:  >%s<\n", line);
		if (gbs_valid) {
		    strncpy(gbs_name, &line[7], strlen(&line[7]));
		    strcat(gbs_name, ".cs2");
		    config_data.filename = gbs_name;
		}
	    }
	}
    } else {
	fprintf(stderr, "can't open gleisbild.cs2: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
