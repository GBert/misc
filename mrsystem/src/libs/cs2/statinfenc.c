#include <string.h>
#include <bytestream.h>
#include "cs2.h"

int Cs2StatusInfoEncode(char *Buffer, int NumCfg, int NumStat,
                        unsigned long SerienNummer, char *ArtikelNummer,
                        char *ArtikelName)
{
   Buffer[0] = (char)NumCfg;
   Buffer[1] = (char)NumStat;
   Buffer[2] = (char)0;
   Buffer[3] = (char)0;
   SetLongToByteArray((unsigned char *)(&Buffer[4]), SerienNummer);
   memset(&(Buffer[8]), 0, 8);
   strncpy(&(Buffer[8]), ArtikelNummer, 8);
   strcpy(&(Buffer[16]), ArtikelName);
   return(16 + strlen(ArtikelName));
}
