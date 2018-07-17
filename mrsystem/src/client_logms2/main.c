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
#include "../client_ms2/can_client.h"
#include "../common/logms2.h"

#define SOFTWARE_VERSION "1.05"

static void usage(char *name)
{
   printf("mrlogms2 V%s\nUsage:\n", SOFTWARE_VERSION);
   printf("%s ([-c <can if>] ) | -?\n", name);
   puts("-c - name of can interface");
   puts("-? - this help");
}

int main(int argc, char *argv[])
{  Logms2Struct *Logms2;
   IoFktStruct *IoFunctions;
   ConfigStruct *Config;
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
      ConfigCmdLine(Config, "c:?", NumArgs, ValArgs);
      if (ConfigGetIntVal(Config, CfgUsageVal))
      {
         usage(argv[0]);
         Ret = 1;
      }
      Now = time(NULL);
      if (ConfigGetIntVal(Config, CfgVerboseVal))
         printf("start with no fork at %s\n", asctime(localtime(&Now)));
      IoFunctions = CanClientInit(1, ConfigGetStrVal(Config, CfgCanIfVal));
      if (IoFunctions != (IoFktStruct *)NULL)
      {
         Logms2 = Logms2Create();
         if (Logms2 != (Logms2Struct *)NULL)
         {
            Logms2Init(Logms2, 1, IoFunctions);
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
   return(Ret);
}
