#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include "uci.h"

/** @file */

/**
* @brief &Ouml;ffnen einer UCI cfg Datei
*
* Diese Funktion wird aufgerufen, wenn auf einem Socket Daten gelesen werden
* k&ouml;nnen. Die Funktion hat folgende Parameter:
*
* @param[in] Pfad Verzeichnis, in dem die UCI Dateien liegen.
* @param[in] BaseName Name der UCI Datei ohne Extension. Die .uci Datei
*            bekommt den Basisnamen, der auch in dem Link (Formular) auf
*            dieses Verzeichnis eingetragen ist. Die .def und zugeh&ouml;rige
*            .cfg Datei bekommt den Basisnamen, der auch in die .uci Datei als
*            Konfigseite eingetragen ist.
*
* @return Pointer auf Filestruktur oder NULL im Fehlerfall.
*/
FILE *UciOpenUciWr(char *Pfad, char *BaseName)
{
   return(UciOpenWr(Pfad, BaseName, UCI_UCI_EXTENSION));
}
