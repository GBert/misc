#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_cs2ms2.h"

int MrEthCs2ConnectTcpClient(struct sockaddr_in *UdpSrvAddr)
{  int Socket;
   struct sockaddr_in ServAddr;

   Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (Socket >= 0)
   {
      memset(&ServAddr, 0, sizeof(ServAddr));
      ServAddr.sin_family = AF_INET;
      ServAddr.sin_addr.s_addr = UdpSrvAddr->sin_addr.s_addr;
      ServAddr.sin_port = htons(MR_CS2_UDP_LOCAL_PORT);
      if (connect(Socket, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
      {
         close(Socket);
         Socket = -1;
      }
   }
   return(Socket);
}
