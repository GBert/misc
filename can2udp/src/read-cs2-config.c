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

#include "read-cs2-config.h"
#include "cs2-token.h"

#define FRAME_SIZE	13
#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

#define DEBUG		0

#define debug_print(...) \
            do { if (DEBUG) fprintf(stdout, ##__VA_ARGS__); } while (0)

#define check_modify(a, b) \
	    do { if ( a ) b = a; } while (0)

#define check_free(a) \
	    do { if ( a ) free(a); } while (0)

char *track_dir   = { "/gleisbilder" };
char *track_name  = { "gleisbild.cs2" };
char *loco_name  = { "lokomotive.cs2" };
char *mags_name  = { "magnetartikel.cs2" };
char *auto_name  = { "fahrstrassen.cs2" };
char *gbs_default = { "gbs-0" };

struct track_page_t *track_page = NULL;
struct track_data_t *track_data = NULL;
struct loco_data_t *loco_data = NULL;

char *fgets_buffer(char *dest, int max, char *src) {

    if (*src == 0) {
	*dest = 0;
	return NULL;
    }
    do {
	*dest++ = *src++;
    } while ((*src != 0x0a) && (--max));
    *dest = 0;

    return ++src;
}

int strip_ms2_spaces(uint8_t *st, int len) {
    int i, index, nl;
    index = 0;
    nl = 0;

    /* deleting MS2 specific superfluos spaces after \n in a row
     * 20 2E 66 6B 74 0A 20 20   ' .fkt.  '  -> delete 20 20
     * 20 2E 2E 74 79 70 3D 30   ' ..typ=0'
     * 0A 20 20 20 20 20 20 20   '.       '  -> delete 20 20 20 20 20 20 20
     */

    for (i = 0; i < len; i++) {
	if ((i % 8) && nl)
	    continue;
	else
	    nl = 0;
	if (st[i] == 0x0a)
	    nl = 1;
	st[index++] = st[i];
    }
    st[index] = 0;
    return 0;
}

int get_value(char *st, char *search) {
    char line[MAXSIZE];
    char *sret;
    char *config = st;

    sret = fgets_buffer(line, MAXSIZE, config);
    while (sret != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (strncmp(line, search, strlen(line)) == 0)
	    return strtoul(&line[strlen(search)], NULL, 10) ;
	sret = fgets_buffer(line, MAXSIZE, config);
	config = sret;
    }
    return 0;
}

int get_char_index(const char **list, char *str) {
    int index;

    index = 0;

    while (*list[index]) {
	if (strstr(str, list[index]) == str)
	    return index;
	index++;
    }
    return -1;
}

void delete_all_track_pages(void) {
    struct track_page_t *cpage, *tmp;

    HASH_ITER(hh, track_page, cpage, tmp) {
	HASH_DEL(track_page, cpage);
	check_free(cpage->name);
	free(cpage);
    }
}

void delete_all_track_data(void) {
    struct track_data_t *ctrack, *tmp;

    HASH_ITER(hh, track_data, ctrack, tmp) {
	HASH_DEL(track_data, ctrack);
	check_free(ctrack->text);
	free(ctrack);
    }
}

void delete_all_loco_data(void) {
    struct loco_data_t *cloco, *tmp;

    HASH_ITER(hh, loco_data, cloco, tmp) {
	HASH_DEL(loco_data, cloco);
	check_free(cloco->name);
	check_free(cloco->type);
	check_free(cloco->mfxAdr);
	free(cloco);
    }
}

int add_loco(struct loco_data_t *loco, int incomplete) {
    struct loco_data_t *l;

    HASH_FIND_STR(loco_data, loco->name, l);


    if (l == NULL) {
	/* if ((!loco->name) || (!loco->type))
	    return (EXIT_FAILURE); */
	if (!loco->name)
	    return (EXIT_FAILURE);

	l = (struct loco_data_t *)calloc(1, sizeof(struct loco_data_t));
	if (!l) {
	    fprintf(stderr, "%s: can't calloc loco data: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	l->name = calloc(strlen(loco->name) + 1, 1);
	if (!l->name) {
	    fprintf(stderr, "%s: can't calloc loco name: %s\n", __func__, strerror(errno));
	    free(l);
	    return (EXIT_FAILURE);
	}
	strcpy(l->name, loco->name);

	if (loco->icon) {
	    l->icon = calloc(strlen(loco->icon) + 1, 1);
	    if (!l->icon) {
		fprintf(stderr, "%s: can't calloc loco icon string: %s\n", __func__, strerror(errno));
		free(l);
		return (EXIT_FAILURE);
	    }
	    strcpy(l->icon, loco->icon);
	}

	l->mfxAdr = calloc(1, sizeof(struct mfxAdr_t));
	if (!l->mfxAdr) {
	    fprintf(stderr, "can't calloc buffer for loco mfx data: %s\n", strerror(errno));
	    return (EXIT_FAILURE);
	}

	if (loco->type) {
	    l->type = calloc(strlen(loco->type) + 1, 1);
	    if (!l->type) {
		fprintf(stderr, "%s: can't calloc protocol type: %s\n", __func__, strerror(errno));
		free(l);
		return (EXIT_FAILURE);
	    }
	    strcpy(l->type, loco->type);
	}

	l->minor = loco->minor;
	l->major = loco->major;
	l->id = loco->id;
	l->uid = loco->uid;
	l->direction = loco->direction;
	l->number = loco->number;
	l->velocity = loco->velocity;
	l->address = loco->address;
	l->sid = loco->sid;
	l->symbol = loco->symbol;
	l->long_uid = loco->long_uid;
	l->address = loco->address;
	l->mfxuid = loco->mfxuid;
	l->acc_delay = loco->acc_delay;
	l->slow_down_delay = loco->slow_down_delay;
	l->volume = loco->volume;
	l->progmask = loco->progmask;
	l->tmax = loco->tmax;
	l->vmax = loco->vmax;
	l->vmin = loco->vmin;
	l->xprot = loco->xprot;
	l->spm = loco->spm;
	l->ft = loco->ft;
	l->mfxtype = loco->mfxtype;
	l->intraction = loco->intraction;
	HASH_ADD_STR(loco_data, name, l);
    } else {
	check_modify(loco->minor, l->minor);
	check_modify(loco->major, l->major);
	check_modify(loco->id, l->id);
	check_modify(loco->direction, l->direction);
	check_modify(loco->velocity, l->velocity);
	check_modify(loco->address, l->address);
	check_modify(loco->sid, l->sid);
	check_modify(loco->symbol, l->symbol);
	check_modify(loco->long_uid, l->long_uid);
	check_modify(loco->address, l->address);
	check_modify(loco->mfxuid, l->mfxuid);
	check_modify(loco->acc_delay, l->acc_delay);
	check_modify(loco->slow_down_delay, l->slow_down_delay);
	check_modify(loco->volume, l->volume);
	check_modify(loco->progmask, l->progmask);
	check_modify(loco->tmax, l->tmax);
	check_modify(loco->vmax, l->vmax);
	check_modify(loco->vmin, l->vmin);
	check_modify(loco->spm, l->spm);
	check_modify(loco->xprot, l->xprot);
	check_modify(loco->ft, l->ft);
	check_modify(loco->mfxtype, l->mfxtype);
	check_modify(loco->intraction, l->intraction);
    }
    l->incomplete = incomplete;
    memcpy(l->function, loco->function, sizeof(l->function));

    memcpy(l->mfxAdr, loco->mfxAdr, sizeof(struct mfxAdr_t));

    return (EXIT_SUCCESS);
}

int get_loco_max(void) {
    struct loco_data_t *l;
    int loco_max = -1;

    for (l = loco_data; l != NULL; l = l->hh.next) {
	if (l->number > loco_max)
	    loco_max = l->number;
    }
    return(loco_max);
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
	if (td->text) {
	    t->text = calloc(strlen(td->text) + 1, 1);
	    if (!t->text) {
		fprintf(stderr, "%s: can't calloc track text: %s\n", __func__, strerror(errno));
		free(t);
		return (EXIT_FAILURE);
	    }
	    strcpy(t->text, td->text);
	}
	t->id = td->id;
	t->type = td->type;
	t->rotation = td->rotation;
	t->item = td->item;
	t->state = td->state;
	/* TODO: side ??? */
	HASH_ADD_INT(track_data, id, t);	/* id: name of key field */
    } else {
	check_modify(td->type, t->type);
	check_modify(td->rotation, t->rotation);
	check_modify(td->item, t->item);
	check_modify(td->state, t->state);
    }

    /* does text exists ? */
    if (td->text) {
	/* the extra check for t->text seems to be needed */
	if ((t->text) && (strcmp(t->text, td->text))) {
	    check_free(t->text);
	    t->text = calloc(strlen(td->text) + 1, 1);
	    if (!t->text) {
		fprintf(stderr, "%s: can't calloc track text: %s\n", __func__, strerror(errno));
		return (EXIT_FAILURE);
	    }
	    strcpy(t->text, td->text);
	}
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
	t->name = calloc(strlen(name) + 1, 1);
	if (!t->name) {
	    fprintf(stderr, "%s: can't calloc track page name: %s\n", __func__, strerror(errno));
	    free(t);
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
    } else {
	check_modify(page->xoffset, t->xoffset);
	check_modify(page->yoffset, t->yoffset);
	check_modify(page->major, t->major);
	check_modify(page->minor, t->minor);
	check_modify(page->width, t->width);
	check_modify(page->height, t->height);
    }
    return (EXIT_SUCCESS);
}

int td_id_sort(struct track_data_t *a, struct track_data_t *b) {
    return (a->id - b->id);
}

int tp_id_sort(struct track_page_t *a, struct track_page_t *b) {
    return (a->id - b->id);
}

void sort_tp_by_id(void) {
    HASH_SORT(track_page, tp_id_sort);
}

void sort_td_by_id(void) {
    HASH_SORT(track_data, td_id_sort);
}

void print_pages(void) {
    struct track_page_t *s;

    for (s = track_page; s != NULL; s = s->hh.next) {
	printf("page %2u: name %s\n", s->id, s->name);
    }
}

void print_gbstats(void) {
    struct track_data_t *t;

    printf("[gleisbild]\n");
    printf("version\n");
    printf(" .major=1\n");

    for (t = track_data; t != NULL; t = t->hh.next) {
	printf("element\n");
	printf(" .id=0x%x\n", t->id);
	if (t->state)
	    printf(" .zustand=%d\n", t->state);
    }
}

void print_tracks(void) {
    struct track_data_t *t;

    for (t = track_data; t != NULL; t = t->hh.next) {
	printf("element\n");
	printf(" .id=0x%05x\n", t->id);
	printf(" .artikel=%d\n", t->item);
    }
}

void print_locos(FILE *file) {
    int i;
    struct loco_data_t *l;

    l = loco_data;
    fprintf(file, "[lokomotive]\n");
    fprintf(file, "version\n");
    if (l->major) fprintf(file, " .major=%d\n", l->major);
    fprintf(file, " .minor=%d\n", l->minor);
    fprintf(file, "session\n");
    fprintf(file, " .id=%d\n", l->id);

    for (l = loco_data; l != NULL; l = l->hh.next) {
	fprintf(file, "lokomotive\n");
	fprintf(file, " .name=%s\n", l->name);
	if (l->direction) fprintf(file, " .richtung=%d\n", l->direction);
	fprintf(file, " .uid=0x%x\n", l->uid);
	fprintf(file, " .adresse=0x%x\n", l->address);
	fprintf(file, " .typ=%s\n", l->type);
	if (l->sid)	fprintf(file, " .sid=0x%x\n", l->sid);
	if (l->mfxuid)	fprintf(file, " .mfxuid=0x%x\n", l->mfxuid);
	if (l->icon)	fprintf(file, " .icon=%s\n", l->icon);
	if (l->symbol)	fprintf(file, " .symbol=%d\n", l->symbol);
	if (l->acc_delay) fprintf(file, " .av=%d\n", l->acc_delay);
	if (l->slow_down_delay) fprintf(file, " .bv=%d\n", l->slow_down_delay);
	if (l->volume)	fprintf(file, " .volume=%d\n", l->volume);
	if (l->progmask) fprintf(file, " .progmask=0x%x\n", l->progmask);
	fprintf(file, " .tachomax=%d\n", l->tmax);
	if (l->vmax)	fprintf(file, " .vmax=%d\n", l->vmax);
	if (l->vmin)	fprintf(file, " .vmin=%d\n", l->vmin);
	if (l->xprot)	fprintf(file, " .xprot=%d\n", l->xprot);
	if (l->spm)	fprintf(file, " .spm=%d\n", l->spm);
	if (l->ft)	fprintf(file, " .ft=0x%x\n", l->ft);
	if (l->mfxtype)	fprintf(file, " .mfxtyp=%d\n", l->mfxtype);
	for (i=0; i < MAX_LOCO_FUNCTIONS; i++) {
	    fprintf(file, " .funktionen\n");
	    fprintf(file, " ..nr=%d\n", i);
	    if (l->function[i].type) fprintf(file, " ..typ=%d\n", l->function[i].type);
	    if (l->function[i].value) fprintf(file, " ..wert=%d\n", l->function[i].value);
	    if (l->function[i].duration) fprintf(file, " ..dauer=%d\n", l->function[i].duration);
	    if (l->function[i].forward) fprintf(file, " ..vorwaerts=0x%x\n", l->function[i].forward);
	    if (l->function[i].backward) fprintf(file, " ..rueckwaerts=0x%x\n", l->function[i].backward);
	}
	if (l->intraction) fprintf(file, " .inTraktion=0x%08x\n", l->intraction);
    }
}

int read_track_data(char *config_file) {
    int l0_token_n, l1_token_n, element;
    FILE *fp;
    char line[MAXSIZE];
    struct track_page_t *tp;
    struct track_data_t *td;

    /* trigger for data save */
    element = 0;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "%s: can't open track file %s: %s\n", __func__, config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    tp = calloc(1, sizeof(struct track_page_t));
    if (!tp) {
	fprintf(stderr, "%s: can't calloc track page: %s\n", __func__, strerror(errno));
	fclose(fp);
	return (EXIT_FAILURE);
    }

    td = calloc(1, sizeof(struct track_data_t));
    if (!td) {
	fprintf(stderr, "%s: can't calloc track data: %s\n", __func__, strerror(errno));
	free(tp);
	fclose(fp);
	return (EXIT_FAILURE);
    }

    while (fgets(line, MAXSIZE, fp) != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l0_token_n = get_char_index(l0_token, line);
	    switch (l0_token_n) {
	    case L0_PAGE:
		tp->id = 0;
		debug_print("match seite:   >%s<\n", line);
		break;
	    case L0_ELEMENT:
		if (element) {
		    add_track_data(td);
		    memset(td, 0, sizeof(struct track_data_t));
		}
		element = 1;
		break;
	    }
	} else {
	    l1_token_n = get_char_index(l1_token, line);
	    switch (l1_token_n) {
	    case L1_MAJOR:
		tp->major = strtoul(&line[L1_MAJOR_LENGTH], NULL, 10);
		debug_print("match major:   >%d<\n", tp->major);
		break;
	    case L1_MINOR:
		tp->minor = strtoul(&line[L1_MINOR_LENGTH], NULL, 10);
		debug_print("match minor:   >%d<\n", tp->minor);
		break;
	    case L1_XOFFSET:
		tp->xoffset = strtoul(&line[L1_XOFFSET_LENGTH], NULL, 10);
		debug_print("match xoffset: >%d<\n", tp->xoffset);
		break;
	    case L1_YOFFSET:
		tp->yoffset = strtoul(&line[L1_YOFFSET_LENGTH], NULL, 10);
		debug_print("match yoffset: >%d<\n", tp->yoffset);
		break;
	    case L1_WIDTH:
		tp->width = strtoul(&line[L1_WIDTH_LENGTH], NULL, 10);
		debug_print("match width:   >%d<\n", tp->width);
		break;
	    case L1_HEIGHT:
		tp->height = strtoul(&line[L1_HEIGHT_LENGTH], NULL, 10);
		debug_print("match height:  >%d<\n", tp->height);
		break;
	    case L1_ID:
		td->id = strtoul(&line[L1_ID_LENGTH], NULL, 16);
		debug_print("match id:      >0x%05x<\n", td->id);
		break;
	    case L1_TYPE:
		td->type = get_char_index(track_types, &line[L1_TYPE_LENGTH]);
		debug_print("match type:    >%d<\n", td->type);
		break;
	    case L1_ROTATION:
		td->rotation = strtoul(&line[L1_ROTATION_LENGTH], NULL, 10);
		debug_print("match rotation:>%d<\n", td->rotation);
		break;
	    case L1_ITEM:
		td->item = strtoul(&line[L1_ITEM_LENGTH], NULL, 10);
		debug_print("match item:    >%d<\n", td->item);
		break;
	    case L1_TEXT:
		td->text = &line[L1_TEXT_LENGTH];
		debug_print("match text:    >%s<\n", td->text);
		break;
	    case L1_STATE:
		td->state = strtoul(&line[L1_STATE_LENGTH], NULL, 10);
		debug_print("match state:   >%d<\n", td->state);
		break;
	    case L1_DEVICEID:
		td->deviceid = strtoul(&line[L1_DEVICEID_LENGTH], NULL, 10);
		debug_print("match deviceId:>%d<\n", td->deviceid);
		break;
	    default:
		fprintf(stderr, "unknown:       >%s<\n", line);
		break;
	    }
	}
    }
    /* store last entry if any */
    if (td->id)
	add_track_data(td);

    free(tp);
    free(td);
    fclose(fp);
    return EXIT_SUCCESS;
}

int read_track_config(char *config_file) {
    int gbs_valid, l0_token_n, l1_token_n;
    FILE *fp;
    char line[MAXSIZE];
    struct track_page_t *page;

    gbs_valid = 0;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    page = calloc(1, sizeof(struct track_page_t));
    if (page == NULL) {
	fprintf(stderr, "can't calloc bufer for track pages: %s\n", strerror(errno));
	fclose(fp);
	return (EXIT_FAILURE);
    }

    while (fgets(line, MAXSIZE, fp) != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l0_token_n = get_char_index(l0_token, line);
	    if (l0_token_n == L0_PAGE) {
		gbs_valid = 1;
		page->id = 0;
		debug_print("match seite:   >%s<\n", line);
	    }
	} else {
	    l1_token_n = get_char_index(l1_token, line);
	    switch (l1_token_n) {
	    case L1_ID:
		page->id = strtoul(&line[L1_ID_LENGTH], NULL, 10);
		debug_print("match id:      >%d<\n", page->id);
		break;
	    case L1_MAJOR:
		page->major = strtoul(&line[L1_MAJOR_LENGTH], NULL, 10);
		debug_print("match major:   >%d<\n", page->major);
		break;
	    case L1_MINOR:
		page->minor = strtoul(&line[L1_MINOR_LENGTH], NULL, 10);
		debug_print("match minor:   >%d<\n", page->minor);
		break;
	    case L1_XOFFSET:
		page->xoffset = strtoul(&line[L1_XOFFSET_LENGTH], NULL, 10);
		debug_print("match xoffset: >%d<\n", page->xoffset);
		break;
	    case L1_YOFFSET:
		page->yoffset = strtoul(&line[L1_YOFFSET_LENGTH], NULL, 10);
		debug_print("match yoffset: >%d<\n", page->yoffset);
		break;
	    case L1_WIDTH:
		page->width = strtoul(&line[L1_WIDTH_LENGTH], NULL, 10);
		debug_print("match width:   >%d<\n", page->width);
		break;
	    case L1_HEIGHT:
		page->height = strtoul(&line[L1_HEIGHT_LENGTH], NULL, 10);
		debug_print("match height:  >%d<\n", page->height);
		break;
	    case L1_NAME:
		if (gbs_valid) {
		    debug_print("match name:    >%s<  id %d\n", &line[L1_NAME_LENGTH], page->id);
		    add_track_page(page, &line[L1_NAME_LENGTH]);
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
	if (asprintf(&filename, "%s/%s.cs2", dir, s->name) < 0)
	    fprintf(stderr, "can't alloc memory for filename: %s\n", __func__);
	read_track_data(filename);
	free(filename);
    }
}

int read_loco_data(char *config_file, int incomplete, int config_type) {
    int l0_token_n, l1_token_n, l2_token_n, loco_complete;
    FILE *fp = NULL;
    char line[MAXSIZE];
    char *name = NULL, *type =NULL, *icon = NULL, *sret = NULL;
    int16_t function, temp, mfx_data;
    struct loco_data_t *loco;
    struct mfxAdr_t *mfx;

    function = -1;
    mfx_data = -1;
    temp = -1;

    /* trigger for new entry */
    loco_complete = 0;

    if (config_type && CONFIG_FILE) {
	if ((fp = fopen(config_file, "r")) == NULL) {
	    fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	    return (EXIT_FAILURE);
	}
    }

    loco = calloc(1, sizeof(struct loco_data_t));
    if (loco == NULL) {
	fprintf(stderr, "can't calloc buffer for loco data: %s\n", strerror(errno));
	fclose(fp);
	return (EXIT_FAILURE);
    }

    loco->mfxAdr = calloc(1, sizeof(struct mfxAdr_t));
    if (loco->mfxAdr == NULL) {
	fprintf(stderr, "can't calloc buffer for loco mfx data: %s\n", strerror(errno));
	free(loco);
	fclose(fp);
	return (EXIT_FAILURE);
    }
    mfx = loco->mfxAdr;

    loco->name = NULL;
    loco->type = NULL;
    loco->icon = NULL;


    if (config_type & CONFIG_FILE) {
	sret = fgets(line, MAXSIZE, fp);
    } else {
	sret = fgets_buffer(line, MAXSIZE, config_file);
        config_file = sret;
    }

    while (sret != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l0_token_n = get_char_index(l0_token, line);
	    switch (l0_token_n) {
	    case L00_LOCO:
	    case L00_LOCO_NUMBER:
	    case L0_VERSION:
	    case L0_SESSION:
		break;
	    case L00_LOCO_SHORT:
	    case L0_LOC:
	    case L0_LOCO:
		/* TODO: next loco */
		if (loco_complete) {
		    add_loco(loco, incomplete);
		    memset(loco->mfxAdr, 0, sizeof(struct mfxAdr_t));
		    memset(loco, 0, sizeof(struct loco_data_t));
		    loco->mfxAdr = mfx;
		    check_free(name);
		    check_free(type);
		    /* check_free(icon); */
		} else {
		    loco_complete = 1;
		}
		break;
	    default:
		printf(">>%s\n", line);
		break;
	    }
	    /* Level 1 */
	} else if (line[2] != '.') {
	    l1_token_n = get_char_index(l1_token, line);
	    switch (l1_token_n) {
	    case L1_FCT:
		mfx_data = -1;
		function++;
		break;
	    case L1_FUNCTION:
		mfx_data = -1;
		break;
	    case L1_MFXADR:
		mfx_data = 0;
		break;
	    case L1_ID:
		loco->id = strtoul(&line[L1_ID_LENGTH], NULL, 10);
		debug_print("match id:        >%d<\n", loco->id);
		break;
	    case L1_MAJOR:
		loco->major = strtoul(&line[L1_MAJOR_LENGTH], NULL, 10);
		debug_print("match major:     >%d<\n", loco->major);
		break;
	    case L1_MINOR:
		loco->minor = strtoul(&line[L1_MINOR_LENGTH], NULL, 10);
		debug_print("match minor:     >%d<\n", loco->minor);
		break;
	    case L1_DIRECTION:
		loco->direction = strtoul(&line[L1_DIRECTION_LENGTH], NULL, 10);
		debug_print("match direction: >%d<\n", loco->direction);
		break;
	    case L1_VALUE:
		loco->number = strtoul(&line[L1_VALUE_LENGTH], NULL, 10);
		debug_print("match value:  >%d<\n", loco->number);
		break;
	    case L1_VELOCITY:
		loco->velocity = strtoul(&line[L1_VELOCITY_LENGTH], NULL, 10);
		debug_print("match velocity:  >%d<\n", loco->velocity);
		break;
	    case L1_UID:
		loco->uid = strtoul(&line[L1_UID_LENGTH], NULL, 16);
		debug_print("match uid:       >0x%04x<\n", loco->uid);
		break;
	    case L1_NAME:
		if (asprintf(&name, "%s", &line[L1_NAME_LENGTH]) < 0)
		    fprintf(stderr, "can't alloc memory for loco->name: %s\n", __func__);
		function = -1;
		loco->name = name;
		debug_print("match name:      >%s<\n", loco->name);
		break;
	    case L1_TYPE:
		if (asprintf(&type, "%s", &line[L1_TYPE_LENGTH]) < 0)
		    fprintf(stderr, "can't alloc memory for loco->type: %s\n", __func__);
		loco->type = type;
		debug_print("match type:      >%s<\n", loco->type);
		break;
	    case L1_ICON:
		if (asprintf(&icon, "%s", &line[L1_ICON_LENGTH]) < 0)
		    fprintf(stderr, "can't alloc memory for loco->icon: %s\n", __func__);
		loco->icon = icon;
		debug_print("match icon:      >%s<\n", loco->icon);
		break;
	    case L1_SID:
		loco->sid = strtoul(&line[L1_SID_LENGTH], NULL, 16);
		debug_print("match sid:       >0x%x<\n", loco->sid);
		break;
	    case L1_MFXUID:
		loco->mfxuid = strtoul(&line[L1_MFXUID_LENGTH], NULL, 16);
		debug_print("match mfxuid:    >0x%08x<\n", loco->mfxuid);
		break;
	    case L1_SYMBOL:
		loco->symbol = strtoul(&line[L1_SYMBOL_LENGTH], NULL, 10);
		debug_print("match symbol:    >%d<\n", loco->symbol);
		break;
	    case L1_AV:
		loco->acc_delay = strtoul(&line[L1_AV_LENGTH], NULL, 10);
		debug_print("match av:        >%d<\n", loco->acc_delay);
		break;
	    case L1_BV:
		loco->slow_down_delay = strtoul(&line[L1_BV_LENGTH], NULL, 10);
		debug_print("match bv:        >%d<\n", loco->slow_down_delay);
		break;
	    case L1_VOLUME:
		loco->volume = strtoul(&line[L1_VOLUME_LENGTH], NULL, 10);
		debug_print("match volume:    >%d<\n", loco->volume);
		break;
	    case L1_PROGMASK:
		loco->progmask = strtoul(&line[L1_PROGMASK_LENGTH], NULL, 16);
		debug_print("match progmask:  >0x%02x<\n", loco->progmask);
		break;
	    case L1_VMIN:
		loco->vmin = strtoul(&line[L1_VMIN_LENGTH], NULL, 10);
		debug_print("match vmin:      >%d<\n", loco->vmin);
		break;
	    case L1_VMAX:
		loco->vmax = strtoul(&line[L1_VMAX_LENGTH], NULL, 10);
		debug_print("match vmax:      >%d<\n", loco->vmax);
		break;
	    case L1_XPROT:
		loco->xprot = strtoul(&line[L1_XPROT_LENGTH], NULL, 10);
		debug_print("match xprot:     >%d<\n", loco->xprot);
		break;
	    case L1_TMAX:
		loco->tmax = strtoul(&line[L1_TMAX_LENGTH], NULL, 10);
		debug_print("match tmax:      >%d<\n", loco->tmax);
		break;
	    case L1_SPM:
		loco->spm = strtoul(&line[L1_SPM_LENGTH], NULL, 10);
		debug_print("match spm:       >%d<\n", loco->spm);
		break;
	    case L1_FT:
		loco->ft = strtoul(&line[L1_FT_LENGTH], NULL, 16);
		debug_print("match ft:        >0x%x<\n", loco->ft);
		break;
	    case L1_MFXTYPE:
		loco->mfxtype = strtoul(&line[L1_MFXTYPE_LENGTH], NULL, 10);
		debug_print("match mfxtype:   >%d<\n", loco->mfxtype);
		break;
	    case L1_ADDRESS:
		loco->address = strtoul(&line[L1_ADDRESS_LENGTH], NULL, 16);
		debug_print("match address:   >0x%x<\n", loco->address);
		break;
	    case L1_INTRACTION:
		loco->intraction = strtoul(&line[L1_INTRACTION_LENGTH], NULL, 16);
		debug_print("match intraction:  >0x%08x<\n", loco->intraction);
		break;
	    default:
		printf(">>%s<<\n", line);
		break;
	    }
	    /* Level 2 */
	} else {
	    l2_token_n = get_char_index(l2_token, line);
	    if (mfx_data >= 0)
		    debug_print(" mfx data");
	    switch (l2_token_n) {
	    /* TODO function value check */
	    /* function token */
	    case L2_NUMBER:
		function = strtoul(&line[L2_NUMBER_LENGTH], NULL, 10) & 0x1f;
		break;
	    case L2_TYPE:
		if (function >= 0) {
		    temp = strtoul(&line[L2_TYPE_LENGTH], NULL, 10);
		    loco->function[function].type = temp;
		    debug_print(" loco function %2d type %3d\n", function, temp);
		}
		break;
	    case L2_DURATION:
		if (function >= 0) {
		    temp = strtoul(&line[L2_DURATION_LENGTH], NULL, 10);
		    loco->function[function].duration = temp;
		    debug_print(" loco function %2d duration %3d\n", function, temp);
		}
		break;
	    case L2_VALUE:
		if (function >= 0) {
		    temp = strtoul(&line[L2_VALUE_LENGTH], NULL, 10);
		    loco->function[function].value = temp;
		    debug_print(" loco function %2d value %d\n", function, temp);
		}
		break;
	    case L2_FORWARD:
		if (function >= 0) {
		    temp = strtoul(&line[L2_FOWARD_LENGTH], NULL, 10);
		    loco->function[function].forward = temp;
		    debug_print(" loco function %2d forward 0x%0x\n", function, temp);
		}
		break;
	    case L2_BACKWARD:
		if (function >= 0) {
		    temp = strtoul(&line[L2_BACKWARD_LENGTH], NULL, 10);
		    loco->function[function].backward = temp;
		    debug_print(" loco function %2d backward 0x%0x\n", function, temp);
		}
		break;
	    /* mfxAdr token */
	    case L2_TARGET:
		if (mfx_data >= 0) {
		    loco->mfxAdr->target = strtoul(&line[L2_TARGET_LENGTH], NULL, 10);
		    debug_print(" mfxAdr target %d\n", loco->mfxAdr->target);
		}
		break;
	    case L2_NAME:
		if (mfx_data >= 0) {
		    loco->mfxAdr->name = strtoul(&line[L2_NAME_LENGTH], NULL, 10);
		    debug_print(" mfxAdr name %d\n", loco->mfxAdr->name);
		}
		break;
	    case L2_ADDRESS:
		if (mfx_data >= 0) {
		    loco->mfxAdr->address = strtoul(&line[L2_ADDRESS_LENGTH], NULL, 10);
		    debug_print(" mfxAdr addr %d\n", loco->mfxAdr->address);
		}
		break;
	    case L2_XCEL:
		if (mfx_data >= 0) {
		    loco->mfxAdr->xcel = strtoul(&line[L2_XCEL_LENGTH], NULL, 10);
		    debug_print(" mfxAdr xcel %d\n", loco->mfxAdr->xcel);
		}
		break;
	    case L2_SPEEDTABLE:
		if (mfx_data >= 0) {
		    loco->mfxAdr->speedtable = strtoul(&line[L2_SPEEDTABLE_LENGTH], NULL, 10);
		    debug_print(" mfxAdr speedtable %d\n", loco->mfxAdr->speedtable);
		}
		break;
	    case L2_VOLUME:
		if (mfx_data >= 0) {
		    loco->mfxAdr->volume = strtoul(&line[L2_VOLUME_LENGTH], NULL, 10);
		    debug_print(" mfxAdr voulume %d\n", loco->mfxAdr->volume);
		}
		break;
	    case L2_NUMFUNCTION:
		if (mfx_data >= 0) {
		    loco->mfxAdr->numfunction = strtoul(&line[L2_NUMFUNCTION_LENGTH], NULL, 10);
		    debug_print(" mfxAdr numfunction %d\n", loco->mfxAdr->numfunction);
		}
		break;
	    case L2_FUNCTION:
		if (mfx_data >= 0) {
		    loco->mfxAdr->function = strtoul(&line[L2_FUNCTION_LENGTH], NULL, 10);
		    debug_print(" mfxAdr func %d\n", loco->mfxAdr->function);
		}
		break;
	    default:
		printf(">>%s<<\n", line);
		break;
	    }
	}
	if (config_type & CONFIG_FILE) {
	    sret = fgets(line, MAXSIZE, fp);
	} else {
	    sret = fgets_buffer(line, MAXSIZE, config_file);
	    config_file = sret;
	}
    }
    if (loco->name)
	add_loco(loco, incomplete);
    if (name)
	free(name);
    if (type)
	free(type);
    if (icon)
	free(icon);
    free(mfx);
    free(loco);
    if (config_type && CONFIG_FILE)
	fclose(fp);

    printf("loco data count: %u\n", HASH_COUNT(loco_data));
    return (EXIT_SUCCESS);
}
