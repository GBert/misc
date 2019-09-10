#include <scanner.h>
#include "srcp_parse.h"
#include "srcp.h"

int SrcpDecGetCmd(char *Line, int Length)
{  int Token, Ret;

   if (SrcpParserInternalInit(Line, Length))
   {
      Token = SrcpParserInternalParse();
      switch (Token)
      {
         case PARSER_EOF:
            Ret = SRCP_CMD_ERROR;
            break;
         case PARSER_SET:
            Ret = SRCP_CMD_SET;
            break;
         case PARSER_GET:
            Ret = SRCP_CMD_GET;
            break;
         case PARSER_WAIT:
            Ret = SRCP_CMD_WAIT;
            break;
         case PARSER_INIT:
            Ret = SRCP_CMD_INIT;
            break;
         case PARSER_TERM:
            Ret = SRCP_CMD_TERM;
            break;
         case PARSER_VERIFY:
            Ret = SRCP_CMD_VERIFY;
            break;
         case PARSER_RESET:
            Ret = SRCP_CMD_RESET;
            break;
         case PARSER_GO:
            Ret = SRCP_CMD_GO;
            break;
         case PARSER_ERROR:
            Ret = SRCP_CMD_ERROR;
            break;
         case PARSER_WRITE:
            Ret = SRCP_CMD_WRITE;
            break;
         case PARSER_READ:
            Ret = SRCP_CMD_READ;
            break;
      }
      SrcpParserInternalExit();
   }
   else
   {
      Ret = SRCP_CMD_ERROR;
   }
   return(Ret);
}
