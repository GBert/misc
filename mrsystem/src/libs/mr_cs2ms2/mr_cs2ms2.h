#ifndef MR_CS2MS2_H
#define MR_CS2MS2_H

/**
* @mainpage mr_cs2ms2
*
* Definitionen f&uuml;r internes Handling der dekodierten *.cs2 Frames.
*
* @author Michael Bernstein
*/

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <boolean.h>
#include <bitmask.h>

/** @file */

/** Konstanten f&uuml;r Anzahl CAN Datenbytes */
#define MR_CS2_NUM_CAN_BYTES 8

typedef struct {
   unsigned long Id;
   unsigned char Dlc; /* = number of data bytes */
   unsigned char Data[MR_CS2_NUM_CAN_BYTES];
   BOOL IsCs2;
   unsigned Hash;     /* in network order */
   unsigned Response;
   unsigned Command;
   unsigned Prio;
} MrCs2CanDataType;

/** Konstanten f&uuml;er Zugriff auf MrCs2CanDataType Struktur */
#define MrCs2IsCs2(CanData)                  (CanData)->IsCs2
#define MrCs2GetCommand(CanData)             (CanData)->Command
#define MrCs2GetSystemSubcmd(CanData)        (CanData)->Data[4]
#define MrCs2SetCommand(CanData,Cmd)         (CanData)->Command=Cmd
#define MrCs2SetSystemSubcmd(CanData,SubCmd) (CanData)->Data[4]=SubCmd
#define MrCs2GetId(CanData)                  (CanData)->Id
#define MrCs2SetId(CanData,id)               (CanData)->Id=id
#define MrCs2GetDlc(CanData)                 (CanData)->Dlc
#define MrCs2SetDlc(CanData,dlc)             (CanData)->Dlc=dlc
#define MrCs2GetIsCs2(CanData)               (CanData)->IsCs2
#define MrCs2SetIsCs2(CanData,is_cs2)        (CanData)->IsCs2=is_cs2
#define MrCs2GetHash(CanData)                (CanData)->Hash
#define MrCs2SetHash(CanData,hash)           (CanData)->Hash=hash
#define MrCs2GetResponse(CanData)            (CanData)->Response
#define MrCs2SetResponse(CanData,response)   (CanData)->Response=response
#define MrCs2GetPrio(CanData)                (CanData)->Prio
#define MrCs2SetPrio(CanData,prio)           (CanData)->Prio=prio
#define MrCs2GetData(CanData)                (CanData)->Data

/* Anmeldung */
#define MS1_CAN_ID_NODE_SHIFT          0
#define MS1_CAN_ID_DIRECTION_SHIFT     7
#define MS1_CAN_ID_MID_SHIFT           8
#define MS1_CAN_ID_STUFE_SHIFT        15
#define MS1_CAN_ID_UID_SHIFT          18
#define MS1_CAN_ID_PRIO_SHIFT         26

#define MS1_CAN_ID_NODE_MASK          BIT_MASK( 0, 6)
#define MS1_CAN_ID_DIRECTION_MASK     BIT_MASK( 7, 7)
#define MS1_CAN_ID_MID_MASK           BIT_MASK( 8,14)
#define MS1_CAN_ID_STUFE_MASK         BIT_MASK(15,17)
#define MS1_CAN_ID_UID_MASK           BIT_MASK(18,25)
#define MS1_CAN_ID_PRIO_MASK          BIT_MASK(26,28)

/* Aufruest/Betriebsphase */
#define MS1_CAN_ID_CMD_SHIFT           7
#define MS1_CAN_ID_OBJECT_SHIFT        10
#define MS1_CAN_ID_HANDLE1_SHIFT       10
#define MS1_CAN_ID_HANDLE2_SHIFT       18

#define MS1_CAN_ID_CMD_MASK           BIT_MASK( 7, 9) 
#define MS1_CAN_ID_OBJECT_MASK        BIT_MASK(10,25)
#define MS1_CAN_ID_HANDLE1_MASK       BIT_MASK(10,25)
#define MS1_CAN_ID_HANDLE2_MASK       BIT_MASK(10,25)

/** Konstanten f&uuml;r CS2 Ethernet */
#define MR_CS2_UDP_LOCAL_PORT  15731
#define MR_CS2_UDP_REMOTE_PORT 15730
#define MR_CS2_UDP_LENGTH 13

/** Returnwerte f&uuml;r MrEthCs2Recv() */
#define MR_ETH_CS2_RCV_ERROR  -1
#define MR_ETH_CS2_RCV_CLOSED 0
#define MR_ETH_CS2_RCV_OK     1

#define MrEthCs2Close(socket)   close(socket)

void MrEthCs2Encode(char *UdpFrame, MrCs2CanDataType *CanMsg);
void MrEthCs2Decode(MrCs2CanDataType *CanMsg, char *UdpFrame);
int MrEthCs2StartServer(void);
int MrEthCs2StartBcServer(void);
int MrEthCs2StartAppServer(void);
int MrEthCs2Accept(int ServSock);
void MrEthCs2MkBcAddr(struct sockaddr_in *baddr, char *IpAddr);
int MrEthCs2Recv(int Socket, struct sockaddr_in *ClntAddr, char *Data);
void MrEthCs2SendTo(int Socket, struct sockaddr_in *baddr, char *Data);
void MrEthCs2Send(int Socket, char *Data);
int MrEthCs2ConnectClient(void);
int MrEthCs2ConnectTcpClient(struct sockaddr_in *UdpSrvAddr);

void MrCs2Decode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame);
void MrCs2Encode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame);

void MrCs2DumpCanMsg(MrCs2CanDataType *CanMsg, char *Info);
void MrCs2Trace(MrCs2CanDataType *CanMsg);

#endif
