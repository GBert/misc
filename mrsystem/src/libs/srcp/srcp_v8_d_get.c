#include "srcp_parse.h"
#include "srcp.h"

int SrcpV8DecGet(char *Line, int Length, SrcpV8ParamType *Params)
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
               Ret = SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO;
               break;
            case PARSER_GA:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.GaParams.Port = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY;
               break;
            case PARSER_FB:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.FbParams.Addr = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR;
               break;
            case PARSER_GM:
               Ret = SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE;
               break;
            case PARSER_SM:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.SmParams.DecoderAddress = SrcpParserInternalGetGanz();
               Ret = SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE;
               break;
            case PARSER_TIME:
               Ret = SRCP_V8_DEVICE_GROUP_TIME;
               break;
             case PARSER_LOCK:
               Ret = SRCP_V8_DEVICE_GROUP_LOCK;
               break;
            case PARSER_POWER:
               Ret = SRCP_V8_DEVICE_GROUP_POWER;
               break;
            case PARSER_SERVER:
               Ret = SRCP_V8_DEVICE_GROUP_SERVER;
               break;
            case PARSER_SESSION:
               Token = SrcpParserInternalParse();
               Params->DeviceGroupParms.SessionParams.SessionId = SrcpParserInternalGetGanz();
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
