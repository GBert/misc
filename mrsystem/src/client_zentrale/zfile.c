#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <zlib.h>
#include <bytestream.h>
#include <boolean.h>
#include <mr_cs2ms2.h>
#include <cs2.h>
#include "zfile.h"

ZlibFile *ZFileCreate(void)
{  ZlibFile *NewData;

   NewData = (ZlibFile *)malloc(sizeof(ZlibFile));
   if (NewData != (ZlibFile *)NULL)
   {
      ZFileSetCrc(NewData, 0);
      ZFileSetLength(NewData, 0l);
      ZFileSetInputData(NewData, (unsigned char *)NULL);
      ZFileSetInputLength(NewData, 0);
      ZFileSetBuffer(NewData, (unsigned char *)NULL);
   }
   return(NewData);
}

void ZFileDestroy(ZlibFile *Data)
{
   free(Data);
}

void ZFileInit(ZlibFile *Data, char *Input, int Length)
{
   ZFileSetInputData(Data, (unsigned char *)Input);
   ZFileSetInputLength(Data, Length);
}

void ZFileExit(ZlibFile *Data)
{
   if (ZFileGetBuffer(Data) != (unsigned char *)NULL)
   {
      free(ZFileGetBuffer(Data));
      ZFileSetBuffer(Data, (unsigned char *)NULL);
   }
}

BOOL ZFileCompress(ZlibFile *Data)
{  int ZlibRet;
   z_stream strm;
   BOOL Ret;

   ZFileSetBuffer(Data, (unsigned char *)malloc(ZFileGetInputLength(Data) +
                                                 sizeof(gz_header)));
   if (ZFileGetBuffer(Data) != (unsigned char *)NULL)
   {
      strm.avail_in = ZFileGetInputLength(Data);
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.next_in = ZFileGetInputData(Data);
      strm.next_out = &(ZFileGetBuffer(Data)[4]);
      strm.avail_out = strm.avail_in;
      ZlibRet = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
      if (ZlibRet == Z_OK)
      {
         ZlibRet = deflate(&strm, Z_FINISH);
         if (ZlibRet == Z_STREAM_END)
         {
            ZlibRet = deflateEnd(&strm);
            ZFileSetLength(Data, ZFileGetInputLength(Data) - strm.avail_out + 4);
            ZFileSetFrameLength(Data, ((ZFileGetLength(Data) + 7) & 0xfff8));
            SetLongToByteArray(ZFileGetBuffer(Data), ZFileGetInputLength(Data));
            if ((ZFileGetFrameLength(Data) - ZFileGetLength(Data)) > 0)
               memset(&(ZFileGetBuffer(Data)[ZFileGetLength(Data)]), 0,
                      ZFileGetFrameLength(Data) - ZFileGetLength(Data));
            ZFileSetCrc(Data,
                        Cs2CalcCrc(ZFileGetBuffer(Data),
                                   ZFileGetFrameLength(Data)));
            Ret = TRUE;
         }
         else
         {
            Ret = FALSE;
         }
      }
      else
      {
         Ret = FALSE;
      }
   }
   else
   {
      Ret = FALSE;
   }
   return(Ret);
}

BOOL ZFileUnCompress(ZlibFile *Data)
{  int ZlibRet;
   z_stream strm;
   BOOL Ret;

   ZFileSetLength(Data, GetLongFromByteArray(ZFileGetInputData(Data)));
   ZFileSetBuffer(Data, (unsigned char *)malloc(ZFileGetLength(Data)));
   if (ZFileGetBuffer(Data) != (unsigned char *)NULL)
   {
      strm.avail_in = ZFileGetInputLength(Data);
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.next_in = &(ZFileGetInputData(Data)[4]);
      strm.next_out = ZFileGetBuffer(Data);
      strm.avail_out = ZFileGetLength(Data);
      ZlibRet = inflateInit(&strm);
      if (ZlibRet == Z_OK)
      {
         ZlibRet = inflate(&strm, Z_NO_FLUSH);
         inflateEnd(&strm);
         if (ZlibRet == Z_STREAM_END)
         {
            Ret = TRUE;
         }
         else
         {
            Ret = FALSE;
         }
      }
      else
      {
         Ret = FALSE;
      }
   }
   else
   {
      Ret = FALSE;
   }
   return(Ret);
}
