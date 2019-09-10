#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include "mr_cs2ms2.h"

void MrEthCs2SendTo(int Socket, struct sockaddr_in *ClntAddr, char *Data)
{  struct sockaddr_in ServAddr;

   memcpy(&ServAddr, ClntAddr, sizeof(struct sockaddr_in));
   ServAddr.sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
   sendto(Socket, Data, MR_CS2_UDP_LENGTH, 0, 
          (struct sockaddr *)&ServAddr, sizeof(ServAddr));
}
