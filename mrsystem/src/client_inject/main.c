#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#include "inject.h"

#define SOFTWARE_VERSION "1.03"

typedef enum { EvalIntern, EvalCmdline, EvalFile } EvalModeTyp;

static void usage(char *name)
{
   printf("mrinject V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-a <addr> | -i <iface>] -p <port> [-v] ) | -?\n", name);
   puts("-a - network address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-v - verbose");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  InjectStruct *Inject;
   ConfigStruct *Config;
   time_t Now;
   int Ret, NumArgs, NameArgIndex, DLC, CanBytes[8], i;
   unsigned long CanId;
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
               CanId = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               Param = argv[NameArgIndex];
               DLC = strtol(Param, (char **)NULL, 0);
               NameArgIndex++;
               for (i = 0; i < 8; i++)
               {
                  Param = argv[NameArgIndex];
                  CanBytes[i] = strtol(Param, (char **)NULL, 0);
                  NameArgIndex++;
               }
               EvalMode = EvalCmdline;
            }
         }
         else
         {
            EvalMode = EvalIntern;
         }
         Now = time(NULL);
         if (ConfigGetIntVal(Config, CfgVerboseVal))
            printf("start with no fork at %s\n", asctime(localtime(&Now)));
         Inject = InjectCreate();
         if (Inject != (InjectStruct *)NULL)
         {
            InjectInit(Inject, ConfigGetIntVal(Config, CfgVerboseVal),
                       ConfigGetStrVal(Config, CfgIfaceVal),
                       ConfigGetStrVal(Config, CfgAddrVal),
                       ConfigGetIntVal(Config, CfgPortVal));
            switch (EvalMode)
            {
               case EvalIntern:
                  InjectRun(Inject);
                  break;
               case EvalCmdline:
                  InjectRunCmd(Inject, CanId, DLC, CanBytes);
                  break;
               case EvalFile:
                  InjectRunFile(Inject, Filename);
                  break;
            }
            InjectDestroy(Inject);
            Ret = 0;
         }
         else
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("can not create inject module");
            Ret = 2;
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
