#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "mr_can.h"

int MrEthCs2StartBcServer(void)
{  int Socket, on;

   /* Create socket for sending/receiving datagrams */
   Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (Socket >= 0)
   {
      on = 1;
      if (setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
   }
   else
   {
      close(Socket);
      Socket = -1;
   }
   return(Socket);
}
