#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "mr_ipc.h"

int MrIpcAccept(int ServerSock)
{  int ClntSock;                /* Socket descriptor for client */
   struct sockaddr_in ClntAddr; /* Client address */
   unsigned int ClntLen;        /* Length of client address data structure */
   int Flags;

   /* Set the size of the in-out parameter */
   ClntLen = sizeof(ClntAddr);
   /* Wait for a client to connect */
   ClntSock = accept(ServerSock, (struct sockaddr *)&ClntAddr, &ClntLen);
   if (ClntSock >= 0)
   {
      Flags = fcntl(ClntSock, F_GETFL, 0);
      if (Flags != -1)
      {
         fcntl(ClntSock, F_SETFL, Flags | O_NONBLOCK);
      }
      else
      {
         close(ClntSock);
         ClntSock = -1;
      }
   }
   return(ClntSock);
}
