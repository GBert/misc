#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief Verbindung zur drehscheibe aufbauen
*
* Diese Funktion stellt eine Verbindung zur drehscheibe her.
*
* @param[in] Address IP Adresse zur drehscheibe
* @param[in] Port Portnummer der drehscheibe
*
* @return Sockert
*/
int MrIpcConnect(char *IpAddr, int Port)
{  int sock;
   struct sockaddr_in ServerAddr;

   sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock < 0)
   {
      return(-1);
   }
   else
   {
      memset(&ServerAddr, 0, sizeof(ServerAddr));
      ServerAddr.sin_family = AF_INET;
      ServerAddr.sin_addr.s_addr = inet_addr(IpAddr);
      ServerAddr.sin_port = htons(Port);
      if (connect(sock, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) < 0)
      {
         close(sock);
         return(-2);
      }
      else
      {
         return(sock);
      }
   }
}
