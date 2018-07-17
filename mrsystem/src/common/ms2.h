#ifndef MS2_H
#define MS2_H

#include <boolean.h>
#include <queue.h>
#include "can_io.h"

#define MASTER_MODE_PROXY      0
#define MASTER_MODE_MS2_MASTER 1
#define MASTER_MODE_MRS_MASTER 2

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   unsigned int ZentraleMode;
   Queue *GbCommands;
   unsigned int NumBuffers;
   Queue *CmdBuffers;
   time_t LastTime;
   MrCs2CanDataType *ActualCmd;
   IoFktStruct *IoFunctions;
   int Ms2PollSock;
} Ms2Struct;

#define Ms2SetVerbose(Data, Verbose)       (Data)->Verbosity=Verbose
#define Ms2SetInterface(Data, Iface)       (Data)->Interface=Iface
#define Ms2SetAddress(Data, Addr)          (Data)->Address=Addr
#define Ms2SetServerPort(Data, Port)       (Data)->ServerPort=Port
#define Ms2SetClientSock(Data, Sock)       (Data)->ClientSock=Sock
#define Ms2SetZentraleMode(Data, Zentrale) (Data)->ZentraleMode=Zentrale
#define Ms2SetGbCommands(Data, CmdQueue)   (Data)->GbCommands=CmdQueue
#define Ms2SetNumBuffers(Data, Num)        (Data)->NumBuffers=Num
#define Ms2SetCmdBuffers(Data, Buffers)    (Data)->CmdBuffers=Buffers
#define Ms2SetLastTime(Data, Time)         (Data)->LastTime=Time
#define Ms2SetActualCmd(Data, Cmd)         (Data)->ActualCmd=Cmd
#define Ms2SetIoFunctions(Data, Fkts)      (Data)->IoFunctions=Fkts
#define Ms2SetMs2PollSock(Data, Sock)      (Data)->Ms2PollSock=Sock

#define Ms2GetVerbose(Data)      (Data)->Verbosity
#define Ms2GetInterface(Data)    (Data)->Interface
#define Ms2GetAddress(Data)      (Data)->Address
#define Ms2GetServerPort(Data)   (Data)->ServerPort
#define Ms2GetClientSock(Data)   (Data)->ClientSock
#define Ms2GetZentraleMode(Data) (Data)->ZentraleMode
#define Ms2GetGbCommands(Data)   (Data)->GbCommands
#define Ms2GetNumBuffers(Data)   (Data)->NumBuffers
#define Ms2GetCmdBuffers(Data)   (Data)->CmdBuffers
#define Ms2GetLastTime(Data)     (Data)->LastTime
#define Ms2GetActualCmd(Data)    (Data)->ActualCmd
#define Ms2GetIoFunctions(Data)  (Data)->IoFunctions
#define Ms2GetMs2PollSock(Data)  (Data)->Ms2PollSock

Ms2Struct *Ms2Create(void);
void Ms2Destroy(Ms2Struct *Data);
void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, int ZentraleMode, IoFktStruct *IoFunctions);
void Ms2Run(Ms2Struct *Data);

#endif
