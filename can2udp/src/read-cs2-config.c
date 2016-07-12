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

#include "cs2-config.h"

#define FRAME_SIZE	13
#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

int read_track_file(struct track_data_t *config_data, char *config_file) {
    char gbs_name[MAXNAME];
    int gbs_valid;
    char gbs[MAXGBS];
    FILE *fp;
    char line[MAXSIZE];

    gbs_valid = 0;

    if ((fp = fopen(config_file, "r")) != NULL) {
	while (fgets(line, MAXSIZE, fp) != NULL) {
	    if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = 0;
	    if (strstr(line, "seite") == line) {
		gbs_valid = 1;
		printf("match seite: >%s<\n", line);
	    } else if (strstr(line, " .id=") == line) {
		strncpy(gbs, &line[5], strlen(&line[5]));
		printf("match id:    >%s<\n", line);
		config_data->name = gbs;
	    } else if (strstr(line, " .name=") == line) {
		printf("match name:  >%s<\n", line);
		if (gbs_valid) {
		    strncpy(gbs_name, &line[7], strlen(&line[7]));
		    strcat(gbs_name, ".cs2");
		    /* config_data->filename = gbs_name; */
		}
	    }
	}
    } else {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int read_track_config(struct config_data_t *config_data, char *config_file) {
    char gbs_name[MAXNAME];
    int gbs_valid;
    char gbs[MAXGBS];
    FILE *fp;
    char line[MAXSIZE];

    gbs_valid = 0;

    if ((fp = fopen(config_file, "r")) != NULL) {
	while (fgets(line, MAXSIZE, fp) != NULL) {
	    if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = 0;
	    if (strstr(line, "seite") == line) {
		gbs_valid = 1;
		printf("match seite: >%s<\n", line);
	    } else if (strstr(line, " .id=") == line) {
		strncpy(gbs, &line[5], strlen(&line[5]));
		printf("match id:    >%s<\n", line);
		config_data->name = gbs;
	    } else if (strstr(line, " .name=") == line) {
		printf("match name:  >%s<\n", line);
		if (gbs_valid) {
		    strncpy(gbs_name, &line[7], strlen(&line[7]));
		    strcat(gbs_name, ".cs2");
		    config_data->filename = gbs_name;
		}
	    }
	}
    } else {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    struct track_config_t *track_config;
    struct config_data_t config_data;
    char *dir;
    char *track_file;

    dir = calloc(MAXDIR, 1);
    if (dir == NULL) {
	fprintf(stderr, "can't alloc bufer for directory string: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (argc == 2) {
	strncpy(dir, argv[1], MAXDIR - 1);
	config_data.directory = dir;
    } else {
	fprintf(stderr, "usage: %s <dir> \n", basename(argv[0]));
	exit(EXIT_FAILURE);
    }

    track_config = calloc(sizeof(track_config), 1);
    config_data.track_config = track_config;

    config_data.verbose = 1;

    track_file = calloc(strlen(track_name) + strlen(config_data.directory) + 2, 1);
    strcpy(track_file, config_data.directory);
    strcat(track_file, "/");
    strcat(track_file, track_name);

    printf("gbn : >%s<\n", track_file);

    config_data.name = gbs_default;
    strcat(config_data.directory, track_dir);

    read_track_config(&config_data, track_file);

    return EXIT_SUCCESS;
}
