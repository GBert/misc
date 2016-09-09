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

char *track_dir   = { "/gleisbilder" };
char *track_name  = { "gleisbild.cs2" };
char *loco_name  = { "lokomotive.cs2" };
char *mags_name  = { "magnetartikel.cs2" };
char *auto_name  = { "fahrstrassen.cs2" };
char *gbs_default = { "gbs-0" };

enum {
    L00_TRACK_PAGE = 0,
    L00_TRACK,
    L00_LOCO,
    L00_MAGS,
    L00_ATRACK,
    L0_LOCO,
    L0_ATRACK,
    L0_VERSION,
    L0_SESSION,
    L0_ITEM,
    L0_ELEMENT,
    L0_SIZE,
    L0_LAST_USED,
    L0_PAGE
};

const char *l0_token [] = {
    "[gleisbildseite]",
    "[gleisbild]",
    "[lokomotive]",
    "[magnetartikel]",
    "[fahrstrassen]",
    "lokomotive",
    "fahrstrasse",
    "version",
    "session",
    "artikel",
    "element",
    "groesse",
    "zuletztBenutzt",
    "seite",
    "\0"
};

enum {
    L1_FUNCTION = 0,
    L1_MAJOR,
    L1_MINOR,
    L1_ID,
    L1_UID,
    L1_DIRECTION,
    L1_VELOCITY,
    L1_ADDRESS,
    L1_SID,
    L1_MFXUID,
    L1_SYMBOL,
    L1_ICON,
    L1_AV,
    L1_BV,
    L1_VOLUME,
    L1_PROGMASK,
    L1_VMIN,
    L1_VMAX,
    L1_XPROT,
    L1_TMAX,
    L1_SPM,
    L1_FT,
    L1_MFXTYPE,
    L1_NAME,
    L1_SURNAME,
    L1_TYPE,
    L1_ROTATION,
    L1_ITEM,
    L1_XOFFSET,
    L1_YOFFSET,
    L1_WIDTH,
    L1_HEIGHT,
    L1_TEXT,
    L1_STATE,
    L1_DEVICEID
};

const char *l1_token [] = {
    " .funktionen",
    " .major=",
    " .minor=",
    " .id=",
    " .uid=",
    " .richtung=",
    " .velocity=",
    " .adresse=",
    " .sid=",
    " .mfxuid=",
    " .symbol=",
    " .icon=",
    " .av=",
    " .bv=",
    " .volume=",
    " .progmask=",
    " .vmin=",
    " .vmax=",
    " .xprot=",
    " .tachomax=",
    " .spm=",
    " .ft=",
    " .mfxtyp=",
    " .name=",
    " .vorname=",
    " .typ=",
    " .drehung=",
    " .artikel=",
    " .xoffset=",
    " .yoffset=",
    " .width=",
    " .height=",
    " .text=",
    " .zustand=",
    " .deviceId=",
    "\0"
};

/* TODO : use strlen macro */
#define L1_MAJOR_LENGTH		8
#define L1_MINOR_LENGTH		8
#define L1_ID_LENGTH		5
#define L1_UID_LENGTH		6
#define L1_DIRECTION_LENGTH	11
#define L1_VELOCITY_LENGTH	11
#define L1_ADDRESS_LENGTH	10
#define L1_SID_LENGTH		6
#define L1_MFXUID_LENGTH	9
#define L1_SYMBOL_LENGTH	9
#define L1_ICON_LENGTH		7
#define L1_AV_LENGTH		5
#define L1_BV_LENGTH		5
#define L1_VOLUME_LENGTH	9
#define L1_PROGMASK_LENGTH	11
#define L1_VMIN_LENGTH		7
#define L1_VMAX_LENGTH		7
#define L1_XPROT_LENGTH		8
#define L1_TMAX_LENGTH		11
#define L1_SPM_LENGTH		6
#define L1_FT_LENGTH		5
#define L1_MFXTYPE_LENGTH	9
#define L1_NAME_LENGTH		7
#define L1_SURNAME_LENGTH	10
#define L1_TYPE_LENGTH		6
#define L1_ROTATION_LENGTH	10
#define L1_ITEM_LENGTH		10
#define L1_XOFFSET_LENGTH	10
#define L1_YOFFSET_LENGTH	10
#define L1_WIDTH_LENGTH		8
#define L1_HEIGHT_LENGTH	9
#define L1_TEXT_LENGTH		7
#define L1_STATE_LENGTH		10
#define L1_DEVICEID_LENGTH	11

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

const char *track_types[] = {
    "leer",
    "gerade",
    "bogen",
    "linksweiche",
    "rechtsweiche",
    "dreiwegweiche",
    "kreuzung",
    "unterfuehrung",
    "prellbock",
    "doppelbogen",
    "tunnel",
    "yweiche",
    "dkweiche",
    "dkweiche_2",
    "signal",
    "s88kontakt",
    "s88bogen",
    "pfeil",
    "fahrstrasse",
    "text",
    "k84_einfach",
    "k84_doppelt",
    "entkuppler",
    "entkuppler_1",
    "std_rot",
    "std_gruen",
    "std_rot_gruen_0",
    "std_rot_gruen_1",
    "signal_hp02",
    "signal_hp012",
    "signal_hp01s",
    "signal_p_hp012s",
    "signal_f_hp012s",
    "signal_p_hp012",
    "signal_f_hp01",
    "signal_f_hp02",
    "signal_f_hp012",
    "signal_sh01",
    "schiebebuehne_0",
    "schiebebuehne_1",
    "schiebebuehne_2",
    "schiebebuehne_3",
    "drehscheibe_alt_0",
    "drehscheibe_alt_1",
    "drehscheibe_alt_2",
    "drehscheibe_alt_3",
    "drehscheibe_dig_0",
    "drehscheibe_dig_1",
    "drehscheibe_dig_2",
    "drehscheibe_dig_3",
    "drehscheibe_dig_4",
    "drehscheibe_dig_5",
    "drehscheibe_dig_6",
    "drehscheibe_dig_7",
    "drehscheibe_dig_8",
    "drehscheibe_dig_9",
    "drehscheibe_dig_10",
    "drehscheibe_dig_11",
    "drehscheibe_dig_12",
    "drehscheibe_dig_13",
    "drehscheibe_dig_14",
    "drehscheibe_dig_15",
    "drehscheibe_dig_16",
    "drehscheibe_dig_17",
    "drehscheibe_dig_18",
    "drehscheibe_dig_19",
    "drehscheibe_dig_20",
    "drehscheibe_dig_21",
    "drehscheibe_dig_22",
    "drehscheibe_dig_23",
    "drehscheibe_dig_24",
    "drehscheibe_dig_25",
    "drehscheibe_dig_26",
    "drehscheibe_dig_27",
    "drehscheibe_dig_28",
    "drehscheibe_dig_29",
    "drehscheibe_dig_30",
    "drehscheibe_dig_31",
    "sonstige_gbs",
    "\0"
};

char *mag_types[] = {
    "digitaldrehscheibe",
    "DKW 1 Antrieb",
    "DKW 2 Antriebe",
    "drehscheibe_alt",
    "dreiwegweiche",
    "entkupplungsgleis",
    "entkupplungsgleis_1",
    "formsignal_HP01",
    "formsignal_HP012",
    "formsignal_HP012_SH01",
    "formsignal_HP02",
    "formsignal_SH01",
    "k84_ausgang",
    "k84_doppelausgang",
    "lichtsignal_HP01",
    "lichtsignal_HP012",
    "lichtsignal_HP012_SH01",
    "lichtsignal_HP02",
    "lichtsignal_SH01",
    "linksweiche",
    "rechtsweiche",
    "schiebebuehne",
    "std_gruen",
    "std_rot",
    "std_rot_gruen",
    "urc_lichtsignal_HP01",
    "urc_lichtsignal_HP012",
    "urc_lichtsignal_HP012_SH01",
    "urc_lichtsignal_SH01",
    "y_weiche",
    "\0",
};

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

enum {
    L2_NUMBER = 0,
    L2_TYPE,
    L2_DURATION,
    L2_VALUE,
    L2_TARGET,
    L2_NAME,
    L2_ADDRESS,
    L2_XCEL,
    L2_SPEEDTABLE,
    L2_VOLUME,
    L2_NUMFUNCTION,
    L2_FUNKTION
};
    
const char *l2_token [] = {
    " ..nr=",
    " ..typ=",
    " ..dauer=",
    " ..wert=",
    " ..target=",
    " ..name=",
    " ..addr=",
    " ..xcel=",
    " ..speedtable=",
    " ..volume=",
    " ..numfunct=",
    " ..func=",
    "\0"
};

/* TODO : use strlen macro */
#define L2_NUMBER_LENGTH	6
#define L2_DURATION_LENGTH	9
#define L2_TYPE_LENGTH		7
#define L2_VALUE_LENGTH		8
#define L2_TARGET_LENGTH	10
#define L2_NAME_LENGTH		8
#define L2_ADDRESS_LENGTH	8
#define L2_SPEEDTABLE_LENGTH	14
#define L2_VOLUME_LENGTH	10
#define L2_NUMFUNCTION_LENGTH	10
#define L2_FUNCTION_LENGTH	8

unsigned char pre_mfx[]   = { 0x02, 0xf5, 0x00 };
unsigned char pre_other[] = { 0x02, 0xc5, 0x00 };

struct loco_func {
    uint8_t number;
    uint8_t type;
    uint16_t duration;
    uint16_t value;
};
 
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

/* symbol :0=e loco, 1=Diesel loco, 2=steam loco, 3=no icon
 */

struct loco_data_t {
    unsigned int major;
    unsigned int minor;
    unsigned int id;
    unsigned int uid;
    unsigned int direction;
    unsigned int velocity;
    unsigned int long_uid;
    char *name;
    char *surname;
    char *type;
    char *icon;
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
    unsigned int xprot;
    unsigned int traction;
    unsigned int spm;
    unsigned int ft;
    unsigned int mfxtype;
    struct loco_func function[16];
    struct mfxAdr_t *mfxAdr;
    UT_hash_handle hh;
};

#endif /* _CS2_CONFIG_H_ */
