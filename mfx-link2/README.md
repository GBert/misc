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
