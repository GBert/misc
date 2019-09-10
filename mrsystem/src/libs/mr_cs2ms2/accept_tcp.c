#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_cs2ms2.h"

int MrEthCs2Accept(int ServSock)
{  int ClntSock;
   struct sockaddr_in ClntAddr;
   unsigned int ClntLen;

   /* Set the size of the in-out parameter */
   ClntLen = sizeof(ClntAddr);
   /* Wait for a client to connect */
   ClntSock = accept(ServSock, (struct sockaddr *)&ClntAddr, &ClntLen);
   return(ClntSock);
}
