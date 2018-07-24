Einfacher Gleisboxbooster
=========================

Optionen:
- ohne öffnen der Gleisbox 
- automatishce Gleisspannungsabschaltung (Stopp) nach 250ms
- direkte Anbindung an die Gleisbox (nur zwei Widerstände + Transitor)

BOM
---
Reichelt	https://www.reichelt.de/my/1491379

|Reference	| Value		| Footprint	|
|---------------|---------------|---------------|
|C1,C3,C11	|100nF		|RM2.5		|
|C2		|10uF		|RM2.5		|
|C12		|2,2uF		|MKS 2, RM5	|
|D1		|1N4148		|DO33		|
|D2,D3		|LED 2mA	|5mm		|
|Q1		|BC517		|TO-92		|
|R1,R3,R5,R7	|1k5 Ohm	|0207		|
|R2,R4		|390 Ohm	|0207		|
|R11,R12	|100 kOhm	|0207		|
|U1		|MC7805		|TO-220		|
|U2		|6N137		|DIP-8		|
|U3		|NE555		|DIP-8		|
|Sockel		|DIP-8		|DIP-8		|

BOM (über interne Signale der Gleisbox)
---------------------------------------
|Reference	| Value		| Footprint	|
|---------------|---------------|---------------|
|Q21		|BC517		|TO-92		|
|R21		|1k5 Ohm	|0207		|
|R22		|390 Ohm	|0207		|

