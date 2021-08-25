#include <stddef.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief IPC Object initialisieren
*
* Die MrIpcInit Funktion initialisiert die internen Variablen.
*
* @param[in] Data Zeiger auf die IPC Struktur
*/
void MrIpcInit(MrIpcCmdType *Data)
{
   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCommand(Data, MrIpcCmdNull);
   }
}
