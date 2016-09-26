#include <stdlib.h>
#include <unistd.h>
#include <inipars.h>
#include "config.h"


void ConfigCmdLine(ConfigStruct *Data, char *optstr, int argc, char *argv[])
{  int c;

   do {
      c = getopt(argc, argv, optstr);
      switch (c)
      {
         case '8':
            ConfigAddStrVal(Data, CfgWakeUpS88, optarg);
            break;
         case 'b':
            ConfigAddIntVal(Data, CfgBcVal, TRUE);
            ConfigAddStrVal(Data, CfgUdpBcVal, optarg);
            break;
         case 'c':
            ConfigAddStrVal(Data, CfgCanIfVal, optarg);
            break;
         case 'f':
            ConfigAddIntVal(Data, CfgForkVal, FALSE);
            break;
         case 'g':
            ConfigAddStrVal(Data, CfgStartVal, optarg);
            break;
         case 'i':
            ConfigAddStrVal(Data, CfgIfaceVal, optarg);
            break;
         case 'l':
            ConfigAddStrVal(Data, CfgPathVal, optarg);
            break;
         case 'p':
            ConfigAddIntVal(Data, CfgPortVal, atoi(optarg));
            break;
         case 's':
            ConfigAddIntVal(Data, CfgProtokollVal, atoi(optarg));
            break;
         case 't':
            ConfigAddIntVal(Data, CfgTraceVal, TRUE);
            break;
         case 'v':
            ConfigAddIntVal(Data, CfgVerboseVal, TRUE);
            break;
         case 'z':
            ConfigAddIntVal(Data, CfgZentraleVal, atoi(optarg));
            break;
         case 'x':
            ConfigAddIntVal(Data, CfgConnTcpVal, TRUE);
            break;
         case '?':
            ConfigAddIntVal(Data, CfgUsageVal, TRUE);
            break;
         default:
            break;
      }
   } while (c != -1);
}
