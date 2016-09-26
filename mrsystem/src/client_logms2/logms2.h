#ifndef LOGMS2_H
#define LOGMS2_H

#include <arpa/inet.h>
#include <boolean.h>

typedef struct {
   BOOL Verbosity;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   char CanName[30];
   int CanSock;
} Logms2Struct;

#define Logms2SetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define Logms2SetInterface(Data, Iface) (Data)->Interface=Iface
#define Logms2SetAddress(Data, Addr)    (Data)->Address=Addr
#define Logms2SetServerPort(Data, Port) (Data)->ServerPort=Port
#define Logms2SetClientSock(Data, Sock) (Data)->ClientSock=Sock
#define Logms2SetCanName(Data, Name)    strcpy((Data)->CanName, Name)
#define Logms2SetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define Logms2GetVerbose(Data)    (Data)->Verbosity
#define Logms2GetInterface(Data)  (Data)->Interface
#define Logms2GetAddress(Data)    (Data)->Address
#define Logms2GetServerPort(Data) (Data)->ServerPort
#define Logms2GetClientSock(Data) (Data)->ClientSock
#define Logms2GetCanName(Data)    (Data)->CanName
#define Logms2GetCanSock(Data)    (Data)->CanSock

Logms2Struct *Logms2Create(void);
void Logms2Destroy(Logms2Struct *Data);
void Logms2Init(Logms2Struct *Data, BOOL Verbose, char *Interface, char *Addr,
                int Port, char *CanIf);
void Logms2Run(Logms2Struct *Data);

#endif
