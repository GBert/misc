#ifndef SLCAN_CLIENT_H
#define SLCAN_CLIENT_H

#include <boolean.h>
#include <mr_can.h>
#include "../client_ms2/can_io.h"

typedef struct {
   BOOL Verbosity;
   char *DevName;
   int CanFd;
} SlcanClientStruct;

#define SlcanClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define SlcanClientSetDevName(Data, Device)  (Data)->DevName=Device
#define SlcanClientSetCanFd(Data, Fd)        (Data)->CanFd=Fd

#define SlcanClientGetVerbose(Data) (Data)->Verbosity
#define SlcanClientGetDevName(Data) (Data)->DevName
#define SlcanClientGetCanFd(Data)   (Data)->CanFd

IoFktStruct *SlcanClientInit(BOOL Verbose, char *DevIf);
void SlcanClientExit(IoFktStruct *Data);

#endif
