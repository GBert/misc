#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <mr_can.h>
#include "can_io.h"
#include "can_client.h"


static int CanClientGetFd(void *private)
{
   return(CanClientGetCanSock((CanClientStruct *)private));
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
            CanClientSetCanSock(Data, -1);
         }
      }
      else
      {
         close(CanClientGetCanSock(Data));
         CanClientSetCanSock(Data, -1);
      }
   }
   return(CanClientGetCanSock(Data) >= 0);
}

static void CanClientClose(void *private)
{
   close(CanClientGetCanSock((CanClientStruct *)private));
}

static int CanClientRead(void *private, CanFrameStruct *CanFrame)
{  struct can_frame ReadCanFrame;
   int i, Ret;

   Ret = read(CanClientGetCanSock((CanClientStruct *)private),
              (void *)&ReadCanFrame, sizeof(struct can_frame));
   if (Ret > 0)
   {
      CanFrame->CanId = ReadCanFrame.can_id;
      CanFrame->CanDlc = ReadCanFrame.can_dlc;
      for (i = 0; i < 8; i++)
         CanFrame->CanData[i] = ReadCanFrame.data[i];
   }
   return(Ret);
}

static int CanClientWrite(void *private, CanFrameStruct *CanFrame)
{  struct can_frame SendCanFrame;
   int i;

   SendCanFrame.can_id = CanFrame->CanId;
   SendCanFrame.can_dlc = CanFrame->CanDlc;
   for (i = 0; i < 8; i++)
      SendCanFrame.data[i] = CanFrame->CanData[i];
   return(write(CanClientGetCanSock((CanClientStruct *)private),
                (const void *)&SendCanFrame, sizeof(struct can_frame)));
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
      CanClientSetCanSock(Data, -1);
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
