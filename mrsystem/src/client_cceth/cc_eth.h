#ifndef CAN_ETH_H
#define CAN_ETH_H

#include <arpa/inet.h>
#include <menge.h>
#include <boolean.h>
#include "can_io.h"

#define FD_POS_TCP_SERVER 0x01
#define FD_POS_TCP_CLIENT 0x02
#define FD_POS_ENDE       0x03

typedef struct {
   int ClientSock;
   struct sockaddr_in ClientAddr;
} ClientInfo;

typedef struct {
   BOOL Verbosity;
   int OutsideTcpPort;
   int OutsideTcpSock;
   int ClientTcpSock;
   struct sockaddr_in ClientAddr;
   Menge *Clients;
   MengeIterator *ClientIter;
   MengeIterator *FdSearchIter;
   int FdPos;
   ClientInfo *FdSearchClient;
} CcEthStruct;

#define CcEthSetVerbose(Data, Verbose)       (Data)->Verbosity=Verbose
#define CcEthSetOutsideTcpPort(Data, Port)   (Data)->OutsideTcpPort=Port
#define CcEthSetOutsideTcpSock(Data, Sock)   (Data)->OutsideTcpSock=Sock
#define CcEthSetClientTcpSock(Data, Sock)    (Data)->ClientTcpSock=Sock
#define CcEthSetClients(Data, Client)        (Data)->Clients=Client
#define CcEthSetClientIter(Data, Iter)       (Data)->ClientIter=Iter
#define CcEthSetFdSearchIter(Data, Iter)     (Data)->FdSearchIter=Iter
#define CcEthSetFdPos(Data, Pos)             (Data)->FdPos=Pos
#define CcEthSetFdSearchClient(Data, Client) (Data)->FdSearchClient=Client

#define CcEthGetVerbose(Data)        (Data)->Verbosity
#define CcEthGetOutsideTcpPort(Data) (Data)->OutsideTcpPort
#define CcEthGetOutsideTcpSock(Data) (Data)->OutsideTcpSock
#define CcEthGetClientTcpSock(Data)  (Data)->ClientTcpSock
#define CcEthGetClientAddr(Data)     (Data)->ClientAddr
#define CcEthGetClients(Data)        (Data)->Clients
#define CcEthGetClientIter(Data)     (Data)->ClientIter
#define CcEthGetFdSearchIter(Data)   (Data)->FdSearchIter
#define CcEthGetFdPos(Data)          (Data)->FdPos
#define CcEthGetFdSearchClient(Data) (Data)->FdSearchClient

IoFktStruct *CcEthInit(BOOL Verbose, int Port);
void CcEthExit(IoFktStruct *Data);

#endif
