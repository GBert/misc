/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _READ_CS2_CONFIG_H
#define _READ_CS2_CONFIG_H

#include "cs2-config.h"

#define MAXSIZE		16384
#define MAXDIR		256
#define MAXNAME		256
#define MAXGBS		16
#define MAXSTRING	1024

#define CONFIG_STRING	0x00
#define CONFIG_FILE	0x01

#define MFXDEC		0x00000001
#define MS2FKT		0x00000002

int get_char_index(const char **list, char *str);
int get_value(char *st, char *search);
void delete_all_track_pages(void);
void delete_all_track_data(void);
void delete_all_loco_data(void);
void delete_all_loco_names(void);
void delete_all_magnet_data(void);
void delete_loco_by_name(char *name);
void shrink_loco_list(char *name);
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
void print_loco(FILE *file, struct loco_data_t *l, unsigned int mask);
void print_loco_by_name(FILE * file, char *name, unsigned int mask);
void print_all_locos(FILE *file, unsigned int mask);
void print_all_locos_short(FILE *file);
void print_loco_names(FILE *file);
int read_magnet_data(char *config_file, int config_type);
int read_track_data(char *config_file);
int read_track_config(char *config_file);
void read_track_pages(char *dir);
int read_loco_data(char *config_file, int config_type);
int read_loco_names(char *config_file);
void show_loco_names(FILE *file, int low, int high);
int get_loco_max(void);
int get_loco_uid(char *name);

int strip_ms2_spaces(uint8_t *data, int len);
#endif /* _READ_CS2_CONFIG_H */
