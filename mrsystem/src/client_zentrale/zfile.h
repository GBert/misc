#ifndef ZFILE_H
#define ZFILE_H

#include <boolean.h>

#define LOK_DATEI "lokomotive.cs2"

typedef struct {
   unsigned short Crc;
   unsigned long Length;
   unsigned long FrameLength;
   unsigned char *InputData;
   int InputLength;
   unsigned char *Buffer;
} ZlibFile;

#define ZFileSetCrc(Data,crc)         Data->Crc=crc
#define ZFileSetLength(Data,len)      Data->Length=len
#define ZFileSetFrameLength(Data,len) Data->FrameLength=len
#define ZFileSetInputData(Data,buf)   Data->InputData=buf
#define ZFileSetInputLength(Data,len) Data->InputLength=len
#define ZFileSetBuffer(Data,buf)      Data->Buffer=buf

#define ZFileGetCrc(Data)         Data->Crc
#define ZFileGetLength(Data)      Data->Length
#define ZFileGetFrameLength(Data) Data->FrameLength
#define ZFileGetInputData(Data)   Data->InputData
#define ZFileGetInputLength(Data) Data->InputLength
#define ZFileGetBuffer(Data)      Data->Buffer

ZlibFile *ZFileCreate(void);
void ZFileDestroy(ZlibFile *Data);
void ZFileInit(ZlibFile *Data, char *Input, int Length);
void ZFileExit(ZlibFile *Data);
BOOL ZFileCompress(ZlibFile *Data);
BOOL ZFileUnCompress(ZlibFile *Data);

#endif
