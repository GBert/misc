#include <stdlib.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief IPC Object erzeugen
*
* Die MrIpcCreate Funktion wird als erste Funktion aufgerufen, um die Struktur
* anzulegen.
*
* @return Zeiger auf IPC Struktur
*/
MrIpcCmdType *MrIpcCreate(void)
{  MrIpcCmdType *NewIpcCmd;

   NewIpcCmd = (MrIpcCmdType *)malloc(sizeof(MrIpcCmdType));
   return NewIpcCmd;
}
