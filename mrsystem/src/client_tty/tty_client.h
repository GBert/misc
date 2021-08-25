#ifndef CC_CLIENT_H
#define CC_CLIENT_H

#include <boolean.h>
#include <mr_cs2ms2.h>
#include "can_io.h"

#define FD_POS_TTY  0x01
#define FD_POS_ENDE 0x02

typedef struct {
   BOOL Verbosity;
   char *DevName;
   int TtyFd;
   unsigned char ReadCanFrame[MR_CS2_UDP_LENGTH];
   int ReadBytes;
   long LastReadMs;
   time_t LastReadSeconds;
   int FdPos;
} TtyClientStruct;

#define TtyClientSetVerbose(Data, Verbose)   (Data)->Verbosity=Verbose
#define TtyClientSetDevName(Data, Device)    (Data)->DevName=Device
#define TtyClientSetTtyFd(Data, Fd)          (Data)->TtyFd=Fd
#define TtyClientSetReadBytes(Data, Num)     (Data)->ReadBytes=Num
#define TtyClientSetLastReadMs(Data, ms)     (Data)->LastReadMs=ms
#define TtyClientSetLastReadSeconds(Data, s) (Data)->LastReadSeconds=s
#define TtyClientSetFdPos(Data, Pos)         (Data)->FdPos=Pos

#define TtyClientGetVerbose(Data)         (Data)->Verbosity
#define TtyClientGetDevName(Data)         (Data)->DevName
#define TtyClientGetTtyFd(Data)           (Data)->TtyFd
#define TtyClientGetReadCanFrame(Data)    (Data)->ReadCanFrame
#define TtyClientGetReadBytes(Data)       (Data)->ReadBytes
#define TtyClientGetLastReadMs(Data)      (Data)->LastReadMs
#define TtyClientGetLastReadSeconds(Data) (Data)->LastReadSeconds
#define TtyClientGetFdPos(Data)           (Data)->FdPos

IoFktStruct *TtyClientInit(BOOL Verbose, char *DevIf);
void TtyClientExit(IoFktStruct *Data);

#endif
