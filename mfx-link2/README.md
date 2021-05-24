mfx-Booster/Link - SMD Version
==============================

Booster incl. mfx-R&uuml;kmeldung zusammen mit einem IBT-2 Modul / SMD Version

Kombination aus [mfx-R&uuml;ckmelder](http://www.persmodelrailroad.net/mfx_boost.html) und [DCC Booster](http://www.trainelectronics.com/DCC_Arduino/DCC_Booster), der auch MM und mfx kann
[!["mfx-BoosterLink"](https://github.com/GBert/misc/raw/master/mfx-link/pictures/mfx-BoosterLink_sketch_s.png)](https://github.com/GBert/misc/raw/master/mfx-link/pictures/mfx-BoosterLink_sketch.png)

Der mfx-Booster/Link bietet zusammen mit dem IBT-2 Modul und einem entsprechenden Netzteil eine kosteng&uuml;nstige Erweiterung f&uuml;r eine leistungsstarke Zentrale (8A).

Bauteile
--------

Geeignete &Uuml;bertrager:
```
Murata 53040C
Pulse Electronics PA1005.040QNL
```

BOM
---

|Part|Value|Footprint|LCSC Part Number|
|-------|----------|---------|----------------|
|C1,C2,C3,C6|100nF|0805|C49678|
|C4,C5|10uf Elko|||
|C7|1nF|0805|C46653|
|D1;D2;D3;D4;D5|US1M|D_SMA|C412437|
|D6;D7;D8|1N4148WSD_SOD-323F|C2128|
|D9||B5819W or BAT43|D_SOD-123|C8598|
|D10|LED red|LED 0805|C84256|
|D13|LED white|LED 0805|C34499|
|Q1|MMBT5551 or BC817|SOT-23|C2145|
|R0,R1|820|0805|C17837|
|R2|390|0805|C17655|
|R3|22|0805|C17521|
|R4,R6,R16,R19|4k7|0805|C17673|
|R5|82|0805|C17841|
|R7|1k|0805|C17513|
|R8,R9,R10,R11,R12,R13,R14|1k|1206|C4410|
|R15|1k5|0805|C4310|
|R17|10k|0805|C17414|
|R18|560|0805|C28636|
|R20,R21|3k3|0805|C26010|
|T1|Murata 53040C|||
|U1|6N137|DIL8||
|U2|7805|TO-252-2 DPAK|C58069|
|U3|LM393|SOIC-8|C7955|
|U4|78L05|SOT-89-3|C71137|


JLCPB SMT Assembly
------------------

|Art      | Datei |
|---------|-------|
|PCB      |gerber/mfx-link2_062021.ZIP|
|BOM      |mfx-link2.csv|
|CPL      |gerber/mfx-link2-top-pos.csv|

Reichelt Listen
---------------

SMD Teile (entspricht JLCPCB Bestückung)
[mfx-booster-link2-smd](https://www.reichelt.de/my/1850634)

Rest (ohne IBT-2 und ohne LCD)
[mfx-booster-link2](https://www.reichelt.de/my/1850613)

Changelog
---------
| Version | Anmerkung |
|---------|-----------|
|062017   |erste Version|
|052021   |überarbeitete Version für SMT Fertigung bei JLCPCB|
|062021   |R17 gegen Masse nicht gegen VCC|
|         |1N4148 Pads vergrössert|
|         |1nF Kondensator hinzugefügt|
|         |R18 vergrössert von 270 auf 560 Ohm wegen LED Stärke|
