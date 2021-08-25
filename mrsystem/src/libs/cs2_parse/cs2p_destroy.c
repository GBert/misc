#include <stdlib.h>
#include <scanner.h>
#include "cs2parse.h"

/** @file */

/**
* @brief Freigeben eines Parsers
*
* Die Cs2pDestroy() Funktion wird als letzte Funktion aufgerufen und gibt die
* Struktur wieder frei.
*
* @param[in] Data Zeiger auf die Parserstruktur
*/
void Cs2pDestroy(Cs2parser *Data)
{
   ScanDestroy(Cs2pGetScanner(Data));
   free(Data);
}
