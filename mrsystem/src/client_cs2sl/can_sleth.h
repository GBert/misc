#ifndef CAN_SLETH_H
#define CAN_SLETH_H

#include <arpa/inet.h>
#include <boolean.h>
#include "can_io.h"

#define FD_POS_UDP        0x01
#define FD_POS_TCP        0x02
#define FD_POS_ENDE       0x03

typedef struct {
   BOOL Verbosity;
   BOOL Cs2Addr;
   BOOL DoTcp;
   int OutsideUdpSock;
   int OutsideTcpSock;
   struct sockaddr_in ClientAddr;
   int FdPos;
} SlethClientStruct;

#define SlethClientSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#define SlethClientSetCs2Addr(Data, HaveCs2)     (Data)->Cs2Addr=HaveCs2
#define SlethClientSetDoTcp(Data, Tcp)           (Data)->DoTcp=Tcp
#define SlethClientSetOutsideUdpSock(Data, Sock) (Data)->OutsideUdpSock=Sock
#define SlethClientSetOutsideTcpSock(Data, Sock) (Data)->OutsideTcpSock=Sock
#define SlethClientSetFdPos(Data, Pos)           (Data)->FdPos=Pos

#define SlethClientGetVerbose(Data)        (Data)->Verbosity
#define SlethClientGetCs2Addr(Data)        (Data)->Cs2Addr
#define SlethClientGetDoTcp(Data)          (Data)->DoTcp
#define SlethClientGetOutsideUdpSock(Data) (Data)->OutsideUdpSock
#define SlethClientGetOutsideTcpSock(Data) (Data)->OutsideTcpSock
#define SlethClientGetClientAddr(Data)     (Data)->ClientAddr
#define SlethClientGetFdPos(Data)          (Data)->FdPos

IoFktStruct *SlethClientInit(BOOL Verbose, BOOL DoTcp);
void SlethClientExit(IoFktStruct *Data);

#endif
