#ifndef MR_IPC_H
#define MR_IPC_H

/**
* @mainpage mr_ipc
*
* @ brief Kommunikation von/zur drehscheibe
*
* Die Bibliothek mr_ipc fa&szlig;t Datentypen und Funktionen zusammen, die
* f&uuml;r die Kommunikation der Clients mit der drehscheibe n&ouml;tig sind.
* Dies umfa&szlig;t zum einen den Datentyp, der zwischen den Programmen
* &uuml;bertragen wird, als auch Funktionen, um eine Verbbindung zu dem Server
* zu &ouml;ffnen und Daten zu &uuml;bertragen. Der Name leitet sich von Inter
* Process Communication ab. Das Protokoll orientiert sich an den CAN
* Nachrichten von M&auml;rklin, da das Verteilen der M&auml;rklin CAN
* Nachrichten der Haupteinsatzzweck ist. Ein universelles und
* herstellerunabh&auml;ngies Protkoll l&auml;&szlig;t sich wegen
* unterschiedlicher Konzepte und M&ouml;glichkeiten nur schwer definieren.
* Einige CAN Nachrichten haben keine Entsprechnung im IPC Protokoll. Diese
* Nachrichten werden "roh" als CAN Frame &uuml;bertragen.
*
* @author Michael Bernstein
*/

#include <unistd.h>
#include <sys/socket.h>
#include <cs2.h>
#include <mr_cs2ms2.h>

/** @file */

/*
* @brief Konstanten f&uuml;r den Returncode der Funktion MrIpcSend()
*/
#define MR_IPC_RCV_ERROR  -1
#define MR_IPC_RCV_CLOSED 0
#define MR_IPC_RCV_OK     1

/** Konstanten f&uuml;r Broadcast als Ziel-/Sendersocket */
#define MR_IPC_SOCKET_ALL -1

typedef enum { Off, On } SwitchType;
typedef enum { Forward, Backward } DirectionType;
typedef enum { Left, Right } PositionType;

/**
* @brief Datentyp f&uuml;r die Kommandos von/zur drehscheibe
*/
typedef enum {
   MrIpcCmdNull,                /*  0 empty command */
   MrIpcCmdRun,                 /*  1 system start/stop command */
   MrIpcCmdTrackProto,          /*  2 system Gleis Protokoll */
   MrIpcCmdLocomotiveSpeed,     /*  3 command to set speed of a locomotive */
   MrIpcCmdLocomotiveDirection, /*  4 command to set direction of a locomotive */
   MrIpcCmdLocomotiveFunction,  /*  5 command to set function of a locomotive */
   MrIpcCmdAccSwitch,           /*  6 command to switch accesoires */
   MrIpcCmdRequestMember,       /*  7 command to request CAN member */
   MrIpcCmdMember,              /*  8 command to announce member */
   MrIpcCmdRequestLocName,      /*  9 command to request loc name from MS2 */
   MrIpcCmdRequestLocInfo,      /* 10 command to request loc info from MS2 */
   MrIpcCmdRequestFile,         /* 11 command to request a .cs2 file from CS2 */
   MrIpcCmdCfgHeader,           /* 12 command with header of cfg data */
   MrIpcCmdCfgZHeader,          /* 13 command with header of z-packed cfg data */
   MrIpcCmdCfgData,             /* 14 command with data of cfg data */
   MrIpcCmdCanBootldrGeb,       /* 15 command can bootloader gebunden */
   MrIpcCmdStatusRequest,       /* 16 command request status */
   MrIpcCmdStatusSize,          /* 17 command with number of packets in status */
   MrIpcCmdStatusData,          /* 18 command with status data */
   MrIpcCmdStatusResponse,      /* 19 command response status */
   MrIpcCmdRequestMesswert,     /* 20 command request system status */
   MrIpcCmdSetConfig,           /* 21 command set config system status */
   MrIpcCmdConfigResponse,      /* 22 command response set config system status */
   MrIpcCmdMesswertResponse,    /* 23 command response request system status */
   MrIpcCmdRequestLokListe,     /* 24 command to request lokliste from MS2 */
   MrIpcCmdIntern,              /* 25 internal command */
} MrIpcCommandValue;

/**
* @brief datentyp f&uuml;r Kommandoparameter als Werte
*/
typedef struct {
   unsigned long Lp1;
   unsigned int Ip1;
   unsigned int Ip2;
} IntDatas;

/**
* @brief Datentyp f&uuml;r Kommandoparameter als CAN Daten
*/
typedef struct {
   unsigned char Dlc;
   unsigned char Data[2 * MR_CS2_NUM_CAN_BYTES + 1];
} RawDatas;

/**
* @brief Datentype f&uuml;r ein Kommando von/zur drehscheibe
*/
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

/**
* @brief Makros, um Felder im Kommando zu setzen
*/
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

/**
* @brief Makros, um Felder aus dem Kommando zu lesen
*/
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

#define MrIpcInternalPollMs2 0x0001

/**
* @brief Makros um Funktionen auf andere zu mappen
*/
#define MrIpcClose(socket) close(socket)
#define MrIpcCalcHash(Data,Uid) MrIpcSetCanHash(Data,Cs2CalcHash(Uid))

MrIpcCmdType *MrIpcCreate(void);
void MrIpcDestroy(MrIpcCmdType *Data);
void MrIpcInit(MrIpcCmdType *Data);
void MrIpcExit(MrIpcCmdType *Data);
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
void MrIpcCmdSetTrackProto(MrIpcCmdType *Data, unsigned Protokoll);
void MrIpcCmdSetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long Addr,
                                unsigned Speed);
void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction);
void MrIpcCmdSetLocomotiveFkt(MrIpcCmdType *Data, unsigned long Addr,
                              unsigned Function, SwitchType Switch);
void MrIpcCmdSetAccPos(MrIpcCmdType *Data, unsigned long Addr,
                       PositionType Position, unsigned int Current);
void MrIpcCmdSetRequest(MrIpcCmdType *Data);
void MrIpcCmdSetRequestMember(MrIpcCmdType *Data);
void MrIpcCmdSetMember(MrIpcCmdType *Data, unsigned long Addr,
                       unsigned Version, unsigned Type);
void MrIpcCmdSetReqestLocname(MrIpcCmdType *Data, unsigned StartIdx,
                              unsigned EndIdx);
void MrIpcCmdSetReqestLocinfo(MrIpcCmdType *Data, char *Locname);
void MrIpcCmdSetReqestLoclist(MrIpcCmdType *Data);
void MrIpcCmdSetQuery(MrIpcCmdType *Data, unsigned char Dlc, char *Name);
void MrIpcCmdSetCfgHeader(MrIpcCmdType *Data, unsigned long Length,
                          unsigned Crc);
void MrIpcCmdSetCfgZHeader(MrIpcCmdType *Data, unsigned long Length,
                           unsigned Crc);
void MrIpcCmdSetCfgData(MrIpcCmdType *Data, char *Buf);
void MrIpcCmdSetCanBootldr(MrIpcCmdType *Data, unsigned Dlc,
                           unsigned char *CanData);
void MrIpcCmdSetStatusRequest(MrIpcCmdType *Data, unsigned long Addr,
                              unsigned int Index);
void MrIpcCmdSetStatusPos(MrIpcCmdType *Data, unsigned long Addr,
                          unsigned int Index, unsigned int NumPackets);
void MrIpcCmdSetStatusData(MrIpcCmdType *Data, unsigned char *Buf);
void MrIpcCmdSetStatusResponse(MrIpcCmdType *Data, unsigned long Addr,
                               unsigned int Index);
void MrIpcCmdSetMesswertRequest(MrIpcCmdType *Data, unsigned long Addr,
                                unsigned int Kanalnummer);
void MrIpcCmdSetSetConfig(MrIpcCmdType *Data, unsigned long Addr,
                          unsigned int Kanalnummer, unsigned int Value);
void MrIpcCmdSetConfigResponse(MrIpcCmdType *Data, unsigned long Addr,
                               unsigned int Kanalnummer, unsigned int Response);

void MrIpcCmdGetNull(MrIpcCmdType *Data, unsigned char *Dlc, char *CanData);
void MrIpcCmdGetRun(MrIpcCmdType *Data, SwitchType *Switch);
void MrIpcCmdGetTrackProto(MrIpcCmdType *Data, unsigned *Protokoll);
void MrIpcCmdGetLocomotiveDir(MrIpcCmdType *Data, unsigned long *Addr,
                              DirectionType *Direction);
void MrIpcCmdGetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long *Addr,
                                unsigned *Speed);
void MrIpcCmdGetLocomotiveFkt(MrIpcCmdType *Data, unsigned long *Addr,
                              unsigned *Function, SwitchType *Switch);
void MrIpcCmdGetAccPos(MrIpcCmdType *Data, unsigned long *Addr,
                       PositionType *Position, unsigned int *Current);
#define MrIpcCmdGetRequestMember(Data)
void MrIpcCmdGetMember(MrIpcCmdType *Data, unsigned long *Addr,
                       unsigned *Version, unsigned *Type);
void MrIpcCmdGetReqestLocname(MrIpcCmdType *Data, unsigned *StartIdx,
                              unsigned *EndIdx);
void MrIpcCmdGetReqestLocinfo(MrIpcCmdType *Data, char *Locinfo);
#define MrIpcCmdGetReqestLoclist(Data)
void MrIpcCmdGetQuery(MrIpcCmdType *Data, char *Name);
void MrIpcCmdGetCfgHeader(MrIpcCmdType *Data, unsigned long *Length,
                          unsigned *Crc);
void MrIpcCmdGetCfgZHeader(MrIpcCmdType *Data, unsigned long *Length,
                           unsigned *Crc);
void MrIpcCmdGetCfgData(MrIpcCmdType *Data, char *Buf);
void MrIpcCmdGetCanBootldr(MrIpcCmdType *Data, unsigned *Dlc,
                           char *CanData);
void MrIpcCmdGetStatusRequest(MrIpcCmdType *Data, unsigned long *Addr,
                              unsigned int *Index);
void MrIpcCmdGetStatusPos(MrIpcCmdType *Data, unsigned long *Addr,
                          unsigned int *Index, unsigned int *NumPackets);
void MrIpcCmdGetStatusData(MrIpcCmdType *Data, unsigned char *Buf);
#define MrIpcCmdGetStatusResponse(Data, Addr, Index) MrIpcCmdGetStatusRequest(Data, Addr, Index)
void MrIpcCmdGetMesswertRequest(MrIpcCmdType *Data, unsigned long *Addr,
                                unsigned int *Kanalnummer);
void MrIpcCmdGetSetConfig(MrIpcCmdType *Data, unsigned long *Addr,
                          unsigned int *Kanalnummer, unsigned int *Value);
void MrIpcCmdGetConfigResponse(MrIpcCmdType *Data, unsigned long *Addr,
                               unsigned int *Kanalnummer, unsigned int *Response);
#define MrIpcCmdGetMesswertResponse(Data, Addr, Kanalnummer, Value) MrIpcCmdGetConfigResponse(Data, Addr, Kanalnummer, Value)
void MrIpcCmdSetMesswertResponse(MrIpcCmdType *Data, unsigned long Addr,
                                 unsigned int Kanalnummer, unsigned int Response);

#endif
