#include <stdlib.h>
#include <scanner.h>
#include "inipars.h"

void IniParsExit(IniParsStruct *Data)
{
   ScanExit(IniParsGetScanner(Data));
}
