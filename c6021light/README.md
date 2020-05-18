connect6021 für Keyboard 6040
=============================

Belegung der Buchse (Keyboard 6040 links)
```
SDA  SCL                      8V   8V
 2a   4b   6b   8b  10b  12b  14b  16b
 2a   4a   6a   8a  10a  12a  14a  16a
GND  GND  GND  GND  GND  GND  GND  GND
```

```
Für die Übertragung von Weichenstellbefehlen genügen drei Byte als Schaltanforderung.
Es sind dies die Bytes für Empfänger (Zentrale), Keyboard-Nr. (0 bis 15) und das Datenbyte.
Das Datenbyte enthält die Angabe der Decoder-Ausgangsnummer (Bits 0 bis 3) und der Decoder- Teiladresse (Bits 4 und 5).
Mit Bit 0 wird die Schaltrichtung bestimmt (0=Rot, 1=Grün), mit Bit 3 wird der Schaltstrom eingeschaltet.
Da mit einem Keyboard (6040) vier Decoder angesprochen werden können, bestimmen die Bits 4 und 5, um welche Vierergruppe von Tastenpaaren und somit für welchen der vier für dieses Keyboard zuständigen Decoder sich der Schaltbefehl handelt.
Bei der Bildung der realen Decoder-Adresse (im binären Zahlenformat) stellen diese zwei Bits die Bits 0 und 1 dar. Die vier Bits der Keyboard-Adresse bilden dann die Bits 2 bis 5 der Decoder-Adresse. Diese Decoder-Adresse wird noch um 1 erhöht, da die Decoder-Adressierung im Märklin Digital-System immer mit der Adresse "01" beginnt. Somit können maximal 64 Decoder im Bereich von "01" bis "64" angesprochen werden.
Aufbau des Datenbytes:

    Bit 0 = Schaltrichtung, 0=Rot, 1=Grün
    Bit 1 = Decoderausgang LSB (Least Significant Bit)
    Bit 2 = Decoderausgang MSB (Most Significant Bit)
    Bit 3 = Schaltstrom, 0=aus, 1=ein
    Bit 4 = Decoder-Teiladresse LSB
    Bit 5 = Decoder-Teiladresse MSB 

Aufbau der Schaltanforderung:

Empfänger		Sender			Datenbyte		Stoppbits
1111 1110	0	001X XXX0	0	00XX XXXX	0	10
Zentrale	Q	Geräte-Adr.	Q			Q

Die Bestätigung von der Zentrale lautet:

Empfänger		Sender			Datenbyte		Stoppbit
001X XXX0	0	1111 1110	0	00XX XXXX	0	0
Geräte-Adr.	Q	Zentrale	Q			Q

```

Sequenz

Taste 0 rot

0xfe 0x20 0x08

0x20 0xfe 0x08 (Osci wA 0x10 -> Data 0x20)
