#include <stdio.h>
#include "uci.h"


void UciWrDefInt(FILE *FilePointer, char *Name, int MinVal, int MaxVal,
                 char *HelpText)
{
   fprintf(FilePointer, "%s %s \"%u %u\" \"%s\"\n",
           Name, UCI_TYPE_INT, MinVal, MaxVal, HelpText);
}
