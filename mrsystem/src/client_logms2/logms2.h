#ifndef LOGMS2_H
#define LOGMS2_H

#include <boolean.h>
#include "../client_ms2/can_io.h"

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   IoFktStruct *IoFunctions;
} Logms2Struct;

#define Logms2SetVerbose(Data, Verbose)  (Data)->Verbosity=Verbose
#define Logms2SetInterface(Data, Iface)  (Data)->Interface=Iface
#define Logms2SetAddress(Data, Addr)     (Data)->Address=Addr
#define Logms2SetServerPort(Data, Port)  (Data)->ServerPort=Port
#define Logms2SetClientSock(Data, Sock)  (Data)->ClientSock=Sock
#define Logms2SetIoFunctions(Data, Fkts) (Data)->IoFunctions=Fkts

#define Logms2GetVerbose(Data)     (Data)->Verbosity
#define Logms2GetInterface(Data)   (Data)->Interface
#define Logms2GetAddress(Data)     (Data)->Address
#define Logms2GetServerPort(Data)  (Data)->ServerPort
#define Logms2GetClientSock(Data)  (Data)->ClientSock
#define Logms2GetIoFunctions(Data) (Data)->IoFunctions

Logms2Struct *Logms2Create(void);
void Logms2Destroy(Logms2Struct *Data);
void Logms2Init(Logms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
                int Port, IoFktStruct *IoFunctions);
void Logms2Run(Logms2Struct *Data);

#endif
