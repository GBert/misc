#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "uci.h"


FILE *UciOpenCfgWr(char *Pfad, char *BaseName)
{
   return(UciOpenWr(Pfad, BaseName, UCI_CFG_EXTENSION));
}
