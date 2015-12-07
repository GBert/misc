#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "mr_can.h"

void MrEthCs2Send(int Socket, struct sockaddr_in *ClntAddr, char *Data)
{  struct sockaddr_in ServAddr;

   memset(&ServAddr, 0, sizeof(ServAddr));
   ServAddr.sin_family = AF_INET;
   ServAddr.sin_addr.s_addr = ClntAddr->sin_addr.s_addr;
   ServAddr.sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
   sendto(Socket, Data, MR_CS2_UDP_LENGTH, 0, 
          (struct sockaddr *)&ServAddr, sizeof(ServAddr));
}
