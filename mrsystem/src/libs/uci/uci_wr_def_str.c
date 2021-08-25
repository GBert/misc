#include <stdio.h>
#include "uci.h"


void UciWrDefStr(FILE *FilePointer, char *Name, unsigned int MinChars,
                 unsigned int MaxChars, char *HelpText)
{
   fprintf(FilePointer, "%s %s \"%u %u\" \"%s\"\n",
           Name, UCI_TYPE_STR, MinChars, MaxChars, HelpText);
}
