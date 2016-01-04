#include <stdlib.h>
#include <stdio.h>
#include <boolean.h>
#include <map.h>
#include <scanner.h>
#include "inipars.h"


BOOL IniParsReadFile(IniParsStruct *Data, Map *CfgValues)
{  int Token, i;
   BOOL Ret;
   IniValue *ValuePtr;

   Ret = TRUE;
   do {
      Token = Scan(IniParsGetScanner(Data));
      if (Token == EofSy)
      {
      }
      else if (Token == HashSy)
      {
         ScanUeberlesen(IniParsGetScanner(Data));
         Token = Scan(IniParsGetScanner(Data));
      }
      else
      {
         i = 0;
         while ((i < IniParsGetAnzKeywords(Data)) &&
                (IniParsGetKeywords(Data)[i].Symbol != Token))
         {
            i++;
         }
         if (i < IniParsGetAnzKeywords(Data))
         {
            Token = Scan(IniParsGetScanner(Data));
            if ((IniParsGetKeywords(Data)[i].DataType == IniParsString) &&
                (Token == ZeichenkettenSy))
            {
               ValuePtr = (IniValue *)MapGet(CfgValues,
                  (MapKeyType)IniParsGetKeywords(Data)[i].Keyword);
               if (ValuePtr == (IniValue *)NULL)
               {
                  ValuePtr = malloc(sizeof(IniValue));
                  ValuePtr->DataType = IniParsString;
                  ValuePtr->IntValue = 0;
                  ScanString(IniParsGetScanner(Data), ValuePtr->StringValue);
                  MapSet(CfgValues,
                         (MapKeyType *)strdup(IniParsGetKeywords(Data)[i].Keyword),
	                 (MapDataType)ValuePtr);
               }
               else
               {
                  ValuePtr->DataType = IniParsString;
                  ValuePtr->IntValue = 0;
                  ScanString(IniParsGetScanner(Data), ValuePtr->StringValue);
               }
            }
            else if ((IniParsGetKeywords(Data)[i].DataType == IniParsInt) &&
                     (Token == GzahlSy))
            {
               ValuePtr = (IniValue *)MapGet(CfgValues,
                  (MapKeyType)IniParsGetKeywords(Data)[i].Keyword);
               if (ValuePtr == (IniValue *)NULL)
               {
                  ValuePtr = malloc(sizeof(IniValue));
                  ValuePtr->DataType = IniParsInt;
                  ValuePtr->IntValue = ScanGanz(IniParsGetScanner(Data));
                  ValuePtr->StringValue[0] = '\0';
                  MapSet(CfgValues,
	                 (MapKeyType)strdup(IniParsGetKeywords(Data)[i].Keyword),
	                 (MapDataType)ValuePtr);
               }
               else
               {
                  ValuePtr->DataType = IniParsInt;
                  ValuePtr->IntValue = ScanGanz(IniParsGetScanner(Data));
                  ValuePtr->StringValue[0] = '\0';
               }
            }
            else
            {
               Ret = FALSE;
            }
         }
         else
         {
            Token = Scan(IniParsGetScanner(Data));
            Ret = FALSE;
         }
         Token = Scan(IniParsGetScanner(Data));
         if (Token == HashSy)
         {
            ScanUeberlesen(IniParsGetScanner(Data));
            Token = Scan(IniParsGetScanner(Data));
         }
      }
   } while (Token != EofSy);
   return(Ret);
}
