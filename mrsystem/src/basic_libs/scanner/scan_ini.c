#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"

void ScanInit(Scanner *Handle, char *DateiName, char *Zeile, int Laenge,
              int AnzKeywords, ScanKeyword *Keywords)
{  int i;

   if (DateiName != NULL)
   {
      Handle->Puffer = Handle->StaticPuffer;
      Handle->ScanFile = fopen(DateiName, "r");
      if (Handle->ScanFile != (FILE *)NULL)
      {
         Handle->AnzZeichen = (int)fread(Handle->Puffer, 1,
                                         SCAN_BLOCK_LAENGE,
                                         Handle->ScanFile);
      }
   }
   else
   {
      Handle->ScanFile = (FILE *)NULL;
      Handle->Puffer = Zeile;
      Handle->AnzZeichen = Laenge;
   }
   Handle->AktZeichen = 0;
   if ((AnzKeywords > 0) && (Keywords != (ScanKeyword *)NULL))
   {
      Handle->Keywords = (ScanKeyword *)calloc(AnzKeywords,
                          sizeof(ScanKeyword));
      if (Handle->Keywords != (ScanKeyword *)NULL)
      {
         Handle->NumKeywords = AnzKeywords;
         for (i = 0; i < Handle->NumKeywords; i++)
         {
            strcpy(Handle->Keywords[i].Keyword, Keywords[i].Keyword);
            Handle->Keywords[i].Symbol = Keywords[i].Symbol;
         }
      }
      else
      {
         Handle->NumKeywords = 0;
      }
   }
   else
   {
      Handle->NumKeywords = 0;
      Handle->Keywords = (ScanKeyword *)NULL;
   }
}
