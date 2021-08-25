#ifndef CS2ETH_H
#define CS2ETH_H

#include <arpa/inet.h>
#include <boolean.h>
#include "can_io.h"

typedef struct {
   BOOL Verbosity;
#ifdef TRACE
   BOOL Trace;
#endif
   char Interface[32];
   char Address[20];
   int ServerPort;
   int ClientSock;
   BOOL HideMs2;
   IoFktStruct *IoFunctions;
} Cs2ethStruct;

#define Cs2ethSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#ifdef TRACE
#define Cs2ethSetTrace(Data, DoTrace)       (Data)->Trace=DoTrace
#endif
#define Cs2ethSetInterface(Data, Iface)     strcpy((Data)->Interface,Iface)
#define Cs2ethSetAddress(Data, Addr)        strcpy((Data)->Address,Addr)
#define Cs2ethSetServerPort(Data, Port)     (Data)->ServerPort=Port
#define Cs2ethSetClientSock(Data, Sock)     (Data)->ClientSock=Sock
#define Cs2ethSetHideMs2(Data, Hide)        (Data)->HideMs2=Hide
#define Cs2ethSetIoFunctions(Data, Fkts)    (Data)->IoFunctions=Fkts

#define Cs2ethGetVerbose(Data)       (Data)->Verbosity
#ifdef TRACE
#define Cs2ethGetTrace(Data)         (Data)->Trace
#endif
#define Cs2ethGetInterface(Data)      (Data)->Interface
#define Cs2ethGetAddress(Data)        (Data)->Address
#define Cs2ethGetServerPort(Data)     (Data)->ServerPort
#define Cs2ethGetClientSock(Data)     (Data)->ClientSock
#define Cs2ethGetHideMs2(Data)        (Data)->HideMs2
#define Cs2ethGetIoFunctions(Data)    (Data)->IoFunctions

Cs2ethStruct *Cs2ethCreate(void);
void Cs2ethDestroy(Cs2ethStruct *Data);
void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port, char *HideMs2,
#ifdef TRACE
                BOOL Trace,
#endif
                IoFktStruct *IoFunctions);
void Cs2ethRun(Cs2ethStruct *Data);

#endif
