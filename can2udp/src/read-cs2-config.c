/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <assert.h>
#include <libgen.h>

#include "cs2-config.h"
#include "lokinfo.h"

#define FRAME_SIZE	13
#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

struct track_page_t *track_page = NULL;
struct track_data_t *track_data = NULL;
struct loco_data_t *loco_data = NULL;

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

int add_loco(struct loco_data_t *loco, char *name) {
    struct loco_data_t *l;

    HASH_FIND_INT(loco_data, &loco->id, l);
    if (l == NULL) {
	l = (struct loco_data_t *)calloc(1, sizeof(struct loco_data_t));
	if (!l) {
	    fprintf(stderr, "%s: can't calloc loco data: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	l->name = calloc(1, strlen(name) + 1);
	if (!l->name) {
	    fprintf(stderr, "%s: can't calloc loco name: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(l->name, name);

	l->proto = calloc(1, strlen(loco->proto) + 1);
	if (!l->proto) {
	    fprintf(stderr, "%s: can't calloc protocol name: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(l->proto, loco->proto);

	l->id = loco->id;
	l->long_uid = loco->long_uid;
	l->address = loco->address;
	l->typ = loco->typ;
	l->mfxuid = loco->mfxuid;
	l->acc_delay = loco->acc_delay;
	l->slow_down_delay = loco->slow_down_delay;
	l->volume = loco->volume;
	l->vmax = loco->vmax;
	l->vmin = loco->vmin;
	/* TODO: mfx struct */
    }
    return (EXIT_SUCCESS);
}

int add_track_data(struct track_data_t *td) {
    struct track_data_t *t;

    HASH_FIND_INT(track_data, &td->id, t);	/* id already in the hash? */
    if (t == NULL) {
	t = (struct track_data_t *)calloc(1, sizeof(struct track_data_t));
	if (!t) {
	    fprintf(stderr, "%s: can't calloc track data struct: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	if (td->name) {
	    t->name = calloc(1, strlen(td->name) + 1);
	    if (!t->name) {
		fprintf(stderr, "%s: can't calloc track page name: %s\n", __func__, strerror(errno));
		return (EXIT_FAILURE);
	    }
	    strcpy(t->name, td->name);
	}
	t->id = td->id;
	t->type = td->type;
	t->rotation = td->rotation;
	t->item = td->item;
	t->state = td->state;
	/* TODO: side ??? */
	HASH_ADD_INT(track_data, id, t);	/* id: name of key field */
    } else {
	if (t->type)
	    t->type = td->type;
	if (t->rotation)
	    t->rotation = td->rotation;
	if (t->item)
	    t->item = td->item;
	if (t->state)
	    t->state = td->state;
    }
    return (EXIT_SUCCESS);
}

int add_track_page(struct track_page_t *page, char *name) {
    struct track_page_t *t;

    HASH_FIND_INT(track_page, &page->id, t);	/* id already in the hash? */
    if (t == NULL) {
	t = (struct track_page_t *)calloc(1, sizeof(struct track_page_t));
	if (!t) {
	    fprintf(stderr, "%s: can't calloc track page: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	t->name = calloc(1, strlen(name) + 1);
	if (!t->name) {
	    fprintf(stderr, "%s: can't calloc track page name: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(t->name, name);
	t->id = page->id;
	t->xoffset = page->xoffset;
	t->yoffset = page->yoffset;
	t->major = page->major;
	t->minor = page->minor;
	t->width = page->width;
	t->height = page->height;
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

int read_track_data(char *config_file) {
    int l01_token_n, l2_token_n;
    FILE *fp;
    char line[MAXSIZE];
    struct track_page_t *track_page;
    struct track_data_t *track_data;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "%s: can't open track file %s: %s\n", __func__, config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    track_page = calloc(1, sizeof(struct track_page_t));
    track_data = calloc(1, sizeof(struct track_data_t));

    while (fgets(line, MAXSIZE, fp) != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l01_token_n = get_char_index(l01_token, line);
	    if (l01_token_n == L1_PAGE) {
		track_page->id = 0;
		printf("match seite:   >%s<\n", line);
	    }
	} else {
	    l2_token_n = get_char_index2(l2_token, line);
	    switch (l2_token_n) {
	    case L2_ID:
		track_page->id = strtoul(&line[L2_ID_LENGTH], NULL, 0);
		printf("match id:      >0x%06x<\n", track_page->id);
		break;
	    case L2_MAJOR:
		track_page->major = strtoul(&line[L2_MAJOR_LENGTH], NULL, 0);
		printf("match major:   >%d<\n", track_page->major);
		break;
	    case L2_MINOR:
		track_page->minor = strtoul(&line[L2_MINOR_LENGTH], NULL, 0);
		printf("match minor:   >%d<\n", track_page->minor);
		break;
	    case L2_XOFFSET:
		track_page->xoffset = strtoul(&line[L2_XOFFSET_LENGTH], NULL, 0);
		printf("match xoffset: >%d<\n", track_page->xoffset);
		break;
	    case L2_YOFFSET:
		track_page->yoffset = strtoul(&line[L2_YOFFSET_LENGTH], NULL, 0);
		printf("match yoffset: >%d<\n", track_page->yoffset);
		break;
	    case L2_WIDTH:
		track_page->width = strtoul(&line[L2_WIDTH_LENGTH], NULL, 0);
		printf("match width:   >%d<\n", track_page->width);
		break;
	    case L2_HEIGHT:
		track_page->height = strtoul(&line[L2_HEIGHT_LENGTH], NULL, 0);
		printf("match height:  >%d<\n", track_page->height);
		break;
	    case L2_TYPE:
		track_data->type = get_char_index(track_types,&line[L2_TYPE_LENGTH]);
		printf("match type:    >%d<\n", track_data->type);
		break;
	    case L2_ROTATION:
		track_data->rotation = strtoul(&line[L2_ROTATION_LENGTH], NULL, 0);
		printf("match rotation:>%d<\n", track_data->rotation);
		break;
	    case L2_ITEM:
		track_data->item = strtoul(&line[L2_ITEM_LENGTH], NULL, 0);
		printf("match item:    >%d<\n", track_data->item);
		break;
	    case L2_TEXT:
		track_data->text = &line[L2_TEXT_LENGTH];
		printf("match text:    >%s<\n", track_data->text);
		break;
	    case L2_STATE:
		track_data->state = strtoul(&line[L2_STATE_LENGTH], NULL, 0);
		printf("match state:   >%d<\n", track_data->state);
		break;
	    case L2_DEVICEID:
		track_data->deviceid = strtoul(&line[L2_DEVICEID_LENGTH], NULL, 0);
		printf("match deviceId:>%d<\n", track_data->deviceid);
		break;
	    default:
		printf("unknown:       >%s<\n", line);
		break;
	    }
	}
    }
    free(track_page);
    free(track_data);
    fclose(fp);
    return EXIT_SUCCESS;
}

int read_track_config(char *config_file) {
    int gbs_valid, l01_token_n, l2_token_n;
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
	line[strcspn(line, "\r\n")] = 0;
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
	    case L2_MAJOR:
		page->major = strtoul(&line[L2_MAJOR_LENGTH], NULL, 0);
		printf("match major:   >%d<\n", page->major);
		break;
	    case L2_MINOR:
		page->minor = strtoul(&line[L2_MINOR_LENGTH], NULL, 0);
		printf("match minor:   >%d<\n", page->minor);
		break;
	    case L2_XOFFSET:
		page->xoffset = strtoul(&line[L2_XOFFSET_LENGTH], NULL, 0);
		printf("match xoffset: >%d<\n", page->xoffset);
		break;
	    case L2_YOFFSET:
		page->yoffset = strtoul(&line[L2_YOFFSET_LENGTH], NULL, 0);
		printf("match yoffset: >%d<\n", page->yoffset);
		break;
	    case L2_WIDTH:
		page->width = strtoul(&line[L2_WIDTH_LENGTH], NULL, 0);
		printf("match width:   >%d<\n", page->width);
		break;
	    case L2_HEIGHT:
		page->height = strtoul(&line[L2_HEIGHT_LENGTH], NULL, 0);
		printf("match height:  >%d<\n", page->height);
		break;
	    case L2_NAME:
		if (gbs_valid) {
		    printf("match name:    >%s<  id %d\n", &line[L2_NAME_LENGTH], page->id);
		    add_track_page(page, &line[L2_NAME_LENGTH]);
		}
		break;
	    default:
		printf("unknown:       >%s<\n", line);
		break;
	    }
	}
    }
    free(page);
    fclose(fp);
    return EXIT_SUCCESS;
}

void read_track_pages(char *dir) {
    struct track_page_t *s;
    char *filename;

    for (s = track_page; s != NULL; s = s->hh.next) {
	asprintf(&filename, "%s/%s.cs2", dir, s->name);
	read_track_data(filename);
    }
}

int read_loco_data(char *config_file) {
    int l01_token_n, l2_token_n, loco_complete;
    FILE *fp;
    char line[MAXSIZE];
    struct loco_data_t *loco;

    loco_complete = 0;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    loco = calloc(1, sizeof(struct loco_data_t));

    while (fgets(line, MAXSIZE, fp) != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l01_token_n = get_char_index(l01_token, line);
	    if (l01_token_n == L1_LOCO) {
		/* TODO: next loco */
		if (loco_complete)
		    add_loco(loco, loco->name);
	    }

	} else {
	    l2_token_n = get_char_index2(l2_token, line);
	    switch (l2_token_n) {
	    case L2_ID:
		loco->id = strtoul(&line[L2_ID_LENGTH], NULL, 0);
		printf("match id:      >%d<\n", loco->id);
		break;
	    }
	}
    }
    free(loco);
    fclose(fp);
    return (EXIT_SUCCESS);
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
    read_track_pages(config_data.directory);

    return EXIT_SUCCESS;
}
