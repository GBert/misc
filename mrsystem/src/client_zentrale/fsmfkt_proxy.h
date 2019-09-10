static int PeriodicTimeoutWaitNormal(void *PrivData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)PrivData;
   if (ZentraleGetVerbose(Data))
      printf("FSM: timeout, new state %d\n",STATE_NORMAL);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   LokMarkAllUndeleted(ZentraleGetLoks(Data));
   CanMemberSetIsChanged(ZentraleGetCanMember(Data), TRUE);
   CanMemberClearConfig(ZentraleGetCanMember(Data));
   return(STATE_NORMAL);
}

static int HandleMemberWaitCs2Proxy(void *Priv, void *SignalData)
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
      if (Type == CS2_DEVID_CS2)
      {
         QueryMembers(Data, FALSE);
         if (ZentraleGetVerbose(Data))
            printf("FSM: new state %d\n",STATE_WAIT_FOR_MS2);
         CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_CS2);
         CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      }
      PingAnswerToS88(Data, &NewCanMember);
   }
   else
   {
      CanMemberInfoSetIsInvalid(OldCanMember, FALSE);
   }
   if (ZentraleGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleCfgHeaderProxy(void *Priv, void *SignalData,
                                int NewStateForContinue, int NewStateForStay)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   if (DoCfgHeader(Data, CmdFrame, TRUE))
   {
      return(NewStateForContinue);
   }
   else
   {
      return(NewStateForStay);
   }
}

static int HandleGetLokCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_GET_LOK_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleLokCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_LOK_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleMagCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_MAG_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleFsCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_FS_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleGbsCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_GBS_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleGpgCfgHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_GPG_CS2_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleLokCvrHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_LOK_CVR_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleMagCvrHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_MAG_CVR_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleGbsCvrHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_GBS_CVR_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleFsCvrHeaderProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleCfgHeaderProxy(Priv, SignalData,
                                   STATE_WAIT_FS_CVR_CFG_DATA,
                                   STATE_NORMAL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleLCfgDataProxy(void *Priv, void *SignalData,
                               int SignalContinue, int SignalStay,
                               char *CfgFile)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   if (DoCfgData(Priv, SignalData))
   {
      if (CfgFile != (char *)NULL)
      {
         MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), CfgFile);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      }
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",SignalContinue);
      return(SignalContinue);
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",SignalStay);
      return(SignalStay);
   }
}

static int HandleGetLokCfgDataProxy(void *Priv, void *SignalData)
{  int NewState;
   ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   NewState = HandleLCfgDataProxy(Priv, SignalData,
                                  STATE_NORMAL,
                                  STATE_GET_LOK_CS2_CFG_DATA, NULL);
   if (NewState == STATE_NORMAL)
   {
      CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   }
   else
   {
      CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   }
   return(NewState);
}

static int HandleLokCfgDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_KEYBD) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_MAG_CS2_CFG_HDR,
                                 STATE_WAIT_LOK_CS2_CFG_DATA, CS2_CFG_MAGS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_MEM) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_FS_CS2_CFG_HDR,
                                 STATE_WAIT_LOK_CS2_CFG_DATA, CS2_CFG_FS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_LAYOUT) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_GBS_CS2_CFG_HDR,
                                 STATE_WAIT_LOK_CS2_CFG_DATA, CS2_CFG_GBS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_LOK_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
   else
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_LOK_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
}

static int HandleMagCfgDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_MEM) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_FS_CS2_CFG_HDR,
                                 STATE_WAIT_MAG_CS2_CFG_DATA, CS2_CFG_FS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_LAYOUT) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_GBS_CS2_CFG_HDR,
                                 STATE_WAIT_MAG_CS2_CFG_DATA, CS2_CFG_GBS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_MAG_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
   else
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_MAG_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
}

static int HandleFsCfgDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)Priv;
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_PING);
   if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_LAYOUT) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_GBS_CS2_CFG_HDR,
                                 STATE_WAIT_FS_CS2_CFG_DATA, CS2_CFG_GBS));
   else if ((ZentraleGetSyncMask(Data) & MRSYSTEM_CFG_SYNC_CONTR) != 0)
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_FS_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
   else
      return(HandleLCfgDataProxy(Priv, SignalData, STATE_WAIT_LOK_CVR_CFG_HDR,
                                 STATE_WAIT_FS_CS2_CFG_DATA,
                                 CS2_CFG_LOK_STAT));
}

static int HandleGbsCfgDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;
   char PageNummerStr[CS2_NUM_CAN_BYTES + 1];

   Data = (ZentraleStruct *)Priv;
   if (DoCfgData(Priv, SignalData))
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: %d gleisbild pages\n",
                GleisbildGetNumPages(ZentraleGetGleisbild(Data)));
      ZentraleSetActualIndex(Data, 0);
      if (ZentraleGetActualIndex(Data) < GleisbildGetNumPages(ZentraleGetGleisbild(Data)))
      {
         if (ZentraleGetVerbose(Data))
            printf("FSM: fetch %d of %d\n", ZentraleGetActualIndex(Data),
                   GleisbildGetNumPages(ZentraleGetGleisbild(Data)));
         sprintf(PageNummerStr, "%s%d", CS2_CFG_GBS_PAGE, ZentraleGetActualIndex(Data));
         MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), PageNummerStr);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data))
            printf("FSM: rewrite new state %d\n",STATE_WAIT_GPG_CS2_CFG_HDR);
         return(STATE_WAIT_GPG_CS2_CFG_HDR);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            printf("FSM: rewrite new state %d\n",STATE_NORMAL);
         return(STATE_NORMAL);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",STATE_WAIT_GBS_CS2_CFG_DATA);
      return(STATE_WAIT_GBS_CS2_CFG_DATA);
   }
}

static int HandleGpgCfgDataProxy(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;
   char PageNummerStr[9];

   Data = (ZentraleStruct *)Priv;
   if (DoCfgData(Priv, SignalData))
   {
      ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
      if (ZentraleGetActualIndex(Data) < GleisbildGetNumPages(ZentraleGetGleisbild(Data)))
      {
         if (ZentraleGetVerbose(Data))
            printf("FSM: fetch %d of %d\n", ZentraleGetActualIndex(Data),
                   GleisbildGetNumPages(ZentraleGetGleisbild(Data)));
         sprintf(PageNummerStr, "%s%d", CS2_CFG_GBS_PAGE, ZentraleGetActualIndex(Data));
         MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), PageNummerStr);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data))
            printf("FSM: rewrite new state %d\n",STATE_WAIT_GPG_CS2_CFG_HDR);
         return(STATE_WAIT_GPG_CS2_CFG_HDR);
      }
      else
      {
         MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), CS2_CFG_LOK_STAT);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         if (ZentraleGetVerbose(Data))
            printf("FSM: rewrite new state %d\n",STATE_WAIT_LOK_CVR_CFG_HDR);
         return(STATE_WAIT_LOK_CVR_CFG_HDR);
      }
   }
   else
   {
      if (ZentraleGetVerbose(Data))
         printf("FSM: rewrite new state %d\n",STATE_WAIT_GPG_CS2_CFG_DATA);
      return(STATE_WAIT_GPG_CS2_CFG_DATA);
   }
}

static int PeriodicPing(void *PrivData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)PrivData;
   MrIpcHlMemberResponse(&Cmd, ZentraleGetUid(Data), CS2_DEVID_CS2);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
   return(STATE_NO_CHANGE);
}

static int PeriodicTimeoutCs2(void *PrivData)
{  ZentraleStruct *Data;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)PrivData;
   MrIpcHlConfigResponse(&Cmd, ZentraleGetUid(Data), CS2_CFG_LOKS);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
   LokMarkAllDeleted(ZentraleGetLoks(Data));
   if (ZentraleGetVerbose(Data))
      printf("FSM: timer, new state %d\n",STATE_WAIT_LOK_CS2_CFG_HDR);
   CronDisable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_CS2);
   CronEnable(ZentraleGetCronJobs(Data), PERIODIC_NAME_TO_NORMAL);
   return(STATE_WAIT_LOK_CS2_CFG_HDR);
}
