#include "srcp_parse.h"
#include "srcp.h"

int SrcpV7DecRead(char *Line, int Length, SrcpV7ParamType *Params)
{  int Token, Ret;

   if (SrcpParserInternalInit(Line, Length))
   {
      Ret = SRCP_V7_TARGET_ERROR;
      do {
         Token = SrcpParserInternalParse();
         switch (Token)
         {
            case PARSER_EOF:
               break;
            case PARSER_READ:
               break;
            case PARSER_GL:
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GlParams.Protocol,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GlParams.Type,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               Params->TargetParms.GlParams.Addr = SrcpParserInternalGetGanz();
               Ret = SRCP_V7_TARGET_GL_GENERIC_LOCO;
               break;
            case PARSER_GA:
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GaParams.Protocol,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               strcpy(Params->TargetParms.GaParams.Type,
                      SrcpParserInternalGetString());
               Token = SrcpParserInternalParse();
               Params->TargetParms.GaParams.Addr = SrcpParserInternalGetGanz();
               Ret = SRCP_V7_TARGET_GA_GENERIC_ACCESSORY;
               break;
            case PARSER_TIME:
               Ret = SRCP_V7_TARGET_TIME;
               break;
            case PARSER_POWER:
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
