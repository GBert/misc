#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "uci.h"


FILE *UciOpenHlpWr(char *Pfad, char *BaseName)
{
   return(UciOpenWr(Pfad, BaseName, UCI_HLP_EXTENSION));
}
