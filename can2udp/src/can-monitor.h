/* ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <info@gerhard-bertelsmann.de> wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in return
 * Gerhard Bertelsmann
 * ----------------------------------------------------------------------------
 *
 */

#ifndef _CAN_MONITOR_H
#define _CAN_MONITOR_H

#define MAX_MESSWERTE	256

#if 0					/* FIXME: actually not used in canmon */
struct statusdaten_t {
    uint32_t geraete_id;
    uint8_t messwerte;
    uint8_t kanaele;
    uint32_t serienummer;
    char artikelnummer[8];
    char geratebezeichnung[32];
};
#endif

struct messwert_t {
    uint64_t geraete_id_messwert;
    uint8_t anfragenummer;
    int8_t potenz;
    uint8_t farbe_bereich1;
    uint8_t farbe_bereich2;
    uint8_t farbe_bereich3;
    uint8_t farbe_bereich4;
    uint16_t nullpunkt;
    uint16_t ende_bereich1;
    uint16_t ende_bereich2;
    uint16_t ende_bereich3;
    uint16_t ende_bereich4;
    char *bezeichnung_start;
    char *bezeichnung_ende;
    char *einheit;
};

struct knoten {
    void *daten;
    struct knoten *next;
};

#if 0					/* FIXME: actually not used in canmon */
enum cs2_copy_state {
    CS2_STATE_INACTIVE,
    CS2_STATE_NORMAL_CONFIG,
    CS2_STATE_TRACK_SUM,
    CS2_STATE_GET_TRACKS,
    CS2_STATE_BROADCAST_UPDATE
};

struct id_node {
    uint32_t id;
    uint8_t slave_node;
    struct id_node *next;
};
#endif

struct cs2_config_data_t {
/*  int deflated_stream_size; */
    int deflated_size;
    int deflated_size_counter;
    int inflated_size;
    uint16_t crc;
/*  char *dir;		*/	/* FIXME: which members are required in this structure? */
/*  char *name;		*/
    char name[256];
/*  int next;		*/
/*  int verbose;	*/
/*  int track_index;	*/
/*  unsigned int state;	*/
/*  int start;		*/
/*  int stream;		*/
/*  int cs2_tcp_socket;	*/
/*  int cs2_config_copy;*/
/*  unsigned int ddi;	*/
    uint8_t *deflated_data;
/*  uint8_t *inflated_data;*/
/*  char **page_name;	*/
};


int config_write(struct cs2_config_data_t *config_data);
uint16_t CRCCCITT(unsigned char *data, size_t length, unsigned short seed);

#endif /* _CAN_MONITOR_H */

