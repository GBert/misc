#ifndef CONFIGDATA_H
#define CONFIGDATA_H

#include <stdio.h>
#include <map.h>
#include <menge.h>

#define CS2_MESSWERT_HPGL_DE   "Hauptgleis"
#define CS2_MESSWERT_HPGL_EN   "Maintrack"
#define CS2_MESSWERT_PGL_DE    "Programmiergleis"
#define CS2_MESSWERT_PGL_EN    "Programtrack"
#define CS2_MESSWERT_TRACK_DE  "Boostergleis"
#define CS2_MESSWERT_TRACK_EN  "Boostertrack"
#define CS2_MESSWERT_VOLT_DE   "Versorgungsspannung"
#define CS2_MESSWERT_VOLT_EN   "Supplyvoltage"
#define CS2_MESSWERT_ACHSEN_DE "Achsen"
#define CS2_MESSWERT_ACHSEN_EN "Axes"
#define CS2_MESSWERT_TEMP_DE   "Temperatur"
#define CS2_MESSWERT_TEMP_EN   "Temperature"
#define CS2_MESSWERT_TEMPO_DE  "Geschwindigkeit"
#define CS2_MESSWERT_TEMPO_EN  "Speed"

#define CS2_KONFIG_KENNER_AUSWAHLLISTE 1
#define CS2_KONFIG_KENNER_WERT         2

#define MAX_GERAETE_BEZEICHNUNG 80

typedef struct {
   char ArtikelNr[9];
   char GeraeteBezeichnung[MAX_GERAETE_BEZEICHNUNG];
   unsigned int NumMesswerte;
   unsigned int NumKonfigwerte;
   unsigned long SerialNum;
} ConfigGeraetTyp;

#define ConfigGeraetTypSetNumMesswerte(Data,Val)   (Data)->NumMesswerte=Val
#define ConfigGeraetTypSetNumKonfigwerte(Data,Val) (Data)->NumKonfigwerte=Val
#define ConfigGeraetTypSetSerialNum(Data,Val)      (Data)->SerialNum=Val

#define ConfigGeraetTypGetArtikelNr(Data)          (Data)->ArtikelNr
#define ConfigGeraetTypGetGeraeteBezeichnung(Data) (Data)->GeraeteBezeichnung
#define ConfigGeraetTypGetNumMesswerte(Data)       (Data)->NumMesswerte
#define ConfigGeraetTypGetNumKonfigwerte(Data)     (Data)->NumKonfigwerte
#define ConfigGeraetTypGetSerialNum(Data)          (Data)->SerialNum

typedef struct {
   unsigned int Index;
   unsigned int Abfragekanalnummer;
   unsigned int MesswertPotenz;
   unsigned int FarbeBereich1;
   unsigned int FarbeBereich2;
   unsigned int FarbeBereich3;
   unsigned int FarbeBereich4;
   unsigned int Nullpunkt0;
   unsigned int EndeBereich1;
   unsigned int EndeBereich2;
   unsigned int EndeBereich3;
   unsigned int EndeBereich4;
   unsigned int AktuellerMesswert;
   char *Messwertbezeichnung;
   char *BezeichnungStart;
   char *BezeichnungEnde;
   char *Einheit;
} ConfigMesswertTyp;

#define ConfigMesswertTypSetIndex(Data,Val)               (Data)->Index=Val
#define ConfigMesswertTypSetAbfragekanalnummer(Data,Val)  (Data)->Abfragekanalnummer=Val
#define ConfigMesswertTypSetMesswertPotenz(Data,Val)      (Data)->MesswertPotenz=Val
#define ConfigMesswertTypSetFarbeBereich1(Data,Val)       (Data)->FarbeBereich1=Val
#define ConfigMesswertTypSetFarbeBereich2(Data,Val)       (Data)->FarbeBereich2=Val
#define ConfigMesswertTypSetFarbeBereich3(Data,Val)       (Data)->FarbeBereich3=Val
#define ConfigMesswertTypSetFarbeBereich4(Data,Val)       (Data)->FarbeBereich4=Val
#define ConfigMesswertTypSetNullpunkt0(Data,Val)          (Data)->Nullpunkt0=Val
#define ConfigMesswertTypSetEndeBereich1(Data,Val)        (Data)->EndeBereich1=Val
#define ConfigMesswertTypSetEndeBereich2(Data,Val)        (Data)->EndeBereich2=Val
#define ConfigMesswertTypSetEndeBereich3(Data,Val)        (Data)->EndeBereich3=Val
#define ConfigMesswertTypSetEndeBereich4(Data,Val)        (Data)->EndeBereich4=Val
#define ConfigMesswertTypSetAktuellerMesswert(Data,Val)   (Data)->AktuellerMesswert=Val
#define ConfigMesswertTypSetMesswertbezeichnung(Data,Val) (Data)->Messwertbezeichnung=Val
#define ConfigMesswertTypSetBezeichnungStart(Data,Val)    (Data)->BezeichnungStart=Val
#define ConfigMesswertTypSetBezeichnungEnde(Data,Val)     (Data)->BezeichnungEnde=Val
#define ConfigMesswertTypSetEinheit(Data,Val)             (Data)->Einheit=Val

#define ConfigMesswertTypGetIndex(Data)               (Data)->Index
#define ConfigMesswertTypGetAbfragekanalnummer(Data)  (Data)->Abfragekanalnummer
#define ConfigMesswertTypGetMesswertPotenz(Data)      (Data)->MesswertPotenz
#define ConfigMesswertTypGetFarbeBereich1(Data)       (Data)->FarbeBereich1
#define ConfigMesswertTypGetFarbeBereich2(Data)       (Data)->FarbeBereich2
#define ConfigMesswertTypGetFarbeBereich3(Data)       (Data)->FarbeBereich3
#define ConfigMesswertTypGetFarbeBereich4(Data)       (Data)->FarbeBereich4
#define ConfigMesswertTypGetNullpunkt0(Data)          (Data)->Nullpunkt0
#define ConfigMesswertTypGetEndeBereich1(Data)        (Data)->EndeBereich1
#define ConfigMesswertTypGetEndeBereich2(Data)        (Data)->EndeBereich2
#define ConfigMesswertTypGetEndeBereich3(Data)        (Data)->EndeBereich3
#define ConfigMesswertTypGetEndeBereich4(Data)        (Data)->EndeBereich4
#define ConfigMesswertTypGetAktuellerMesswert(Data)   (Data)->AktuellerMesswert
#define ConfigMesswertTypGetMesswertbezeichnung(Data) (Data)->Messwertbezeichnung
#define ConfigMesswertTypGetBezeichnungStart(Data)    (Data)->BezeichnungStart
#define ConfigMesswertTypGetBezeichnungEnde(Data)     (Data)->BezeichnungEnde
#define ConfigMesswertTypGetEinheit(Data)             (Data)->Einheit

typedef struct {
   unsigned int Konfigkanalnummer;
   unsigned int KennerAuswahlliste;
   unsigned int AnzahlAuswahlpunkte;
   unsigned int JetzigeEinstellung;
   unsigned int Res;
   char *Auswahlbezeichnung;
   char **Auswahl;
} ConfigAuswahlTyp;

#define ConfigAuswahlSetKonfigkanalnummer(Data,Val)   (Data)->Konfigkanalnummer=Val
#define ConfigAuswahlSetKennerAuswahlliste(Data,Val)  (Data)->KennerAuswahlliste=Val
#define ConfigAuswahlSetAnzahlAuswahlpunkte(Data,Val) (Data)->AnzahlAuswahlpunkte=Val
#define ConfigAuswahlSetJetzigeEinstellung(Data,Val)  (Data)->JetzigeEinstellung=Val
#define ConfigAuswahlSetRes(Data,Val)                 (Data)->Res=Val
#define ConfigAuswahlSetAuswahlbezeichnung(Data,Val)  (Data)->Auswahlbezeichnung=Val
#define ConfigAuswahlSetAuswahl(Data,Val)             (Data)->Auswahl=Val
#define ConfigAuswahlSetAuswahlIndex(Data,Index,Val)  (Data)->Auswahl[Index]=Val

#define ConfigAuswahlGetKonfigkanalnummer(Data)   (Data)->Konfigkanalnummer
#define ConfigAuswahlGetKennerAuswahlliste(Data)  (Data)->KennerAuswahlliste
#define ConfigAuswahlGetAnzahlAuswahlpunkte(Data) (Data)->AnzahlAuswahlpunkte
#define ConfigAuswahlGetJetzigeEinstellung(Data)  (Data)->JetzigeEinstellung
#define ConfigAuswahlGetRes(Data)                 (Data)->Res
#define ConfigAuswahlGetAuswahlbezeichnung(Data)  (Data)->Auswahlbezeichnung
#define ConfigAuswahlGetAuswahl(Data)             (Data)->Auswahl
#define ConfigAuswahlGetAuswahlIndex(Data,Index)  (Data)->Auswahl[Index]

typedef struct {
   unsigned int Konfigurationskanalnummer;
   unsigned int KennerSlider;
   unsigned int UntererWert;
   unsigned int ObererWert;
   unsigned int AktuelleEinstellung;
   char *Auswahlbezeichnung;
   char *BezeichnungStart;
   char *BezeichnungEnde;
   char *Einheit;
} ConfigWertTyp;

#define ConfigWertSetKonfigurationskanalnummer(Data,Val) (Data)->Konfigurationskanalnummer=Val
#define ConfigWertSetKennerSlider(Data,Val)              (Data)->KennerSlider=Val
#define ConfigWertSetUntererWert(Data,Val)               (Data)->UntererWert=Val
#define ConfigWertSetObererWert(Data,Val)                (Data)->ObererWert=Val
#define ConfigWertSetAktuelleEinstellung(Data,Val)       (Data)->AktuelleEinstellung=Val
#define ConfigWertSetAuswahlbezeichnung(Data,Val)        (Data)->Auswahlbezeichnung=Val
#define ConfigWertSetBezeichnungStart(Data,Val)          (Data)->BezeichnungStart=Val
#define ConfigWertSetBezeichnungEnde(Data,Val)           (Data)->BezeichnungEnde=Val
#define ConfigWertSetEinheit(Data,Val)                   (Data)->Einheit=Val

#define ConfigWertGetKonfigurationskanalnummer(Data) (Data)->Konfigurationskanalnummer
#define ConfigWertGetKennerSlider(Data)              (Data)->KennerSlider
#define ConfigWertGetUntererWert(Data)               (Data)->UntererWert
#define ConfigWertGetObererWert(Data)                (Data)->ObererWert
#define ConfigWertGetAktuelleEinstellung(Data)       (Data)->AktuelleEinstellung
#define ConfigWertGetAuswahlbezeichnung(Data)        (Data)->Auswahlbezeichnung
#define ConfigWertGetBezeichnungStart(Data)          (Data)->BezeichnungStart
#define ConfigWertGetBezeichnungEnde(Data)           (Data)->BezeichnungEnde
#define ConfigWertGetEinheit(Data)                   (Data)->Einheit

#define CONFIG_TYP_AUSWAHL 0
#define CONFIG_TYP_WERT    1

typedef struct {
   unsigned int Index;
   unsigned int Typ;
   union {
      ConfigAuswahlTyp *Auswahl;
      ConfigWertTyp *Wert;
   } TypWerte;
} ConfigTyp;

#define ConfigTypSetIndex(Data,Val)      (Data)->Index=Val
#define ConfigTypSetTyp(Data,Val)        (Data)->Typ=Val
#define ConfigTypSetTypAuswahl(Data,Val) (Data)->TypWerte.Auswahl=Val
#define ConfigTypSetTypWert(Data,Val)    (Data)->TypWerte.Wert=Val

#define ConfigTypGetIndex(Data)      (Data)->Index
#define ConfigTypGetTyp(Data)        (Data)->Typ
#define ConfigTypGetTypAuswahl(Data) (Data)->TypWerte.Auswahl
#define ConfigTypGetTypWert(Data)    (Data)->TypWerte.Wert

typedef struct {
   ConfigGeraetTyp GeraeteTyp;
   Menge *Messwerte;
   Menge *Typen;
} ConfigStatusInfo;

#define ConfigStatusInfoSetMesswerte(Data,Val) (Data)->Messwerte=Val
#define ConfigStatusInfoSetTypen(Data,Val)     (Data)->Typen=Val

#define ConfigStatusInfoGetGeraeteTyp(Data) &((Data)->GeraeteTyp)
#define ConfigStatusInfoGetMesswerte(Data)  (Data)->Messwerte
#define ConfigStatusInfoGetTypen(Data)      (Data)->Typen

#define ConfigStatusInfoAddMesswerte(Data,Val) MengeAdd(ConfigStatusInfoGetMesswerte(Data),(MengeDataType)Val)
#define ConfigStatusInfoAddTypen(Data,Val)     MengeAdd(ConfigStatusInfoGetTypen(Data),(MengeDataType)Val)

ConfigMesswertTyp *ConfigMesswertTypCreate(void);
void ConfigMesswertTypDestroy(ConfigMesswertTyp *Data);
void ConfigMesswertTypInit(ConfigMesswertTyp *Data);
void ConfigMesswertTypExit(ConfigMesswertTyp *Data);
ConfigTyp *ConfigTypCreate(void);
void ConfigTypDestroy(ConfigTyp *Data);
void ConfigTypInit(ConfigTyp *Data);
void ConfigTypExit(ConfigTyp *Data);
ConfigWertTyp *ConfigWertTypCreate(void);
void ConfigWertTypDestroy(ConfigWertTyp *Data);
void ConfigWertTypInit(ConfigWertTyp *Data);
void ConfigWertTypExit(ConfigWertTyp *Data);
ConfigAuswahlTyp *ConfigAuswahlTypCreate(void);
void ConfigAuswahlTypDestroy(ConfigAuswahlTyp *Data);
void ConfigAuswahlTypInit(ConfigAuswahlTyp *Data);
void ConfigAuswahlTypExit(ConfigAuswahlTyp *Data);
ConfigStatusInfo *ConfigStatusInfoCreate(void);
void ConfigStatusInfoDestroy(ConfigStatusInfo *Data);
void ConfigStatusInfoClear(ConfigStatusInfo *Data);
void ConfigStatusInfoInit(ConfigStatusInfo *Data);
void ConfigStatusInfoExit(ConfigStatusInfo *Data);
void ConfigStatusInfoGetMrsystem(ConfigStatusInfo *Data,
                                 unsigned Major, unsigned Minor);
unsigned long ConfigStatusMrsystemGeraetebeschreibung(char *Buf,
                                                      unsigned int Major,
                                                      unsigned int Minor);
unsigned long ConfigStatusGeraetebeschreibung(ConfigGeraetTyp *GeraetTyp,
                                              char *Buf, unsigned long Len);
unsigned long ConfigStatusMesswerte(ConfigMesswertTyp *ConfigMesswert,
                                    char *Buf, unsigned long Len);
unsigned long ConfigStatusKonfigwerte(ConfigTyp *Config, char *Buf,
                                      unsigned long Len);
void ConfigStatusSaveMesswerte(ConfigStatusInfo *ConfigInfo, FILE *MesswertCs2Stream);
void ConfigStatusSaveKonfigwerte(ConfigStatusInfo *ConfigInfo, FILE *MesswertCs2Stream);
void ConfigStatusWebDeviceMesswerte(MengeIterator *IterToWrite,
                                    char *FilePath, unsigned long Uid);
void ConfigStatusWebOneKonfig(ConfigTyp *KonfigToWrite,
                              FILE *KonfigUciStream, FILE *ValueUciStream);
void ConfigStatusWebDeviceKonfig(MengeIterator *IterToWrite,
                                 char *FilePath, unsigned long Uid);

#endif
