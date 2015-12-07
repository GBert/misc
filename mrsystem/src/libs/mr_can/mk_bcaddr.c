#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "mr_can.h"

void MrEthCs2MkBcAddr(struct sockaddr_in *baddr, char *IpAddr)
{
   bzero(baddr, sizeof(*baddr));
   baddr->sin_family = AF_INET;
   baddr->sin_port = htons(MR_CS2_UDP_REMOTE_PORT);
   inet_pton(AF_INET, IpAddr, &baddr->sin_addr);
}
