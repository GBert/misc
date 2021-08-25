#include <inipars.h>
#include "config.h"


void ConfigReadfile(ConfigStruct *Data)
{
   IniParsReadFile(ConfigGetParser(Data), ConfigGetConfig(Data));
}
