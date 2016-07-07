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

struct track_t {
    unsigned int version;
    unsigned int size;
    char *last_used;
    unsigned int id;
    unsigned int name;
    UT_hash_handle ut_hash;
}; 

#endif
