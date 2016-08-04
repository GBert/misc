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

char *track_dir  = { "/gleisbilder" };
char *track_name = { "gleisbild.cs2" };
char *gbs_default    = { "gbs-0" };

enum {
    L0_TRACK = 0,
    L0_LOCO,
    L0_MAGS,
    L0_ATRACK,
    L1_LOCO,
    L1_ATRACK,
    L1_VERSION,
    L1_SESSION,
    L1_ITEM,
    L1_ELEMENT,
    L1_SIZE,
    L1_LAST_USED,
    L1_PAGE,
};

const char *l01_token [] = {
    "[gleisbild]",
    "[lokomotove]",
    "[magnetartikel]",
    "[fahrstrassen]",
    "lokomotive",
    "fahrstrasse",
    "version",
    "sessiion",
    "artikel",
    "element",
    "groesse",
    "zuletztBenutzt",
    "seite",
    NULL
};

enum {
    L2_MAJOR = 0,
    L2_MINOR,
    L2_ID,
    L2_NAME,
    L2_SURNAME,
    L2_TYPE,
    L2_ITEM,
    L2_XOFFSET,
    L2_YOFFSET,
};

const char *l2_token [] = {
    " .major=",
    " .minor=",
    " .id=",
    " .name=",
    " .vorname=",
    " .typ=",
    " .artikel=",
    " .xoffset=",
    " .yoffset=",
    NULL
};

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
    struct track_config_t *track_config;
};

/* [gleisbild]
version
 .major=1
groesse
zuletztBenutzt
 .name=Eschenhofen
seite
 .name=Tastenmatrix
seite
 .id=1
 .name=SBF S1
*/

struct track_config_t {
    unsigned int major;
    unsigned int minor;
    unsigned int size;
    unsigned int xoffset;
    unsigned int yoffset;
    char *last_used;
    unsigned int id;
    char *name;
    UT_hash_handle ut_hash;
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
    "NULL"
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
    NULL
};

struct track_data_t {
    unsigned int id;
    unsigned int version_major;
    unsigned int version_minor;
    int item;
    int state;
    unsigned int type;
    unsigned int rotation;
    unsigned int deviceId;
    char *text;
    char *name;
    UT_hash_handle ut_hash;
};

#endif
