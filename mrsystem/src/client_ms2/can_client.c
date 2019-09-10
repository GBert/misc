#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <mr_cs2ms2.h>
#include "can_io.h"
#include "can_client.h"


static int CanClientGetFd(void *private)
{  CanClientStruct *Data;
   int ReturnFd;

   Data = (CanClientStruct *)private;
   switch (CanClientGetFdPos(Data))
   {
      case FD_POS_CAN:
         ReturnFd = CanClientGetCanSock(Data);
         CanClientSetFdPos(Data, FD_POS_ENDE);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         CanClientSetFdPos(Data, FD_POS_CAN);
         break;
   }
   return(ReturnFd);
}

static BOOL CanClientOpen(void *private)
{  CanClientStruct *Data;
   struct sockaddr_can caddr;
   struct ifreq ifr;

   Data = (CanClientStruct *)private;
   strcpy(ifr.ifr_name, CanClientGetCanName(Data));
   CanClientSetCanSock(Data, socket(PF_CAN, SOCK_RAW, CAN_RAW));
   if (CanClientGetCanSock(Data) >= 0)
   {
      caddr.can_family = AF_CAN;
      if (ioctl(CanClientGetCanSock(Data), SIOCGIFINDEX, &ifr) >= 0)
      {
         caddr.can_ifindex = ifr.ifr_ifindex;
         if (bind(CanClientGetCanSock(Data), (struct sockaddr *)&caddr, sizeof(caddr)) < 0)
         {
            close(CanClientGetCanSock(Data));
            CanClientSetCanSock(Data, IOFKT_INVALID_FD);
         }
         else
         {
            CanClientSetFdPos(Data, FD_POS_CAN);
         }
      }
      else
      {
         close(CanClientGetCanSock(Data));
         CanClientSetCanSock(Data, IOFKT_INVALID_FD);
      }
   }
   return(CanClientGetCanSock(Data) >= 0);
}

static void CanClientClose(void *private)
{  CanClientStruct *Data;

   Data = (CanClientStruct *)private;
   close(CanClientGetCanSock((CanClientStruct *)private));
   CanClientSetCanSock(Data, IOFKT_INVALID_FD);
}

static BOOL CanClientRead(void *private, int fd, BOOL PendingData,
                          MrCs2CanDataType *CanMsg)
{  struct can_frame ReadCanFrame;
   int Ret;

   if (PendingData)
   {
      return(FALSE);
   }
   else
   {
      Ret = read(fd, (void *)&ReadCanFrame, sizeof(struct can_frame));
      if (Ret > 0)
      {
         MrCs2Decode(CanMsg, &ReadCanFrame);
         return(TRUE);
      }
      else
      {
         return(FALSE);
      }
   }
}

static BOOL CanClientWrite(void *private, int ReceiverSocket,
                           MrCs2CanDataType *CanMsg)
{  struct can_frame SendCanFrame;

   MrCs2Encode(CanMsg, &SendCanFrame);
   return(write(CanClientGetCanSock((CanClientStruct *)private),
                (const void *)&SendCanFrame, sizeof(struct can_frame)) ==
          sizeof(struct can_frame));
}

static IoFktStruct CanClientIoFunctions =
{
   NULL,
   CanClientGetFd,
   CanClientOpen,
   CanClientClose,
   CanClientRead,
   CanClientWrite
};

IoFktStruct *CanClientInit(BOOL Verbose, char *CanIf)
{  CanClientStruct *Data;

   Data = (CanClientStruct *)malloc(sizeof(CanClientStruct));
   if (Data != (CanClientStruct *)NULL)
   {
      CanClientSetVerbose(Data, Verbose);
      CanClientSetCanSock(Data, IOFKT_INVALID_FD);
      CanClientSetCanName(Data, CanIf);
      CanClientIoFunctions.private = (void *)Data;
      return(&CanClientIoFunctions);
   }
   else
      return(NULL);
}

void CanClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
