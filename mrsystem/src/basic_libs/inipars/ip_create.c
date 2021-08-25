#include <stdlib.h>
#include <scanner.h>
#include "inipars.h"

IniParsStruct *IniParsCreate(void)
{  IniParsStruct *NewData;

   NewData = (IniParsStruct *)malloc(sizeof(IniParsStruct));
   if (NewData != NULL)
   {
      IniParsSetScanner(NewData, ScanCreate());
      if (IniParsGetScanner(NewData) == (Scanner *)NULL)
      {
         free(NewData);
         NewData = (IniParsStruct *)NULL;
      }
   }
   return(NewData);
}
