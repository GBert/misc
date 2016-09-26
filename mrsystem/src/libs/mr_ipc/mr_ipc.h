#ifndef MR_IPC_H
#define MR_IPC_H

#include <unistd.h>
#include <sys/socket.h>
#include <mr_can.h>

/* return values for function MrIpcSend() */
#define MR_IPC_RCV_ERROR  -1
#define MR_IPC_RCV_CLOSED 0
#define MR_IPC_RCV_OK     1

#define MR_IPC_SOCKET_ALL -1

typedef enum { Off, On } SwitchType;
typedef enum { Forward, Backward } DirectionType;
typedef enum { Left, Right } PositionType;

/* return values for function MrIpcCmdGetCmd() and Command field */
typedef enum {
   MrIpcCmdNull,                /* empty command */
   MrIpcCmdRun,                 /* system start/stop command */
   MrIpcCmdTrackProto,          /* system Gleis Protokoll */
   MrIpcCmdLocomotiveSpeed,     /* command to set speed of a locomotive */
   MrIpcCmdLocomotiveDirection, /* command to set direction of a locomotive */
   MrIpcCmdLocomotiveFunction,  /* command to set function of a locomotive */
   MrIpcCmdAccSwitch,           /* command to switch accesoires */
   MrIpcCmdRequestMember,       /* command to request CAN member */
   MrIpcCmdMember,              /* command to announce member */
   MrIpcCmdRequestLocName,      /* command to request loc name from MS2 */
   MrIpcCmdRequestLocInfo,      /* command to request loc info from MS2 */
   MrIpcCmdRequestFile,         /* command to request a .cs2 file from CS2 */
   MrIpcCmdCfgHeader,           /* command with header of cfg data */
   MrIpcCmdCfgZHeader,          /* command with header of z-packed cfg data */
   MrIpcCmdCfgData,             /* command with data of cfg data */
   MrIpcCmdSystemStatusVal,     /* command to set systam status cfg val */
   MrIpcCmdCanBootldrGeb        /* command can bootloader gebunden */
} MrIpcCommandValue;

typedef struct {
   unsigned long Lp1;
   unsigned int Ip1;
   unsigned int Ip2;
} IntDatas;

typedef struct {
   unsigned char Dlc;
   unsigned char Data[2 * MR_CS2_NUM_CAN_BYTES + 1];
} RawDatas;

typedef struct {
   MrIpcCommandValue Command;
   int SenderSocket;
   int ReceiverSocket;
   unsigned int CanResponse;
   unsigned int CanHash;
   unsigned int CanCommand;
   unsigned int CanPrio;
   union {
      IntDatas Ints;
      RawDatas Raws;
   } Parms;
} MrIpcCmdType;

#define MrIpcSetCommand(Dat,c)        (Dat)->Command=htons(c)
#define MrIpcSetSenderSocket(Dat,v)   (Dat)->SenderSocket=v
#define MrIpcSetReceiverSocket(Dat,v) (Dat)->ReceiverSocket=v
#define MrIpcSetCanResponse(Dat,v)    (Dat)->CanResponse=htons(v)
#define MrIpcSetCanHash(Dat,v)        (Dat)->CanHash=htons(v)
#define MrIpcSetCanCommand(Dat,v)     (Dat)->CanCommand=htons(v)
#define MrIpcSetCanPrio(Dat,v)        (Dat)->CanPrio=htons(v)
#define MrIpcSetIntLp1(Dat,v)         (Dat)->Parms.Ints.Lp1=(unsigned long)htonl(v)
#define MrIpcSetIntIp1(Dat,v)         (Dat)->Parms.Ints.Ip1=htons(v)
#define MrIpcSetIntIp2(Dat,v)         (Dat)->Parms.Ints.Ip2=htons(v)
#define MrIpcSetRawDlc(Dat,v)         (Dat)->Parms.Raws.Dlc=v
#define MrIpcSetRawDataI(Dat,i,v)     (Dat)->Parms.Raws.Data[i]=v

#define MrIpcGetCommand(Dat)        ntohs((Dat)->Command)
#define MrIpcGetSenderSocket(Dat)   (Dat)->SenderSocket
#define MrIpcGetReceiverSocket(Dat) (Dat)->ReceiverSocket
#define MrIpcGetCanResponse(Dat)    ntohs((Dat)->CanResponse)
#define MrIpcGetCanHash(Dat)        ntohs((Dat)->CanHash)
#define MrIpcGetCanCommand(Dat)     ntohs((Dat)->CanCommand)
#define MrIpcGetCanPrio(Dat)        ntohs((Dat)->CanPrio)
#define MrIpcGetIntLp1(Dat)         (unsigned long)ntohl((Dat)->Parms.Ints.Lp1)
#define MrIpcGetIntIp1(Dat)         ntohs((Dat)->Parms.Ints.Ip1)
#define MrIpcGetIntIp2(Dat)         ntohs((Dat)->Parms.Ints.Ip2)
#define MrIpcGetRawDlc(Dat)         (Dat)->Parms.Raws.Dlc
#define MrIpcGetRawData(Dat)        (Dat)->Parms.Raws.Data
#define MrIpcGetRawDataI(Dat,i)     (Dat)->Parms.Raws.Data[i]

#define MrIpcClose(socket) close(socket)
#define MrIpcCalcHash(Data,Uid) MrIpcSetCanHash(Data,MrCs2CalcHash(Uid))

MrIpcCmdType *MrIpcCreate(void);
void MrIpcDestroy(MrIpcCmdType *Data);
void MrIpcInit(MrIpcCmdType *Data);
void MrIpcExit(MrIpcCmdType *Data);
void MrIpcClear(MrIpcCmdType *Data);
int MrIpcConnect(char *Address, int Port);
int MrIpcConnectIf(char *Interface, int Port);
int MrIpcStartServer(char *Adress, int Port);
int MrIpcStartServerIf(char *Interface, int Port);
int MrIpcAccept(int ServerSock);
int MrIpcSend(int socket, MrIpcCmdType *Data);
int MrIpcRecv(int socket, MrIpcCmdType *Data);

void MrIpcEncodeFromCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg);
void MrIpcDecodeToCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg);

void MrIpcCmdSetNull(MrIpcCmdType *Data, unsigned char Dlc,
                     unsigned char *CanData);
void MrIpcCmdSetRun(MrIpcCmdType *Data, SwitchType Switch);
void MrIpcCmdSetTrackProto(MrIpcCmdType *Data, int Protokoll);
void MrIpcCmdSetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long Addr,
                                unsigned Speed);
void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction);
void MrIpcCmdSetLocomotiveFkt(MrIpcCmdType *Data, unsigned long Addr,
                              unsigned Function, SwitchType Switch);
void MrIpcCmdSetAccPos(MrIpcCmdType *Data, unsigned long Addr,
                       PositionType Position);
void MrIpcCmdSetRequest(MrIpcCmdType *Data);
void MrIpcCmdSetRequestMember(MrIpcCmdType *Data);
void MrIpcCmdSetMember(MrIpcCmdType *Data, unsigned long Addr,
                       unsigned Version, unsigned Type);
void MrIpcCmdSetReqestLocname(MrIpcCmdType *Data, unsigned StartIdx,
                              unsigned EndIdx);
void MrIpcCmdSetReqestLocinfo(MrIpcCmdType *Data, char *Locname);
void MrIpcCmdSetQuery(MrIpcCmdType *Data, char *Name);
void MrIpcCmdSetCfgHeader(MrIpcCmdType *Data, unsigned long Length, int Crc);
void MrIpcCmdSetCfgZHeader(MrIpcCmdType *Data, unsigned long Length,
                           unsigned Crc);
void MrIpcCmdSetCfgData(MrIpcCmdType *Data, char *Buf);
void MrIpcCmdSetSystemStatusVal(MrIpcCmdType *Data, unsigned long Addr,
                                unsigned int Channel, unsigned int Value);
void MrIpcCmdSetCanBootldr(MrIpcCmdType *Data, unsigned char Dlc,
                           unsigned char *CanData);

void MrIpcCmdGetNull(MrIpcCmdType *Data, unsigned char *Dlc, char *CanData);
void MrIpcCmdGetRun(MrIpcCmdType *Data, SwitchType *Switch);
void MrIpcCmdGetTrackProto(MrIpcCmdType *Data, int *Protokoll);
void MrIpcCmdGetLocomotiveDir(MrIpcCmdType *Data, unsigned long *Addr,
                              DirectionType *Direction);
void MrIpcCmdGetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long *Addr,
                                unsigned *Speed);
void MrIpcCmdGetLocomotiveFkt(MrIpcCmdType *Data, unsigned long *Addr,
                              unsigned *Function, SwitchType *Switch);
void MrIpcCmdGetAccPos(MrIpcCmdType *Data, unsigned long *Addr,
                       PositionType *Position);
#define MrIpcCmdGetRequestMember(Data)
void MrIpcCmdGetMember(MrIpcCmdType *Data, unsigned long *Addr,
                       unsigned *Version, unsigned *Type);
void MrIpcCmdGetReqestLocname(MrIpcCmdType *Data, unsigned *StartIdx,
                              unsigned *EndIdx);
void MrIpcCmdGetReqestLocinfo(MrIpcCmdType *Data, char *Locinfo);
void MrIpcCmdGetQuery(MrIpcCmdType *Data, char *Name);
void MrIpcCmdGetCfgHeader(MrIpcCmdType *Data, unsigned long *Length, int *Crc);
void MrIpcCmdGetCfgZHeader(MrIpcCmdType *Data, unsigned long *Length,
                           unsigned *Crc);
void MrIpcCmdGetCfgData(MrIpcCmdType *Data, char *Buf);
void MrIpcCmdGetSystemStatusVal(MrIpcCmdType *Data, unsigned long *Addr, unsigned int *Channel, unsigned int *Value);
void MrIpcCmdGetCanBootldr(MrIpcCmdType *Data, unsigned char *Dlc,
                           char *CanData);

#endif
