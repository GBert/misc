#ifndef CONFIG_H
#define CONFIG_H
/**
* @mainpage config
*
* Funktionen f&uuml;r das Einlesen der Konfiguration.
*
* Nach dem Anlegen eines Config Objektes und Initialisierung kann die
* Konfigdatei eingelesen bzw. die Kommandozeile an den Damon geparst werden.
* Danach k&ouml;nnen die einzelnen Knfigurationswerte abgefragt werden. Es
* gibt sowohl f&uuml;r Zahlen als auch f&uuml;r Zeichenketten eine eigene
* Abfragefunktion. Es gibt auch Funktionen, um einen Konfigurationswert zu
* setzen.
*
* @author Michael Bernstein
*/

#include <map.h>
#include <inipars.h>

/** @file */

/**
* Konstanten f&uuml;r den Pfad der Konfigdatei
*/
#define MRSYSTEM_CONFIG_FILE "/etc/mrsystem"

/**
* Konstanten f&uuml;r Konfigurationswerte
*/
#define MRSYSTEM_CFG_PROTO_MOTOROLA 0x01
#define MRSYSTEM_CFG_PROTO_MFX      0x02
#define MRSYSTEM_CFG_PROTO_DCC      0x04

#define MRSYSTEM_CFG_SYSTEM_START "start"
#define MRSYSTEM_CFG_SYSTEM_STOP  "stop"
#define MRSYSTEM_CFG_SYSTEM_HIDE   "hide"
#define MRSYSTEM_CFG_SYSTEM_UNHIDE "unhide"

#define MRSYSTEM_CFG_SYNC_PERIODIC 0x01
#define MRSYSTEM_CFG_SYNC_KEYBD    0x02
#define MRSYSTEM_CFG_SYNC_LAYOUT   0x04
#define MRSYSTEM_CFG_SYNC_MEM      0x08
#define MRSYSTEM_CFG_SYNC_CONTR    0x10

#define DISABLE_WAKEUP_S88 "0"

/**
* Datentypen f&uuml;r Konfigurationsvariable
*/
typedef enum { CfgPortVal, CfgBcVal, CfgForkVal, CfgTraceVal, CfgVerboseVal,
               CfgUsageVal, CfgZentraleVal, CfgProtokollVal, CfgSyncVal,
               CfgConnTcpVal, CfgEmuHostCom,  CfgNumLokfkts,
               CfgWriteWebVal } CfgIntValues;
typedef enum { CfgIfaceVal, CfgAddrVal, CfgCanIfVal, CfgPathVal,
               CfgUdpBcVal, CfgStartVal, CfgWakeUpS88, CfgGpioS88,
               CfgHideMs2Val, CfgSerialLineVal } CfgStrValues;
/**
* Datentypen f&uuml;r Funktionen der Konfigurationslib
*/
typedef struct {
   Map *Config;
   IniParsStruct *Parser;
} ConfigStruct;

/**
* Makros, um Felder im Kommando zu setzen und lesen
*/
#define ConfigSetConfig(Data, Val) (Data)->Config=Val
#define ConfigSetParser(Data, Val) (Data)->Parser=Val
#define ConfigGetConfig(Data) (Data)->Config
#define ConfigGetParser(Data) (Data)->Parser

ConfigStruct *ConfigCreate(void);
void ConfigDestroy(ConfigStruct *Data);
void ConfigInit(ConfigStruct *Data, char *IniFile);
void ConfigExit(ConfigStruct *Data);
void ConfigReadfile(ConfigStruct *Data);
int ConfigCmdLine(ConfigStruct *Data, char *optstr, int argc, char *argv[]);
int ConfigGetIntVal(ConfigStruct *Data, CfgIntValues Value);
char *ConfigGetStrVal(ConfigStruct *Data, CfgStrValues Value);
void ConfigAddIntVal(ConfigStruct *Data, CfgIntValues ValueTyp, int Value);
void ConfigAddStrVal(ConfigStruct *Data, CfgStrValues ValueTyp, char *Value);

#endif
