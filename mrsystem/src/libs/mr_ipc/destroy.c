#include <stdlib.h>
#include <stddef.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief IPC Object freigeben
*
* Die MrIpcDestroy Funktion wird als letzte Funktion aufgerufen und gibt die
* Struktur wieder frei.
*
* @param[in] Data Zeiger auf die IPC Struktur
*/
void MrIpcDestroy(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      free(Data);
   }
}
