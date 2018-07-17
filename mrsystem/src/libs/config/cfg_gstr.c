#include <stdlib.h>
#include <string.h>
#include <map.h>
#include <inipars.h>
#include "config.h"
#include "cfg_intern.h"


static char EmptyString[] = "";

char *ConfigGetStrVal(ConfigStruct *Data, CfgStrValues Value)
{  IniValue *CfgWert;
   char *p;

   switch (Value)
   {
      case CfgIfaceVal:
         p = INI_STR_IFACE;
         break;
      case CfgAddrVal:
         p = INI_STR_ADDRESS;
         break;
      case CfgCanIfVal:
         p = INI_STR_CAN_IFACE;
         break;
      case CfgPathVal:
         p = INI_STR_CS2_PATH;
         break;
      case CfgUdpBcVal:
         p = INI_STR_UDP_BC;
         break;
      case CfgStartVal:
         p = INI_STR_START;
         break;
      case CfgWakeUpS88:
         p = INI_STR_WAKEUP_S88;
         break;
      case CfgGpioS88:
         p = INI_STR_GPIO_S88;
         break;
      case CfgHideMs2Val:
         p = INI_STR_HIDE_MS2;
         break;
      case CfgSerialLineVal:
         p = INI_STR_SERIAL_LINE;
         break;
   }
   CfgWert = (IniValue *)MapGet(ConfigGetConfig(Data), (MapKeyType)p);
   if (CfgWert != (IniValue *)NULL)
      return(CfgWert->StringValue);
   else
      return(EmptyString);
}
