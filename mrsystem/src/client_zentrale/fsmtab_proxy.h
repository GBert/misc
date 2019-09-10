static StateFktType StateProxyWaitForMs2[NUM_SIGNALS] = {
   /* STATE_WAIT_FOR_MS2 */
   HandleTimer,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcTrackProto */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandlePing,          /* MrIpcCmdRequestMember */
   HandleOther,         /* MrIpcCmdMember */
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
static StateFktType StateProxyWaitCs[NUM_SIGNALS] = {
   /* STATE_WAIT_CS2 */
   HandleTimer,               /* timer */
   HandleOther,               /* MrIpcCmdNull */
   HandleOther,               /* MrIpcCmdRun */
   HandleOther,               /* MrIpcTrackProto */
   HandleLokSpeed,            /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,        /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,               /* MrIpcCmdAccSwitch */
   HandleOther,               /* MrIpcCmdRequestMember */
   HandleMemberWaitCs2Proxy,  /* MrIpcCmdMember */
   HandleOther,               /* MrIpcCmdRequestLocName */
   HandleOther,               /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,         /* MrIpcCmdRequestFile */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,               /* MrIpcCmdCfgData */
   HandleCanBootldr,          /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,       /* MrIpcCmdStatusRequest */
   HandleOther,               /* MrIpcCmdStatusSize */
   HandleOther,               /* MrIpcCmdStatusData */
   HandleOther,               /* MrIpcCmdStatusResponse */
   HandleOther,               /* MrIpcCmdRequestMesswert */
   HandleOther,               /* MrIpcCmdSetConfig */
   HandleOther,               /* MrIpcCmdConfigResponse */
   HandleOther,               /* MrIpcCmdMesswertResponse */
   HandleOther,               /* MrIpcCmdRequestLokListe */
   HandleOther,               /* MrIpcCmdIntern */
};
static StateFktType StateProxyNormal[NUM_SIGNALS] = {
   /* STATE_NORMAL */
   HandleTimer,               /* timer */
   HandleOther,               /* MrIpcCmdNull */
   HandleOther,               /* MrIpcCmdRun */
   HandleOther,               /* MrIpcTrackProto */
   HandleLokSpeed,            /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,        /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,               /* MrIpcCmdAccSwitch */
   HandlePing,                /* MrIpcCmdRequestMember */
   HandleMemberProxy,         /* MrIpcCmdMember */
   HandleOther,               /* MrIpcCmdRequestLocName */
   HandleOther,               /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,         /* MrIpcCmdRequestFile */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,               /* MrIpcCmdCfgData */
   HandleCanBootldr,          /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,       /* MrIpcCmdStatusRequest */
   HandleOther,               /* MrIpcCmdStatusSize */
   HandleOther,               /* MrIpcCmdStatusData */
   HandleOther,               /* MrIpcCmdStatusResponse */
   HandleOther,               /* MrIpcCmdRequestMesswert */
   HandleOther,               /* MrIpcCmdSetConfig */
   HandleOther,               /* MrIpcCmdConfigResponse */
   HandleOther,               /* MrIpcCmdMesswertResponse */
   HandleOther,               /* MrIpcCmdRequestLokListe */
   HandleOther,               /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitLokCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOK_CS2_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleLokCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleLokCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyGetGetLokCs2CfgData[NUM_SIGNALS] = {
   /* STATE_GET_LOK_CS2_CFG_DATA */
   HandleTimer,             /* timer */
   HandleOther,             /* MrIpcCmdNull */
   HandleOther,             /* MrIpcCmdRun */
   HandleOther,             /* MrIpcTrackProto */
   HandleLokSpeed,          /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,      /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,       /* MrIpcCmdLocomotiveFunction */
   HandleOther,             /* MrIpcCmdAccSwitch */
   HandlePing,              /* MrIpcCmdRequestMember */
   HandleMemberProxy,       /* MrIpcCmdMember */
   HandleOther,             /* MrIpcCmdRequestLocName */
   HandleOther,             /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,       /* MrIpcCmdRequestFile */
   HandleOther,             /* MrIpcCmdCfgHeader */
   HandleOther,             /* MrIpcCmdCfgZHeader */
   HandleGetLokCfgDataProxy,/* MrIpcCmdCfgData */
   HandleCanBootldr,        /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,     /* MrIpcCmdStatusRequest */
   HandleOther,             /* MrIpcCmdStatusSize */
   HandleOther,             /* MrIpcCmdStatusData */
   HandleOther,             /* MrIpcCmdStatusResponse */
   HandleOther,             /* MrIpcCmdRequestMesswert */
   HandleOther,             /* MrIpcCmdSetConfig */
   HandleOther,             /* MrIpcCmdConfigResponse */
   HandleOther,             /* MrIpcCmdMesswertResponse */
   HandleOther,             /* MrIpcCmdRequestLokListe */
   HandleOther,             /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitLokCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOK_CS2_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleLokCfgDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitMagCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_MAG_CS2_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleMagCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleMagCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitMagCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_MAG_CS2_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleMagCfgDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitFsCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_FS_CS2_CFG_HDR */
   HandleTimer,           /* timer */
   HandleOther,           /* MrIpcCmdNull */
   HandleOther,           /* MrIpcCmdRun */
   HandleOther,           /* MrIpcTrackProto */
   HandleLokSpeed,        /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,    /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,     /* MrIpcCmdLocomotiveFunction */
   HandleOther,           /* MrIpcCmdAccSwitch */
   HandlePing,            /* MrIpcCmdRequestMember */
   HandleMemberProxy,     /* MrIpcCmdMember */
   HandleOther,           /* MrIpcCmdRequestLocName */
   HandleOther,           /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,     /* MrIpcCmdRequestFile */
   HandleFsCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleFsCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,           /* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitFsCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_FS_CS2_CFG_DATA */
   HandleTimer,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcTrackProto */
   HandleLokSpeed,      /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,  /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,   /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandlePing,          /* MrIpcCmdRequestMember */
   HandleMemberProxy,   /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,   /* MrIpcCmdRequestFile */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleOther,         /* MrIpcCmdCfgZHeader */
   HandleFsCfgDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitGbsCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CS2_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleGbsCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGbsCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitGbsCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CS2_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleGbsCfgDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitGpgCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CS2_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleGpgCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGpgCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitGpgCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CS2_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleGpgCfgDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitLokCvrCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOK_CVR_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleLokCvrHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleLokCvrHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitLokCvrCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOK_CVR_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleLokCvrDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitMagCvrCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_MAG_CVR_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleMagCvrHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleMagCvrHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitMagCvrCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_MAG_CVR_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleMagCvrDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitGbsCvrCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CVR_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleGbsCvrHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGbsCvrHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitGbsCvrCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CVR_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleGbsCvrDataProxy,/* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitFsCvrCs2CfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CVR_CFG_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgHeader */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleOther,            /* MrIpcCmdStatusSize */
   HandleOther,            /* MrIpcCmdStatusData */
   HandleOther,            /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitFsCvrCs2CfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_GBS_CVR_CFG_DATA */
   HandleTimer,          /* timer */
   HandleOther,          /* MrIpcCmdNull */
   HandleOther,          /* MrIpcCmdRun */
   HandleOther,          /* MrIpcTrackProto */
   HandleLokSpeed,       /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,   /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,    /* MrIpcCmdLocomotiveFunction */
   HandleOther,          /* MrIpcCmdAccSwitch */
   HandlePing,           /* MrIpcCmdRequestMember */
   HandleMemberProxy,    /* MrIpcCmdMember */
   HandleOther,          /* MrIpcCmdRequestLocName */
   HandleOther,          /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,    /* MrIpcCmdRequestFile */
   HandleOther,          /* MrIpcCmdCfgHeader */
   HandleOther,          /* MrIpcCmdCfgZHeader */
   HandleFsCvrDataProxy, /* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitCfgDataHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA_HDR */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgHeader */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
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
static StateFktType StateProxyWaitCfgDataData[NUM_SIGNALS] = {
   /* STATE_WAIT_CFG_DATA_DATA */
   HandleTimer,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcTrackProto */
   HandleLokSpeed,         /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,     /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,      /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandlePing,             /* MrIpcCmdRequestMember */
   HandleMemberProxy,      /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,      /* MrIpcCmdRequestFile */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgHeader */
   HandleFsCvrHeaderProxy, /* MrIpcCmdCfgZHeader */
   HandleOther,            /* MrIpcCmdCfgData */
   HandleCanBootldr,       /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,    /* MrIpcCmdStatusRequest */
   HandleCfgDataLateHeader,/* MrIpcCmdStatusSize */
   HandleCfgDataData,      /* MrIpcCmdStatusData */
   HandleStatusResponse,   /* MrIpcCmdStatusResponse */
   HandleOther,            /* MrIpcCmdRequestMesswert */
   HandleOther,            /* MrIpcCmdSetConfig */
   HandleOther,            /* MrIpcCmdConfigResponse */
   HandleOther,            /* MrIpcCmdMesswertResponse */
   HandleOther,            /* MrIpcCmdRequestLokListe */
   HandleOther,            /* MrIpcCmdIntern */
};
static StateFktType StateProxyWaitSysCfgMesswert[NUM_SIGNALS] = {
   /* STATE_WAIT_SYS_CFG_MESSWERT */
   HandleTimer,               /* timer */
   HandleOther,               /* MrIpcCmdNull */
   HandleOther,               /* MrIpcCmdRun */
   HandleOther,               /* MrIpcTrackProto */
   HandleLokSpeed,            /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection,        /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,               /* MrIpcCmdAccSwitch */
   HandlePing,                /* MrIpcCmdRequestMember */
   HandleMemberProxy,         /* MrIpcCmdMember */
   HandleOther,               /* MrIpcCmdRequestLocName */
   HandleOther,               /* MrIpcCmdRequestLocInfo */
   HandleFileRequest,         /* MrIpcCmdRequestFile */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgHeader */
   HandleGetLokCfgHeaderProxy,/* MrIpcCmdCfgZHeader */
   HandleOther,               /* MrIpcCmdCfgData */
   HandleCanBootldr,          /* MrIpcCmdCanBootldrGeb */
   HandleStatusRequest,       /* MrIpcCmdStatusRequest */
   HandleOther,               /* MrIpcCmdStatusSize */
   HandleOther,               /* MrIpcCmdStatusData */
   HandleOther,               /* MrIpcCmdStatusResponse */
   HandleOther,               /* MrIpcCmdRequestMesswert */
   HandleOther,               /* MrIpcCmdSetConfig */
   HandleOther,               /* MrIpcCmdConfigResponse */
   HandleMesswertResponse,    /* MrIpcCmdMesswertResponse */
   HandleOther,               /* MrIpcCmdRequestLokListe */
   HandleOther,               /* MrIpcCmdIntern */
};

static SignalFunctionsType StateMachineFunctionsProxy[NUM_STATES] = {
   StateProxyWaitForMs2,          /* STATE_WAIT_FOR_MS2 */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKNAME_CFG_HDR */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKNAME_CFG_DATA */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKINFO_CFG_HDR */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKINFO_CFG_DATA */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_CFG_DATA */
   StateProxyWaitCs,              /* STATE_WAIT_CS2 */
   StateProxyNormal,              /* STATE_NORMAL */
   StateProxyGetGetLokCs2CfgData, /* STATE_GET_LOK_CS2_CFG_DATA */
   StateProxyWaitLokCs2CfgHdr,    /* STATE_WAIT_LOK_CS2_CFG_HDR */
   StateProxyWaitLokCs2CfgData,   /* STATE_WAIT_LOK_CS2_CFG_DATA */
   StateProxyWaitMagCs2CfgHdr,    /* STATE_WAIT_MAG_CS2_CFG_HDR */
   StateProxyWaitMagCs2CfgData,   /* STATE_WAIT_MAG_CS2_CFG_DATA */
   StateProxyWaitFsCs2CfgHdr,     /* STATE_WAIT_FS_CS2_CFG_HDR */
   StateProxyWaitFsCs2CfgData,    /* STATE_WAIT_FS_CS2_CFG_DATA */
   StateProxyWaitGbsCs2CfgHdr,    /* STATE_WAIT_GBS_CS2_CFG_HDR */
   StateProxyWaitGbsCs2CfgData,   /* STATE_WAIT_GBS_CS2_CFG_DATA */
   StateProxyWaitGpgCs2CfgHdr,    /* STATE_WAIT_GPG_CS2_CFG_HDR */
   StateProxyWaitGpgCs2CfgData,   /* STATE_WAIT_GPG_CS2_CFG_DATA */
   StateProxyWaitLokCvrCs2CfgHdr, /* STATE_WAIT_LOK_CVR_CFG_HDR */
   StateProxyWaitLokCvrCs2CfgData,/* STATE_WAIT_LOK_CVR_CFG_DATA */
   StateProxyWaitMagCvrCs2CfgHdr, /* STATE_WAIT_MAG_CVR_CFG_HDR */
   StateProxyWaitMagCvrCs2CfgData,/* STATE_WAIT_MAG_CVR_CFG_DATA */
   StateProxyWaitGbsCvrCs2CfgHdr, /* STATE_WAIT_GBS_CVR_CFG_HDR */
   StateProxyWaitGbsCvrCs2CfgData,/* STATE_WAIT_GBS_CVR_CFG_DATA */
   StateProxyWaitFsCvrCs2CfgHdr,  /* STATE_WAIT_FS_CVR_CFG_HDR */
   StateProxyWaitFsCvrCs2CfgData, /* STATE_WAIT_FS_CVR_CFG_DATA */
   StateProxyWaitCfgDataHdr,      /* STATE_WAIT_CFG_DATA_HDR */
   StateProxyWaitCfgDataData,     /* STATE_WAIT_CFG_DATA_DATA */
   StateProxyWaitSysCfgMesswert,  /* STATE_WAIT_SYS_CFG_MESSWERT */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_MS2_LOKNAME_CFG_HDR */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_MS2_LOKNAME_CFG_DATA */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_MS2_LOKINFO_CFG_HDR */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_MS2_LOKINFO_CFG_DATA */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKLISTE_CFG_HDR */
   (SignalFunctionsType)NULL,     /* STATE_WAIT_LOKLISTE_CFG_DATA */
};
