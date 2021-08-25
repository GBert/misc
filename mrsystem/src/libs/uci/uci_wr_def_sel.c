#include <stdio.h>
#include "uci.h"


void UciWrDefSel(FILE *FilePointer, char *Name, char *SelText, char *HelpText)
{
   fprintf(FilePointer, "%s %s \"%s\" \"%s\"\n",
           Name, UCI_TYPE_SEL, SelText, HelpText);
}
