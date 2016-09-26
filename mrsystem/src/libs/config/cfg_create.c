#include <stdlib.h>
#include <map.h>
#include <inipars.h>
#include "config.h"


ConfigStruct *ConfigCreate(void)
{  ConfigStruct *NewData;

   NewData = (ConfigStruct *)malloc(sizeof(ConfigStruct));
   if (NewData != (ConfigStruct *)NULL)
   {
      ConfigSetConfig(NewData, MapCreate());
      if (ConfigGetConfig(NewData) != (Map *)NULL)
      {
         ConfigSetParser(NewData, IniParsCreate());
         if (ConfigGetParser(NewData) == (IniParsStruct *)NULL)
         {
            MapDestroy(ConfigGetConfig(NewData));
            free(NewData);
            NewData = (ConfigStruct *)NULL;
         }
      }
      else
      {
         free(NewData);
         NewData = (ConfigStruct *)NULL;
      }
   }
   return(NewData);
}
