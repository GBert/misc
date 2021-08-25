#include <string.h>
#include <bytestream.h>
#include "cs2.h"

void Cs2StatusInfoDecode(char *Buffer, int *NumCfg, int *NumStat,
                         unsigned long *SerienNummer, char *ArtikelNummer,
                         char *ArtikelName)
{
   *NumCfg = (int)(Buffer[0]);
   *NumStat = (int)(Buffer[1]);
   *SerienNummer = GetLongFromByteArray((unsigned char *)(&Buffer[4]));
   strncpy(ArtikelNummer, &(Buffer[8]), 8);
   ArtikelNummer[8] = '\0';
   strcpy(ArtikelName, &(Buffer[16]));
}
