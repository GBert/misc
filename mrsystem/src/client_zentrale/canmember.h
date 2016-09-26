#ifndef CANMEMBER_H
#define CANMEMBER_H

#include <boolean.h>
#include <map.h>


#define S88_UID_PREFIX 0x53380000ul

typedef struct {
   BOOL IsInvalid;
   unsigned long Uid;
   unsigned int Version;
   unsigned int Type;
} CanMemberInfo;

#define CanMemberInfoSetIsInvalid(Data, Val) (Data)->IsInvalid=Val
#define CanMemberInfoSetUid(Data, Val)       (Data)->Uid=Val
#define CanMemberInfoSetVersion(Data, Val)   (Data)->Version=Val
#define CanMemberInfoSetType(Data, Val)      (Data)->Type=Val

#define CanMemberInfoGetIsInvalid(Data) (Data)->IsInvalid
#define CanMemberInfoGetUid(Data)       (Data)->Uid
#define CanMemberInfoGetVersion(Data)   (Data)->Version
#define CanMemberInfoGetType(Data)      (Data)->Type

typedef struct {
   int NumCanMember;
   Map *CanMemberDb;
} CanMemberStruct;

#define CanMemberSetNumMembers(Data,Val)  (Data)->NumCanMember=Val
#define CanMemberSetCanMemberDb(Data,Val) (Data)->CanMemberDb=Val

#define CanMemberGetNumMembers(Data)  (Data)->NumCanMember
#define CanMemberGetCanMemberDb(Data) (Data)->CanMemberDb

CanMemberStruct *CanMemberCreate(void);
void CanMemberDestroy(CanMemberStruct *Data);
void CanMemberInit(CanMemberStruct *Data);
void CanMemberExit(CanMemberStruct *Data);
void CanMemberClear(CanMemberStruct *Data);
void CanMemberInsert(CanMemberStruct *Data, CanMemberInfo *CanMember);
void CanMemberDelete(CanMemberStruct *Data, CanMemberInfo *CanMember);
CanMemberInfo *CanMemberSearch(CanMemberStruct *Data, unsigned long Uid);
void CanMemberMarkAllInvalid(CanMemberStruct *Data);
void CanMemberDelAllInvalid(CanMemberStruct *Data);

#endif
