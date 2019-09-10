#ifndef ZENTRALE_H
#define ZENTRALE_H

#include <boolean.h>
#include <fsm.h>
#include "canmember.h"
#include "cron.h"
#include "lok.h"
#include "magnetartikel.h"
#include "gleisbild.h"
#include "gleisbildpage.h"
#include "fahrstrasse.h"
#include "zfile.h"
#include "cs2cfg.h"

#define MM2_PRG_TEXT "mm2_prg"

#define MM2_PRG_TYP 0x01

typedef struct {
   char Name[17];
} ZentraleLokName;

#define ZentraleLokNameSetLokName(Data, LokName) strcpy((Data)->Name,LokName)

#define ZentraleLokNameGetLokName(Data) (Data)->Name

#define MASTER_MODE_PROXY      0
#define MASTER_MODE_MS2_MASTER 1
#define MASTER_MODE_MRS_MASTER 2

typedef struct {
   int Length;
   int Interval;
   int TCycle;
} S88SystemStatusConfig;

#define S88SystemStatusConfigSetLength(Data, Value)   (Data)->Length=Value
#define S88SystemStatusConfigSetInterval(Data, Value) (Data)->Interval=Value
#define S88SystemStatusConfigSetTCycle(Data, Value)   (Data)->TCycle=Value

#define S88SystemStatusConfigGetLength(Data)   (Data)->Length
#define S88SystemStatusConfigGetInterval(Data) (Data)->Interval
#define S88SystemStatusConfigGetTCycle(Data)   (Data)->TCycle

typedef struct {
   BOOL Verbosity;
   int MasterMode;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   BOOL ShouldWakeUpS88;
   char *WakeUpS88;
   FsmStruct *StateMachine;
   char *LocPath;
   int Protokolle;
   BOOL SystemStart;
   int SyncMask;
   unsigned MajorVersion;
   unsigned MinorVersion;
   float HardVersion;
   unsigned SerialNumber;
   unsigned long GfpUid;
   unsigned long Uid;
   BOOL WriteWeb;
   ZlibFile *PackedCs2File;
   BOOL HaveDb;
   BOOL IsInPoll;
   Cs2CfgData *Cs2CgfDaten;
   int ActualIndex;
   int NumLoks;
   int MaxLoks;
   LokInfo ActualLok;
   unsigned long ActualConfig;
   MengeIterator ActualKonfigWert;
   ConfigMesswertTyp *ActualMesswert;
   ConfigTyp *ActualKonfig;
   ZentraleLokName *LokNamen;
   CanMemberStruct *CanMember;
   CronStruct *CronJobs;
   LokStruct *Loks;
   MagnetartikelStruct *Magnetartikel;
   GleisbildStruct *Gleisbild;
   GleisbildPageStruct **GleisPages;
   FahrstrasseStruct *Fahrstrasse;
   S88SystemStatusConfig S88Bus[3];
} ZentraleStruct;

#define ZentraleSetVerbose(Data, Verbose)               (Data)->Verbosity=Verbose
#define ZentraleSetMasterMode(Data, Master)             (Data)->MasterMode=Master
#define ZentraleSetInterface(Data, Iface)               (Data)->Interface=Iface
#define ZentraleSetAddress(Data, Addr)                  (Data)->Address=Addr
#define ZentraleSetServerPort(Data, Port)               (Data)->ServerPort=Port
#define ZentraleSetClientSock(Data, Sock)               (Data)->ClientSock=Sock
#define ZentraleSetShouldWakeUpS88(Data, DoWakeUp)      (Data)->ShouldWakeUpS88=DoWakeUp
#define ZentraleSetWakeUpS88(Data, WakeUp)              (Data)->WakeUpS88=WakeUp
#define ZentraleSetStateMachine(Data, Fsm)              (Data)->StateMachine=Fsm
#define ZentraleSetLocPath(Data, Path)                  (Data)->LocPath=Path
#define ZentraleSetProtokolle(Data, Protos)             (Data)->Protokolle=Protos
#define ZentraleSetSystemStart(Data, Start)             (Data)->SystemStart=Start
#define ZentraleSetSyncMask(Data, Mask)                 (Data)->SyncMask=Mask
#define ZentraleSetMajorVersion(Data, Major)            (Data)->MajorVersion=Major
#define ZentraleSetMinorVersion(Data, Minor)            (Data)->MinorVersion=Minor
#define ZentraleSetHardVersion(Data, Hard)              (Data)->HardVersion=Hard
#define ZentraleSetSerialNumber(Data, Serial)           (Data)->SerialNumber=Serial
#define ZentraleSetGfpUid(Data, MyUid)                  (Data)->GfpUid=MyUid
#define ZentraleSetUid(Data, MyUid)                     (Data)->Uid=MyUid
#define ZentraleSetWriteWeb(Data, Write)                (Data)->WriteWeb=Write
#define ZentraleSetIsInPoll(Data, Poll)                 (Data)->IsInPoll=Poll
#define ZentraleSetPackedCs2File(Data, Zfile)           (Data)->PackedCs2File=Zfile
#define ZentraleSetCs2CfgDaten(Data, Cs2Daten)          (Data)->Cs2CgfDaten=Cs2Daten
#define ZentraleSetActualIndex(Data, i)                 (Data)->ActualIndex=i
#define ZentraleSetNumLoks(Data, i)                     (Data)->NumLoks=i
#define ZentraleSetMaxLoks(Data, i)                     (Data)->MaxLoks=i
#define ZentraleSetMaxLoks(Data, i)                     (Data)->MaxLoks=i
#define ZentraleSetActualConfig(Data, Uid)              (Data)->ActualConfig=Uid
#define ZentraleSetActualMesswert(Data, Messwert)       (Data)->ActualMesswert=Messwert
#define ZentraleSetActualKonfig(Data, Konfig)           (Data)->ActualKonfig=Konfig
#define ZentraleSetLokNamen(Data, Namen)                (Data)->LokNamen=Namen
#define ZentraleSetLokNamenNr(Data, i, Namen)           strcpy((Data)->LokNamen[i].Name, Namen)
#define ZentraleSetCanMember(Data, CanMemberDb)         (Data)->CanMember=CanMemberDb
#define ZentraleSetCronJobs(Data, CronTab)              (Data)->CronJobs=CronTab
#define ZentraleSetLoks(Data, LoksDb)                   (Data)->Loks=LoksDb
#define ZentraleSetMagnetartikel(Data, MagnetartikelDb) (Data)->Magnetartikel=MagnetartikelDb
#define ZentraleSetGleisbild(Data, GleisbildDb)         (Data)->Gleisbild=GleisbildDb
#define ZentraleSetGleisPages(Data, GleisPagesArray)    (Data)->GleisPages=GleisPagesArray
#define ZentraleSetNrGleisPages(Data, i, GleisPage)     (Data)->GleisPages[i]=GleisPage
#define ZentraleSetFahrstrasse(Data, FahrstrasseDb)     (Data)->Fahrstrasse=FahrstrasseDb
#define ZentraleSetS88BusIdxLength(Data, i, Val)        (Data)->S88Bus[i].Length=Val
#define ZentraleSetS88BusIdxInterval(Data, i, Val)      (Data)->S88Bus[i].Interval=Val
#define ZentraleSetS88BusIdxTCycle(Data, i, Val)        (Data)->S88Bus[i].TCycle=Val

#define ZentraleGetVerbose(Data)              (Data)->Verbosity
#define ZentraleGetMasterMode(Data)           (Data)->MasterMode
#define ZentraleGetInterface(Data)            (Data)->Interface
#define ZentraleGetAddress(Data)              (Data)->Address
#define ZentraleGetServerPort(Data)           (Data)->ServerPort
#define ZentraleGetShouldWakeUpS88(Data)      (Data)->ShouldWakeUpS88
#define ZentraleGetWakeUpS88(Data)            (Data)->WakeUpS88
#define ZentraleGetClientSock(Data)           (Data)->ClientSock
#define ZentraleGetStateMachine(Data)         (Data)->StateMachine
#define ZentraleGetLocPath(Data)              (Data)->LocPath
#define ZentraleGetProtokolle(Data)           (Data)->Protokolle
#define ZentraleGetSystemStart(Data)          (Data)->SystemStart
#define ZentraleGetSyncMask(Data)             (Data)->SyncMask
#define ZentraleGetMajorVersion(Data)         (Data)->MajorVersion
#define ZentraleGetMinorVersion(Data)         (Data)->MinorVersion
#define ZentraleGetHardVersion(Data)          (Data)->HardVersion
#define ZentraleGetSerialNumber(Data)         (Data)->Serial
#define ZentraleGetGfpUid(Data)               (Data)->GfpUid
#define ZentraleGetUid(Data)                  (Data)->Uid
#define ZentraleGetWriteWeb(Data)             (Data)->WriteWeb
#define ZentraleGetIsInPoll(Data)             (Data)->IsInPoll
#define ZentraleGetPackedCs2File(Data)        (Data)->PackedCs2File
#define ZentraleGetCs2CfgDaten(Data)          (Data)->Cs2CgfDaten
#define ZentraleGetLoks(Data)                 (Data)->Loks
#define ZentraleGetActualIndex(Data)          (Data)->ActualIndex
#define ZentraleGetNumLoks(Data)              (Data)->NumLoks
#define ZentraleGetMaxLoks(Data)              (Data)->MaxLoks
#define ZentraleGetActualLok(Data)            (&((Data)->ActualLok))
#define ZentraleGetActualConfig(Data)         (Data)->ActualConfig
#define ZentraleGetActualKonfigWert(Data)     (Data)->ActualKonfigWert
#define ZentraleGetActualMesswert(Data)       (Data)->ActualMesswert
#define ZentraleGetActualKonfig(Data)         (Data)->ActualKonfig
#define ZentraleGetLokNamen(Data)             (Data)->LokNamen
#define ZentraleGetLokNamenNr(Data, i)        (Data)->LokNamen[i].Name
#define ZentraleGetCanMember(Data)            (Data)->CanMember
#define ZentraleGetCronJobs(Data)             (Data)->CronJobs
#define ZentraleGetLoks(Data)                 (Data)->Loks
#define ZentraleGetMagnetartikel(Data)        (Data)->Magnetartikel
#define ZentraleGetGleisbild(Data)            (Data)->Gleisbild
#define ZentraleGetGleisPages(Data)           (Data)->GleisPages
#define ZentraleGetNrGleisPages(Data, i)      (Data)->GleisPages[i]
#define ZentraleGetFahrstrasse(Data)          (Data)->Fahrstrasse
#define ZentraleGetS88BusIdxLength(Data, i)   (Data)->S88Bus[i].Length
#define ZentraleGetS88BusIdxInterval(Data, i) (Data)->S88Bus[i].Interval
#define ZentraleGetS88BusIdxTCycle(Data, i)   (Data)->S88Bus[i].TCycle

ZentraleStruct *ZentraleCreate(void);
void ZentraleDestroy(ZentraleStruct *Data);
void ZentraleInit(ZentraleStruct *Data, BOOL Verbose, int MasterMode,
                  char *Interface, char *Addr, int Port, char *LocPath,
                  int Protokolle, char *SystemStart, int SyncMask,
                  char *WakeUpS88, int NumLokFkts, BOOL WriteWeb);
void ZentraleInitFsm(ZentraleStruct *Data, int MasterMode);
void ZentraleExit(ZentraleStruct *Data);
void ZentraleRun(ZentraleStruct *Data);

#endif
