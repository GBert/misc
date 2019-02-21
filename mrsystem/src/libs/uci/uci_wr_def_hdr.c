#include <stdio.h>
#include "uci.h"


void UciWrDefHdr(FILE *FilePointer, char *BaseName)
{
   fprintf(FilePointer, "//%s%s\n", BaseName, UCI_DEF_EXTENSION);
   fputs("//Start-Parameter fuer Maerklin-Zentrale: definitions\n", FilePointer);
   fputs("//last change: 16.09.2018\n", FilePointer);
   fputs("//put type in quotes here if value has to quoted in config file\n", FilePointer);
   fputs("//name type range description\n", FilePointer);
}
