#ifndef CC_CLIENT_H
#define CC_CLIENT_H

#include <boolean.h>
#include <mr_cs2ms2.h>
#include <can_io.h>

#define FD_POS_CAN        0x01
#define FD_POS_ENDE       0x02

typedef struct {
   BOOL Verbosity;
   char *DevName;
   int CanFd;
   char ReadCanFrame[MR_CS2_UDP_LENGTH];
   int ReadBytes;
   long LastReadMs;
   time_t LastReadSeconds;
   int FdPos;
} CcClientStruct;

#define CcClientSetVerbose(Data, Verbose)   (Data)->Verbosity=Verbose
#define CcClientSetDevName(Data, Device)    (Data)->DevName=Device
#define CcClientSetCanFd(Data, Fd)          (Data)->CanFd=Fd
#define CcClientSetReadBytes(Data, Num)     (Data)->ReadBytes=Num
#define CcClientSetLastReadMs(Data, ms)     (Data)->LastReadMs=ms
#define CcClientSetLastReadSeconds(Data, s) (Data)->LastReadSeconds=s
#define CcClientSetFdPos(Data, Pos)         (Data)->FdPos=Pos

#define CcClientGetVerbose(Data)         (Data)->Verbosity
#define CcClientGetDevName(Data)         (Data)->DevName
#define CcClientGetCanFd(Data)           (Data)->CanFd
#define CcClientGetReadCanFrame(Data)    (Data)->ReadCanFrame
#define CcClientGetReadBytes(Data)       (Data)->ReadBytes
#define CcClientGetLastReadMs(Data)      (Data)->LastReadMs
#define CcClientGetLastReadSeconds(Data) (Data)->LastReadSeconds
#define CcClientGetFdPos(Data)           (Data)->FdPos

IoFktStruct *CcClientInit(BOOL Verbose, char *DevIf);
void CcClientExit(IoFktStruct *Data);

#endif
