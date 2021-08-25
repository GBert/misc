#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief Verbindung zur drehscheibe aufbauen
*
* Diese Funktion stellt eine Verbindung zur drehscheibe her.
*
* @param[in] Interface Interface zur drehscheibe
* @param[in] Address IP Adresse zur drehscheibe
* @param[in] Port Portnummer der drehscheibe
*
* @return Sockert
*/
int MrIpcConnectIf(char *Interface, int Port)
{  int sock;
   struct sockaddr_in ServerAddr;
   struct ifreq ifr;

   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock < 0)
   {
      sock = -1;
   }
   else
   {
      memset(&ifr, 0, sizeof(ifr));
      ifr.ifr_addr.sa_family = AF_INET;
      if (Interface != NULL)
         snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", Interface);
      else
         snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "eth0");
      if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
      {
         close(sock);
         sock = -1;
      }
      else
      {
         memset(&ServerAddr, 0, sizeof(ServerAddr));
         ServerAddr.sin_family = AF_INET;
         ServerAddr.sin_addr.s_addr = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
         ServerAddr.sin_port = htons(Port);
         if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
         {
            close(sock);
            sock = -1;
         }
      }
   }
   return(sock);
}
