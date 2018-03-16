#include <stdlib.h>
#include <map.h>
#include <inipars.h>
#include "config.h"


void ConfigDestroy(ConfigStruct *Data)
{
   if (Data != (ConfigStruct *)NULL)
   {
      if (ConfigGetParser(Data) != (IniParsStruct *)NULL)
      {
         IniParsDestroy(ConfigGetParser(Data));
      }
      if (ConfigGetConfig(Data) != (Map *)NULL)
      {
         MapDestroy(ConfigGetConfig(Data));
      }
      free(Data);
   }
}
