#include <scanner.h>
#include "srcp_parse.h"
#include "srcp.h"

int SrcpDecHandshake(char *Line, int Length)
{  int Token, Ret;

   if (SrcpParserInternalInit(Line, Length))
   {
      Token = SrcpParserInternalParse();
      switch (Token)
      {
         case PARSER_PROTOCOL:
            Token = SrcpParserInternalParse();
            switch (Token)
            {
               case PARSER_SRCP:
                  Token = SrcpParserInternalParse();
                  switch (Token)
                  {
                     case PARSER_ZAHL:
                        if (SrcpParserInternalGetGanz() == 0)
                        {
                           Token = SrcpParserInternalParse();
                           switch (Token)
                           {
                              case PARSER_PUNKT:
                                 Token = SrcpParserInternalParse();
                                 switch (Token)
                                 {
                                    case PARSER_ZAHL:
                                       if (SrcpParserInternalGetGanz() == 8)
                                       {
                                          Ret = SRCP_HANDSHAKE_OK;
                                       }
                                       else
                                       {
                                          Ret = SRCP_HANDSHAKE_ERROR;
                                       }
                                       break;
                                    default:
                                       Ret = SRCP_HANDSHAKE_ERROR;
                                       break;
                                 }
                                 break;
                              default:
                                 Ret = SRCP_HANDSHAKE_ERROR;
                                 break;
                           }
                        }
                        else
                        {
                           Ret = SRCP_HANDSHAKE_ERROR;
                        }
                        break;
                     default:
                        Ret = SRCP_HANDSHAKE_ERROR;
                        break;
                  }
                  break;
               default:
                  Ret = SRCP_HANDSHAKE_ERROR;
                  break;
            }
            break;
         case PARSER_CONNECTIONMODE:
            Token = SrcpParserInternalParse();
            switch (Token)
            {
               case PARSER_SRCP:
                  Token = SrcpParserInternalParse();
                  switch (Token)
                  {
                     case PARSER_BEZEICHNER:
                        if (strcmp(SrcpParserInternalGetString(),
                                   "INFO") == 0)
                        {
                           Ret = SRCP_HANDSHAKE_MODE_INFO;
                        }
                        else if (strcmp(SrcpParserInternalGetString(),
                                        "COMMAND") == 0)
                        {
                           Ret = SRCP_HANDSHAKE_MODE_COMMAND;
                        }
                        break;
                     default:
                        Ret = SRCP_HANDSHAKE_ERROR;
                        break;
                  }
                  break;
               default:
                  break;
            }
            break;
         default:
            break;
      }
      SrcpParserInternalExit();
   }
   else
   {
      Ret = SRCP_HANDSHAKE_ERROR;
   }
   return(Ret);
}
