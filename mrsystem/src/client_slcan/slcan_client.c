#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mr_can.h>
#include "../client_ms2/can_io.h"
#include "slcan_client.h"


static int SlcanClientGetFd(void *private)
{
   return(SlcanClientGetCanFd((SlcanClientStruct *)private));
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

static int SlcanClientRead(void *private, CanFrameStruct *CanFrame)
{  char buffer[255], cmd;
   int i, Ret, DlcPos, tmp;

   i = 0;
   do {
      Ret = read(SlcanClientGetCanFd((SlcanClientStruct *)private),
                 (void *)&buffer[i], 1);
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
            CanFrame->CanDlc = buffer[DlcPos] - '0';
            buffer[DlcPos] = 0;
            CanFrame->CanId = strtoul(buffer + 1, NULL, 16);
            /*
               if (!(cmd & 0x20))
                  CanFrame->CanId |= CAN_EFF_FLAG;
               if ((cmd | 0x20) == 'r')
                  CanFrame->CanId |= CAN_RTR_FLAG;
            */
            for (i = 0, DlcPos++; i < CanFrame->CanDlc; i++)
            {
               tmp = asc2nibble(buffer[DlcPos++]);
               if (tmp > 0x0F)
                  Ret = 0;
               CanFrame->CanData[i] = (tmp << 4);
               tmp = asc2nibble(buffer[DlcPos++]);
               if (tmp > 0x0F)
                  Ret = 0;
               CanFrame->CanData[i] |= tmp;
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
   return(Ret);
}

static int SlcanClientWrite(void *private, CanFrameStruct *CanFrame)
{  char buffer[255];
   int i, idx;

   sprintf(buffer, "T%08lX%d", CanFrame->CanId, CanFrame->CanDlc);
   idx = strlen(buffer);
   for (i = 0; i < CanFrame->CanDlc; i++)
      sprintf(&buffer[idx + 2 * i], "%02X", CanFrame->CanData[i]);
   strcat(buffer, "\r");
   return(write(SlcanClientGetCanFd((SlcanClientStruct *)private),
                (const void *)buffer, strlen(buffer)));
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
