#include <stdio.h>
#include "uci.h"


void UciWrUciHdr(FILE *FilePointer, char *CfgName, UciSelType SelType)
{
   fputs("//sysconf.uci\n", FilePointer);
   fputs("// cfg list for system configuration\n", FilePointer);
   fputs("//last change: 16.09.2018\n", FilePointer);
   fprintf(FilePointer, "cfgname = %s\n", CfgName);
   fputs("//define type of selector (one of \"but\", \"tab\" or \"sel\"), \"but\" is used as default if nothing is defined\n", FilePointer);
   switch (SelType)
   {
      case UciSelTypeBut:
         fputs("seltype = but\n", FilePointer);
         break;
      case UciSelTypeSel:
         fputs("seltype = sel\n", FilePointer);
         break;
      case UciSelTypeTab:
         /* fall trought */
      default:
         fputs("seltype = tab\n", FilePointer);
         break;
   }
   fputs("\n", FilePointer);
   fputs("// tabtext cfg-file definitions-file (optionally quoted)\n", FilePointer);
}
