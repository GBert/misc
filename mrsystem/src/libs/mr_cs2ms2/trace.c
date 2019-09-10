#include <stdlib.h>
#include <stdio.h>
#include <cs2.h>
#include "mr_cs2ms2.h"

void MrCs2Trace(MrCs2CanDataType *CanMsg)
{  int i, p1, p2, p3, p4, p5;
   unsigned int s1;
   unsigned long Uid, LocId, Length;
   char Buf[9];

   if (MrCs2GetIsCs2(CanMsg))
   {
      printf("can data 0x%lx %d\n    ", CanMsg->Id, CanMsg->Dlc);
      for (i = 0; i < 8; i++)
         printf("0x%02x ", CanMsg->Data[i]);
      printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
             CanMsg->Hash, CanMsg->Response, CanMsg->Command, CanMsg->Prio);

      switch (MrCs2GetCommand(CanMsg))
      {
         case CS2_CMD_SYSTEM:
            puts("System-Befehle\nSubcommand: ");
            switch (MrCs2GetSystemSubcmd(CanMsg))
            {
               case CS2_SUBCMD_SYSTEM_STOP:
                  Cs2DecSysStop(CanMsg->Data, &Uid);
                  printf("System Stopp (Uid = 0x%lx)\n", Uid);
                  break;
               case CS2_SUBCMD_SYSTEM_GO:
                  Cs2DecSysGo(CanMsg->Data, &Uid);
                  printf("System Go (Uid = 0x%lx)\n", Uid);
                  break;
               case CS2_SUBCMD_SYSTEM_HALT:
                  Cs2DecSysHalt(CanMsg->Data, &Uid);
                  printf("System Halt (Uid = 0x%lx)\n", Uid);
                  break;
               case CS2_SUBCMD_SYSTEM_EMERGENCY_HALT:
                  Cs2DecSysEmergencyHalt(CanMsg->Data, &Uid);
                  printf("Lok Nothalt (Uid = 0x%lx)\n", Uid);
                  break;
               case MS2_SUBCMD_SYSTEM_CYCLE_END:
                  Cs2DecSysCycleEnd(CanMsg->Data, &Uid);
                  printf("Lok Zyklus Stopp(beenden) (Uid = 0x%lx)\n", Uid);
                  break;
               case MS2_SUBCMD_SYSTEM_DATA_PROTOCOLL:
                  Cs2DecSysDataProtocoll(CanMsg->Data, &Uid, &p1);
                  printf("Lok Datenprotokoll (Uid = 0x%lx, proto = %d)\n",
                         Uid, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_DECODER_TIMEING:
                  Cs2DecSysDecoderTimeing(CanMsg->Data, &Uid, &p1);
                  printf("Schaltzeit Zubehördecoder (Uid = 0x%lx, time = %d ms)\n",
                         Uid, p1 * 10);
                  break;
               case CS2_SUBCMD_SYSTEM_MFX_FAST_READ:
                  Cs2DecSysMfxFastRead(CanMsg->Data, &Uid, &p1);
                  printf("Fast Read für mfx (Uid = 0x%lx, mgx-sid = %d)\n",
                         Uid, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL:
                  Cs2DecSysTrackProtocoll(CanMsg->Data, &Uid, &p1);
                  printf("Gleisprotokoll frei schalten (Uid = 0x%lx, proto = %d)\n",
                         Uid, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER:
                  Cs2DecSysResetMfxCounter(CanMsg->Data, &Uid, &p1);
                  printf("System MFX Neuanmeldezähler setzen (Uid = 0x%lx, counter = %d)\n",
                         Uid, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_OVERLOAD:
                  Cs2DecSysOverload(CanMsg->Data, &Uid, &p1);
                  printf("System Überlast (Uid = 0x%lx, channel = %d)\n",
                         Uid, p1);
                  break;
               case CS2_SUBCMD_SYSTEM_STATUS:
                  switch (MrCs2GetDlc(CanMsg))
                  {
                     case 6:
                        Cs2DecSysStatus6(CanMsg->Data, &Uid, &p1);
                        printf("System Status  (Uid = 0x%lx, channel = %d)\n",
                               Uid, p1);
                        break;
                     case 7:
                        Cs2DecSysStatus7(CanMsg->Data, &Uid, &p1, &p2);
                        printf("System Status  (Uid = 0x%lx, channel = %d, %s)\n",
                               Uid, p1, p2?"true":"false");
                        break;
                     case 8:
                        Cs2DecSysStatus8(CanMsg->Data, &Uid, &p1, &p2);
                        printf("System Status  (Uid = 0x%lx, channel = %d, value = %ds)\n",
                               Uid, p1, p2);
                        break;
                     default:
                        puts("System Status (unknown parameter)");
                        break;
                  }
                  break;
               case CS2_SUBCMD_SYSTEM_SYSTEM_ID:
                  switch (MrCs2GetDlc(CanMsg))
                  {
                     case 5:
                        Cs2DecSysSystemId5(CanMsg->Data, &Uid);
                        printf("System Kennung (Uid = 0x%lx)\n", Uid);
                        break;
                     case 7:
                        Cs2DecSysSystemId7(CanMsg->Data, &Uid, &p1);
                        printf("System Kennung (Uid = 0x%lx, id = %d)\n",
                               Uid, p1);
                        break;
                     default:
                        puts("System Kennung (unknown parameter)");
                        break;
                  }
                  break;
               case CS2_SUBCMD_SYSTEM_MFX_SEEK:
                  puts("Mfx Seek, parameter not defined");
                  break;
               case CS2_SUBCMD_SYSTEM_SYSTEM_RESET:
                  Cs2DecSysSystemReset(CanMsg->Data, &Uid, &p1);
                  printf("System Reset (Uid = 0x%lx, target = %d)\n",
                         Uid, p1);
                  break;
               default:
                  puts("system subcommand not defined");
                  break;
            }
            break;
         case CS2_CMD_DISCOVERY:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecDiscovery0(CanMsg->Data);
                  puts("Lok Discovery");
                  break;
               case 1:
                  Cs2DecDiscovery1(CanMsg->Data, &p1);
                  printf("Lok Discovery (Proto = %d)\n", p1);
                  break;
               case 5:
                  Cs2DecDiscovery5(CanMsg->Data, &Uid, &p1);
                  printf("Lok Discovery (Uid = 0x%lx, Range = %d)\n",
                         Uid, p1);
                  break;
               case 6:
                  Cs2DecDiscovery6(CanMsg->Data, &Uid, &p1, &p2);
                  printf("Lok Discovery (Uid = 0x%lx, Range = %d, Ask = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Lok Discovery (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_BIND:
            Cs2DecBind(CanMsg->Data, &Uid, &s1);
            printf("MFX Bind (Uid = 0x%lx, Sid = 0x%x)\n", Uid, s1);
            break;
         case CS2_CMD_MFX_VERIFY:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecMfxVerify6(CanMsg->Data, &Uid, &s1);
                  printf("MFX Verify (Uid = 0x%lx, Sid = 0x%x)\n", Uid, s1);
                  break;
               case 7:
                  Cs2DecMfxVerify7(CanMsg->Data, &Uid, &s1, &p2);
                  printf("MFX Verify (Uid = 0x%lx, Sid = 0x%x, Ask = %d)\n",
                         Uid, s1, p2);
                  break;
               default:
                  puts("MFX Verify (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_SPEED:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecSpeed4(CanMsg->Data, &LocId);
                  printf("Lok Geschwindigkeit (LocId = 0x%lx)\n", LocId);
                  break;
               case 6:
                  Cs2DecSpeed6(CanMsg->Data, &LocId, &p1);
                  printf("Lok Geschwindigkeit (LocId = 0x%lx, Speed = %d)\n",
                         LocId, p1);
                  break;
               default:
                  puts("Lok Geschwindigkeit (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_DIRECTION:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecDirection4(CanMsg->Data, &LocId);
                  printf("Lok Richtung (LocId = 0x%lx)\n", LocId);
                  break;
               case 5:
                  Cs2DecDirection5(CanMsg->Data, &LocId, &p1);
                  printf("Lok Richtung (LocId = 0x%lx, Direction = %d)\n",
                         LocId, p1);
                  break;
               default:
                  puts("Lok Richtung (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_FUNCTION:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecFunction5(CanMsg->Data, &LocId, &p1);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d)\n",
                         LocId, p1);
                  break;
               case 6:
                  Cs2DecFunction6(CanMsg->Data, &LocId, &p1, &p2);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d, Value = %d)\n",
                         LocId, p1, p2);
                  break;
               case 8:
                  Cs2DecFunction8(CanMsg->Data, &LocId, &p1, &p2, &p3);
                  printf("Lok Funktion (LocId = 0x%lx, Function = %d, Value = %d, FktVal = %d)\n",
                         LocId, p1, p2, p3);
                  break;
               default:
                  puts("Lok Funktion (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_READ_CONFIG:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecReadConfig6(CanMsg->Data, &LocId, &p1);
                  printf("Read Config (LocId = 0x%lx, CvIndex = %d)\n",
                         LocId, p1);
                  break;
               case 7:
                  Cs2DecReadConfig7(CanMsg->Data, &LocId, &p1, &p2);
                  printf("Read Config (LocId = 0x%lx, CvIndex = %d, Param = %d)\n",
                         LocId, p1, p2);
                  break;
               default:
                  puts("Read Config (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_WRITE_CONFIG:
            Cs2DecWriteConfig(CanMsg->Data, &LocId, &p1, &p2, &p3);
            printf("Write Config (LocId = 0x%lx, CvIndex = %d, Value = %d, CtrlRslt = %d)\n",
                   LocId, p1, p2, p3);
            break;
         case CS2_CMD_ACC_SWITCH:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecAccSwitch6(CanMsg->Data, &LocId, &p1, &p2);
                  printf("Zubehör Schalten (LocId = 0x%lx, Position = %d, Current = %d)\n",
                         LocId, p1, p2);
                  break;
               case 8:
                  Cs2DecAccSwitch8(CanMsg->Data, &LocId, &p1, &p2, &p3);
                  printf("Zubehör Schalten (LocId = 0x%lx, Position = %d, Current = %d, TimeFkt = %d)\n",
                         LocId, p1, p2, p3);
                  break;
               default:
                  puts("Zubehör Schalten (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_ACC_CONF:
            puts("Zubehör Konfig (unknown parameter)");
            break;
         case CS2_CMD_S88_POLL:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecS88Poll5(CanMsg->Data, &Uid, &p1);
                  printf("S88 Polling (Feedback) (Uid = 0x%lx, NumModuls = %d)\n",
                         Uid, p1);
                  break;
               case 7:
                  Cs2DecS88Poll7(CanMsg->Data, &Uid, &p1, &p2);
                  printf("S88 Polling (Feedback) (Uid = 0x%lx, Modul = %d, State = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("S88 Polling (Feedback) (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_S88_EVENT:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 4:
                  Cs2DecS88Event4(CanMsg->Data, &p1, &p2);
                  printf("S88 Event (Device = %d, Contact = %d)\n", p1, p2);
                  break;
               case 5:
                  Cs2DecS88Event5(CanMsg->Data, &p1, &p2, &p3);
                  printf("S88 Event (Device = %d, Contact = %d, Param = %d)\n",
                         p1, p2, p3);
                  break;
               case 8:
                  Cs2DecS88Event8(CanMsg->Data, &p1, &p2, &p3, &p4, &p5);
                  printf("S88 Event (Device = %d, Contact = %d, StateOld = %d, StateNew = %d, Time = %d)\n",
                         p1, p2, p3, p4, p5);
                  break;
               default:
                  puts("S88 Event (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_SX1_EVENT:
            puts("SX1 Event (unknown parameter)");
            break;
         case CS2_CMD_PING:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecPing0(CanMsg->Data);
                  puts("Softwarestand Anfrage/Teilnehmer Ping");
                  break;
               case 8:
                  Cs2DecPing8(CanMsg->Data, &Uid, &p1, &p2);
                  printf("Softwarestand Anfrage/Teilnehmer Ping (Uid = 0x%lx, SwVersion = %d, DeviceId = 0x%x)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Softwarestand Anfrage/Teilnehmer Ping (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_OFFER_UPDATE:
            puts("Updateangebot (unknown parameter)");
            break;
         case CS2_CMD_READ_CFG_DATA:
            puts("Read Config Data (unknown parameter)");
            break;
         case CS2_CMD_BOOTLDR_CAN:
            puts("Bootloader CAN gebunden, 'Service' (unknown parameter)");
            break;
         case CS2_CMD_BOOTLDR_TRACK:
            puts("Bootloader Schienen gebunden, 'Service' (unknown parameter)");
            break;
         case CS2_CMD_STATUS:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 5:
                  Cs2DecStatus5(CanMsg->Data, &Uid, &p1);
                  printf("Statusdaten Konfiguration (Uid = 0x%lx, Index = %d)\n",
                         Uid, p1);
                  break;
               case 6:
                  Cs2DecStatus6(CanMsg->Data, &Uid, &p1, &p2);
                  printf("Statusdaten Konfiguration (Uid = 0x%lx, Index = %d, NumPackets = %d)\n",
                         Uid, p1, p2);
                  break;
               default:
                  puts("Statusdaten Konfiguration (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_CONFIG_QUERY:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 0:
                  Cs2DecConfigQuery0(CanMsg->Data);
                  puts("Anfordern Config Data, 'Data Query'");
                  break;
               case 8:
                  Cs2DecConfigQuery(CanMsg->Data, Buf);
                  printf("Anfordern Config Data, 'Data Query' (FileName = %s)\n",
                         Buf);
                  break;
            }
            break;
         case CS2_CMD_CFGDAT_STREAM:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecCfgdatStream6(CanMsg->Data, &Length, &p1);
                  printf("Config Data Stream (Length = %ld, Crc = %d)\n",
                         Length, p1);
                  break;
               case 7:
                  Cs2DecCfgdatStream7(CanMsg->Data, &Length, &p1);
                  printf("Config Zlib Data Stream (Length = %ld, Crc = %d)\n",
                         Length, p1);
                  break;
               case 8:
                  Cs2DecCfgdatStream8(CanMsg->Data, Buf);
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
         case CS2_CMD_CONNECT_6021:
            puts("60128 (Connect 6021) Data Stream");
            puts("alte Bezeichnung '6021 adapter'");
            puts("(unknown parameter)");
            break;
         case CS2_CMD_AUTOMATIC:
            switch (MrCs2GetDlc(CanMsg))
            {
               case 6:
                  Cs2DecAutomatic6(CanMsg->Data, &p1, &p2, &p3, &p4);
                  printf("automatic (DeviceId = %d, Func = %d, Status = %d, Param = %d)\n",
                         p1, p2, p3, p4);
                  break;
               case 8:
                  Cs2DecAutomatic8(CanMsg->Data, &p1, &p2, &LocId);
                  printf("automatic (DeviceId = %d, Func = %d, LocId = 0x%lx)\n",
                         p1, p2, LocId);
                  break;
               default:
                  puts("automatic (unknown parameter)");
                  break;
            }
            break;
         case CS2_CMD_DEBUG_MESSAGE:
            Cs2DecDebugMessage(CanMsg->Data, Buf);
            printf("Debug Message >%s<\n", Buf);
            break;
         default:
            puts("CAN command not defined");
            break;
      }
      puts("");
   }
}
