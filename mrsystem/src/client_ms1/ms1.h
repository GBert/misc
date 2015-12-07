#ifndef MS1_H
#define MS1_H

#include <arpa/inet.h>
#include <boolean.h>

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   char *CanName;
   int CanSock;
} Ms1Struct;

#define Ms1SetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define Ms1SetInterface(Data, Iface) (Data)->Interface=Iface
#define Ms1SetAddress(Data, Addr)    (Data)->Address=Addr
#define Ms1SetServerPort(Data, Port) (Data)->ServerPort=Port
#define Ms1SetClientSock(Data, Sock) (Data)->ClientSock=Sock
#define Ms1SetCanName(Data, Name)    (Data)->CanName=Name
#define Ms1SetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define Ms1GetVerbose(Data)    (Data)->Verbosity
#define Ms1GetInterface(Data)  (Data)->Interface
#define Ms1GetAddress(Data)    (Data)->Address
#define Ms1GetServerPort(Data) (Data)->ServerPort
#define Ms1GetClientSock(Data) (Data)->ClientSock
#define Ms1GetCanName(Data)    (Data)->CanName
#define Ms1GetCanSock(Data)    (Data)->CanSock

Ms1Struct *Ms1Create(void);
void Ms1Destroy(Ms1Struct *Data);
void Ms1Init(Ms1Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, char *CanIf);
void Ms1Run(Ms1Struct *Data);

#endif
