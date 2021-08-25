/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _CS2_CONFIG_H
#define _CS2_CONFIG_H

#include "uthash.h"

/* must be power of 2 */
#define MAX_LOCO_FUNCTIONS	32

extern char *track_dir;
extern char *track_name;
extern char *loco_name;
extern char *magnet_name;
extern char *auto_name;
extern char *gbs_default;

struct config_data_t {
    int deflated_stream_size;
    int deflated_size;
    int inflated_size;
    int verbose;
    uint16_t crc;
    char *name;
    char *directory;
    char *filename;
    uint8_t *deflated_data;
    uint8_t *inflated_data;
};

struct track_page_t {
    unsigned int id;
    unsigned int major;
    unsigned int minor;
    unsigned int size;
    unsigned int xoffset;
    unsigned int yoffset;
    unsigned int width;
    unsigned int height;
    char *name;
    char *file_name;
    UT_hash_handle hh;
};

struct track_data_t {
    unsigned int id;
    unsigned int deviceid;
    char *text;
    int8_t item;
    int8_t state;
    int8_t type;
    int8_t rotation;
    UT_hash_handle hh;
};

struct loco_func {
    uint8_t number;
    uint8_t type;
    uint8_t forward;
    uint8_t backward;
    uint16_t duration;
    uint16_t value;
};
 
struct mfxAdr_t {
    unsigned int target;
    unsigned int name;
    unsigned int address;
    unsigned int xcel;
    unsigned int speedtable;
    unsigned int volume;
    unsigned int numfunction;
    unsigned int function;
};

struct loco_config_t {
    int eeprom_max_size;
    unsigned int eeprom_size;
    int id;
    FILE *fp;
    char *filename;
    unsigned char *bin;
};

/* symbol :0=e loco, 1=Diesel loco, 2=steam loco, 3=no icon
 */

struct loco_data_t {
    unsigned int major;
    unsigned int minor;
    unsigned int id;
    unsigned int uid;
    unsigned int direction;
    int number;
    unsigned int velocity;
    unsigned int long_uid;
    char *name;
    char *surname;
    char *type;
    char *icon;
    unsigned int dv;
    unsigned int address;
    unsigned int sid;
    unsigned int mfxuid;
    unsigned int symbol;
    unsigned int acc_delay;		/* av */
    unsigned int slow_down_delay;	/* bv */
    unsigned int volume;
    unsigned int progmask;
    unsigned int tmax;
    unsigned int vmin;
    unsigned int vmax;
    unsigned int tacho;
    unsigned int xprot;
    unsigned int location;
    unsigned int journey;
    unsigned int traction;
    unsigned int spm;
    unsigned int ft;
    unsigned int mfxtype;
    unsigned int intraction;
    struct loco_func function[MAX_LOCO_FUNCTIONS];
    unsigned int loco_func_summary;
    struct mfxAdr_t *mfxAdr;
    UT_hash_handle hh, hha;
};

struct loco_names_t {
    char *name;
    unsigned int number;
    unsigned int max_value;
    UT_hash_handle hh;
};

struct magnet_data_t {
    unsigned int major;
    unsigned int minor;
    unsigned int id;
    char *name;
    unsigned int switchtime;
    uint16_t type;
    uint16_t odd;
    uint8_t direction;
    uint8_t decoder;
    uint8_t decoder_type;
    uint8_t position;
    UT_hash_handle hh;
};

#endif /* _CS2_CONFIG_H_ */
