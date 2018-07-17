#ifndef C4L_CLIENT_H
#define C4L_CLIENT_H

#include <boolean.h>
#include "can_io.h"

#define FD_POS_CAN        0x01
#define FD_POS_ENDE       0x02

typedef struct {
   BOOL Verbosity;
   char *CanName;
   int CanFd;
   int FdPos;
} C4lClientStruct;

#define C4lClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define C4lClientSetCanName(Data, Name)    (Data)->CanName=Name
#define C4lClientSetCanFd(Data, Fd)        (Data)->CanFd=Fd
#define C4lClientSetFdPos(Data, Pos)       (Data)->FdPos=Pos

#define C4lClientGetVerbose(Data) (Data)->Verbosity
#define C4lClientGetCanName(Data) (Data)->CanName
#define C4lClientGetCanFd(Data)   (Data)->CanFd
#define C4lClientGetFdPos(Data)   (Data)->FdPos

IoFktStruct *C4lClientInit(BOOL Verbose, char *CanIf);
void C4lClientExit(IoFktStruct *Data);

#endif
