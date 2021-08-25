#include <inipars.h>
#include "config.h"


void ConfigExit(ConfigStruct *Data)
{
   IniParsExit(ConfigGetParser(Data));
}
