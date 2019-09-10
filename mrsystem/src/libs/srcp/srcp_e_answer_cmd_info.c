#include <stdio.h>
#include "srcp.h"

typedef struct {
   int ErrorCode;
   char *AnswerTemplate;
} AnswerStruct;

static AnswerStruct AnswerCommandInfoTable[] = {
   { SRCP_ERRNO_ANTWORT_COMMAND_INFO_OK, SRCP_STR_ANTWORT_COMMAND_INFO_OK },
   { SRCP_ERRNO_ANTWORT_COMMAND_INFOINIT_OK, SRCP_STR_ANTWORT_COMMAND_INFOINIT_OK },
   { SRCP_ERRNO_ANTWORT_COMMAND_INFOTERM_OK, SRCP_STR_ANTWORT_COMMAND_INFOTERM_OK },
};


int SrcpEncAnswerCommandInfo(char *Buf, int Answer, char *Param)
{
   return(sprintf(Buf, AnswerCommandInfoTable[Answer].AnswerTemplate, Param)+1);
}
