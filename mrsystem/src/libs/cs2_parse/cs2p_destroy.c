#include <stdlib.h>
#include <scanner.h>
#include "cs2parse.h"


void Cs2pDestroy(Cs2parser *Data)
{
   ScanDestroy(Cs2pGetScanner(Data));
   free(Data);
}
