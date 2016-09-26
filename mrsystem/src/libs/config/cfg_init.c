#include <stdlib.h>
#include <string.h>
#include <map.h>
#include <inipars.h>
#include "config.h"
#include "cfg_intern.h"


static void InitConfigValues(ConfigStruct *Data)
{  IniValue *ValuePtr;

   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 10142;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_PORT),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_UDP_BC),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "127.0.0.1");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_ADDRESS),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "can0");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_CAN_IFACE),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "/www/config");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_CS2_PATH),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "192.168.0.255");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_BC),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_FORK),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_TRACE),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_VERBOSE),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_USAGE),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 1;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_ZENTRALE),
          (MapDataType)ValuePtr);
}

void ConfigInit(ConfigStruct *Data, char *IniFile)
{  IniParsKeyword IniKeywords[6];

   MapInit(ConfigGetConfig(Data), (CmpFkt)strcmp, (MapKeyDelCbFkt)free,
           (MapDataDelCbFkt)free);
   strcpy(IniKeywords[0].Keyword, INI_STR_PORT);
   IniKeywords[0].Symbol = INI_TOK_PORT;
   IniKeywords[0].DataType = IniParsInt;
   strcpy(IniKeywords[1].Keyword, INI_STR_ADDRESS);
   IniKeywords[1].Symbol = INI_TOK_ADDR;
   IniKeywords[1].DataType = IniParsString;
   strcpy(IniKeywords[2].Keyword, INI_STR_IFACE);
   IniKeywords[2].Symbol = INI_TOK_IFACE;
   IniKeywords[2].DataType = IniParsString;
   strcpy(IniKeywords[3].Keyword, INI_STR_UDP_BC);
   IniKeywords[3].Symbol = INI_TOK_UDP_BC;
   IniKeywords[3].DataType = IniParsInt;
   strcpy(IniKeywords[4].Keyword, INI_STR_CAN_IFACE);
   IniKeywords[4].Symbol = INI_TOK_CAN_IFACE;
   IniKeywords[4].DataType = IniParsString;
   strcpy(IniKeywords[5].Keyword, INI_STR_ZENTRALE);
   IniKeywords[5].Symbol = INI_TOK_ZENTRALE;
   IniKeywords[5].DataType = IniParsInt;
   IniParsInit(ConfigGetParser(Data), IniFile, 6, IniKeywords);
   InitConfigValues(Data);
}
