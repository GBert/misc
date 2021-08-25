#include <stdlib.h>
#include <scanner.h>
#include "inipars.h"

void IniParsDestroy(IniParsStruct *Data)
{
   ScanDestroy(IniParsGetScanner(Data));
   free(Data);
}
