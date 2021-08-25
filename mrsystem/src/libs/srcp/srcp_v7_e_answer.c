#include <stdio.h>
#include "srcp.h"

int SrcpV7EncAnswer(char *Buf, int Error)
{
   return(sprintf(Buf, SRCP_V7_STR_ANTWORT_INFO, Error));
}
