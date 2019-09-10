#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#include "inject_ipc.h"

#define SOFTWARE_VERSION "1.00"

typedef enum { EvalCmdline, EvalFile } EvalModeTyp;

static void usage(char *name)
{
   printf("mrinjectipc V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-a <addr> | -i <iface>] -p <port> [-v] (<file> | <parm>) | -?\n", name);
   puts("-a - network address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-v - verbose");
   puts("-? - this help");
   puts("");
   puts("<file> - filename with comands, syntax is like <parm>");
   puts("<parm> - IPC frame to send. Syntax is:");
   puts("         <Command> <SenderSocket> <ReceiverSocket> <CanResponse> <CanHash> <CanCommand> <CanPrio> <Lp1> <Ip1> <Ip2>");
}

int main(int argc, char *argv[])
{  InjectIpcStruct *Inject;
   ConfigStruct *Config;
   time_t Now;
   MrIpcCommandValue Command;
   int SenderSocket, ReceiverSocket, Ret, NumArgs, NameArgIndex;
   unsigned int CanResponse, CanHash, CanCommand, CanPrio, Ip1, Ip2;
   unsigned long Lp1;
   char **ValArgs, *Filename, *Param;
   EvalModeTyp EvalMode;

   Config = ConfigCreate();
   if (Config != (ConfigStruct *)NULL)
   {
      NumArgs = argc;
      ValArgs = argv;
      ConfigInit(Config, MRSYSTEM_CONFIG_FILE);
      ConfigReadfile(Config);
      NameArgIndex = ConfigCmdLine(Config, "a:i:p:v?", NumArgs, ValArgs);
      if (ConfigGetIntVal(Config, CfgUsageVal))
      {
         usage(argv[0]);
         Ret = 1;
      }
      else
      {
         if (argc >= NameArgIndex)
         {
            if (argc == NameArgIndex)
            {
               Filename = argv[NameArgIndex];
               EvalMode = EvalFile;
            }
            else
            {
               Param = argv[NameArgIndex];
               Command = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               SenderSocket = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               ReceiverSocket = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               CanResponse = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               CanHash = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               CanCommand = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               CanPrio = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               Lp1 = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               Ip1 = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               Ip2 = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               EvalMode = EvalCmdline;
            }
            Now = time(NULL);
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               printf("start with no fork at %s\n", asctime(localtime(&Now)));
            Inject = InjectIpcCreate();
            if (Inject != (InjectIpcStruct *)NULL)
            {
               InjectIpcInit(Inject, ConfigGetIntVal(Config, CfgVerboseVal),
                             ConfigGetStrVal(Config, CfgIfaceVal),
                             ConfigGetStrVal(Config, CfgAddrVal),
                             ConfigGetIntVal(Config, CfgPortVal));
               switch (EvalMode)
               {
                  case EvalCmdline:
                     InjectIpcRunCmd(Inject, Command, SenderSocket,
                                     ReceiverSocket, CanResponse, CanHash,
                                     CanCommand, CanPrio, Lp1, Ip1, Ip2);
                     break;
                  case EvalFile:
                     InjectIpcRunFile(Inject, Filename);
                     break;
               }
               InjectIpcDestroy(Inject);
               Ret = 0;
            }
            else
            {
               if (ConfigGetIntVal(Config, CfgVerboseVal))
                  puts("can not create inject module");
               Ret = 2;
            }
         }
         else
         {
            usage(argv[0]);
            Ret = 1;
         }
      }
      ConfigExit(Config);
      ConfigDestroy(Config);
   }
   else
   {
      Ret = 3;
   }
   return(Ret);
}
