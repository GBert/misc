#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#if 1
#include <sys/ioctl.h>
#endif
#include <arpa/inet.h>
#include <net/if.h>
#include "mr_ipc.h"
#include <errno.h>

#define MAXPENDING 4

int MrIpcStartServerIf(char *Interface, int Port)
{  int sock;
   struct sockaddr_in ServerAddr;
   struct ifreq ifr;

   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock >= 0)
   {
      memset(&ifr, 0, sizeof(ifr));
      if (Interface != NULL)
         snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", Interface);
      else
         snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth0");
#if 1
      ifr.ifr_addr.sa_family = AF_INET;
      if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
#else
      if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr,
                     sizeof(ifr)) < 0)
#endif
      {
         close(sock);
         sock = -1;
      }
      else
      {
         memset(&ServerAddr, 0, sizeof(ServerAddr));
         ServerAddr.sin_family = AF_INET;
#if 1
         ServerAddr.sin_addr.s_addr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
#else
         ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
         ServerAddr.sin_port = htons(Port);
         if (bind(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
         {
            close(sock);
            sock = -1;
         }
         else
         {
            if (listen(sock, MAXPENDING) < 0)
            {
               close(sock);
               sock = -1;
            }
         }
      }
   }
   return(sock);
}
