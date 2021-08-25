#include <stdlib.h>
#include <string.h>
#include <cs2parse.h>
#include "zentrale.h"
#include "cs2cfg.h"


Cs2CfgData *Cs2CfgDataCreate(void)
{  Cs2CfgData *NewData;

   NewData = (Cs2CfgData *)malloc(sizeof(Cs2CfgData));
   if (NewData != (Cs2CfgData *)NULL)
   {
      Cs2CfgDataSetCfgDatas(NewData, MapCreate());
      if (Cs2CfgDataGetCfgDatas(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (Cs2CfgData *)NULL;
      }
   }
   return(NewData);
}

void Cs2CfgDataDestroy(Cs2CfgData *Data)
{
   MapDestroy(Cs2CfgDataGetCfgDatas(Data));
   free(Data);
}

static int Cs2DataCmp(void *d1, void *d2)
{
   return((unsigned int)d1 - (unsigned int)d2);
}

void Cs2CfgDataInit(Cs2CfgData *Data, BOOL Verbose)
{
   Cs2CfgDataSetVerbose(Data, Verbose);
   MapInit(Cs2CfgDataGetCfgDatas(Data), (CmpFkt)Cs2DataCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void Cs2CfgDataExit(Cs2CfgData *Data)
{
}

BOOL Cs2CfgDataStart(Cs2CfgData *Data, unsigned int CanHash,
                     unsigned long Length, BOOL IsZlibCompressed)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)malloc(sizeof(Cs2CfgEntry));
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      Cs2CfgEntrySetCfgLength(Entry, Length);
      Cs2CfgEntrySetIsZlibCompressed(Entry, IsZlibCompressed);
      Cs2CfgEntrySetCfgHaveRead(Entry, 0l);
      Cs2CfgEntrySetCfgBuffer(Entry, (char *)malloc(Length + 7));
      if (Cs2CfgEntryGetCfgBuffer(Entry) != (char *)NULL)
      {
         MapSet(Cs2CfgDataGetCfgDatas(Data), (MapKeyType)CanHash,
                (MapDataType)Entry);
      }
      else
      {
         free(Entry);
         Entry = (Cs2CfgEntry *)NULL;
      }
   }
   return(Entry != (Cs2CfgEntry *)NULL);
}

void Cs2CfgDataSetLen(Cs2CfgData *Data, unsigned int CanHash,
                      unsigned long Length)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      Cs2CfgEntrySetCfgLength(Entry, Length);
   }
}

BOOL Cs2CfgDataAllRead(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetCfgHaveRead(Entry) >= Cs2CfgEntryGetCfgLength(Entry));
   }
   else
   {
      return(FALSE);
   }
}

BOOL Cs2CfgDataNextBuf(Cs2CfgData *Data, unsigned int CanHash, char *Buf)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      memcpy(Cs2CfgEntryGetCfgBuffer(Entry) + Cs2CfgEntryGetCfgHaveRead(Entry),
             Buf, 8);
      Cs2CfgEntrySetCfgHaveRead(Entry, Cs2CfgEntryGetCfgHaveRead(Entry) + 8);
      return(Cs2CfgEntryGetCfgHaveRead(Entry) >= Cs2CfgEntryGetCfgLength(Entry));
   }
   else
   {
      return(FALSE);
   }
}

unsigned long Cs2CfgDataGetLength(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetCfgLength(Entry));
   }
   else
   {
      return(0);
   }
}

unsigned long Cs2CfgDataGetHaveRead(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetCfgHaveRead(Entry));
   }
   else
   {
      return(0);
   }
}

char *Cs2CfgDataGetBuf(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetCfgBuffer(Entry));
   }
   else
   {
      return((char *)NULL);
   }
}

unsigned long Cs2CfgDataGetLen(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetCfgHaveRead(Entry));
   }
   else
   {
      return(0);
   }
}

BOOL Cs2CfgDataGetIsCompressed(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      return(Cs2CfgEntryGetIsZlibCompressed(Entry));
   }
   else
   {
      return(TRUE);
   }
}

void Cs2CfgDataEnd(Cs2CfgData *Data, unsigned int CanHash)
{  Cs2CfgEntry *Entry;

   Entry = (Cs2CfgEntry *)MapGet(Cs2CfgDataGetCfgDatas(Data),
                                 (MapKeyType)CanHash);
   if (Entry != (Cs2CfgEntry *)NULL)
   {
      Cs2CfgEntrySetCfgLength(Entry, 0);
      Cs2CfgEntrySetCfgHaveRead(Entry, 0l);
      if (Cs2CfgEntryGetCfgBuffer(Entry) != (char *)NULL)
      {
         free(Cs2CfgEntryGetCfgBuffer(Entry));
         Cs2CfgEntrySetCfgBuffer(Entry, (char *)NULL);
      }
      MapDel(Cs2CfgDataGetCfgDatas(Data), (MapKeyType)CanHash);
   }
}
