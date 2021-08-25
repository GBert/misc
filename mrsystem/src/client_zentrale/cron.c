#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <boolean.h>
#include <fsm.h>
#include "cron.h"

typedef struct {
   time_t ActualTime;
   int NewState;
} CronCheckWalkStruct;

#define CronCheckWalkStructSetActualTime(Data,Val) (Data)->ActualTime=Val
#define CronCheckWalkStructSetNewState(Data,Val  ) (Data)->NewState=Val

#define CronCheckWalkStructGetActualTime(Data) (Data)->ActualTime
#define CronCheckWalkStructGetNewState(Data)   (Data)->NewState

CronStruct *CronCreate(void)
{  CronStruct *NewData;

   NewData = (CronStruct *)malloc(sizeof(CronStruct));
   if (NewData != (CronStruct *)NULL)
   {
      CronSetCronFkts(NewData, MapCreate());
      if (CronGetCronFkts(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (CronStruct *)NULL;
      }
   }
   return(NewData);
}

void CronDestroy(CronStruct *Data)
{
   if (CronGetCronFkts(Data) != (Map *)NULL)
      MapDestroy(CronGetCronFkts(Data));
   free(Data);
}

void CronInit(CronStruct *Data)
{
   MapInit(CronGetCronFkts(Data), (CmpFkt)strcmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void CronExit(CronStruct *Data)
{
}

void CronAdd(CronStruct *Data, char *Name, unsigned int Intervall,
             CronFktType CronFkt, void *PrivData)
{  CronEntryStruct *Entry;

   Entry = (CronEntryStruct *)malloc(sizeof(CronEntryStruct));
   if (Entry != (CronEntryStruct *)NULL)
   {
      CronEntrySetName(Entry, Name);
      CronEntrySetIsActive(Entry, TRUE);
      CronEntrySetIsOneTime(Entry, FALSE);
      CronEntrySetIntervall(Entry, Intervall);
      CronEntrySetNextTrigger(Entry, time((time_t*)NULL) + Intervall);
      CronEntrySetCronFkt(Entry, CronFkt);
      CronEntrySetPrivData(Entry, PrivData);
      MapSet(CronGetCronFkts(Data),
             (MapKeyType)CronEntryGetName(Entry),
             (MapDataType)Entry);
   }
}

void CronEnable(CronStruct *Data, char *Name)
{  CronEntryStruct *Entry;

   Entry = (CronEntryStruct *)MapGet(CronGetCronFkts(Data), (MapKeyType)Name);
   if (Entry != (CronEntryStruct *)NULL)
   {
      CronEntrySetIsActive(Entry, TRUE);
      CronEntrySetNextTrigger(Entry, time((time_t*)NULL) +
                              CronEntryGetIntervall(Entry));
   }
}

void CronResume(CronStruct *Data, char *Name)
{  CronEntryStruct *Entry;

   Entry = (CronEntryStruct *)MapGet(CronGetCronFkts(Data), (MapKeyType)Name);
   if (Entry != (CronEntryStruct *)NULL)
   {
      CronEntrySetIsActive(Entry, TRUE);
   }
}

void CronDisable(CronStruct *Data, char *Name)
{  CronEntryStruct *Entry;

   Entry = (CronEntryStruct *)MapGet(CronGetCronFkts(Data), (MapKeyType)Name);
   if (Entry != (CronEntryStruct *)NULL)
   {
      CronEntrySetIsActive(Entry, FALSE);
   }
}

static void CheckCron(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CronEntryStruct *CronEntry;
   CronCheckWalkStruct *CronWalkData;
   int NewState;

   CronEntry = (CronEntryStruct *)Daten;
   CronWalkData = (CronCheckWalkStruct *)PrivData;
   if ((CronCheckWalkStructGetActualTime(CronWalkData) > CronEntryGetNextTrigger(CronEntry)) &&
       CronEntryGetIsActive(CronEntry))
   {
      NewState = CronEntryGetCronFkt(CronEntry)(CronEntryGetPrivData(CronEntry));
      if (NewState != STATE_NO_CHANGE)
      {
         CronCheckWalkStructSetNewState(CronWalkData, NewState);
      }
      CronEntrySetNextTrigger(CronEntry,
                              CronCheckWalkStructGetActualTime(CronWalkData) +
                              CronEntryGetIntervall(CronEntry));
   }
}

int CronDo(CronStruct *Data)
{  CronCheckWalkStruct CronWalkData;

   CronCheckWalkStructSetActualTime(&CronWalkData, time((time_t *)NULL));
   CronCheckWalkStructSetNewState(&CronWalkData, STATE_NO_CHANGE);
   MapWalkAscend(CronGetCronFkts(Data), (MapWalkCbFkt)CheckCron,
                 (void *)&CronWalkData);
   return(CronCheckWalkStructGetNewState(&CronWalkData));
}
