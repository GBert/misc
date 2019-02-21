#include <stddef.h>
#include "mr_ipc.h"

void MrIpcEncodeFromCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg)
{  int p1, p2, p3, p4, p5;
   unsigned int s1;
   unsigned long Uid, LocId, Length;
   char Buf[9];

   if (Data != (MrIpcCmdType *)NULL)
   {
      MrIpcSetCanResponse(Data, MrCs2GetResponse(CanMsg));
      MrIpcSetCanHash(Data, MrCs2GetHash(CanMsg));
      MrIpcSetCanCommand(Data, MrCs2GetCommand(CanMsg));
      MrIpcSetCanPrio(Data, MrCs2GetPrio(CanMsg));
      switch (MrMs2GetCommand(CanMsg))
      {
         case MR_MS2_CMD_SYSTEM:
            switch (MrMs2GetSystemSubcmd(CanMsg))
            {
               case MR_MS2_SUBCMD_SYSTEM_STOP:
                  MrMs2DecSysStop(CanMsg, &Uid);
                  MrIpcCmdSetRun(Data, Off);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_GO:
                  MrMs2DecSysGo(CanMsg, &Uid);
                  MrIpcCmdSetRun(Data, On);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_HALT:
                  MrMs2DecSysHalt(CanMsg, &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_EMERGENCY_HALT:
                  MrMs2DecSysEmergencyHalt(CanMsg, &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_CYCLE_END:
                  MrMs2DecSysCycleEnd(CanMsg, &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_DATA_PROTOCOLL:
                  MrMs2DecSysDataProtocoll(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_DECODER_TIMEING:
                  MrMs2DecSysDecoderTimeing(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_MFX_FAST_READ:
                  MrMs2DecSysMfxFastRead(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL:
                  MrCs2DecSysTrackProtocoll(CanMsg, &Uid, &p1);
                  MrIpcCmdSetTrackProto(Data, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER:
                  MrMs2DecSysResetMfxCounter(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_OVERLOAD:
                  MrCs2DecSysOverload(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_STATUS:
                  switch (MrCs2GetNumParamBytes(CanMsg))
                  {
                     case 6:
                        MrMs2DecSysStatus6(CanMsg, &Uid, &p1);
                        MrIpcCmdSetMesswertRequest(Data, Uid, p1);
                        break;
                     case 7:
                        MrMs2DecSysStatus7(CanMsg, &Uid, &p1, &p2);
                        MrIpcCmdSetConfigResponse(Data, Uid, p1, p2);
                        break;
                     case 8:
                        MrMs2DecSysStatus8(CanMsg, &Uid, &p1, &p2);
                        if (MrCs2GetResponse(CanMsg))
                           MrIpcCmdSetMesswertResponse(Data, Uid, p1, p2);
                        else
                           MrIpcCmdSetSetConfig(Data, Uid, p1, p2);
                        break;
                     default:
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                  }
                  break;
               case MR_MS2_SUBCMD_SYSTEM_SYSTEM_ID:
                  switch (MrCs2GetNumParamBytes(CanMsg))
                  {
                     case 5:
                        MrMs2DecSysSystemId5(CanMsg, &Uid);
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                     case 7:
                        MrMs2DecSysSystemId7(CanMsg, &Uid, &p1);
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                     default:
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                  }
                  break;
               case MR_MS2_SUBCMD_SYSTEM_MFX_SEEK:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case MR_MS2_SUBCMD_SYSTEM_SYSTEM_RESET:
                  MrMs2DecSysSystemReset(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_DISCOVERY:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecDiscovery0(CanMsg);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 1:
                  MrCs2DecDiscovery1(CanMsg, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  MrCs2DecDiscovery5(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 6:
                  MrCs2DecDiscovery6(CanMsg, &Uid, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_BIND:
            MrCs2DecBind(CanMsg, &Uid, &s1);
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_MFX_VERIFY:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecMfxVerify6(CanMsg, &Uid, &s1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  MrCs2DecMfxVerify7(CanMsg, &Uid, &s1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_SPEED:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecSpeed4(CanMsg, &LocId);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 6:
                  MrCs2DecSpeed6(CanMsg, &LocId, &p1);
                  MrIpcCmdSetLocomotiveSpeed(Data, LocId, p1);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_DIRECTION:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecDirection4(CanMsg, &LocId);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  MrCs2DecDirection5(CanMsg, &LocId, &p1);
                  MrIpcCmdSetLocomotiveDir(Data, LocId, (DirectionType)p1);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_FUNCTION:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecFunction5(CanMsg, &LocId, &p1);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, Off);
                  break;
               case 6:
                  MrCs2DecFunction6(CanMsg, &LocId, &p1, &p2);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, p2);
                  break;
               case 8:
                  MrCs2DecFunction8(CanMsg, &LocId, &p1, &p2, &p3);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, p2);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_READ_CONFIG:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecReadConfig6(CanMsg, &LocId, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  MrCs2DecReadConfig7(CanMsg, &LocId, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_WRITE_CONFIG:
            MrCs2DecWriteConfig(CanMsg, &LocId, &p1, &p2, &p3);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_ACC_SWITCH:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecAccSwitch6(CanMsg, &LocId, &p1, &p2);
                  MrIpcCmdSetAccPos(Data, LocId, (PositionType)p1, p2);
                  break;
               case 8:
                  MrCs2DecAccSwitch8(CanMsg, &LocId, &p1, &p2, &p3);
                  MrIpcCmdSetAccPos(Data, LocId, (PositionType)p1, p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_ACC_CONF:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_S88_POLL:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecS88Poll5(CanMsg, &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  MrCs2DecS88Poll7(CanMsg, &Uid, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_S88_EVENT:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecS88Event4(CanMsg, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  MrCs2DecS88Event5(CanMsg, &p1, &p2, &p3);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 8:
                  MrCs2DecS88Event8(CanMsg, &p1, &p2, &p3, &p4, &p5);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_SX1_EVENT:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_PING:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecPing0(CanMsg);
                  MrIpcCmdSetRequestMember(Data);
                  break;
               case 8:
                  MrCs2DecPing8(CanMsg, &Uid, &p1, &p2);
                  MrIpcCmdSetMember(Data, Uid, p1, p2);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_OFFER_UPDATE:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_READ_CFG_DATA:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_BOOTLDR_CAN:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_MS2_CMD_BOOTLDR_TRACK:
            MrCs2DecCanBootldr(CanMsg, Buf);
            MrIpcCmdSetCanBootldr(Data, MrCs2GetDlc(CanMsg),
                                  (unsigned char *)Buf);
            break;
         case MR_MS2_CMD_STATUS:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecStatus5(CanMsg, &Uid, &p1);
                  if (MrCs2GetResponse(CanMsg))
                     MrIpcCmdSetStatusResponse(Data, Uid, p1);
                  else
                     MrIpcCmdSetStatusRequest(Data, Uid, p1);
                  break;
               case 6:
                  MrCs2DecStatus6(CanMsg, &Uid, &p1, &p2);
                  MrIpcCmdSetStatusPos(Data, Uid, p1, p2);
                  break;
               case 8:
                  MrCs2DecStatus8(CanMsg, Buf);
                  MrIpcCmdSetStatusData(Data, MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_CONFIG_QUERY:
            MrCs2DecConfigQuery(CanMsg, Buf);
            MrIpcCmdSetQuery(Data, MrCs2GetDlc(CanMsg), Buf);
            break;
         case MR_MS2_CMD_CFGDAT_STREAM:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecCfgdatStream6(CanMsg, &Length, &p1);
                  MrIpcCmdSetCfgHeader(Data, Length, p1);
                  break;
               case 7:
                  MrCs2DecCfgdatStream7(CanMsg, &Length, &p1);
                  MrIpcCmdSetCfgZHeader(Data, Length, p1);
                  break;
               case 8:
                  MrCs2DecCfgdatStream8(CanMsg, Buf);
                  MrIpcCmdSetCfgData(Data, Buf);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case MR_MS2_CMD_CONNECT_6021:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case MR_CS2_CMD_AUTOMATIC:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecAutomatic6(CanMsg, &p1, &p2, &p3, &p4);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 8:
                  MrCs2DecAutomatic8(CanMsg, &p1, &p2, &LocId);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         default:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
      }
   }
}
