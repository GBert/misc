#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <bytestream.h>
#include <write_cs2.h>
#include <uci.h>
#include "config.h"


#define MRSYSTEM_BEZEICHNUNG "mrsystem"


ConfigMesswertTyp *ConfigMesswertTypCreate(void)
{  ConfigMesswertTyp *NewData;

   NewData = (ConfigMesswertTyp *)malloc(sizeof(ConfigMesswertTyp));
   return(NewData);
}

void ConfigMesswertTypDestroy(ConfigMesswertTyp *Data)
{
   if (ConfigMesswertTypGetMesswertbezeichnung(Data) != (char *)NULL)
      free(ConfigMesswertTypGetMesswertbezeichnung(Data));
   if (ConfigMesswertTypGetBezeichnungStart(Data) != (char *)NULL)
      free(ConfigMesswertTypGetBezeichnungStart(Data));
   if (ConfigMesswertTypGetBezeichnungEnde(Data) != (char *)NULL)
      free(ConfigMesswertTypGetBezeichnungEnde(Data));
   if (ConfigMesswertTypGetEinheit(Data) != (char *)NULL)
      free(ConfigMesswertTypGetEinheit(Data));
   free(Data);
}

void ConfigMesswertTypInit(ConfigMesswertTyp *Data)
{
   ConfigMesswertTypSetMesswertbezeichnung(Data, (char *)NULL);
   ConfigMesswertTypSetBezeichnungStart(Data, (char *)NULL);
   ConfigMesswertTypSetBezeichnungEnde(Data, (char *)NULL);
   ConfigMesswertTypSetEinheit(Data, (char *)NULL);
}

void ConfigMesswertTypExit(ConfigMesswertTyp *Data)
{
}

ConfigTyp *ConfigTypCreate(void)
{  ConfigTyp *NewData;

   NewData = (ConfigTyp *)malloc(sizeof(ConfigTyp));
   return(NewData);
}

void ConfigTypDestroy(ConfigTyp *Data)
{
   switch (ConfigTypGetTyp(Data))
   {
      case CONFIG_TYP_AUSWAHL:
         if (ConfigTypGetTypAuswahl(Data) != (ConfigAuswahlTyp *)NULL)
            ConfigAuswahlTypDestroy(ConfigTypGetTypAuswahl(Data));
         break;
      case CONFIG_TYP_WERT:
         if (ConfigTypGetTypWert(Data) != (ConfigWertTyp *)NULL)
            ConfigWertTypDestroy(ConfigTypGetTypWert(Data));
         break;
   }
   free(Data);
}

void ConfigTypInit(ConfigTyp *Data)
{
   ConfigTypSetTyp(Data, CONFIG_TYP_AUSWAHL);
   ConfigTypSetTypAuswahl(Data, (ConfigAuswahlTyp *)NULL);
}

void ConfigTypExit(ConfigTyp *Data)
{
   switch (ConfigTypGetTyp(Data))
   {
      case CONFIG_TYP_AUSWAHL:
         if (ConfigTypGetTypAuswahl(Data) != (ConfigAuswahlTyp *)NULL)
            ConfigAuswahlTypExit(ConfigTypGetTypAuswahl(Data));
         break;
      case CONFIG_TYP_WERT:
         if (ConfigTypGetTypWert(Data) != (ConfigWertTyp *)NULL)
            ConfigWertTypExit(ConfigTypGetTypWert(Data));
         break;
   }
}

ConfigWertTyp *ConfigWertTypCreate(void)
{  ConfigWertTyp *NewData;

   NewData = (ConfigWertTyp *)malloc(sizeof(ConfigWertTyp));
   return(NewData);
}

void ConfigWertTypDestroy(ConfigWertTyp *Data)
{
   if (ConfigWertGetAuswahlbezeichnung(Data) != (char *)NULL)
      free(ConfigWertGetAuswahlbezeichnung(Data));
   if (ConfigWertGetBezeichnungStart(Data) != (char *)NULL)
      free(ConfigWertGetBezeichnungStart(Data));
   if (ConfigWertGetBezeichnungEnde(Data) != (char *)NULL)
      free(ConfigWertGetBezeichnungEnde(Data));
   if (ConfigWertGetEinheit(Data) != (char *)NULL)
      free(ConfigWertGetEinheit(Data));
   free(Data);
}

void ConfigWertTypInit(ConfigWertTyp *Data)
{
   ConfigWertSetKonfigurationskanalnummer(Data, 0);
   ConfigWertSetKennerSlider(Data, 0);
   ConfigWertSetUntererWert(Data, 0);
   ConfigWertSetObererWert(Data, 0);
   ConfigWertSetAktuelleEinstellung(Data, 0);
   ConfigWertSetAuswahlbezeichnung(Data, (char *)NULL);
   ConfigWertSetBezeichnungStart(Data, (char *)NULL);
   ConfigWertSetBezeichnungEnde(Data, (char *)NULL);
   ConfigWertSetEinheit(Data, (char *)NULL);
}

void ConfigWertTypExit(ConfigWertTyp *Data)
{
}

ConfigAuswahlTyp *ConfigAuswahlTypCreate(void)
{  ConfigAuswahlTyp *NewData;

   NewData = (ConfigAuswahlTyp *)malloc(sizeof(ConfigTyp));
   return(NewData);
}

void ConfigAuswahlTypDestroy(ConfigAuswahlTyp *Data)
{  int i;

   if (ConfigAuswahlGetAuswahlbezeichnung(Data) != (char *)NULL)
      free(ConfigAuswahlGetAuswahlbezeichnung(Data));
   if (ConfigAuswahlGetAuswahl(Data) != (char **)NULL)
   {
      for (i = 0; i < ConfigAuswahlGetAnzahlAuswahlpunkte(Data); i++)
      {
         if (ConfigAuswahlGetAuswahlIndex(Data, i) != (char *)NULL)
            free(ConfigAuswahlGetAuswahlIndex(Data, i));
      }
   }
   free(Data);
}

void ConfigAuswahlTypInit(ConfigAuswahlTyp *Data)
{
   ConfigAuswahlSetKonfigkanalnummer(Data, 0);
   ConfigAuswahlSetKennerAuswahlliste(Data, 0);
   ConfigAuswahlSetAnzahlAuswahlpunkte(Data, 0);
   ConfigAuswahlSetJetzigeEinstellung(Data, 0);
   ConfigAuswahlSetRes(Data, 0);
   ConfigAuswahlSetAuswahlbezeichnung(Data, (char *)NULL);
   ConfigAuswahlSetAuswahl(Data, (char **)NULL);
}

void ConfigAuswahlTypExit(ConfigAuswahlTyp *Data)
{
}

ConfigStatusInfo *ConfigStatusInfoCreate(void)
{  ConfigStatusInfo *NewData;

   NewData = (ConfigStatusInfo *)malloc(sizeof(ConfigStatusInfo));
   if (NewData != (ConfigStatusInfo *)NULL)
   {
      ConfigStatusInfoSetMesswerte(NewData, MengeCreate());
      if (ConfigStatusInfoGetMesswerte(NewData) != (Menge *)NULL)
      {
         ConfigStatusInfoSetTypen(NewData, MengeCreate());
         if (ConfigStatusInfoGetTypen(NewData) == (Menge *)NULL)
         {
            MengeDestroy(ConfigStatusInfoGetMesswerte(NewData));
            free(NewData);
            NewData = (ConfigStatusInfo *)NULL;
         }
      }
      else
      {
         free(NewData);
         NewData = (ConfigStatusInfo *)NULL;
      }
   }
   return(NewData);
}

void ConfigStatusInfoDestroy(ConfigStatusInfo *Data)
{
   if (ConfigStatusInfoGetMesswerte(Data) != (Menge *)NULL)
      MengeDestroy(ConfigStatusInfoGetMesswerte(Data));
   if (ConfigStatusInfoGetTypen(Data) != (Menge *)NULL)
      MengeDestroy(ConfigStatusInfoGetTypen(Data));
   free(Data);
}

static int ConfigStatusInfoMesswerteCmp(void *d1, void *d2)
{  ConfigMesswertTyp *Data1, *Data2;

   Data1 = (ConfigMesswertTyp *)d1;
   Data2 = (ConfigMesswertTyp *)d2;
   return(ConfigMesswertTypGetIndex(Data1)-ConfigMesswertTypGetIndex(Data2));
}

static int ConfigStatusInfoTypenCmp(void *d1, void *d2)
{  ConfigTyp *Data1, *Data2;

   Data1 = (ConfigTyp *)d1;
   Data2 = (ConfigTyp *)d2;
   return(ConfigTypGetIndex(Data1)-ConfigTypGetIndex(Data2));
}

void ConfigStatusInfoInit(ConfigStatusInfo *Data)
{
   MengeInit(ConfigStatusInfoGetMesswerte(Data), ConfigStatusInfoMesswerteCmp,
             (MengeDelCbFkt)ConfigMesswertTypDestroy);
   MengeInit(ConfigStatusInfoGetTypen(Data), ConfigStatusInfoTypenCmp,
             (MengeDelCbFkt)ConfigTypDestroy);
}

void ConfigStatusInfoExit(ConfigStatusInfo *Data)
{
}

void ConfigStatusInfoClear(ConfigStatusInfo *Data)
{
   if (ConfigStatusInfoGetMesswerte(Data) != (Menge *)NULL)
      MengeDestroy(ConfigStatusInfoGetMesswerte(Data));
   ConfigStatusInfoSetMesswerte(Data, MengeCreate());
   MengeInit(ConfigStatusInfoGetMesswerte(Data), ConfigStatusInfoMesswerteCmp,
             (MengeDelCbFkt)ConfigMesswertTypDestroy);
   if (ConfigStatusInfoGetTypen(Data) != (Menge *)NULL)
      MengeDestroy(ConfigStatusInfoGetTypen(Data));
   ConfigStatusInfoSetTypen(Data, MengeCreate());
   MengeInit(ConfigStatusInfoGetTypen(Data), ConfigStatusInfoTypenCmp,
             (MengeDelCbFkt)ConfigTypDestroy);
}

void ConfigStatusInfoGetMrsystem(ConfigStatusInfo *Data,
                                 unsigned Major, unsigned Minor)
{
   strcpy(ConfigGeraetTypGetArtikelNr(ConfigStatusInfoGetGeraeteTyp(Data)),
          "-");
   strcpy(ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(Data)),
          MRSYSTEM_BEZEICHNUNG);
   ConfigGeraetTypSetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(Data), 0);
   ConfigGeraetTypSetNumKonfigwerte(ConfigStatusInfoGetGeraeteTyp(Data), 0);
   ConfigGeraetTypSetSerialNum(ConfigStatusInfoGetGeraeteTyp(Data),
                               Major * 10 + Minor);
}

unsigned long ConfigStatusMrsystemGeraetebeschreibung(char *Buf,
                                                      unsigned int Major,
                                                      unsigned int Minor)
{  ConfigStatusInfo *MrsystemInfo;
   unsigned long Len;

   MrsystemInfo = ConfigStatusInfoCreate();
   if (MrsystemInfo != (ConfigStatusInfo *)NULL)
   {
      ConfigStatusInfoInit(MrsystemInfo);
      ConfigStatusInfoGetMrsystem(MrsystemInfo, Major, Minor);
      Buf[0] = ConfigGeraetTypGetNumMesswerte(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo));
      Buf[1] = ConfigGeraetTypGetNumKonfigwerte(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo));
      SetLongToByteArray((unsigned char *)&(Buf[4]),
                         ConfigGeraetTypGetSerialNum(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)));
      memcpy(&(Buf[8]),
             ConfigGeraetTypGetArtikelNr(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)),
             8);
      strcpy(&(Buf[16]),
             ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)));
      Len = 16 + strlen(ConfigGeraetTypGetGeraeteBezeichnung(ConfigStatusInfoGetGeraeteTyp(MrsystemInfo)));
      ConfigStatusInfoExit(MrsystemInfo);
      ConfigStatusInfoDestroy(MrsystemInfo);
      return(Len);
   }
   else
   {
      return(0);
   }
}

unsigned long ConfigStatusGeraetebeschreibung(ConfigGeraetTyp *GeraetTyp,
                                              char *Buf, unsigned long Len)
{
   ConfigGeraetTypSetNumMesswerte(GeraetTyp, Buf[0]);
   ConfigGeraetTypSetNumKonfigwerte(GeraetTyp, Buf[1]);
   ConfigGeraetTypSetSerialNum(GeraetTyp,
                               GetLongFromByteArray((unsigned char *)&(Buf[4])));
   memcpy(ConfigGeraetTypGetArtikelNr(GeraetTyp), &(Buf[8]), 8);
   ConfigGeraetTypGetArtikelNr(GeraetTyp)[8] = '\0';
   if ((16 + strlen(&(Buf[16]))) < Len)
   {
      strncpy(ConfigGeraetTypGetGeraeteBezeichnung(GeraetTyp), &(Buf[16]),
              MAX_GERAETE_BEZEICHNUNG);
      ConfigGeraetTypGetGeraeteBezeichnung(GeraetTyp)[MAX_GERAETE_BEZEICHNUNG - 1] = '\0';
   }
   else
   {
      ConfigGeraetTypGetGeraeteBezeichnung(GeraetTyp)[0] = '\0';
   }
   return(16 + strlen(ConfigGeraetTypGetGeraeteBezeichnung(GeraetTyp)));
}

unsigned long ConfigStatusMesswerte(ConfigMesswertTyp *ConfigMesswert,
                                    char *Buf, unsigned long Len)
{  char *PBuf;
   unsigned long Index;

   ConfigMesswertTypSetAktuellerMesswert(ConfigMesswert, 0);
   Index = 0;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetAbfragekanalnummer(ConfigMesswert, *PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetMesswertPotenz(ConfigMesswert, *PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetFarbeBereich1(ConfigMesswert, (unsigned int)*PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetFarbeBereich2(ConfigMesswert, (unsigned int)*PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetFarbeBereich3(ConfigMesswert, (unsigned int)*PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetFarbeBereich4(ConfigMesswert, (unsigned int)*PBuf);
   Index++;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetNullpunkt0(ConfigMesswert, GetIntFromByteArray((unsigned char *)PBuf));
   Index += 2;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetEndeBereich1(ConfigMesswert, GetIntFromByteArray((unsigned char *)PBuf));
   Index += 2;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetEndeBereich2(ConfigMesswert, GetIntFromByteArray((unsigned char *)PBuf));
   Index += 2;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetEndeBereich3(ConfigMesswert, GetIntFromByteArray((unsigned char *)PBuf));
   Index += 2;
   PBuf = &(Buf[Index]);
   ConfigMesswertTypSetEndeBereich4(ConfigMesswert, GetIntFromByteArray((unsigned char *)PBuf));
   Index += 2;
   PBuf = &(Buf[Index]);
   if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
   {
      ConfigMesswertTypSetMesswertbezeichnung(ConfigMesswert, strdup(PBuf));
      Index += (unsigned int)strlen(PBuf) + 1;
      PBuf = &(Buf[Index]);
      if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
      {
         ConfigMesswertTypSetBezeichnungStart(ConfigMesswert, strdup(PBuf));
         Index += (unsigned int)strlen(PBuf) + 1;
         PBuf = &(Buf[Index]);
         if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
         {
            ConfigMesswertTypSetBezeichnungEnde(ConfigMesswert, strdup(PBuf));
            Index += (unsigned int)strlen(PBuf) + 1;
            PBuf = &(Buf[Index]);
            if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
            {
               ConfigMesswertTypSetEinheit(ConfigMesswert, strdup(PBuf));
               Index += (unsigned int)strlen(PBuf) + 1;
            }
         }
      }
   }
   return(Index);
}

unsigned long ConfigStatusKonfigwerte(ConfigTyp *Config, char *Buf,
                                      unsigned long Len)
{  char *PBuf;
   unsigned int AuswahlIndex;
   unsigned long Index, Consumed;

   Consumed = 0;
   if (Buf[1] == CS2_KONFIG_KENNER_AUSWAHLLISTE)
   {
      ConfigTypSetTyp(Config, CONFIG_TYP_AUSWAHL);
      ConfigTypSetTypAuswahl(Config, ConfigAuswahlTypCreate());
      if (ConfigTypGetTypAuswahl(Config) != (ConfigAuswahlTyp *)NULL)
      {
         ConfigAuswahlTypInit(ConfigTypGetTypAuswahl(Config));
         Index = 0;
         PBuf = &(Buf[Index]);
         ConfigAuswahlSetKonfigkanalnummer(ConfigTypGetTypAuswahl(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigAuswahlSetKennerAuswahlliste(ConfigTypGetTypAuswahl(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigAuswahlSetAnzahlAuswahlpunkte(ConfigTypGetTypAuswahl(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigAuswahlSetJetzigeEinstellung(ConfigTypGetTypAuswahl(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigAuswahlSetRes(ConfigTypGetTypAuswahl(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
         {
            ConfigAuswahlSetAuswahlbezeichnung(ConfigTypGetTypAuswahl(Config), strdup(PBuf));
            Index += strlen(ConfigAuswahlGetAuswahlbezeichnung(ConfigTypGetTypAuswahl(Config))) + 1;
            PBuf = &(Buf[Index]);
            if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
            {
               ConfigAuswahlSetAuswahl(ConfigTypGetTypAuswahl(Config), malloc(sizeof(char *) * ConfigAuswahlGetAnzahlAuswahlpunkte(ConfigTypGetTypAuswahl(Config))));
               if (ConfigTypGetTypAuswahl(Config)->Auswahl != (char **)NULL)
               {
                  for (AuswahlIndex = 0; AuswahlIndex < ConfigTypGetTypAuswahl(Config)->AnzahlAuswahlpunkte; AuswahlIndex++)
                  {
                     if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
                     {
                        ConfigAuswahlSetAuswahlIndex(ConfigTypGetTypAuswahl(Config), AuswahlIndex, strdup(PBuf));
                        Index += strlen(ConfigAuswahlGetAuswahlIndex(ConfigTypGetTypAuswahl(Config), AuswahlIndex)) + 1;
                        PBuf = &(Buf[Index]);
                     }
                  }
               }
            }
         }
         Consumed = Index - 1;
      }
   }
   else if (Buf[1] == CS2_KONFIG_KENNER_WERT)
   {
      ConfigTypSetTyp(Config, CONFIG_TYP_WERT);
      ConfigTypSetTypWert(Config, ConfigWertTypCreate());
      if (ConfigTypGetTypWert(Config) != (ConfigWertTyp *)NULL)
      {
         ConfigWertTypInit(ConfigTypGetTypWert(Config));
         Index = 0;
         PBuf = &(Buf[Index]);
         ConfigWertSetKonfigurationskanalnummer(ConfigTypGetTypWert(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigWertSetKennerSlider(ConfigTypGetTypWert(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigWertSetUntererWert(ConfigTypGetTypWert(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigWertSetObererWert(ConfigTypGetTypWert(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         ConfigWertSetAktuelleEinstellung(ConfigTypGetTypWert(Config), GetIntFromByteArray((unsigned char *)PBuf));
         Index += 2;
         PBuf = &(Buf[Index]);
         if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
         {
            ConfigWertSetAuswahlbezeichnung(ConfigTypGetTypWert(Config), strdup(PBuf));
            Index += strlen(ConfigWertGetAuswahlbezeichnung(ConfigTypGetTypWert(Config))) + 1;
            PBuf = &(Buf[Index]);
            if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
            {
               ConfigWertSetBezeichnungStart(ConfigTypGetTypWert(Config), strdup(PBuf));
               Index += strlen(ConfigWertGetBezeichnungStart(ConfigTypGetTypWert(Config))) + 1;
               PBuf = &(Buf[Index]);
               if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
               {
                  ConfigWertSetBezeichnungEnde(ConfigTypGetTypWert(Config), strdup(PBuf));
                  Index += strlen(ConfigWertGetBezeichnungEnde(ConfigTypGetTypWert(Config))) + 1;
                  PBuf = &(Buf[Index]);
                  if ((Index + (unsigned int)strlen(PBuf) + 1) < Len)
                  {
                     ConfigWertSetEinheit(ConfigTypGetTypWert(Config), strdup(PBuf));
                     Index += strlen(ConfigWertGetEinheit(ConfigTypGetTypWert(Config))) + 1;
                  }
               }
            }
         }
         Consumed = Index - 1;
      }
   }
   return(Consumed);
}

void ConfigStatusSaveMesswerte(ConfigStatusInfo *ConfigInfo, FILE *MesswertCs2Stream)
{  MengeIterator IterToWrite;
   ConfigMesswertTyp *ConfigMesswert;

   MengeInitIterator(&IterToWrite, ConfigStatusInfoGetMesswerte(ConfigInfo));
   ConfigMesswert = (ConfigMesswertTyp *)MengeFirst(&IterToWrite);
   while (ConfigMesswert != (ConfigMesswertTyp *)NULL)
   {
      Cs2WriteTitleByName(MesswertCs2Stream, "messwerte", 1);
      Cs2WriteIntValueByName(MesswertCs2Stream, "abfragekanalnummer", ConfigMesswertTypGetAbfragekanalnummer(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "messwertpotenz", ConfigMesswertTypGetMesswertPotenz(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "farbebereich1", ConfigMesswertTypGetFarbeBereich1(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "farbebereich2", ConfigMesswertTypGetFarbeBereich2(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "farbebereich3", ConfigMesswertTypGetFarbeBereich3(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "farbebereich4", ConfigMesswertTypGetFarbeBereich4(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "nullpunkt0", ConfigMesswertTypGetNullpunkt0(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "endebereich1", ConfigMesswertTypGetEndeBereich1(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "endebereich2", ConfigMesswertTypGetEndeBereich2(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "endebereich3", ConfigMesswertTypGetEndeBereich3(ConfigMesswert), 2);
      Cs2WriteIntValueByName(MesswertCs2Stream, "endebereich4", ConfigMesswertTypGetEndeBereich4(ConfigMesswert), 2);
      if (ConfigMesswertTypGetMesswertbezeichnung(ConfigMesswert) != (char *)NULL)
         Cs2WriteStringValueByName(MesswertCs2Stream, "messwertbezeichnung", ConfigMesswertTypGetMesswertbezeichnung(ConfigMesswert), 2);
      else
         Cs2WriteStringValueByName(MesswertCs2Stream, "messwertbezeichnung", "", 2);
      if (ConfigMesswertTypGetBezeichnungStart(ConfigMesswert) != (char *)NULL)
         Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungstart", ConfigMesswertTypGetBezeichnungStart(ConfigMesswert), 2);
      else
         Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungstart", "", 2);
      if (ConfigMesswertTypGetBezeichnungEnde(ConfigMesswert) != (char *)NULL)
         Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungende", ConfigMesswertTypGetBezeichnungEnde(ConfigMesswert), 2);
      else
         Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungende", "", 2);
      if (ConfigMesswertTypGetEinheit(ConfigMesswert) != (char *)NULL)
         Cs2WriteStringValueByName(MesswertCs2Stream, "einheit", ConfigMesswertTypGetEinheit(ConfigMesswert), 2);
      else
         Cs2WriteStringValueByName(MesswertCs2Stream, "einheit", "", 2);
      ConfigMesswert = (ConfigMesswertTyp *)MengeNext(&IterToWrite);
   }
}

void ConfigStatusSaveKonfigwerte(ConfigStatusInfo *ConfigInfo, FILE *MesswertCs2Stream)
{  MengeIterator IterToWrite;
   ConfigTyp *Config;
   int i;

   MengeInitIterator(&IterToWrite, ConfigStatusInfoGetTypen(ConfigInfo));
   Config = (ConfigTyp *)MengeFirst(&IterToWrite);
   while (Config != (ConfigTyp *)NULL)
   {
      Cs2WriteTitleByName(MesswertCs2Stream, "konfigwerte", 1);
      if (ConfigTypGetTyp(Config) == CONFIG_TYP_AUSWAHL)
      {
         Cs2WriteTitleByName(MesswertCs2Stream, "auswahlwert", 2);
         Cs2WriteIntValueByName(MesswertCs2Stream, "konfigkanalnummer", ConfigAuswahlGetKonfigkanalnummer(ConfigTypGetTypAuswahl(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "kennerauswahlliste", ConfigAuswahlGetKennerAuswahlliste(ConfigTypGetTypAuswahl(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "anzahlauswahlpunkte", ConfigAuswahlGetAnzahlAuswahlpunkte(ConfigTypGetTypAuswahl(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "jetzigeeinstellung", ConfigAuswahlGetJetzigeEinstellung(ConfigTypGetTypAuswahl(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "res", ConfigAuswahlGetRes(ConfigTypGetTypAuswahl(Config)), 3);
         if (ConfigAuswahlGetAuswahlbezeichnung(ConfigTypGetTypAuswahl(Config)) != (char *)NULL)
            Cs2WriteStringValueByName(MesswertCs2Stream, "auswahlbezeichnung", ConfigAuswahlGetAuswahlbezeichnung(ConfigTypGetTypAuswahl(Config)), 3);
         else
            Cs2WriteStringValueByName(MesswertCs2Stream, "auswahlbezeichnung", "", 3);
         if (ConfigAuswahlGetAuswahl(ConfigTypGetTypAuswahl(Config)) != (char **)NULL)
         {
            for (i = 0; i < ConfigAuswahlGetAnzahlAuswahlpunkte(ConfigTypGetTypAuswahl(Config)); i++)
               if (ConfigAuswahlGetAuswahlIndex(ConfigTypGetTypAuswahl(Config), i) != (char *)NULL)
                  Cs2WriteStringValueByName(MesswertCs2Stream, "", ConfigAuswahlGetAuswahlIndex(ConfigTypGetTypAuswahl(Config), i), 4);
               else
                  Cs2WriteStringValueByName(MesswertCs2Stream, "", "", 4);
         }
      }
      else if (ConfigTypGetTyp(Config) == CONFIG_TYP_WERT)
      {
         Cs2WriteTitleByName(MesswertCs2Stream, "wert", 2);
         Cs2WriteIntValueByName(MesswertCs2Stream, "kanalnummer", ConfigWertGetKonfigurationskanalnummer(ConfigTypGetTypWert(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "slider", ConfigWertGetKennerSlider(ConfigTypGetTypWert(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "untererwert", ConfigWertGetUntererWert(ConfigTypGetTypWert(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "obererwert", ConfigWertGetObererWert(ConfigTypGetTypWert(Config)), 3);
         Cs2WriteIntValueByName(MesswertCs2Stream, "einstellung", ConfigWertGetAktuelleEinstellung(ConfigTypGetTypWert(Config)), 3);
         if (ConfigWertGetAuswahlbezeichnung(ConfigTypGetTypWert(Config)) != (char *)NULL)
            Cs2WriteStringValueByName(MesswertCs2Stream, "auswahlbezeichnung", ConfigWertGetAuswahlbezeichnung(ConfigTypGetTypWert(Config)), 3);
         else
            Cs2WriteStringValueByName(MesswertCs2Stream, "auswahlbezeichnung", "", 3);
         if (ConfigWertGetBezeichnungStart(ConfigTypGetTypWert(Config)) != (char *)NULL)
            Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichungstart", ConfigWertGetBezeichnungStart(ConfigTypGetTypWert(Config)), 3);
         else
            Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichungstart", "", 3);
         if (ConfigWertGetBezeichnungEnde(ConfigTypGetTypWert(Config)) != (char *)NULL)
            Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungende", ConfigWertGetBezeichnungEnde(ConfigTypGetTypWert(Config)), 3);
         else
            Cs2WriteStringValueByName(MesswertCs2Stream, "bezeichnungende", "", 3);
         if (ConfigWertGetEinheit(ConfigTypGetTypWert(Config)) != (char *)NULL)
            Cs2WriteStringValueByName(MesswertCs2Stream, "einheit", ConfigWertGetEinheit(ConfigTypGetTypWert(Config)), 3);
         else
            Cs2WriteStringValueByName(MesswertCs2Stream, "einheit", "", 3);
      }
      Config = (ConfigTyp *)MengeNext(&IterToWrite);
   }
}

static char *BuildUciName(char *Description)
{  static char UciName[80];
   int i, j;

   i = 0;
   j = 0;
   while (Description[i] != '\0')
   {
      if (Description[i] != ' ')
      {
         UciName[j] = Description[i];
         j++;
      }
      i++;
   }
   UciName[j] = '\0';
   return(UciName);
}

void ConfigStatusWebOneMesswerte(ConfigMesswertTyp *MesswertToWrite,
                                 FILE *MesswertUciStream, FILE *ValueUciStream)
{  char IntStr[10], *UciName;
#ifndef WRITE_ALL_INFORMATIONS
   char DescStr[50], HlpStr[80];
#endif

#ifdef WRITE_ALL_INFORMATIONS
   UciWrTxt(MesswertUciStream, ValueUciStream, "Index", 1, 3, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetAbfragekanalnummer(MesswertToWrite));
   UciWrTxt(MesswertCs2Stream, ValueCs2Stream, "Abfragekanalnummer", 1, 4,
            IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetMesswertPotenz(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "MesswertPotenz", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetFarbeBereich1(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "FarbeBereich1", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetFarbeBereich2(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "FarbeBereich2", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetFarbeBereich3(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "FarbeBereich3", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetFarbeBereich4(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "FarbeBereich4", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetNullpunkt0(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "Nullpunkt0", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetEndeBereich1(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "EndeBereich1", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetEndeBereich2(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "EndeBereich2", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetEndeBereich3(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "EndeBereich3", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetEndeBereich4(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "EndeBereich4", 1, 10, IntStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetAktuellerMesswert(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "AktuellerMesswert", 1, 10,
            IntStr);
   UciWrTxt(MesswertUciStream, ValueUciStream, "Messwertbezeichnung", 1, 50,
            ConfigMesswertTypGetMesswertbezeichnung(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "BezeichnungStart", 1, 50,
            ConfigMesswertTypGetBezeichnungStart(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "BezeichnungEnde", 1, 50,
            ConfigMesswertTypGetBezeichnungEnde(MesswertToWrite));
   UciWrTxt(MesswertUciStream, ValueUciStream, "Einheit", 1, 50,
            ConfigMesswertTypGetEinheit(MesswertToWrite));
#else
   sprintf(DescStr, "%d/%d_%s",
           ConfigMesswertTypGetIndex(MesswertToWrite),
           ConfigMesswertTypGetAbfragekanalnummer(MesswertToWrite),
           ConfigMesswertTypGetMesswertbezeichnung(MesswertToWrite));
   UciName = BuildUciName(DescStr);
   sprintf(HlpStr, "Potenz %d, Einheit %s",
           ConfigMesswertTypGetMesswertPotenz(MesswertToWrite),
           ConfigMesswertTypGetEinheit(MesswertToWrite));
   UciWrDefTxt(MesswertUciStream, UciName, 1, 10, HlpStr);
   sprintf(IntStr, "%d", ConfigMesswertTypGetAktuellerMesswert(MesswertToWrite));
   UciWrCfgStr(ValueUciStream, UciName, IntStr);
#endif
}

void ConfigStatusWebDeviceMesswerte(MengeIterator *IterToWrite,
                                    char *FilePath, unsigned long Uid)
{  FILE *MesswertUciStream, *ValueUciStream;
   ConfigMesswertTyp *MesswertToWrite;
   char IntStr[10];

   /* add uci tab config */
   sprintf(IntStr, "%ld", Uid);
   MesswertUciStream = UciOpenDefWr(FilePath, IntStr);
   if (MesswertUciStream != (FILE *)NULL)
   {
      UciWrDefHdr(MesswertUciStream, IntStr);
      ValueUciStream = UciOpenCfgWr(FilePath, IntStr);
      if (ValueUciStream != (FILE *)NULL)
      {
         fchmod(fileno(MesswertUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         fchmod(fileno(ValueUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         UciWrDefHdr(MesswertUciStream, IntStr);
         UciWrTxt(MesswertUciStream, ValueUciStream, "uid", 3, 3, IntStr);
         MesswertToWrite = (ConfigMesswertTyp *)MengeFirst(IterToWrite);
         while (MesswertToWrite != (ConfigMesswertTyp *)NULL)
         {
            ConfigStatusWebOneMesswerte(MesswertToWrite, MesswertUciStream,
                                        ValueUciStream);
            MesswertToWrite = (ConfigMesswertTyp *)MengeNext(IterToWrite);
         }
         UciCloseDef(MesswertUciStream);
      }
      UciCloseCfg(ValueUciStream);
   }
}

void ConfigStatusWebOneKonfig(ConfigTyp *KonfigToWrite,
                              FILE *KonfigUciStream, FILE *ValueUciStream)
{  char IntStr[10];
   ConfigAuswahlTyp *AuswahlToWrite;
   ConfigWertTyp *WertToWrite;
   char *UciName;
#ifndef WRITE_ALL_INFORMATIONS
   char DescStr[50], HlpStr[80];
#endif
   int i;

#ifdef WRITE_ALL_INFORMATIONS
   sprintf(IntStr, "%d", ConfigTypGetIndex(KonfigToWrite));
   UciWrTxt(KonfigUciStream, ValueUciStream, "Index", 1, 3, IntStr);
   switch (ConfigTypGetTyp(KonfigToWrite))
   {
      case CONFIG_TYP_AUSWAHL:
         AuswahlToWrite = ConfigTypGetTypAuswahl(KonfigToWrite);
         sprintf(IntStr, "%d", ConfigAuswahlGetKonfigkanalnummer(AuswahlToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Abfragekanalnummer", 1,
                  4, IntStr);
         sprintf(IntStr, "%d", ConfigAuswahlGetKennerAuswahlliste(AuswahlToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Kenner Auswahlliste", 1,
                  4, IntStr);
         sprintf(IntStr, "%d", ConfigAuswahlGetAnzahlAuswahlpunkte(AuswahlToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Anzahl der Auswahlpunkte",
                  1, 3, IntStr);
         sprintf(IntStr, "%d", ConfigAuswahlGetRes(AuswahlToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Res", 1, 10, IntStr);
         strcpy(IntStr, ConfigAuswahlGetAuswahlIndex(AuswahlToWrite, 0));
         for (i = 1; i < ConfigAuswahlGetAnzahlAuswahlpunkte(AuswahlToWrite); i++)
         {
            strcat(IntStr, "");
            strcat(IntStr, ConfigAuswahlGetAuswahlIndex(AuswahlToWrite, i));
         }
         UciName = BuildUciName(ConfigAuswahlGetAuswahlbezeichnung(AuswahlToWrite));
         UciWrDefRb(KonfigUciStream, UciName, IntStr,
                    ConfigAuswahlGetAuswahlbezeichnung(AuswahlToWrite));
         UciWrCfgInt(ValueUciStream, UciName,
                     ConfigAuswahlGetJetzigeEinstellung(AuswahlToWrite));
         break;
      case CONFIG_TYP_WERT:
         WertToWrite = ConfigTypGetTypWert(KonfigToWrite);
         sprintf(IntStr, "%d", ConfigWertGetKonfigurationskanalnummer(WertToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Abfragekanalnummer", 1,
                  4, IntStr);
         sprintf(IntStr, "%d", ConfigWertGetKennerSlider(WertToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Kenner Slider", 1, 4,
                  IntStr);
         UciWrTxt(KonfigUciStream, ValueUciStream, "Bezeichnung Start", 1,
                  50, ConfigWertGetBezeichnungStart(WertToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Bezeichnung Ende", 1,
                  50, ConfigWertGetBezeichnungEnde(WertToWrite));
         UciWrTxt(KonfigUciStream, ValueUciStream, "Einheit", 1, 50,
                  ConfigWertGetEinheit(WertToWrite));
         UciName = BuildUciName(ConfigWertGetAuswahlbezeichnung(WertToWrite));
         UciWrDefInt(KonfigUciStream, UciName,
                     ConfigWertGetUntererWert(WertToWrite),
                     ConfigWertGetObererWert(WertToWrite),
                     ConfigWertGetAuswahlbezeichnung(WertToWrite));
         UciWrCfgInt(ValueUciStream, UciName,
                     ConfigWertGetAktuelleEinstellung(WertToWrite));
         break;
   }
#else
   switch (ConfigTypGetTyp(KonfigToWrite))
   {
      case CONFIG_TYP_AUSWAHL:
         AuswahlToWrite = ConfigTypGetTypAuswahl(KonfigToWrite);
         sprintf(DescStr, "%d/%d_%d",
                 ConfigTypGetIndex(KonfigToWrite),
                 ConfigAuswahlGetKonfigkanalnummer(AuswahlToWrite),
                 ConfigAuswahlGetRes(AuswahlToWrite));
         for (i = 1; i < ConfigAuswahlGetAnzahlAuswahlpunkte(AuswahlToWrite); i++)
         {
            strcat(IntStr, "");
            strcat(IntStr, ConfigAuswahlGetAuswahlIndex(AuswahlToWrite, i));
         }
         sprintf(HlpStr, "%d", ConfigAuswahlGetRes(AuswahlToWrite));
         UciName = BuildUciName(DescStr);
         UciWrDefRb(KonfigUciStream, UciName, IntStr, HlpStr);
         UciWrCfgInt(ValueUciStream, UciName,
                     ConfigAuswahlGetJetzigeEinstellung(AuswahlToWrite));
         break;
      case CONFIG_TYP_WERT:
         WertToWrite = ConfigTypGetTypWert(KonfigToWrite);
         sprintf(DescStr, "%d/%d_%s",
                 ConfigTypGetIndex(KonfigToWrite),
                 ConfigWertGetKonfigurationskanalnummer(WertToWrite),
                  ConfigWertGetAuswahlbezeichnung(WertToWrite));
         UciName = BuildUciName(DescStr);
         sprintf(HlpStr, "%s", ConfigWertGetEinheit(WertToWrite));
         UciWrDefInt(KonfigUciStream, UciName,
                     ConfigWertGetUntererWert(WertToWrite),
                     ConfigWertGetObererWert(WertToWrite),
                     HlpStr);
         UciWrCfgInt(ValueUciStream, UciName,
                     ConfigWertGetAktuelleEinstellung(WertToWrite));
         break;
   }
#endif
}

void ConfigStatusWebDeviceKonfig(MengeIterator *IterToWrite,
                                 char *FilePath, unsigned long Uid)
{  FILE *KonfigUciStream, *ValueUciStream;
   ConfigTyp *KonfigToWrite;
   char IntStr[10];

   /* add uci tab config */
   sprintf(IntStr, "%ld", Uid);
   KonfigUciStream = UciOpenDefWr(FilePath, IntStr);
   if (KonfigUciStream != (FILE *)NULL)
   {
      UciWrDefHdr(KonfigUciStream, IntStr);
      ValueUciStream = UciOpenCfgWr(FilePath, IntStr);
      if (ValueUciStream != (FILE *)NULL)
      {
         fchmod(fileno(KonfigUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         fchmod(fileno(ValueUciStream),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
         UciWrDefHdr(KonfigUciStream, IntStr);
         UciWrDefTxt(KonfigUciStream, "uid", 3, 3, "uid");
         UciWrCfgStr(ValueUciStream, "uid", IntStr);
         KonfigToWrite = (ConfigTyp *)MengeFirst(IterToWrite);
         while (KonfigToWrite != (ConfigTyp *)NULL)
         {
            ConfigStatusWebOneKonfig(KonfigToWrite, KonfigUciStream,
                                     ValueUciStream);
            KonfigToWrite = (ConfigTyp *)MengeNext(IterToWrite);
         }
         UciCloseDef(KonfigUciStream);
      }
      UciCloseCfg(ValueUciStream);
   }
}
