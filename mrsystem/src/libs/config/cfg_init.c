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
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "255.255.255.255");
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
   strcpy(ValuePtr->StringValue, "/var/www/config");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_CS2_PATH),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
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
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_PROTOKOLL),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "stop");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_START),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "sync");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_SYNC),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_CONN_TCP),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, DISABLE_WAKEUP_S88);
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_WAKEUP_S88),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "unhide");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_HIDE_MS2),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 7970;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_EMU_HOST_COM),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsString;
   ValuePtr->IntValue = 0;
   strcpy(ValuePtr->StringValue, "tty0,500000");
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_SERIAL_LINE),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 32;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_NUM_LOKFKT),
          (MapDataType)ValuePtr);
   ValuePtr = malloc(sizeof(IniValue));
   ValuePtr->DataType = IniParsInt;
   ValuePtr->IntValue = 0;
   ValuePtr->StringValue[0] = '\0';
   MapSet(ConfigGetConfig(Data), (MapKeyType)strdup(INI_STR_WRITE_WEB),
          (MapDataType)ValuePtr);
}

void ConfigInit(ConfigStruct *Data, char *IniFile)
{  IniParsKeyword IniKeywords[19];

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
   IniKeywords[3].DataType = IniParsString;
   strcpy(IniKeywords[4].Keyword, INI_STR_CAN_IFACE);
   IniKeywords[4].Symbol = INI_TOK_CAN_IFACE;
   IniKeywords[4].DataType = IniParsString;
   strcpy(IniKeywords[5].Keyword, INI_STR_CS2_PATH);
   IniKeywords[5].Symbol = INI_TOK_CS2_PATH;
   IniKeywords[5].DataType = IniParsString;
   strcpy(IniKeywords[6].Keyword, INI_STR_BC);
   IniKeywords[6].Symbol = INI_TOK_BC;
   IniKeywords[6].DataType = IniParsInt;
   strcpy(IniKeywords[7].Keyword, INI_STR_ZENTRALE);
   IniKeywords[7].Symbol = INI_TOK_ZENTRALE;
   IniKeywords[7].DataType = IniParsInt;
   strcpy(IniKeywords[8].Keyword, INI_STR_PROTOKOLL);
   IniKeywords[8].Symbol = INI_TOK_PROTOKOLL;
   IniKeywords[8].DataType = IniParsInt;
   strcpy(IniKeywords[9].Keyword, INI_STR_START);
   IniKeywords[9].Symbol = INI_TOK_START;
   IniKeywords[9].DataType = IniParsString;
   strcpy(IniKeywords[10].Keyword, INI_STR_SYNC);
   IniKeywords[10].Symbol = INI_TOK_SYNC;
   IniKeywords[10].DataType = IniParsInt;
   strcpy(IniKeywords[11].Keyword, INI_STR_CONN_TCP);
   IniKeywords[11].Symbol = INI_TOK_CONN_TCP;
   IniKeywords[11].DataType = IniParsInt;
   strcpy(IniKeywords[12].Keyword, INI_STR_WAKEUP_S88);
   IniKeywords[12].Symbol = INI_TOK_WAKEUP_S88;
   IniKeywords[12].DataType = IniParsString;
   strcpy(IniKeywords[13].Keyword, INI_STR_GPIO_S88);
   IniKeywords[13].Symbol = INI_TOK_GPIO_S88;
   IniKeywords[13].DataType = IniParsString;
   strcpy(IniKeywords[14].Keyword, INI_STR_HIDE_MS2);
   IniKeywords[14].Symbol = INI_TOK_HIDE_MS2;
   IniKeywords[14].DataType = IniParsString;
   strcpy(IniKeywords[15].Keyword, INI_STR_EMU_HOST_COM);
   IniKeywords[15].Symbol = INI_TOK_EMU_HOST_COM;
   IniKeywords[15].DataType = IniParsInt;
   strcpy(IniKeywords[16].Keyword, INI_STR_SERIAL_LINE);
   IniKeywords[16].Symbol = INI_TOK_SERIAL_LINE;
   IniKeywords[16].DataType = IniParsString;
   strcpy(IniKeywords[17].Keyword, INI_STR_NUM_LOKFKT);
   IniKeywords[17].Symbol = INI_TOK_NUM_LOKFKT;
   IniKeywords[17].DataType = IniParsInt;
   strcpy(IniKeywords[18].Keyword, INI_STR_WRITE_WEB);
   IniKeywords[18].Symbol = INI_TOK_WRITE_WEB;
   IniKeywords[18].DataType = IniParsInt;
   IniParsInit(ConfigGetParser(Data), IniFile, 19, IniKeywords);
   InitConfigValues(Data);
}
