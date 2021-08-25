#ifndef MR_IPCHL_H
#define MR_IPCHL_H

/**
* @mainpage mr_ipchl
*
* Routinen um f&uuml;r bestimmte IPC Kommandos alle Parameter am St&uuml;ck
* zu setzen.
*
* @author Michael Bernstein
*/

#include <mr_ipc.h>

/** @file */

void MrIpcHlCanBootldr(MrIpcCmdType *Cmd, unsigned long Uid,
                       unsigned long Length, unsigned char *Data);
void MrIpcHlConfigHeader(MrIpcCmdType *Cmd, unsigned long Uid,
                         unsigned int Index, unsigned int Length);
void MrIpcHlConfigResponse(MrIpcCmdType *Cmd, unsigned long Uid, char *CfgFile);
void MrIpcHlConfigvalue(MrIpcCmdType *Cmd, unsigned long Uid,
                        unsigned long DstUid, unsigned int Index,
                        unsigned int Length);
void MrIpcHlLocinfoRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int SenderSock, int ReceiverSocket,
                           char *LokName);
void MrIpcHlLocnameRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int SenderSock, int ReceiverSock,
                           unsigned int Index);
void MrIpcHlLoclistRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int SenderSock, int ReceiverSock);
void MrIpcHlMemberRequest(MrIpcCmdType *Cmd, unsigned long Uid);
void MrIpcHlMemberResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                           unsigned int DeviceId);
void MrIpcHlMesswertRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                            unsigned long DstUid, unsigned int Kanal);
void MrIpcHlStatusResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                           int ReceiverSock);
void MrIpcHlStatusNameResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                           int ReceiverSock, unsigned char *StatusName);
void MrIpcHlStatusPosResponse(MrIpcCmdType *Cmd, unsigned long Uid,
                              int ReceiverSock, unsigned long Addr,
                              unsigned int Index, unsigned int NumPackets);
void MrIpcHlStatusRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                          unsigned long DstUid, unsigned int Index);
void MrIpcHlCfgHeaderRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                             int ReceiverSock, unsigned long Length,
                             unsigned Crc);
void MrIpcHlCfgDataRequest(MrIpcCmdType *Cmd, unsigned long Uid,
                           int ReceiverSock, char *Name);

#endif
