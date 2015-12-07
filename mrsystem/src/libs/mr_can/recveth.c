#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_can.h"

BOOL MrEthCs2Recv(int Socket, struct sockaddr_in *ClntAddr, char *Data)
{  unsigned int ClntAddrLen;
   int RecvMsgSize;

   /* Set the size of the in-out parameter */
   ClntAddrLen = sizeof(*ClntAddr);
   RecvMsgSize = recvfrom(Socket, Data, MR_CS2_UDP_LENGTH, 0,
                          (struct sockaddr *)ClntAddr, &ClntAddrLen);
   return(RecvMsgSize > 0);
}
