#ifndef CANMEMBER_H
#define CANMEMBER_H

#include <boolean.h>
#include <map.h>
#include "config.h"


#define S88_UID_PREFIX 0x53380000ul

typedef struct {
   BOOL IsInvalid;
   unsigned long Uid;
   unsigned int Version;
   unsigned int Type;
   BOOL ConfigQuerried;
   ConfigStatusInfo *ConfigStatus;
} CanMemberInfo;

#define CanMemberInfoSetIsInvalid(Data, Val)      (Data)->IsInvalid=Val
#define CanMemberInfoSetUid(Data, Val)            (Data)->Uid=Val
#define CanMemberInfoSetVersion(Data, Val)        (Data)->Version=Val
#define CanMemberInfoSetType(Data, Val)           (Data)->Type=Val
#define CanMemberInfoSetConfigQuerried(Data, Val) (Data)->ConfigQuerried=Val
#define CanMemberInfoSetConfigStatus(Data, Val)   (Data)->ConfigStatus=Val

#define CanMemberInfoGetIsInvalid(Data)      (Data)->IsInvalid
#define CanMemberInfoGetUid(Data)            (Data)->Uid
#define CanMemberInfoGetVersion(Data)        (Data)->Version
#define CanMemberInfoGetType(Data)           (Data)->Type
#define CanMemberInfoGetConfigQuerried(Data) (Data)->ConfigQuerried
#define CanMemberInfoGetConfigStatus(Data)   (Data)->ConfigStatus

typedef struct {
   int NumCanMember;
   Map *CanMemberDb;
   BOOL IsChanged;
} CanMemberStruct;

#define CanMemberSetNumMembers(Data,Val)  (Data)->NumCanMember=Val
#define CanMemberSetCanMemberDb(Data,Val) (Data)->CanMemberDb=Val
#define CanMemberSetIsChanged(Data,Val)   (Data)->IsChanged=Val

#define CanMemberGetNumMembers(Data)  (Data)->NumCanMember
#define CanMemberGetCanMemberDb(Data) (Data)->CanMemberDb
#define CanMemberGetIsChanged(Data)   (Data)->IsChanged

#define CanMemberGetNumMesswerte(Data) ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(Data)))
#define CanMemberGetNumKonfigwerte(Data) ConfigGeraetTypGetNumKonfigwerte(ConfigStatusInfoGetGeraeteTyp(CanMemberInfoGetConfigStatus(Data)))

CanMemberInfo *CanMemberInfoCreate(void);
void CanMemberInfoDestroy(CanMemberInfo *Data);
void CanMemberInfoInit(CanMemberInfo *Data);
void CanMemberInfoExit(CanMemberInfo *Data);
CanMemberStruct *CanMemberCreate(void);
void CanMemberDestroy(CanMemberStruct *Data);
void CanMemberClearConfig(CanMemberStruct *Data);
void CanMemberInit(CanMemberStruct *Data);
void CanMemberExit(CanMemberStruct *Data);
void CanMemberClear(CanMemberStruct *Data);
void CanMemberInsert(CanMemberStruct *Data, CanMemberInfo *CanMember);
void CanMemberDelete(CanMemberStruct *Data, CanMemberInfo *CanMember);
CanMemberInfo *CanMemberSearch(CanMemberStruct *Data, unsigned long Uid);
CanMemberInfo *CanMemberSearchNotConfigured(CanMemberStruct *Data);
CanMemberInfo *CanMemberSearchMs2(CanMemberStruct *Data);
void CanMemberMarkAllInvalid(CanMemberStruct *Data);
void CanMemberDelAllInvalid(CanMemberStruct *Data);
void CanMemberSaveMesswerteCs2(CanMemberStruct *Data, char *LocFilePath);
void CanMemberSaveMesswerteWeb(CanMemberStruct *Data, char *LocFilePath,
                               unsigned long LocalUid,
                               unsigned Major, unsigned Minor);

#endif
