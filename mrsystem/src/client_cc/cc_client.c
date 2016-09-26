#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mr_can.h>
#include "../client_ms2/can_io.h"
#include "cc_client.h"


static int CcClientGetFd(void *private)
{
   return(CcClientGetCanFd((CcClientStruct *)private));
}

static BOOL CcClientOpen(void *private)
{  CcClientStruct *Data;
   char DeviceName[80];
   struct timespec spec;
   long ms;
   time_t s;

   Data = (CcClientStruct *)private;
   strcpy(DeviceName, "/dev/");
   strcat(DeviceName, CcClientGetDevName(Data));
   CcClientSetCanFd(Data, open(DeviceName, O_RDWR));
   if (CcClientGetCanFd(Data) >= 0)
   {
/*
500000 Baud, 8 Datenbits, keine Parität, 1 Stopp-Bit, CTS-Kontrolle "ein"
      if (ioctl(CcClientGetCanSock(Data), SIOCGIFINDEX, &ifr) >= 0)
      {
      }
      else
      {
         close(CcClientGetCanSock(Data));
         CcClientSetCanFd(Data, -1);
      }
*/
      clock_gettime(CLOCK_REALTIME, &spec);
      s = spec.tv_sec;
      ms = (long)(spec.tv_nsec / 1.0e6);
      CcClientSetLastReadMs(Data, ms);
      CcClientSetLastReadSeconds(Data, s);
      CcClientSetReadBytes(Data, 0);
   }
   return(CcClientGetCanFd(Data) >= 0);
}

static void CcClientClose(void *private)
{
   close(CcClientGetCanFd((CcClientStruct *)private));
}

static int CcClientRead(void *private, CanFrameStruct *CanFrame)
{  unsigned char ReadCanFrame[13];
   int i, Ret;
   struct timespec spec;
   long ms;
   time_t s;
   CcClientStruct *Data;

   Data = (CcClientStruct *)private;
   clock_gettime(CLOCK_REALTIME, &spec);
   s = spec.tv_sec;
   ms = (long)(spec.tv_nsec / 1.0e6);
   if ((s - CcClientGetLastReadSeconds(Data)) > 1)
   {
      CcClientSetReadBytes(Data, 0);
   }
   else if (ms != CcClientGetLastReadMs(Data))
   {
      CcClientSetReadBytes(Data, 0);
   }
   Ret = read(CcClientGetCanFd(Data),
              (void *)&CcClientGetReadCanFrame(Data)[CcClientGetReadBytes(Data)],
              MR_CS2_UDP_LENGTH - CcClientGetReadBytes(Data));
   if (Ret > 0)
   {
      CcClientSetReadBytes(Data, CcClientGetReadBytes(Data) + Ret);
      if (CcClientGetReadBytes(Data) == MR_CS2_UDP_LENGTH)
      {
         CanFrame->CanId = ((unsigned long)ReadCanFrame[0] << 24) |
                           ((unsigned long)ReadCanFrame[1] << 16) |
                           ((unsigned long)ReadCanFrame[2] <<  8) |
                           ((unsigned long)ReadCanFrame[3] <<  0);
         CanFrame->CanDlc = ReadCanFrame[4];
         for (i = 0; i < 8; i++)
            CanFrame->CanData[i] = ReadCanFrame[5 + i];
      }
      else
      {
         Ret = 0;
      }
      CcClientSetLastReadMs(Data, ms);
      CcClientSetLastReadSeconds(Data, s);
   }
   return(Ret);
}

static int CcClientWrite(void *private, CanFrameStruct *CanFrame)
{  unsigned char SendCanFrame[13];
   struct timespec Req, Rem;
   int i, Ret;

   SendCanFrame[0] = (CanFrame->CanId >> 24) & 0xff;
   SendCanFrame[1] = (CanFrame->CanId >> 16) & 0xff;
   SendCanFrame[2] = (CanFrame->CanId >>  8) & 0xff;
   SendCanFrame[3] = (CanFrame->CanId >>  0) & 0xff;
   SendCanFrame[4] = CanFrame->CanDlc;
   for (i = 0; i < 8; i++)
      SendCanFrame[5 + i] = CanFrame->CanData[i];
   Ret = write(CcClientGetCanFd((CcClientStruct *)private),
               (const void *)SendCanFrame, sizeof(SendCanFrame));
   Req.tv_sec = 0;
   Req.tv_nsec = 2 * 10e6;
   nanosleep(&Req, &Rem);
   return(Ret);
}

static IoFktStruct CcClientIoFunctions =
{
   NULL,
   CcClientGetFd,
   CcClientOpen,
   CcClientClose,
   CcClientRead,
   CcClientWrite
};

IoFktStruct *CcClientInit(BOOL Verbose, char *DevIf)
{  CcClientStruct *Data;

   Data = (CcClientStruct *)malloc(sizeof(CcClientStruct));
   if (Data != (CcClientStruct *)NULL)
   {
      CcClientSetVerbose(Data, Verbose);
      CcClientSetCanFd(Data, -1);
      CcClientSetDevName(Data, DevIf);
      CcClientIoFunctions.private = (void *)Data;
      return(&CcClientIoFunctions);
   }
   else
      return(NULL);
}

void CcClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
