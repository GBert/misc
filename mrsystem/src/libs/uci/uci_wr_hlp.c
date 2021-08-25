#include <stdio.h>
#include "uci.h"


void UciWrHlp(FILE *FilePointer, char *Name, char *Beschreibung)
{
   fprintf(FilePointer, "%s \"%s\"\n", Name, Beschreibung);
}
