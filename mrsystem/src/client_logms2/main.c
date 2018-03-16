#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <boolean.h>
#include <config.h>
#include "can_io.h"
#include "can_client.h"
#include "logms2.h"

#define SOFTWARE_VERSION "1.02"

static void usage(char *name)
{
   printf("mrlogms2 V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [[-a <addr> | -i <iface>] -p <port>] [-i <can if>] ) | -?\n", name);
   puts("-a - ip address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-c - name of can interface");
   puts("-f - dont fork to go in background");
   puts("-v - verbose");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Logms2Struct *Logms2;
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
      ConfigCmdLine(Config, "a:i:c:p:fs:v?", NumArgs, ValArgs);
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
            /* error in fork */
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("ERROR: can not go to backgound");
            Ret = 4;
         }
         else if (ChildPid == 0)
         {
            if (ConfigGetIntVal(Config, CfgVerboseVal))
               puts("child running");
            IoFunctions = CanClientInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                        ConfigGetStrVal(Config, CfgCanIfVal));
            if (IoFunctions != (IoFktStruct *)NULL)
            {
               Logms2 = Logms2Create();
               if (Logms2 != (Logms2Struct *)NULL)
               {
                  Logms2Init(Logms2, ConfigGetIntVal(Config, CfgVerboseVal),
                             ConfigGetStrVal(Config, CfgIfaceVal),
                             ConfigGetStrVal(Config, CfgAddrVal),
                             ConfigGetIntVal(Config, CfgPortVal),
                             IoFunctions);
                  Logms2Run(Logms2);
                  Logms2Destroy(Logms2);
                  Ret = 0;
               }
               else
               {
                  Ret = 2;
               }
               CanClientExit(IoFunctions);
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
         IoFunctions = CanClientInit(ConfigGetIntVal(Config, CfgVerboseVal),
                                     ConfigGetStrVal(Config, CfgCanIfVal));
         if (IoFunctions != (IoFktStruct *)NULL)
         {
            Logms2 = Logms2Create();
            if (Logms2 != (Logms2Struct *)NULL)
            {
               Logms2Init(Logms2, ConfigGetIntVal(Config, CfgVerboseVal),
                          ConfigGetStrVal(Config, CfgIfaceVal),
                          ConfigGetStrVal(Config, CfgAddrVal),
                          ConfigGetIntVal(Config, CfgPortVal),
                          IoFunctions);
               Logms2Run(Logms2);
               Logms2Destroy(Logms2);
               Ret = 0;
            }
            else
            {
               Ret = 2;
            }
            CanClientExit(IoFunctions);
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
