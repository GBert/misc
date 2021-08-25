#ifndef LOG_H
#define LOG_H

#include <boolean.h>

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
} LogStruct;

#define LogSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define LogSetInterface(Data, Iface) (Data)->Interface=Iface
#define LogSetAddress(Data, Addr)    (Data)->Address=Addr
#define LogSetServerPort(Data, Port) (Data)->ServerPort=Port
#define LogSetClientSock(Data, Sock) (Data)->ClientSock=Sock

#define LogGetVerbose(Data)    (Data)->Verbosity
#define LogGetInterface(Data)  (Data)->Interface
#define LogGetAddress(Data)    (Data)->Address
#define LogGetServerPort(Data) (Data)->ServerPort
#define LogGetClientSock(Data) (Data)->ClientSock

LogStruct *LogCreate(void);
void LogDestroy(LogStruct *Data);
void LogInit(LogStruct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port);
void LogRun(LogStruct *Data);

#endif
