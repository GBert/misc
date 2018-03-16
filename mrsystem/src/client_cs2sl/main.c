#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <boolean.h>
#include <config.h>
#include "cs2sl.h"

#define SOFTWARE_VERSION "1.01"

static void usage(char *name)
{
   printf("mrcs2sl V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> | -x)", name);
#ifdef TRACE
   printf(" -t");
#endif
   puts(" | -?\n");
   puts("-a - network interface to drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
   puts("-x - use tcp connection");
#ifdef TRACE
   puts("-t - trace can messages from udp");
#endif
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Cs2slStruct *Cs2sl;
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
      ConfigCmdLine(Config, "a:p:ftv?", NumArgs, ValArgs);
#else
      ConfigCmdLine(Config, "a:p:fv?", NumArgs, ValArgs);
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
            Cs2sl = Cs2slCreate();
            if (Cs2sl != (Cs2slStruct *)NULL)
            {
               Cs2slInit(Cs2sl, ConfigGetIntVal(Config, CfgVerboseVal),
                         ConfigGetStrVal(Config, CfgIfaceVal),
                         ConfigGetStrVal(Config, CfgAddrVal),
                         ConfigGetIntVal(Config, CfgPortVal),
                         ConfigGetIntVal(Config, CfgConnTcpVal)
#ifdef TRACE
                         , ConfigGetIntVal(Config, CfgTraceVal)
#endif
                         );
               Cs2slRun(Cs2sl);
               Cs2slDestroy(Cs2sl);
               Ret = 0;
            }
            else
            {
               Ret = 2;
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
         Cs2sl = Cs2slCreate();
         if (Cs2sl != (Cs2slStruct *)NULL)
         {
            Cs2slInit(Cs2sl, ConfigGetIntVal(Config, CfgVerboseVal),
                      ConfigGetStrVal(Config, CfgIfaceVal),
                      ConfigGetStrVal(Config, CfgAddrVal),
                      ConfigGetIntVal(Config, CfgPortVal),
                      ConfigGetIntVal(Config, CfgConnTcpVal)
#ifdef TRACE
                      , ConfigGetIntVal(Config, CfgTraceVal)
#endif
                      );
            Cs2slRun(Cs2sl);
            Cs2slDestroy(Cs2sl);
            Ret = 0;
         }
         else
         {
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
