#ifndef MS1_H
#define MS1_H

#include <boolean.h>
#include "../client_ms2/can_io.h"

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   IoFktStruct *IoFunctions;
} Ms1Struct;
 
#define Ms1SetVerbose(Data, Verbose)  (Data)->Verbosity=Verbose
#define Ms1SetInterface(Data, Iface)  (Data)->Interface=Iface
#define Ms1SetAddress(Data, Addr)     (Data)->Address=Addr
#define Ms1SetServerPort(Data, Port)  (Data)->ServerPort=Port
#define Ms1SetClientSock(Data, Sock)  (Data)->ClientSock=Sock
#define Ms1SetIoFunctions(Data, Fkts) (Data)->IoFunctions=Fkts

#define Ms1GetVerbose(Data)     (Data)->Verbosity
#define Ms1GetInterface(Data)   (Data)->Interface
#define Ms1GetAddress(Data)     (Data)->Address
#define Ms1GetServerPort(Data)  (Data)->ServerPort
#define Ms1GetClientSock(Data)  (Data)->ClientSock
#define Ms1GetIoFunctions(Data) (Data)->IoFunctions

Ms1Struct *Ms1Create(void);
void Ms1Destroy(Ms1Struct *Data);
void Ms1Init(Ms1Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, IoFktStruct *IoFunctions);
void Ms1Run(Ms1Struct *Data);

#endif
