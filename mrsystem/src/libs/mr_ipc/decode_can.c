#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

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
      CanHash = (MrCs2GetHash(CanMsg) & ~MR_CS2_MASK_HASH_MAGIC) | MR_CS2_HASH_MAGIC;
      MrCs2SetId(CanMsg,
                 MrCs2EncodeId(CanHash, MrCs2GetResponse(CanMsg),
                               MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg)));
      switch (MrIpcGetCommand(Data))
      {
         case MrIpcCmdNull:
            MrIpcCmdGetNull(Data, &(MrCs2GetDlc(CanMsg)),
                            (char *)MrCs2GetData(CanMsg));
            break;
         case MrIpcCmdRun:
            MrIpcCmdGetRun(Data, &Switch);
            if (Switch == Off)
               MrCs2EncSysStop(CanMsg, MR_CS2_UID_BROADCAST);
            else if (Switch == On)
               MrCs2EncSysGo(CanMsg, MR_CS2_UID_BROADCAST);
            break;
         case MrIpcCmdTrackProto:
            MrIpcCmdGetTrackProto(Data, &p1);
            MrCs2EncSysTrackProtocoll(CanMsg, MR_CS2_UID_BROADCAST, p1);
            break;
         case MrIpcCmdLocomotiveSpeed:
            MrIpcCmdGetLocomotiveSpeed(Data, &Addr, &p1);
            MrCs2EncSpeed6(CanMsg, Addr, p1);
            break;
         case MrIpcCmdLocomotiveDirection:
            MrIpcCmdGetLocomotiveDir(Data, &Addr, &Direction);
            MrCs2EncDirection5(CanMsg, Addr, (int)Direction);
            break;
         case MrIpcCmdAccSwitch:
            MrIpcCmdGetAccPos(Data, &Addr, &Position, &p2);
            MrCs2EncAccSwitch6(CanMsg, Addr, (int)Position, p2);
            break;
         case MrIpcCmdLocomotiveFunction:
            MrIpcCmdGetLocomotiveFkt(Data, &Addr, &p1, &Switch);
            MrCs2EncFunction6(CanMsg, Addr, p1, (int)Switch);
            break;
         case MrIpcCmdRequestMember:
            MrIpcCmdGetRequestMember(Data);
            MrCs2EncPing0(CanMsg);
            break;
         case MrIpcCmdMember:
            MrIpcCmdGetMember(Data, &Addr, &p1, &p2);
            MrCs2EncPing8(CanMsg, Addr, p1, p2);
            break;
         case MrIpcCmdRequestFile:
            MrIpcCmdGetQuery(Data, Buf);
            if (MrIpcGetRawDlc(Data) == 0)
               MrCs2EncConfigQuery0(CanMsg);
            else
               MrCs2EncConfigQuery(CanMsg, Buf);
            break;
         case MrIpcCmdCfgHeader:
            MrIpcCmdGetCfgHeader(Data, &Length, &p1);
            MrCs2EncCfgdatStream6(CanMsg, Length, p1);
            break;
            break;
         case MrIpcCmdCfgZHeader:
            MrIpcCmdGetCfgZHeader(Data, &Length, &p1);
            MrCs2EncCfgdatStream7(CanMsg, Length, p1);
            break;
         case MrIpcCmdCfgData:
            MrIpcCmdGetCfgData(Data, Buf);
            MrCs2EncCfgdatStream8(CanMsg, Buf);
            break;
         case MrIpcCmdSystemStatusVal:
            MrIpcCmdGetSystemStatusVal(Data, &Addr, &p1, &p2);
            MrCs2EncSysStatus8(CanMsg, Addr, p1, p2);
            break;
         case MrIpcCmdCanBootldrGeb:
            MrIpcCmdGetCanBootldr(Data, &p1, Buf);
            MrCs2SetDlc(CanMsg, p1);
            MrCs2EncCanBootldr(CanMsg, Buf);
            break;
         case MrIpcCmdStatusRequest:
            MrIpcCmdGetStatusRequest(Data, &Addr, &p1);
            MrCs2EncStatus5(CanMsg, Addr, p1);
            break;
         case MrIpcCmdStatusSize:
            MrIpcCmdGetStatusPos(Data, &Addr, &p1, &p2);
            MrCs2EncStatus6(CanMsg, Addr, p1, p2);
            break;
         case MrIpcCmdStatusData:
            MrIpcCmdGetStatusData(Data, (unsigned char*)Buf);
            MrCs2EncStatus8(CanMsg, Buf);
            break;
      }
   }
}
