/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 */

#ifndef _LOKINFO_H
#define _LOKINFO_H

#include "uthash.h"
/*
lok
.uid=0x4005
.name=M4
.adresse=0x9
.typ=mfx
.mfxuid=0xff001234
.av=64
.bv=48
.volume=64
.vmax=255
.vmin=12
.fkt
..nr=0
..typ=32
.mfxAdr
..target=2
..name=3
..addr=136
..xcel=78
..speedtable=79
..volume=117
..numfunc=16
..func=21

*/

unsigned char pre_mfx[]   = { 0x02, 0xf5, 0x00 };
unsigned char pre_other[] = { 0x02, 0xc5, 0x00 };

struct mfxAdr_t {
    unsigned int target;
    char *name;
    unsigned int address;
    unsigned int xcel;
    unsigned int speedtable;
    unsigned int volume;
    unsigned int numfunc;
    unsigned int funcy;
};

struct loco_config_t {
    int eeprom_max_size;
    unsigned int eeprom_size;
    int id;
    FILE *fp;
    char *filename;
    unsigned char *bin;
};

struct loco_data_t {
    unsigned int long_uid;
    unsigned int id;
    char *name;
    char *proto;
    unsigned int address;
    unsigned int typ;
    unsigned int mfxuid;
    unsigned int acc_delay;		/* av */
    unsigned int slow_down_delay;	/* bv */
    unsigned int volume;
    unsigned int vmax;
    unsigned int vmin;
    struct mfxAdr_t mfxAdr;
    UT_hash_handle hh;
};

#endif /* _LOKINFO_H_ */
