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

extern struct track_page_t *track_page;
extern struct track_data_t *track_data;
extern struct loco_data_t *loco_data, *loco_data_by_uid;
extern struct loco_names_t *loco_names;

uint8_t loco_get_direction(unsigned int uid) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->direction);
    return(255);
}

int loco_set_direction(unsigned int uid, uint8_t direction) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l) {
	l->direction = direction;
	return(EXIT_SUCCESS);
    }
    return(EXIT_FAILURE);
}

uint8_t loco_get_function(unsigned int uid, uint8_t function) {
    struct loco_data_t *l;

    function &= MAX_LOCO_FUNCTIONS - 1;
    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->function[function].value);
    return(0);
}

unsigned int loco_get_func_summary(unsigned int uid) {
    struct loco_data_t *l;

    HASH_FIND(hha, loco_data_by_uid, &uid, sizeof(unsigned int), l);
    if (l)
	return(l->loco_func_summary);
    return(0);
}

int loco_set_function(unsigned int uid, uint8_t function, uint8_t value) {
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

void print_locos_by_uid(void) {
    struct loco_data_t *l, *tmp;

    HASH_ITER(hha, loco_data_by_uid, l, tmp) {
	printf("0x%08X %s\n", l->uid, l->name);
    }
}

