// ms1relay.h : MS1 adaption to MCS
//
// C 2020-2021 Rainer Müller
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#ifndef _MS1_RELAY_H
#define _MS1_RELAY_H

enum { OH_PING = 1, OH_ROOT, OH_SYST, OH_SDES = 8, OH_LOCO };   // predefined object handles

/********* MS1 database *********/

typedef struct _ms1dat {
    uint8_t  slaveuid[4];
    uint8_t  slvversion[2];
    uint8_t  slvname[18];
    uint8_t  slvstackcnt;
    uint16_t slvstack[10];
    uint16_t slvassignedOH;
    uint8_t  multiadd;
    uint8_t  direction;
} ms1dat_t;

/******** loco database *********/

typedef struct _locodat {
    uint16_t    locoid;
    uint8_t     locoicon;
    uint8_t     funmbr;
    uint8_t     futype[10];
    char        loconame[16];
} locodat_t;

int readDBfile(char *filename, int entries, int verbose);

uint16_t getNextOH(uint8_t node, uint16_t lastOH);

locodat_t *getlocodat(uint16_t handle);

int updatereadDB(char *filename, int entries, int verbose);

void locostackAdd(uint8_t node, uint16_t objhandle);

void locostackRemove(uint8_t node, uint16_t objhandle);


#endif /* _MS1_RELAY_H */
