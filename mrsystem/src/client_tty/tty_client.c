#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include "can_io.h"
#include "tty_client.h"


static int TtyClientGetFd(void *private)
{  TtyClientStruct *Data;
   int ReturnFd;

   Data = (TtyClientStruct *)private;
   switch (TtyClientGetFdPos(Data))
   {
      case FD_POS_TTY:
         ReturnFd = TtyClientGetTtyFd(Data);
         TtyClientSetFdPos(Data, FD_POS_ENDE);
         break;
      case FD_POS_ENDE:
         ReturnFd = IOFKT_INVALID_FD;
         TtyClientSetFdPos(Data, FD_POS_TTY);
         break;
   }
   return(ReturnFd);
}

static BOOL TtyClientOpen(void *private)
{  TtyClientStruct *Data;
   char DeviceName[80], *BaudPtr;
   int Baud;
   struct termios SerialTerminal;
   struct timespec spec;
   long ms;
   time_t s;

   Data = (TtyClientStruct *)private;
   strcpy(DeviceName, "/dev/");
   strcat(DeviceName, TtyClientGetDevName(Data));
   /* parse name to strip name from speed parameter */
   BaudPtr = strtok(DeviceName, ",");
   /* skip first token which is name of serial device */
   BaudPtr = strtok(NULL, ",");
   if (BaudPtr == NULL)
   {
      if (TtyClientGetVerbose(Data))
         puts("no baud given, use default 500000");
      Baud = B500000;
   }
   else
   {
      if (TtyClientGetVerbose(Data))
         printf("use %s baud\n", BaudPtr);
      switch (atol(BaudPtr))
      {
         case 0:
            Baud = B0; /* hang up */
            break;
         case 50:
            Baud = B50;
            break;
         case 75:
            Baud = B75;
            break;
         case 110:
            Baud = B110;
            break;
         case 134:
            Baud = B134;
            break;
         case 150:
            Baud = B150;
            break;
         case 200:
            Baud = B200;
            break;
         case 300:
            Baud = B300;
            break;
         case 600:
            Baud = B600;
            break;
         case 1200:
            Baud = B1200;
            break;
         case 1800:
            Baud = B1800;
            break;
         case 2400:
            Baud = B2400;
            break;
         case 4800:
            Baud = B4800;
            break;
         case 9600:
            Baud = B9600;
            break;
         case 19200:
            Baud = B19200;
            break;
         case 38400:
            Baud = B38400;
            break;
         case 57600:
            Baud = B57600;
            break;
         case 115200:
            Baud = B115200;
            break;
         case 230400:
            Baud = B230400;
            break;
         case 460800:
            Baud = B460800;
            break;
         case 500000:
            Baud = B500000;
            break;
         case 576000:
            Baud = B576000;
            break;
         case 921600:
            Baud = B921600;
            break;
         case 1000000:
            Baud = B1000000;
            break;
         case 1152000:
            Baud = B1152000;
            break;
         case 1500000:
            Baud = B1500000;
            break;
         case 2000000:
            Baud = B2000000;
            break;
         case 2500000:
            Baud = B2500000;
            break;
         case 3000000:
            Baud = B3000000;
            break;
         case 3500000:
            Baud = B3500000;
            break;
         case 4000000:
            Baud = B4000000;
            break;
         default:
            Baud = B115200;
      }
   }
   /* open serial device */
   TtyClientSetTtyFd(Data, open(DeviceName, O_RDWR | O_NONBLOCK));
   if (TtyClientGetTtyFd(Data) >= 0)
   {
      if (isatty(TtyClientGetTtyFd(Data)))
      {
         if (TtyClientGetVerbose(Data))
            puts("get terminal settings");
         if (tcgetattr(TtyClientGetTtyFd(Data), &SerialTerminal) == 0)
         {
            if (TtyClientGetVerbose(Data))
               puts("set terminal settings");
            cfsetospeed(&SerialTerminal, Baud);
            cfmakeraw(&SerialTerminal);
            SerialTerminal.c_iflag &= ~IXON;
            SerialTerminal.c_oflag &= ~IXON;
            SerialTerminal.c_cflag |= CRTSCTS;
            if ((tcsetattr(TtyClientGetTtyFd(Data), TCSAFLUSH, &SerialTerminal) < 0) &&
                TtyClientGetVerbose(Data))
            {
               puts("can not set terminal settings");
               printf("%s\n", strerror(errno));
            }
         }
         else
         {
            if (TtyClientGetVerbose(Data))
               printf("can not get termios for %s\n", DeviceName);
         }
      }
      clock_gettime(CLOCK_REALTIME, &spec);
      s = spec.tv_sec;
      ms = (long)(spec.tv_nsec / 1.0e6);
      TtyClientSetLastReadMs(Data, ms);
      TtyClientSetLastReadSeconds(Data, s);
      TtyClientSetReadBytes(Data, 0);
      TtyClientSetFdPos(Data, FD_POS_TTY);
   }
   return(TtyClientGetTtyFd(Data) >= 0);
}

static void TtyClientClose(void *private)
{  TtyClientStruct *Data;

   Data = (TtyClientStruct *)private;
   close(TtyClientGetTtyFd(Data));
   TtyClientSetTtyFd(Data, IOFKT_INVALID_FD);
}

static BOOL TtyClientRead(void *private, int fd, BOOL PendingData,
                          MrCs2CanDataType *CanMsg)
{  unsigned char ReadCanFrame[13];
   int i, Ret;
   struct timespec spec;
   long ms;
   time_t s;
   TtyClientStruct *Data;
   BOOL RetVal;
   unsigned CanHash, Response, Command, Prio;

   if (PendingData)
   {
      RetVal = FALSE;
   }
   else
   {
      Data = (TtyClientStruct *)private;
      clock_gettime(CLOCK_REALTIME, &spec);
      s = spec.tv_sec;
      ms = (long)(spec.tv_nsec / 1.0e6);
      if ((s - TtyClientGetLastReadSeconds(Data)) > 1)
      {
         TtyClientSetReadBytes(Data, 0);
      }
      else if (ms != TtyClientGetLastReadMs(Data))
      {
         TtyClientSetReadBytes(Data, 0);
      }
      Ret = read(fd,
                 (void *)&TtyClientGetReadCanFrame(Data)[TtyClientGetReadBytes(Data)],
              MR_CS2_UDP_LENGTH - TtyClientGetReadBytes(Data));
      if (Ret > 0)
      {
         TtyClientSetReadBytes(Data, TtyClientGetReadBytes(Data) + Ret);
         if (TtyClientGetReadBytes(Data) == MR_CS2_UDP_LENGTH)
         {
            MrCs2SetId(CanMsg, ((unsigned long)ReadCanFrame[0] << 24) |
                               ((unsigned long)ReadCanFrame[1] << 16) |
                               ((unsigned long)ReadCanFrame[2] <<  8) |
                               ((unsigned long)ReadCanFrame[3] <<  0));
            MrCs2SetDlc(CanMsg, ReadCanFrame[4]);
            for (i = 0; i < 8; i++)
               MrCs2GetData(CanMsg)[i] = ReadCanFrame[5 + i];
            Cs2DecodeId(MrCs2GetId(CanMsg), &CanHash, &Response, &Command, &Prio);
            MrCs2SetHash(CanMsg, CanHash & ~CS2_MASK_HASH_MAGIC);
            MrCs2SetResponse(CanMsg, Response);
            MrCs2SetCommand(CanMsg, Command);
            MrCs2SetPrio(CanMsg, Prio);
            MrCs2SetIsCs2(CanMsg, Cs2IsCs2Msg(CanHash, MrCs2GetCommand(CanMsg)));
            RetVal = TRUE;
         }
         else
         {
            RetVal = FALSE;
         }
         TtyClientSetLastReadMs(Data, ms);
         TtyClientSetLastReadSeconds(Data, s);
      }
      else
      {
         RetVal = FALSE;
      }
   }
   return(RetVal);
}

static BOOL TtyClientWrite(void *private, int ReceiverSocket,
                           MrCs2CanDataType *CanMsg)
{  unsigned char SendCanFrame[13];
   struct timespec Req, Rem;
   int i, Ret;

   SendCanFrame[0] = (MrCs2GetId(CanMsg) >> 24) & 0xff;
   SendCanFrame[1] = (MrCs2GetId(CanMsg) >> 16) & 0xff;
   SendCanFrame[2] = (MrCs2GetId(CanMsg) >>  8) & 0xff;
   SendCanFrame[3] = (MrCs2GetId(CanMsg) >>  0) & 0xff;
   SendCanFrame[4] = MrCs2GetDlc(CanMsg);
   for (i = 0; i < 8; i++)
      SendCanFrame[5 + i] = MrCs2GetData(CanMsg)[i];
   Ret = write(TtyClientGetTtyFd((TtyClientStruct *)private),
               (const void *)SendCanFrame, sizeof(SendCanFrame));
   Req.tv_sec = 0;
   Req.tv_nsec = 2 * 10e6;
   nanosleep(&Req, &Rem);
   return(Ret == sizeof(SendCanFrame));
}

static IoFktStruct TtyClientIoFunctions =
{
   NULL,
   TtyClientGetFd,
   TtyClientOpen,
   TtyClientClose,
   TtyClientRead,
   TtyClientWrite
};

IoFktStruct *TtyClientInit(BOOL Verbose, char *DevIf)
{  TtyClientStruct *Data;

   Data = (TtyClientStruct *)malloc(sizeof(TtyClientStruct));
   if (Data != (TtyClientStruct *)NULL)
   {
      TtyClientSetVerbose(Data, Verbose);
      TtyClientSetTtyFd(Data, IOFKT_INVALID_FD);
      TtyClientSetDevName(Data, DevIf);
      TtyClientIoFunctions.private = (void *)Data;
      return(&TtyClientIoFunctions);
   }
   else
      return(NULL);
}

void TtyClientExit(IoFktStruct *Data)
{
   if (Data->private != (void *)NULL)
      free(Data->private);
}
