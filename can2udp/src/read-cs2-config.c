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

int get_char_index(const char **list, char *str) {
    int index;

    index = 0;

    while (list[index]) {
	if (strlen(list[index]) == strlen(str))
	    if (strcmp(list[index], str) == 0)
		return index;
	index++;
    }
    return -1;
}

int get_char_index2(const char **list, char *str) {
    int index;

    index = 0;

    while (list[index]) {
	if (strstr(str, list[index]) == str)
	    return index;
	index++;
    }
    return -1;
}

int read_track_config(struct track_config_t *config_data, char *config_file) {
    char gbs_name[MAXNAME];
    int gbs_valid, id, l01_token_n, l2_token_n, ret;
    /* char gbs[MAXGBS]; */
    FILE *fp;
    char line[MAXSIZE];

    gbs_valid = 0;
    id = 0;

    if ((fp = fopen(config_file, "r")) != NULL) {
	while (fgets(line, MAXSIZE, fp) != NULL) {
	    if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = 0;
	    if (line[0] != ' ') {
		l01_token_n = get_char_index(l01_token, line);
		if (l01_token_n == L1_PAGE) {
		    gbs_valid = 1;
		    config_data->id = 0;
		    printf("match seite:   >%s<\n", line);
		}
	    } else {
		l2_token_n = get_char_index2(l2_token, line);
		switch (l2_token_n) {
		case L2_ID:
		    config_data->id = strtoul(&line[strlen(l2_token[l2_token_n])], NULL, 0);
		    printf("match id:      >%d<\n", config_data->id);
		    break;
		case L2_XOFFSET:
		    config_data->xoffset = strtoul(&line[strlen(l2_token[l2_token_n])], NULL, 0);
		    printf("match xoffset: >%d<\n", config_data->xoffset);
		    break;
		case L2_YOFFSET:
		    config_data->yoffset = strtoul(&line[strlen(l2_token[l2_token_n])], NULL, 0);
		    printf("match yoffset: >%d<\n", config_data->yoffset);
		    break;
		case L2_MAJOR:
		    config_data->major = strtoul(&line[strlen(l2_token[l2_token_n])], NULL, 0);
		    printf("match major:   >%d<\n", config_data->major);
		    break;
		case L2_MINOR:
		    config_data->minor = strtoul(&line[strlen(l2_token[l2_token_n])], NULL, 0);
		    printf("match minor:   >%d<\n", config_data->minor);
		    break;
		case L2_TYPE:
		    printf("match typ:     >%s<\n", line);
		    ret = get_char_index(track_types, &line[strlen(l2_token[l2_token_n])]);
		    if (ret >= 0) {
			printf("hit  %d -> %s\n", ret, track_types[ret]);
		    } else {
			printf("miss %d\n", ret);
		    }
		    break;
		case L2_NAME:
		    printf("match name:    >%s<\n", line);
		    if (gbs_valid) {
			strncpy(gbs_name, &line[7], strlen(&line[7]));
			strcat(gbs_name, ".cs2");
			config_data->name = gbs_name;
		    }
		    break;
		default:
		    printf("unknown:       >%s<\n", line);
		    break;
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
    if (track_file[strlen(track_file) - 1] != '/')
	strcat(track_file, "/");
    strcat(track_file, track_name);

    printf("gbn : >%s<\n", track_file);

    config_data.name = gbs_default;
    strcat(config_data.directory, track_dir);

    read_track_config(config_data.track_config, track_file);

    return EXIT_SUCCESS;
}
