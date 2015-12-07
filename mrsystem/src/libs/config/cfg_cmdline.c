#include <unistd.h>
#include <inipars.h>
#include "config.h"


void ConfigCmdLine(ConfigStruct *Data, char *optstr, int argc, char *argv[])
{  int c;

   do {
      c = getopt(argc, argv, optstr);
      switch (c)
      {
         case 'b':
            ConfigAddIntVal(Data, CfgUdpBcVal, TRUE);
            ConfigAddStrVal(Data, CfgBcVal, optarg);
            break;
         case 'c':
            ConfigAddStrVal(Data, CfgCanIfVal, optarg);
            break;
         case 'f':
            ConfigAddIntVal(Data, CfgForkVal, FALSE);
            break;
         case 'i':
            ConfigAddStrVal(Data, CfgIfaceVal, optarg);
            break;
         case 'p':
            ConfigAddIntVal(Data, CfgPortVal, atoi(optarg));
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
         case '?':
            ConfigAddIntVal(Data, CfgUsageVal, TRUE);
            break;
         default:
            break;
      }
   } while (c != -1);
}
