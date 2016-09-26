#ifndef CONFIG_H
#define CONFIG_H

#include <map.h>
#include <inipars.h>

#define MRSYSTEM_CONFIG_FILE "/etc/mrsystem"

typedef enum { CfgPortVal, CfgUdpBcVal, CfgForkVal, CfgTraceVal,
               CfgVerboseVal, CfgUsageVal, CfgZentraleVal } CfgIntValues;
typedef enum { CfgIfaceVal, CfgAddrVal, CfgCanIfVal, CfgPathVal,
               CfgBcVal } CfgStrValues;
typedef struct {
   Map *Config;
   IniParsStruct *Parser;
} ConfigStruct;

#define ConfigSetConfig(Data, Val) (Data)->Config=Val
#define ConfigSetParser(Data, Val) (Data)->Parser=Val
#define ConfigGetConfig(Data) (Data)->Config
#define ConfigGetParser(Data) (Data)->Parser

ConfigStruct *ConfigCreate(void);
void ConfigDestroy(ConfigStruct *Data);
void ConfigInit(ConfigStruct *Data, char *IniFile);
void ConfigExit(ConfigStruct *Data);
void ConfigReadfile(ConfigStruct *Data);
void ConfigCmdLine(ConfigStruct *Data, char *optstr, int argc, char *argv[]);
int ConfigGetIntVal(ConfigStruct *Data, CfgIntValues Value);
char *ConfigGetStrVal(ConfigStruct *Data, CfgStrValues Value);
void ConfigAddIntVal(ConfigStruct *Data, CfgIntValues ValueTyp, int Value);
void ConfigAddStrVal(ConfigStruct *Data, CfgStrValues ValueTyp, char *Value);

#endif
