#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <boolean.h>
#include "write_cs2.h"


#define PATH "/var/www/config/"


static char *FileNames[] = {
   "",
   CS2_FILE_STRING_LOKOMOTIVE
};

static char *ParagraphNames[] = {
   "",
   CS2_PARAGRAPH_STRING_LOK,
   CS2_PARAGRAPH_STRING_NUMLOKS,
   CS2_PARAGRAPH_STRING_LOKOMOTIVE,
   CS2_PARAGRAPH_STRING_GLEISBILD,
   CS2_PARAGRAPH_STRING_GLEISBILDSEITE,
   CS2_PARAGRAPH_STRING_MAGNETARTIKEL,
   CS2_PARAGRAPH_STRING_FAHRSTRASSEN
};

static char *TypNames[] = {
   "",
   CS2_TYP_STRING_MM2_DIL8,
   CS2_TYP_STRING_MM2_LOK
};


char *Cs2GetTypAsString(int Typ)
{
   return(TypNames[Typ]);
}

FILE *Cs2OpenByName(char *Name)
{
   return(fopen(Name, "w"));
}

FILE *Cs2OpenByType(int Cs2FileType)
{  char FullPath[256];

   sprintf(FullPath, "%s%s", PATH, FileNames[Cs2FileType]);
   return(fopen(FullPath, "w"));
}

void Cs2Close(FILE *Stream)
{
   fclose(Stream);
}

void Cs2WriteParagraphByName(FILE *Stream, char *Paragraph)
{
   fprintf(Stream, "[%s]\n", Paragraph);
}

void Cs2WriteParagraphByType(FILE *Stream, int Cs2ParagraphType)
{
   fprintf(Stream, "[%s]\n", ParagraphNames[Cs2ParagraphType]);
}

void Cs2WriteTitleByName(FILE *Stream, char *Title, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s\n", Title);
}

void Cs2WriteStringValueByName(FILE *Stream, char *Title, char *Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=%s\n", Title, Value);
}

void Cs2WriteIntValueByName(FILE *Stream, char *Title, int Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=%d\n", Title, Value);
}

void Cs2WriteHexValueByName(FILE *Stream, char *Title, int Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=0x%x\n", Title, Value);
}

void Cs2WriteULongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=0x%lx\n", Title, Value);
}

void Cs2WriteHexLongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=0x%lx\n", Title, Value);
}

void Cs2WriteFloatValueByName(FILE *Stream, char *Title, float Value, int Level)
{  int i;

   if (Level > 0)
      fputc(' ', Stream);
   for (i = 0; i < Level; i++)
      fputc('.', Stream);
   fprintf(Stream, "%s=%1.3f\n", Title, (double)Value);
}
