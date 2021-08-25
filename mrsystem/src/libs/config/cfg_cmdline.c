#include <stdlib.h>
#include <unistd.h>
#include <inipars.h>
#include "config.h"


int ConfigCmdLine(ConfigStruct *Data, char *optstr, int argc, char *argv[])
{  int c;

   do {
      c = getopt(argc, argv, optstr);
      switch (c)
      {
         case '2':
            ConfigAddIntVal(Data, CfgSyncVal, atoi(optarg));
            break;
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
         case 'e':
            ConfigAddIntVal(Data, CfgEmuHostCom, atoi(optarg));
            break;
         case 'f':
            ConfigAddIntVal(Data, CfgForkVal, FALSE);
            break;
         case 'g':
            ConfigAddStrVal(Data, CfgStartVal, optarg);
            break;
         case 'h':
            ConfigAddStrVal(Data, CfgHideMs2Val, optarg);
            break;
         case 'i':
            ConfigAddStrVal(Data, CfgIfaceVal, optarg);
            break;
         case 'k':
            ConfigAddIntVal(Data, CfgNumLokfkts, atoi(optarg));
            break;
         case 'l':
            ConfigAddStrVal(Data, CfgPathVal, optarg);
            break;
         case 'm':
            ConfigAddStrVal(Data, CfgSerialLineVal, optarg);
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
         case 'w':
            ConfigAddIntVal(Data, CfgWriteWebVal, TRUE);
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
   return(optind);
}
