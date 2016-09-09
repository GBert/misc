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

#define FRAME_SIZE	13
#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

#define DEBUG		0

#define debug_print(...) \
            do { if (DEBUG) fprintf(stdin, ##__VA_ARGS__); } while (0)

#define check_modify(a, b) \
	    do { if ( a ) b = a; } while (0)

struct track_page_t *track_page = NULL;
struct track_data_t *track_data = NULL;
struct loco_data_t *loco_data = NULL;

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

void delete_all_track_pages() {
    struct track_page_t *cpage, *tmp;

    HASH_ITER(hh, track_page, cpage, tmp) {
	HASH_DEL(track_page, cpage);
	if (cpage->name)
	    free(cpage->name);
	free(cpage);
    }
}

void delete_all_track_data() {
    struct track_data_t *ctrack, *tmp;

    HASH_ITER(hh, track_data, ctrack, tmp) {
	HASH_DEL(track_data, ctrack);
	if (ctrack->text)
	    free(ctrack->text);
	free(ctrack);
    }
}

void delete_all_loco_data() {
    struct loco_data_t *cloco, *tmp;

    HASH_ITER(hh, loco_data, cloco, tmp) {
	HASH_DEL(loco_data, cloco);
	if (cloco->name)
	    free(cloco->name);
	if (cloco->type)
	    free(cloco->type);
	if (cloco->mfxAdr)
	    free(cloco->mfxAdr);
	free(cloco);
    }
}

int add_loco(struct loco_data_t *loco) {
    struct loco_data_t *l;

    HASH_FIND_INT(loco_data, &loco->uid, l);
    if (l == NULL) {
	if ((!loco->name) || (!loco->type))
	    return (EXIT_FAILURE);

	l = (struct loco_data_t *)calloc(1, sizeof(struct loco_data_t));
	if (!l) {
	    fprintf(stderr, "%s: can't calloc loco data: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	l->name = calloc(1, strlen(loco->name) + 1);
	if (!l->name) {
	    fprintf(stderr, "%s: can't calloc loco name: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(l->name, loco->name);

	l->type = calloc(1, strlen(loco->type) + 1);
	if (!l->type) {
	    fprintf(stderr, "%s: can't calloc protocol type: %s\n", __func__, strerror(errno));
	    return (EXIT_FAILURE);
	}
	strcpy(l->type, loco->type);

	l->uid = loco->uid;
	l->direction = loco->direction;
	l->velocity = loco->velocity;
	l->address = loco->address;
	l->sid = loco->sid;
	l->long_uid = loco->long_uid;
	l->address = loco->address;
	l->mfxuid = loco->mfxuid;
	l->acc_delay = loco->acc_delay;
	l->slow_down_delay = loco->slow_down_delay;
	l->volume = loco->volume;
	l->progmask = loco->progmask;
	l->vmax = loco->vmax;
	l->vmin = loco->vmin;
	l->mfxtype = loco->mfxtype;
	HASH_ADD_INT(loco_data, uid, l);
	/* TODO: mfx & function struct */
    } else {
	check_modify(loco->direction, l->direction);
	check_modify(loco->velocity, l->velocity);
	check_modify(loco->address, l->address);
	check_modify(loco->sid, l->sid);
	check_modify(loco->long_uid, l->long_uid);
	check_modify(loco->address, l->address);
	check_modify(loco->mfxuid, l->mfxuid);
	check_modify(loco->acc_delay, l->acc_delay);
	check_modify(loco->slow_down_delay, l->slow_down_delay);
	check_modify(loco->volume, l->volume);
	check_modify(loco->progmask, l->progmask);
	check_modify(loco->vmax, l->vmax);
	check_modify(loco->vmin, l->vmin);
	check_modify(loco->mfxtype, l->mfxtype);
	/* TODO: mfx & function struct */
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
	if (td->text) {
	    t->text = calloc(1, strlen(td->text) + 1);
	    if (!t->text) {
		fprintf(stderr, "%s: can't calloc track text: %s\n", __func__, strerror(errno));
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
	    if (t->text)
		free(t->text);
	    t->text = calloc(1, strlen(td->text) + 1);
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
	printf("page %2d: name %s\n", s->id, s->name);
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
	return (EXIT_FAILURE);
    }

    td = calloc(1, sizeof(struct track_data_t));
    if (!td) {
	fprintf(stderr, "%s: can't calloc track data: %s\n", __func__, strerror(errno));
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
	asprintf(&filename, "%s/%s.cs2", dir, s->name);
	read_track_data(filename);
	free(filename);
    }
}

int read_loco_data(char *config_file) {
    int l0_token_n, l1_token_n, l2_token_n, loco_complete;
    FILE *fp;
    char line[MAXSIZE];
    char *name, *type, *icon;
    int16_t function, temp;
    struct loco_data_t *loco;
    struct mfxAdr_t *mfx;

    function = -1;
    temp = -1;

    /* trigger for new entry */
    loco_complete = 0;

    if ((fp = fopen(config_file, "r")) == NULL) {
	fprintf(stderr, "can't open config file %s: %s\n", config_file, strerror(errno));
	return (EXIT_FAILURE);
    }

    loco = calloc(1, sizeof(struct loco_data_t));
    if (loco == NULL) {
	fprintf(stderr, "can't calloc buffer for loco data: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }

    loco->mfxAdr = calloc(1, sizeof(struct mfxAdr_t));
    if (loco->mfxAdr == NULL) {
	fprintf(stderr, "can't calloc buffer for loco mfx data: %s\n", strerror(errno));
	return (EXIT_FAILURE);
    }
    mfx = loco->mfxAdr;

    loco->name = NULL;

    while (fgets(line, MAXSIZE, fp) != NULL) {
	line[strcspn(line, "\r\n")] = 0;
	if (line[0] != ' ') {
	    l0_token_n = get_char_index(l0_token, line);
	    if (l0_token_n == L0_LOCO) {
		/* TODO: next loco */
		if (loco_complete) {
		    add_loco(loco);
		    memset(loco->mfxAdr, 0, sizeof(struct mfxAdr_t));
		    memset(loco, 0, sizeof(struct loco_data_t));
		    loco->mfxAdr = mfx;
		    free(name);
		    free(type);
		    free(icon);
		} else {
		    loco_complete = 1;
		}
	    } else {
		printf(">>%s\n", line);
	    }
	    /* Level 1 */
	} else if (line[2] != '.') {
	    l1_token_n = get_char_index(l1_token, line);
	    switch (l1_token_n) {
	    case L1_FUNCTION:
		break;
	    case L1_ID:
		loco->id = strtoul(&line[L1_ID_LENGTH], NULL, 10);
		printf("match id:        >%d<\n", loco->id);
		break;
	    case L1_MAJOR:
		loco->major = strtoul(&line[L1_MAJOR_LENGTH], NULL, 10);
		printf("match major:     >%d<\n", loco->major);
		break;
	    case L1_MINOR:
		loco->minor = strtoul(&line[L1_MINOR_LENGTH], NULL, 10);
		printf("match minor:     >%d<\n", loco->minor);
		break;
	    case L1_DIRECTION:
		loco->direction = strtoul(&line[L1_DIRECTION_LENGTH], NULL, 10);
		printf("match direction: >%d<\n", loco->direction);
		break;
	    case L1_VELOCITY:
		loco->velocity = strtoul(&line[L1_VELOCITY_LENGTH], NULL, 10);
		printf("match velocity:  >%d<\n", loco->velocity);
		break;
	    case L1_UID:
		loco->uid = strtoul(&line[L1_UID_LENGTH], NULL, 16);
		printf("match uid:       >0x%04x<\n", loco->uid);
		break;
	    case L1_NAME:
		asprintf(&name, "%s", &line[L1_NAME_LENGTH]);
		loco->name = name;
		printf("match name:      >%s<\n", loco->name);
		break;
	    case L1_TYPE:
		asprintf(&type, "%s", &line[L1_TYPE_LENGTH]);
		loco->type = type;
		printf("match type:      >%s<\n", loco->type);
		break;
	    case L1_SID:
		loco->sid = strtoul(&line[L1_SID_LENGTH], NULL, 16);
		printf("match sid:       >0x%x<\n", loco->sid);
		break;
	    case L1_MFXUID:
		loco->mfxuid = strtoul(&line[L1_MFXUID_LENGTH], NULL, 16);
		printf("match mfxuid:    >0x%08x<\n", loco->mfxuid);
		break;
	    case L1_SYMBOL:
		loco->symbol = strtoul(&line[L1_SYMBOL_LENGTH], NULL, 10);
		printf("match symbol:    >0%d<\n", loco->symbol);
		break;
	    case L1_ICON:
		asprintf(&icon, "%s", &line[L1_ICON_LENGTH]);
		loco->icon = icon;
		printf("match icon:      >%s<\n", loco->icon);
		break;
	    case L1_AV:
		loco->acc_delay = strtoul(&line[L1_AV_LENGTH], NULL, 10);
		printf("match av:        >%d<\n", loco->acc_delay);
		break;
	    case L1_BV:
		loco->slow_down_delay = strtoul(&line[L1_BV_LENGTH], NULL, 10);
		printf("match bv:        >%d<\n", loco->slow_down_delay);
		break;
	    case L1_VOLUME:
		loco->volume = strtoul(&line[L1_VOLUME_LENGTH], NULL, 10);
		printf("match volume:    >%d<\n", loco->volume);
		break;
	    case L1_PROGMASK:
		loco->progmask = strtoul(&line[L1_PROGMASK_LENGTH], NULL, 16);
		printf("match progmask:  >0x%02x<\n", loco->progmask);
		break;
	    case L1_VMIN:
		loco->vmin = strtoul(&line[L1_VMIN_LENGTH], NULL, 10);
		printf("match vmin:      >%d<\n", loco->vmin);
		break;
	    case L1_VMAX:
		loco->vmax = strtoul(&line[L1_VMAX_LENGTH], NULL, 10);
		printf("match vmax:      >%d<\n", loco->vmax);
		break;
	    case L1_TMAX:
		loco->tmax = strtoul(&line[L1_TMAX_LENGTH], NULL, 10);
		printf("match tmax:      >%d<\n", loco->tmax);
		break;
	    case L1_SPM:
		loco->spm = strtoul(&line[L1_SPM_LENGTH], NULL, 10);
		printf("match spm:       >%d<\n", loco->spm);
		break;
	    case L1_FT:
		loco->ft = strtoul(&line[L1_FT_LENGTH], NULL, 16);
		printf("match ft:        >0x%x<\n", loco->ft);
		break;
	    case L1_MFXTYPE:
		loco->mfxtype = strtoul(&line[L1_MFXTYPE_LENGTH], NULL, 10);
		printf("match mfxtype:   >%d<\n", loco->mfxtype);
		break;
	    case L1_ADDRESS:
		loco->address = strtoul(&line[L1_ADDRESS_LENGTH], NULL, 16);
		printf("match address:   >0x%x<\n", loco->address);
		break;
	    default:
		printf(">>%s<<\n", line);
		break;
	    }
	    /* Level 2 */
	} else {
	    l2_token_n = get_char_index(l2_token, line);
	    switch (l2_token_n) {
	    case L2_NUMBER:
		function = strtoul(&line[L2_NUMBER_LENGTH], NULL, 10) & 0x0f;
		break;
	    case L2_TYPE:
		if (function >= 0) {
		    temp = strtoul(&line[L2_TYPE_LENGTH], NULL, 10);
		    loco->function[function].type = temp;
		    printf(" loco function %2d type %3d\n", function, temp);
		}
		break;
	    case L2_DURATION:
		if (function >= 0) {
		    temp = strtoul(&line[L2_DURATION_LENGTH], NULL, 10);
		    loco->function[function].duration = temp;
		    printf(" loco function %2d duration %3d\n", function, temp);
		}
		break;
	    case L2_VALUE:
		if (function >= 0) {
		    temp = strtoul(&line[L2_VALUE_LENGTH], NULL, 10);
		    loco->function[function].value = temp;
		    printf(" loco function %2d value %d\n", function, temp);
		}
		break;
	    default:
		printf(">>%s<<\n", line);
		break;
	    }
	}
    }
    if (loco->uid)
	add_loco(loco);
    free(name);
    free(type);
    free(icon);
    free(mfx);
    free(loco);
    fclose(fp);
    return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    struct config_data_t config_data;
    char *dir, *var_dir;
    char *track_file;
    char *loco_file;

    var_dir = calloc(1, MAXDIR);
    if (var_dir == NULL) {
	fprintf(stderr, "can't alloc bufer for directory string: %s\n", strerror(errno));
	exit(EXIT_FAILURE);
    }

    if (argc == 2) {
	strncpy(var_dir, argv[1], MAXDIR - 1);
	config_data.directory = var_dir;
	asprintf(&dir, "%s", var_dir);
    } else {
	fprintf(stderr, "usage: %s <dir> \n", basename(argv[0]));
	free(var_dir);
	exit(EXIT_FAILURE);
    }

    config_data.verbose = 1;

    track_file = calloc(1, strlen(track_name) + strlen(config_data.directory) + 2);
    strcpy(track_file, config_data.directory);
    if (track_file[strlen(track_file) - 1] != '/')
	strcat(track_file, "/");
    strcat(track_file, track_name);

    /* printf("gbn : >%s<\n", track_file); */

    config_data.name = gbs_default;
    strcat(config_data.directory, track_dir);

    printf("reading track pages ...\n");
    read_track_config(track_file);
    sort_tp_by_id();
    print_pages();
    printf("reading track files ...\n");
    read_track_pages(config_data.directory);
    printf("sorting track data ...\n");
    sort_td_by_id();
    /* print_tracks(); */
    /* print_gbstats(); */

    asprintf(&loco_file, "%s/%s", dir, loco_name);
    read_loco_data(loco_file);

    printf("\n\ntrack pages: %u\n", HASH_COUNT(track_page));
    printf("track data elements: %u\n", HASH_COUNT(track_data));
    printf("loco data: %u\n", HASH_COUNT(loco_data));

    delete_all_track_pages();
    delete_all_track_data();
    delete_all_loco_data();

    free(loco_file);
    free(dir);
    free(var_dir);
    free(track_file);
    printf("sizeof(loco_data_t): %ld\n", sizeof(struct loco_data_t));
    printf("sizeof(track_data_t): %ld\n", sizeof(struct track_data_t));
    return EXIT_SUCCESS;
}
