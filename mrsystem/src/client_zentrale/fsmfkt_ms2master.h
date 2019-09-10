static void DisableLongPolls(ZentraleStruct *Data)
{
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
}

static void ReenableLongPolls(ZentraleStruct *Data, char *FromLongPoll)
{
   if (strcmp(FromLongPoll, PERIODIC_NAME_QUERY) == 0)
      CronResume(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
   else
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
   if (strcmp(FromLongPoll, PERIODIC_NAME_POLLMS2) == 0)
   {
      if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_PERIODIC) != 0)
      {
         CronResume(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
      else
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
   }
   else
   {
      if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_PERIODIC) != 0)
      {
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
      else
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
   }
}

static int PeriodicQueryTimeoutWaitNormal(void *PrivData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)PrivData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: timeout, new state %d\n",STATE_NORMAL);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   ReenableLongPolls(Data, PERIODIC_NAME_POLLMS2);
   LokMarkAllUndeleted(ZentraleGetLoks(Data));
   CanMemberSetIsChanged(ZentraleGetCanMember(Data), TRUE);
   CanMemberClearConfig(ZentraleGetCanMember(Data));
   return(STATE_NORMAL);
}

static int PeriodicEndQueryMembersMs2Master(void *PrivData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)PrivData;
   ZentraleSetIsInPoll(Data, FALSE);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_END_QUERY);
   ReenableLongPolls(Data, PERIODIC_NAME_POLLMS2);
   return(STATE_NO_CHANGE);
}

static int HandleCfgDataMs2Master(void *Priv, void *SignalData)
{
   if (DoCfgData(Priv, SignalData))
   {  ZentraleStruct *Data;

      Data = (ZentraleStruct *)Priv;
      ReenableLongPolls(Data, PERIODIC_NAME_POLLMS2);
      return(STATE_NORMAL);
   }
   else
   {
      return(STATE_NO_CHANGE);
   }
}

static int HandleCfgDataLateHeaderMs2Master(void *Priv, void *SignalData)
{  int NewState;

   NewState = HandleCfgDataLateHeader(Priv, SignalData);
   if (NewState == STATE_NORMAL)
   {  ZentraleStruct *Data;

      Data = (ZentraleStruct *)Priv;
      ReenableLongPolls(Data, PERIODIC_NAME_POLLMS2);
   }
   return(NewState);
}

static int PollMs2(ZentraleStruct *Data, int Version)
{  MrIpcCmdType CmdFrame;

   LokMarkAllDeleted(ZentraleGetLoks(Data));
   if (Version > 0x0300)
   {
      /* Version > 3.xx, hole lokliste */
      MrIpcHlLoclistRequest(&CmdFrame, ZentraleGetUid(Data),
                            MR_IPC_SOCKET_ALL, MR_IPC_SOCKET_ALL);
      MrIpcSend(ZentraleGetClientSock(Data), &CmdFrame);
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",STATE_WAIT_LOKLISTE_CFG_HDR);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      return(STATE_WAIT_LOKLISTE_CFG_HDR);
   }
   else
   {
      /* Version < 3.xx, hole loknamen */
      ZentraleSetActualIndex(Data, 0);
      MrIpcHlLocnameRequest(&CmdFrame, ZentraleGetUid(Data),
                            MR_IPC_SOCKET_ALL, MR_IPC_SOCKET_ALL,
                            ZentraleGetActualIndex(Data));
      MrIpcSend(ZentraleGetClientSock(Data), &CmdFrame);
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_HDR);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      return(STATE_WAIT_LOKNAME_CFG_HDR);
   }
}

static int HandleMemberWaitMs2(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned int Version, Type;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data))
      printf("FSM: CAN member %lu, version %d, type 0x%x\n",
             Uid, Version, Type);
   if ((Type == CS2_DEVID_MS2_1) || (Type == CS2_DEVID_MS2_2))
   {
      return(PollMs2(Data, Version));
   }
   else
      return(STATE_NO_CHANGE);
}

static int HandleMemberMs2Master(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned int Version, Type;
   CanMemberInfo NewCanMember, *OldCanMember;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data))
      printf("FSM: CAN member %lu, version %d, type 0x%x\n", Uid, Version, Type);
   OldCanMember = CanMemberSearch(ZentraleGetCanMember(Data), Uid);
   if (OldCanMember == (CanMemberInfo *)NULL)
   {
      CanMemberInfoSetUid(&NewCanMember, Uid);
      CanMemberInfoSetVersion(&NewCanMember, Version);
      CanMemberInfoSetType(&NewCanMember, Type);
      CanMemberInfoSetConfigQuerried(&NewCanMember, FALSE);
      CanMemberInsert(ZentraleGetCanMember(Data), &NewCanMember);
      CanMemberSetIsChanged(ZentraleGetCanMember(Data), TRUE);
      PingAnswerToS88(Data, &NewCanMember);
   }
   else
   {
      if (CanMemberInfoGetIsInvalid(OldCanMember))
      {
         CanMemberInfoSetIsInvalid(OldCanMember, FALSE);
      }
   }
   if (Type == CS2_DEVID_CS2)
   {
      QueryMembers(Data, FALSE);
      DoPingMember(Data, CS2_DEVID_CS2);
      DoPingMember(Data, CS2_DEVID_CS2GUI);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int DoLokinfoWaitCfgData(ZentraleStruct *Data, void *SignalData,
                                int NewStateForNextStep,
                                int NewStateForContinue,
                                int NewStateForStay)
{  MrIpcCmdType Cmd, *CmdFrame;
   char Buf[17];

   if (ZentraleGetVerbose(Data))
      puts("DoLokinfoWaitCfgData");
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         MrIpcGetCanHash(CmdFrame), Buf))
   {
      if (ZentraleGetVerbose(Data))
         puts("HandleLokinfoCfgData: lokinfo complete");
      if (HandleCfgLokinfo(Data, CmdFrame))
      {
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
         if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_PERIODIC) != 0)
         {
            if (ZentraleGetVerbose(Data))
               puts("enable periodic poll of MS2");
            CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
         }
         else
         {
            if (ZentraleGetVerbose(Data))
               puts("disable periodic poll of MS2");
            CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
         }
         if (ZentraleGetVerbose(Data))
             printf("FSM: new state %d\n",STATE_NORMAL);
         return(NewStateForNextStep);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            printf("request lokinfo %d >%s< from %d\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleGetLokNamenNr(Data,
                                         ZentraleGetActualIndex(Data)),
                   ZentraleGetNumLoks(Data));
         MrIpcHlLocinfoRequest(&Cmd, ZentraleGetUid(Data),
                               MR_IPC_SOCKET_ALL,
                               MrIpcGetSenderSocket(CmdFrame),
                               ZentraleGetLokNamenNr(Data,
                                                     ZentraleGetActualIndex(Data)));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
         if (ZentraleGetVerbose(Data))
            printf("FSM: new state %d\n",NewStateForContinue);
         return(NewStateForContinue);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
      {
         printf("FSM: LOKINFO CFG data, aktuell %lu von %lu\n",
                Cs2CfgDataGetHaveRead(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)),
                Cs2CfgDataGetLength(ZentraleGetCs2CfgDaten(Data), MrIpcGetCanHash(CmdFrame)));
         printf("FSM: new state %d\n",STATE_WAIT_LOKINFO_CFG_DATA);
      }
      return(NewStateForStay);
   }
}

static int HandleLokinfoWaitCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   if (ZentraleGetVerbose(Data))
      puts("HandleLokinfoWaitCfgData");
   NewState = DoLokinfoWaitCfgData(Data, SignalData,
                                   STATE_NORMAL,
                                   STATE_WAIT_LOKINFO_CFG_HDR,
                                   STATE_WAIT_LOKINFO_CFG_DATA);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   else
   {
      DisableLongPolls(Data);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int HandleMs2LokinfoWaitCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NewState;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLokinfoWaitCfgData(Data, SignalData,
                                   STATE_NORMAL,
                                   STATE_WAIT_MS2_LOKINFO_CFG_HDR,
                                   STATE_WAIT_MS2_LOKINFO_CFG_DATA);
   if (NewState == STATE_NORMAL)
   {
      ReenableLongPolls(Data, PERIODIC_NAME_QUERY);
   }
   else
   {
      DisableLongPolls(Data);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   return(NewState);
}

static int DoLoknameWaitCfgHeader(void *Priv, void *SignalData,
                                  int NewStateForContinue,
                                  int NewStateForStay)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (DoCfgHeader(Data, CmdFrame, FALSE))
   {
      if (ZentraleGetVerbose(Data))
      {
         puts("HandleLoknameWaitCfgHeader: continue");
         printf("FSM: new state %d\n",STATE_WAIT_LOKNAME_CFG_DATA);
      }
      return(NewStateForContinue);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
      {
         puts("HandleLoknameWaitCfgHeader: error");
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(NewStateForStay);
   }
}

static int HandleLoknameWaitCfgHeader(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLoknameWaitCfgHeader(Priv, SignalData,
                                     STATE_WAIT_LOKNAME_CFG_DATA,
                                     STATE_WAIT_FOR_MS2);
   if (NewState == STATE_WAIT_FOR_MS2)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   else
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int HandleMs2LoknameWaitCfgHeader(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLoknameWaitCfgHeader(Priv, SignalData,
                                     STATE_WAIT_MS2_LOKNAME_CFG_DATA,
                                     STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      ReenableLongPolls(Data, PERIODIC_NAME_QUERY);
   }
   else
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int DoLoknameCfgData(ZentraleStruct *Data, void *SignalData,
                            int NewStateForNextStep,
                            int NewStateForContinue,
                            int NewStateForStay)
{  MrIpcCmdType Cmd, *CmdFrame;
   char Buf[8];

   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         MrIpcGetCanHash(CmdFrame), Buf))
   {
      if (HandleCfgLoknamen(Data, CmdFrame))
      {
         ZentraleSetActualIndex(Data, 0);
         if (ZentraleGetVerbose(Data))
            printf("request lokinfo %d >%s< from %d\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleGetLokNamenNr(Data, ZentraleGetActualIndex(Data)),
                   ZentraleGetNumLoks(Data));
         MrIpcHlLocinfoRequest(&Cmd, ZentraleGetUid(Data),
                               MR_IPC_SOCKET_ALL,
                               MrIpcGetSenderSocket(CmdFrame),
                               ZentraleGetLokNamenNr(Data,
                                                     ZentraleGetActualIndex(Data)));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
         if (ZentraleGetVerbose(Data))
            printf("FSM: new state %d\n", NewStateForNextStep);
         return(NewStateForNextStep);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            printf("request lokname %d\n", ZentraleGetActualIndex(Data));
         MrIpcHlLocnameRequest(&Cmd, ZentraleGetUid(Data),
                               MR_IPC_SOCKET_ALL,
                               MrIpcGetSenderSocket(CmdFrame),
                               ZentraleGetActualIndex(Data));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data))
            printf("FSM: new state %d\n",NewStateForContinue);
         return(NewStateForContinue);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",NewStateForStay);
      return(NewStateForStay);
   }
}

static int HandleLoknameCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   NextState = DoLoknameCfgData(Data, SignalData,
                                STATE_WAIT_LOKINFO_CFG_HDR,
                                STATE_WAIT_LOKNAME_CFG_HDR,
                                STATE_WAIT_LOKNAME_CFG_DATA);
   if (NextState == STATE_WAIT_LOKNAME_CFG_HDR)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NextState);
}

static int HandleMs2LoknameCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   NextState = DoLoknameCfgData(Data, SignalData,
                                STATE_WAIT_MS2_LOKINFO_CFG_HDR,
                                STATE_WAIT_MS2_LOKNAME_CFG_HDR,
                                STATE_WAIT_MS2_LOKNAME_CFG_DATA);
   if (NextState == STATE_WAIT_MS2_LOKNAME_CFG_HDR)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   return(NextState);
}

static int DoLokinfoWaitCfgHeader(ZentraleStruct *Data, void *SignalData,                                  int NewStateForContinue,
                                  int NewStateForStay)
{  MrIpcCmdType *CmdFrame;

   CmdFrame = (MrIpcCmdType *)SignalData;
   if (DoCfgHeader(Data, CmdFrame, FALSE))
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n", NewStateForContinue);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      return(NewStateForContinue);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n", NewStateForStay);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      ReenableLongPolls(Data, PERIODIC_NAME_POLLMS2);
      return(NewStateForStay);
   }
}

static int HandleLokinfoWaitCfgHeader(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLokinfoWaitCfgHeader(Data, SignalData,
                                     STATE_WAIT_LOKINFO_CFG_DATA,
                                     STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   else
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int HandleMs2LokinfoWaitCfgHeader(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLokinfoWaitCfgHeader(Data, SignalData,
                                     STATE_WAIT_MS2_LOKINFO_CFG_DATA,
                                     STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   else
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int PeriodicQueryMembers(void *PrivData)
{  ZentraleStruct *Data;
   int NextState;

   Data = (ZentraleStruct *)PrivData;
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   NextState = QueryMembers(Data,
                            FsmGetState(ZentraleGetStateMachine(Data)) == STATE_NORMAL);
   if ((NextState == STATE_NORMAL) || (NextState == STATE_NO_CHANGE))
   {
      CronResume(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   return(NextState);
}

static int PeriodicPollMs2(void *PrivData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)PrivData;
   if (ZentraleGetVerbose(Data))
      puts("FSM: periodic task poll MS2");
   LokMarkAllDeleted(ZentraleGetLoks(Data));
   ZentraleSetActualIndex(Data, 0);
   MrIpcHlLocnameRequest(&Cmd, ZentraleGetUid(Data),
                         ZentraleGetClientSock(Data), MR_IPC_SOCKET_ALL,
                         ZentraleGetActualIndex(Data));
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
   DisableLongPolls(Data);
   CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   return(STATE_WAIT_MS2_LOKNAME_CFG_HDR);
}

static int PeriodicQueryLokname(void *PrivData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)PrivData;
   if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
   {
      if (ZentraleGetVerbose(Data))
         printf("request lokname %d\n", ZentraleGetActualIndex(Data));
      MrIpcHlLocnameRequest(&Cmd, ZentraleGetUid(Data),
                            ZentraleGetClientSock(Data), MR_IPC_SOCKET_ALL,
                            ZentraleGetActualIndex(Data));
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      ZentraleSetActualIndex(Data, 0);
      if (ZentraleGetVerbose(Data))
         printf("request lokinfo %d >%s< from %d\n",
                ZentraleGetActualIndex(Data),
                ZentraleGetLokNamenNr(Data, ZentraleGetActualIndex(Data)),
                ZentraleGetNumLoks(Data));
      MrIpcHlLocinfoRequest(&Cmd, ZentraleGetUid(Data),
                            ZentraleGetClientSock(Data), MR_IPC_SOCKET_ALL,
                            ZentraleGetLokNamenNr(Data,
                                                  ZentraleGetActualIndex(Data)));
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKNAME);
   return(STATE_NO_CHANGE);
}

static int PeriodicQueryLokinfo(void *PrivData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)PrivData;
   if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
   {
      if (ZentraleGetVerbose(Data))
         printf("request lokinfo %d >%s< from %d\n",
                ZentraleGetActualIndex(Data),
                ZentraleGetLokNamenNr(Data,
                                      ZentraleGetActualIndex(Data)),
                ZentraleGetNumLoks(Data));
      MrIpcHlLocinfoRequest(&Cmd, ZentraleGetUid(Data),
                            ZentraleGetClientSock(Data), MR_IPC_SOCKET_ALL,
                            ZentraleGetLokNamenNr(Data,
                                                  ZentraleGetActualIndex(Data)));
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_PERIODIC) != 0)
      {
         if (ZentraleGetVerbose(Data))
            puts("enable periodic poll of MS2");
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            puts("disable periodic poll of MS2");
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
      }
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
   }
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_LOKINFO);
   return(STATE_NO_CHANGE);
}

static int PeriodicTimeoutWaitMS2(void *PrivData)
{
   return(STATE_WAIT_FOR_MS2);
}

static int DoLoklisteWaitCfgHeader(void *Priv, void *SignalData,
                                   int NewStateForContinue,
                                   int NewStateForStay)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (DoCfgHeader(Data, CmdFrame, FALSE))
   {
      if (ZentraleGetVerbose(Data))
      {
         puts("HandleLoklisteWaitCfgHeader: continue");
         printf("FSM: new state %d\n",STATE_WAIT_LOKLISTE_CFG_DATA);
      }
      return(NewStateForContinue);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
      {
         puts("HandleLoklisteWaitCfgHeader: error");
         printf("FSM: new state %d\n",STATE_NORMAL);
      }
      return(NewStateForStay);
   }
}

static int HandleLoklisteWaitCfgHeader(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = DoLoklisteWaitCfgHeader(Priv, SignalData,
                                      STATE_WAIT_LOKLISTE_CFG_DATA,
                                      STATE_WAIT_FOR_MS2);
   if (NewState == STATE_WAIT_FOR_MS2)
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_QUERY);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_POLLMS2);
   }
   else
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
   }
   return(NewState);
}

static int DoLoklisteCfgData(ZentraleStruct *Data, void *SignalData,
                             int NewStateForContinue,
                             int NewStateForStay)
{  MrIpcCmdType Cmd, *CmdFrame;
   char Buf[8];

   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   if (Cs2CfgDataNextBuf(ZentraleGetCs2CfgDaten(Data),
                         MrIpcGetCanHash(CmdFrame), Buf))
   {
      HandleCfgLokliste(Data, CmdFrame);
      ZentraleSetActualIndex(Data, 0);
      if (ZentraleGetVerbose(Data))
         printf("request lokinfo %d >%s< from %d\n",
                ZentraleGetActualIndex(Data),
                ZentraleGetLokNamenNr(Data, ZentraleGetActualIndex(Data)),
                ZentraleGetNumLoks(Data));
      MrIpcHlLocinfoRequest(&Cmd, ZentraleGetUid(Data),
                            MR_IPC_SOCKET_ALL,
                            MrIpcGetSenderSocket(CmdFrame),
                            ZentraleGetLokNamenNr(Data,
                                                  ZentraleGetActualIndex(Data)));
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_MS2);
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n", NewStateForContinue);
      return(NewStateForContinue);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: new state %d\n",NewStateForStay);
      return(NewStateForStay);
   }
}

static int HandleLoklisteCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   int NextState;

   Data = (ZentraleStruct *)Priv;
   NextState = DoLoklisteCfgData(Data, SignalData,
                                 STATE_WAIT_LOKINFO_CFG_HDR,
                                 STATE_WAIT_LOKLISTE_CFG_DATA);
   return(NextState);
}

static int HandleMemberInternalCmd(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   CanMemberInfo *Ms2;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   switch (MrIpcGetCanCommand(CmdFrame))
   {
      case MrIpcInternalPollMs2:
         ZentraleSetActualIndex(Data, 0);
         Ms2 = CanMemberSearchMs2(ZentraleGetCanMember(Data));
         return(PollMs2(Data, CanMemberInfoGetVersion(Ms2)));
         break;
      default:
         return(STATE_NO_CHANGE);
         break;
   }
}
