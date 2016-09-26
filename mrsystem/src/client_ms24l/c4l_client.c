#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <can4linux.h>
#include <mr_can.h>
#include "../client_ms2/can_io.h"
#include "c4l_client.h"


static int C4lClientGetFd(void *private)
{
   return(C4lClientGetCanFd((C4lClientStruct *)private));
}

static BOOL C4lClientOpen(void *private)
{  C4lClientStruct *Data;
   char DeviceName[80];

   Data = (CanClientStruct *)private;
   strcpy(DeviceName, "/dev/");
   strcat(DeviceName, C4lClientGetDevName(Data));
   C4lClientSetCanFd(Data, open(DeviceName, O_RDWR));
   return(C4lClientGetCanFd(Data) >= 0);
}

static void C4lClientClose(void *private)
{
   close(C4lClientGetCanFd((C4lClientStruct *)private));
}

static int C4lClientRead(void *private, CanFrameStruct *CanFrame)
{  canmsg_t ReadCanFrame;
   int i, Ret;

   Ret = read(C4lClientGetCanFd((C4lClientStruct *)private),
              (void *)&ReadCanFrame, 1);
   if (Ret > 0)
   {
      CanFrame->CanId = ReadCanFrame.id;
      CanFrame->CanDlc = ReadCanFrame.length;
      for (i = 0; i < 8; i++)
         CanFrame->CanData[i] = ReadCanFrame.data[i];
   }
   return(Ret);
}

static int C4lClientWrite(void *private, CanFrameStruct *CanFrame)
{  canmsg_t SendCanFrame;
   int i;

   SendCanFrame.id = CanFrame->CanId;
   SendCanFrame.length = CanFrame->CanDlc;
   for (i = 0; i < 8; i++)
      SendCanFrame.data[i] = CanFrame->CanData[i];
   return(write(C4lClientGetCanSock((C4lClientStruct *)private),
                (const void *)&SendCanFrame, 1));
}

static IoFktStruct C4lClientIoFunctions =
{
   NULL,
   C4lClientGetFd,
   C4lClientOpen,
   C4lClientClose,
   C4lClientRead,
   C4lClientWrite
};

IoFktStruct *C4lClientInit(BOOL Verbose, char *CanIf)
{  C4lClientStruct *Data;

   Data = (C4lClientStruct *)malloc(sizeof(C4lClientStruct));
   if (Data != (C4lClientStruct *)NULL)
   {
      C4lClientSetVerbose(Data, Verbose);
      C4lClientSetCanSock(Data, -1);
      C4lClientSetCanName(Data, CanIf);
      C4lClientIoFunctions.private = (void *)Data;
      return(&C4lClientIoFunctions);
   }
   else
      return(NULL);
}

void C4lClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
