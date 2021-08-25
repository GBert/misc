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
      case CfgBcVal:
         p = INI_STR_BC;
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
      case CfgProtokollVal:
         p = INI_STR_PROTOKOLL;
         break;
      case CfgSyncVal:
         p = INI_STR_SYNC;
         break;
      case CfgConnTcpVal:
         p = INI_STR_CONN_TCP;
         break;
      case CfgEmuHostCom:
         p = INI_STR_EMU_HOST_COM;
         break;
      case CfgNumLokfkts:
         p = INI_STR_NUM_LOKFKT;
         break;
      case CfgWriteWebVal:
         p = INI_STR_WRITE_WEB;
         break;
   }
   CfgWert = (IniValue *)MapGet(ConfigGetConfig(Data), (MapKeyType)p);
   if (CfgWert != (IniValue *)NULL)
      return(CfgWert->IntValue);
   else
      return(0);
}
