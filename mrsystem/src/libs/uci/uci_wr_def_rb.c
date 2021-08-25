#include <stdio.h>
#include "uci.h"


void UciWrDefRb(FILE *FilePointer, char *Name, char *RbText, char *HelpText)
{
   fprintf(FilePointer, "%s %s \"%s\" \"%s\"\n",
           Name, UCI_TYPE_RB, RbText, HelpText);
}
