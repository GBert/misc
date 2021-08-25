#include <stdio.h>
#include "uci.h"


void UciWrUciTab(FILE *FilePointer, char *TabName, char *SubDir)
{
   fprintf(FilePointer, "%s ./%s/%s%s ./%s/%s%s\n", TabName,
           SubDir, TabName, UCI_CFG_EXTENSION,
           SubDir, TabName, UCI_DEF_EXTENSION);
}
