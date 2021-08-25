#ifndef SLCAN_CLIENT_H
#define SLCAN_CLIENT_H

#include <boolean.h>
#include <mr_cs2ms2.h>
#include "can_io.h"

#define FD_POS_CAN        0x01
#define FD_POS_ENDE       0x02

typedef struct {
   BOOL Verbosity;
   char *DevName;
   int CanFd;
   int FdPos;
} SlcanClientStruct;

#define SlcanClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define SlcanClientSetDevName(Data, Device)  (Data)->DevName=Device
#define SlcanClientSetCanFd(Data, Fd)        (Data)->CanFd=Fd
#define SlcanClientSetFdPos(Data, Pos)       (Data)->FdPos=Pos

#define SlcanClientGetVerbose(Data) (Data)->Verbosity
#define SlcanClientGetDevName(Data) (Data)->DevName
#define SlcanClientGetCanFd(Data)   (Data)->CanFd
#define SlcanClientGetFdPos(Data)   (Data)->FdPos

IoFktStruct *SlcanClientInit(BOOL Verbose, char *DevIf);
void SlcanClientExit(IoFktStruct *Data);

#endif
