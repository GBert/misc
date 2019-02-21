#include <stdio.h>
#include "uci.h"


void UciWrDefIp(FILE *FilePointer, char *Name, char *HelpText)
{
   fprintf(FilePointer, "%s %s \"\" \"%s\"\n", Name, UCI_TYPE_IP, HelpText);
}
