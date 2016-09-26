#ifndef CC_CLIENT_H
#define CC_CLIENT_H

#include <boolean.h>
#include <mr_can.h>
#include "../client_ms2/can_io.h"

typedef struct {
   BOOL Verbosity;
   char *DevName;
   int CanFd;
   unsigned char ReadCanFrame[MR_CS2_UDP_LENGTH];
   int ReadBytes;
   long LastReadMs;
   time_t LastReadSeconds;
} CcClientStruct;

#define CcClientSetVerbose(Data, Verbose)   (Data)->Verbosity=Verbose
#define CcClientSetDevName(Data, Device)    (Data)->DevName=Device
#define CcClientSetCanFd(Data, Fd)          (Data)->CanFd=Fd
#define CcClientSetReadBytes(Data, Num)     (Data)->ReadBytes=Num
#define CcClientSetLastReadMs(Data, ms)     (Data)->LastReadMs=ms
#define CcClientSetLastReadSeconds(Data, s) (Data)->LastReadSeconds=s

#define CcClientGetVerbose(Data)         (Data)->Verbosity
#define CcClientGetDevName(Data)         (Data)->DevName
#define CcClientGetCanFd(Data)           (Data)->CanFd
#define CcClientGetReadCanFrame(Data)    (Data)->ReadCanFrame
#define CcClientGetReadBytes(Data)       (Data)->ReadBytes
#define CcClientGetLastReadMs(Data)      (Data)->LastReadMs
#define CcClientGetLastReadSeconds(Data) (Data)->LastReadSeconds

IoFktStruct *CcClientInit(BOOL Verbose, char *DevIf);
void CcClientExit(IoFktStruct *Data);

#endif
