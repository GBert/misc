#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <boolean.h>
#include <map.h>
#include <menge.h>
#include "canmember.h"

CanMemberStruct *CanMemberCreate(void)
{  CanMemberStruct *NewData;

   NewData = (CanMemberStruct *)malloc(sizeof(CanMemberStruct));
   if (NewData != (CanMemberStruct *)NULL)
   {
      CanMemberSetNumMembers(NewData, 0);
      CanMemberSetCanMemberDb(NewData, MapCreate());
      if (CanMemberGetCanMemberDb(NewData) == (Map *)NULL)
      {
         free(NewData);
         NewData = (CanMemberStruct *)NULL;
      }
   }
   return(NewData);
}

void CanMemberDestroy(CanMemberStruct *Data)
{
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
      MapDestroy(CanMemberGetCanMemberDb(Data));
   free(Data);
}

static int CanMemberUidCmp(void *d1, void *d2)
{
   return((unsigned long)d1 - (unsigned long)d2);
}

void CanMemberInit(CanMemberStruct *Data)
{
   CanMemberSetNumMembers(Data, 0);
   MapInit(CanMemberGetCanMemberDb(Data), (CmpFkt)CanMemberUidCmp,
           (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
}

void CanMemberExit(CanMemberStruct *Data)
{
}

void CanMemberClear(CanMemberStruct *Data)
{
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
      MapDestroy(CanMemberGetCanMemberDb(Data));
   CanMemberSetCanMemberDb(Data, MapCreate());
   CanMemberSetNumMembers(Data, 0);
   if (CanMemberGetCanMemberDb(Data) != (Map *)NULL)
   {
      MapInit(CanMemberGetCanMemberDb(Data), (CmpFkt)CanMemberUidCmp,
              (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
   }
}

void CanMemberInsert(CanMemberStruct *Data, CanMemberInfo *CanMember)
{  unsigned long Uid;
   CanMemberInfo *OldCanMember;

   Uid = CanMemberInfoGetUid(CanMember);
   OldCanMember = (CanMemberInfo *)MapGet(CanMemberGetCanMemberDb(Data),
                                          (MapKeyType)Uid);
   if (OldCanMember != (CanMemberInfo *)NULL)
   {
      CanMemberInfoSetIsInvalid(OldCanMember, FALSE);
      CanMemberInfoSetUid(OldCanMember, CanMemberInfoGetUid(CanMember));
      CanMemberInfoSetVersion(OldCanMember, CanMemberInfoGetVersion(CanMember));
      CanMemberInfoSetType(OldCanMember, CanMemberInfoGetType(CanMember));
   }
   else
   {
      OldCanMember = (CanMemberInfo *)malloc(sizeof(CanMemberInfo));
      if (OldCanMember != (CanMemberInfo *)NULL)
      {
         CanMemberInfoSetIsInvalid(OldCanMember,  FALSE);
         CanMemberInfoSetUid(OldCanMember, CanMemberInfoGetUid(CanMember));
         CanMemberInfoSetVersion(OldCanMember, CanMemberInfoGetVersion(CanMember));
         CanMemberInfoSetType(OldCanMember, CanMemberInfoGetType(CanMember));
         MapSet(CanMemberGetCanMemberDb(Data),
                (MapKeyType)Uid, (MapDataType)OldCanMember);
         CanMemberSetNumMembers(Data, CanMemberGetNumMembers(Data) + 1);
      }
   }
}

void CanMemberDelete(CanMemberStruct *Data, CanMemberInfo *CanMember)
{  unsigned long Uid;

   Uid = CanMemberInfoGetUid(CanMember);
   MapDel(CanMemberGetCanMemberDb(Data), (MapKeyType)Uid);
   CanMemberSetNumMembers(Data, CanMemberGetNumMembers(Data) - 1);
}

CanMemberInfo *CanMemberSearch(CanMemberStruct *Data, unsigned long Uid)
{
   return((CanMemberInfo *)MapGet(CanMemberGetCanMemberDb(Data),
                                  (MapKeyType)Uid));
}

static void MarkInvalid(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;

   Data = (CanMemberInfo *)Daten;
   CanMemberInfoSetIsInvalid(Data, TRUE);
}

void CanMemberMarkAllInvalid(CanMemberStruct *Data)
{
   MapWalkAscend(CanMemberGetCanMemberDb(Data), (MapWalkCbFkt)MarkInvalid,
                 (void *)NULL);
}

typedef struct {
   Map *CanMemberDb;
   Menge *MemberToDel;
} CollectStruct;

static void CollectInvalid(void *PrivData, MapKeyType Key, MapDataType Daten)
{  CanMemberInfo *Data;
   CollectStruct *Collector;
   unsigned long *UidToDel;

   Data = (CanMemberInfo *)Daten;
   Collector = (CollectStruct *)PrivData;
   if (CanMemberInfoGetIsInvalid(Data))
   {
      UidToDel = (unsigned long *)malloc(sizeof(unsigned long));
      if (UidToDel != (unsigned long *)NULL)
      {
         *UidToDel = CanMemberInfoGetUid(Data);
         MengeAdd(Collector->MemberToDel, (MengeDataType)UidToDel);
      }
   }
}

int MemberDelCmp(void *d1, void *d2)
{
   return((int)((unsigned long)d1 - (unsigned long)d2));
}

void CanMemberDelAllInvalid(CanMemberStruct *Data)
{  Menge *MemberToDel;
   MengeIterator IterToDel;
   CollectStruct Collector;
   unsigned long *UidToDel;

   MemberToDel = MengeCreate();
   MengeInit(MemberToDel, MemberDelCmp, (MengeDelCbFkt)free);
   Collector.CanMemberDb = CanMemberGetCanMemberDb(Data);
   Collector.MemberToDel = MemberToDel;
   MapWalkAscend(CanMemberGetCanMemberDb(Data), (MapWalkCbFkt)CollectInvalid,
                 (void *)&Collector);
   MengeInitIterator(&IterToDel, MemberToDel);
   UidToDel = (unsigned long *)MengeFirst(&IterToDel);
   while (UidToDel != 0l)
   {
      MapDel(CanMemberGetCanMemberDb(Data), (MapKeyType)*UidToDel);
      UidToDel = (unsigned long *)MengeNext(&IterToDel);
   }
   MengeDestroy(MemberToDel);
}
