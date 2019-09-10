#ifndef MR_SRCP_H
#define MR_SRCP_H

#include <arpa/inet.h>
#include <boolean.h>
#include <menge.h>
#include <fsm.h>
#include <can_io.h>

#define FD_POS_TCP_SERVER 0x01
#define FD_POS_TCP_CLIENT 0x02
#define FD_POS_ENDE       0x03
#define SCRP_CLIENT_MODE_COMMAND 1
#define SCRP_CLIENT_MODE_INFO    2
#define MAX_PENDING_CAN_NUM 17

typedef struct {
   int ClientSock;
   struct sockaddr_in ClientAddr;
   int ClientMode;
   FsmStruct *StateMachine;
   time_t LastMsgTstamp;
} ClientInfo;

#define SrcpClientSetClientSock(Data, Sock)  (Data)->ClientSock=Sock
#define SrcpClientSetClientMode(Data, Mode)  (Data)->ClientMode=Mode
#define SrcpClientSetStateMachine(Data, Fsm) (Data)->StateMachine=Fsm
#define SrcpClientSetLastMsgTstamp(Data, Ts) (Data)->LastMsgTstamp=Ts

#define SrcpClientGetClientSock(Data)    (Data)->ClientSock
#define SrcpClientGetClientAddr(Data)    (Data)->ClientAddr
#define SrcpClientGetClientMode(Data)    (Data)->ClientMode
#define SrcpClientGetStateMachine(Data)  (Data)->StateMachine
#define SrcpClientGetLastMsgTstamp(Data) (Data)->LastMsgTstamp

typedef struct {
   BOOL Verbosity;
   int OutsideTcpSock;
   Menge *Clients;
   MengeIterator *ClientIter;
   MengeIterator *FdSearchIter;
   int FdPos;
   ClientInfo *FdSearchClient;
   int NumPending;
   MrCs2CanDataType PendingCanMsg[MAX_PENDING_CAN_NUM];
} SrcpStruct;

#define SrcpSetVerbose(Data, Verbose)       (Data)->Verbosity=Verbose
#define SrcpSetOutsideTcpSock(Data, Sock)   (Data)->OutsideTcpSock=Sock
#define SrcpSetClients(Data, Client)        (Data)->Clients=Client
#define SrcpSetClientIter(Data, Iter)       (Data)->ClientIter=Iter
#define SrcpSetFdSearchIter(Data, Iter)     (Data)->FdSearchIter=Iter
#define SrcpSetFdPos(Data, Pos)             (Data)->FdPos=Pos
#define SrcpSetFdSearchClient(Data, Client) (Data)->FdSearchClient=Client
#define SrcpSetParser(Data, Pars)           (Data)->Parser=Pars
#define SrcpSetNumPending(Data, Num)        (Data)->NumPending=Num

#define SrcpGetVerbose(Data)            (Data)->Verbosity
#define SrcpGetOutsideTcpSock(Data)     (Data)->OutsideTcpSock
#define SrcpGetClients(Data)            (Data)->Clients
#define SrcpGetClientIter(Data)         (Data)->ClientIter
#define SrcpGetFdSearchIter(Data)       (Data)->FdSearchIter
#define SrcpGetFdPos(Data)              (Data)->FdPos
#define SrcpGetFdSearchClient(Data)     (Data)->FdSearchClient
#define SrcpGetParser(Data)             (Data)->Parser
#define SrcpGetNumPending(Data)         (Data)->NumPending
#define SrcpGetPendingCanMsg(Data, Num) &((Data)->PendingCanMsg[Num])

IoFktStruct *SrcpInit(BOOL Verbose);
void SrcpExit(IoFktStruct *Data);
void SrcpSendAnswer(ClientInfo *Data, char *Msg);
void SrcpInitFsm(SrcpStruct *Data, ClientInfo *Client);
BOOL SrcpDoFsm(SrcpStruct *Data, ClientInfo *Client, char *Line, int Length,
               MrCs2CanDataType *CanMsg);

#endif
