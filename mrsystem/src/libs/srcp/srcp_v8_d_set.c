#include "srcp_parse.h"
#include "srcp.h"

int SrcpV8DecSet(char *Line, int Length, SrcpV8ParamType *Params)
{  int Token, Ret;

   if (SrcpParserInternalInit(Line, Length))
   {
      Ret = SRCP_V8_DEVICE_GROUP_ERROR;
      Token = SrcpParserInternalParse();
      Token = SrcpParserInternalParse();
      Params->Bus = SrcpParserInternalGetGanz();
      do {
         Token = SrcpParserInternalParse();
         switch (Token)
         {
            case PARSER_GL:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GlParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GlParams.DriveMode = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GlParams.V = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GlParams.VMax = SrcpParserInternalGetGanz();
               Params->DeviceGroupParms.GlParams.NumF = 0;
               Token = SrcpParserInternalParse();
               while ((Token == PARSER_ZAHL) &&
                      (Params->DeviceGroupParms.GlParams.NumF < 16))
               {
                  Params->DeviceGroupParms.GlParams.f[Params->DeviceGroupParms.GlParams.NumF] = SrcpParserInternalGetGanz();
                  Token = SrcpParserInternalParse();
                  Params->DeviceGroupParms.GlParams.NumF++;
               }
               Ret = SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO;
               break;
            case PARSER_GA:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Port = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Value = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Delay = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY;
               break;
            case PARSER_FB:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.FbParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.FbParams.Value = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR;
               break;
            case PARSER_GM:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GmParams.SendTo = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GmParams.ReplyTo = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE;
               break;
            case PARSER_SM:
               Ret = SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE;
               break;
            case PARSER_TIME:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.TimeParams.JulDay = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.TimeParams.Hour = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.TimeParams.Minute = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.TimeParams.Second = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_TIME;
               break;
            case PARSER_LOCK:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.LockParams.DeviceGroup = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.LockParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.LockParams.Duration = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_LOCK;
               break;
            case PARSER_POWER:
               Ret = SRCP_V8_DEVICE_GROUP_POWER;
               Token = SrcpParserInternalParse();
               switch (Token)
               {
                  case PARSER_ON:
                     Params->DeviceGroupParms.PowerParams.PowerOn = TRUE;
                     break;
                  case PARSER_OFF:
                     Params->DeviceGroupParms.PowerParams.PowerOn = FALSE;
                     break;
                  default:
                     Ret = SRCP_V8_DEVICE_GROUP_UNKNOWN;
                     break;
               }
               break;
            case PARSER_SERVER:
               Ret = SRCP_V8_DEVICE_GROUP_SERVER;
               break;
            case PARSER_SESSION:
               Ret = SRCP_V8_DEVICE_GROUP_SESSION;
               break;
            case PARSER_DESCRIPTION:
               Ret = SRCP_V8_DEVICE_GROUP_DESCRIPTION;
               break;
            default:
               Ret = SRCP_V8_DEVICE_GROUP_UNKNOWN;
               break;
         }
      } while (Token != PARSER_EOF);
      SrcpParserInternalExit();
   }
   else
   {
      Ret = SRCP_V8_DEVICE_GROUP_ERROR;
   }
   return(Ret);
}
