#ifndef DREHSCHEIBE_H
#define DREHSCHEIBE_H

#include <boolean.h>
#include <menge.h>
#include <stack.h>

typedef struct {
   int ClientSock;
} DrehscheibeClientStruct;

#define DrehclientSetSock(Data, Sock) (Data)->ClientSock=Sock

#define DrehclientGetSock(Data) (Data)->ClientSock

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ServerSock;
   Menge *SocketClients;
   Stack *RemoveClients;
} DrehscheibeStruct;

#define DrehscheibeSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define DrehscheibeSetInterface(Data, Iface) (Data)->Interface=Iface
#define DrehscheibeSetAddress(Data, Addr)    (Data)->Address=Addr
#define DrehscheibeSetServerPort(Data, Port) (Data)->ServerPort=Port
#define DrehscheibeSetServerSock(Data, Sock) (Data)->ServerSock=Sock
#define DrehscheibeSetClient(Data, Client)   (Data)->SocketClients=Client
#define DrehscheibeSetRemove(Data, Remove)   (Data)->RemoveClients=Remove

#define DrehscheibeGetVerbose(Data)    (Data)->Verbosity
#define DrehscheibeGetInterface(Data)  (Data)->Interface
#define DrehscheibeGetAddress(Data)    (Data)->Address
#define DrehscheibeGetServerPort(Data) (Data)->ServerPort
#define DrehscheibeGetServerSock(Data) (Data)->ServerSock
#define DrehscheibeGetClient(Data)     (Data)->SocketClients
#define DrehscheibeGetRemove(Data)     (Data)->RemoveClients

DrehscheibeStruct *DrehscheibeCreate(void);
void DrehscheibeDestroy(DrehscheibeStruct *Data);
void DrehscheibeInit(DrehscheibeStruct *Data, BOOL Verbose, char *Interface,
                     char *Addr, int Port);
void DrehscheibeRun(DrehscheibeStruct *Data);

#endif
