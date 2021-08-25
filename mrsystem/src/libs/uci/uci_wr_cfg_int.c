#include <stdio.h>
#include "uci.h"


void UciWrCfgInt(FILE *FilePointer, char *Name, int Wert)
{
   fprintf(FilePointer, "%s %d\n", Name, Wert);
}
