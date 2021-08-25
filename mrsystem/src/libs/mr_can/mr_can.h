#ifndef MR_CAN_H
#define MR_CAN_H

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <boolean.h>
#include <bitmask.h>
#include "m_cs2ms2.h"

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

/* Definitionen fuer CS2 Ethernet */
#define MR_CS2_UDP_LOCAL_PORT  15731
#define MR_CS2_UDP_REMOTE_PORT 15730
#define MR_CS2_UDP_LENGTH 13

/* return values for function MrEthCs2Recv() */
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

#endif
