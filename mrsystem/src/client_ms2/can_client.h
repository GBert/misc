#ifndef CAN_CLIENT_H
#define CAN_CLIENT_H

#include <boolean.h>
#include "can_io.h"

typedef struct {
   BOOL Verbosity;
   char *CanName;
   int CanSock;
} CanClientStruct;

#define CanClientSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define CanClientSetCanName(Data, Name)    (Data)->CanName=Name
#define CanClientSetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define CanClientGetVerbose(Data) (Data)->Verbosity
#define CanClientGetCanName(Data) (Data)->CanName
#define CanClientGetCanSock(Data) (Data)->CanSock

IoFktStruct *CanClientInit(BOOL Verbose, char *CanIf);
void CanClientExit(IoFktStruct *Data);

#endif
