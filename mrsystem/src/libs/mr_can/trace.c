#include <stdlib.h>
#include <stdio.h>
#include "mr_can.h"

void MrCs2Trace(MrCs2CanDataType *CanMsg)
{  int i, p1, p2, p3, p4, p5;
   unsigned int s1;
   unsigned long Uid, LocId, Length;
   char Buf[9];

   if (MrMs2IsMs2(CanMsg))
   {
      printf("can data 0x%lx %d\n    ", CanMsg->Id, CanMsg->Dlc);
      for (i = 0; i < 8; i++)
         printf("0x%02x ", CanMsg->Data[i]);
      printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
             CanMsg->Hash, CanMsg->Response, CanMsg->Command, CanMsg->Prio);

      switch (MrMs2GetCommand(CanMsg))
      {
         case MR_MS2_CMD_SYSTEM:
            puts("System-Befehle\nSubcommand: ");
            switch (MrMs2GetSystemSubcmd(CanMsg))
            {
               case MR_MS2_SUBCMD_SYSTEM_STOP:
                  MrMs2DecSysStop(CanMsg, &Uid);
                  printf("System Stopp (Uid = 0x%lx)\n", Uid);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_GO:
                  MrMs2DecSysGo(CanMsg, &Uid);
                  printf("System Go (Uid = 0x%lx)\n", Uid);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_HALT:
                  MrMs2DecSysHalt(CanMsg, &Uid);
                  printf("System Halt (Uid = 0x%lx)\n", Uid);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_EMERGENCY_HALT:
                  MrMs2DecSysEmergencyHalt(CanMsg, &Uid);
                  printf("Lok Nothalt (Uid = 0x%lx)\n", Uid);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_CYCLE_END:
                  MrMs2DecSysCycleEnd(CanMsg, &Uid);
                  printf("Lok Zyklus Stopp(beenden) (Uid = 0x%lx)\n", Uid);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_DATA_PROTOCOLL:
                  MrMs2DecSysDataProtocoll(CanMsg, &Uid, &p1);
                  printf("Lok Datenprotokoll (Uid = 0x%lx, proto = %d)\n",
                         Uid, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_DECODER_TIMEING:
                  MrMs2DecSysDecoderTimeing(CanMsg, &Uid, &p1);
                  printf("Schaltzeit Zubehördecoder (Uid = 0x%lx, time = %d ms)\n",
                         Uid, p1 * 10);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_MFX_FAST_READ:
                  MrMs2DecSysMfxFastRead(CanMsg, &Uid, &p1);
                  printf("Fast Read für mfx (Uid = 0x%lx, mgx-sid = %d)\n",
                         Uid, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL:
                  MrCs2DecSysTrackProtocoll(CanMsg, &Uid, &p1);
                  printf("Gleisprotokoll frei schalten (Uid = 0x%lx, proto = %d)\n",
                         Uid, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER:
                  MrMs2DecSysResetMfxCounter(CanMsg, &Uid, &p1);
                  printf("System MFX Neuanmeldezähler setzen (Uid = 0x%lx, counter = %d)\n",
                         Uid, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_OVERLOAD:
                  MrCs2DecSysOverload(CanMsg, &Uid, &p1);
                  printf("System Überlast (Uid = 0x%lx, channel = %d)\n",
                         Uid, p1);
                  break;
               case MR_MS2_SUBCMD_SYSTEM_STATUS:
                  switch (MrCs2GetNumParamBytes(CanMsg))
                  {
                     case 6:
                        MrMs2DecSysStatus6(CanMsg, &Uid, &p1);
                        printf("System Status  (Uid = 0x%lx, channel = %d)\n",
                               Uid, p1);
                        break;
                     case 7:
                        MrMs2DecSysStatus7(CanMsg, &Uid, &p1, &p2);
                        printf("System Status  (Uid = 0x%lx, channel = %d, %s)\n",
                               Uid, p1, p2?"true":"false");
                        break;
                     case 8:
                        MrMs2DecSysStatus8(CanMsg, &Uid, &p1, &p2);
                        printf("System Status  (Uid = 0x%lx, channel = %d, value = %ds)\n",
                               Uid, p1, p2);
                        break;
                     default:
                        puts("System Status (unknown parameter)");
                        break;
                  }
                  break;
               case MR_MS2_SUBCMD_SYSTEM_SYSTEM_ID:
                  switch (MrCs2GetNumParamBytes(CanMsg))
                  {
                     case 5:
                        MrMs2DecSysSystemId5(CanMsg, &Uid);
                        printf("System Kennung (Uid = 0x%lx)\n", Uid);
                        break;
                     case 7:
                        MrMs2DecSysSystemId7(CanMsg, &Uid, &p1);
                        printf("System Kennung (Uid = 0x%lx, id = %d)\n",
                               Uid, p1);
                        break;
                     default:
                        puts("System Kennung (unknown parameter)");
                        break;
                  }
                  break;
               case MR_MS2_SUBCMD_SYSTEM_MFX_SEEK:
                  puts("Mfx Seek, parameter not defined");
                  break;
               case MR_MS2_SUBCMD_SYSTEM_SYSTEM_RESET:
                  MrMs2DecSysSystemReset(CanMsg, &Uid, &p1);
                  printf("System Reset (Uid = 0x%lx, target = %d)\n",
                         Uid, p1);
                  break;
               default:
                  puts("system subcommand not defined");
                  break;
            }
            break;
         case MR_MS2_CMD_DISCOVERY:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecDiscovery0(CanMsg);
                  puts("Lok Discovery");
                  break;
               case 1:
                  MrCs2DecDiscovery1(CanMsg, &p1);
                  printf("Lok Discovery (Proto = %d)\n", p1);
                  break;
               case 5:
                  MrCs2DecDiscovery5(CanMsg, &Uid, &p1);
                  printf("Lok Discovery (Uid = 0x%lx, Range = %d)\n",
                         Uid, p1);
                  break;
               case 6:
                  MrCs2DecDiscovery6(CanMsg, &Uid, &p1, &p2);
                  printf("Lok Discovery (Uid = 0x%lx, Range = %d, Ask = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Lok Discovery (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_BIND:
            MrCs2DecBind(CanMsg, &Uid, &s1);
            printf("MFX Bind (Uid = 0x%lx, Sid = 0x%x)\n", Uid, s1);
            break;
         case MR_MS2_CMD_MFX_VERIFY:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecMfxVerify6(CanMsg, &Uid, &s1);
                  printf("MFX Verify (Uid = 0x%lx, Sid = 0x%x)\n", Uid, s1);
                  break;
               case 7:
                  MrCs2DecMfxVerify7(CanMsg, &Uid, &s1, &p2);
                  printf("MFX Verify (Uid = 0x%lx, Sid = 0x%x, Ask = %d)\n",
                         Uid, s1, p2);
                  break;
               default:
                  puts("MFX Verify (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_SPEED:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecSpeed4(CanMsg, &LocId);
                  printf("Lok Geschwindigkeit (LocId = 0x%lx)\n", LocId);
                  break;
               case 6:
                  MrCs2DecSpeed6(CanMsg, &LocId, &p1);
                  printf("Lok Geschwindigkeit (LocId = 0x%lx, Speed = %d)\n",
                         LocId, p1);
                  break;
               default:
                  puts("Lok Geschwindigkeit (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_DIRECTION:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecDirection4(CanMsg, &LocId);
                  printf("Lok Richtung (LocId = 0x%lx)\n", LocId);
                  break;
               case 5:
                  MrCs2DecDirection5(CanMsg, &LocId, &p1);
                  printf("Lok Richtung (LocId = 0x%lx, Direction = %d)\n",
                         LocId, p1);
                  break;
               default:
                  puts("Lok Richtung (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_FUNCTION:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecFunction5(CanMsg, &LocId, &p1);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d)\n",
                         LocId, p1);
                  break;
               case 6:
                  MrCs2DecFunction6(CanMsg, &LocId, &p1, &p2);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d, Value = %d)\n",
                         LocId, p1, p2);
                  break;
               case 8:
                  MrCs2DecFunction8(CanMsg, &LocId, &p1, &p2, &p3);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d, Value = %d, FktVal = %d)\n",
                         LocId, p1, p2, p3);
                  break;
               default:
                  puts("Lok Funktion (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_READ_CONFIG:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecReadConfig6(CanMsg, &LocId, &p1);
                  printf("Read Config (LocId = 0x%lx, CvIndex = %d)\n",
                         LocId, p1);
                  break;
               case 7:
                  MrCs2DecReadConfig7(CanMsg, &LocId, &p1, &p2);
                  printf("Read Config (LocId = 0x%lx, CvIndex = %d, Param = %d)\n",
                         LocId, p1, p2);
                  break;
               default:
                  puts("Read Config (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_WRITE_CONFIG:
            MrCs2DecWriteConfig(CanMsg, &LocId, &p1, &p2, &p3);
            printf("Write Config (LocId = 0x%lx, CvIndex = %d, Value = %d, CtrlRslt = %d)\n",
                   LocId, p1, p2, p3);
            break;
         case MR_MS2_CMD_ACC_SWITCH:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecAccSwitch6(CanMsg, &LocId, &p1, &p2);
                  printf("Zubehör Schalten (LocId = 0x%lx, Position = %d, Current = %d)\n",
                         LocId, p1, p2);
                  break;
               case 8:
                  MrCs2DecAccSwitch8(CanMsg, &LocId, &p1, &p2, &p3);
                  printf("Zubehör Schalten (LocId = 0x%lx, Position = %d, Current = %d, TimeFkt = %d)\n",
                         LocId, p1, p2, p3);
                  break;
               default:
                  puts("Zubehör Schalten (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_ACC_CONF:
            puts("Zubehör Konfig (unknown parameter)");
            break;
         case MR_MS2_CMD_S88_POLL:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecS88Poll5(CanMsg, &Uid, &p1);
                  printf("S88 Polling (Feedback) (Uid = 0x%lx, NumModuls = %d)\n",
                         Uid, p1);
                  break;
               case 7:
                  MrCs2DecS88Poll7(CanMsg, &Uid, &p1, &p2);
                  printf("S88 Polling (Feedback) (Uid = 0x%lx, Modul = %d, State = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("S88 Polling (Feedback) (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_S88_EVENT:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 4:
                  MrCs2DecS88Event4(CanMsg, &p1, &p2);
                  printf("S88 Event (Device = %d, Contact = %d)\n", p1, p2);
                  break;
               case 5:
                  MrCs2DecS88Event5(CanMsg, &p1, &p2, &p3);
                  printf("S88 Event (Device = %d, Contact = %d, Param = %d)\n",
                         p1, p2, p3);
                  break;
               case 8:
                  MrCs2DecS88Event8(CanMsg, &p1, &p2, &p3, &p4, &p5);
                  printf("S88 Event (Device = %d, Contact = %d, StateOld = %d, StateNew = %d, Time = %d)\n",
                         p1, p2, p3, p4, p5);
                  break;
               default:
                  puts("S88 Event (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_SX1_EVENT:
            puts("SX1 Event (unknown parameter)");
            break;
         case MR_MS2_CMD_PING:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecPing0(CanMsg);
                  puts("Softwarestand Anfrage/Teilnehmer Ping");
                  break;
               case 8:
                  MrCs2DecPing8(CanMsg, &Uid, &p1, &p2);
                  printf("Softwarestand Anfrage/Teilnehmer Ping (Uid = 0x%lx, SwVersion = %d, DeviceId = 0x%x)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Softwarestand Anfrage/Teilnehmer Ping (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_OFFER_UPDATE:
            puts("Updateangebot (unknown parameter)");
            break;
         case MR_MS2_CMD_READ_CFG_DATA:
            puts("Read Config Data (unknown parameter)");
            break;
         case MR_MS2_CMD_BOOTLDR_CAN:
            puts("Bootloader CAN gebunden, 'Service' (unknown parameter)");
            break;
         case MR_MS2_CMD_BOOTLDR_TRACK:
            puts("Bootloader Schienen gebunden, 'Service' (unknown parameter)");
            break;
         case MR_MS2_CMD_STATUS:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 5:
                  MrCs2DecStatus5(CanMsg, &Uid, &p1);
                  printf("Statusdaten Konfiguration (Uid = 0x%lx, Index = %d)\n",
                         Uid, p1);
                  break;
               case 6:
                  MrCs2DecStatus6(CanMsg, &Uid, &p1, &p2);
                  printf("Statusdaten Konfiguration (Uid = 0x%lx, Index = %d, NumPackets = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Statusdaten Konfiguration (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_CONFIG_QUERY:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 0:
                  MrCs2DecConfigQuery0(CanMsg);
                  puts("Anfordern Config Data, 'Data Query'");
                  break;
               case 8:
                  MrCs2DecConfigQuery(CanMsg, Buf);
                  printf("Anfordern Config Data, 'Data Query' (FileName = %s)\n",
                         Buf);
                  break;
            }
            break;
         case MR_MS2_CMD_CFGDAT_STREAM:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecCfgdatStream6(CanMsg, &Length, &p1);
                  printf("Config Data Stream (Length = %ld, Crc = %d)\n",
                         Length, p1);
                  break;
               case 7:
                  MrCs2DecCfgdatStream7(CanMsg, &Length, &p1);
                  printf("Config Zlib Data Stream (Length = %ld, Crc = %d)\n",
                         Length, p1);
                  break;
               case 8:
                  MrCs2DecCfgdatStream8(CanMsg, Buf);
                  printf("Config Data Stream (");
                  for (i = 0; i < 8; i++)
                     printf("0x%x ", Buf[i]);
                  puts("");
                  break;
               default:
                  puts("Config Data Stream (unknown parameter)");
                  break;
            }
            break;
         case MR_MS2_CMD_CONNECT_6021:
            puts("60128 (Connect 6021) Data Stream");
            puts("alte Bezeichnung '6021 adapter'");
            puts("(unknown parameter)");
            break;
         case MR_CS2_CMD_AUTOMATIC:
            switch (MrCs2GetNumParamBytes(CanMsg))
            {
               case 6:
                  MrCs2DecAutomatic6(CanMsg, &p1, &p2, &p3, &p4);
                  printf("automatic (DeviceId = %d, Func = %d, Status = %d, Param = %d)\n",
                         p1, p2, p3, p4);
                  break;
               case 8:
                  MrCs2DecAutomatic8(CanMsg, &p1, &p2, &LocId);
                  printf("automatic (DeviceId = %d, Func = %d, LocId = 0x%lx)\n",
                         p1, p2, LocId);
                  break;
               default:
                  puts("automatic (unknown parameter)");
                  break;
            }
            break;
         default:
            puts("CAN command not defined");
            break;
      }
      puts("");
   }
}
