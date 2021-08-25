#ifndef LOK_H
#define LOK_H

#include <boolean.h>
#include <map.h>

#define LOK_NUM_FUNCTIONS 32

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   char *LocPath;
   BOOL HaveSpecialLok;
   int SpecialLokAddr;
   char SpecialLokName[17];
   Map *SpecialActions;
} LokStruct;

#define LokSetVerbose(Data, Verbose)       (Data)->Verbosity=Verbose
#define LokSetInterface(Data, Iface)       (Data)->Interface=Iface
#define LokSetAddress(Data, Addr)          (Data)->Address=Addr
#define LokSetServerPort(Data, Port)       (Data)->ServerPort=Port
#define LokSetClientSock(Data, Sock)       (Data)->ClientSock=Sock
#define LokSetLocPath(Data, Path)          (Data)->LocPath=Path
#define LokSetHaveSpecialLok(Data, Have)   (Data)->HaveSpecialLok=Have
#define LokSetSpecialLokAddr(Data, Addr)   (Data)->SpecialLokAddr=Addr
#define LokSetSpecialLokName(Data, Name)   strcpy((Data)->SpecialLokName, Name)
#define LokSetSpecialActions(Data, Action) (Data)->SpecialActions=Action

#define LokGetVerbose(Data)        (Data)->Verbosity
#define LokGetInterface(Data)      (Data)->Interface
#define LokGetAddress(Data)        (Data)->Address
#define LokGetServerPort(Data)     (Data)->ServerPort
#define LokGetClientSock(Data)     (Data)->ClientSock
#define LokGetLocPath(Data)        (Data)->LocPath
#define LokGetHaveSpecialLok(Data) (Data)->HaveSpecialLok
#define LokGetSpecialLokAddr(Data) (Data)->SpecialLokAddr
#define LokGetSpecialLokName(Data) (Data)->SpecialLokName
#define LokGetSpecialActions(Data) (Data)->SpecialActions

LokStruct *LokCreate(void);
void LokDestroy(LokStruct *Data);
void LokInit(LokStruct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, char *LocPath);
void LokRun(LokStruct *Data);

#endif
