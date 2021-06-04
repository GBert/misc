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
|----|-----|---------|----------------|
|C1,C2,C3,C6|100nF|0805|C49678|
|C4,C5|10uf Elko|SMD||
|C7|1nF|0805|C46653|
|D1,D2,D3,D4,D5|US1M|SMA|C412437|
|D6,D7,D8|1N4148WS|SOD-323F|C2128|
|D9|B5819W or BAT43|SOD-123|C8598|
|D10|LED red|LED 0805|C84256|
|D13|LED white|LED 0805|C34499|
|Q1|MMBT5551 or BC817|SOT-23|C2145|
|R0,R1|820 Ohm|0805|C17837|
|R2|390 Ohm|0805|C17655|
|R3|22 Ohm|0805|C17561|
|R4,R6,R16,R19|4k7 Ohm|0805|C17673|
|R5|82 Ohm|0805|C17841|
|R7|1k Ohm|0805|C17513|
|R8,R9,R10,R11,R12,R13,R14|1k Ohm|1206|C4410|
|R15|1k5 Ohm|0805|C4310|
|R17,23|10k Ohm|0805|C17414|
|R18|560 Ohm|0805|C28636|
|R20,R21|3k3 Ohm|0805|C26010|
|R22|39k Ohm|0805|C25826|
|SW1|Micro Taster|||
|T1|Murata 53040C|||
|U1|6N137|DIL8||
|U2|7805|TO-252-2 DPAK|C58069|
|U3|LM393|SOIC-8|C7955|
|U4|78L05|SOT-89-3|C71137|
|U5|PIC16F1709|SO-20||
|P1|AKL-101-04|4pin RM5.08||
|P1|AKL-059-02|2pin RM3.5||
|P4,P5|Grove Socket|GRV CONNEC4PIN||

JLCPB SMT Assembly
------------------

|Art      | Datei |
|---------|-------|
|PCB      |gerber/mfx-link2_072021.ZIP|
|BOM      |mfx-link2.csv|
|CPL      |gerber/mfx-link2-top-pos.csv|

Reichelt Listen
---------------

SMD Teile (entspricht JLCPCB Bestückung)
[mfx-booster-link2-smd](https://www.reichelt.de/my/1850634)

Rest (ohne IBT-2 und ohne LCD)
[mfx-booster-link2](https://www.reichelt.de/my/1850613)

IBT-Mod
-------

[!["IBT-2 Mod"](https://raw.githubusercontent.com/GBert/misc/master/mfx-link2/pictures/mfx-link-current-sense_s.jpg)](https://raw.githubusercontent.com/GBert/misc/master/mfx-link2/pictures/mfx-link-current-sense.jpg)

Beschaltung
-----------

[!["IBT-2 Mod"](https://raw.githubusercontent.com/GBert/misc/master/mfx-link2/pictures/mfxlb_wire_s.jpg)](https://raw.githubusercontent.com/GBert/misc/master/mfx-link2/pictures/mfxlb_wire.jpg)

Changelog
---------
| Version | Anmerkung |
|---------|-----------|
|062017   |erste Version|
|052021   |überarbeitete Version für SMT Fertigung bei JLCPCB|
|062021   |R17 gegen Masse nicht gegen VCC|
|         |C7 1nF Kondensator hinzugefügt|
|         |1N4148 Pads vergrössert|
|         |R18 vergrössert von 270 auf 560 Ohm wegen LED Stärke|
|072021   |Spannungsteilier R22/23 zur Messung der Versorgungsspannung|
