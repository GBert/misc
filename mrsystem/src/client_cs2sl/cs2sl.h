#ifndef CS2SL_H
#define CS2SL_H

#include <arpa/inet.h>
#include <boolean.h>

typedef struct {
   BOOL Verbosity;
#ifdef TRACE
   BOOL Trace;
#endif
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   BOOL Cs2Addr;
   BOOL DoTcp;
   int OutsideUdpSock;
   int OutsideBcSock;
   int OutsideTcpSock;
   struct sockaddr_in ClientAddr;
} Cs2slStruct;

#define Cs2slSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#ifdef TRACE
#define Cs2slSetTrace(Data, DoTrace)       (Data)->Trace=DoTrace
#endif
#define Cs2slSetInterface(Data, Iface)     (Data)->Interface=Iface
#define Cs2slSetAddress(Data, Addr)        (Data)->Address=Addr
#define Cs2slSetServerPort(Data, Port)     (Data)->ServerPort=Port
#define Cs2slSetClientSock(Data, Sock)     (Data)->ClientSock=Sock
#define Cs2slSetCs2Addr(Data, HaveCs2)     (Data)->Cs2Addr=HaveCs2
#define Cs2slSetDoTcp(Data, Tcp)           (Data)->DoTcp=Tcp
#define Cs2slSetOutsideUdpSock(Data, Sock) (Data)->OutsideUdpSock=Sock
#define Cs2slSetOutsideBcSock(Data, Sock)  (Data)->OutsideBcSock=Sock
#define Cs2slSetOutsideTcpSock(Data, Sock) (Data)->OutsideTcpSock=Sock

#define Cs2slGetVerbose(Data)       (Data)->Verbosity
#ifdef TRACE
#define Cs2slGetTrace(Data)         (Data)->Trace
#endif
#define Cs2slGetInterface(Data)      (Data)->Interface
#define Cs2slGetAddress(Data)        (Data)->Address
#define Cs2slGetServerPort(Data)     (Data)->ServerPort
#define Cs2slGetClientSock(Data)     (Data)->ClientSock
#define Cs2slGetCs2Addr(Data)        (Data)->Cs2Addr
#define Cs2slGetDoTcp(Data)          (Data)->DoTcp
#define Cs2slGetOutsideUdpSock(Data) (Data)->OutsideUdpSock
#define Cs2slGetOutsideBcSock(Data)  (Data)->OutsideBcSock
#define Cs2slGetOutsideTcpSock(Data) (Data)->OutsideTcpSock
#define Cs2slGetClientAddr(Data)     (Data)->ClientAddr

Cs2slStruct *Cs2slCreate(void);
void Cs2slDestroy(Cs2slStruct *Data);
void Cs2slInit(Cs2slStruct *Data, BOOL Verbose, char *Iface, char *Addr,
               int Port, BOOL DoTcp
#ifdef TRACE
               , BOOL Trace
#endif
              );
void Cs2slRun(Cs2slStruct *Data);

#endif
