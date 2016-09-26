#ifndef MS2_H
#define MS2_H

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
   BOOL IsZentrale;
} Ms2Struct;

#define Ms2SetVerbose(Data, Verbose)   (Data)->Verbosity=Verbose
#define Ms2SetInterface(Data, Iface)   (Data)->Interface=Iface
#define Ms2SetAddress(Data, Addr)      (Data)->Address=Addr
#define Ms2SetServerPort(Data, Port)   (Data)->ServerPort=Port
#define Ms2SetClientSock(Data, Sock)   (Data)->ClientSock=Sock
#define Ms2SetCanName(Data, Name)      (Data)->CanName=Name
#define Ms2SetCanSock(Data, Sock)      (Data)->CanSock=Sock
#define Ms2SetZentrale(Data, Zentrale) (Data)->IsZentrale=Zentrale

#define Ms2GetVerbose(Data)    (Data)->Verbosity
#define Ms2GetInterface(Data)  (Data)->Interface
#define Ms2GetAddress(Data)    (Data)->Address
#define Ms2GetServerPort(Data) (Data)->ServerPort
#define Ms2GetClientSock(Data) (Data)->ClientSock
#define Ms2GetCanName(Data)    (Data)->CanName
#define Ms2GetCanSock(Data)    (Data)->CanSock
#define Ms2GetZentrale(Data)   (Data)->IsZentrale

Ms2Struct *Ms2Create(void);
void Ms2Destroy(Ms2Struct *Data);
void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, char *CanIf, BOOL IsZentrale);
void Ms2Run(Ms2Struct *Data);

#endif
