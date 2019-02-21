#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "uci.h"


FILE *UciOpenUciWr(char *Pfad, char *BaseName)
{
   return(UciOpenWr(Pfad, BaseName, UCI_UCI_EXTENSION));
}
