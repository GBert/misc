#ifndef CRON_H
#define CRON_H

#include <boolean.h>
#include <map.h>

#define MAX_CRON_NAME_LEN 32

typedef int (*CronFktType)(void *PrivData);

typedef struct {
   char Name[MAX_CRON_NAME_LEN];
   BOOL IsActive;
   BOOL IsOneTime;
   unsigned int Intervall;
   time_t NextTrigger;
   CronFktType CronFkt;
   void *PrivData;
} CronEntryStruct;

#define CronEntrySetName(Data,Val)        strncpy((Data)->Name,Val,MAX_CRON_NAME_LEN)
#define CronEntrySetIsActive(Data,Val)    (Data)->IsActive=Val
#define CronEntrySetIsOneTime(Data,Val)   (Data)->IsOneTime=Val
#define CronEntrySetIntervall(Data,Val)   (Data)->Intervall=Val
#define CronEntrySetNextTrigger(Data,Val) (Data)->NextTrigger=Val
#define CronEntrySetCronFkt(Data,Val)     (Data)->CronFkt=Val
#define CronEntrySetPrivData(Data,Val)    (Data)->PrivData=Val

#define CronEntryGetName(Data)        (Data)->Name
#define CronEntryGetIsActive(Data)    (Data)->IsActive
#define CronEntryGetIsOneTime(Data)   (Data)->IsOneTime
#define CronEntryGetIntervall(Data)   (Data)->Intervall
#define CronEntryGetNextTrigger(Data) (Data)->NextTrigger
#define CronEntryGetCronFkt(Data)     (Data)->CronFkt
#define CronEntryGetPrivData(Data)    (Data)->PrivData

typedef struct {
   Map *CronFkts;
} CronStruct;

#define CronSetCronFkts(Data,Val) (Data)->CronFkts=Val

#define CronGetCronFkts(Data) (Data)->CronFkts

CronStruct *CronCreate(void);
void CronDestroy(CronStruct *Data);
void CronInit(CronStruct *Data);
void CronExit(CronStruct *Data);
void CronAdd(CronStruct *Data, char *Name, unsigned int Intervall,
             CronFktType CronFkt, void *PrivData);
void CronEnable(CronStruct *Data, char *Name);
void CronResume(CronStruct *Data, char *Name);
void CronDisable(CronStruct *Data, char *Name);
int CronDo(CronStruct *Data);

#endif
