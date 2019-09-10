#include "srcp_parse.h"
#include "srcp.h"

int SrcpV7DecSet(char *Line, int Length, SrcpV7ParamType *Params)
{  int Token, Ret, i;

   if (SrcpParserInternalInit(Line, Length))
   {
      Ret = SRCP_V7_TARGET_ERROR;
      do {
         Token = SrcpParserInternalParse();
         switch (Token)
         {
            case PARSER_EOF:
               break;
            case PARSER_SET:
               break;
            case PARSER_GL:
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GlParams.Protocol,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.Direction = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.V = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.VMax = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.f[0] = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.NumF = SrcpParserInternalGetGanz();
               for (i=1; i<=Params->TargetParms.GlParams.NumF; i++)
               {
                  Token = SrcpParserInternalParse();
                  Params->TargetParms.GlParams.f[i] = SrcpParserInternalGetGanz();
               }
               Ret = SRCP_V7_TARGET_GL_GENERIC_LOCO;
               break;
            case PARSER_GA:
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GaParams.Protocol,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               Params->TargetParms.GaParams.Addr = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GaParams.Port = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GaParams.Value = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.GaParams.Delay = SrcpParserInternalGetGanz();
               Ret = SRCP_V7_TARGET_GA_GENERIC_ACCESSORY;
               break;
            case PARSER_TIME:
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.JulDay = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.Hour = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.Minute = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.Second = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.Fx = SrcpParserInternalGetGanz();
               Token = SrcpParserInternalParse();
               Params->TargetParms.TimeParams.Fy = SrcpParserInternalGetGanz();
               Ret = SRCP_V7_TARGET_TIME;
               break;
            case PARSER_POWER:
               Token = SrcpParserInternalParse();
               switch (Token)
               {
                  case PARSER_ON:
                     Params->TargetParms.PowerParams.PowerOn = TRUE;
                     break;
                  case PARSER_OFF:
                     Params->TargetParms.PowerParams.PowerOn = FALSE;
                     break;
                  default:
                     break;
               }
               Ret = SRCP_V7_TARGET_POWER;
               break;
            case PARSER_FB:
               Ret = SRCP_V7_TARGET_FB_FEEDBACK;
               break;
            default:
               Ret = SRCP_V7_TARGET_UNKNOWN;
               break;
         }
      } while (Token != PARSER_EOF);
      SrcpParserInternalExit();
   }
   else
   {
      Ret = SRCP_V7_TARGET_ERROR;
   }
   return(Ret);
}
