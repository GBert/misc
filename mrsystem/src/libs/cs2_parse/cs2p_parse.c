#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <boolean.h>
#include <scanner.h>
#include "cs2parse.h"
#include "cs2ptoken.h"

static void PrintError(Cs2parser *Data, char *ErrorString)
{  int i;

   if (Cs2pGetVerbose(Data))
   {
      fprintf(stderr, "\nERROR: %s\n%s\n", ErrorString,
              Cs2pGetScanner(Data)->Puffer);
      for (i = 0; i < Cs2pGetScanner(Data)->AktZeichen; i++)
         fputc(' ', stderr);
      fputs("^\n\n", stderr);
   }
}

static void DoParagraph(Cs2parser *Data)
{  int Token;

   Token = Scan(Cs2pGetScanner(Data));
   if ((Token == PARSER_TOKEN_KEYWORD_LOK) ||
       (Token == PARSER_TOKEN_KEYWORD_NUMLOKS) ||
       (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE) ||
       (Token == PARSER_TOKEN_KEYWORD_GERAET) ||
       (Token == PARSER_TOKEN_KEYWORD_GLEISBILD) ||
       (Token == PARSER_TOKEN_KEYWORD_MAGNETARTIKEL) ||
       (Token == PARSER_TOKEN_KEYWORD_FAHRSTRASSEN) ||
       (Token == PARSER_TOKEN_KEYWORD_GLEISBILDSEITE) ||
       (Token == PARSER_TOKEN_KEYWORD_LOKSTATUS) ||
       (Token == PARSER_TOKEN_KEYWORD_LOKLISTE))
   {
      Cs2pSetName(Data, ScanGetString(Cs2pGetScanner(Data)));
      if (Cs2pGetVerbose(Data))
         printf("Parser: paragraph >%s< token %d\n", Cs2pGetName(Data), Token);
      if (Token == PARSER_TOKEN_KEYWORD_LOK)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_LOK);
      else if (Token == PARSER_TOKEN_KEYWORD_NUMLOKS)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_NUMLOKS);
      else if (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_LOKOMOTIVE);
      else if (Token == PARSER_TOKEN_KEYWORD_GERAET)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_GERAET);
      else if (Token == PARSER_TOKEN_KEYWORD_GLEISBILD)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_GLEISBILD);
      else if (Token == PARSER_TOKEN_KEYWORD_MAGNETARTIKEL)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_MAGNETARTIKEL);
      else if (Token == PARSER_TOKEN_KEYWORD_FAHRSTRASSEN)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_FAHRSTRASSEN);
      else if (Token == PARSER_TOKEN_KEYWORD_GLEISBILDSEITE)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_GLEISBILDSEITE);
      else if (Token == PARSER_TOKEN_KEYWORD_LOKSTATUS)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_LOKSTATUS);
      else if (Token == PARSER_TOKEN_KEYWORD_LOKLISTE)
         Cs2pSetSubType(Data, PARSER_PARAGRAPH_LOKLISTE);
      else
         Cs2pSetSubType(Data, 0);
      Token = Scan(Cs2pGetScanner(Data));
      switch (Token)
      {
         case MengezuSy:
            do {
               Token = Scan(Cs2pGetScanner(Data));
            } while (Token != EolnSy);
            break;
         default:
            PrintError(Data, "Unexpected token for paragraph!");
            break;
      }
   }
   else
   {
      Cs2pSetSubType(Data, 0);
      if (Cs2pGetVerbose(Data))
         printf("Parser: found %d\n", Token);
   }
}

static void DoValue(Cs2parser *Data, int StartToken)
{  int Token;

   Token = StartToken;
   while (Token == PunktSy)
   {
      if (Token == PunktSy)
      {
         Cs2pSetLevel(Data, Cs2pGetLevel(Data) + 1);
      }
      Token = Scan(Cs2pGetScanner(Data));
   }
   if ((Token == PARSER_TOKEN_KEYWORD_LOK) ||
       (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE) ||
       (Token == PARSER_TOKEN_KEYWORD_NAME) ||
       (Token == PARSER_TOKEN_KEYWORD_WERT) ||
       (Token == PARSER_TOKEN_KEYWORD_UID) ||
       (Token == PARSER_TOKEN_KEYWORD_ADRESSE) ||
       (Token == PARSER_TOKEN_KEYWORD_TYP) ||
       (Token == PARSER_TOKEN_KEYWORD_MFXUID) ||
       (Token == PARSER_TOKEN_KEYWORD_SYMBOL) ||
       (Token == PARSER_TOKEN_KEYWORD_AV) ||
       (Token == PARSER_TOKEN_KEYWORD_BV) ||
       (Token == PARSER_TOKEN_KEYWORD_VOLUME) ||
       (Token == PARSER_TOKEN_KEYWORD_VELOCITY) ||
       (Token == PARSER_TOKEN_KEYWORD_RICHTUNG) ||
       (Token == PARSER_TOKEN_KEYWORD_VMAX) ||
       (Token == PARSER_TOKEN_KEYWORD_VMIN) ||
       (Token == PARSER_TOKEN_KEYWORD_FKT) ||
       (Token == PARSER_TOKEN_KEYWORD_DAUER) ||
       (Token == PARSER_TOKEN_KEYWORD_GERAET) ||
       (Token == PARSER_TOKEN_KEYWORD_VERSION) ||
       (Token == PARSER_TOKEN_KEYWORD_MINOR) ||
       (Token == PARSER_TOKEN_KEYWORD_SERNUM) ||
       (Token == PARSER_TOKEN_KEYWORD_GFPUID) ||
       (Token == PARSER_TOKEN_KEYWORD_GUIUID) ||
       (Token == PARSER_TOKEN_KEYWORD_HARDVERS) ||
       (Token == PARSER_TOKEN_KEYWORD_MAJOR) ||
       (Token == PARSER_TOKEN_KEYWORD_SESSION) ||
       (Token == PARSER_TOKEN_KEYWORD_ID) ||
       (Token == PARSER_TOKEN_KEYWORD_SID) ||
       (Token == PARSER_TOKEN_KEYWORD_ICON) ||
       (Token == PARSER_TOKEN_KEYWORD_TACHOMAX) ||
       (Token == PARSER_TOKEN_KEYWORD_XPROTOKOLL) ||
       (Token == PARSER_TOKEN_KEYWORD_MFXTYP) ||
       (Token == PARSER_TOKEN_KEYWORD_STAND) ||
       (Token == PARSER_TOKEN_KEYWORD_FAHRT) ||
       (Token == PARSER_TOKEN_KEYWORD_FUNKTIONEN) ||
       (Token == PARSER_TOKEN_KEYWORD_NR) ||
       (Token == PARSER_TOKEN_KEYWORD_VORWAERTS) ||
       (Token == PARSER_TOKEN_KEYWORD_RUECKWAERTS) ||
       (Token == PARSER_TOKEN_KEYWORD_INTRAKTION) ||
       (Token == PARSER_TOKEN_KEYWORD_SEITE) ||
       (Token == PARSER_TOKEN_KEYWORD_GROESSE) ||
       (Token == PARSER_TOKEN_KEYWORD_ZULETZT_BENUTZT) ||
       (Token == PARSER_TOKEN_KEYWORD_ARTIKEL) ||
       (Token == PARSER_TOKEN_KEYWORD_STELLUNG) ||
       (Token == PARSER_TOKEN_KEYWORD_SCHALTZEIT) ||
       (Token == PARSER_TOKEN_KEYWORD_UNGERADE) ||
       (Token == PARSER_TOKEN_KEYWORD_DECODER) ||
       (Token == PARSER_TOKEN_KEYWORD_DECTYP) ||
       (Token == PARSER_TOKEN_KEYWORD_FAHRSTRASSE) |
       (Token == PARSER_TOKEN_KEYWORD_S88) ||
       (Token == PARSER_TOKEN_KEYWORD_S88EIN) ||
       (Token == PARSER_TOKEN_KEYWORD_EXTERN) ||
       (Token == PARSER_TOKEN_KEYWORD_ITEM) ||
       (Token == PARSER_TOKEN_KEYWORD_MAGNETARTIKEL) ||
       (Token == PARSER_TOKEN_KEYWORD_FSVERWEIS) ||
       (Token == PARSER_TOKEN_KEYWORD_ELEMENT) ||
       (Token == PARSER_TOKEN_KEYWORD_DREHUNG) ||
       (Token == PARSER_TOKEN_KEYWORD_TEXT) ||
       (Token == PARSER_TOKEN_KEYWORD_ZUSTAND) ||
       (Token == PARSER_TOKEN_KEYWORD_DEVICE_ID) ||
       (Token == PARSER_TOKEN_KEYWORD_PAGE) ||
       (Token == PARSER_TOKEN_KEYWORD_SEKUNDE) ||
       (Token == PARSER_TOKEN_KEYWORD_IDX) ||
       (Token == PARSER_TOKEN_KEYWORD_ON) ||
       (Token == PARSER_TOKEN_KEYWORD_DV) ||
       (Token == PARSER_TOKEN_KEYWORD_LLINDEX) ||
       (Token == PARSER_TOKEN_KEYWORD_CRC))
   {
      Cs2pSetName(Data, ScanGetString(Cs2pGetScanner(Data)));
      if (Cs2pGetVerbose(Data))
         printf("Parser: value name >%s<\n", Cs2pGetName(Data));
      if (Token == PARSER_TOKEN_KEYWORD_LOK)
         Cs2pSetSubType(Data, PARSER_VALUE_LOK);
      if (Token == PARSER_TOKEN_KEYWORD_LOKOMOTIVE)
         Cs2pSetSubType(Data, PARSER_VALUE_LOKOMOTIVE);
      else if (Token == PARSER_TOKEN_KEYWORD_NAME)
         Cs2pSetSubType(Data, PARSER_VALUE_NAME);
      else if (Token == PARSER_TOKEN_KEYWORD_WERT)
         Cs2pSetSubType(Data, PARSER_VALUE_WERT);
      else if (Token == PARSER_TOKEN_KEYWORD_UID)
         Cs2pSetSubType(Data, PARSER_VALUE_UID);
      else if (Token == PARSER_TOKEN_KEYWORD_ADRESSE)
         Cs2pSetSubType(Data, PARSER_VALUE_ADRESSE);
      else if (Token == PARSER_TOKEN_KEYWORD_TYP)
         Cs2pSetSubType(Data, PARSER_VALUE_TYP);
      else if (Token == PARSER_TOKEN_KEYWORD_MFXUID)
         Cs2pSetSubType(Data, PARSER_VALUE_MFXUID);
      else if (Token == PARSER_TOKEN_KEYWORD_SYMBOL)
         Cs2pSetSubType(Data, PARSER_VALUE_SYMBOL);
      else if (Token == PARSER_TOKEN_KEYWORD_AV)
         Cs2pSetSubType(Data, PARSER_VALUE_AV);
      else if (Token == PARSER_TOKEN_KEYWORD_BV)
         Cs2pSetSubType(Data, PARSER_VALUE_BV);
      else if (Token == PARSER_TOKEN_KEYWORD_VOLUME)
         Cs2pSetSubType(Data, PARSER_VALUE_VOLUME);
      else if (Token == PARSER_TOKEN_KEYWORD_VELOCITY)
         Cs2pSetSubType(Data, PARSER_VALUE_VELOCITY);
      else if (Token == PARSER_TOKEN_KEYWORD_RICHTUNG)
         Cs2pSetSubType(Data, PARSER_VALUE_RICHTUNG);
      else if (Token == PARSER_TOKEN_KEYWORD_VMAX)
         Cs2pSetSubType(Data, PARSER_VALUE_VMAX);
      else if (Token == PARSER_TOKEN_KEYWORD_VMIN)
         Cs2pSetSubType(Data, PARSER_VALUE_VMIN);
      else if (Token == PARSER_TOKEN_KEYWORD_FKT)
         Cs2pSetSubType(Data, PARSER_VALUE_FKT);
      else if (Token == PARSER_TOKEN_KEYWORD_DAUER)
         Cs2pSetSubType(Data, PARSER_VALUE_DAUER);
      else if (Token == PARSER_TOKEN_KEYWORD_GERAET)
         Cs2pSetSubType(Data, PARSER_VALUE_GERAET);
      else if (Token == PARSER_TOKEN_KEYWORD_VERSION)
         Cs2pSetSubType(Data, PARSER_VALUE_VERSION);
      else if (Token == PARSER_TOKEN_KEYWORD_MINOR)
         Cs2pSetSubType(Data, PARSER_VALUE_MINOR);
      else if (Token == PARSER_TOKEN_KEYWORD_SERNUM)
         Cs2pSetSubType(Data, PARSER_VALUE_SERNUM);
      else if (Token == PARSER_TOKEN_KEYWORD_GFPUID)
         Cs2pSetSubType(Data, PARSER_VALUE_GFPUID);
      else if (Token == PARSER_TOKEN_KEYWORD_GUIUID)
         Cs2pSetSubType(Data, PARSER_VALUE_GUIUID);
      else if (Token == PARSER_TOKEN_KEYWORD_HARDVERS)
         Cs2pSetSubType(Data, PARSER_VALUE_HARDVERS);
      else if (Token == PARSER_TOKEN_KEYWORD_MAJOR)
         Cs2pSetSubType(Data, PARSER_VALUE_MAJOR);
      else if (Token == PARSER_TOKEN_KEYWORD_SESSION)
         Cs2pSetSubType(Data, PARSER_VALUE_SESSION);
      else if (Token == PARSER_TOKEN_KEYWORD_ID)
         Cs2pSetSubType(Data, PARSER_VALUE_ID);
      else if (Token == PARSER_TOKEN_KEYWORD_SID)
         Cs2pSetSubType(Data, PARSER_VALUE_SID);
      else if (Token == PARSER_TOKEN_KEYWORD_ICON)
         Cs2pSetSubType(Data, PARSER_VALUE_ICON);
      else if (Token == PARSER_TOKEN_KEYWORD_TACHOMAX)
         Cs2pSetSubType(Data, PARSER_VALUE_TACHOMAX);
      else if (Token == PARSER_TOKEN_KEYWORD_XPROTOKOLL)
         Cs2pSetSubType(Data, PARSER_VALUE_XPROTOKOLL);
      else if (Token == PARSER_TOKEN_KEYWORD_MFXTYP)
         Cs2pSetSubType(Data, PARSER_VALUE_MFXTYP);
      else if (Token == PARSER_TOKEN_KEYWORD_STAND)
         Cs2pSetSubType(Data, PARSER_VALUE_STAND);
      else if (Token == PARSER_TOKEN_KEYWORD_FAHRT)
         Cs2pSetSubType(Data, PARSER_VALUE_FAHRT);
      else if (Token == PARSER_TOKEN_KEYWORD_FUNKTIONEN)
         Cs2pSetSubType(Data, PARSER_VALUE_FUNKTIONEN);
      else if (Token == PARSER_TOKEN_KEYWORD_NR)
         Cs2pSetSubType(Data, PARSER_VALUE_NR);
      else if (Token == PARSER_TOKEN_KEYWORD_VORWAERTS)
         Cs2pSetSubType(Data, PARSER_VALUE_VORWAERTS);
      else if (Token == PARSER_TOKEN_KEYWORD_RUECKWAERTS)
         Cs2pSetSubType(Data, PARSER_VALUE_RUECKWAERTS);
      else if (Token == PARSER_TOKEN_KEYWORD_INTRAKTION)
         Cs2pSetSubType(Data, PARSER_VALUE_INTRAKTION);
      else if (Token == PARSER_TOKEN_KEYWORD_SEITE)
         Cs2pSetSubType(Data, PARSER_VALUE_SEITE);
      else if (Token == PARSER_TOKEN_KEYWORD_GROESSE)
         Cs2pSetSubType(Data, PARSER_VALUE_GROESSE);
      else if (Token == PARSER_TOKEN_KEYWORD_ZULETZT_BENUTZT)
         Cs2pSetSubType(Data, PARSER_VALUE_ZULETZT_BENUTZT);
      else if (Token == PARSER_TOKEN_KEYWORD_ARTIKEL)
         Cs2pSetSubType(Data, PARSER_VALUE_ARTIKEL);
      else if (Token == PARSER_TOKEN_KEYWORD_STELLUNG)
         Cs2pSetSubType(Data, PARSER_VALUE_STELLUNG);
      else if (Token == PARSER_TOKEN_KEYWORD_SCHALTZEIT)
         Cs2pSetSubType(Data, PARSER_VALUE_SCHALTZEIT);
      else if (Token == PARSER_TOKEN_KEYWORD_UNGERADE)
         Cs2pSetSubType(Data, PARSER_VALUE_UNGERADE);
      else if (Token == PARSER_TOKEN_KEYWORD_DECODER)
         Cs2pSetSubType(Data, PARSER_VALUE_DECODER);
      else if (Token == PARSER_TOKEN_KEYWORD_DECTYP)
         Cs2pSetSubType(Data, PARSER_VALUE_DECTYP);
      else if (Token == PARSER_TOKEN_KEYWORD_FAHRSTRASSE)
         Cs2pSetSubType(Data, PARSER_VALUE_FAHRSTRASSE);
      else if (Token == PARSER_TOKEN_KEYWORD_S88)
         Cs2pSetSubType(Data, PARSER_VALUE_S88);
      else if (Token == PARSER_TOKEN_KEYWORD_S88EIN)
         Cs2pSetSubType(Data, PARSER_VALUE_S88EIN);
      else if (Token == PARSER_TOKEN_KEYWORD_EXTERN)
         Cs2pSetSubType(Data, PARSER_VALUE_EXTERN);
      else if (Token == PARSER_TOKEN_KEYWORD_MAGNETARTIKEL)
         Cs2pSetSubType(Data, PARSER_VALUE_MAGNETARTIKEL);
      else if (Token == PARSER_TOKEN_KEYWORD_ITEM)
         Cs2pSetSubType(Data, PARSER_VALUE_ITEM);
      else if (Token == PARSER_TOKEN_KEYWORD_FSVERWEIS)
         Cs2pSetSubType(Data, PARSER_VALUE_FSVERWEIS);
      else if (Token == PARSER_TOKEN_KEYWORD_ELEMENT)
         Cs2pSetSubType(Data, PARSER_VALUE_ELEMENT);
      else if (Token == PARSER_TOKEN_KEYWORD_DREHUNG)
         Cs2pSetSubType(Data, PARSER_VALUE_DREHUNG);
      else if (Token == PARSER_TOKEN_KEYWORD_TEXT)
         Cs2pSetSubType(Data, PARSER_VALUE_TEXT);
      else if (Token == PARSER_TOKEN_KEYWORD_ZUSTAND)
         Cs2pSetSubType(Data, PARSER_VALUE_ZUSTAND);
      else if (Token == PARSER_TOKEN_KEYWORD_DEVICE_ID)
         Cs2pSetSubType(Data, PARSER_VALUE_DEVICE_ID);
      else if (Token == PARSER_TOKEN_KEYWORD_PAGE)
         Cs2pSetSubType(Data, PARSER_VALUE_PAGE);
      else if (Token == PARSER_TOKEN_KEYWORD_SEKUNDE)
         Cs2pSetSubType(Data, PARSER_VALUE_SEKUNDE);
      else if (Token == PARSER_TOKEN_KEYWORD_IDX)
         Cs2pSetSubType(Data, PARSER_VALUE_IDX);
      else if (Token == PARSER_TOKEN_KEYWORD_ON)
         Cs2pSetSubType(Data, PARSER_VALUE_ON);
      else if (Token == PARSER_TOKEN_KEYWORD_DV)
         Cs2pSetSubType(Data, PARSER_VALUE_DV);
      else if (Token == PARSER_TOKEN_KEYWORD_LLINDEX)
         Cs2pSetSubType(Data, PARSER_VALUE_LLINDEX);
      else if (Token == PARSER_TOKEN_KEYWORD_CRC)
         Cs2pSetSubType(Data, PARSER_VALUE_CRC);
      Token = Scan(Cs2pGetScanner(Data));
      switch (Token)
      {
         case GleichSy:
            if (Cs2pGetVerbose(Data))
               puts("Parser: found '='");
            Token = ScanEol(Cs2pGetScanner(Data));
            Cs2pSetValue(Data, ScanGetString(Cs2pGetScanner(Data)));
            if (Cs2pGetVerbose(Data))
               printf("Parser: value >%s<\n",
                      ScanGetString(Cs2pGetScanner(Data)));
            Token = Scan(Cs2pGetScanner(Data));
            break;
         case EolnSy:
            if (Cs2pGetVerbose(Data))
               puts("Parser: found eol");
            Cs2pSetValue(Data, "");
            break;
         default:
            PrintError(Data, "Unexpected token for value!");
            break;
      }
   }
   else
      PrintError(Data, "Unexpected token for name-value!");
}

/** @file */

/**
 * @defgroup CS2P_PARSE Cs2pParse Group
 *
 * @{
 */

/**
* @brief Parsen des n&auml;chsten Ausdrucks
*
* Diese Funktion parst den Eingabestream. Sie liefert f&uuml;r jeden Aufruf
* den Typ der Information dieser Zeile bzw. den erkannten Typ der Information.
* Diese Funktion wird ein einer Schleife  aufgerufen, bis das Ende der
* Eingangsdaten erreicht ist.
*
* Damit k&ouml;nnte eine Parserschleife wie folgt (Ausschnitt) aussehen:
*
*    do {
*       LineInfo = Cs2pParse(LokParser);
*       switch (LineInfo)
*       {
*          case PARSER_ERROR:
*             break;
*          case PARSER_EOF:
*             break;
*          case PARSER_PARAGRAPH:
*             switch (Cs2pGetSubType(LokParser))
*             {
*                default:
*                   break;
*             }
*             break;
*          case PARSER_VALUE:
*             switch (Cs2pGetSubType(LokParser))
*             {
*                case PARSER_VALUE_LOKOMOTIVE:
*                   break;
*                case PARSER_VALUE_LOK:
*                   break;
*             }
*             break;
*      }
*    } while (LineInfo != PARSER_EOF);</PRE></TD></TR></TABLE>
*
* @param[in] Data Zeiger auf die Parserstruktur
*
* @return Typ der erkannten Information
*/
int Cs2pParse(Cs2parser *Data)
{  int Token, Ret;

   Token = Scan(Cs2pGetScanner(Data));
   Ret = PARSER_ERROR;
   switch (Token)
   {
      case EofSy:
      case EolnSy:
         if (Cs2pGetVerbose(Data))
            puts("Parser: EOF reached");
         Cs2pSetType(Data, PARSER_EOF);
         Ret = PARSER_EOF;
         break;
      case MengeaufSy:
         if (Cs2pGetVerbose(Data))
            puts("Parser: Paragraph found");
         Cs2pSetType(Data, PARSER_PARAGRAPH);
         DoParagraph(Data);
         Ret = PARSER_PARAGRAPH;
         break;
      case PunktSy:
         if (Cs2pGetVerbose(Data))
            puts("Parser: Value found");
         Cs2pSetLevel(Data, 0);
         Cs2pSetType(Data, PARSER_VALUE);
         DoValue(Data, Token);
         Ret = PARSER_VALUE;
         break;
      default:
         if (Token >= PARSER_TOKEN_KEYWORD_LOK)
         {
            if (Cs2pGetVerbose(Data))
               puts("Parser: Value found");
            Cs2pSetLevel(Data, 0);
            Cs2pSetType(Data, PARSER_VALUE);
            DoValue(Data, Token);
            Ret = PARSER_VALUE;
         }
         else
         {
            if (Cs2pGetVerbose(Data))
               printf("Parser: token 0x%x\n", Token);
            PrintError(Data, "Unexpected character!");
            Ret = PARSER_ERROR;
         }
         break;
   }
   return(Ret);
}

/** @} */
