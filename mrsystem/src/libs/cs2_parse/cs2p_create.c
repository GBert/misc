#include <stdlib.h>
#include <scanner.h>
#include "cs2parse.h"

/** @file */

/**
* @brief Erzeugen eines Parsers
*
* Die Cs2pCreate() Funktion wird als erste Funktion aufgerufen, um die
* Struktur anzulegen.
*
* @return Zeiger auf die Parserstruktur oder NULL im Fehlerfall.
*/
Cs2parser *Cs2pCreate(void)
{  Cs2parser *NewData;

   NewData = (Cs2parser *)malloc(sizeof(Cs2parser));
   if (NewData != NULL)
   {
      Cs2pSetScanner(NewData, ScanCreate());
      if (Cs2pGetScanner(NewData) == (Scanner *)NULL)
      {
         free(NewData);
         NewData = (Cs2parser *)NULL;
      }
   }
   return(NewData);
}
