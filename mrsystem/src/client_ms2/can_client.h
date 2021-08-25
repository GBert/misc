#ifndef CAN_CLIENT_H
#define CAN_CLIENT_H

#include <boolean.h>
#include "can_io.h"

#define FD_POS_CAN        0x01
#define FD_POS_ENDE       0x02

typedef struct {
   BOOL Verbosity;
   char *CanName;
   int CanSock;
   int FdPos;
} CanClientStruct;

#define CanClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define CanClientSetCanName(Data, Name)    (Data)->CanName=Name
#define CanClientSetCanSock(Data, Sock)    (Data)->CanSock=Sock
#define CanClientSetFdPos(Data, Pos)       (Data)->FdPos=Pos

#define CanClientGetVerbose(Data) (Data)->Verbosity
#define CanClientGetCanName(Data) (Data)->CanName
#define CanClientGetCanSock(Data) (Data)->CanSock
#define CanClientGetFdPos(Data)   (Data)->FdPos

IoFktStruct *CanClientInit(BOOL Verbose, char *CanIf);
void CanClientExit(IoFktStruct *Data);

#endif
