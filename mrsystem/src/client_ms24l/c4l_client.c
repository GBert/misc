#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <can4linux.h>
#include <mr_cs2ms2.h>
#include "can_io.h"
#include "c4l_client.h"


static int C4lClientGetFd(void *private)
{  C4lClientStruct *Data;
   int ReturnFd;

   Data = (C4lClientStruct *)private;
   switch (C4lClientGetFdPos(Data))
   {
      case FD_POS_CAN:
         ReturnFd = C4lClientGetCanFd(Data);
         C4lClientSetFdPos(Data, FD_POS_ENDE);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         C4lClientSetFdPos(Data, FD_POS_CAN);
         break;
   }
   return(ReturnFd);
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
   C4lClientSetCanSock(Data, IOFKT_INVALID_FD);
}

static BOOL C4lClientRead(void *private, int fd, BOOL PendingData,
                          MrCs2CanDataType *CanMsg)
{  canmsg_t ReadCanFrame;
   int i, Ret;
   unsigned CanHash, Response, Command, Prio;

   if (PendingData)
   {
      return(FALSE);
   }
   else
   {
      Ret = read(fd, (void *)&ReadCanFrame, sizeof(canmsg_t));
      if (Ret > 0)
      {
         MrCs2SetId(CanMsg, ReadCanFrame.id);
         MrCs2SetDlc(CanMsg, ReadCanFrame.length);
         for (i = 0; i < 8; i++)
            MrCs2GetData(CanMsg)[i] = ReadCanFrame.data[i];
         MrCs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
         MrCs2SetHash(CanMsg, CanHash & ~MR_CS2_MASK_HASH_MAGIC);
         MrCs2SetResponse(CanMsg, Response);
         MrCs2SetCommand(CanMsg, Command);
         MrCs2SetPrio(CanMsg, Prio);
         MrCs2SetIsCs2(CanMsg, MrCs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
         return(TRUE);
      }
      else
      {
         return(FALSE);
      }
   }
}

static BOOL C4lClientWrite(void *private, int ReceiverSocket,
                           MrCs2CanDataType *CanMsg)
{  canmsg_t SendCanFrame;
   int i;

   SendCanFrame.id = MrCs2GetId(CanMsg);
   SendCanFrame.length = MrCs2GetDlc(CanMsg);
   for (i = 0; i < 8; i++)
      SendCanFrame.data[i] = MrCs2GetData(CanMag)[i];
   return(write(C4lClientGetCanSock((C4lClientStruct *)private),
                (const void *)&SendCanFrame, sizeof(canmsg_t)) ==
          sizeof(canmsg_t));
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
      C4lClientSetCanSock(Data, IOFKT_INVALID_FD);
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
