#ifndef WRITE_CS2_H
#define WRITE_CS2_H

/**
* @mainpage cs2_parse
*
* Funktionen f&uuml;r das Schreiben einer *.cs2 Datei
*
* Das Schreiben einer *.cs2 Datei erfolgt auf Basis der C I/O Streams (FILE *).
* F&uuml;r das &Ouml;ffnen der Datei zum Schreiben gibt es Funktionen, die den
* Pfad (zum Apache Standard Webroot) automatisch hinzuf&uuml;gen und auch
* automatisch den Namen der entsprechenden *.cs2 Datei kennen.
*
* @author Michael Bernstein
*/

#include <stdio.h>

/** @file */

/**
* @addtogroup CS2P_WRITE
*
* @brief Konstanten f&uuml;r den Returnwert der Parse-Funktion
*
* @{
*/

/**
* @brief Konstanten f&uuml;r Dateityp
*/
#define CS2_FILE_TYPE_LOKOMOTIVE 1

/**
* @brief Konstanten f&uuml;r Dateinamen
*/
#define CS2_FILE_STRING_LOKOMOTIVE           "lokomotive.cs2"
#define CS2_FILE_STRING_MAGNETARTIKEL        "magnetartikel.cs2"
#define CS2_FILE_STRING_GLEISBILD            "gleisbild.cs2"
#define CS2_FILE_STRING_FAHRSTRASSE          "fahrstrassen.cs2"
#define CS2_FILE_STRING_STATUS_LOKOMOTIVE    "lokstatus.sr2"
#define CS2_FILE_STRING_STATUS_MAGNETARTIKEL "magnetartikel.sr2"
#define CS2_FILE_STRING_STATUS_GLEISBILD     "gleisbild.sr2"
#define CS2_FILE_STRING_STATUS_FAHRSTRASSE   "fahrstrassen.sr2"

/**
* @brief Konstanten f&uuml;r Paragraphen/Abs&auml;tze (Token und Text)
*/
#define CS2_PARAGRAPH_TYPE_LOK            1
#define CS2_PARAGRAPH_TYPE_NUMLOKS        2
#define CS2_PARAGRAPH_TYPE_LOKOMOTIVE     3
#define CS2_PARAGRAPH_TYPE_GLEISBILD      4
#define CS2_PARAGRAPH_TYPE_GLEISBILDSEITE 5
#define CS2_PARAGRAPH_TYPE_MAGNETARTIKEL  6
#define CS2_PARAGRAPH_TYPE_FAHRSTRASSEN   7

/**
* @brief Konstanten f&uuml;r Paragraph-/Absatznamen (Token und Text)
*/
#define CS2_PARAGRAPH_STRING_LOK            "lok"
#define CS2_PARAGRAPH_STRING_NUMLOKS        "numloks"
#define CS2_PARAGRAPH_STRING_LOKOMOTIVE     "lokomotive"
#define CS2_PARAGRAPH_STRING_GLEISBILD      "gleisbild"
#define CS2_PARAGRAPH_STRING_GLEISBILDSEITE "gleisbildseite"
#define CS2_PARAGRAPH_STRING_MAGNETARTIKEL  "magnetartikel"
#define CS2_PARAGRAPH_STRING_FAHRSTRASSEN   "fahrstrassen"

/**
* @brief Konstanten f&uuml;r Magnetartikel Konfigurationswerte
*/
#define CS_MAGNETARTIKEL_TYP_STRING_STD_ROT_GRUEN              "std_rot_gruen"
#define CS_MAGNETARTIKEL_TYP_STRING_STD_ROT                    "std_rot"
#define CS_MAGNETARTIKEL_TYP_STRING_STD_GRUEN                  "std_gruen"
#define CS_MAGNETARTIKEL_TYP_STRING_ENTKUPPLUNGSGLEIS          "entkupplungsgleis"
#define CS_MAGNETARTIKEL_TYP_STRING_ENTKUPPLUNGSGLEIS_1        "entkupplungsgleis_1"
#define CS_MAGNETARTIKEL_TYP_STRING_RECHTSWEICHE               "rechtsweiche"
#define CS_MAGNETARTIKEL_TYP_STRING_LINKSWEICHE                "linksweiche"
#define CS_MAGNETARTIKEL_TYP_STRING_Y_WEICHE                   "y_weiche"
#define CS_MAGNETARTIKEL_TYP_STRING_K84_AUSGANG                "k84_ausgang"
#define CS_MAGNETARTIKEL_TYP_STRING_K84_DOPPELAUSGANG          "k84_doppelausgang"
#define CS_MAGNETARTIKEL_TYP_STRING_DREIWEGWEICHE              "dreiwegweiche"
#define CS_MAGNETARTIKEL_TYP_STRING_DKW_2_ANTRIEBE             "DKW 2 Antriebe"
#define CS_MAGNETARTIKEL_TYP_STRING_DKW_1_ANTRIEB              "DKW 1 Antrieb"
#define CS_MAGNETARTIKEL_TYP_STRING_LICHTSIGNAL_HP01           "lichtsignal_HP01"
#define CS_MAGNETARTIKEL_TYP_STRING_LICHTSIGNAL_HP02           "lichtsignal_HP02"
#define CS_MAGNETARTIKEL_TYP_STRING_LICHTSIGNAL_HP012          "lichtsignal_HP012"
#define CS_MAGNETARTIKEL_TYP_STRING_LICHTSIGNAL_HP012_SH01     "lichtsignal_HP012_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_LICHTSIGNAL_SH01           "lichtsignal_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_FORMSIGNAL_HP01            "formsignal_HP01"
#define CS_MAGNETARTIKEL_TYP_STRING_FORMSIGNAL_HP02            "formsignal_HP02"
#define CS_MAGNETARTIKEL_TYP_STRING_FORMSIGNAL_HP012           "formsignal_HP012"
#define CS_MAGNETARTIKEL_TYP_STRING_FORMSIGNAL_HP012_SH01      "formsignal_HP012_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_FORMSIGNAL_SH01            "formsignal_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_URC_LICHTSIGNAL_HP01       "urc_lichtsignal_HP01"
#define CS_MAGNETARTIKEL_TYP_STRING_URC_LICHTSIGNAL_HP012      "urc_lichtsignal_HP012"
#define CS_MAGNETARTIKEL_TYP_STRING_URC_LICHTSIGNAL_HP012_SH01 "urc_lichtsignal_HP012_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_URC_LICHTSIGNAL_SH01       "urc_lichtsignal_SH01"
#define CS_MAGNETARTIKEL_TYP_STRING_SCHIEBEBUEHNE              "schiebebuehne"
#define CS_MAGNETARTIKEL_TYP_STRING_DREHSCHEIBE_ALT            "drehscheibe_alt"
#define CS_MAGNETARTIKEL_TYP_STRING_DIGITALDREHSCHEIBE         "digitaldrehscheibe"

/**
* @brief Konstanten f&uuml;r Gleisbild Konfigurationswerte
*/
#define CS2_GLEISBILD_TYP_STRING_LEER               "leer"
#define CS2_GLEISBILD_TYP_STRING_GERADE             "gerade"
#define CS2_GLEISBILD_TYP_STRING_KREUZUNG           "kreuzung"
#define CS2_GLEISBILD_TYP_STRING_UNTERFUEHRUNG      "unterfuehrung"
#define CS2_GLEISBILD_TYP_STRING_PRELLBOCK          "prellbock"
#define CS2_GLEISBILD_TYP_STRING_BOGEN              "bogen"
#define CS2_GLEISBILD_TYP_STRING_DOPPELBOGEN        "doppelbogen"
#define CS2_GLEISBILD_TYP_STRING_TUNNEL             "tunnel"
#define CS2_GLEISBILD_TYP_STRING_LINKSWEICHE        "linksweiche"
#define CS2_GLEISBILD_TYP_STRING_RECHTSWEICHE       "rechtsweiche"
#define CS2_GLEISBILD_TYP_STRING_DREIWEGWEICHE      "dreiwegweiche"
#define CS2_GLEISBILD_TYP_STRING_YWEICHE            "yweiche"
#define CS2_GLEISBILD_TYP_STRING_DKWEICHE           "dkweiche"
#define CS2_GLEISBILD_TYP_STRING_DKWEICHE_2         "dkweiche_2"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL             "signal"
#define CS2_GLEISBILD_TYP_STRING_S88KONTAKT         "s88kontakt"
#define CS2_GLEISBILD_TYP_STRING_S88BOGEN           "s88bogen"
#define CS2_GLEISBILD_TYP_STRING_PFEIL              "pfeil"
#define CS2_GLEISBILD_TYP_STRING_FAHRSTRASSE        "fahrstrasse"
#define CS2_GLEISBILD_TYP_STRING_TEXT               "text"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_HP02        "signal_hp02"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_HP012       "signal_hp012"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_HP01S       "signal_hp01s"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_P_HP012S    "signal_p_hp012s"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_F_HP012S    "signal_f_hp012s"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_P_HP012     "signal_p_hp012"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_F_HP01      "signal_f_hp01"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_F_HP02      "signal_f_hp02"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_F_HP012     "signal_f_hp012"
#define CS2_GLEISBILD_TYP_STRING_SIGNAL_SH01        "signal_sh01"
#define CS2_GLEISBILD_TYP_STRING_K84_EINFACH        "k84_einfach"
#define CS2_GLEISBILD_TYP_STRING_K84_DOPPELT        "k84_doppelt"
#define CS2_GLEISBILD_TYP_STRING_ENTKUPPLER         "entkuppler"
#define CS2_GLEISBILD_TYP_STRING_ENTKUPPLER_1       "entkuppler_1"
#define CS2_GLEISBILD_TYP_STRING_STD_ROT            "std_rot"
#define CS2_GLEISBILD_TYP_STRING_STD_GRUEN          "std_gruen"
#define CS2_GLEISBILD_TYP_STRING_STD_ROT_GRUEN_0    "std_rot_gruen_0"
#define CS2_GLEISBILD_TYP_STRING_STD_ROT_GRUEN_1    "std_rot_gruen_1"
#define CS2_GLEISBILD_TYP_STRING_SCHIEBEBUEHNE_0    "schiebebuehne_0"
#define CS2_GLEISBILD_TYP_STRING_SCHIEBEBUEHNE_1    "schiebebuehne_1"
#define CS2_GLEISBILD_TYP_STRING_SCHIEBEBUEHNE_2    "schiebebuehne_2"
#define CS2_GLEISBILD_TYP_STRING_SCHIEBEBUEHNE_3    "schiebebuehne_3"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_ALT_0  "drehscheibe_alt_0"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_ALT_1  "drehscheibe_alt_1 "
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_ALT_2  "drehscheibe_alt_2"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_ALT_3  "drehscheibe_alt_3"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_0  "drehscheibe_dig_0"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_1  "drehscheibe_dig_1"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_2  "drehscheibe_dig_2"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_3  "drehscheibe_dig_3"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_4  "drehscheibe_dig_4"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_5  "drehscheibe_dig_5"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_6  "drehscheibe_dig_6"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_7  "drehscheibe_dig_7"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_8  "drehscheibe_dig_8"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_9  "drehscheibe_dig_9"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_10 "drehscheibe_dig_10"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_11 "drehscheibe_dig_11"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_12 "drehscheibe_dig_12"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_13 "drehscheibe_dig_13"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_14 "drehscheibe_dig_14"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_15 "drehscheibe_dig_15"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_16 "drehscheibe_dig_16"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_17 "drehscheibe_dig_17"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_18 "drehscheibe_dig_18"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_19 "drehscheibe_dig_19"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_20 "drehscheibe_dig_20"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_21 "drehscheibe_dig_21"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_22 "drehscheibe_dig_22"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_23 "drehscheibe_dig_23"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_24 "drehscheibe_dig_24"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_25 "drehscheibe_dig_25"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_26 "drehscheibe_dig_26"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_27 "drehscheibe_dig_27"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_28 "drehscheibe_dig_28"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_29 "drehscheibe_dig_29"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_30 "drehscheibe_dig_30"
#define CS2_GLEISBILD_TYP_STRING_DREHSCHEIBE_DIG_31 "drehscheibe_dig_31"


char *Cs2GetTypAsString(int Typ);
FILE *Cs2OpenByName(char *Name);
FILE *Cs2OpenByType(int Cs2FileType);
void Cs2Close(FILE *Stream);
void Cs2WriteParagraphByName(FILE *Stream, char *Paragraph);
void Cs2WriteParagraphByType(FILE *Stream, int Cs2ParagraphType);
void Cs2WriteTitleByName(FILE *Stream, char *Title, int Level);
void Cs2WriteStringValueByName(FILE *Stream, char *Title, char *Value, int Level);
void Cs2WriteIntValueByName(FILE *Stream, char *Title, int Value, int Level);
void Cs2WriteHexValueByName(FILE *Stream, char *Title, int Value, int Level);
void Cs2WriteULongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level);
void Cs2WriteHexLongValueByName(FILE *Stream, char *Title, unsigned long Value, int Level);
void Cs2WriteFloatValueByName(FILE *Stream, char *Title, float Value, int Level);

/** @} */

#endif
