#include <stdio.h>
#include "uci.h"


void UciWrInt(FILE *DefFilePointer, FILE *CfgFilePointer, char *Name,
              int MinVal, int MaxVal, int Value)
{
   UciWrDefInt(DefFilePointer, Name, MinVal, MaxVal, Name);
   UciWrCfgInt(CfgFilePointer, Name, Value);
}
