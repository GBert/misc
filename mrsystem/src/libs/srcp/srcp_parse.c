#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <boolean.h>
#include <scanner.h>
#include "srcp_parse.h"


#define PARSER_VALUE_STRING_SET            "SET"
#define PARSER_VALUE_STRING_GET            "GET"
#define PARSER_VALUE_STRING_CHECK          "CHECK"
#define PARSER_VALUE_STRING_WAIT           "WAIT"
#define PARSER_VALUE_STRING_INIT           "INIT"
#define PARSER_VALUE_STRING_TERM           "TERM"
#define PARSER_VALUE_STRING_VERIFY         "VERIFY"
#define PARSER_VALUE_STRING_RESET          "RESET"
#define PARSER_VALUE_STRING_GO             "GO"
#define PARSER_VALUE_STRING_SRCP           "SRCP"
#define PARSER_VALUE_STRING_PROTOCOL       "PROTOCOL"
#define PARSER_VALUE_STRING_CONNECTIONMODE "CONNECTIONMODE"
#define PARSER_VALUE_STRING_WRITE          "WRITE"
#define PARSER_VALUE_STRING_READ           "READ"
#define PARSER_VALUE_STRING_GL          "GL"
#define PARSER_VALUE_STRING_GA          "GA"
#define PARSER_VALUE_STRING_FB          "FB"
#define PARSER_VALUE_STRING_GM          "GM"
#define PARSER_VALUE_STRING_SM          "SM"
#define PARSER_VALUE_STRING_TIME        "TIME"
#define PARSER_VALUE_STRING_LOCK        "LOCK"
#define PARSER_VALUE_STRING_POWER       "POWER"
#define PARSER_VALUE_STRING_SERVER      "SERVER"
#define PARSER_VALUE_STRING_SESSION     "SESSION"
#define PARSER_VALUE_STRING_DESCRIPTION "DESCRIPTION"
#define PARSER_VALUE_STRING_ON  "ON"
#define PARSER_VALUE_STRING_OFF "OFF"


#define PARSER_TOKEN_KEYWORD_SET            0x0101
#define PARSER_TOKEN_KEYWORD_GET            0x0102
#define PARSER_TOKEN_KEYWORD_CHECK          0x0103
#define PARSER_TOKEN_KEYWORD_WAIT           0x0104
#define PARSER_TVKEN_KEYWORD_INIT           0x0105
#define PARSER_TOKEN_KEYWORD_TERM           0x0106
#define PARSER_TOKEN_KEYWORD_VERIFY         0x0107
#define PARSER_TOKEN_KEYWORD_RESET          0x0108
#define PARSER_TOKEN_KEYWORD_GO             0x0109
#define PARSER_TOKEN_KEYWORD_SRCP           0x0111
#define PARSER_TOKEN_KEYWORD_PROTOCOL       0x0112
#define PARSER_TOKEN_KEYWORD_CONNECTIONMODE 0x0113
#define PARSER_TOKEN_KEYWORD_WRITE          0x0114
#define PARSER_TOKEN_KEYWORD_READ           0x0115
#define PARSER_TOKEN_KEYWORD_GL          0x0121
#define PARSER_TOKEN_KEYWORD_GA          0x0122
#define PARSER_TOKEN_KEYWORD_FB          0x0123
#define PARSER_TOKEN_KEYWORD_GM          0x0124
#define PARSER_TOKEN_KEYWORD_SM          0x0125
#define PARSER_TOKEN_KEYWORD_TIME        0x0126
#define PARSER_TOKEN_KEYWORD_LOCK        0x0127
#define PARSER_TOKEN_KEYWORD_POWER       0x0128
#define PARSER_TOKEN_KEYWORD_SERVER      0x0129
#define PARSER_TOKEN_KEYWORD_SESSION     0x012A
#define PARSER_TOKEN_KEYWORD_DESCRIPTION 0x012B
#define PARSER_TOKEN_KEYWORD_ON  0x0130
#define PARSER_TOKEN_KEYWORD_OFF 0x0131


static ScanKeyword Keywords[] = {
   { PARSER_VALUE_STRING_SET,            PARSER_TOKEN_KEYWORD_SET },
   { PARSER_VALUE_STRING_GET,            PARSER_TOKEN_KEYWORD_GET },
   { PARSER_VALUE_STRING_CHECK,          PARSER_TOKEN_KEYWORD_CHECK },
   { PARSER_VALUE_STRING_WAIT,           PARSER_TOKEN_KEYWORD_WAIT },
   { PARSER_VALUE_STRING_INIT,           PARSER_TVKEN_KEYWORD_INIT },
   { PARSER_VALUE_STRING_TERM,           PARSER_TOKEN_KEYWORD_TERM },
   { PARSER_VALUE_STRING_VERIFY,         PARSER_TOKEN_KEYWORD_VERIFY },
   { PARSER_VALUE_STRING_RESET,          PARSER_TOKEN_KEYWORD_RESET },
   { PARSER_VALUE_STRING_GO,             PARSER_TOKEN_KEYWORD_GO },
   { PARSER_VALUE_STRING_SRCP,           PARSER_TOKEN_KEYWORD_SRCP },
   { PARSER_VALUE_STRING_PROTOCOL,       PARSER_TOKEN_KEYWORD_PROTOCOL },
   { PARSER_VALUE_STRING_CONNECTIONMODE, PARSER_TOKEN_KEYWORD_CONNECTIONMODE },
   { PARSER_VALUE_STRING_WRITE,          PARSER_TOKEN_KEYWORD_WRITE },
   { PARSER_VALUE_STRING_READ,           PARSER_TOKEN_KEYWORD_READ },
   /* set subcomands */
   { PARSER_VALUE_STRING_GL,             PARSER_TOKEN_KEYWORD_GL },
   { PARSER_VALUE_STRING_GA,             PARSER_TOKEN_KEYWORD_GA },
   { PARSER_VALUE_STRING_FB,             PARSER_TOKEN_KEYWORD_FB },
   { PARSER_VALUE_STRING_GM,             PARSER_TOKEN_KEYWORD_GM },
   { PARSER_VALUE_STRING_SM,             PARSER_TOKEN_KEYWORD_SM },
   { PARSER_VALUE_STRING_TIME,           PARSER_TOKEN_KEYWORD_TIME },
   { PARSER_VALUE_STRING_LOCK,           PARSER_TOKEN_KEYWORD_LOCK },
   { PARSER_VALUE_STRING_POWER,          PARSER_TOKEN_KEYWORD_POWER },
   { PARSER_VALUE_STRING_SERVER,         PARSER_TOKEN_KEYWORD_SERVER },
   { PARSER_VALUE_STRING_SESSION,        PARSER_TOKEN_KEYWORD_SESSION },
   { PARSER_VALUE_STRING_DESCRIPTION,    PARSER_TOKEN_KEYWORD_DESCRIPTION },
   /* parameter to subcomands */
   { PARSER_VALUE_STRING_ON,             PARSER_TOKEN_KEYWORD_ON },
   { PARSER_VALUE_STRING_OFF,            PARSER_TOKEN_KEYWORD_OFF },
};
static SrcpParser *InternalParser = (SrcpParser *)NULL;


SrcpParser *SrcpParserCreate(void)
{  SrcpParser *NewData;

   NewData = (SrcpParser *)malloc(sizeof(SrcpParser));
   if (NewData != NULL)
   {
      SrcpParserSetScanner(NewData, ScanCreate());
      if (SrcpParserGetScanner(NewData) == (Scanner *)NULL)
      {
         free(NewData);
         NewData = (SrcpParser *)NULL;
      }
   }
   return(NewData);
}

void SrcpParserDestroy(SrcpParser *Data)
{
   ScanDestroy(SrcpParserGetScanner(Data));
   free(Data);
}

void SrcpParserInit(SrcpParser *Data, char *InputLine, int Len)
{
   SrcpParserSetVerbose(Data, FALSE);
   ScanInit(SrcpParserGetScanner(Data), (char *)NULL, InputLine, Len,
            27, Keywords);
}

BOOL SrcpParserInternalInit(char *InputLine, int Len)
{  BOOL Ret;

   if (InternalParser == (SrcpParser *)NULL)
   {
      InternalParser = SrcpParserCreate();
      if (InternalParser != (SrcpParser *)NULL)
      {
         SrcpParserInit(InternalParser, InputLine, Len);
         Ret = TRUE;
      }
      else
      {
         Ret = FALSE;
      }
   }
   else
   {
      SrcpParserInit(InternalParser, InputLine, Len);
      Ret = TRUE;
   }
   return(Ret);
}

void SrcpParserExit(SrcpParser *Data)
{
}

void SrcpParserInternalExit(void)
{
   SrcpParserExit(InternalParser);
}

static void PrintError(SrcpParser *Data, char *ErrorString)
{  int i;

   if (SrcpParserGetVerbose(Data))
   {
      fprintf(stderr, "\nERROR: %s\n%s\n", ErrorString,
              SrcpParserGetScanner(Data)->Puffer);
      for (i = 0; i < SrcpParserGetScanner(Data)->AktZeichen; i++)
         fputc(' ', stderr);
      fputs("^\n\n", stderr);
   }
}

int SrcpParserParse(SrcpParser *Data)
{  int Token, Ret;

   Token = Scan(SrcpParserGetScanner(Data));
   switch (Token)
   {
      case EofSy:
      case EolnSy:
         if (SrcpParserGetVerbose(Data))
            puts("Parser: EOF reached");
         Ret = PARSER_EOF;
         break;
      case GzahlSy:
         Ret = PARSER_ZAHL;
         break;
      case PunktSy:
         Ret = PARSER_PUNKT;
         break;
      case BezeichnerSy:
         Ret = PARSER_BEZEICHNER;
         break;
      case PARSER_TOKEN_KEYWORD_SET:
         Ret = PARSER_SET;
         break;
      case PARSER_TOKEN_KEYWORD_GET:
         Ret = PARSER_GET;
         break;
      case PARSER_TOKEN_KEYWORD_CHECK:
         Ret = PARSER_CHECK;
         break;
      case PARSER_TOKEN_KEYWORD_WAIT:
         Ret = PARSER_WAIT;
         break;
      case PARSER_TVKEN_KEYWORD_INIT:
         Ret = PARSER_INIT;
         break;
      case PARSER_TOKEN_KEYWORD_TERM:
         Ret = PARSER_TERM;
         break;
      case PARSER_TOKEN_KEYWORD_VERIFY:
         Ret = PARSER_VERIFY;
         break;
      case PARSER_TOKEN_KEYWORD_RESET:
         Ret = PARSER_RESET;
         break;
      case PARSER_TOKEN_KEYWORD_GO:
         Ret = PARSER_GO;
         break;
      case PARSER_TOKEN_KEYWORD_SRCP:
         Ret = PARSER_SRCP;
         break;
      case PARSER_TOKEN_KEYWORD_PROTOCOL:
         Ret = PARSER_PROTOCOL;
         break;
      case PARSER_TOKEN_KEYWORD_CONNECTIONMODE:
         Ret = PARSER_CONNECTIONMODE;
         break;
      case PARSER_TOKEN_KEYWORD_WRITE:
         Ret = PARSER_WRITE;
         break;
      case PARSER_TOKEN_KEYWORD_READ:
         Ret = PARSER_READ;
         break;
      case PARSER_TOKEN_KEYWORD_GL:
         Ret = PARSER_GL;
         break;
      case PARSER_TOKEN_KEYWORD_GA:
         Ret = PARSER_GA;
         break;
      case PARSER_TOKEN_KEYWORD_FB:
         Ret = PARSER_FB;
         break;
      case PARSER_TOKEN_KEYWORD_GM:
         Ret = PARSER_GM;
         break;
      case PARSER_TOKEN_KEYWORD_SM:
         Ret = PARSER_SM;
         break;
      case PARSER_TOKEN_KEYWORD_TIME:
         Ret = PARSER_TIME;
         break;
      case PARSER_TOKEN_KEYWORD_LOCK:
         Ret = PARSER_LOCK;
         break;
      case PARSER_TOKEN_KEYWORD_POWER:
         Ret = PARSER_POWER;
         break;
      case PARSER_TOKEN_KEYWORD_SERVER:
         Ret = PARSER_SERVER;
         break;
      case PARSER_TOKEN_KEYWORD_SESSION:
         Ret = PARSER_SESSION;
         break;
      case PARSER_TOKEN_KEYWORD_DESCRIPTION:
         Ret = PARSER_DESCRIPTION;
      case PARSER_TOKEN_KEYWORD_ON:
         Ret = PARSER_ON;
         break;
      case PARSER_TOKEN_KEYWORD_OFF:
         Ret = PARSER_OFF;
         break;
      default:
         if (SrcpParserGetVerbose(Data))
            printf("Parser: token 0x%x\n", Token);
         PrintError(Data, "Unexpected character!");
         Ret = PARSER_ERROR;
         break;
   }
   return(Ret);
}

int SrcpParserInternalParse(void)
{
   return(SrcpParserParse(InternalParser));
}

char *SrcpParserInternalGetString(void)
{
   return(ScanGetString(SrcpParserGetScanner(InternalParser)));
}

int SrcpParserInternalGetGanz(void)
{
   return(ScanGanz(SrcpParserGetScanner(InternalParser)));
}
