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

struct statusdaten_t {
    uint32_t geraete_id;
    uint8_t messwerte;
    uint8_t kanaele;
    uint32_t serienummer;
    char artikelnummer[8];
    char geratebezeichnung[32];
};

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

#endif /* _CAN_MONITOR_H */

