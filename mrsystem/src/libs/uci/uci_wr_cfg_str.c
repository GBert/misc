#include <stdio.h>
#include "uci.h"


void UciWrCfgStr(FILE *FilePointer, char *Name, char *Wert)
{
   fprintf(FilePointer, "%s \"%s\"\n", Name, Wert);
}
