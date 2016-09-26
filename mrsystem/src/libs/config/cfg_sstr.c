#include <stdlib.h>
#include <string.h>
#include <map.h>
#include <inipars.h>
#include "config.h"
#include "cfg_intern.h"


void ConfigAddStrVal(ConfigStruct *Data, CfgStrValues ValueTyp, char *Value)
{  char *p;
   IniValue *ValuePtr;

   switch (ValueTyp)
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
      case CfgBcVal:
         p = INI_STR_BC;
         break;
   }
   ValuePtr = (IniValue *)MapGet(ConfigGetConfig(Data), (MapKeyType)p);
   if (ValuePtr == (IniValue *)NULL)
   {
      ValuePtr = malloc(sizeof(IniValue));
      ValuePtr->DataType = IniParsString;
      ValuePtr->IntValue = 0;
      strcpy(ValuePtr->StringValue, Value);
      MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(p),
             (MapDataType)ValuePtr);
   }
   else
   {
      ValuePtr->DataType = IniParsString;
      ValuePtr->IntValue = 0;
      strcpy(ValuePtr->StringValue, Value);
   }
}
