#ifndef SRCP_PARSE_H
#define SRCP_PARSE_H


#include <boolean.h>
#include <scanner.h>


#define PARSER_ERROR      0x00
#define PARSER_EOF        0x01
#define PARSER_ZAHL       0x02
#define PARSER_PUNKT      0x03
#define PARSER_BEZEICHNER 0x04
#define PARSER_SET            0x05
#define PARSER_GET            0x06
#define PARSER_CHECK          0x07
#define PARSER_WAIT           0x08
#define PARSER_INIT           0x09
#define PARSER_TERM           0x0A
#define PARSER_VERIFY         0x0B
#define PARSER_RESET          0x0C
#define PARSER_GO             0x0D
#define PARSER_SRCP           0x0E
#define PARSER_PROTOCOL       0x0F
#define PARSER_CONNECTIONMODE 0x10
#define PARSER_WRITE          0x11
#define PARSER_READ           0x12
#define PARSER_GL          0x13
#define PARSER_GA          0x14
#define PARSER_FB          0x15
#define PARSER_GM          0x16
#define PARSER_SM          0x17
#define PARSER_TIME        0x18
#define PARSER_LOCK        0x19
#define PARSER_POWER       0x1A
#define PARSER_SERVER      0x1B
#define PARSER_SESSION     0x1C
#define PARSER_DESCRIPTION 0x1D
#define PARSER_ON  0x1C
#define PARSER_OFF 0x1D

typedef struct {
   BOOL Verbose;
   Scanner *Scanner;
   int Type;
} SrcpParser;

#define SrcpParserSetVerbose(Data, Value) (Data)->Verbose=Value
#define SrcpParserSetScanner(Data, Scn)   (Data)->Scanner=Scn
#define SrcpParserSetType(Data, Value)    (Data)->Type=Val

#define SrcpParserGetVerbose(Data) (Data)->Verbose
#define SrcpParserGetScanner(Data) (Data)->Scanner
#define SrcpParserGetType(Data)    (Data)->Type


SrcpParser *SrcpParserCreate(void);
void SrcpParserDestroy(SrcpParser *Data);
void SrcpParserInit(SrcpParser *Data, char *InputLine, int Len);
void SrcpParserExit(SrcpParser *Data);
int SrcpParserParse(SrcpParser *Data);
BOOL SrcpParserInternalInit(char *InputLine, int Len);
void SrcpParserInternalExit(void);
int SrcpParserInternalParse(void);
char *SrcpParserInternalGetString(void);
int SrcpParserInternalGetGanz(void);

#endif
