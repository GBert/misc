#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <boolean.h>
#include <config.h>
#include "ms1.h"

#define SOFTWARE_VERSION "1.02"

static void usage(char *name)
{
   printf("mrms1 V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> [-i <can if>] ) | -?\n", name);
   puts("-a - ip address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-c - name of can interface");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Ms1Struct *Ms1;
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
      ConfigCmdLine(Config, "a:i:p:c:fs:v?", NumArgs, ValArgs);
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
            Ms1 = Ms1Create();
            if (Ms1 != (Ms1Struct *)NULL)
            {
               Ms1Init(Ms1, ConfigGetIntVal(Config, CfgVerboseVal),
                       ConfigGetStrVal(Config, CfgIfaceVal),
                       ConfigGetStrVal(Config, CfgAddrVal),
                       ConfigGetIntVal(Config, CfgPortVal),
                       ConfigGetStrVal(Config, CfgCanIfVal));
               Ms1Run(Ms1);
               Ms1Destroy(Ms1);
               Ret = 0;
            }
            else
            {
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
         Ms1 = Ms1Create();
         if (Ms1 != (Ms1Struct *)NULL)
         {
            Ms1Init(Ms1, ConfigGetIntVal(Config, CfgVerboseVal),
                    ConfigGetStrVal(Config, CfgIfaceVal),
                    ConfigGetStrVal(Config, CfgAddrVal),
                    ConfigGetIntVal(Config, CfgPortVal),
                    ConfigGetStrVal(Config, CfgCanIfVal));
            Ms1Run(Ms1);
            Ms1Destroy(Ms1);
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
