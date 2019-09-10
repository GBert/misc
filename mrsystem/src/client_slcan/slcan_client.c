#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mr_cs2ms2.h>
#include "can_io.h"
#include "slcan_client.h"


static int SlcanClientGetFd(void *private)
{  SlcanClientStruct *Data;
   int ReturnFd;

   Data = (SlcanClientStruct *)private;
   switch (SlcanClientGetFdPos(Data))
   {
      case FD_POS_CAN:
         ReturnFd = SlcanClientGetCanFd(Data);
         SlcanClientSetFdPos(Data, FD_POS_ENDE);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         SlcanClientSetFdPos(Data, FD_POS_CAN);
         break;
   }
   return(ReturnFd);
}

static BOOL SlcanClientOpen(void *private)
{  SlcanClientStruct *Data;
   char DeviceName[80];

   Data = (SlcanClientStruct *)private;
   strcpy(DeviceName, "/dev/");
   strcat(DeviceName, SlcanClientGetDevName(Data));
   SlcanClientSetCanFd(Data, open(DeviceName, O_RDWR));
   if (SlcanClientGetCanFd(Data) >= 0)
   {
   }
   return(SlcanClientGetCanFd(Data) >= 0);
}

static void SlcanClientClose(void *private)
{
   close(SlcanClientGetCanFd((SlcanClientStruct *)private));
}

static int asc2nibble(char c)
{
   if ((c >= '0') && (c <= '9'))
      return c - '0';
   else if ((c >= 'A') && (c <= 'F'))
      return c - 'A' + 10;
   else if ((c >= 'a') && (c <= 'f'))
      return c - 'a' + 10;
   else
      return 16;
}

static BOOL SlcanClientRead(void *private, int fd, BOOL PendingData,
                            MrCs2CanDataType *CanMsg)
{  char buffer[255], cmd;
   int i, Ret, DlcPos, tmp;

   if (PendingData)
   {
      Ret = -1;
   }
   else
   {
      i = 0;
      do {
         Ret = read(fd, (void *)&buffer[i], 1);
         i++;
      } while ((i < 255) && (Ret > 0) &&
               (buffer[i-1] != '\n') && (buffer[i-1] != '\r'));
      if ((i < 255) && (Ret > 0) &&
          ((buffer[i-1] == '\n') || (buffer[i-1] == '\r')))
      {
         cmd = buffer[0];
         if ((cmd == 't') || (cmd == 'T') || (cmd == 'r') || (cmd == 'R'))
         {
            if (cmd & 0x20)
               DlcPos = 4;
            else
               DlcPos = 9;
            if ((buffer[DlcPos] >= '0') && (buffer[DlcPos] < '9'))
            {
               MrCs2SetDlc(CanMsg, buffer[DlcPos] - '0');
               buffer[DlcPos] = 0;
               MrCs2SetId(CanMsg, strtoul(buffer + 1, NULL, 16));
               /*
                  if (!(cmd & 0x20))
                     CanFrame->CanId |= CAN_EFF_FLAG;
                  if ((cmd | 0x20) == 'r')
                     CanFrame->CanId |= CAN_RTR_FLAG;
               */
               for (i = 0, DlcPos++; i < MrCs2GetDlc(CanMsg); i++)
               {
                  tmp = asc2nibble(buffer[DlcPos++]);
                  if (tmp > 0x0F)
                     Ret = 0;
                  MrCs2GetData(CanMsg)[i] = (tmp << 4);
                  tmp = asc2nibble(buffer[DlcPos++]);
                  if (tmp > 0x0F)
                     Ret = 0;
                  MrCs2GetData(CanMsg)[i] |= tmp;
               }
            }
            else
            {
               Ret = 0;
            }
         }
         else
         {
            Ret = 0;
         }
      }
   }
   return(Ret > 0);
}

static BOOL SlcanClientWrite(void *private, int ReceiverSocket,
                             MrCs2CanDataType *CanMsg)
{  char buffer[255];
   int i, idx;

   sprintf(buffer, "T%08lX%d", MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
   idx = strlen(buffer);
   for (i = 0; i < MrCs2GetDlc(CanMsg); i++)
      sprintf(&buffer[idx + 2 * i], "%02X", MrCs2GetData(CanMsg)[i]);
   strcat(buffer, "\r");
   return(write(SlcanClientGetCanFd((SlcanClientStruct *)private),
                (const void *)buffer, strlen(buffer)) == strlen(buffer));
}

static IoFktStruct SlcanClientIoFunctions =
{
   NULL,
   SlcanClientGetFd,
   SlcanClientOpen,
   SlcanClientClose,
   SlcanClientRead,
   SlcanClientWrite
};

IoFktStruct *SlcanClientInit(BOOL Verbose, char *CanIf)
{  SlcanClientStruct *Data;

   Data = (SlcanClientStruct *)malloc(sizeof(SlcanClientStruct));
   if (Data != (SlcanClientStruct *)NULL)
   {
      SlcanClientSetVerbose(Data, Verbose);
      SlcanClientSetCanFd(Data, -1);
      SlcanClientSetDevName(Data, CanIf);
      SlcanClientIoFunctions.private = (void *)Data;
      return(&SlcanClientIoFunctions);
   }
   else
      return(NULL);
}

void SlcanClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
