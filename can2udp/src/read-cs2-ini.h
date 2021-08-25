/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _READ_CS2_INI_H
#define _READ_CS2_INI_H

#define MAXSIZE         16384
#define MAXDIR          256
#define MAXNAME         256
#define MAXGBS          16
#define MAXSTRING       1024

#define DEBUG           0

#define debug_print(...) \
            do { if (DEBUG) fprintf(stdin, ##__VA_ARGS__); } while (0)

#define check_modify(a, b) \
            do { if ( a ) b = a; } while (0)

#define check_free(a) \
            do { if ( a ) free(a); } while (0)

#include	"cs2-config.h"

int get_char_index(const char **list, char *str);
void delete_all_track_pages(void);
void delete_all_track_data(void);
void delete_all_loco_data(void);
int add_loco(struct loco_data_t *loco);
int add_track_data(struct track_data_t *td);
int add_track_page(struct track_page_t *page, char *name);
int td_id_sort(struct track_data_t *a, struct track_data_t *b);
int tp_id_sort(struct track_page_t *a, struct track_page_t *b);
void sort_tp_by_id(void);
void sort_td_by_id(void);
void print_pages(void);
void print_gbstats(void);
void print_tracks(void);
int read_track_data(char *config_file);
int read_track_config(char *config_file);
void read_track_pages(char *dir);
int read_loco_data(char *config_file);

#endif /* _READ_CS2_INI_H */
