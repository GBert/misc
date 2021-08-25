#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_cs2ms2.h"

int MrEthCs2ConnectClient(void)
{  int Socket;
   struct sockaddr_in ServAddr;

   Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (Socket >= 0)
   {
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
      ServAddr.sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
      if (bind(Socket, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
   }
   return(Socket);
}
