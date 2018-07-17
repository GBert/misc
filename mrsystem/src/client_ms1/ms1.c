#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/can.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <mr_can.h>
#include "ms1.h"
#include "../client_ms2/can_client.h"
#include "maerklin.h"

#define SELECT_TIMEOUT 100
#define MAXDG   4096	/* maximum datagram size */
#define MAXUDP  16	/* maximum datagram size */

static BOOL Loop = TRUE;

Ms1Struct *Ms1Create(void)
{  Ms1Struct *Data;

   Data = (Ms1Struct *)malloc(sizeof(Ms1Struct));
   if (Data != (Ms1Struct *)NULL)
   {
      Ms1SetVerbose(Data, FALSE);
      Ms1SetInterface(Data, (char *)NULL);
      Ms1SetServerPort(Data, -1);
      Ms1SetClientSock(Data, -1);
      Ms1SetIoFunctions(Data, (IoFktStruct *)NULL);
   }
   return(Data);
}

void Ms1Destroy(Ms1Struct *Data)
{
   if (Ms1GetVerbose(Data))
      puts("destroy ms1");
   free(Data);
}

void Ms1Init(Ms1Struct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, IoFktStruct *IoFunctions)
{
   Ms1SetVerbose(Data, Verbose);
   Ms1SetInterface(Data, Interface);
   Ms1SetAddress(Data, Addr);
   Ms1SetServerPort(Data, Port);
   Ms1SetClientSock(Data, -1);
   Ms1SetIoFunctions(Data, IoFunctions);
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(Ms1Struct *Data)
{  struct sigaction SigStruct;
   struct sockaddr_in baddr;
   const int on = 1;

   if ((strlen(Ms1GetInterface(Data)) > 0) &&
       ((strlen(Ms1GetAddress(Data)) == 0) ||
        (strcmp(Ms1GetAddress(Data), "0.0.0.0") == 0)))
   {
      Ms1SetClientSock(Data,
                       MrIpcConnectIf(Ms1GetInterface(Data),
                                      Ms1GetServerPort(Data)));
   }
   else
   {
      Ms1SetClientSock(Data,
                       MrIpcConnect(Ms1GetAddress(Data),
                                    Ms1GetServerPort(Data)));
   }
   if (Ms1GetClientSock(Data) >= 0)
   {
      if (Ms1GetIoFunctions(Data)->Open(Ms1GetIoFunctions(Data)->private))
      {
         if (Ms1GetVerbose(Data))
            puts("ready for incoming comands from system");
         SigStruct.sa_handler = SigHandler;
         sigemptyset(&SigStruct.sa_mask);
         SigStruct.sa_flags = 0;
         sigaction(SIGINT, &SigStruct, NULL);
         sigaction(SIGQUIT, &SigStruct, NULL);
         sigaction(SIGTERM, &SigStruct, NULL);
         return(TRUE);
      }
      else
      {
         MrIpcClose(Ms1GetClientSock(Data));
         return(FALSE);
      }
   }
   else
   {
      return(FALSE);
   }
}

static void Stop(Ms1Struct *Data)
{
   if (Ms1GetVerbose(Data))
      puts("stop network client");
   MrIpcClose(Ms1GetClientSock(Data));
   Ms1GetIoFunctions(Data)->Close(Ms1GetIoFunctions(Data)->private);
}

static void ProcessSystemData(Ms1Struct *Data, MrIpcCmdType *CmdFrame)
{  CanFrameStruct CanFrame;

/* TODO: encode can frame from CmdFrame */
   Ms1GetIoFunctions(Data)->Write(Ms1GetIoFunctions(Data)->private, &CanFrame);
}

static void HandleSystemData(Ms1Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Ms1GetVerbose(Data))
      puts("new data available");
   do {
      MrIpcInit(&CmdFrame); /* init our receive struct */
      RcvReturnValue = MrIpcRecv(Ms1GetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         if (Ms1GetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         if (Ms1GetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         if (Ms1GetVerbose(Data))
            puts("read new comand frame from socket");
         ProcessSystemData(Data, &CmdFrame);
      }
   } while (RcvReturnValue > 0);
}

static int get_wert_for_index (int index) {

    switch (index) {
        case 1:
            return(0x00); //GO
            break;

        case 2:
            return(0x00); //kein Kurzschluss
            break;

        case 3:
            return(0x00);
            break;

        case 4:
            return(0x01); //Normalbetrieb
            break;

        default:
            break;
    }

}

static void split_canid(struct ms1_handle *handle, uint32_t id) {
    bzero(handle,sizeof(struct ms1_handle));
    handle->prio=(id >> 26) & 0x03;
    if (handle->prio == 3) {
        handle->node = id & 0x7FU;
        id = id >> 7;
        handle->direction = id & 0x01U;
        id = id >> 1;
        handle->MID = id & 0x7FU;
        id = id >> 7;
        handle->stage = id & 0x07U;
        id = id >> 3;
        handle->UID = id & 0xFFU;
    } else {
        handle->node = id & 0x7FU;
        id = id >> 7;
        handle->command = id & 0x7FU;
        id = id >> 3;
        handle->object_handle= id & 0xFFFF;
    }
}

static void ProcessCanData(Ms1Struct *Data, CanFrameStruct *CanFrame)
{  MrIpcCmdType CmdFrame;
   struct can_frame out_msg;
   int canid, uid, nbytes, i, udpsend, sc;
   unsigned char udpframe[MAXDG];
   int ms_uid[4];
   int statustyp[64];
   int lastmfxnr = 4;
   struct ms1_handle my_ms1_handle;

   MrIpcInit(&CmdFrame); /* init the struct to the system */
      canid = 0;
/* TODO: set Cmd and send to drehscheibe instead of call to write */
			if (CanFrame->CanId & CAN_EFF_FLAG) { // only EFF frames are valid
			
				
			// prepare UDP frame 
			bzero(udpframe,13);
			CanFrame->CanId &= CAN_EFF_MASK;
			udpframe[0] = (CanFrame->CanId >> 24) & 0x000000FF;
			udpframe[1] = (CanFrame->CanId >> 16) & 0x000000FF;
			udpframe[2] = (CanFrame->CanId >>  8) & 0x000000FF;
			udpframe[3] = CanFrame->CanId & 0x000000FF;
			udpframe[4] = CanFrame->CanDlc;
			memcpy(&udpframe[5],&CanFrame->CanData,CanFrame->CanDlc);
				
			//CAN Frame ID zerlegen
            split_canid(&my_ms1_handle, CanFrame->CanId);


            // Slave Anmeldung Mobile Station erkannt
			if (my_ms1_handle.prio == 0x07 /*0b111*/) {
				
				udpsend = 0; //dieser Frame soll nicht per UDP weitergeleitet werden
				
				switch(my_ms1_handle.stage) {
				
					case 0x00 : //Stufe 0
						
						printf("found MS1 slave device\n");
						printf("<- 0x%X Stufe 0\n",CanFrame->CanId);
				
						ms_uid[3] = (CanFrame->CanId & 0x3FC0000) >> 18;
						//printf("MS UID[3]: 0x%X\n",ms_uid[3]);
				
						// sende Quittung Stufe 0
						out_msg.can_id = CanFrame->CanId ^ 0x80;  //MID=0
						
						printf("-> 0x%X Stufe 0 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x01; //Master UID[3]
						out_msg.data[1] = 0x01; //Master UID[2]
						out_msg.data[2] = 0x01; //Master UID[1]
						out_msg.data[3] = 0x01; //Master UID[0]
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x01;
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x00; //MS-Master
					
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
					
					case 0x04 /*0b00100*/ : //Stufe 1
						
						printf("<- 0x%X Stufe 1\n",CanFrame->CanId);
						
						ms_uid[2] = (CanFrame->CanId & 0x3FC0000) >> 18;
						//printf("MS UID[2]: 0x%X\n",ms_uid[2]);
						
						// sende Quittung Stufe 1
						out_msg.can_id = CanFrame->CanId ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 1 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x00; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x97;
						out_msg.data[4] = 0x01; //MS-Master
						out_msg.data[5] = 0x00; //MS-Master
						out_msg.data[6] = 0xc0; //MS-Master
						out_msg.data[7] = 0x10; //MS-Master
					
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
						
					case 0x08 /*0b01000*/ : //Stufe 2
						
						printf("<- 0x%X Stufe 2\n",CanFrame->CanId);
						
						ms_uid[1] = (CanFrame->CanId & 0x3FC0000) >> 18;
						//printf("MS UID[1]: 0x%X\n",ms_uid[1]);
						
						// sende Quittung Stufe 2
						out_msg.can_id = CanFrame->CanId ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 2 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x01; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x69;
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x00;
						out_msg.data[6] = 0x40;
						out_msg.data[7] = 0x10; 
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}						
						
						break;
				
						
					case 0x0c /*0b01100*/ : // Stufe 3
						
						printf("<- 0x%X Stufe 3\n",CanFrame->CanId);
						
						ms_uid[0] = (CanFrame->CanId & 0x3FC0000) >> 18;
						//printf("MS UID[0]: 0x%X\n",ms_uid[0]);
						
						// sende Quittung Stufe 3
						out_msg.can_id = CanFrame->CanId ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 3 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x00; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x40;
						out_msg.data[3] = 0x10;
						out_msg.data[4] = 0x01;
						out_msg.data[5] = 0x03;
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x84; //MS-Master
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");							
						}
												
						break;	
						
					case 0x10 /*0b10000*/ : //Stufe 4
						printf("<- 0x%X Stufe 4\n",CanFrame->CanId);
						break;
						
					case 0x14 /*0b10100*/ : //Stufe 5
						printf("<- 0x%X Stufe 5\n",CanFrame->CanId);
						break;
						
					case 0x18 /*0b11000*/ : //Stufe 6
						printf("<- 0x%X Stufe 6\n",CanFrame->CanId);
						break;
						
					case 0x1c /*0b11100*/ : //Stufe 7
						
						printf("<- 0x%X Stufe 7\n",CanFrame->CanId);
						
						// sende Quittung Stufe 7
						out_msg.can_id = CanFrame->CanId ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 7 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = ms_uid[3];
						out_msg.data[1] = ms_uid[2];
						out_msg.data[2] = ms_uid[1];
						out_msg.data[3] = ms_uid[0];
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x01;
						out_msg.data[6] = 0x01; //Knotennr. bei CS eigentlich min 3 
						out_msg.data[7] = 0x00; //ACHTUNG!!! 0x01 = Update!!! NIE BENUTZEN!!!
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
						
				} // Ende Switch

			}

			// Aufruestung MS
			if (my_ms1_handle.prio == 0x06 /*0b110*/) {
				
				udpsend = 0; //dieser Frame soll nicht per UDP weitergeleitet werden
				
				// welcher Befehl wurde gesendet
                switch (my_ms1_handle.MID) {

                    case 0x01 /*0b001*/ : 

                        switch (CanFrame->CanData[1]) {

                            case 0x03 : // Get Lok Typ

                                printf("<- 0x%X Get Lok Type\n",CanFrame->CanId);

                                out_msg.can_id = CanFrame->CanId - 1;

                                printf("-> 0x%X Send Lok Type\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 6;
                                out_msg.data[0] = CanFrame->CanData[0];
                                out_msg.data[1] = CanFrame->CanData[1];
                                out_msg.data[2] = CanFrame->CanData[2];
                                out_msg.data[3] = CanFrame->CanData[3];
                                out_msg.data[4] = 0x00;
                                out_msg.data[5] = 0x03; //Dampflok

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                            case 0x01 : // Schienenformat

                                printf("<- 0x%X Request Schienenformat\n",CanFrame->CanId);

                                out_msg.can_id = CanFrame->CanId - 1;

                                printf("-> 0x%X Send Schienenformat\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 8;
                                out_msg.data[0] = CanFrame->CanData[0];
                                out_msg.data[1] = CanFrame->CanData[1];
                                out_msg.data[2] = CanFrame->CanData[2];
                                out_msg.data[3] = CanFrame->CanData[3];
                                out_msg.data[4] = 0x00;
                                out_msg.data[6] = 0x00;

                                if (CanFrame->CanData[2] == 0x00) {
                                    out_msg.data[5] = 0x03;
                                    out_msg.data[7] = 0x03;
                                }

                                if (CanFrame->CanData[2] == 0x01) {
                                    out_msg.data[5] = 0x4E;
                                    out_msg.data[7] = 0x00;
                                }

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                        }



                        break;


					case 0x04 /*0b100*/ : // Anfrage bzgl. Slave Description

						printf("<- 0x%X Anfrage Slave Description\n",CanFrame->CanId);

						out_msg.can_id = CanFrame->CanId - 1;

						printf("-> 0x%X Anfrage Slave Description Quittung\n",out_msg.can_id);

						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x40;;
						out_msg.data[1] = CanFrame->CanData[1];
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x00;
						out_msg.data[4] = 0x00; //Obj H
						out_msg.data[5] = 0x01;	//Obj L
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x01;

						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}

						break;

                     case 0x03 /*0b011*/ :

                        // System Status-Handle oder Loksat?

                        switch (CanFrame->CanData[0]) {

                            case 0x03 : // Anfrage bzgl. System-Status-Handle

                                printf("<- 0x%X Anfrage System-Status-Handle\n",CanFrame->CanId);

                                out_msg.can_id = CanFrame->CanId - 1;

                                printf("-> 0x%X Anfrage System-Status-Handle Quittung\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 8;
                                out_msg.data[0] = 0x03;
                                out_msg.data[1] = 0x00;
                                out_msg.data[2] = 0x00;
                                out_msg.data[3] = 0x00;
                                out_msg.data[4] = 0x00; //Obj H
                                out_msg.data[5] = 0x02;	//Obj L
                                out_msg.data[6] = 0x00;
                                out_msg.data[7] = 0x01;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                            case 0x80 : // Lokstat

                                //dirty hack! Es wird testweise eine einzige Lok übertragen

                                if (CanFrame->CanData[2] == 0x00) { //erste Anfrage, noch keine Lok. Eine Lok wird gesendet

                                    printf("<- 0x%X Lokstat Austausch (1)\n",CanFrame->CanId);

                                    out_msg.can_id = CanFrame->CanId - 1;

                                    printf("-> 0x%X Lokstat Austausch (1)\n",out_msg.can_id);

                                    out_msg.can_id &= CAN_EFF_MASK;
                                    out_msg.can_id |= CAN_EFF_FLAG;
                                    out_msg.can_dlc = 8;
                                    out_msg.data[0] = CanFrame->CanData[0];
                                    out_msg.data[1] = CanFrame->CanData[1];
                                    out_msg.data[2] = 0x00;
                                    out_msg.data[3] = 0x00;
                                    out_msg.data[4] = 0x02;
                                    out_msg.data[5] = 0x02;
                                    out_msg.data[6] = 0x00;
                                    out_msg.data[7] = 0x01;
                                }

                                if (CanFrame->CanData[2] == 0x02) { //bereits eine Lok vorhanden, wir senden keine neue

                                    printf("<- 0x%X Lokstat Austausch (2)\n",CanFrame->CanId);

                                    out_msg.can_id = CanFrame->CanId - 1;

                                    printf("-> 0x%X Lokstat Austausch (2)\n",out_msg.can_id);

                                    out_msg.can_id &= CAN_EFF_MASK;
                                    out_msg.can_id |= CAN_EFF_FLAG;
                                    out_msg.can_dlc = 4;
                                    out_msg.data[0] = CanFrame->CanData[0];
                                    out_msg.data[1] = CanFrame->CanData[1];
                                    out_msg.data[2] = CanFrame->CanData[2];
                                    out_msg.data[3] = CanFrame->CanData[3];
                                }

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;
                            }

                        break;



                    case 0x02 /*0b010*/ : // Anfrage bzgl. System-Status-Austausch

                        printf("<- 0x%X Anfrage System-Status-Austausch\n",CanFrame->CanId);


                        if (CanFrame->CanData[1] == 0x01) { //Anfrage nach dem Statustyp

                            if (CanFrame->CanData[0] <= 0x04) {

                                //Verknüpfung Index und Statustyp an Slave senden
                                out_msg.can_id = CanFrame->CanId - 1;

                                printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                                out_msg.can_dlc = 3;
                                out_msg.data[0] = CanFrame->CanData[0]; //INDEX
                                out_msg.data[1] = 0x01;
                                out_msg.data[2] = CanFrame->CanData[0]; //eigentlich STATUSTYP

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                statustyp[CanFrame->CanData[0]] = out_msg.data[2];  //Verknüpfung Index und Statustyp speichern

                                if (CanFrame->CanData[0] == 0x03) { // 0x03 ubekannter STATUSTYP, es wird direkt der WERT gesendet
                                    goto Wertsenden;
                                }
                            }


                            if (CanFrame->CanData[0] == 0x05) {

                                //Verknüpfung Index und Statustyp an Slave senden
                                out_msg.can_id = CanFrame->CanId - 1;

                                printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                                out_msg.can_dlc = 2;
                                out_msg.data[0] = CanFrame->CanData[0]; //INDEX
                                out_msg.data[1] = 0x01;

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                statustyp[CanFrame->CanData[0]] = out_msg.data[2];  //Verknüpfung Index und Statustyp speichern

                            }

                        }


                        if (CanFrame->CanData[1] == 0x00) { //Anfrage nach dem Wert

                            Wertsenden:

                            printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                            //Wert für Index an Slave senden
                            out_msg.can_id = CanFrame->CanId - 1;

                            out_msg.can_dlc = 3;
                            out_msg.data[0] = CanFrame->CanData[0]; //Index
                            out_msg.data[1] = 0x00;
                            out_msg.data[2] = get_wert_for_index(CanFrame->CanData[0]);

                            out_msg.can_id &= CAN_EFF_MASK;
                            out_msg.can_id |= CAN_EFF_FLAG;

                            if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                perror("CAN write error");
                            }

                        }

                        break;


				} // Ende Switch

			}

			// Zyklische Überwachung
			if (my_ms1_handle.prio == 0x03 /*0b011*/) {

                if (CanFrame->CanDlc == 4) {

                    printf("<- 0x%X Ping 1\n",CanFrame->CanId);

                    out_msg.can_id = CanFrame->CanId - 1;
                    //out_msg.can_id = 0xC000380;  //Master Knoten ist 00

                    printf("-> 0x%X Pong 1\n",out_msg.can_id);

                    out_msg.can_id &= CAN_EFF_MASK;
                    out_msg.can_id |= CAN_EFF_FLAG;
                    out_msg.can_dlc = 8;
                    out_msg.data[0] = 0x00;
                    out_msg.data[1] = 0x00;
                    out_msg.data[2] = 0x00;
                    out_msg.data[3] = CanFrame->CanData[3];
                    out_msg.data[4] = 0x01; //master uid 3
                    out_msg.data[5] = 0x01; //master uid 2
                    out_msg.data[6] = 0x01; //master uid 1
                    out_msg.data[7] = 0x01; //master uid 0



                    if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                        perror("CAN write error");
                    }

                    out_msg.can_id = CanFrame->CanId - 1;
                    //out_msg.can_id = 0xC000380;

                    printf("-> 0x%X Pong 2\n",out_msg.can_id);

                    out_msg.can_id &= CAN_EFF_MASK;
                    out_msg.can_id |= CAN_EFF_FLAG;
                    out_msg.can_dlc = 8;
                    out_msg.data[0] = 0x00;
                    out_msg.data[1] = 0x00;
                    out_msg.data[2] = 0x00;
                    out_msg.data[3] = 0x01;  //CanFrame->CanData[3];
                    out_msg.data[4] = ms_uid[3];
                    out_msg.data[5] = ms_uid[2];
                    out_msg.data[6] = ms_uid[1];
                    out_msg.data[7] = ms_uid[0];

                    if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                        perror("CAN write error");
                    }

                }

                if (CanFrame->CanDlc == 8) {

                    printf("<- 0x%X Ping 2\n",CanFrame->CanId);

                }

			}

			// Das Programm reagiert auf die Erkennung eines mfx-Dekoders und vergibt eine SID (Befehl 3.2: MFX Bind)
			if (((CanFrame->CanId & 0xFFFF0000) == 0x00030000UL) && ( CanFrame->CanDlc== 5)) {

				udpsend = 1;

/* this should be done in client_zentrale */
				printf("CAN>UDP MFX 0x%06X\n",CanFrame->CanId);
				lastmfxnr = lastmfxnr + 1;

				out_msg.can_id = 0x040301UL;
				out_msg.can_id &= CAN_EFF_MASK;
				out_msg.can_id |= CAN_EFF_FLAG;
				out_msg.can_dlc = 6;
				out_msg.data[0] = CanFrame->CanData[0];
				out_msg.data[1] = CanFrame->CanData[1];
				out_msg.data[2] = CanFrame->CanData[2];
				out_msg.data[3] = CanFrame->CanData[3];
				out_msg.data[4] = (lastmfxnr >> 8) & 0x00FF;
				out_msg.data[5] = lastmfxnr & 0x00FF;

				if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
					perror("CAN write error MFX Nr __");
				}

				uid = CanFrame->CanData[3] | (CanFrame->CanData[2] << 8) | (CanFrame->CanData[1] << 16) | (CanFrame->CanData[0] << 24);

				udpframe[0] = (out_msg.can_id >> 24) & 0x000000FF;
				udpframe[1] = (out_msg.can_id >> 16) & 0x000000FF;
				udpframe[2] = (out_msg.can_id >> 8) & 0x000000FF;
				udpframe[3] =  out_msg.can_id & 0x000000FF;
				udpframe[4] =  out_msg.can_dlc;

				for (i = 0; i < 8; ++i)
				{
					if (i < out_msg.can_dlc)
						udpframe[5+i] = out_msg.data[i];
					else
						udpframe[5+i] = 0;
				}

				printf("\nNeuer mfx-Dekoder (UID 0x%X) angemeldet, Nummer: %d\n> ", uid, lastmfxnr);

				//muss jetzt noch in Lokliste eingetragen werden!!!
				FILE *datei;
				datei = fopen ("lokliste.txt", "a");

				if (datei != NULL) {
					fprintf (datei, "%d;%X\n>", lastmfxnr, uid);
					fclose (datei);					
				}

			}

			// send UDP frame
			if (udpsend == 1) { // nicht jeder CAN Frame muss über UDP raus!
/* should be done in client_cs2eth */
#if 0
				s=sendto(sb, udpframe, 13, 0, (struct sockaddr *)&baddr, sizeof(baddr));
				if (s != 13)
					perror("UDP write __");
#endif
				if (Ms1GetVerbose(Data) ) {
					printf("->CAN>UDP CANID 0x%06X R",CanFrame->CanId);
					printf(" [%d]", udpframe[4]);
					for (i=5; i<5+CanFrame->CanDlc;i++) {
						printf(" %02x", udpframe[i]);
					}
					printf("\n");
				}
			}
		}
      MrIpcSend(Ms1GetClientSock(Data), &CmdFrame);
}

static void HandleCanData(Ms1Struct *Data)
{  CanFrameStruct CanFrame;

   if (Ms1GetIoFunctions(Data)->Read(Ms1GetIoFunctions(Data)->private,
                                           &CanFrame) == sizeof(CanFrameStruct))
   {
      ProcessCanData(Data, &CanFrame);
   }
}

void Ms1Run(Ms1Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (Ms1GetVerbose(Data))
            printf("add client socket %d\n", Ms1GetClientSock(Data));
         FD_SET(Ms1GetClientSock(Data), &ReadFds);
         if (Ms1GetClientSock(Data) > HighFd)
            HighFd = Ms1GetClientSock(Data);
         if (Ms1GetVerbose(Data))
            printf("add can socket %d\n",
                   Ms1GetIoFunctions(Data)->GetFd(Ms1GetIoFunctions(Data)->private));
         FD_SET(Ms1GetIoFunctions(Data)->GetFd(Ms1GetIoFunctions(Data)->private),
                &ReadFds);
         if (Ms1GetIoFunctions(Data)->GetFd(Ms1GetIoFunctions(Data)->private) > HighFd)
            HighFd = Ms1GetIoFunctions(Data)->GetFd(Ms1GetIoFunctions(Data)->private);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Ms1GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Ms1GetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (Ms1GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (Ms1GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(Ms1GetClientSock(Data), &ReadFds))
            {
               HandleSystemData(Data);
            }
            if (FD_ISSET(Ms1GetIoFunctions(Data)->GetFd(Ms1GetIoFunctions(Data)->private),
                         &ReadFds))
            {
               HandleCanData(Data);
            }
         }
      }
      Stop(Data);
   }
}
