#include <stddef.h>
#include <string.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief CAN aus IPC erzeugen
*
* Diese Funktion decodiert das IPC Paket in eine CanMsg.
*
* @param[in] Data Zeiger auf die IPC Struktur
* @param[in] CanMsg CAN Message f&uuml; decodiertes IPC Paket
*/
void MrIpcDecodeToCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg)
{  unsigned long Addr, Length;
   unsigned int p1, p2, CanHash;
   SwitchType Switch;
   DirectionType Direction;
   PositionType Position;
   char Buf[9];

   if (Data != (MrIpcCmdType *)NULL)
   {
      memset(MrCs2GetData(CanMsg), 0, MR_CS2_NUM_CAN_BYTES);
      MrCs2SetIsCs2(CanMsg, TRUE);
      MrCs2SetResponse(CanMsg, MrIpcGetCanResponse(Data));
      MrCs2SetHash(CanMsg, MrIpcGetCanHash(Data));
      MrCs2SetCommand(CanMsg, MrIpcGetCanCommand(Data));
      MrCs2SetPrio(CanMsg, MrIpcGetCanPrio(Data));
      CanHash = (MrCs2GetHash(CanMsg) & ~CS2_MASK_HASH_MAGIC) | CS2_HASH_MAGIC;
      MrCs2SetId(CanMsg,
                 Cs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                             MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
      switch (MrIpcGetCommand(Data))
      {
         case MrIpcCmdNull:
            MrIpcCmdGetNull(Data, &(MrCs2GetDlc(CanMsg)),
                            (char *)MrCs2GetData(CanMsg));
            break;
         case MrIpcCmdRun:
            MrIpcCmdGetRun(Data, &Switch);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            if (Switch == Off)
            {
               MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_STOP);
               MrCs2SetDlc(CanMsg, 5);
               Cs2EncSysStop(MrCs2GetData(CanMsg), CS2_UID_BROADCAST);
            }
            else if (Switch == On)
            {
               MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_GO);
               MrCs2SetDlc(CanMsg, 5);
               Cs2EncSysGo(MrCs2GetData(CanMsg), CS2_UID_BROADCAST);
            }
            break;
         case MrIpcCmdTrackProto:
            MrIpcCmdGetTrackProto(Data, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncSysTrackProtocoll(MrCs2GetData(CanMsg), CS2_UID_BROADCAST, p1);
            break;
         case MrIpcCmdLocomotiveSpeed:
            MrIpcCmdGetLocomotiveSpeed(Data, &Addr, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_SPEED);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncSpeed6(MrCs2GetData(CanMsg), Addr, p1);
            break;
         case MrIpcCmdLocomotiveDirection:
            MrIpcCmdGetLocomotiveDir(Data, &Addr, &Direction);
            MrCs2SetCommand(CanMsg, CS2_CMD_DIRECTION);
            MrCs2SetDlc(CanMsg, 5);
            Cs2EncDirection5(MrCs2GetData(CanMsg), Addr, (int)Direction);
            break;
         case MrIpcCmdAccSwitch:
            MrIpcCmdGetAccPos(Data, &Addr, &Position, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_ACC_SWITCH);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncAccSwitch6(MrCs2GetData(CanMsg), Addr, (int)Position, p2);
            break;
         case MrIpcCmdLocomotiveFunction:
            MrIpcCmdGetLocomotiveFkt(Data, &Addr, &p1, &Switch);
            MrCs2SetCommand(CanMsg, CS2_CMD_FUNCTION);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncFunction6(MrCs2GetData(CanMsg), Addr, p1, (int)Switch);
            break;
         case MrIpcCmdRequestMember:
            MrIpcCmdGetRequestMember(Data);
            MrCs2SetCommand(CanMsg, CS2_CMD_PING);
            MrCs2SetDlc(CanMsg, 0);
            Cs2EncPing0(MrCs2GetData(CanMsg));
            break;
         case MrIpcCmdMember:
            MrIpcCmdGetMember(Data, &Addr, &p1, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_PING);
            MrCs2SetDlc(CanMsg, 8);
            Cs2EncPing8(MrCs2GetData(CanMsg), Addr, p1, p2);
            break;
         case MrIpcCmdRequestFile:
            MrIpcCmdGetQuery(Data, Buf);
            MrCs2SetCommand(CanMsg, CS2_CMD_CONFIG_QUERY);
            if (MrIpcGetRawDlc(Data) == 0)
            {
               MrCs2SetDlc(CanMsg, 0);
               Cs2EncConfigQuery0(MrCs2GetData(CanMsg));
            }
            else
            {
               MrCs2SetDlc(CanMsg, 8);
               Cs2EncConfigQuery(MrCs2GetData(CanMsg), Buf);
            }
            break;
         case MrIpcCmdCfgHeader:
            MrIpcCmdGetCfgHeader(Data, &Length, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_CFGDAT_STREAM);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncCfgdatStream6(MrCs2GetData(CanMsg), Length, p1);
            break;
         case MrIpcCmdCfgZHeader:
            MrIpcCmdGetCfgZHeader(Data, &Length, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_CFGDAT_STREAM);
            MrCs2SetDlc(CanMsg, 7);
            Cs2EncCfgdatStream7(MrCs2GetData(CanMsg), Length, p1);
            break;
         case MrIpcCmdCfgData:
            MrIpcCmdGetCfgData(Data, Buf);
            MrCs2SetCommand(CanMsg, CS2_CMD_CFGDAT_STREAM);
            MrCs2SetDlc(CanMsg, 8);
            Cs2EncCfgdatStream8(MrCs2GetData(CanMsg), Buf);
            break;
         case MrIpcCmdCanBootldrGeb:
            MrIpcCmdGetCanBootldr(Data, &p1, Buf);
            MrCs2SetCommand(CanMsg, CS2_CMD_BOOTLDR_CAN);
            MrCs2SetDlc(CanMsg, p1);
            Cs2EncCanBootldr(MrCs2GetData(CanMsg), Buf);
            break;
         case MrIpcCmdStatusRequest:
            MrIpcCmdGetStatusRequest(Data, &Addr, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_STATUS);
            MrCs2SetDlc(CanMsg, 5);
            Cs2EncStatus5(MrCs2GetData(CanMsg), Addr, p1);
            break;
         case MrIpcCmdStatusSize:
            MrIpcCmdGetStatusPos(Data, &Addr, &p1, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_STATUS);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncStatus6(MrCs2GetData(CanMsg), Addr, p1, p2);
            break;
         case MrIpcCmdStatusData:
            MrIpcCmdGetStatusData(Data, (unsigned char*)Buf);
            MrCs2SetCommand(CanMsg, CS2_CMD_STATUS);
            MrCs2SetDlc(CanMsg, 8);
            Cs2EncStatus8(MrCs2GetData(CanMsg), Buf);
            break;
         case MrIpcCmdStatusResponse:
            MrIpcCmdGetStatusResponse(Data, &Addr, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_STATUS);
            MrCs2SetDlc(CanMsg, 5);
            Cs2EncStatus5(MrCs2GetData(CanMsg), Addr, p1);
            break;
         case MrIpcCmdRequestMesswert:
            MrIpcCmdGetMesswertRequest(Data, &Addr, &p1);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_STATUS);
            MrCs2SetDlc(CanMsg, 6);
            Cs2EncSysStatus6(MrCs2GetData(CanMsg), Addr, p1);
            break;
         case MrIpcCmdSetConfig:
            MrIpcCmdGetSetConfig(Data, &Addr, &p1, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_STATUS);
            MrCs2SetDlc(CanMsg, 8);
            Cs2EncSysStatus8(MrCs2GetData(CanMsg), Addr, p1, p2);
            break;
         case MrIpcCmdConfigResponse:
            MrIpcCmdGetConfigResponse(Data, &Addr, &p1, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_STATUS);
            MrCs2SetDlc(CanMsg, 7);
            Cs2EncSysStatus7(MrCs2GetData(CanMsg), Addr, p1, p2);
            break;
         case MrIpcCmdMesswertResponse:
            MrIpcCmdGetMesswertResponse(Data, &Addr, &p1, &p2);
            MrCs2SetCommand(CanMsg, CS2_CMD_SYSTEM);
            MrCs2SetSystemSubcmd(CanMsg, CS2_SUBCMD_SYSTEM_STATUS);
            MrCs2SetDlc(CanMsg, 8);
            Cs2EncSysStatus8(MrCs2GetData(CanMsg), Addr, p1, p2);
            break;
      }
   }
}
