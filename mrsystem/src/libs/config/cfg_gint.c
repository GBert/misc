#include <map.h>
#include <inipars.h>
#include "config.h"
#include "cfg_intern.h"
#include <stdio.h>


int ConfigGetIntVal(ConfigStruct *Data, CfgIntValues Value)
{  IniValue *CfgWert;
   char *p;

   switch (Value)
   {
      case CfgPortVal:
         p = INI_STR_PORT;
         break;
      case CfgUdpBcVal:
         p = INI_STR_UDP_BC;
         break;
      case CfgForkVal:
         p = INI_STR_FORK;
         break;
      case CfgTraceVal:
         p = INI_STR_TRACE;
         break;
      case CfgVerboseVal:
         p = INI_STR_VERBOSE;
         break;
      case CfgUsageVal:
         p = INI_STR_USAGE;
         break;
      case CfgZentraleVal:
         p = INI_STR_ZENTRALE;
         break;
   }
   CfgWert = (IniValue *)MapGet(ConfigGetConfig(Data), (MapKeyType)p);
   if (CfgWert != (IniValue *)NULL)
      return(CfgWert->IntValue);
   else
      return(0);
}
