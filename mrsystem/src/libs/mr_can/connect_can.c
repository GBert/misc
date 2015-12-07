#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "mr_can.h"

int MrConnectCan(char *IfName)
{  int Socket;
   struct sockaddr_can caddr;
   struct ifreq ifr;

   strcpy(ifr.ifr_name, IfName);
   Socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
   if (Socket >= 0)
   {
      caddr.can_family = AF_CAN;
      if (ioctl(Socket, SIOCGIFINDEX, &ifr) >= 0)
      {
         caddr.can_ifindex = ifr.ifr_ifindex;
         if (bind(Socket, (struct sockaddr *)&caddr, sizeof(caddr)) < 0)
         {
            close(Socket);
            Socket = -1;
         }
      }
      else
      {
         close(Socket);
         Socket = -1;
      }
   }
   return(Socket);
}
