#include <stddef.h>
#include <cs2.h>
#include <cs2.h>
#include <mr_cs2ms2.h>
#include "mr_ipc.h"

/** @file */

/**
* @brief IPC aus CAN erzeugen
*
* Diese Funktion codiert die CanMsg in ein IPC Paket.
*
* @param[in] Data Zeiger auf die IPC Struktur
* @param[in] CanMsg zu codierende CAN Message
*/
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
      switch (MrCs2GetCommand(CanMsg))
      {
         case CS2_CMD_SYSTEM:
            switch (MrCs2GetSystemSubcmd(CanMsg))
            {
               case CS2_SUBCMD_SYSTEM_STOP:
                  Cs2DecSysStop(MrCs2GetData(CanMsg), &Uid);
                  MrIpcCmdSetRun(Data, Off);
                  break;
               case CS2_SUBCMD_SYSTEM_GO:
                  Cs2DecSysGo(MrCs2GetData(CanMsg), &Uid);
                  MrIpcCmdSetRun(Data, On);
                  break;
               case CS2_SUBCMD_SYSTEM_HALT:
                  Cs2DecSysHalt(MrCs2GetData(CanMsg), &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_EMERGENCY_HALT:
                  Cs2DecSysEmergencyHalt(MrCs2GetData(CanMsg), &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_CYCLE_END:
                  Cs2DecSysCycleEnd(MrCs2GetData(CanMsg), &Uid);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL:
                  Cs2DecSysDataProtocoll(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_DECODER_TIMEING:
                  Cs2DecSysDecoderTimeing(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_MFX_FAST_READ:
                  Cs2DecSysMfxFastRead(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL:
                  Cs2DecSysTrackProtocoll(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetTrackProto(Data, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER:
                  Cs2DecSysResetMfxCounter(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_OVERLOAD:
                  Cs2DecSysOverload(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_STATUS:
                  switch (MrCs2GetDlc(CanMsg))
                  {
                     case 6:
                        Cs2DecSysStatus6(MrCs2GetData(CanMsg), &Uid, &p1);
                        MrIpcCmdSetMesswertRequest(Data, Uid, p1);
                        break;
                     case 7:
                        Cs2DecSysStatus7(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
                        MrIpcCmdSetConfigResponse(Data, Uid, p1, p2);
                        break;
                     case 8:
                        Cs2DecSysStatus8(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
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
               case CS2_SUBCMD_SYSTEM_SYSTEM_ID:
                  switch (MrCs2GetDlc(CanMsg))
                  {
                     case 5:
                        Cs2DecSysSystemId5(MrCs2GetData(CanMsg), &Uid);
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                     case 7:
                        Cs2DecSysSystemId7(MrCs2GetData(CanMsg), &Uid, &p1);
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                     default:
                        MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                        MrCs2GetData(CanMsg));
                        break;
                  }
                  break;
               case CS2_SUBCMD_SYSTEM_MFX_SEEK:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case CS2_SUBCMD_SYSTEM_SYSTEM_RESET:
                  Cs2DecSysSystemReset(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_DISCOVERY:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecDiscovery0(MrCs2GetData(CanMsg));
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 1:
                  Cs2DecDiscovery1(MrCs2GetData(CanMsg), &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  Cs2DecDiscovery5(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 6:
                  Cs2DecDiscovery6(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_BIND:
            Cs2DecBind(MrCs2GetData(CanMsg), &Uid, &s1);
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_MFX_VERIFY:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecMfxVerify6(MrCs2GetData(CanMsg), &Uid, &s1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  Cs2DecMfxVerify7(MrCs2GetData(CanMsg), &Uid, &s1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_SPEED:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecSpeed4(MrCs2GetData(CanMsg), &LocId);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 6:
                  Cs2DecSpeed6(MrCs2GetData(CanMsg), &LocId, &p1);
                  MrIpcCmdSetLocomotiveSpeed(Data, LocId, p1);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_DIRECTION:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecDirection4(MrCs2GetData(CanMsg), &LocId);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  Cs2DecDirection5(MrCs2GetData(CanMsg), &LocId, &p1);
                  MrIpcCmdSetLocomotiveDir(Data, LocId, (DirectionType)p1);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_FUNCTION:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecFunction5(MrCs2GetData(CanMsg), &LocId, &p1);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, Off);
                  break;
               case 6:
                  Cs2DecFunction6(MrCs2GetData(CanMsg), &LocId, &p1, &p2);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, p2);
                  break;
               case 8:
                  Cs2DecFunction8(MrCs2GetData(CanMsg), &LocId, &p1, &p2, &p3);
                  MrIpcCmdSetLocomotiveFkt(Data, LocId, p1, p2);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_READ_CONFIG:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecReadConfig6(MrCs2GetData(CanMsg), &LocId, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  Cs2DecReadConfig7(MrCs2GetData(CanMsg), &LocId, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_WRITE_CONFIG:
            Cs2DecWriteConfig(MrCs2GetData(CanMsg), &LocId, &p1, &p2, &p3);
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_ACC_SWITCH:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecAccSwitch6(MrCs2GetData(CanMsg), &LocId, &p1, &p2);
                  MrIpcCmdSetAccPos(Data, LocId, (PositionType)p1, p2);
                  break;
               case 8:
                  Cs2DecAccSwitch8(MrCs2GetData(CanMsg), &LocId, &p1, &p2, &p3);
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
         case CS2_CMD_ACC_CONF:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_S88_POLL:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecS88Poll5(MrCs2GetData(CanMsg), &Uid, &p1);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 7:
                  Cs2DecS88Poll7(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_S88_EVENT:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecS88Event4(MrCs2GetData(CanMsg), &p1, &p2);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 5:
                  Cs2DecS88Event5(MrCs2GetData(CanMsg), &p1, &p2, &p3);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 8:
                  Cs2DecS88Event8(MrCs2GetData(CanMsg), &p1, &p2, &p3, &p4, &p5);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_SX1_EVENT:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_PING:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecPing0(MrCs2GetData(CanMsg));
                  MrIpcCmdSetRequestMember(Data);
                  break;
               case 8:
                  Cs2DecPing8(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
                  MrIpcCmdSetMember(Data, Uid, p1, p2);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_OFFER_UPDATE:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_READ_CFG_DATA:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_BOOTLDR_CAN:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_BOOTLDR_TRACK:
            Cs2DecCanBootldr(MrCs2GetData(CanMsg), Buf);
            MrIpcCmdSetCanBootldr(Data, MrCs2GetDlc(CanMsg),
                                  (unsigned char *)Buf);
            break;
         case CS2_CMD_STATUS:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecStatus5(MrCs2GetData(CanMsg), &Uid, &p1);
                  if (MrCs2GetResponse(CanMsg))
                     MrIpcCmdSetStatusResponse(Data, Uid, p1);
                  else
                     MrIpcCmdSetStatusRequest(Data, Uid, p1);
                  break;
               case 6:
                  Cs2DecStatus6(MrCs2GetData(CanMsg), &Uid, &p1, &p2);
                  MrIpcCmdSetStatusPos(Data, Uid, p1, p2);
                  break;
               case 8:
                  Cs2DecStatus8(MrCs2GetData(CanMsg), Buf);
                  MrIpcCmdSetStatusData(Data, MrCs2GetData(CanMsg));
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_CONFIG_QUERY:
            Cs2DecConfigQuery(MrCs2GetData(CanMsg), Buf);
            MrIpcCmdSetQuery(Data, MrCs2GetDlc(CanMsg), Buf);
            break;
         case CS2_CMD_CFGDAT_STREAM:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecCfgdatStream6(MrCs2GetData(CanMsg), &Length, &p1);
                  MrIpcCmdSetCfgHeader(Data, Length, p1);
                  break;
               case 7:
                  Cs2DecCfgdatStream7(MrCs2GetData(CanMsg), &Length, &p1);
                  MrIpcCmdSetCfgZHeader(Data, Length, p1);
                  break;
               case 8:
                  Cs2DecCfgdatStream8(MrCs2GetData(CanMsg), Buf);
                  MrIpcCmdSetCfgData(Data, Buf);
                  break;
               default:
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
            }
            break;
         case CS2_CMD_CONNECT_6021:
            MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                            MrCs2GetData(CanMsg));
            break;
         case CS2_CMD_AUTOMATIC:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecAutomatic6(MrCs2GetData(CanMsg), &p1, &p2, &p3, &p4);
                  MrIpcCmdSetNull(Data, MrCs2GetDlc(CanMsg),
                                  MrCs2GetData(CanMsg));
                  break;
               case 8:
                  Cs2DecAutomatic8(MrCs2GetData(CanMsg), &p1, &p2, &LocId);
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
