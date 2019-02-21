#include <time.h>
#include <scanner.h>
#include <fsm.h>
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

#define V7_ERROR_NOT_SUPPORTED  -1
#define V7_ERROR_NO_INFORMATION -2
#define V7_ERROR_TIMEOUT        -3

#define ANTWORT_HANDSHAKE_GO_OK       "200 OK GO %d"
#define ANTWORT_HANDSHAKE_PROTO_OK    "201 OK PROTOCOL SRCP"
#define ANTWORT_HANDSHAKE_CONMODE_OK  "202 OK CONNECTIONMODE"
#define ANTWORT_HANDSHAKE_PROTO_ERR   "400 ERROR unsupported protocol"
#define ANTWORT_HANDSHAKE_CONMODE_ERR "401 ERROR unsupported connection mode"
#define ANTWORT_HANDSHAKE_INSUFFD_ERR "402 ERROR insufficient data"
#define ANTWORT_HANDSHAKE_NORES_ERR   "500 ERROR out of resources"
#define ANTWORT_V7_INFO "INFO %d"
#define ANTWORT_COMMAND_INFO_OK     "100 INFO %s"
#define ANTWORT_COMMAND_INFOINIT_OK "101 INFO %s"
#define ANTWORT_COMMAND_INFOTERM_OK "102 INFO %s"
#define ANTWORT_COMMAND_OK          "200 OK"
#define ANTWORT_COMMAND_UNKNOWN_ERR "410 ERROR unknown command"
#define ANTWORT_COMMAND_UNVAL_ERR   "411 ERROR unknown value"
#define ANTWORT_COMMAND_WRVAL_ERR   "412 ERROR wrong value"
#define ANTWORT_COMMAND_DEVLOCK     "414 ERROR device locked"
#define ANTWORT_COMMAND_FORBID_ERR  "415 ERROR forbidden"
#define ANTWORT_COMMAND_NODATA_ERR  "416 ERROR no data"
#define ANTWORT_COMMAND_TIMEOUT     "417 ERROR timeout"
#define ANTWORT_COMMAND_TOLONG_ERR  "418 ERROR list too long"
#define ANTWORT_COMMAND_TOSHORT_ERR "419 ERROR list too short"
#define ANTWORT_COMMAND_UNPROT_ERR  "420 ERROR unsupported device protocol"
#define ANTWORT_COMMAND_UNDEV_ERR   "421 ERROR unsupported device"
#define ANTWORT_COMMAND_UNGRP_ERR   "422 ERROR unsupported device group"
#define ANTWORT_COMMAND_UNOP_ERR    "423 ERROR unsupported operation"
#define ANTWORT_COMMAND_REINIT_ERR  "424 ERROR device reinitialized"
#define ANTWORT_COMMAND_UNSUPP_ERR  "425 ERROR not supported"
#define ANTWORT_COMMAND_ERR         "499 ERROR unspecified error"

typedef struct {
   ClientInfo *Client;
   MrCs2CanDataType *CanMsg;
   BOOL HaveAnswer;
} SignalParam;

#define SignalDataSetClient(Data, Clnt)     (Data)->Client=Clnt
#define SignalDataSetCanMsg(Data, Msg)      (Data)->CanMsg=Msg
#define SignalDataSetHaveAnswer(Data, Answ) (Data)->HaveAnswer=Answ

#define SignalDataGetClient(Data)     (Data)->Client
#define SignalDataGetCanMsg(Data)     (Data)->CanMsg
#define SignalDataGetHaveAnswer(Data) (Data)->HaveAnswer

static int HandleSetHandshake(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_PROTOCOL:
         Token = SrcpParserParse(SrcpGetParser(Data));
         switch (Token)
         {
            case PARSER_SRCP:
               Token = SrcpParserParse(SrcpGetParser(Data));
               switch (Token)
               {
                  case PARSER_ZAHL:
                     if (ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data))) == 0)
                     {
                        Token = SrcpParserParse(SrcpGetParser(Data));
                        switch (Token)
                        {
                           case PARSER_PUNKT:
                              Token = SrcpParserParse(SrcpGetParser(Data));
                              switch (Token)
                              {
                                 case PARSER_ZAHL:
                                    if (ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data))) == 8)
                                    {
                                       SrcpSendAnswer(SignalDataGetClient(Signal),
                                                      ANTWORT_HANDSHAKE_PROTO_OK);
                                    }
                                    else
                                    {
                                       SrcpSendAnswer(SignalDataGetClient(Signal),
                                                      ANTWORT_HANDSHAKE_PROTO_ERR);
                                    }
                                    break;
                                 default:
                                    SrcpSendAnswer(SignalDataGetClient(Signal),
                                                   ANTWORT_HANDSHAKE_PROTO_ERR);
                                    break;
                              }
                              break;
                           default:
                              SrcpSendAnswer(SignalDataGetClient(Signal),
                                             ANTWORT_HANDSHAKE_PROTO_ERR);
                              break;
                        }
                     }
                     else
                     {
                        SrcpSendAnswer(SignalDataGetClient(Signal),
                                       ANTWORT_HANDSHAKE_PROTO_ERR);
                     }
                     break;
                  default:
                     SrcpSendAnswer(SignalDataGetClient(Signal),
                                    ANTWORT_HANDSHAKE_PROTO_ERR);
                     break;
               }
               break;
            default:
               SrcpSendAnswer(SignalDataGetClient(Signal),
                              ANTWORT_HANDSHAKE_PROTO_ERR);
               break;
         }
         break;
      case PARSER_CONNECTIONMODE:
         Token = SrcpParserParse(SrcpGetParser(Data));
         switch (Token)
         {
            case PARSER_SRCP:
               Token = SrcpParserParse(SrcpGetParser(Data));
               switch (Token)
               {
                  case PARSER_BEZEICHNER:
                     if (strcmp(ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))),
                                "INFO") == 0)
                     {
                        SrcpClientSetClientMode(SignalDataGetClient(Signal),
                                                SCRP_CLIENT_MODE_INFO);
                        SrcpSendAnswer(SignalDataGetClient(Signal),
                                       ANTWORT_HANDSHAKE_CONMODE_OK);
                     }
                     else if (strcmp(ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))),
                                     "COMMAND") == 0)
                     {
                        SrcpClientSetClientMode(SignalDataGetClient(Signal),
                                                SCRP_CLIENT_MODE_COMMAND);
                        SrcpSendAnswer(SignalDataGetClient(Signal),
                                       ANTWORT_HANDSHAKE_CONMODE_OK);
                     }
                     break;
                  default:
                     SrcpSendAnswer(SignalDataGetClient(Signal),
                                    ANTWORT_HANDSHAKE_CONMODE_ERR);
                     break;
               }
               break;
            default:
               break;
         }
         break;
      default:
         if (SrcpGetVerbose(Data))
            printf("Parser: token 0x%x\n", Token);
         break;
   }
   if (SrcpGetVerbose(Data))
      printf("FSM: new state %d\n",STATE_NO_CHANGE);
   return(STATE_NO_CHANGE);
}

static int HandleGoHandshake(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   char Antwort[15];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   sprintf(Antwort, ANTWORT_HANDSHAKE_GO_OK,
           SrcpClientGetClientSock(SignalDataGetClient(Signal)));
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
   int Token, Addr, JulDay, Hour, Minute, Second, Fx, Fy, Value, Port, Delay,
       Direction, V, VMax, f[16], NumF, i;
   char Protocol[5];
   MrCs2CanDataType *SaveCanMsg;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse set command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Direction = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         V = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         VMax = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         Token = SrcpParserParse(SrcpGetParser(Data));
         f[0] = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         Token = SrcpParserParse(SrcpGetParser(Data));
         NumF = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         for (i=1; i<=NumF; i++)
         {
            Token = SrcpParserParse(SrcpGetParser(Data));
            f[i] = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         }
         MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
         if (VMax == 0)
         {
            MrCs2EncSpeed6(SignalDataGetCanMsg(Signal), Addr, V);
         }
         else
         {
            MrCs2EncSpeed6(SignalDataGetCanMsg(Signal), Addr, V * 1000 / VMax);
         }
         SaveCanMsg = SrcpGetPendingCanMsg(Data, 0);
         SrcpSetNumPending(Data, 1);
         MrCs2SetId(SaveCanMsg, MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SaveCanMsg, TRUE);
         MrCs2SetHash(SaveCanMsg, MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SaveCanMsg, 0);
         MrCs2SetPrio(SaveCanMsg, MR_CS2_PRIO_1);
         MrCs2EncDirection5(SaveCanMsg, Addr, Direction);
         for (i=0; i<=NumF; i++)
         {
            SaveCanMsg = SrcpGetPendingCanMsg(Data, i + 1);
            SrcpSetNumPending(Data, i + 2);
            MrCs2SetId(SaveCanMsg, MR_CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SaveCanMsg, TRUE);
            MrCs2SetHash(SaveCanMsg, MrCs2CalcHash(MR_CS2_UID_BROADCAST));
            MrCs2SetResponse(SaveCanMsg, 0);
            MrCs2SetPrio(SaveCanMsg, MR_CS2_PRIO_1);
            MrCs2EncFunction6(SaveCanMsg, Addr, i, f[i]);
         }
         SignalDataSetHaveAnswer(Signal, TRUE);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Port = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Delay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
         MrCs2EncAccSwitch8(SignalDataGetCanMsg(Signal), Addr, Value,
                            0, Delay);
         SignalDataSetHaveAnswer(Signal, TRUE);
         break;
      case PARSER_TIME:
         SrcpParserParse(SrcpGetParser(Data));
         JulDay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Hour = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Minute = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Second = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Fx = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Fy = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         break;
      case PARSER_POWER:
         Token = SrcpParserParse(SrcpGetParser(Data));
         switch (Token)
         {
            case PARSER_ON:
               MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
               MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
               MrCs2SetHash(SignalDataGetCanMsg(Signal),
                            MrCs2CalcHash(MR_CS2_UID_BROADCAST));
               MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
               MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
               MrCs2EncSysGo(SignalDataGetCanMsg(Signal),
                             MR_CS2_UID_BROADCAST);
               SignalDataSetHaveAnswer(Signal, TRUE);
               break;
            case PARSER_OFF:
               MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
               MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
               MrCs2SetHash(SignalDataGetCanMsg(Signal),
                            MrCs2CalcHash(MR_CS2_UID_BROADCAST));
               MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
               MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
               MrCs2EncSysStop(SignalDataGetCanMsg(Signal),
                               MR_CS2_UID_BROADCAST);
               SignalDataSetHaveAnswer(Signal, TRUE);
               break;
            default:
               break;
         }
         break;
      case PARSER_FB:
         break;
      default:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleGetV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Addr, Port;
   char Protocol[5], ModuleType[5], Antwort[100], Params[100];
   struct tm Now;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
      puts("parse get command");
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Port = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_TIME:
         mktime(&Now);
         sprintf(Params, "INFO TIME %d %d %d %d 0 0",
                 Now.tm_mday, Now.tm_hour, Now.tm_min, Now.tm_sec);
         sprintf(Antwort, ANTWORT_COMMAND_INFO_OK, Params);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_POWER:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(ModuleType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      default:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWaitV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Addr, Value, Timeout, JulDay, Hour, Minute, Second;
   char ModuleType[5], Antwort[100];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse wait command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_GA:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_TIME:
         SrcpParserParse(SrcpGetParser(Data));
         JulDay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Hour = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Minute = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Second = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_POWER:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(ModuleType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Timeout = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      default:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleInitV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, JulDay, Hour, Minute, Second, Fx, Fy;
   char ModuleType[5];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse init command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         break;
      case PARSER_GA:
         break;
      case PARSER_TIME:
         SrcpParserParse(SrcpGetParser(Data));
         JulDay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Hour = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Minute = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Second = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Fx = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Fy = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         break;
      case PARSER_POWER:
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(ModuleType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         break;
      default:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleTermV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token;
   char ModuleType[5];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse term command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         break;
      case PARSER_GA:
         break;
      case PARSER_TIME:
         break;
      case PARSER_POWER:
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(ModuleType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleVerifyV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, DestAddr, Value;
   char Protocol[10], DestType[5];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         break;
      case PARSER_TIME:
         break;
      case PARSER_POWER:
         break;
      case PARSER_FB:
         break;
      default:
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWriteV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, DestAddr, Value;
   char Protocol[10], DestType[5], Antwort[100];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_TIME:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_POWER:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_FB:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      default:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleReadV7(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, DestAddr;
   char Protocol[10], DestType[5], Antwort[100];

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(Protocol,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         strcpy(DestType,
                ScanGetString(SrcpParserGetScanner(SrcpGetParser(Data))));
         SrcpParserParse(SrcpGetParser(Data));
         DestAddr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_TIME:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_POWER:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_FB:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      default:
         sprintf(Antwort, ANTWORT_V7_INFO, V7_ERROR_NOT_SUPPORTED);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleSetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, DeviceGroup, Addr, Duration, JulDay, Hour, Minute, Second,
       SendTo, ReplyTo, Value, Port, Delay, DriveMode, V, VMax, f[16], NumF, i;
   MrCs2CanDataType *SaveCanMsg;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse set command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         DriveMode = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         V = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         VMax = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         NumF = 0;
         Token = SrcpParserParse(SrcpGetParser(Data));
         while (Token == PARSER_ZAHL && NumF<16)
         {
            f[NumF] = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
            Token = SrcpParserParse(SrcpGetParser(Data));
         }
         MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
         if (VMax == 0)
         {
            MrCs2EncSpeed6(SignalDataGetCanMsg(Signal), Addr, V);
         }
         else
         {
            MrCs2EncSpeed6(SignalDataGetCanMsg(Signal), Addr, V * 1000 / VMax);
         }
         SaveCanMsg = SrcpGetPendingCanMsg(Data, 0);
         SrcpSetNumPending(Data, 1);
         MrCs2SetId(SaveCanMsg, MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SaveCanMsg, TRUE);
         MrCs2SetHash(SaveCanMsg, MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SaveCanMsg, 0);
         MrCs2SetPrio(SaveCanMsg, MR_CS2_PRIO_1);
         MrCs2EncDirection5(SaveCanMsg, Addr, DriveMode);
         for (i=0; i<NumF; i++)
         {
            SaveCanMsg = SrcpGetPendingCanMsg(Data, i + 1);
            SrcpSetNumPending(Data, i + 2);
            MrCs2SetId(SaveCanMsg, MR_CS2_UID_BROADCAST);
            MrCs2SetIsCs2(SaveCanMsg, TRUE);
            MrCs2SetHash(SaveCanMsg, MrCs2CalcHash(MR_CS2_UID_BROADCAST));
            MrCs2SetResponse(SaveCanMsg, 0);
            MrCs2SetPrio(SaveCanMsg, MR_CS2_PRIO_1);
            MrCs2EncFunction6(SaveCanMsg, Addr, i, f[i]);
         }
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Port = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Delay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
         MrCs2EncAccSwitch8(SignalDataGetCanMsg(Signal), Addr, Value,
                            0, Delay);
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_GM:
         SrcpParserParse(SrcpGetParser(Data));
         SendTo = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         ReplyTo = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNSUPP_ERR);
         break;
      case PARSER_TIME:
         SrcpParserParse(SrcpGetParser(Data));
         JulDay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Hour = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Minute = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Second = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_LOCK:
         SrcpParserParse(SrcpGetParser(Data));
         DeviceGroup = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Duration = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_POWER:
         Token = SrcpParserParse(SrcpGetParser(Data));
         switch (Token)
         {
            case PARSER_ON:
               MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
               MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
               MrCs2SetHash(SignalDataGetCanMsg(Signal),
                            MrCs2CalcHash(MR_CS2_UID_BROADCAST));
               MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
               MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
               MrCs2EncSysGo(SignalDataGetCanMsg(Signal),
                             MR_CS2_UID_BROADCAST);
               SignalDataSetHaveAnswer(Signal, TRUE);
               SrcpSendAnswer(SignalDataGetClient(Signal),
                              ANTWORT_COMMAND_OK);
               break;
            case PARSER_OFF:
               MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
               MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
               MrCs2SetHash(SignalDataGetCanMsg(Signal),
                            MrCs2CalcHash(MR_CS2_UID_BROADCAST));
               MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
               MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
               MrCs2EncSysStop(SignalDataGetCanMsg(Signal),
                               MR_CS2_UID_BROADCAST);
               SignalDataSetHaveAnswer(Signal, TRUE);
               SrcpSendAnswer(SignalDataGetClient(Signal),
                              ANTWORT_COMMAND_OK);
               break;
            default:
               SrcpSendAnswer(SignalDataGetClient(Signal),
                              ANTWORT_COMMAND_UNKNOWN_ERR);
               break;
         }
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleGetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, Addr, Port, DecoderAddress, SessionId;
   char Antwort[100], Params[100];
   struct tm Now;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
      puts("parse get command");
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GA:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Port = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_NODATA_ERR);
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_NODATA_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SM:
         SrcpParserParse(SrcpGetParser(Data));
         DecoderAddress = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_TIME:
         mktime(&Now);
         sprintf(Params, "0 TIME %d %d %d %d",
                 Now.tm_mday, Now.tm_hour, Now.tm_min, Now.tm_sec);
         sprintf(Antwort, ANTWORT_COMMAND_INFO_OK, Params);
         SrcpSendAnswer(SignalDataGetClient(Signal), Antwort);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SESSION:
         SrcpParserParse(SrcpGetParser(Data));
         SessionId = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleCheckV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, DecoderAddress, Type;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse check command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_FB:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_TIME:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleWaitV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, Addr, Value, Timeout, JulDay, Hour, Minute, Second;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse wait command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_FB:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Value = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Timeout = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_TIME:
         SrcpParserParse(SrcpGetParser(Data));
         JulDay = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Hour = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Minute = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Second = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleInitV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse init command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_FB:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_TIME:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleTermV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, Addr, DeviceGroup;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse term command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_FB:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_TIME:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_LOCK:
         SrcpParserParse(SrcpGetParser(Data));
         DeviceGroup = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpParserParse(SrcpGetParser(Data));
         Addr = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_POWER:
         MrCs2SetId(SignalDataGetCanMsg(Signal), MR_CS2_UID_BROADCAST);
         MrCs2SetIsCs2(SignalDataGetCanMsg(Signal), TRUE);
         MrCs2SetHash(SignalDataGetCanMsg(Signal),
                      MrCs2CalcHash(MR_CS2_UID_BROADCAST));
         MrCs2SetResponse(SignalDataGetCanMsg(Signal), 0);
         MrCs2SetPrio(SignalDataGetCanMsg(Signal), MR_CS2_PRIO_1);
         MrCs2EncSysStop(SignalDataGetCanMsg(Signal),
                         MR_CS2_UID_BROADCAST);
         SignalDataSetHaveAnswer(Signal, TRUE);
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleVerifyV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus, DecoderAddress;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse verify command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
   Token = SrcpParserParse(SrcpGetParser(Data));
   switch (Token)
   {
      case PARSER_GL:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_FB:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SM:
         SrcpParserParse(SrcpGetParser(Data));
         DecoderAddress = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
         SrcpSendAnswer(SignalDataGetClient(Signal), ANTWORT_COMMAND_OK);
         break;
      case PARSER_TIME:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
         break;
   }
   return(STATE_NO_CHANGE);
}

static int HandleResetV8(void *Priv, void *SignalData)
{  SrcpStruct *Data;
   SignalParam *Signal;
   int Token, Bus;

   Data = (SrcpStruct *)Priv;
   Signal = (SignalParam *)SignalData;
   if (SrcpGetVerbose(Data))
   {
      puts("parse reset command");
   }
   Token = SrcpParserParse(SrcpGetParser(Data));
   if (Token == PARSER_ZAHL)
   {
      Bus = ScanGanz(SrcpParserGetScanner(SrcpGetParser(Data)));
      Token = SrcpParserParse(SrcpGetParser(Data));
   }
   else
   {
      Bus = 0;
   }
   switch (Token)
   {
      case PARSER_GL:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GA:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_FB:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_GM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SM:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_TIME:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_LOCK:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_POWER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SERVER:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_SESSION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      case PARSER_DESCRIPTION:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNOP_ERR);
         break;
      default:
         SrcpSendAnswer(SignalDataGetClient(Signal),
                        ANTWORT_COMMAND_UNKNOWN_ERR);
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
{  int Signal, Token;
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
   SrcpParserInit(SrcpGetParser(Data), Line, Length);
   Token = SrcpParserParse(SrcpGetParser(Data));
   if (SrcpGetVerbose(Data))
      printf("Befehl (Token) 0x%x\n", Token);
   switch (Token)
   {
      case PARSER_EOF:
         Signal = SIGNAL_INVALID;
         break;
      case PARSER_SET:
         Signal = SIGNAL_SET;
         break;
      case PARSER_GET:
         Signal = SIGNAL_GET;
         break;
      case PARSER_WAIT:
         Signal = SIGNAL_WAIT;
         break;
      case PARSER_INIT:
         Signal = SIGNAL_INIT;
         break;
      case PARSER_TERM:
         Signal = SIGNAL_TERM;
         break;
      case PARSER_VERIFY:
         Signal = SIGNAL_VERIFY;
         break;
      case PARSER_RESET:
         Signal = SIGNAL_RESET;
         break;
      case PARSER_GO:
         Signal = SIGNAL_GO;
         break;
      case PARSER_ERROR:
         Signal = SIGNAL_INVALID;
         break;
      case PARSER_WRITE:
         Signal = SIGNAL_WRITE;
         break;
      case PARSER_READ:
         Signal = SIGNAL_READ;
         break;
   }
   if (Signal != SIGNAL_INVALID)
   {
      FsmDo(SrcpClientGetStateMachine(Client), Signal, (void *)&SignalData);
   }
   SrcpParserExit(SrcpGetParser(Data));
   return(SignalDataGetHaveAnswer(&SignalData));
}
