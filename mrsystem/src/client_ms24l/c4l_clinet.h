#ifndef C4L_CLIENT_H
#define C4L_CLIENT_H

#include <boolean.h>
#include "../client_ms2/can_io.h"

typedef struct {
   BOOL Verbosity;
   char *CanName;
   int CanFd;
} C4lClientStruct;

#define C4lClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define C4lClientSetCanName(Data, Name)    (Data)->CanName=Name
#define C4lClientSetCanFd(Data, Fd)        (Data)->CanFd=Fd

#define C4lClientGetVerbose(Data) (Data)->Verbosity
#define C4lClientGetCanName(Data) (Data)->CanName
#define C4lClientGetCanFd(Data)   (Data)->CanFd

IoFktStruct *C4lClientInit(BOOL Verbose, char *CanIf);
void C4lClientExit(IoFktStruct *Data);

#endif
