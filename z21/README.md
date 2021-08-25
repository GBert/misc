Roco WLAN Maus / Gleisbox
=========================

[![Z21_Emu](https://github.com/GBert/railroad/raw/master/z21/pictures/wlm_ms2_married_s.jpg)](https://github.com/GBert/railroad/raw/master/z21/pictures/wlm_ms2_married.jpg)

Mit diesem kleinen Programm kann Rocos WLAN Maus 10813 zusammen mit CS2/CS3 bzw. 'Gleisbox als Zentrale' verwendet werden. Das Programm baut dazu eine Verbindung zum CAN Gateway bzw. can2lan auf. Bei vorhandener Lok-Datei (lokomotive.cs2) entfällt das aufwendige Anlegen der Loks in der WLAN Maus.

Starten mit
```
z21emu -f # Vordegrund zum Test
```

Loks einlesen
----------------

Märklins Lokdaten (lokomotive.cs2) können auf die WLAN Maus übertragen werden:
(Lok Modus) Pfleil oben + Menu - (Lok) OK - (6 Enpfangen) OK -> RECV

Auf derm MS2 über Lok 'Lokliste' (Motorola Adresse 1 F2 ) oder
```
cansend can0 000C0300#000000010201
```
Die Namen werden auf 10 Buchstaben/Ziffern gekürzt


Backup
------

WLAN Maus Senden

```
21:27:28.545 ->UDP    len 0x000f ID 0x0040 e9 f1 00 03 00 01 4c 4f 4b 31 00   
21:27:28.724 ->UDP    len 0x000f ID 0x0040 e9 f1 00 03 00 01 4c 4f 4b 31 00
```

WLAN Maus Empfangen
```
0f 00 -- 40 00 -- e9 f1 -- 00 03 -- 00  -- 04   -- 4c 4f 4b 31       -- 66
10 00 -- 40 00 -- ea f1 -- 00 47 -- 01  -- 04   -- 42 52 31 38 32    -- 72
10 00 -- 40 00 -- ea f1 -- 12 67 -- 02  -- 04   -- 53 43 48 4f 4d    -- 32
11 00 -- 40 00 -- eb f1 -- 00 0a -- 03  -- 04   -- 41 42 43 44 45 46 -- 10
LEN   -- CMD1  -- CMD2  -- ADDR  -- IDX -- ANZ  -- NAME als ASCII    -- XOR
``` 

https://www.open4me.de/index.php/2017/06/zz21-wlan-maus-lok-bibliothek-befehle/#more-848

Todos
-----

- add .prg, .regs, .traktion and .blocks to cs2 reader
- work on turnouts / DCC-MM scheme
- current overload -> Z21
- measurements -> Z21
  

