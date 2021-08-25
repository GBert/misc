static StateFktType StateMs2MasterWaitForMs2[NUM_SIGNALS] = {
   /* STATE_WAIT_FOR_MS2 */
   HandleTimer,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcTrackProto */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandleOther,         /* MrIpcCmdRequestMember */
   HandleMemberWaitMs2, /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdRequestFile */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleOther,         /* MrIpcCmdCfgZHeader */
   HandleOther,         /* MrIpcCmdCfgData */
   HandleCanBootldr,    /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest, /* MrIpcCmdStatusRequest */
   HandleOther,         /* MrIpcCmdStatusSize */
   HandleOther,         /* MrIpcCmdStatusData */
   HandleOther,         /* MrIpcCmdStatusResponse */
   HandleOther,         /* MrIpcCmdRequestMesswert */
   HandleOther,         /* MrIpcCmdSetConfig */
   HandleOther,         /* MrIpcCmdConfigResponse */
   HandleOther,         /* MrIpcCmdMesswertResponse */
   HandleOther,         /* MrIpcCmdRequestLokListe */
   HandleOther,         /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitLoknameCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_HDR */
   HandleTimer,                /* timer */
   HandleOther,                /* MrIpcCmdNull */
   HandleOther,                /* MrIpcCmdRun */
   HandleOther,                /* MrIpcTrackProto */
   HandleOther,                /* MrIpcCmdLocomotiveSpeed */
   HandleOther,                /* MrIpcCmdLocomotiveDirection */
   HandleOther,                /* MrIpcCmdLocomotiveFunction */
   HandleOther,                /* MrIpcCmdAccSwitch */
   HandleOther,                /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,      /* MrIpcCmdMember */
   HandleOther,                /* MrIpcCmdRequestLocName */
   HandleOther,                /* MrIpcCmdRequestLocInfo */
   HandleOther,                /* MrIpcCmdRequestFile */
   HandleLoknameWaitCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther,                /* MrIpcCmdCfgZHeader */
   HandleOther,                /* MrIpcCmdCfgData */
   HandleCanBootldr,           /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,        /* MrIpcCmdStatusRequest */
   HandleOther,                /* MrIpcCmdStatusSize */
   HandleOther,                /* MrIpcCmdStatusData */
   HandleOther,                /* MrIpcCmdStatusResponse */
   HandleOther,                /* MrIpcCmdRequestMesswert */
   HandleOther,                /* MrIpcCmdSetConfig */
   HandleOther,                /* MrIpcCmdConfigResponse */
   HandleOther,                /* MrIpcCmdMesswertResponse */
   HandleOther,                /* MrIpcCmdRequestLokListe */
   HandleOther,                /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitLoknameCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_DATA */
   HandleTimer,           /* timer */
   HandleOther,           /* MrIpcCmdNull */
   HandleOther,           /* MrIpcCmdRun */
   HandleOther,           /* MrIpcTrackProto */
   HandleOther,           /* MrIpcCmdLocomotiveSpeed */
   HandleOther,           /* MrIpcCmdLocomotiveDirection */
   HandleOther,           /* MrIpcCmdLocomotiveFunction */
   HandleOther,           /* MrIpcCmdAccSwitch */
   HandleOther,           /* MrIpcCmdRequestMember */
   HandleMemberMs2Master, /* MrIpcCmdMember */
   HandleOther,           /* MrIpcCmdRequestLocName */
   HandleOther,           /* MrIpcCmdRequestLocInfo */
   HandleOther,           /* MrIpcCmdRequestFile */
   HandleOther,           /* MrIpcCmdCfgHeader */
   HandleOther,           /* MrIpcCmdCfgZHeader */
   HandleLoknameCfgData,  /* MrIpcCmdCfgData */
   HandleCanBootldr,      /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,   /* MrIpcCmdStatusRequest */
   HandleOther,           /* MrIpcCmdStatusSize */
   HandleOther,           /* MrIpcCmdStatusData */
   HandleOther,           /* MrIpcCmdStatusResponse */
   HandleOther,           /* MrIpcCmdRequestMesswert */
   HandleOther,           /* MrIpcCmdSetConfig */
   HandleOther,           /* MrIpcCmdConfigResponse */
   HandleOther,           /* MrIpcCmdMesswertResponse */
   HandleOther,           /* MrIpcCmdRequestLokListe */
   HandleOther,           /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitLokinfoCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_HDR */
   HandleTimer,                /* timer */
   HandleOther,                /* MrIpcCmdNull */
   HandleOther,                /* MrIpcCmdRun */
   HandleOther,                /* MrIpcTrackProto */
   HandleOther,                /* MrIpcCmdLocomotiveSpeed */
   HandleOther,                /* MrIpcCmdLocomotiveDirection */
   HandleOther,                /* MrIpcCmdLocomotiveFunction */
   HandleOther,                /* MrIpcCmdAccSwitch */
   HandleOther,                /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,      /* MrIpcCmdMember */
   HandleOther,                /* MrIpcCmdRequestLocName */
   HandleOther,                /* MrIpcCmdRequestLocInfo */
   HandleOther,                /* MrIpcCmdRequestFile */
   HandleLokinfoWaitCfgHeader, /* MrIpcCmdCfgHeader */
   HandleLokinfoWaitCfgHeader, /* MrIpcCmdCfgZHeader */
   HandleOther,                /* MrIpcCmdCfgData */
   HandleCanBootldr,           /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,        /* MrIpcCmdStatusRequest */
   HandleOther,                /* MrIpcCmdStatusSize */
   HandleOther,                /* MrIpcCmdStatusData */
   HandleOther,                /* MrIpcCmdStatusResponse */
   HandleOther,                /* MrIpcCmdRequestMesswert */
   HandleOther,                /* MrIpcCmdSetConfig */
   HandleOther,                /* MrIpcCmdConfigResponse */
   HandleOther,                /* MrIpcCmdMesswertResponse */
   HandleOther,                /* MrIpcCmdRequestLokListe */
   HandleOther,                /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitLokinfoCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_DATA */
   HandleTimer,              /* timer */
   HandleOther,              /* MrIpcCmdNull */
   HandleOther,              /* MrIpcCmdRun */
   HandleOther,              /* MrIpcTrackProto */
   HandleOther,              /* MrIpcCmdLocomotiveSpeed */
   HandleOther,              /* MrIpcCmdLocomotiveDirection */
   HandleOther,              /* MrIpcCmdLocomotiveFunction */
   HandleOther,              /* MrIpcCmdAccSwitch */
   HandleOther,              /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,    /* MrIpcCmdMember */
   HandleOther,              /* MrIpcCmdRequestLocName */
   HandleOther,              /* MrIpcCmdRequestLocInfo */
   HandleOther,              /* MrIpcCmdRequestFile */
   HandleOther,              /* MrIpcCmdCfgHeader */
   HandleOther,              /* MrIpcCmdCfgZHeader */
   HandleLokinfoWaitCfgData, /* MrIpcCmdCfgData */
   HandleCanBootldr,         /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,      /* MrIpcCmdStatusRequest */
   HandleOther,              /* MrIpcCmdStatusSize */
   HandleOther,              /* MrIpcCmdStatusData */
   HandleOther,              /* MrIpcCmdStatusResponse */
   HandleOther,              /* MrIpcCmdRequestMesswert */
   HandleOther,              /* MrIpcCmdSetConfig */
   HandleOther,              /* MrIpcCmdConfigResponse */
   HandleOther,              /* MrIpcCmdMesswertResponse */
   HandleOther,              /* MrIpcCmdRequestLokListe */
   HandleOther,              /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandleOther,          /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,/* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleCfgData,        /* MrIpcCmdCfgData */
   HandleCanBootldr,     /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,  /* MrIpcCmdStatusRequest */
   HandleOther,          /* MrIpcCmdStatusSize */
   HandleOther,          /* MrIpcCmdStatusData */
   HandleOther,          /* MrIpcCmdStatusResponse */
   HandleOther,          /* MrIpcCmdRequestMesswert */
   HandleOther,          /* MrIpcCmdSetConfig */
   HandleOther,          /* MrIpcCmdConfigResponse */
   HandleOther,          /* MrIpcCmdMesswertResponse */
   HandleOther,          /* MrIpcCmdRequestLokListe */
   HandleOther,          /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterNormal[NUM_SIGNALS] = {
   /* STATE_NORMAL */
   HandleTimer,                 /* timer */
   HandleOther,                 /* MrIpcCmdNull */
   HandleOther,                 /* MrIpcCmdRun */
   HandleOther,                 /* MrIpcTrackProto */
   HandleLokSpeed,              /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,          /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,           /* MrIpcCmdLocomotiveFunction */
   HandleOther,                 /* MrIpcCmdAccSwitch */
   HandlePing,                  /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,       /* MrIpcCmdMember */
   HandleOther,                 /* MrIpcCmdRequestLocName */
   HandleOther,                 /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,           /* MrIpcCmdRequestFile */
   HandleOther,                 /* MrIpcCmdCfgHeader */
   HandleOther,                 /* MrIpcCmdCfgZHeader */
   HandleCfgDataMs2Master,      /* MrIpcCmdCfgData */
   HandleCanBootldr,            /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,         /* MrIpcCmdStatusRequest */
   HandleOther,                 /* MrIpcCmdStatusSize */
   HandleOther,                 /* MrIpcCmdStatusData */
   HandleOther,                 /* MrIpcCmdStatusResponse */
   HandleOther,                 /* MrIpcCmdRequestMesswert */
   HandleOther,                 /* MrIpcCmdSetConfig */
   HandleOther,                 /* MrIpcCmdConfigResponse */
   HandleOther,                 /* MrIpcCmdMesswertResponse */
   HandleOther,                 /* MrIpcCmdRequestLokListe */
   HandleMemberInternalCmd,     /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitCfgDataHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandleOther,            /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,  /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleOther,            /* MrIpcCmdCfgHeader */
   HandleOther,            /* MrIpcCmdCfgZHeader */
   HandleCfgDataMs2Master, /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleCfgDataHeader,    /* MrIpcCmdStatusSize */
   HandleCfgDataFirstData, /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitCfgDataData[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA_DATA */
   HandleTimer,                     /* timer */
   HandleOther,                     /* MrIpcCmdNull */
   HandleOther,                     /* MrIpcCmdRun */
   HandleOther,                     /* MrIpcTrackProto */
   HandleLokSpeed,                  /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,              /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,               /* MrIpcCmdLocomotiveFunction */
   HandleOther,                     /* MrIpcCmdAccSwitch */
   HandleOther,                     /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,           /* MrIpcCmdMember */
   HandleOther,                     /* MrIpcCmdRequestLocName */
   HandleOther,                     /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,               /* MrIpcCmdRequestFile */
   HandleOther,                     /* MrIpcCmdCfgHeader */
   HandleOther,                     /* MrIpcCmdCfgZHeader */
   HandleCfgDataMs2Master,          /* MrIpcCmdCfgData */
   HandleCanBootldr,                /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,             /* MrIpcCmdStatusRequest */
   HandleCfgDataLateHeaderMs2Master,/* MrIpcCmdStatusSize */
   HandleCfgDataData,               /* MrIpcCmdStatusData */
   HandleStatusResponse,            /* MrIpcCmdStatusResponse */
   HandleOther,                     /* MrIpcCmdRequestMesswert */
   HandleOther,                     /* MrIpcCmdSetConfig */
   HandleOther,                     /* MrIpcCmdConfigResponse */
   HandleOther,                     /* MrIpcCmdMesswertResponse */
   HandleOther,                     /* MrIpcCmdRequestLokListe */
   HandleOther,                     /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitSysCfgMesswert[NUM_SIGNALS] = {
   /* STATE_WAIT_SYS_CFG_MESSWERT */
   HandleTimer,                 /* timer */
   HandleOther,                 /* MrIpcCmdNull */
   HandleOther,                 /* MrIpcCmdRun */
   HandleOther,                 /* MrIpcTrackProto */
   HandleLokSpeed,              /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,          /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,           /* MrIpcCmdLocomotiveFunction */
   HandleOther,                 /* MrIpcCmdAccSwitch */
   HandlePing,                  /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,       /* MrIpcCmdMember */
   HandleOther,                 /* MrIpcCmdRequestLocName */
   HandleOther,                 /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,           /* MrIpcCmdRequestFile */
   HandleOther,                 /* MrIpcCmdCfgHeader */
   HandleOther,                 /* MrIpcCmdCfgZHeader */
   HandleCfgDataMs2Master,      /* MrIpcCmdCfgData */
   HandleCanBootldr,            /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,         /* MrIpcCmdStatusRequest */
   HandleOther,                 /* MrIpcCmdStatusSize */
   HandleOther,                 /* MrIpcCmdStatusData */
   HandleOther,                 /* MrIpcCmdStatusResponse */
   HandleOther,                 /* MrIpcCmdRequestMesswert */
   HandleOther,                 /* MrIpcCmdSetConfig */
   HandleOther,                 /* MrIpcCmdConfigResponse */
   HandleMesswertResponse,      /* MrIpcCmdMesswertResponse */
   HandleOther,                 /* MrIpcCmdRequestLokListe */
   HandleOther,                 /* MrIpcCmdIntern */
};
static StateFktType StateMs2LoknameCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_MS2_LOKNAME_CFG_HDR */
   HandleTimer,                  /* timer */
   HandleOther,                  /* MrIpcCmdNull */
   HandleOther,                  /* MrIpcCmdRun */
   HandleOther,                  /* MrIpcTrackProto */
   HandleLokSpeed,               /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,           /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,                  /* MrIpcCmdAccSwitch */
   HandlePing,                   /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,        /* MrIpcCmdMember */
   HandleOther,                  /* MrIpcCmdRequestLocName */
   HandleOther,                  /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,            /* MrIpcCmdRequestFile */
   HandleMs2LoknameWaitCfgHeader,/* MrIpcCmdCfgHeader */
   HandleOther,                  /* MrIpcCmdCfgZHeader */
   HandleOther,                  /* MrIpcCmdCfgData */
   HandleCanBootldr,             /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,          /* MrIpcCmdStatusRequest */
   HandleOther,                  /* MrIpcCmdStatusSize */
   HandleOther,                  /* MrIpcCmdStatusData */
   HandleOther,                  /* MrIpcCmdStatusResponse */
   HandleOther,                  /* MrIpcCmdRequestMesswert */
   HandleOther,                  /* MrIpcCmdSetConfig */
   HandleOther,                  /* MrIpcCmdConfigResponse */
   HandleOther,                  /* MrIpcCmdMesswertResponse */
   HandleOther,                  /* MrIpcCmdRequestLokListe */
   HandleOther,                  /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterLoknameCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_MS2_LOKNAME_CFG_DATA */
   HandleTimer,                  /* timer */
   HandleOther,                  /* MrIpcCmdNull */
   HandleOther,                  /* MrIpcCmdRun */
   HandleOther,                  /* MrIpcTrackProto */
   HandleLokSpeed,               /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,           /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,                  /* MrIpcCmdAccSwitch */
   HandlePing,                   /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,        /* MrIpcCmdMember */
   HandleOther,                  /* MrIpcCmdRequestLocName */
   HandleOther,                  /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,            /* MrIpcCmdRequestFile */
   HandleOther,                  /* MrIpcCmdCfgHeader */
   HandleOther,                  /* MrIpcCmdCfgZHeader */
   HandleMs2LoknameCfgData,      /* MrIpcCmdCfgData */
   HandleCanBootldr,             /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,          /* MrIpcCmdStatusRequest */
   HandleOther,                  /* MrIpcCmdStatusSize */
   HandleOther,                  /* MrIpcCmdStatusData */
   HandleOther,                  /* MrIpcCmdStatusResponse */
   HandleOther,                  /* MrIpcCmdRequestMesswert */
   HandleOther,                  /* MrIpcCmdSetConfig */
   HandleOther,                  /* MrIpcCmdConfigResponse */
   HandleOther,                  /* MrIpcCmdMesswertResponse */
   HandleOther,                  /* MrIpcCmdRequestLokListe */
   HandleOther,                  /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterLokinfoCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_MS2_LOKINFO_CFG_HDR */
   HandleTimer,                   /* timer */
   HandleOther,                   /* MrIpcCmdNull */
   HandleOther,                   /* MrIpcCmdRun */
   HandleOther,                   /* MrIpcTrackProto */
   HandleLokSpeed,                /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,            /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,             /* MrIpcCmdLocomotiveFunction */
   HandleOther,                   /* MrIpcCmdAccSwitch */
   HandlePing,                    /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,         /* MrIpcCmdMember */
   HandleOther,                   /* MrIpcCmdRequestLocName */
   HandleOther,                   /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,             /* MrIpcCmdRequestFile */
   HandleMs2LokinfoWaitCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther,                   /* MrIpcCmdCfgZHeader */
   HandleOther,                   /* MrIpcCmdCfgData */
   HandleCanBootldr,              /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,           /* MrIpcCmdStatusRequest */
   HandleOther,                   /* MrIpcCmdStatusSize */
   HandleOther,                   /* MrIpcCmdStatusData */
   HandleOther,                   /* MrIpcCmdStatusResponse */
   HandleOther,                   /* MrIpcCmdRequestMesswert */
   HandleOther,                   /* MrIpcCmdSetConfig */
   HandleOther,                   /* MrIpcCmdConfigResponse */
   HandleOther,                   /* MrIpcCmdMesswertResponse */
   HandleOther,                   /* MrIpcCmdRequestLokListe */
   HandleOther,                   /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterLokinfoCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_MS2_LOKINFO_CFG_DATA */
   HandleTimer,                 /* timer */
   HandleOther,                 /* MrIpcCmdNull */
   HandleOther,                 /* MrIpcCmdRun */
   HandleOther,                 /* MrIpcTrackProto */
   HandleLokSpeed,              /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,          /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,           /* MrIpcCmdLocomotiveFunction */
   HandleOther,                 /* MrIpcCmdAccSwitch */
   HandlePing,                  /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,       /* MrIpcCmdMember */
   HandleOther,                 /* MrIpcCmdRequestLocName */
   HandleOther,                 /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,           /* MrIpcCmdRequestFile */
   HandleOther,                 /* MrIpcCmdCfgHeader */
   HandleOther,                 /* MrIpcCmdCfgZHeader */
   HandleMs2LokinfoWaitCfgData, /* MrIpcCmdCfgData */
   HandleCanBootldr,            /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,         /* MrIpcCmdStatusRequest */
   HandleOther,                 /* MrIpcCmdStatusSize */
   HandleOther,                 /* MrIpcCmdStatusData */
   HandleOther,                 /* MrIpcCmdStatusResponse */
   HandleOther,                 /* MrIpcCmdRequestMesswert */
   HandleOther,                 /* MrIpcCmdSetConfig */
   HandleOther,                 /* MrIpcCmdConfigResponse */
   HandleOther,                 /* MrIpcCmdMesswertResponse */
   HandleOther,                 /* MrIpcCmdRequestLokListe */
   HandleOther,                 /* MrIpcCmdIntern */
};
static StateFktType StateMs2LoklisteCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKLISTE_CFG_HDR */
   HandleTimer,                /* timer */
   HandleOther,                /* MrIpcCmdNull */
   HandleOther,                /* MrIpcCmdRun */
   HandleOther,                /* MrIpcTrackProto */
   HandleLokSpeed,             /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,         /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,          /* MrIpcCmdLocomotiveFunction */
   HandleOther,                /* MrIpcCmdAccSwitch */
   HandlePing,                 /* MrIpcCmdRequestMember */
   HandleMemberMs2Master,      /* MrIpcCmdMember */
   HandleOther,                /* MrIpcCmdRequestLocName */
   HandleOther,                /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,          /* MrIpcCmdRequestFile */
   HandleLoklisteWaitCfgHeader,/* MrIpcCmdCfgHeader */
   HandleLoklisteWaitCfgHeader,/* MrIpcCmdCfgZHeader */
   HandleOther,                /* MrIpcCmdCfgData */
   HandleCanBootldr,           /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,        /* MrIpcCmdStatusRequest */
   HandleOther,                /* MrIpcCmdStatusSize */
   HandleOther,                /* MrIpcCmdStatusData */
   HandleOther,                /* MrIpcCmdStatusResponse */
   HandleOther,                /* MrIpcCmdRequestMesswert */
   HandleOther,                /* MrIpcCmdSetConfig */
   HandleOther,                /* MrIpcCmdConfigResponse */
   HandleOther,                /* MrIpcCmdMesswertResponse */
   HandleOther,                /* MrIpcCmdRequestLokListe */
   HandleOther,                /* MrIpcCmdIntern */
};
static StateFktType StateMs2MasterWaitLoklisteCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKLISTE_CFG_DATA */
   HandleTimer,           /* timer */
   HandleOther,           /* MrIpcCmdNull */
   HandleOther,           /* MrIpcCmdRun */
   HandleOther,           /* MrIpcTrackProto */
   HandleOther,           /* MrIpcCmdLocomotiveSpeed */
   HandleOther,           /* MrIpcCmdLocomotiveDirection */
   HandleOther,           /* MrIpcCmdLocomotiveFunction */
   HandleOther,           /* MrIpcCmdAccSwitch */
   HandleOther,           /* MrIpcCmdRequestMember */
   HandleMemberMs2Master, /* MrIpcCmdMember */
   HandleOther,           /* MrIpcCmdRequestLocName */
   HandleOther,           /* MrIpcCmdRequestLocInfo */
   HandleOther,           /* MrIpcCmdRequestFile */
   HandleOther,           /* MrIpcCmdCfgHeader */
   HandleOther,           /* MrIpcCmdCfgZHeader */
   HandleLoklisteCfgData, /* MrIpcCmdCfgData */
   HandleCanBootldr,      /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,   /* MrIpcCmdStatusRequest */
   HandleOther,           /* MrIpcCmdStatusSize */
   HandleOther,           /* MrIpcCmdStatusData */
   HandleOther,           /* MrIpcCmdStatusResponse */
   HandleOther,           /* MrIpcCmdRequestMesswert */
   HandleOther,           /* MrIpcCmdSetConfig */
   HandleOther,           /* MrIpcCmdConfigResponse */
   HandleOther,           /* MrIpcCmdMesswertResponse */
   HandleOther,           /* MrIpcCmdRequestLokListe */
   HandleOther,           /* MrIpcCmdIntern */
};

static SignalFunctionsType StateMachineFunctionsMs2Master[NUM_STATES] = {
   StateMs2MasterWaitForMs2,         /* STATE_WAIT_FOR_MS2 */
   StateMs2MasterWaitLoknameCfgHdr,  /* STATE_WAIT_LOKNAME_CFG_HDR */
   StateMs2MasterWaitLoknameCfgData, /* STATE_WAIT_LOKNAME_CFG_DATA */
   StateMs2MasterWaitLokinfoCfgHdr,  /* STATE_WAIT_LOKINFO_CFG_HDR */
   StateMs2MasterWaitLokinfoCfgData, /* STATE_WAIT_LOKINFO_CFG_DATA */
   StateMs2MasterWaitCfgData,        /* STATE_WAIT_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_CS2 */
   StateMs2MasterNormal,             /* STATE_NORMAL */
   (SignalFunctionsType)NULL,        /* STATE_GET_LOK_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_LOK_CS2_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_LOK_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_MAG_CS2_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_MAG_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_FS_CS2_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_FS_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GBS_CS2_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GBS_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GPG_CS2_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GPG_CS2_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_LOK_CVR_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_LOK_CVR_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_MAG_CVR_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_MAG_CVR_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GBS_CVR_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_GBS_CVR_CFG_DATA */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_FS_CVR_CFG_HDR */
   (SignalFunctionsType)NULL,        /* STATE_WAIT_FS_CVR_CFG_DATA */
   StateMs2MasterWaitCfgDataHdr,     /* STATE_WAIT_CFG_DATA_HDR */
   StateMs2MasterWaitCfgDataData,    /* STATE_WAIT_CFG_DATA_DATA */
   StateMs2MasterWaitSysCfgMesswert, /* STATE_WAIT_SYS_CFG_MESSWERT */
   StateMs2LoknameCfgHdr,            /* STATE_WAIT_MS2_LOKNAME_CFG_HDR */
   StateMs2MasterLoknameCfgData,     /* STATE_WAIT_MS2_LOKNAME_CFG_DATA */
   StateMs2MasterLokinfoCfgHdr,      /* STATE_WAIT_MS2_LOKINFO_CFG_HDR */
   StateMs2MasterLokinfoCfgData,     /* STATE_WAIT_MS2_LOKINFO_CFG_DATA */
   StateMs2LoklisteCfgHdr,           /* STATE_WAIT_LOKLISTE_CFG_HDR */
   StateMs2MasterWaitLoklisteCfgData /* STATE_WAIT_LOKLISTE_CFG_DATA */
};
