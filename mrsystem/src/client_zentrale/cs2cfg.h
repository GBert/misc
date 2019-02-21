#ifndef CS2CFG_H
#define CS2CFG_H

#include <boolean.h>
#include <map.h>

typedef struct {
   unsigned long CfgLength;
   BOOL IsZlibCompressed;
   unsigned long CfgHaveRead;
   char *CfgBuffer;
} Cs2CfgEntry;

#define Cs2CfgEntrySetCfgLength(Data, Len)           (Data)->CfgLength=Len
#define Cs2CfgEntrySetIsZlibCompressed(Data, IsComp) (Data)->IsZlibCompressed=IsComp
#define Cs2CfgEntrySetCfgHaveRead(Data, Len)         (Data)->CfgHaveRead=Len
#define Cs2CfgEntrySetCfgBuffer(Data, Buf)           (Data)->CfgBuffer=Buf

#define Cs2CfgEntryGetCfgLength(Data)        (Data)->CfgLength
#define Cs2CfgEntryGetIsZlibCompressed(Data) (Data)->IsZlibCompressed
#define Cs2CfgEntryGetCfgHaveRead(Data)      (Data)->CfgHaveRead
#define Cs2CfgEntryGetCfgBuffer(Data)        (Data)->CfgBuffer

typedef struct {
   BOOL Verbose;
   Map *CfgDatas;
} Cs2CfgData;

#define Cs2CfgDataSetVerbose(Data, Verb)   (Data)->Verbose=Verb
#define Cs2CfgDataSetCfgDatas(Data, Datas) (Data)->CfgDatas=Datas

#define Cs2CfgDataGetVerbose(Data)  (Data)->Verbose
#define Cs2CfgDataGetCfgDatas(Data) (Data)->CfgDatas

Cs2CfgData *Cs2CfgDataCreate(void);
void Cs2CfgDataDestroy(Cs2CfgData *Data);
void Cs2CfgDataInit(Cs2CfgData *Data, BOOL Verbose);
void Cs2CfgDataExit(Cs2CfgData *Data);
BOOL Cs2CfgDataStart(Cs2CfgData *Data, unsigned int CanHash,
                     unsigned long Length, BOOL IsZlibCompressed);
void Cs2CfgDataSetLen(Cs2CfgData *Data, unsigned int CanHash,
                      unsigned long Length);
BOOL Cs2CfgDataAllRead(Cs2CfgData *Data, unsigned int CanHash);
BOOL Cs2CfgDataNextBuf(Cs2CfgData *Data, unsigned int CanHash, char *Buf);
unsigned long Cs2CfgDataGetLength(Cs2CfgData *Data, unsigned int CanHash);
unsigned long Cs2CfgDataGetHaveRead(Cs2CfgData *Data, unsigned int CanHash);
char *Cs2CfgDataGetBuf(Cs2CfgData *Data, unsigned int CanHash);
unsigned long Cs2CfgDataGetLen(Cs2CfgData *Data, unsigned int CanHash);
BOOL Cs2CfgDataGetIsCompressed(Cs2CfgData *Data, unsigned int CanHash);
void Cs2CfgDataEnd(Cs2CfgData *Data, unsigned int CanHash);

#endif
