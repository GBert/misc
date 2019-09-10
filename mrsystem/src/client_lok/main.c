#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <config.h>
#include "lok.h"

#define SOFTWARE_VERSION "1.00"

static void usage(char *name)
{
   printf("mrlok V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-v] [-f] [-a <addr> | -i <iface>] -p <port> [-l path] ) | -?\n", name);
   puts("-a - network address of drehscheibe");
   puts("-i - interface to drehscheibe");
   puts("-p - port of drehscheibe");
   puts("-v - verbose (additional outputs)");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  LokStruct *Lok;
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
      ConfigCmdLine(Config, "a:i:p:s:v?", NumArgs, ValArgs);
      if (ConfigGetIntVal(Config, CfgUsageVal))
      {
         usage(argv[0]);
         Ret = 1;
      }
      Now = time(NULL);
      if (ConfigGetIntVal(Config, CfgVerboseVal))
         printf("start with no fork at %s\n", asctime(localtime(&Now)));
      Lok = LokCreate();
      if (Lok != (LokStruct *)NULL)
      {
         LokInit(Lok, ConfigGetIntVal(Config, CfgVerboseVal),
                 ConfigGetStrVal(Config, CfgIfaceVal),
                 ConfigGetStrVal(Config, CfgAddrVal),
                 ConfigGetIntVal(Config, CfgPortVal),
                 ConfigGetStrVal(Config, CfgPathVal));
         LokRun(Lok);
         LokDestroy(Lok);
         Ret = 0;
      }
      else
      {
         Ret = 2;
      }
      ConfigExit(Config);
      ConfigDestroy(Config);
   }
   else
   {
      Ret = 1;
   }
   return(Ret);
}
