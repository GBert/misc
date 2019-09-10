#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "mr_cs2ms2.h"

void MrEthCs2MkBcAddr(struct sockaddr_in *baddr, char *IpAddr)
{
   memset(baddr, 0, sizeof(*baddr));
   baddr->sin_family = AF_INET;
   baddr->sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
   baddr->sin_addr.s_addr = inet_addr(IpAddr);
}
