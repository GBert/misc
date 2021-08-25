#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <boolean.h>
#include <config.h>
#include "can_eth.h"
#include "../common/cs2eth.h"

#define SOFTWARE_VERSION "2.02"

static void usage(char *name)
{
   printf("mrcs2eth V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-h] [-a <addr> | -i <iface>] -p <port> -b <baddr>)", name);
#ifdef TRACE
   printf(" -t");
#endif
   puts(" | -?\n");
   puts("-a - network address of drehscheibe");
   puts("-h - hide MS2");
   puts("-i - interface to drehscheibe");
   puts("-b - send udp as broadcast");
   puts("-p - port of drehscheibe");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
#ifdef TRACE
   puts("-t - trace can messages from udp");
#endif
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Cs2ethStruct *Cs2eth;
   IoFktStruct *IoFunctions;
   ConfigStruct *Config;
   pid_t ChildPid;
   time_t Now;
   int Ret, NumArgs;
   char **ValArgs;

   Config = ConfigCreate();
   if (Config != (ConfigStruct *)NULL)
   {
      NumArgs = argc;
      ValArgs = argv;
      ConfigInit(Config, MRSYSTEM_CONFIG_FILE);
      ConfigReadfile(Config);
#ifdef TRACE
      ConfigCmdLine(Config, "a:b:h:i:p:ftv?", NumArgs, ValArgs);
#else
      ConfigCmdLine(Config, "a:b:h:i:p:fv?", NumArgs, ValArgs);
#endif
      if (ConfigGetIntVal(Config, CfgUsageVal))
      {
         usage(argv[0]);
         Ret = 1;
      }
      else if (ConfigGetIntVal(Config, CfgForkVal))
      {
         ChildPid = fork();
         if (ChildPid == -1)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("ERROR: can not go to backgound");
            ConfigExit(Config);
            ConfigDestroy(Config);
            Ret = 4;
         }
         else if (ChildPid == 0)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("child running");
            IoFunctions = CanEthInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                     ConfigGetIntVal(Config, CfgBcVal),
                                     ConfigGetStrVal(Config, CfgUdpBcVal));
            if (IoFunctions != (IoFktStruct *)NULL)
            {
               Cs2eth = Cs2ethCreate();
               if (Cs2eth != (Cs2ethStruct *)NULL)
               {
                  Cs2ethInit(Cs2eth, ConfigGetIntVal(Config, CfgVerboseVal),
                             ConfigGetStrVal(Config, CfgIfaceVal),
                             ConfigGetStrVal(Config, CfgAddrVal),
                             ConfigGetIntVal(Config, CfgPortVal),
                             ConfigGetStrVal(Config, CfgHideMs2Val),
#ifdef TRACE
                             ConfigGetIntVal(Config, CfgTraceVal),
#endif
                             IoFunctions);
                  Cs2ethRun(Cs2eth);
                  Cs2ethDestroy(Cs2eth);
                  Ret = 0;
               }
               else
               {
                  Ret = 2;
               }
               CanEthExit(IoFunctions);
            }
            else
            {
               puts("ERROR: can not create IoFunctions module");
               Ret = 1;
            }
         }
         else
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("parent terminates");
            signal(SIGCHLD, SIG_IGN);
            Ret = 0;
         }
      }
      else
      {
         Now = time(NULL);
         if (ConfigGetIntVal(Config, CfgVerboseVal))
            printf("start with no fork at %s\n", asctime(localtime(&Now)));
         IoFunctions = CanEthInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                  ConfigGetIntVal(Config, CfgBcVal),
                                  ConfigGetStrVal(Config, CfgUdpBcVal));
         if (IoFunctions != (IoFktStruct *)NULL)
         {
            Cs2eth = Cs2ethCreate();
            if (Cs2eth != (Cs2ethStruct *)NULL)
            {
               Cs2ethInit(Cs2eth, ConfigGetIntVal(Config, CfgVerboseVal),
                          ConfigGetStrVal(Config, CfgIfaceVal),
                          ConfigGetStrVal(Config, CfgAddrVal),
                          ConfigGetIntVal(Config, CfgPortVal),
                          ConfigGetStrVal(Config, CfgHideMs2Val),
#ifdef TRACE
                          ConfigGetIntVal(Config, CfgTraceVal),
#endif
                          IoFunctions);
               Cs2ethRun(Cs2eth);
               Cs2ethDestroy(Cs2eth);
               Ret = 0;
            }
            else
            {
               Ret = 2;
            }
            CanEthExit(IoFunctions);
         }
         else
         {
            puts("ERROR: can not create IoFunctions module");
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
