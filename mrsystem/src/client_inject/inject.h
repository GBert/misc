#ifndef INJECT_H
#define INJECT_H

#include <boolean.h>

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
} InjectStruct;

#define InjectSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define InjectSetInterface(Data, Iface) (Data)->Interface=Iface
#define InjectSetAddress(Data, Addr)    (Data)->Address=Addr
#define InjectSetServerPort(Data, Port) (Data)->ServerPort=Port
#define InjectSetClientSock(Data, Sock) (Data)->ClientSock=Sock

#define InjectGetVerbose(Data)    (Data)->Verbosity
#define InjectGetInterface(Data)  (Data)->Interface
#define InjectGetAddress(Data)    (Data)->Address
#define InjectGetServerPort(Data) (Data)->ServerPort
#define InjectGetClientSock(Data) (Data)->ClientSock

InjectStruct *InjectCreate(void);
void InjectDestroy(InjectStruct *Data);
void InjectInit(InjectStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port);
void InjectRunCmd(InjectStruct *Data, unsigned long Uid, unsigned Response,
                    unsigned Command, unsigned Prio, int DLC, int *CanBytes);
void InjectRunFile(InjectStruct *Data, char *Filename);

#endif
