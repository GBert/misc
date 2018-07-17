#ifndef CS2SL_H
#define CS2SL_H

#include <arpa/inet.h>
#include <boolean.h>
#include "can_io.h"

typedef struct {
   BOOL Verbosity;
#ifdef TRACE
   BOOL Trace;
#endif
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   IoFktStruct *IoFunctions;
} Cs2slStruct;

#define Cs2slSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#ifdef TRACE
#define Cs2slSetTrace(Data, DoTrace)       (Data)->Trace=DoTrace
#endif
#define Cs2slSetInterface(Data, Iface)     (Data)->Interface=Iface
#define Cs2slSetAddress(Data, Addr)        (Data)->Address=Addr
#define Cs2slSetServerPort(Data, Port)     (Data)->ServerPort=Port
#define Cs2slSetClientSock(Data, Sock)     (Data)->ClientSock=Sock
#define Cs2slSetIoFunctions(Data, Fkts)    (Data)->IoFunctions=Fkts

#define Cs2slGetVerbose(Data)       (Data)->Verbosity
#ifdef TRACE
#define Cs2slGetTrace(Data)         (Data)->Trace
#endif
#define Cs2slGetInterface(Data)      (Data)->Interface
#define Cs2slGetAddress(Data)        (Data)->Address
#define Cs2slGetServerPort(Data)     (Data)->ServerPort
#define Cs2slGetClientSock(Data)     (Data)->ClientSock
#define Cs2slGetIoFunctions(Data)    (Data)->IoFunctions

Cs2slStruct *Cs2slCreate(void);
void Cs2slDestroy(Cs2slStruct *Data);
void Cs2slInit(Cs2slStruct *Data, BOOL Verbose, char *Iface, char *Addr,
               int Port,
#ifdef TRACE
               BOOL Trace,
#endif
               IoFktStruct *IoFunctions);
void Cs2slRun(Cs2slStruct *Data);

#endif
