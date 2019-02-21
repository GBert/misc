#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#ifdef DEBUG
#include <lx_debug.h>
#endif
#include "zentrale.h"

#define SOFTWARE_VERSION "4.01"

static void usage(char *name)
{
   printf("mrzentrale V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> [-s <proto>] [-l path] [-z <zentrale>] [-g}) [-2 <syncmask>] [-8 <param>] [ -w ] | -?\n", name);
   puts("-a - network address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-z - 0=proxy mode, 1=ethernet addon for ms2");
   puts("-f - dont fork to go in background");
   puts("-k - number of lokfunctions");
   puts("-v - verbose");
   puts("-l - path where write lokomotive.cs2");
   puts("-s - bitmask for track protocolls, 1=motorola, 2=mfx, 4=dcc");
   puts("-g - system start, start power on tracks");
   puts("-2 - bitmask for syn with master CS2 / MS2, 1=sync periodic, 2=keybd, 2=layout 3=mem 4=contr");
   puts("-8 - start s88 modules, params like wakeup-links-88 from G.B.");
   puts("-w - write can member and config into web page");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  ZentraleStruct *Zentrale;
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
      ConfigCmdLine(Config, "2:8:a:i:p:fgs:vwl:z:?", NumArgs, ValArgs);
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
            puts("ERROR: can not go to backgound");
            Ret = 4;
         }
         else if (ChildPid == 0)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("child running");
            Zentrale = ZentraleCreate();
            if (Zentrale != (ZentraleStruct *)NULL)
            {
               ZentraleInit(Zentrale,
                            ConfigGetIntVal(Config, CfgVerboseVal),
                            ConfigGetIntVal(Config, CfgZentraleVal),
                            ConfigGetStrVal(Config, CfgIfaceVal),
                            ConfigGetStrVal(Config, CfgAddrVal),
                            ConfigGetIntVal(Config, CfgPortVal),
                            ConfigGetStrVal(Config, CfgPathVal),
                            ConfigGetIntVal(Config, CfgProtokollVal),
                            ConfigGetStrVal(Config, CfgStartVal),
                            ConfigGetIntVal(Config, CfgSyncVal),
                            ConfigGetStrVal(Config, CfgWakeUpS88),
                            ConfigGetIntVal(Config, CfgNumLokfkts),
                            ConfigGetIntVal(Config, CfgWriteWebVal));
               ZentraleRun(Zentrale);
               ZentraleDestroy(Zentrale);
               Ret = 0;
            }
            else
            {
               puts("ERROR: can not create Zentrale module");
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
#ifdef DEBUG
         if (ConfigGetIntVal(Config, CfgVerboseVal))
         {
            PmdInit();
         }
#endif
         Now = time(NULL);
         if (ConfigGetIntVal(Config, CfgVerboseVal))
            printf("start with no fork at %s\n", asctime(localtime(&Now)));
         Zentrale = ZentraleCreate();
         if (Zentrale != (ZentraleStruct *)NULL)
         {
            ZentraleInit(Zentrale,
                         ConfigGetIntVal(Config, CfgVerboseVal),
                         ConfigGetIntVal(Config, CfgZentraleVal),
                         ConfigGetStrVal(Config, CfgIfaceVal),
                         ConfigGetStrVal(Config, CfgAddrVal),
                         ConfigGetIntVal(Config, CfgPortVal),
                         ConfigGetStrVal(Config, CfgPathVal),
                         ConfigGetIntVal(Config, CfgProtokollVal),
                         ConfigGetStrVal(Config, CfgStartVal),
                         ConfigGetIntVal(Config, CfgSyncVal),
                         ConfigGetStrVal(Config, CfgWakeUpS88),
                         ConfigGetIntVal(Config, CfgNumLokfkts),
                         ConfigGetIntVal(Config, CfgWriteWebVal));
            ZentraleRun(Zentrale);
            ZentraleDestroy(Zentrale);
            Ret = 0;
         }
         else
         {
            puts("ERROR: can not create Zentrale module");
            Ret = 1;
         }
      }
      ConfigExit(Config);
      ConfigDestroy(Config);
   }
   else
   {
      puts("ERROR: can not create Config module");
      Ret = 3;
   }
   return(Ret);
}
