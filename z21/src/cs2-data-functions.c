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
#include "z21.h"

extern struct track_page_t *track_page;
extern struct track_data_t *track_data;
extern struct loco_data_t *loco_data, *loco_data_by_uid;
extern struct loco_names_t *loco_names;
extern struct magnet_data_t *magnet_data;

struct loco_data_t *create_loco(unsigned int uid) {
    struct loco_data_t *l;

    l = (struct loco_data_t *)calloc(1, sizeof(struct loco_data_t));
    if (!l) {
	fprintf(stderr, "%s: can't calloc loco data: %s\n", __func__, strerror(errno));
	return 0;
    }
    if (uid > 0xC000)
	asprintf(&l->name, "Dummy DCC%d", uid);
    else if (uid > 0x4000)
	asprintf(&l->name, "Dummy MFX%d", uid);
    else
	asprintf(&l->name, "Dummy MM%d", uid);
    l->uid = uid;

    HASH_ADD_STR(loco_data, name, l);
    HASH_ADD(hha, loco_data_by_uid, uid, sizeof(int), l);

    v_printf(" Loco added: %s %d\n", l->name, uid);

    return(l);
}


uint8_t loco_get_direction(unsigned int uid) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->direction);
    else
	create_loco(uid);
    return(EXIT_SUCCESS);
}

int loco_save_direction(unsigned int uid, uint8_t direction) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (!l)
	l = create_loco(uid);
    l->direction = direction;
    return(EXIT_SUCCESS);
}

uint8_t loco_get_function(unsigned int uid, uint8_t function) {
    struct loco_data_t *l;

    function &= MAX_LOCO_FUNCTIONS - 1;
    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->function[function].value);
    else
	create_loco(uid);
    return(0);
}

unsigned int loco_get_func_summary(unsigned int uid) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->loco_func_summary);
    return(0);
}

int loco_save_function(unsigned int uid, uint8_t function, uint8_t value) {
    struct loco_data_t *l;

    function &= MAX_LOCO_FUNCTIONS - 1;
    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l) {
	if (value)
	    l->loco_func_summary |= 1 << function;
	else
	    l->loco_func_summary &= ~(1 << function);
	l->function[function].value = value;
	return(EXIT_SUCCESS);
    }
    return(EXIT_FAILURE);
}

int loco_toggle_function(unsigned int uid, uint8_t function) {
    struct loco_data_t *l;

    function &= MAX_LOCO_FUNCTIONS - 1;
    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l) {
	l->loco_func_summary ^= (1 << function);
	l->function[function].value ^= 1;
	return(EXIT_SUCCESS);
    }
    return(EXIT_FAILURE);
}

unsigned int loco_get_speed(unsigned int uid) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->velocity);
    return(0);
}

int loco_save_speed(unsigned int uid, unsigned int speed) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l) {
	l->velocity = speed;
	return(EXIT_SUCCESS);
    }
    return(EXIT_FAILURE);
}

void print_locos_by_uid(void) {
    struct loco_data_t *l, *tmp;

    HASH_ITER(hha, loco_data_by_uid, l, tmp) {
	printf("0x%08X %s\n", l->uid, l->name);
    }
}

unsigned int get_magnet_type(unsigned int id) {
    struct magnet_data_t *m;
    HASH_FIND(hh, magnet_data, &id, sizeof(unsigned int), m);
    if (m) {
	/* TODO mm2 -> 0 */
	if (m->decoder_type == 0)
	    return(0x3000);
    }
    /* return DCC */
    return(0x3800);
}
