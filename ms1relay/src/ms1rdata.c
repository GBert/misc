// ms1rdata.c : MS1 adaption to MCS
//
// C 2020-2021 Rainer Müller
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "ms1relay.h"

#define BUF 64

// function symbols for MS1
enum { NOSYMB, LIGHT, BEAMER, BELL, HORN, WHISTLE, COUPLER, TORTOISE,
        ABVRAMP, PANTHO, STEAM, FUSYM, SOUNDX, SOUND, LIGHT1, LIGHT2 };

// function symbol translation from CS2 to MS1
static uint8_t xl2to1[] = {
    NOSYMB, LIGHT,  LIGHT,  LIGHT,  BEAMER, SOUND,  PANTHO, STEAM,  TORTOISE, COUPLER,
    HORN,   SOUNDX, WHISTLE,BELL,   FUSYM,  FUSYM,  FUSYM,  FUSYM,  ABVRAMP,  SOUNDX,
    SOUNDX, SOUNDX, SOUNDX, SOUND,  SOUND,  SOUNDX, SOUNDX, SOUNDX, SOUNDX,   SOUNDX,
    SOUNDX, FUSYM,  LIGHT,  LIGHT,  LIGHT,  LIGHT,  SOUNDX, SOUNDX, LIGHT,    SOUND,
    LIGHT,  LIGHT2, LIGHT1, SOUNDX, SOUNDX, SOUNDX, FUSYM,  LIGHT,  LIGHT,    SOUNDX };


locodat_t *pld = NULL;
int16_t loconum;


uint8_t xlfusymb(uint8_t s2)
{
    uint8_t mode = s2 & 128;
    s2 &= 127;
    if (s2 >= sizeof(xl2to1)) return (mode + FUSYM);
    else return (mode + xl2to1[s2]);
}

int readDBfile(char *filename, int entries, int verbose)
{
    char *found, buffer[BUF];

    if (pld == NULL) pld = (locodat_t *) calloc(entries, sizeof(locodat_t));
    if (pld == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        return 12;
    }
    printf("Reading file: %s -> ", filename);
    FILE *sfile = fopen(filename, "r");
    if (sfile == NULL) {
        printf("open failed\n");
        return 8;
    }
    loconum = -1;
    while (fgets(buffer, BUF, sfile) != NULL) {
        for (found = buffer; *found == ' '; found++);  // skip blanks
        switch (*((int *)found)) {
            case 0x6D616E2E:	// name
                if (++loconum >= entries) goto doclose;
                strncpy(pld[loconum].loconame, found + 6, 16);
                for (int i = 0; i < 16; i++)
                    if (pld[loconum].loconame[i] < ' ') pld[loconum].loconame[i] = 0;
                pld[loconum].locoicon = 1;
                pld[loconum].funmbr = 0;
                break;
            case 0x6469752E:	// uid
                pld[loconum].locoid = (uint16_t) strtoul(found + 5, NULL, 0);
                break;
            case 0x6D79732E:	// symbol	1=V, 2=D
                pld[loconum].locoicon = 1 + atoi(found + 8);
                break;
            case 0x79742E2E:	// function type
                if (pld[loconum].funmbr < 9) {     // only F0...F8
                    pld[loconum].futype[pld[loconum].funmbr++] = xlfusymb(atoi(found + 6));
                }
                break;
        }
    }
    loconum++;
doclose:
    fclose(sfile);
    printf("%d entries read\n\n", loconum);

    if (verbose) {
        char name[18] = {0};
        locodat_t *dp = pld;
        for (int l = 0; l < loconum; l++) {
            strncpy(name, dp->loconame, 16);
            printf("OH: %3u UID: %04X, SYM: %u, Name: %s\n  ", OH_LOCO + l, dp->locoid, dp->locoicon, name);
            for (int f = 0; f < dp->funmbr; f++) printf("FUNC%d:%2X  ", f, dp->futype[f]);
            printf("\n");
            dp++;
        }
    }
    return 0;
}

uint16_t getNextOH(uint8_t node, uint16_t lastOH)
{
    if (node == 1) {
        uint16_t index = (lastOH >= OH_LOCO) ? (lastOH - OH_LOCO + 1) : 0;
        if (index < loconum) return OH_LOCO + index;
    }
    return 0;           // no more loco handles
}

locodat_t *getlocodat(uint16_t handle)
{
    if (handle >= OH_LOCO) {
        uint16_t index = handle - OH_LOCO;
        if (index < loconum) return (pld + index);
    }
    return NULL;        // no valid data available
}

int updatereadDB(char *filename, int entries, int verbose)
{
    uint8_t node = 0;           // we actually have only one MS1
    int16_t oldlocos = loconum;
    locostackRemove(node, OH_LOCO);             // remove first loco

    int cc = readDBfile(filename, entries, verbose);
    if (cc) return cc;          // exit on error
    locostackAdd(node, OH_LOCO);                // add new locos
    for (int l = 1; l < oldlocos; l++) {
        usleep(5000);
        locostackRemove(node, OH_LOCO + l);     // remove former locos
    }
    return 0;
}
