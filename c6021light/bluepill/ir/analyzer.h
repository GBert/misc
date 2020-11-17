// analyse.h : Headerdatei für die Konsolenanwendung zur Analyse des Digitalsignals.
// C 2005 Rainer Müller 
// Das Programm unterliegt den Bedingungen der GNU General Public License 3 (GPL3).


#include <stdbool.h>

extern bool detail, mfxdetail;

void analyzer(int start, int dauer);

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


