#include <stdio.h>
#include "uci.h"


void UciWrTxt(FILE *DefFilePointer, FILE *CfgFilePointer, char *Name,
              unsigned int MinChars, unsigned int MaxChars, char *Value)
{
   UciWrDefTxt(DefFilePointer, Name, MinChars, MaxChars, Name);
   UciWrCfgStr(CfgFilePointer, Name, Value);
}
