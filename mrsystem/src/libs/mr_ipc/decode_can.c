#include <stddef.h>
#include <string.h>
#include "mr_ipc.h"

void MrIpcDecodeToCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg)
{  unsigned long Addr, Length;
   unsigned p1, p2;
   SwitchType Switch;
   DirectionType Direction;
   PositionType Position;
   char Buf[9];

   if (Data != (MrIpcCmdType *)NULL)
   {
      MrCs2SetIsCs2(CanMsg, TRUE);
      MrCs2SetResponse(CanMsg, MrIpcGetCanResponse(Data));
      MrCs2SetHash(CanMsg, MrIpcGetCanHash(Data));
      MrCs2SetCommand(CanMsg, MrIpcGetCanCommand(Data));
      MrCs2SetPrio(CanMsg, MrIpcGetCanPrio(Data));
      switch (MrIpcGetCommand(Data))
      {
         case MrIpcCmdNull:
            MrIpcCmdGetNull(Data, &(MrCs2GetDlc(CanMsg)), Buf);
            break;
         case MrIpcCmdRun:
            MrIpcCmdGetRun(Data, &Switch);
            if (Switch == Off)
               MrCs2EncSysStop(CanMsg, MR_CS2_UID_BROADCAST);
            else if (Switch == On)
               MrCs2EncSysGo(CanMsg, MR_CS2_UID_BROADCAST);
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
            MrIpcCmdGetAccPos(Data, &Addr, &Position);
            MrCs2EncAccSwitch6(CanMsg, Addr, (int)Position, 0);
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
      }
   }
}
