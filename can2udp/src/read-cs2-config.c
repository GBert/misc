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

struct track_page_t *track_page = NULL;

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

int add_track_page(int id, char *name) {
    struct track_page_t *t;

    HASH_FIND_INT(track_page, &id, t);	/* id already in the hash? */
    if (t == NULL) {
	t = (struct track_page_t *)calloc(1, sizeof(struct track_page_t));
	if (!t) {
	    fprintf(stderr, "%s: can't calloc track page: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	t->id = id;
	t->name = calloc(1, strlen(name) + 1);
	if (!t->name) {
	    fprintf(stderr, "%s: can't calloc track page name: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(t->name, name);
	HASH_ADD_INT(track_page, id, t);	/* id: name of key field */
    }
    return (EXIT_SUCCESS);
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

int id_sort(struct track_page_t *a, struct track_page_t *b) {
    return (a->id - b->id);
}

void sort_by_id(void) {
    HASH_SORT(track_page, id_sort);
}

void print_pages(void) {
    struct track_page_t *s;

    for (s = track_page; s != NULL; s = s->hh.next) {
	printf("user id %d: name %s\n", s->id, s->name);
    }
}

int read_track_config(char *config_file) {
    int gbs_valid, l01_token_n, l2_token_n, ret;
    FILE *fp;
    char line[MAXSIZE];
    struct track_page_t *page;

    gbs_valid = 0;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    page = calloc(1, sizeof(struct track_page_t));

    while (fgets(line, MAXSIZE, fp) != NULL) {
	if (line[strlen(line) - 2] == '\r')
	    line[strlen(line) - 2] = 0;
	if (line[strlen(line) - 1] == '\n')
	    line[strlen(line) - 1] = 0;
	if (line[0] != ' ') {
	    l01_token_n = get_char_index(l01_token, line);
	    if (l01_token_n == L1_PAGE) {
		gbs_valid = 1;
		page->id = 0;
		printf("match seite:   >%s<\n", line);
	    }
	} else {
	    l2_token_n = get_char_index2(l2_token, line);
	    switch (l2_token_n) {
	    case L2_ID:
		page->id = strtoul(&line[L2_ID_LENGTH], NULL, 0);
		printf("match id:      >%d<\n", page->id);
		break;
	    case L2_XOFFSET:
		page->xoffset = strtoul(&line[L2_XOFFSET_LENGTH], NULL, 0);
		printf("match xoffset: >%d<\n", page->xoffset);
		break;
	    case L2_YOFFSET:
		page->yoffset = strtoul(&line[L2_YOFFSET_LENGTH], NULL, 0);
		printf("match yoffset: >%d<\n", page->yoffset);
		break;
	    case L2_MAJOR:
		page->major = strtoul(&line[L2_MAJOR_LENGTH], NULL, 0);
		printf("match major:   >%d<\n", page->major);
		break;
	    case L2_MINOR:
		page->minor = strtoul(&line[L2_MINOR_LENGTH], NULL, 0);
		printf("match minor:   >%d<\n", page->minor);
		break;
	    case L2_TYPE:
		ret = get_char_index(track_types, &line[L2_TYPE_LENGTH]);
		printf("match typ:     >%s< %d\n", &line[L2_TYPE_LENGTH], ret);
		break;
	    case L2_NAME:
		if (gbs_valid) {
		    printf("match name:    >%s<  id %d\n", &line[L2_NAME_LENGTH], page->id);
		    add_track_page(page->id, &line[L2_NAME_LENGTH]);
		}
		break;
	    default:
		printf("unknown:       >%s<\n", line);
		break;
	    }
	}
    }
    free(page);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    struct track_page_t *track_page;
    struct config_data_t config_data;
    char *dir;
    char *track_file;

    dir = calloc(1, MAXDIR);
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

    track_page = calloc(1, sizeof(track_page));

    config_data.verbose = 1;

    track_file = calloc(1, strlen(track_name) + strlen(config_data.directory) + 2);
    strcpy(track_file, config_data.directory);
    if (track_file[strlen(track_file) - 1] != '/')
	strcat(track_file, "/");
    strcat(track_file, track_name);

    printf("gbn : >%s<\n", track_file);

    config_data.name = gbs_default;
    strcat(config_data.directory, track_dir);

    read_track_config(track_file);
    sort_by_id();
    print_pages();

    return EXIT_SUCCESS;
}
