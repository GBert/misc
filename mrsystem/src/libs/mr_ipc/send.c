#include <string.h>
#include <sys/socket.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief IPC Nachricht senden
*
* Diese Funktion versendet ein IPC Paket
*
* @param[in] socket Socket zur drehscheibe
* @param[in] Data Zeiger auf die IPC Struktur
*
* @return Fehler oder OK (MR_IPC_RCV_OK, MR_IPC_RCV_ERROR)
*/
int MrIpcSend(int socket, MrIpcCmdType *Data)
{  int BytesSend;

   if (Data != (MrIpcCmdType *)NULL)
   {
      BytesSend = send(socket, Data, sizeof(MrIpcCmdType), 0);
      if (BytesSend == sizeof(MrIpcCmdType))
      {
         return MR_IPC_RCV_OK;
      }
      else
      {
         return MR_IPC_RCV_ERROR;
      }
   }
   else
   {
      return MR_IPC_RCV_ERROR;
   }
}
