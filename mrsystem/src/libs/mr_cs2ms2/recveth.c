#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_cs2ms2.h"

int MrEthCs2Recv(int Socket, struct sockaddr_in *ClntAddr, char *Data)
{  unsigned int ClntAddrLen;
   int RecvMsgSize;

   /* Set the size of the in-out parameter */
   ClntAddrLen = sizeof(*ClntAddr);
   RecvMsgSize = recvfrom(Socket, Data, MR_CS2_UDP_LENGTH, 0,
                          (struct sockaddr *)ClntAddr, &ClntAddrLen);
   if (RecvMsgSize < 0)
   {
      /* Error in read, maybe no data left */
      return MR_ETH_CS2_RCV_ERROR;
   }
   else if (RecvMsgSize == 0)
   {
      /* socket was closed at remote side */
      return MR_ETH_CS2_RCV_CLOSED;
   }
   else
   {
      /* we have one complete message */
      return MR_ETH_CS2_RCV_OK;
   }
}
