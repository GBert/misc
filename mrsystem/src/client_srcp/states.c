#include <time.h>
#include <scanner.h>
#include <fsm.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include <srcp.h>
#include "srcp.h"

#define NUM_STATES 3
#define STATE_HANDSHAKE        0
#define STATE_COMMAND_MODE     1
#define STATE_INFORMATION_MODE 2

#define NUM_SIGNALS 11
#define SIGNAL_INVALID -1
#define SIGNAL_SET     0
#define SIGNAL_GET     1
#define SIGNAL_CHECK   2
#define SIGNAL_WAIT    3
#define SIGNAL_INIT    4
#define SIGNAL_TERM    5
#define SIGNAL_VERIFY  6
#define SIGNAL_RESET   7
#define SIGNAL_GO      8
#define SIGNAL_WRITE   9
#define SIGNAL_READ   10

typedef struct {
   ClientInfo *Client;
   MrCs2CanDataType *CanMsg;
   BOOL HaveAnswer;
   char *Line;
   int Length;
} SignalParam;

#define SignalDataSetClient(Data, Clnt)     (Data)->Client=Clnt
#define SignalDataSetCanMsg(Data, Msg)      (Data)->CanMsg=Msg
#define SignalDataSetHaveAnswer(Data, Answ) (Data)->HaveAnswer=Answ
#define SignalDataSetLine(Data, Ln)         (Data)->Line=Ln
#define SignalDataSetLength(Data, Lng)      (Data)->Length=Lng

#define SignalDataGetClient(Data)     (Data)->Client
#define SignalDataGetCanMsg(Data)     (Data)->CanMsg
#define SignalDataGetHaveAnswer(Data) (Data)->HaveAnswer
#define SignalDataGetLine(Data)       (Data)->Line
#define SignalDataGetLength(Data)     (Data)->Length

static int HandleSetHandshake(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   switch (SrcpDecHandshake(SignalDataGetLine(Signal),
                            SignalDataGetLength(Signal)))
   {
      case SRCP_HANDSHAKE_OK:
         SrcpEncAnswerHandshakeProtoOk(Antwort, (char *)NULL);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_HANDSHAKE_MODE_INFO:
         SrcpClientSetClientMode(SignalDataGetClient(Signal),
                                 SCRP_CLIENT_MODE_INFO);
         SrcpEncAnswerHandshakeConmodeOk(Antwort, (char *)NULL);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_HANDSHAKE_MODE_COMMAND:
         SrcpClientSetClientMode(SignalDataGetClient(Signal),
                                 SCRP_CLIENT_MODE_COMMAND);
         SrcpEncAnswerHandshakeConmodeOk(Antwort, (char *)NULL);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_HANDSHAKE_ERROR:
         SrcpEncAnswerHandshakeProtoErr(Antwort, (char *)NULL);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   if (SrcpGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleGoHandshake(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100], Param[100];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   sprintf(Param, "%d", SrcpClientGetClientSock(SignalDataGetClient(Signal)));
   SrcpEncAnswerHandshakeGoOk(Antwort, Param);
   SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
   if (SrcpClientGetClientMode(SignalDataGetClient(Signal)) == SCRP_CLIENT_MODE_INFO)
   {
      return(STATE_INFORMATION_MODE);
   }
   else
   {
      return(STATE_COMMAND_MODE);
   }
}

static int HandleSetV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int i;
   MrCs2CanDataType *SaveCanMsg;
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse set command");
   }
   switch (SrcpV7DecSet(SignalDataGetLine(Signal),
                        SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
         if (Params.TargetParms.GlParams.VMax == 0)
         {
            Cs2EncSpeed6(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                         Params.TargetParms.GlParams.Addr,
                         Params.TargetParms.GlParams.V);
         }
         else
         {
            Cs2EncSpeed6(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                         Params.TargetParms.GlParams.Addr,
                         Params.TargetParms.GlParams.V * 1000 / Params.TargetParms.GlParams.VMax);
         }
         SaveCanMsg = SrcpGetPendingCanMsg(Data, 0);
         SrcpSetNumPending(Data, 1);
         MrCs2SetId(SaveCanMsg, CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SaveCanMsg, TRUE);
         MrCs2SetHash(SaveCanMsg, Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SaveCanMsg, 0);
         MrCs2SetPrio(SaveCanMsg, CS2_PRIO_1);
         Cs2EncDirection5(MrCs2GetData(SaveCanMsg),
                          Params.TargetParms.GlParams.Addr,
                          Params.TargetParms.GlParams.Direction);
         for (i=0; i<=Params.TargetParms.GlParams.NumF; i++)
         {
            SaveCanMsg = SrcpGetPendingCanMsg(Data, i + 1);
            SrcpSetNumPending(Data, i + 2);
            MrCs2SetId(SaveCanMsg, CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SaveCanMsg, TRUE);
            MrCs2SetHash(SaveCanMsg, Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SaveCanMsg, 0);
            MrCs2SetPrio(SaveCanMsg, CS2_PRIO_1);
            Cs2EncFunction6(MrCs2GetData(SaveCanMsg),
                            Params.TargetParms.GlParams.Addr, i,
                            Params.TargetParms.GlParams.f[i]);
         }
         SignalDataSetHaveAnswer(Signal, TRUE);
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
         Cs2EncAccSwitch8(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                          Params.TargetParms.GaParams.Addr,
                          Params.TargetParms.GaParams.Value, 0,
                          Params.TargetParms.GaParams.Delay);
         SignalDataSetHaveAnswer(Signal, TRUE);
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         break;
      case SRCP_V7_TARGET_TIME:
         break;
      case SRCP_V7_TARGET_POWER:
         if (Params.TargetParms.PowerParams.PowerOn)
         {
            MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
            MrCs2SetHash(SignalDataGetCanMsg(Signal),
                         Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
            MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
            Cs2EncSysGo(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                        CS2_UID_BROADCAST);
            SignalDataSetHaveAnswer(Signal, TRUE);
         }
         else
         {
            MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
            MrCs2SetHash(SignalDataGetCanMsg(Signal),
                         Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
            MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
            Cs2EncSysStop(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                          CS2_UID_BROADCAST);
            SignalDataSetHaveAnswer(Signal, TRUE);
         }
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleGetV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100], AntwortParams[100];
   struct tm Now;
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
      puts("parse get command");
   switch (SrcpV7DecGet(SignalDataGetLine(Signal),
                        SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
      case SRCP_V7_TARGET_TIME:
         mktime(&Now);
         sprintf(AntwortParams, "INFO TIME %d %d %d %d 0 0",
                 Now.tm_mday, Now.tm_hour, Now.tm_min, Now.tm_sec);
         SrcpEncAnswerCommandInfoOk(Antwort, AntwortParams);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_POWER:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWaitV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse wait command");
   }
   switch (SrcpV7DecWait(SignalDataGetLine(Signal),
                        SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_TIME:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_POWER:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleInitV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse init command");
   }
   switch (SrcpV7DecInit(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
      case SRCP_V7_TARGET_FB_FEEDBACK:
      case SRCP_V7_TARGET_TIME:
      case SRCP_V7_TARGET_POWER:
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleTermV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse term command");
   }
   switch (SrcpV7DecTerm(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         break;
      case SRCP_V7_TARGET_TIME:
         break;
      case SRCP_V7_TARGET_POWER:
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleVerifyV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   switch (SrcpV7DecVerify(SignalDataGetLine(Signal),
                           SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         break;
      case SRCP_V7_TARGET_TIME:
         break;
      case SRCP_V7_TARGET_POWER:
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWriteV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   switch (SrcpV7DecWrite(SignalDataGetLine(Signal),
                          SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_TIME:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_POWER:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleReadV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV7ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   switch (SrcpV7DecRead(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V7_TARGET_GL_GENERIC_LOCO:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_GA_GENERIC_ACCESSORY:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_FB_FEEDBACK:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_TIME:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_POWER:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V7_TARGET_UNKNOWN:
      case SRCP_V7_TARGET_ERROR:
         SrcpV7EncAnswer(Antwort, SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleSetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int i;
   MrCs2CanDataType *SaveCanMsg;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse set command");
   }
   switch (SrcpV8DecSet(SignalDataGetLine(Signal),
                        SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
         if (Params.DeviceGroupParms.GlParams.VMax == 0)
         {
            Cs2EncSpeed6(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                         Params.DeviceGroupParms.GlParams.Addr,
                         Params.DeviceGroupParms.GlParams.V);
         }
         else
         {
            Cs2EncSpeed6(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                         Params.DeviceGroupParms.GlParams.Addr,
                         Params.DeviceGroupParms.GlParams.V * 1000 / Params.DeviceGroupParms.GlParams.VMax);
         }
         SaveCanMsg = SrcpGetPendingCanMsg(Data, 0);
         SrcpSetNumPending(Data, 1);
         MrCs2SetId(SaveCanMsg, CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SaveCanMsg, TRUE);
         MrCs2SetHash(SaveCanMsg, Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SaveCanMsg, 0);
         MrCs2SetPrio(SaveCanMsg, CS2_PRIO_1);
         Cs2EncDirection5(MrCs2GetData(SaveCanMsg),
                          Params.DeviceGroupParms.GlParams.Addr,
                          Params.DeviceGroupParms.GlParams.DriveMode);
         for (i = 0; i < Params.DeviceGroupParms.GlParams.NumF; i++)
         {
            SaveCanMsg = SrcpGetPendingCanMsg(Data, i + 1);
            SrcpSetNumPending(Data, i + 2);
            MrCs2SetId(SaveCanMsg, CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SaveCanMsg, TRUE);
            MrCs2SetHash(SaveCanMsg, Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SaveCanMsg, 0);
            MrCs2SetPrio(SaveCanMsg, CS2_PRIO_1);
            Cs2EncFunction6(MrCs2GetData(SaveCanMsg),
                            Params.DeviceGroupParms.GlParams.Addr, i,
                            Params.DeviceGroupParms.GlParams.f[i]);
         }
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
         Cs2EncAccSwitch8(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                          Params.DeviceGroupParms.GaParams.Addr,
                          Params.DeviceGroupParms.GaParams.Value, 0,
                          Params.DeviceGroupParms.GaParams.Delay);
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         if (Params.DeviceGroupParms.PowerParams.PowerOn)
         {
            MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
            MrCs2SetHash(SignalDataGetCanMsg(Signal),
                         Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
            MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
            Cs2EncSysGo(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                        CS2_UID_BROADCAST);
            SignalDataSetHaveAnswer(Signal, TRUE);
            SrcpEncAnswerCommandOk(Antwort);
            SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         }
         else
         {
            MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
            MrCs2SetHash(SignalDataGetCanMsg(Signal),
                         Cs2CalcHash(CS2_UID_BROADCAST));
            MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
            MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
            Cs2EncSysStop(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                          CS2_UID_BROADCAST);
            SignalDataSetHaveAnswer(Signal, TRUE);
            SrcpEncAnswerCommandOk(Antwort);
            SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         }
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnsuppErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleGetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100], AntwortParams[100];
   struct tm Now;
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
      puts("parse get command");
   switch (SrcpV8DecGet(SignalDataGetLine(Signal),
                        SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandNodataErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandNodataErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         mktime(&Now);
         sprintf(AntwortParams, "0 TIME %d %d %d %d",
                 Now.tm_mday, Now.tm_hour, Now.tm_min, Now.tm_sec);
         SrcpEncAnswerCommandInfoOk(Antwort, AntwortParams);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleCheckV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse check command");
   }
   switch (SrcpV8DecCheck(SignalDataGetLine(Signal),
                          SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWaitV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse wait command");
   }
   switch (SrcpV8DecWait(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleInitV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse init command");
   }
   switch (SrcpV8DecInit(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleTermV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse term command");
   }
   switch (SrcpV8DecTerm(SignalDataGetLine(Signal),
                         SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         MrCs2SetId(SignalDataGetCanMsg(Signal), CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      Cs2CalcHash(CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), CS2_PRIO_1);
         Cs2EncSysStop(MrCs2GetData(SignalDataGetCanMsg(Signal)),
                       CS2_UID_BROADCAST);
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleVerifyV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   switch (SrcpV8DecVerify(SignalDataGetLine(Signal),
                           SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandOk(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleResetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[100];
   SrcpV8ParamType Params;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse reset command");
   }
   switch (SrcpV8DecReset(SignalDataGetLine(Signal),
                          SignalDataGetLength(Signal), &Params))
   {
      case SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_TIME:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_POWER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_LOCK:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SERVER:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SESSION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_DESCRIPTION:
         SrcpEncAnswerCommandUnopErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case SRCP_V8_DEVICE_GROUP_UNKNOWN:
      case SRCP_V8_DEVICE_GROUP_ERROR:
         SrcpEncAnswerCommandUnknownErr(Antwort);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleOther(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *CmdFrame;

   Data = (SrcpStruct *)Priv;
   CmdFrame = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}


static StateFktType StateHandshake[NUM_SIGNALS] = {
   HandleSetHandshake, /* SIGNAL_SET */
   HandleOther,        /* SIGNAL_GET */
   HandleOther,        /* SIGNAL_CHECK */
   HandleOther,        /* SIGNAL_WAIT */
   HandleOther,        /* SIGNAL_INIT */
   HandleOther,        /* SIGNAL_TERM */
   HandleOther,        /* SIGNAL_VERIFY */
   HandleOther,        /* SIGNAL_RESET */
   HandleGoHandshake,  /* SIGNAL_GO */
   HandleOther,        /* SIGNAL_WRITE */
   HandleOther,        /* SIGNAL_READ */
};

static StateFktType StateCommandModeV7[NUM_SIGNALS] = {
   HandleSetV7,    /* SIGNAL_SET */
   HandleGetV7,    /* SIGNAL_GET */
   HandleOther,    /* SIGNAL_CHECK */
   HandleWaitV7,   /* SIGNAL_WAIT */
   HandleInitV7,   /* SIGNAL_INIT */
   HandleTermV7,   /* SIGNAL_TERM */
   HandleVerifyV7, /* SIGNAL_VERIFY */
   HandleOther,    /* SIGNAL_RESET */
   HandleOther,    /* SIGNAL_GO */
   HandleWriteV7,  /* SIGNAL_WRITE */
   HandleReadV7,   /* SIGNAL_READ */
};

static StateFktType StateCommandModeV8[NUM_SIGNALS] = {
   HandleSetV8,    /* SIGNAL_SET */
   HandleGetV8,    /* SIGNAL_GET */
   HandleCheckV8,  /* SIGNAL_CHECK */
   HandleWaitV8,   /* SIGNAL_WAIT */
   HandleInitV8,   /* SIGNAL_INIT */
   HandleTermV8,   /* SIGNAL_TERM */
   HandleVerifyV8, /* SIGNAL_VERIFY */
   HandleResetV8,  /* SIGNAL_RESET */
   HandleOther,    /* SIGNAL_GO */
   HandleOther,    /* SIGNAL_WRITE */
   HandleOther,    /* SIGNAL_READ */
};

static StateFktType StateInformationMode[NUM_SIGNALS] = {
   HandleOther, /* SIGNAL_SET */
   HandleOther, /* SIGNAL_GET */
   HandleOther, /* SIGNAL_CHECK */
   HandleOther, /* SIGNAL_WAIT */
   HandleOther, /* SIGNAL_INIT */
   HandleOther, /* SIGNAL_TERM */
   HandleOther, /* SIGNAL_VERIFY */
   HandleOther, /* SIGNAL_RESET */
   HandleOther, /* SIGNAL_GO */
   HandleOther, /* SIGNAL_WRITE */
   HandleOther, /* SIGNAL_READ */
};

static SignalFunctionsType StateMachineFunctions[NUM_STATES] = {
   StateHandshake,
   StateCommandModeV8,
   StateInformationMode,
};


void SrcpInitFsm(SrcpStruct *Data, ClientInfo *Client)
{
   FsmInit(SrcpClientGetStateMachine(Client), (void *)Data, STATE_HANDSHAKE,
           NUM_SIGNALS, NUM_STATES, StateMachineFunctions);
}

BOOL SrcpDoFsm(SrcpStruct *Data, ClientInfo *Client, char *Line, int Length,
               MrCs2CanDataType *CanMsg)
{  int Signal;
   SignalParam SignalData;
   time_t Now;

   Now = time(NULL);
   if ((FsmGetState(SrcpClientGetStateMachine(Client)) == STATE_HANDSHAKE) &&
       (Now - SrcpClientGetLastMsgTstamp(Client) > 1))
   {
      StateMachineFunctions[STATE_COMMAND_MODE] = StateCommandModeV7;
      FsmSetState(SrcpClientGetStateMachine(Client), STATE_COMMAND_MODE);
   }
   SignalDataSetClient(&SignalData, Client);
   SignalDataSetCanMsg(&SignalData, CanMsg);
   SignalDataSetHaveAnswer(&SignalData, FALSE);
   SignalDataSetLine(&SignalData, Line);
   SignalDataSetLength(&SignalData, Length);
   SrcpClientSetLastMsgTstamp(Client, Now);
   if (SrcpGetVerbose(Data))
   {
      printf("len %d >%s<\n", Length, Line);
      switch (FsmGetState(SrcpClientGetStateMachine(Client)))
      {
         case STATE_HANDSHAKE:
            puts("State Handshake");
            break;
         case STATE_COMMAND_MODE:
            puts("State Command Mode");
            break;
         case STATE_INFORMATION_MODE:
            puts("State Information Mode");
            break;
      }
   }
   switch (SrcpDecGetCmd(Line, Length))
   {
      case SRCP_CMD_ERROR:
         Signal = SIGNAL_INVALID;
         break;
      case SRCP_CMD_SET:
         Signal = SIGNAL_SET;
         break;
      case SRCP_CMD_GET:
         Signal = SIGNAL_GET;
         break;
      case SRCP_CMD_WAIT:
         Signal = SIGNAL_WAIT;
         break;
      case SRCP_CMD_INIT:
         Signal = SIGNAL_INIT;
         break;
      case SRCP_CMD_TERM:
         Signal = SIGNAL_TERM;
         break;
      case SRCP_CMD_VERIFY:
         Signal = SIGNAL_VERIFY;
         break;
      case SRCP_CMD_RESET:
         Signal = SIGNAL_RESET;
         break;
      case SRCP_CMD_GO:
         Signal = SIGNAL_GO;
         break;
      case SRCP_CMD_WRITE:
         Signal = SIGNAL_WRITE;
         break;
      case SRCP_CMD_READ:
         Signal = SIGNAL_READ;
         break;
   }
   if (Signal != SIGNAL_INVALID)
   {
      FsmDo(SrcpClientGetStateMachine(Client), Signal, (void *)&SignalData);
   }
   return(SignalDataGetHaveAnswer(&SignalData));
}
