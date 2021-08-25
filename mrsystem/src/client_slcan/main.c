#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#include "../common/ms2.h"
#include "slcan_client.h"

#define SOFTWARE_VERSION "1.01"

static void usage(char *name)
{
   printf("mrcc V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> [-i <can if>] [-z <zentrale>]) | -?\n", name);
   puts("-a - network address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-c - name of can interface");
   puts("-z - 0=mrsystem start as slave, 1=start zentrale as master");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Ms2Struct *Ms2Client;
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
      ConfigCmdLine(Config, "a:i:p:c:fs:vz:?", NumArgs, ValArgs);
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
            Ret = 4;
         }
         else if (ChildPid == 0)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("child running");
            IoFunctions = SlcanClientInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                          ConfigGetStrVal(Config, CfgCanIfVal));
            if (IoFunctions != (IoFktStruct *)NULL)
            {
               Ms2Client = Ms2Create();
               if (Ms2Client != (Ms2Struct *)NULL)
               {
                  Ms2Init(Ms2Client, ConfigGetIntVal(Config, CfgVerboseVal),
                          ConfigGetStrVal(Config, CfgIfaceVal),
                          ConfigGetStrVal(Config, CfgAddrVal),
                          ConfigGetIntVal(Config, CfgPortVal),
                          ConfigGetIntVal(Config, CfgZentraleVal),
                          IoFunctions);
                  Ms2Run(Ms2Client);
                  Ms2Destroy(Ms2Client);
                  Ret = 0;
               }
               else
               {
                  Ret = 1;
               }
               SlcanClientExit(IoFunctions);
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
         IoFunctions = SlcanClientInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                       ConfigGetStrVal(Config, CfgCanIfVal));
         if (IoFunctions != (IoFktStruct *)NULL)
         {
            Ms2Client = Ms2Create();
            if (Ms2Client != (Ms2Struct *)NULL)
            {
               Ms2Init(Ms2Client, ConfigGetIntVal(Config, CfgVerboseVal),
                       ConfigGetStrVal(Config, CfgIfaceVal),
                       ConfigGetStrVal(Config, CfgAddrVal),
                       ConfigGetIntVal(Config, CfgPortVal),
                       ConfigGetIntVal(Config, CfgZentraleVal),
                       IoFunctions);
               Ms2Run(Ms2Client);
               Ms2Destroy(Ms2Client);
               Ret = 0;
            }
            else
            {
               Ret = 1;
            }
            SlcanClientExit(IoFunctions);
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
