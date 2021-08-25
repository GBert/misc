#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "mr_cs2ms2.h"

int MrEthCs2StartBcServer(void)
{  int Socket, on;
   struct sockaddr_in ServAddr;

   /* Create socket for sending/receiving datagrams */
   Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (Socket >= 0)
   {
      on = 1;
      if (setsockopt(Socket, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) == 0)
      {
         memset(&ServAddr, 0, sizeof(ServAddr));
         ServAddr.sin_family = AF_INET;
         ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
         ServAddr.sin_port = htons(MR_CS2_UDP_LOCAL_PORT);
         if (bind(Socket, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
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
   }
   else
   {
      close(Socket);
      Socket = -1;
   }
   return(Socket);
}
