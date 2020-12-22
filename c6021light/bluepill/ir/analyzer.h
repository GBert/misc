// analyse.h : Headerdatei für die Konsolenanwendung zur Analyse des Digitalsignals.
// C 2005 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).

#ifndef _ANALYZER_H_
#define _ANALYZER_H_

#define bit_set(var, bit) ((var) |= (1 << (bit)))
#define bit_clear(var, bit) ((var) &= (unsigned)~(1 << (bit)))

#include <stdint.h>
#include <stdbool.h>

extern bool detail, mfxdetail;

struct st_mm {
    int strt, pause;
    int16_t adr;
    int8_t fkt, dat, xdat;
    bool freq2;
};

struct loco_status {
    uint16_t address;
    uint16_t speed;
    uint32_t function;
    uint32_t mask;
    uint32_t timestamp;
};

void analyzer(int start, int dauer);
void new_mm_command(void);
void mm_print(void);
void dcc_analyzer(void);
void mfx_analyzer(int duration);
bool mfx_print(void);

void dcc_accessory(void);
void dcc_cv_acc(int s);
void dcc_decode(void);
void dcc_service(void);
unsigned int mfx_crc(void);
unsigned int mfxwert(int start, int length);
unsigned long mfxlwert(int start, int length);

#endif
