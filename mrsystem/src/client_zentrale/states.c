#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <boolean.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include <fsm.h>
#include <mr_ipc.h>
#include "zentrale.h"
#include "lok.h"


#define PARAGRAPH_LOK     1
#define PARAGRAPH_NUMLOKS 2

#define NUM_STATES 7
#define STATE_WAIT_FOR_MS2          0
#define STATE_WAIT_LOKNAME_CFG_HDR  1
#define STATE_WAIT_LOKNAME_CFG_DATA 2
#define STATE_WAIT_LOKINFO_CFG_HDR  3
#define STATE_WAIT_LOKINFO_CFG_DATA 4
#define STATE_WAIT_CFG_DATA         5
#define STATE_NORMAL                6
#define NUM_SIGNALS 15


static int HandleMemberWaitMs2(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned Version, Type;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: CAN member %lu, version %d, type %d\n", Uid, Version, Type);
   }
   if (Type != MR_CS2_DEVID_WIRED)
/*   if (Type >= 0x30)*/
   {
      ZentraleSetActualIndex(Data, 0);
      MrIpcInit(&Cmd);
      MrIpcSetCanResponse(&Cmd, 0);
      MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
      MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CONFIG_QUERY);
      MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
      MrIpcCmdSetReqestLocname(&Cmd, ZentraleGetActualIndex(Data),
                               ZentraleGetActualIndex(Data) + 1);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_HDR);
      }
      return(STATE_WAIT_LOKNAME_CFG_HDR);
   }
/*   else
      return(STATE_NO_CHANGE);*/
   return(STATE_NO_CHANGE);
}

static void QueryMembers(ZentraleStruct *Data)
{  MrIpcCmdType Cmd;

   MrIpcInit(&Cmd);
   MrIpcSetCanResponse(&Cmd, 0);
   MrIpcCalcHash(&Cmd, MR_CS2_UID_BROADCAST);
   MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_PING);
   MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
   MrIpcCmdSetRequestMember(&Cmd);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
}

static int HandleWaitMs2Timer(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   /* MrIpcCmdType *CmdFrame; */

   Data = (ZentraleStruct *)Priv;
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      puts("FSM: periodic task");
   }
   /* CmdFrame = (MrIpcCmdType *)SignalData; */
   QueryMembers(Data);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleLoknameCfgHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Length;
   unsigned Crc;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOKNAME CFG data length %lu, crc %d\n", Length, Crc);
   }
   ZentraleSetCfgLength(Data, Length);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, malloc(ZentraleGetCfgLength(Data)+7));
   if (ZentraleGetCfgBuffer(Data) == NULL)
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(STATE_NORMAL);
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_DATA);
      }
      return(STATE_WAIT_LOKNAME_CFG_DATA);
   }
}

static int HandleLoknameCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf [8];
   Cs2parser *LokParser;
   int LineInfo, Paragraph;
   MrIpcCmdType Cmd;

   Paragraph = 0;
   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   memcpy(ZentraleGetCfgBuffer(Data) + ZentraleGetCfgHaveRead(Data), Buf, 8);
   ZentraleSetCfgHaveRead(Data, ZentraleGetCfgHaveRead(Data) + 8);
   if (ZentraleGetCfgHaveRead(Data) >= ZentraleGetCfgLength(Data))
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: evaluate lokname cfg\n%s", ZentraleGetCfgBuffer(Data));
      }
      ZentraleGetCfgBuffer(Data)[ZentraleGetCfgLength(Data)] = '\0';
      LokParser = Cs2pCreate();
      Cs2pInit(LokParser, PARSER_TYPE_LOKNAMEN, ZentraleGetCfgBuffer(Data),
               ZentraleGetCfgHaveRead(Data));
      Cs2pSetVerbose(LokParser, FALSE);
      do {
         LineInfo = Cs2pParse(LokParser);
         switch (LineInfo)
         {
            case PARSER_ERROR:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  puts("ERROR in lok cfg");
               }
               break;
            case PARSER_EOF:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  puts("end of lok cfg");
               }
               break;
            case PARSER_PARAGRAPH:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  printf("new paragraph %s in lok cfg\n",
                         Cs2pGetName(LokParser));
               }
               switch (Cs2pGetSubType(LokParser))
               {
                  case PARSER_PARAGRAPH_LOK:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok paragraph in lok cfg");
                     }
                     Paragraph = PARAGRAPH_LOK;
                     break;
                  case PARSER_PARAGRAPH_NUMLOKS:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("numloks paragraph in lok cfg");
                     }
                     Paragraph = PARAGRAPH_NUMLOKS;
                     break;
               }
               break;
            case PARSER_VALUE:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  printf("new value %s=%s in lok cfg\n",
                         Cs2pGetName(LokParser), Cs2pGetValue(LokParser));
               }
               switch (Cs2pGetSubType(LokParser))
               {
                  case PARSER_VALUE_NAME:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        printf("lok name %d in lok cfg\n", ZentraleGetActualIndex(Data));
                     }
                     if (Paragraph == PARAGRAPH_LOK &&
                         ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
                     {
                        ZentraleSetLokNamenNr(Data,
                                              ZentraleGetActualIndex(Data),
                                              Cs2pGetValue(LokParser));
                        ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
                     }
                     break;
                  case PARSER_VALUE_WERT:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("number of loks in lok cfg");
                     }
                     if (Paragraph == PARAGRAPH_NUMLOKS)
                     {
                        ZentraleSetNumLoks(Data, atoi(Cs2pGetValue(LokParser)));
                        if (ZentraleGetVerbose(Data)) {
                           time_stamp();
                           printf("number of loks in lok cfg is %d\n",
                                  ZentraleGetNumLoks(Data));
                        }
                        if (ZentraleGetMaxLoks(Data) < ZentraleGetNumLoks(Data))
                        {
                           ZentraleSetMaxLoks(Data, ZentraleGetNumLoks(Data));
                           ZentraleSetLokNamen(Data, realloc(ZentraleGetLokNamen(Data),
                                                             ZentraleGetNumLoks(Data) * sizeof(ZentraleLokName)));
                           if (ZentraleGetVerbose(Data)) {
                              time_stamp();
                              printf("new number of loks in lok cfg is %d\n",
                                     ZentraleGetNumLoks(Data));
                           }
                        }
                     }
                     break;
               }
               break;
         }
      } while (LineInfo != PARSER_EOF);
      Cs2pDestroy(LokParser);
      free(ZentraleGetCfgBuffer(Data));
      ZentraleSetCfgLength(Data, 0);
      ZentraleSetCfgHaveRead(Data, 0);
      ZentraleSetCfgBuffer(Data, NULL);
      if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
      {
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("request lokname %d\n", ZentraleGetActualIndex(Data));
         }
         MrIpcInit(&Cmd);
         MrIpcSetCanResponse(&Cmd, 0);
         MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
         MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CONFIG_QUERY);
         MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
         MrIpcCmdSetReqestLocname(&Cmd, ZentraleGetActualIndex(Data),
                                  ZentraleGetActualIndex(Data) + 1);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_HDR);
         }
         return(STATE_WAIT_LOKNAME_CFG_HDR);
      }
      else
      {
         ZentraleSetActualIndex(Data, 0);
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("request lokinfo %d >%s< from %d\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleGetLokNamenNr(Data, ZentraleGetActualIndex(Data)),
                   ZentraleGetNumLoks(Data));
         }
         MrIpcInit(&Cmd);
         MrIpcSetCanResponse(&Cmd, 0);
         MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
         MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CONFIG_QUERY);
         MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
         MrIpcCmdSetReqestLocinfo(&Cmd,
                                  ZentraleGetLokNamenNr(Data,
                                                        ZentraleGetActualIndex(Data)));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         LokMarkAllDeleted(ZentraleGetLoks(Data));
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("FSM: new state %d\n",STATE_WAIT_LOKINFO_CFG_HDR);
         }
         return(STATE_WAIT_LOKINFO_CFG_HDR);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n", STATE_WAIT_LOKNAME_CFG_DATA);
      }
      return(STATE_WAIT_LOKNAME_CFG_DATA);
   }
}

static int HandleLokinfoCfgHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Length;
   unsigned Crc;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOKINFO CFG data length %lu, crc %d\n", Length, Crc);
   }
   ZentraleSetCfgLength(Data, Length);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, malloc(ZentraleGetCfgLength(Data)+7));
   if (ZentraleGetCfgBuffer(Data) == NULL)
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(STATE_NORMAL);
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_LOKINFO_CFG_DATA);
      }
      return(STATE_WAIT_LOKINFO_CFG_DATA);
   }
}

static int HandleLokinfoCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[17];
   Cs2parser *LokParser;
   int LineInfo, FktIndex;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   memcpy(ZentraleGetCfgBuffer(Data) + ZentraleGetCfgHaveRead(Data), Buf, 8);
   ZentraleSetCfgHaveRead(Data, ZentraleGetCfgHaveRead(Data) + 8);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOKINFO CFG data, aktuell %lu von %lu\n",
             ZentraleGetCfgHaveRead(Data), ZentraleGetCfgLength(Data));
   }
   if (ZentraleGetCfgHaveRead(Data) >= ZentraleGetCfgLength(Data))
   {
      memset(ZentraleGetActualLok(Data), 0, sizeof(LokInfo));
      FktIndex = -1;
      ZentraleGetCfgBuffer(Data)[ZentraleGetCfgLength(Data)] = '\0';
      LokParser = Cs2pCreate();
      Cs2pInit(LokParser, PARSER_TYPE_LOKINFO, ZentraleGetCfgBuffer(Data),
               ZentraleGetCfgHaveRead(Data));
      Cs2pSetVerbose(LokParser, FALSE);
      do {
         LineInfo = Cs2pParse(LokParser);
         switch (LineInfo)
         {
            case PARSER_ERROR:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  puts("ERROR in lok info cfg");
               }
               break;
            case PARSER_EOF:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  puts("end of lok info cfg");
               }
               break;
            case PARSER_PARAGRAPH:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  printf("new paragraph %s in lok info cfg\n",
                         Cs2pGetName(LokParser));
               }
               switch (Cs2pGetSubType(LokParser))
               {
                  case PARSER_PARAGRAPH_LOKOMOTIVE:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lokomotive paragraph in lok cfg");
                     }
                     break;
               }
               break;
            case PARSER_VALUE:
               if (ZentraleGetVerbose(Data)) {
                  time_stamp();
                  printf("new value %s=%s in lok cfg\n",
                         Cs2pGetName(LokParser), Cs2pGetValue(LokParser));
               }
               switch (Cs2pGetSubType(LokParser))
               {
                  case PARSER_VALUE_LOK:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("neuer lok Eintrag");
                     }
                     break;
                  case PARSER_VALUE_UID:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok uid");
                     }
                     LokInfoSetUid(ZentraleGetActualLok(Data),
                                   strtoul(Cs2pGetValue(LokParser),
                                           NULL, 0));
                     break;
                  case PARSER_VALUE_NAME:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok name");
                     }
                     LokInfoSetName(ZentraleGetActualLok(Data),
                                    Cs2pGetValue(LokParser));
                     break;
                  case PARSER_VALUE_ADRESSE:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok adresse");
                     }
                     LokInfoSetAdresse(ZentraleGetActualLok(Data),
                                       strtoul(Cs2pGetValue(LokParser),
                                               NULL, 0));
                     break;
                  case PARSER_VALUE_TYP:
                     if (Cs2pGetLevel(LokParser) == 1)
                     {
                        if (ZentraleGetVerbose(Data)) {
                           time_stamp();
                           puts("lok typ");
                        }
                        LokInfoSetTyp(ZentraleGetActualLok(Data),
                                      Cs2pGetValue(LokParser));
                     }
                     else if (Cs2pGetLevel(LokParser) == 2)
                     {
                        if (ZentraleGetVerbose(Data)) {
                           time_stamp();
                           puts("lok function typ");
                        }
                        LokInfoSetFktTyp(ZentraleGetActualLok(Data), FktIndex,
                                         strtoul(Cs2pGetValue(LokParser),
                                                 NULL, 0));
                     }
                     break;
                  case PARSER_VALUE_MFXUID:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok mfxuid");
                     }
                     LokInfoSetMfxUid(ZentraleGetActualLok(Data),
                                      strtoul(Cs2pGetValue(LokParser),
                                              NULL, 0));
                     break;
                  case PARSER_VALUE_SYMBOL:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok symbol");
                     }
                     LokInfoSetSymbol(ZentraleGetActualLok(Data),
                                      strtoul(Cs2pGetValue(LokParser),
                                              NULL, 0));
                     break;
                  case PARSER_VALUE_AV:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok av");
                     }
                     LokInfoSetAv(ZentraleGetActualLok(Data),
                                  strtoul(Cs2pGetValue(LokParser),
                                          NULL, 0));
                     break;
                  case PARSER_VALUE_BV:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok bv");
                     }
                     LokInfoSetBv(ZentraleGetActualLok(Data),
                                  strtoul(Cs2pGetValue(LokParser),
                                          NULL, 0));
                     break;
                  case PARSER_VALUE_VOLUME:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok volume");
                     }
                     LokInfoSetVolume(ZentraleGetActualLok(Data),
                                      strtoul(Cs2pGetValue(LokParser),
                                              NULL, 0));
                     break;
                  case PARSER_VALUE_VELOCITY:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok velocity");
                     }
                     LokInfoSetVelocity(ZentraleGetActualLok(Data),
                                        strtoul(Cs2pGetValue(LokParser),
                                                NULL, 0));
                     break;
                  case PARSER_VALUE_RICHTUNG:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok richtung");
                     }
                     LokInfoSetRichtung(ZentraleGetActualLok(Data),
                                        strtoul(Cs2pGetValue(LokParser),
                                                NULL, 0));
                     break;
                  case PARSER_VALUE_VMAX:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok vmax");
                     }
                     LokInfoSetVmax(ZentraleGetActualLok(Data),
                                    strtoul(Cs2pGetValue(LokParser),
                                            NULL, 0));
                     break;
                  case PARSER_VALUE_VMIN:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok vmin");
                     }
                     LokInfoSetVmin(ZentraleGetActualLok(Data),
                                    strtoul(Cs2pGetValue(LokParser),
                                            NULL, 0));
                     break;
                  case PARSER_VALUE_FKT:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok fkt");
                     }
                     FktIndex++;
                     break;
                  case PARSER_VALUE_DAUER:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok function dauer");
                     }
                     LokInfoSetFktDauer(ZentraleGetActualLok(Data), FktIndex,
                                        strtoul(Cs2pGetValue(LokParser),
                                                NULL, 0));
                     break;
                  case PARSER_VALUE_WERT:
                     if (ZentraleGetVerbose(Data)) {
                        time_stamp();
                        puts("lok function wert");
                     }
                     LokInfoSetFktWert(ZentraleGetActualLok(Data), FktIndex,
                                       strtoul(Cs2pGetValue(LokParser),
                                               NULL, 0));
                     break;
               }
               break;
         }
      } while (LineInfo != PARSER_EOF);
      Cs2pDestroy(LokParser);
      free(ZentraleGetCfgBuffer(Data));
      ZentraleSetCfgLength(Data, 0);
      ZentraleSetCfgHaveRead(Data, 0);
      ZentraleSetCfgBuffer(Data, NULL);
      LokInsert(ZentraleGetLoks(Data), ZentraleGetActualLok(Data));
      ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
      if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
      {
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("request lokinfo %d >%s< from %d\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleGetLokNamenNr(Data,
                                         ZentraleGetActualIndex(Data)),
                   ZentraleGetNumLoks(Data));
         }
         MrIpcInit(&Cmd);
         MrIpcSetCanResponse(&Cmd, 0);
         MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
         MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CONFIG_QUERY);
         MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
         MrIpcCmdSetReqestLocinfo(&Cmd,
                                  ZentraleGetLokNamenNr(Data,
                                                        ZentraleGetActualIndex(Data)));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_HDR);
         }
         return(STATE_WAIT_LOKINFO_CFG_HDR);
      }
      else
      {
         if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("save lokomotive.cs2\n");
         }
         LokSaveLokomotiveCs2(ZentraleGetLoks(Data));
         if (ZentraleGetVerbose(Data)) {
             time_stamp();
             printf("FSM: new state %d\n",STATE_NORMAL);
         }
         return(STATE_NORMAL);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_DATA);
      }
      return(STATE_WAIT_LOKINFO_CFG_DATA);
   }
}

static int HandleLokSpeed(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Speed;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Addr, &Speed);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOK Speed addr %lu, speed %d\n", Addr, Speed);
   }
   LokData = LokSearch(ZentraleGetLoks(Data), Addr);
   if (LokData != (LokInfo *)NULL)
      LokInfoSetVelocity(LokData, Speed);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleLokDirection(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   DirectionType Direction;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveDir(CmdFrame, &Addr, &Direction);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOK Direction addr %lu, speed %d\n", Addr, (int)Direction);
   }
   LokData = LokSearch(ZentraleGetLoks(Data), Addr);
   if (LokData != (LokInfo *)NULL)
      LokInfoSetRichtung(LokData, (int)Direction);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleLokFunction(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Function;
   SwitchType Switch;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveFkt(CmdFrame, &Addr, &Function, &Switch);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: LOK Function addr %lu, function %d, value %d\n",
             Addr, Function, (int)Switch);
   }
   LokData = LokSearch(ZentraleGetLoks(Data), Addr);
   if (LokData != (LokInfo *)NULL)
      LokInfoSetFktWert(LokData, Function, (int)Switch);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleFileRequest(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame, Cmd;
   char Name[9], *Dateiname, FullPath[255], *LokomotiveDaten;
   int i, j, FileLength;
   unsigned Hash;
   struct stat attribut;
   FILE *LokomotiveDatei;

   Dateiname = (char *)NULL;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetQuery(CmdFrame, Name);
   Name[8] = '\0';
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: request %s\n", Name);
   }
   if (strncmp(MR_CS2_CFG_LOCINFO, Name, strlen(MR_CS2_CFG_LOCINFO)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LOCNAMES, Name, strlen(MR_CS2_CFG_LOCNAMES)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_MAGINFO, Name, strlen(MR_CS2_CFG_MAGINFO)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LOCDB, Name, strlen(MR_CS2_CFG_LOCDB)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LANGVER, Name, strlen(MR_CS2_CFG_LANGVER)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LANG, Name, strlen(MR_CS2_CFG_LANG)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LDBVER, Name, strlen(MR_CS2_CFG_LDBVER)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_LOKS, Name, strlen(MR_CS2_CFG_LOKS)) == 0)
      Dateiname = CS2_FILE_STRING_LOKOMOTIVE;
   else if (strncmp(MR_CS2_CFG_MAGS, Name, strlen(MR_CS2_CFG_MAGS)) == 0)
      Dateiname = CS2_FILE_STRING_MAGNETARTIKEL;
   else if (strncmp(MR_CS2_CFG_GBS_PAGE, Name, strlen(MR_CS2_CFG_GBS_PAGE)) == 0)
      Dateiname = GleisbildPageGetName(ZentraleGetNrGleisPages(Data,
                                                               atoi(Name + strlen(MR_CS2_CFG_GBS_PAGE))));
   else if (strncmp(MR_CS2_CFG_GBS_STAT, Name, strlen(MR_CS2_CFG_GBS_STAT)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_GBS, Name, strlen(MR_CS2_CFG_GBS)) == 0)
      Dateiname = CS2_FILE_STRING_GLEISBILD;
   else if (strncmp(MR_CS2_CFG_FS, Name, strlen(MR_CS2_CFG_FS)) == 0)
      Dateiname = CS2_FILE_STRING_FAHRSTRASSE;
   else if (strncmp(MR_CS2_CFG_LOK_STAT, Name, strlen(MR_CS2_CFG_LOK_STAT)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_MAG_STAT, Name, strlen(MR_CS2_CFG_MAG_STAT)) == 0)
      Dateiname = (char *)NULL;
   else if (strncmp(MR_CS2_CFG_FS_STAT, Name, strlen(MR_CS2_CFG_FS_STAT)) == 0)
      Dateiname = (char *)NULL;
   if (Dateiname != (char *)NULL)
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: request file %s\n", Dateiname);
      }
      strcpy(FullPath, ZentraleGetLocPath(Data));
      if (FullPath[strlen(FullPath)] != '/')
         strcat(FullPath, "/");
      strcat(FullPath, Dateiname);
      LokomotiveDatei = fopen(FullPath, "r");
      if (LokomotiveDatei != (FILE *)NULL)
      {
         stat(FullPath, &attribut);
         LokomotiveDaten = (char *)malloc(attribut.st_size);
         if (LokomotiveDaten != (char *)NULL)
         {
            FileLength = fread(LokomotiveDaten, 1, attribut.st_size,
                               LokomotiveDatei);
            ZFileInit(ZentraleGetPackedCs2File(Data), LokomotiveDaten,
                      FileLength);
            if (ZFileCompress(ZentraleGetPackedCs2File(Data)))
            {
               MrIpcSetCanResponse(CmdFrame, 1);
               Hash = MrCs2CalcHash(ZentraleGetUid(Data));
               MrIpcSetCanHash(CmdFrame, MrCs2CalcHash(ZentraleGetUid(Data)));
               MrIpcSend(ZentraleGetClientSock(Data), CmdFrame);
               MrIpcSetCanHash(CmdFrame, Hash);
               MrIpcInit(&Cmd);
               MrIpcSetCanResponse(&Cmd, 0);
               MrIpcSetCanHash(&Cmd, Hash);
               MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CFGDAT_STREAM);
               MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_0);
               MrIpcCmdSetCfgHeader(&Cmd,
                   ZFileGetLength(ZentraleGetPackedCs2File(Data)),
                   ZFileGetCrc(ZentraleGetPackedCs2File(Data)));
               MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
               i = 0;
               while (i < ZFileGetFrameLength(ZentraleGetPackedCs2File(Data)))
               {
                  MrIpcInit(&Cmd);
                  MrIpcSetCanResponse(&Cmd, 0);
                  MrIpcSetCanHash(&Cmd, Hash);
                  MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_CFGDAT_STREAM);
                  MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_0);
                  j = 0;
                  while (j < 8)
                  {
                     Name[j] = ZFileGetBuffer(ZentraleGetPackedCs2File(Data))[i + j];
                     j++;
                  }
                  MrIpcCmdSetCfgData(&Cmd, Name);
                  MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
                  i += 8;
               }
            }
            else if (ZentraleGetVerbose(Data)) {
               time_stamp();
               printf("FSM: error in compress file %s\n", Dateiname);
            }
            ZFileExit(ZentraleGetPackedCs2File(Data));
            free(LokomotiveDaten);
         }
         else if (ZentraleGetVerbose(Data)) {
            time_stamp();
            printf("FSM: error in allocate file buffer for %s\n", Dateiname);
         }
         fclose(LokomotiveDatei);
      }
      else if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: error in open file %s\n", FullPath);
      }
   }
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleCfgHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Length;
   unsigned Crc;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: CFG data length %lu, crc %d\n", Length, Crc);
   }
   ZentraleSetCfgLength(Data, Length);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, malloc(ZentraleGetCfgLength(Data) + 6));
   if (ZentraleGetCfgBuffer(Data) == NULL)
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(STATE_NORMAL);
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_CFG_DATA);
      }
      return(STATE_WAIT_CFG_DATA);
   }
}

static int HandleCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf [8];
   Cs2parser *Parser;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   memcpy(ZentraleGetCfgBuffer(Data) + ZentraleGetCfgHaveRead(Data), Buf, 8);
   ZentraleSetCfgHaveRead(Data, ZentraleGetCfgHaveRead(Data) + 8);
   if (ZentraleGetCfgHaveRead(Data) >= ZentraleGetCfgLength(Data))
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: inflate compressed cfg\n");
      }
      ZFileInit(ZentraleGetPackedCs2File(Data), ZentraleGetCfgBuffer(Data),
                ZentraleGetCfgHaveRead(Data));
      if (ZFileUnCompress(ZentraleGetPackedCs2File(Data)))
      {
         Parser = Cs2pCreate();
         Cs2pInit(Parser, PARSER_TYPE_HEADER_CS2,
                  ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                  ZFileGetLength(ZentraleGetPackedCs2File(Data)));
         Cs2pSetVerbose(Parser, FALSE);
         if (Cs2pParse(Parser) == PARSER_PARAGRAPH)
         {
            switch (Cs2pGetSubType(Parser))
            {
               case PARSER_PARAGRAPH_LOKOMOTIVE:
                  break;
               case PARSER_PARAGRAPH_GLEISBILD:
                  {  DIR *d;
                     struct dirent *dir;
                     char GleisbildPageDir[256], GleisbildPageFullName[256];

                     GleisbildClear(ZentraleGetGleisbild(Data));
                     strcpy(GleisbildPageDir, ZentraleGetLocPath(Data));
                     if (GleisbildPageDir[strlen(GleisbildPageDir) - 1] != '/')
                        strcat(GleisbildPageDir, "/");
                     strcat(GleisbildPageDir, MR_CS2_GLEISBILD_PAGE_SUBDIR);
                     d = opendir(GleisbildPageDir);
                     if (d)
                     {
                        while ((dir = readdir(d)) != NULL)
                        {
                           if ((strcmp(dir->d_name, ".") != 0) &&
                               (strcmp(dir->d_name, "..") != 0))
                           {
                              strcpy(GleisbildPageFullName, GleisbildPageDir);
                              if (GleisbildPageFullName[strlen(GleisbildPageFullName) - 1] != '/')
                                 strcat(GleisbildPageFullName, "/");
                              strcat(GleisbildPageFullName, dir->d_name);
                              remove(GleisbildPageFullName);
                           }
                        }
                        closedir(d);
                     }
                     if (GleisbildGetNumPages(ZentraleGetGleisbild(Data)) > 0)
                     {  int i;

                        for (i=0; i<GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
                        {
                           GleisbildPageExit(ZentraleGetNrGleisPages(Data, i));
                           GleisbildPageDestroy(ZentraleGetNrGleisPages(Data, i));
                        }
                        free(ZentraleGetGleisPages(Data));
                     }
                     GleisbildParseGleisbildCs2(ZentraleGetGleisbild(Data),
                                                ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                     GleisbildSaveGleisbildCs2(ZentraleGetGleisbild(Data));
                     if (GleisbildGetNumPages(ZentraleGetGleisbild(Data)) > 0)
                     {  int i;

                        ZentraleSetGleisPages(Data,
                                              (GleisbildPageStruct **)malloc(sizeof(GleisbildPageStruct *) *
                                                                             GleisbildGetNumPages(ZentraleGetGleisbild(Data))));
                        for (i=0; i<GleisbildGetNumPages(ZentraleGetGleisbild(Data)); i++)
                        {
                           ZentraleSetNrGleisPages(Data, i,
                                                   (GleisbildPageStruct *)NULL);
                        }
                     }
                  }
                  break;
               case PARSER_PARAGRAPH_MAGNETARTIKEL:
                  MagnetartikelParseMagnetartikelCs2(ZentraleGetMagnetartikel(Data),
                                                     ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                     ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  MagnetartikelSaveMagnetartikelCs2(ZentraleGetMagnetartikel(Data));
                  break;
               case PARSER_PARAGRAPH_FAHRSTRASSEN:
                  FahrstrasseParseFahrstrasseCs2(ZentraleGetFahrstrasse(Data),
                                                 ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                 ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                  FahrstrasseSaveFahrstrasseCs2(ZentraleGetFahrstrasse(Data));
                  break;
               case PARSER_PARAGRAPH_GLEISBILDSEITE:
                  {
                     GleisbildPageStruct *NewPage;

                     NewPage = GleisbildPageCreate();
                     if (NewPage != (GleisbildPageStruct *)NULL)
                     {
                        GleisbildPageInit(NewPage, ZentraleGetLocPath(Data), "", 0);
                        GleisbildPageParseGleisbildPageCs2(NewPage,
                                                           ZFileGetBuffer(ZentraleGetPackedCs2File(Data)),
                                                           ZFileGetLength(ZentraleGetPackedCs2File(Data)));
                        GleisbildPageSetGleisbildPageFilePath(NewPage,
                                                              ZentraleGetLocPath(Data));
                        GleisbildPageSetGleisbildName(NewPage,
                                                      ZentraleGetLokNamenNr(Data,
                                                                            GleisbildPageStructGetPage(NewPage)));
                        GleisbildPageSaveGleisbildPageCs2(NewPage);
                        ZentraleSetNrGleisPages(Data,
                                                GleisbildPageStructGetPage(NewPage),
                                                NewPage);
                     }
                  }
                  break;
            }
         }
         Cs2pDestroy(Parser);
      }
      ZFileExit(ZentraleGetPackedCs2File(Data));
      free(ZentraleGetCfgBuffer(Data));
      ZentraleSetCfgLength(Data, 0);
      ZentraleSetCfgHaveRead(Data, 0);
      ZentraleSetCfgBuffer(Data, NULL);
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(STATE_NORMAL);
   }
   else
   {
      if (ZentraleGetVerbose(Data)) {
         time_stamp();
         printf("FSM: new state %d\n",STATE_WAIT_CFG_DATA);
      }
      return(STATE_WAIT_CFG_DATA);
   }
}

static int HandlePing(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   /* MrIpcCmdType *CmdFrame, Cmd; */
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   /* CmdFrame = (MrIpcCmdType *)SignalData; */
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      puts("FSM: answer ping");
   }
   MrIpcInit(&Cmd);
   MrIpcSetCanResponse(&Cmd, 1);
   MrIpcCalcHash(&Cmd, ZentraleGetUid(Data));
   MrIpcSetCanCommand(&Cmd, MR_CS2_CMD_PING);
   MrIpcSetCanPrio(&Cmd, MR_CS2_PRIO_2);
   MrIpcCmdSetMember(&Cmd, ZentraleGetUid(Data), 0x100, MR_CS2_DEVID_CS2);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleMember(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned Version, Type;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: CAN member %lu, version %d, type %d\n", Uid, Version, Type);
   }
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static int HandleOther(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (ZentraleGetVerbose(Data)) {
      time_stamp();
      printf("FSM: unhandled %d\n", MrIpcGetCommand(CmdFrame));
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   }
   return(STATE_NO_CHANGE);
}

static StateFktType StateWaitForMs2[NUM_SIGNALS] = {
   /* STATE_WAIT_FOR_MS2 */
   HandleOther,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandlePing,          /* MrIpcCmdRequestMember */
   HandleMemberWaitMs2, /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdRequestFile */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleOther,         /* MrIpcCmdCfgZHeader */
   HandleOther          /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLoknameCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_HDR */
   HandleOther,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcCmdLocomotiveSpeed */
   HandleOther,            /* MrIpcCmdLocomotiveDirection */
   HandleOther,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMember,           /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleOther,            /* MrIpcCmdRequestFile */
   HandleLoknameCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther,            /* MrIpcCmdCfgZHeader */
   HandleOther             /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLoknameCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_DATA */
   HandleOther,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandlePing,          /* MrIpcCmdRequestMember */
   HandleMember,        /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdRequestFile */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleOther,         /* MrIpcCmdCfgZHeader */
   HandleLoknameCfgData /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLokinfoCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_HDR */
   HandleOther,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcCmdLocomotiveSpeed */
   HandleOther,            /* MrIpcCmdLocomotiveDirection */
   HandleOther,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMember,           /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleOther,            /* MrIpcCmdRequestFile */
   HandleLokinfoCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther,            /* MrIpcCmdCfgZHeader */
   HandleOther             /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLokinfoCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_DATA */
   HandleOther,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandlePing,          /* MrIpcCmdRequestMember */
   HandleMember,        /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdRequestFile */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleOther,         /* MrIpcCmdCfgZHeader */
   HandleLokinfoCfgData /* MrIpcCmdCfgData */
};
static StateFktType StateWaitCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA */
   HandleWaitMs2Timer, /* timer */
   HandleOther,        /* MrIpcCmdNull */
   HandleOther,        /* MrIpcCmdRun */
   HandleLokSpeed,     /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection, /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,  /* MrIpcCmdLocomotiveFunction */
   HandleOther,        /* MrIpcCmdAccSwitch */
   HandlePing,         /* MrIpcCmdRequestMember */
   HandleMember,       /* MrIpcCmdMember */
   HandleOther,        /* MrIpcCmdRequestLocName */
   HandleOther,        /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,  /* MrIpcCmdRequestFile */
   HandleOther,        /* MrIpcCmdCfgHeader */
   HandleOther,        /* MrIpcCmdCfgZHeader */
   HandleCfgData       /* MrIpcCmdCfgData */
};
static StateFktType StateNormal[NUM_SIGNALS] = {
   /* STATE_NORMAL */
   HandleWaitMs2Timer, /* timer */
   HandleOther,        /* MrIpcCmdNull */
   HandleOther,        /* MrIpcCmdRun */
   HandleLokSpeed,     /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection, /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,  /* MrIpcCmdLocomotiveFunction */
   HandleOther,        /* MrIpcCmdAccSwitch */
   HandlePing,         /* MrIpcCmdRequestMember */
   HandleMember,       /* MrIpcCmdMember */
   HandleOther,        /* MrIpcCmdRequestLocName */
   HandleOther,        /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,  /* MrIpcCmdRequestFile */
   HandleOther,        /* MrIpcCmdCfgHeader */
   HandleCfgHeader,    /* MrIpcCmdCfgZHeader */
   HandleOther         /* MrIpcCmdCfgData */
};
static SignalFunctionsType StateMachineFunctions[NUM_STATES] = {
   StateWaitForMs2,
   StateWaitLoknameCfgHdr,
   StateWaitLoknameCfgData,
   StateWaitLokinfoCfgHdr,
   StateWaitLokinfoCfgData,
   StateWaitCfgData,
   StateNormal
};

void ZentraleInitFsm(ZentraleStruct *Data, BOOL IsMaster)
{
   if (IsMaster)
   {
      FsmInit(ZentraleGetStateMachine(Data), (void *)Data, STATE_WAIT_FOR_MS2,
              NUM_SIGNALS, NUM_STATES, StateMachineFunctions);
   }
   else
   {
      FsmInit(ZentraleGetStateMachine(Data), (void *)Data, STATE_NORMAL,
              NUM_SIGNALS, NUM_STATES, StateMachineFunctions);
   }
}
