#include <stdlib.h>
#include <string.h>
#include <scanner.h>
#include "inipars.h"

void IniParsInit(IniParsStruct *Data, char *DateiName, int AnzKeywords,
                IniParsKeyword *Keywords)
{  ScanKeyword *ScannerKeywords;
   int i;

   ScannerKeywords = (ScanKeyword *)malloc(AnzKeywords * sizeof(ScanKeyword));
   if (ScannerKeywords != (ScanKeyword *)NULL)
   {
      IniParsSetKeywords(Data,
                         (IniParsKeyword *)malloc(AnzKeywords *
                                                  sizeof(IniParsKeyword)));
      if (IniParsGetKeywords(Data) != (IniParsKeyword *)NULL)
      {
         IniParsSetAnzKeywords(Data, AnzKeywords);
         for (i = 0; i < AnzKeywords; i++)
         {
            strcpy(ScannerKeywords[i].Keyword, Keywords[i].Keyword);
            ScannerKeywords[i].Symbol = Keywords[i].Symbol;
            strcpy(IniParsGetKeywords(Data)[i].Keyword, Keywords[i].Keyword);
            IniParsGetKeywords(Data)[i].Symbol = Keywords[i].Symbol;
            IniParsGetKeywords(Data)[i].DataType = Keywords[i].DataType;
         }
         ScanInit(IniParsGetScanner(Data), DateiName, NULL, 0, AnzKeywords,
                  ScannerKeywords);
      }
      free(ScannerKeywords);
   }
}
