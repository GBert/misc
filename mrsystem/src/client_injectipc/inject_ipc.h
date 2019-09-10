#ifndef INJECTIPC_H
#define INJECTIPC_H

#include <boolean.h>
#include <mr_ipc.h>

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
} InjectIpcStruct;

#define InjectIpcSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define InjectIpcSetInterface(Data, Iface) (Data)->Interface=Iface
#define InjectIpcSetAddress(Data, Addr)    (Data)->Address=Addr
#define InjectIpcSetServerPort(Data, Port) (Data)->ServerPort=Port
#define InjectIpcSetClientSock(Data, Sock) (Data)->ClientSock=Sock

#define InjectIpcGetVerbose(Data)    (Data)->Verbosity
#define InjectIpcGetInterface(Data)  (Data)->Interface
#define InjectIpcGetAddress(Data)    (Data)->Address
#define InjectIpcGetServerPort(Data) (Data)->ServerPort
#define InjectIpcGetClientSock(Data) (Data)->ClientSock

InjectIpcStruct *InjectIpcCreate(void);
void InjectIpcDestroy(InjectIpcStruct *Data);
void InjectIpcInit(InjectIpcStruct *Data, BOOL Verbose, char *Iface,
                   char *Addr, int Port);
void InjectIpcRunCmd(InjectIpcStruct *Data, MrIpcCommandValue Command,
                     int SenderSocket, int ReceiverSocket,
                     unsigned int CanResponse, unsigned int CanHash,
                     unsigned int CanCommand, unsigned int CanPrio,
                     unsigned long Lp1, unsigned int Ip1, unsigned int Ip2);
void InjectIpcRunFile(InjectIpcStruct *Data, char *Filename);

#endif
