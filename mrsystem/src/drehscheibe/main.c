#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#include "drehscheibe.h"

#define SOFTWARE_VERSION "1.02"

static void usage(char *name)
{
   printf("dehscheibe V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> ) | -?\n", name);
   puts("-a - network address to listen");
   puts("-i - interface to drehscheibe");
   puts("-p - port to listen");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  DrehscheibeStruct *Drehscheibe;
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
      ConfigCmdLine(Config, "a:i:p:fs:v?", NumArgs, ValArgs);
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
            return(2);
         }
         else if (ChildPid == 0)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("child running");
            Drehscheibe = DrehscheibeCreate();
            if (Drehscheibe != (DrehscheibeStruct *)NULL)
            {
               DrehscheibeInit(Drehscheibe,
                               ConfigGetIntVal(Config, CfgVerboseVal),
                               ConfigGetStrVal(Config, CfgIfaceVal),
                               ConfigGetStrVal(Config, CfgAddrVal),
                               ConfigGetIntVal(Config, CfgPortVal));
               DrehscheibeRun(Drehscheibe);
               DrehscheibeDestroy(Drehscheibe);
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
         Drehscheibe = DrehscheibeCreate();
         if (Drehscheibe != (DrehscheibeStruct *)NULL)
         {
            DrehscheibeInit(Drehscheibe, ConfigGetIntVal(Config, CfgVerboseVal),
                            ConfigGetStrVal(Config, CfgIfaceVal),
                            ConfigGetStrVal(Config, CfgAddrVal),
                            ConfigGetIntVal(Config, CfgPortVal));
            DrehscheibeRun(Drehscheibe);
            DrehscheibeDestroy(Drehscheibe);
            Ret = 0;
         }
         else
         {
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
