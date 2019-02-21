#include <stdio.h>
#include "uci.h"


void UciWrDefBits(FILE *FilePointer, char *Name, char *BitText, char *HelpText)
{
   fprintf(FilePointer, "%s %s \"%s\" \"%s\"\n",
           Name, UCI_TYPE_BIT, BitText, HelpText);
}
