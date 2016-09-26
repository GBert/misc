#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "mr_ipc.h"

int MrIpcRecv(int socket, MrIpcCmdType *Data)
{  int RecvBytes;

   RecvBytes = recv(socket, Data, sizeof(MrIpcCmdType), 0);
   if (RecvBytes < 0)
   {
      /* Error in read, maybe no data left */
      return MR_IPC_RCV_ERROR;
   }
   else if (RecvBytes == 0)
   {
      /* socket was closed at remote side */
      return MR_IPC_RCV_CLOSED;
   }
   else
   {
      /* we have one complete message */
      return MR_IPC_RCV_OK;
   }
}
