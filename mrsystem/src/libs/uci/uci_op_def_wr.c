#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "uci.h"


FILE *UciOpenDefWr(char *Pfad, char *BaseName)
{
   return(UciOpenWr(Pfad, BaseName, UCI_DEF_EXTENSION));
}
