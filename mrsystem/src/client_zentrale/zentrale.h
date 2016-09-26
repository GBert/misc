#ifndef ZENTRALE_H
#define ZENTRALE_H

#include <boolean.h>
#include <fsm.h>
#include "lok.h"
#include "magnetartikel.h"
#include "gleisbild.h"
#include "gleisbildpage.h"
#include "fahrstrasse.h"
#include "zfile.h"

#define MM2_PRG_TEXT "mm2_prg"

#define MM2_PRG_TYP 0x01

typedef struct {
   char Name[17];
} ZentraleLokName;

typedef struct {
   BOOL Verbosity;
   BOOL IsMaster;
   char *Interface;
   char *Address;
   int ServerPort;
   int ClientSock;
   FsmStruct *StateMachine;
   char *LocPath;
   unsigned MajorVersion;
   unsigned MinorVersion;
   float HardVersion;
   unsigned SerialNumber;
   unsigned long GfpUid;
   unsigned long Uid;
   ZlibFile *PackedCs2File;
   BOOL HaveDb;
   unsigned long CfgLength;
   unsigned long CfgHaveRead;
   char *CfgBuffer;
   int ActualIndex;
   int NumLoks;
   int MaxLoks;
   LokInfo ActualLok;
   ZentraleLokName *LokNamen;
   LokStruct *Loks;
   MagnetartikelStruct *Magnetartikel;
   GleisbildStruct *Gleisbild;
   GleisbildPageStruct **GleisPages;
   FahrstrasseStruct *Fahrstrasse;
} ZentraleStruct;

#define ZentraleSetVerbose(Data, Verbose)               (Data)->Verbosity=Verbose
#define ZentraleSetIsMaster(Data, Master)               (Data)->IsMaster=Master
#define ZentraleSetInterface(Data, Iface)               (Data)->Interface=Iface
#define ZentraleSetAddress(Data, Addr)                  (Data)->Address=Addr
#define ZentraleSetServerPort(Data, Port)               (Data)->ServerPort=Port
#define ZentraleSetClientSock(Data, Sock)               (Data)->ClientSock=Sock
#define ZentraleSetStateMachine(Data, Fsm)              (Data)->StateMachine=Fsm
#define ZentraleSetLocPath(Data, Path)                  (Data)->LocPath=Path
#define ZentraleSetMajorVersion(Data, Major)            (Data)->MajorVersion=Major
#define ZentraleSetMinorVersion(Data, Minor)            (Data)->MinorVersion=Minor
#define ZentraleSetHardVersion(Data, Hard)              (Data)->HardVersion=Hard
#define ZentraleSetSerialNumber(Data, Serial)           (Data)->SerialNumber=Serial
#define ZentraleSetGfpUid(Data, MyUid)                  (Data)->GfpUid=MyUid
#define ZentraleSetUid(Data, MyUid)                     (Data)->Uid=MyUid
#define ZentraleSetPackedCs2File(Data, Zfile)           (Data)->PackedCs2File=Zfile
#define ZentraleSetCfgLength(Data, Len)                 (Data)->CfgLength=Len
#define ZentraleSetCfgHaveRead(Data, Len)               (Data)->CfgHaveRead=Len
#define ZentraleSetCfgBuffer(Data, Buf)                 (Data)->CfgBuffer=Buf
#define ZentraleSetActualIndex(Data, i)                 (Data)->ActualIndex=i
#define ZentraleSetNumLoks(Data, i)                     (Data)->NumLoks=i
#define ZentraleSetMaxLoks(Data, i)                     (Data)->MaxLoks=i
#define ZentraleSetLokNamen(Data, Namen)                (Data)->LokNamen=Namen
#define ZentraleSetLokNamenNr(Data, i, Namen)           strcpy((Data)->LokNamen[i].Name, Namen)
#define ZentraleSetLoks(Data, LoksDb)                   (Data)->Loks=LoksDb
#define ZentraleSetMagnetartikel(Data, MagnetartikelDb) (Data)->Magnetartikel=MagnetartikelDb
#define ZentraleSetGleisbild(Data, GleisbildDb)         (Data)->Gleisbild=GleisbildDb
#define ZentraleSetGleisPages(Data, GleisPagesArray)    (Data)->GleisPages=GleisPagesArray
#define ZentraleSetNrGleisPages(Data, i, GleisPage)     (Data)->GleisPages[i]=GleisPage
#define ZentraleSetFahrstrasse(Data, FahrstrasseDb)     (Data)->Fahrstrasse=FahrstrasseDb

#define ZentraleGetVerbose(Data)         (Data)->Verbosity
#define ZentraleGetIsMaster(Data)        (Data)->IsMaster
#define ZentraleGetInterface(Data)       (Data)->Interface
#define ZentraleGetAddress(Data)         (Data)->Address
#define ZentraleGetServerPort(Data)      (Data)->ServerPort
#define ZentraleGetClientSock(Data)      (Data)->ClientSock
#define ZentraleGetStateMachine(Data)    (Data)->StateMachine
#define ZentraleGetLocPath(Data)         (Data)->LocPath
#define ZentraleGetMajorVersion(Data)    (Data)->MajorVersion
#define ZentraleGetMinorVersion(Data)    (Data)->MinorVersion
#define ZentraleGetHardVersion(Data)     (Data)->HardVersion
#define ZentraleGetSerialNumber(Data)    (Data)->Serial
#define ZentraleGetGfpUid(Data)          (Data)->GfpUid
#define ZentraleGetUid(Data)             (Data)->Uid
#define ZentraleGetPackedCs2File(Data)   (Data)->PackedCs2File
#define ZentraleGetCfgLength(Data)       (Data)->CfgLength
#define ZentraleGetCfgHaveRead(Data)     (Data)->CfgHaveRead
#define ZentraleGetCfgBuffer(Data)       (Data)->CfgBuffer
#define ZentraleGetLoks(Data)            (Data)->Loks
#define ZentraleGetActualIndex(Data)     (Data)->ActualIndex
#define ZentraleGetNumLoks(Data)         (Data)->NumLoks
#define ZentraleGetMaxLoks(Data)         (Data)->MaxLoks
#define ZentraleGetActualLok(Data)       (&((Data)->ActualLok))
#define ZentraleGetLokNamen(Data)        (Data)->LokNamen
#define ZentraleGetLokNamenNr(Data, i)   (Data)->LokNamen[i].Name
#define ZentraleGetLoks(Data)            (Data)->Loks
#define ZentraleGetMagnetartikel(Data)   (Data)->Magnetartikel
#define ZentraleGetGleisbild(Data)       (Data)->Gleisbild
#define ZentraleGetGleisPages(Data)      (Data)->GleisPages
#define ZentraleGetNrGleisPages(Data, i) (Data)->GleisPages[i]
#define ZentraleGetFahrstrasse(Data)     (Data)->Fahrstrasse

ZentraleStruct *ZentraleCreate(void);
void ZentraleDestroy(ZentraleStruct *Data);
void ZentraleInit(ZentraleStruct *Data, BOOL Verbose, BOOL IsMaster,
                  char *Interface, char *Addr, int Port, char *LocPath);
void ZentraleInitFsm(ZentraleStruct *Data, BOOL IsMaster);
void ZentraleExit(ZentraleStruct *Data);
void ZentraleRun(ZentraleStruct *Data);

#endif
