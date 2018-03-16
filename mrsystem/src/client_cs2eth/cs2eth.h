#ifndef CS2ETH_H
#define CS2ETH_H

#include <arpa/inet.h>
#include <menge.h>
#include <boolean.h>

typedef struct {
   BOOL Verbosity;
#ifdef TRACE
   BOOL Trace;
#endif
   BOOL SendUdpBc;
   BOOL UdpConnected;
   char Interface[32];
   char Address[20];
   char BcIp[20];
   int ServerPort;
   int ClientSock;
   int OutsideUdpSock;
   int OutsideTcpSock;
   struct sockaddr_in ClientAddr;
   Menge *Clients;
   MengeIterator *ClientIter;
} Cs2ethStruct;

#define Cs2ethSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#ifdef TRACE
#define Cs2ethSetTrace(Data, DoTrace)       (Data)->Trace=DoTrace
#endif
#define Cs2ethSetSendUdpBc(Data, SendBc)    (Data)->SendUdpBc=SendBc
#define Cs2ethSetUdpConnected(Data, IsCon)  (Data)->UdpConnected=IsCon
#define Cs2ethSetInterface(Data, Iface)     strcpy((Data)->Interface,Iface)
#define Cs2ethSetAddress(Data, Addr)        strcpy((Data)->Address,Addr)
#define Cs2ethSetBcIp(Data, IpAddr)         strcpy((Data)->BcIp,IpAddr)
#define Cs2ethSetServerPort(Data, Port)     (Data)->ServerPort=Port
#define Cs2ethSetClientSock(Data, Sock)     (Data)->ClientSock=Sock
#define Cs2ethSetOutsideUdpSock(Data, Sock) (Data)->OutsideUdpSock=Sock
#define Cs2ethSetOutsideTcpSock(Data, Sock) (Data)->OutsideTcpSock=Sock
#define Cs2ethSetClients(Data, Client)      (Data)->Clients=Client
#define Cs2ethSetClientIter(Data, Iter)     (Data)->ClientIter=Iter

#define Cs2ethGetVerbose(Data)       (Data)->Verbosity
#ifdef TRACE
#define Cs2ethGetTrace(Data)         (Data)->Trace
#endif
#define Cs2ethGetSendUdpBc(Data)      (Data)->SendUdpBc
#define Cs2ethGetUdpConnected(Data)   (Data)->UdpConnected
#define Cs2ethGetInterface(Data)      (Data)->Interface
#define Cs2ethGetAddress(Data)        (Data)->Address
#define Cs2ethGetBcIp(Data)           (Data)->BcIp
#define Cs2ethGetServerPort(Data)     (Data)->ServerPort
#define Cs2ethGetClientSock(Data)     (Data)->ClientSock
#define Cs2ethGetOutsideUdpSock(Data) (Data)->OutsideUdpSock
#define Cs2ethGetOutsideTcpSock(Data) (Data)->OutsideTcpSock
#define Cs2ethGetClientAddr(Data)     (Data)->ClientAddr
#define Cs2ethGetClients(Data)        (Data)->Clients
#define Cs2ethGetClientIter(Data)     (Data)->ClientIter

Cs2ethStruct *Cs2ethCreate(void);
void Cs2ethDestroy(Cs2ethStruct *Data);
void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *Iface, char *Addr,
                int Port, BOOL SendBc, char *BcAddr
#ifdef TRACE
                , BOOL Trace
#endif
               );
void Cs2ethRun(Cs2ethStruct *Data);

#endif
