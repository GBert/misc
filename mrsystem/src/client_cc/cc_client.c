#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mr_cs2ms2.h>
#include <can_io.h>
#include "cc_client.h"


static int CcClientGetFd(void *private)
{  CcClientStruct *Data;
   int ReturnFd;

   Data = (CcClientStruct *)private;
   switch (CcClientGetFdPos(Data))
   {
      case FD_POS_CAN:
         ReturnFd = CcClientGetCanFd(Data);
         CcClientSetFdPos(Data, FD_POS_ENDE);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         CcClientSetFdPos(Data, FD_POS_CAN);
         break;
   }
   return(ReturnFd);
}

static BOOL CcClientOpen(void *private)
{  CcClientStruct *Data;
   char DeviceName[80];
   struct timespec spec;
   long ms;
   time_t s;
   struct termios UsbTerminal;

   Data = (CcClientStruct *)private;
   strcpy(DeviceName, "/dev/");
   strcat(DeviceName, CcClientGetDevName(Data));
   CcClientSetCanFd(Data, open(DeviceName, O_RDWR));
   if (CcClientGetCanFd(Data) >= 0)
   {
      if (tcgetattr(CcClientGetCanFd(Data), &UsbTerminal) == 0)
      {
         cfsetospeed(&UsbTerminal, B500000);
         cfsetispeed(&UsbTerminal, B500000);
         /* disable input processing */
         UsbTerminal.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK |
                                  INPCK | ISTRIP | IXON);
         /* disable output processing */
         UsbTerminal.c_oflag = 0;
         /* hardware flow control */
         UsbTerminal.c_cflag |= CRTSCTS;
         /* set 8 bits, no parity, 1 stop bit */
         UsbTerminal.c_cflag &= ~CSTOPB;
         UsbTerminal.c_cflag &= ~CSIZE;
         UsbTerminal.c_cflag |= CS8;
         UsbTerminal.c_cflag &= ~PARENB;
         /* enable receiver, ignore status lines */
         UsbTerminal.c_cflag |= CREAD | CLOCAL;
         /* disable terminal-generated signals */
         UsbTerminal.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
         UsbTerminal.c_cc[VMIN] = 1;
         tcsetattr(CcClientGetCanFd(Data), TCSAFLUSH, &UsbTerminal);
      }
      clock_gettime(CLOCK_REALTIME, &spec);
      s = spec.tv_sec;
      ms = (long)(spec.tv_nsec / 1.0e6);
      CcClientSetLastReadMs(Data, ms);
      CcClientSetLastReadSeconds(Data, s);
      CcClientSetReadBytes(Data, 0);
      CcClientSetFdPos(Data, FD_POS_CAN);
   }
   return(CcClientGetCanFd(Data) >= 0);
}

static void CcClientClose(void *private)
{  CcClientStruct *Data;

   Data = (CcClientStruct *)private;
   close(CcClientGetCanFd((CcClientStruct *)private));
   CcClientSetCanFd(Data, IOFKT_INVALID_FD);
}

static BOOL CcClientRead(void *private, int fd, BOOL PendingData,
                         MrCs2CanDataType *CanMsg)
{  int Ret;
   struct timespec spec;
   long ms, elapsed_ms;
   time_t s, elapsed_s;
   CcClientStruct *Data;
   BOOL RetVal;

   if (PendingData)
   {
      RetVal = FALSE;
   }
   else
   {
      Data = (CcClientStruct *)private;
      clock_gettime(CLOCK_REALTIME, &spec);
      s = spec.tv_sec;
      ms = (long)(spec.tv_nsec / 1.0e6);
      elapsed_ms = ms - CcClientGetLastReadMs(Data);
      elapsed_s = s - CcClientGetLastReadSeconds(Data);
      if (elapsed_ms < 0)
      {
         elapsed_ms += 1000;
         elapsed_s -= 1;
      }
      if ((elapsed_s > 0) || (elapsed_ms > 50))
      {
         if (CcClientGetReadBytes(Data) != 0)
            puts("CcClientRead: Discard incomplete package");
         CcClientSetReadBytes(Data, 0);
      }
      Ret = read(fd,
                 (void *)&CcClientGetReadCanFrame(Data)[CcClientGetReadBytes(Data)],
                 MR_CS2_UDP_LENGTH - CcClientGetReadBytes(Data));
      if (CcClientGetVerbose(Data))
         printf("CcClientRead: ret %d, %lu s - %lu ms\n",
                Ret, (unsigned long)elapsed_s, (unsigned long)elapsed_ms);
      if (Ret > 0)
      {
         CcClientSetReadBytes(Data, CcClientGetReadBytes(Data) + Ret);
         if (CcClientGetReadBytes(Data) == MR_CS2_UDP_LENGTH)
         {
            MrEthCs2Decode(CanMsg, CcClientGetReadCanFrame(Data));
            CcClientSetReadBytes(Data, 0);
            if (CcClientGetReadBytes(Data) != 0)
               MrCs2DumpCanMsg(CanMsg, "CcClientRead");
            RetVal = TRUE;
         }
         else
         {
            RetVal = FALSE;
         }
         CcClientSetLastReadMs(Data, ms);
         CcClientSetLastReadSeconds(Data, s);
      }
      else
      {
         RetVal = FALSE;
      }
   }
   return(RetVal);
}

static BOOL CcClientWrite(void *private, int ReceiverSocket,
                          MrCs2CanDataType *CanMsg)
{  char SendCanFrame[MR_CS2_UDP_LENGTH];
   struct timespec Req, Rem;
   int Ret;

   if (CcClientGetVerbose((CcClientStruct *)private))
      MrCs2DumpCanMsg(CanMsg, "CcClientWrite");
   MrEthCs2Encode(SendCanFrame, CanMsg);
   Ret = write(CcClientGetCanFd((CcClientStruct *)private),
               (const void *)SendCanFrame, sizeof(SendCanFrame));
   Req.tv_sec = 0;
   Req.tv_nsec = 2 * 10e6;
   nanosleep(&Req, &Rem);
   return(Ret == sizeof(SendCanFrame));
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
      CcClientSetCanFd(Data, IOFKT_INVALID_FD);
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
